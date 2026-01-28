#include "RideShareSystem.h"
#include "Driver.h"
#include "Rider.h"
#include "Trip.h"
#include <iostream>
#include <cstdio>
#include <climits>

RideShareSystem::RideShareSystem() : nextDriverId(0), nextRiderId(0), nextTripId(0) {}

RideShareSystem::~RideShareSystem()
{
    // Clean up drivers
    for (Driver *driver : drivers)
    {
        delete driver;
    }
    drivers.clear();

    // Clean up riders
    for (Rider *rider : riders)
    {
        delete rider;
    }
    riders.clear();

    // Clean up trips
    for (Trip *trip : trips)
    {
        delete trip;
    }
    trips.clear();
}

void RideShareSystem::addLocation(const std::string &name, int zoneID)
{
    city.addLocation(name, zoneID);
}

void RideShareSystem::addRoad(const std::string &locationA, const std::string &locationB, int distance)
{
    city.addRoad(locationA, locationB, distance);
}

Driver *RideShareSystem::addDriver(const std::string &name, const std::string &carModel,
                                   const std::string &numberPlate, const std::string &location, int zoneID)
{
    Driver *newDriver = new Driver(nextDriverId++, name, carModel, numberPlate, location, zoneID);
    drivers.push_back(newDriver);
    return newDriver;
}

Driver *RideShareSystem::addDriver(const std::string &name, const std::string &location, int zoneID)
{
    // Create a driver with default car model and number plate
    Driver *newDriver = new Driver(nextDriverId++, name, "Unknown", "Unknown", location, zoneID);
    drivers.push_back(newDriver);
    return newDriver;
}

Rider *RideShareSystem::addRider(const std::string &name, const std::string &location)
{
    Rider *newRider = new Rider(nextRiderId++, name, location);
    riders.push_back(newRider);
    return newRider;
}

Trip *RideShareSystem::requestTrip(Rider *rider, const std::string &pickup, const std::string &dropoff, Driver *manualDriver)
{
    if (!rider)
    {
        std::cerr << "Error: Rider is null\n";
        return nullptr;
    }

    // Create the trip
    Trip *newTrip = new Trip(nextTripId++, rider, pickup, dropoff, -1);
    newTrip->setCity(&city); // Set city reference for distance calculation
    trips.push_back(newTrip);

    // Record initial state
    rollbackManager.recordState(newTrip);

    // Assign driver: if a manualDriver was provided, use it and skip search.
    Driver *assignedDriver = nullptr;
    if (manualDriver)
    {
        assignedDriver = manualDriver;
        std::cout << "[INFO] Manual override requested. Assigning selected driver.\n";
    }
    else
    {
        // Find and assign nearest available driver
        assignedDriver = dispatchEngine.findNearestDriver(city, drivers, pickup);
    }

    if (assignedDriver)
    {
        newTrip->setDriver(assignedDriver);
        newTrip->setStatus(ASSIGNED);
        assignedDriver->setAvailable(false);
        assignedDriver->setCurrentLocation(pickup);

        if (manualDriver)
        {
            std::cout << "Manual Override: Driver " << assignedDriver->getName() << " assigned successfully." << std::endl;
        }

        std::cout << "\n[OK] TRIP BOOKED\n";
        std::cout << "    Trip ID: " << newTrip->getId() << "\n";
        std::cout << "    Driver: " << assignedDriver->getName() << " (" << assignedDriver->getCarModel() << ")\n";
        std::cout << "    Number Plate: " << assignedDriver->getNumberPlate() << "\n";
        std::cout << "    Route: " << pickup << " -> " << dropoff << "\n\n";
    }
    else
    {
        std::cout << "\n[ERROR] No available drivers at the moment!\n\n";
    }

    return newTrip;
}

bool RideShareSystem::completeTrip(Trip *trip)
{
    if (!trip)
    {
        std::cerr << "Error: Trip is null\n";
        return false;
    }

    // Record state before change
    rollbackManager.recordState(trip);

    // Calculate fare before marking as completed
    trip->calculateFare();

    trip->setStatus(COMPLETED);

    Driver *driver = trip->getDriver();
    if (driver)
    {
        driver->setAvailable(true);
        driver->setCurrentLocation(trip->getDropoff());
    }

    std::cout << "[OK] Trip #" << trip->getId() << " completed.\n";

    // Generate and display receipt
    trip->generateReceipt();

    return true;
}

bool RideShareSystem::cancelTrip(Trip *trip)
{
    if (!trip)
    {
        std::cerr << "Error: Trip is null\n";
        return false;
    }

    // Record state before change
    rollbackManager.recordState(trip);

    trip->setStatus(CANCELLED);

    Driver *driver = trip->getDriver();
    if (driver)
    {
        driver->setAvailable(true);
    }

    std::cout << "[OK] Trip #" << trip->getId() << " cancelled.\n";
    return true;
}

