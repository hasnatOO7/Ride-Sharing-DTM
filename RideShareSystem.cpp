#include "RideShareSystem.h"
#include "Driver.h"
#include "Rider.h"
#include "Trip.h"
#include <iostream>
#include <cstdio>

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

Rider *RideShareSystem::addRider(const std::string &name, const std::string &location)
{
    Rider *newRider = new Rider(nextRiderId++, name, location);
    riders.push_back(newRider);
    return newRider;
}

Trip *RideShareSystem::requestTrip(Rider *rider, const std::string &pickup, const std::string &dropoff)
{
    if (!rider)
    {
        std::cerr << "Error: Rider is null\n";
        return nullptr;
    }

    // Create the trip
    Trip *newTrip = new Trip(nextTripId++, rider, pickup, dropoff);
    trips.push_back(newTrip);

    // Record initial state
    rollbackManager.recordState(newTrip);

    // Find and assign nearest available driver
    Driver *assignedDriver = dispatchEngine.findNearestDriver(city, drivers, pickup);

    if (assignedDriver)
    {
        newTrip->setDriver(assignedDriver);
        newTrip->setStatus(ASSIGNED);
        assignedDriver->setAvailable(false);
        assignedDriver->setCurrentLocation(pickup);

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

    trip->setStatus(COMPLETED);

    Driver *driver = trip->getDriver();
    if (driver)
    {
        driver->setAvailable(true);
        driver->setCurrentLocation(trip->getDropoff());
    }

    std::cout << "[OK] Trip #" << trip->getId() << " completed.\n";
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

    // Add some basic roads between major cities
    addRoad("Kalma Chowk", "Blue Area", 300);
    addRoad("Clifton", "Defence", 8);
    addRoad("Liberty", "Saddar", 250);

    std::cout << "[OK] 8 Pakistani cities initialized with 18 locations\n";
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
    std::cout << "\n========== AVAILABLE DRIVERS ==========\n";
    int driverNum = 1;
    bool found = false;

    for (const Driver *driver : drivers)
    {
        if (driver->getIsAvailable())
        {
            printf(" %d. %s | %s (%s) | %s\n",
                   driverNum++,
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