bool RideShareSystem::undoLastAction()
{
    bool success = rollbackManager.undoLastOperation();
    if (success)
    {
        std::cout << "[OK] Undo completed successfully\n";
    }
    else
    {
        std::cout << "[ERROR] Nothing to undo\n";
    }
    return success;
}

const std::vector<Trip *> &RideShareSystem::getAllTrips() const
{
    return trips;
}

const std::vector<Driver *> &RideShareSystem::getAllDrivers() const
{
    return drivers;
}

const std::vector<Rider *> &RideShareSystem::getAllRiders() const
{
    return riders;
}

const std::vector<CityInfo> &RideShareSystem::getAllCities() const
{
    return cities;
}

void RideShareSystem::setupPakistaniCities()
{
    std::cout << "\n[SETUP] Initializing Pakistani Cities...\n";

    // Define 8 Pakistani cities with their locations
    CityInfo lahore;
    lahore.cityId = 1;
    lahore.cityName = "Lahore";
    lahore.locations = {"Kalma Chowk", "Liberty", "DHA"};
    cities.push_back(lahore);
    addLocation("Kalma Chowk", 1);
    addLocation("Liberty", 1);
    addLocation("DHA", 1);

    CityInfo islamabad;
    islamabad.cityId = 2;
    islamabad.cityName = "Islamabad";
    islamabad.locations = {"Blue Area", "F-10"};
    cities.push_back(islamabad);
    addLocation("Blue Area", 2);
    addLocation("F-10", 2);

    CityInfo karachi;
    karachi.cityId = 3;
    karachi.cityName = "Karachi";
    karachi.locations = {"Clifton", "Defence", "Saddar"};
    cities.push_back(karachi);
    addLocation("Clifton", 3);
    addLocation("Defence", 3);
    addLocation("Saddar", 3);

    CityInfo rawalpindi;
    rawalpindi.cityId = 4;
    rawalpindi.cityName = "Rawalpindi";
    rawalpindi.locations = {"Raja Bazaar", "Saddar"};
    cities.push_back(rawalpindi);
    addLocation("Raja Bazaar", 4);
    addLocation("Saddar", 4);

    CityInfo faisalabad;
    faisalabad.cityId = 5;
    faisalabad.cityName = "Faisalabad";
    faisalabad.locations = {"Clock Tower", "Iqbal Park"};
    cities.push_back(faisalabad);
    addLocation("Clock Tower", 5);
    addLocation("Iqbal Park", 5);

    CityInfo multan;
    multan.cityId = 6;
    multan.cityName = "Multan";
    multan.locations = {"Chowk Bazaar", "Zargarha"};
    cities.push_back(multan);
    addLocation("Chowk Bazaar", 6);
    addLocation("Zargarha", 6);

    CityInfo hafizabad;
    hafizabad.cityId = 7;
    hafizabad.cityName = "Hafizabad";
    hafizabad.locations = {"City Center", "Bypass"};
    cities.push_back(hafizabad);
    addLocation("City Center", 7);
    addLocation("Bypass", 7);

    CityInfo okara;
    okara.cityId = 8;
    okara.cityName = "Okara";
    okara.locations = {"Main Bazaar", "Industrial Area"};
    cities.push_back(okara);
    addLocation("Main Bazaar", 8);
    addLocation("Industrial Area", 8);

    // Add comprehensive roads connecting all 8 cities
    // Within Zone 1 (Lahore)
    addRoad("Kalma Chowk", "Liberty", 8);
    addRoad("Liberty", "DHA", 12);
    addRoad("Kalma Chowk", "DHA", 15);

    // Within Zone 2 (Islamabad)
    addRoad("Blue Area", "F-10", 5);

    // Within Zone 3 (Karachi)
    addRoad("Clifton", "Defence", 8);
    addRoad("Defence", "Saddar", 10);
    addRoad("Clifton", "Saddar", 15);

    // Within Zone 4 (Rawalpindi)
    addRoad("Raja Bazaar", "Saddar", 6);

    // Within Zone 5 (Faisalabad)
    addRoad("Clock Tower", "Iqbal Park", 8);

    // Within Zone 6 (Multan)
    addRoad("Chowk Bazaar", "Zargarha", 6);

    // Within Zone 7 (Hafizabad)
    addRoad("City Center", "Bypass", 5);

    // Within Zone 8 (Okara)
    addRoad("Main Bazaar", "Industrial Area", 8);

    // Inter-city connections (creating a connected network)
    // Lahore to Islamabad
    addRoad("Kalma Chowk", "Blue Area", 300);
    addRoad("Liberty", "F-10", 310);

    // Islamabad to Rawalpindi
    addRoad("Blue Area", "Raja Bazaar", 50);

    // Lahore to Rawalpindi
    addRoad("DHA", "Raja Bazaar", 280);

    // Rawalpindi to Faisalabad
    addRoad("Saddar", "Clock Tower", 220);

    // Lahore to Karachi (long distance)
    addRoad("Kalma Chowk", "Clifton", 1200);

    // Karachi to Multan
    addRoad("Saddar", "Chowk Bazaar", 900);

    // Multan to Faisalabad
    addRoad("Chowk Bazaar", "Clock Tower", 250);

    // Faisalabad to Hafizabad
    addRoad("Clock Tower", "City Center", 180);

    // Hafizabad to Okara
    addRoad("City Center", "Main Bazaar", 120);

    // Okara to Multan
    addRoad("Main Bazaar", "Zargarha", 200);

    // Additional connectivity paths
    addRoad("Liberty", "Saddar", 250);

    std::cout << "[OK] 8 Pakistani cities initialized with 18 locations and comprehensive road network\n";
}

void RideShareSystem::displayCities() const
{
    std::cout << "\n========== CITIES ==========\n";
    for (const CityInfo &c : cities)
    {
        printf(" %d. %-15s\n", c.cityId, c.cityName.c_str());
    }
    std::cout << "============================\n";
}

void RideShareSystem::displayLocationsByCity(int cityId) const
{
    for (const CityInfo &c : cities)
    {
        if (c.cityId == cityId)
        {
            std::cout << "\n===== LOCATIONS IN " << c.cityName << " =====\n";
            for (size_t i = 0; i < c.locations.size(); ++i)
            {
                printf(" %d. %s\n", (int)(i + 1), c.locations[i].c_str());
            }
            std::cout << "================================\n";
            return;
        }
    }
    std::cout << "[ERROR] City not found.\n";
}

void RideShareSystem::displayAvailableDrivers() const
{
    std::cout << "\n========== AVAILABLE DRIVERS ==========" << std::endl;
    bool found = false;
    for (size_t i = 0; i < drivers.size(); ++i)
    {
        const Driver *driver = drivers[i];
        if (driver->getIsAvailable())
        {
            printf(" %d. Driver %s | %s (%s) | %s\n",
                   (int)(i + 1),
                   driver->getName().c_str(),
                   driver->getCarModel().c_str(),
                   driver->getNumberPlate().c_str(),
                   driver->getCurrentLocation().c_str());
            found = true;
        }
    }

    if (!found)
    {
        std::cout << " [INFO] No drivers available right now.\n";
    }
    std::cout << "======================================\n";
}

void RideShareSystem::displayAvailableDriversWithETA(const std::string &pickupLocation) const
{
    std::cout << "\n========== AVAILABLE DRIVERS ==========" << std::endl;
    bool found = false;
    for (size_t i = 0; i < drivers.size(); ++i)
    {
        const Driver *driver = drivers[i];
        if (driver->getIsAvailable())
        {
            // Calculate distance from driver's current location to pickup location
            int distance = city.getShortestDistance(driver->getCurrentLocation(), pickupLocation);

            // If path not found, skip this driver or mark as unavailable
            if (distance == INT_MAX || distance < 0)
            {
                printf(" %d. Driver %s (Unavailable) | %s (%s) | %s\n",
                       (int)(i + 1),
                       driver->getName().c_str(),
                       driver->getCarModel().c_str(),
                       driver->getNumberPlate().c_str(),
                       driver->getCurrentLocation().c_str());
            }
            else
            {
                // Calculate ETA: distance in km / 40 km/h * 60 minutes
                int etaMinutes = (distance / 40.0) * 60;
                printf(" %d. Driver %s (%d mins away) | %s (%s) | %s\n",
                       (int)(i + 1),
                       driver->getName().c_str(),
                       etaMinutes,
                       driver->getCarModel().c_str(),
                       driver->getNumberPlate().c_str(),
                       driver->getCurrentLocation().c_str());
            }
            found = true;
        }
    }

    if (!found)
    {
        std::cout << " [INFO] No drivers available right now.\n";
    }
    std::cout << "======================================\n";
}

Driver *RideShareSystem::getDriverById(int driverId) const
{
    int count = 0;
    for (Driver *driver : drivers)
    {
        if (driver->getIsAvailable())
        {
            count++;
            if (count == driverId)
            {
                return driver;
            }
        }
    }
    return nullptr;
}

std::string RideShareSystem::getLocationById(int cityId, int locationId) const
{
    for (const CityInfo &c : cities)
    {
        if (c.cityId == cityId && locationId > 0 && locationId <= (int)c.locations.size())
        {
            return c.locations[locationId - 1];
        }
    }
    return "";
}
