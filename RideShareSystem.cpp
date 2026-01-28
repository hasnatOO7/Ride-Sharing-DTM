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

Driver *RideShareSystem::addDriver(const std::string &name, const std::string &location, int zoneID)
{
    Driver *newDriver = new Driver(nextDriverId++, name, location, zoneID);
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

    // Check if rider already has an active trip
    Trip *existingTrip = findRiderActiveTrip(rider);
    if (existingTrip)
    {
        std::cout << "[ERROR] Rider '" << rider->getName() << "' is already on a trip!\n";
        std::cout << "        Cannot book another ride. Status: " << existingTrip->getStateString() << "\n";
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

        std::cout << "[OK] Trip #" << newTrip->getId() << " booked for " << rider->getName()
                  << "\n     Assigned to: " << assignedDriver->getName() << "\n";
    }
    else
    {
        std::cout << "[WARNING] Trip #" << newTrip->getId() << " requested but no drivers available\n";
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

    std::cout << "Trip #" << trip->getId() << " completed. Status: " << trip->getStateString() << "\n";
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

    std::cout << "Trip #" << trip->getId() << " cancelled. Status: " << trip->getStateString() << "\n";
    return true;
}

bool RideShareSystem::undoLastAction()
{
    bool success = rollbackManager.undoLastOperation();
    if (success)
    {
        std::cout << "Undo operation completed successfully\n";
    }
    else
    {
        std::cout << "Undo operation failed - no actions to undo\n";
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

void RideShareSystem::displayAllLocations() const
{
    std::cout << "\n--- Available Locations ---\n";
    std::cout << "ZONE 1: CityCenter, Mall, RailwayStation\n";
    std::cout << "ZONE 2: TechPark, University\n";
    std::cout << "ZONE 3: Airport, Hospital, Suburb\n";
}

void RideShareSystem::displayAllRiders() const
{
    if (riders.empty())
    {
        std::cout << "[INFO] No riders registered yet.\n";
        return;
    }

    std::cout << "\n--- Registered Riders ---\n";
    std::cout << "ID | Name             | Current Location\n";
    std::cout << "---+------------------+------------------\n";
    for (const Rider *rider : riders)
    {
        printf("%2d | %-16s | %s\n", rider->getId(), rider->getName().c_str(), rider->getLocation().c_str());
    }
    std::cout << "\n";
}

Trip *RideShareSystem::findRiderActiveTrip(Rider *rider) const
{
    if (!rider)
        return nullptr;

    for (Trip *trip : trips)
    {
        if (trip->getRider() == rider)
        {
            TripState status = trip->getStatus();
            if (status == ASSIGNED || status == ONGOING)
            {
                return trip;
            }
        }
    }
    return nullptr;
}

void RideShareSystem::displayDriverStats() const
{
    if (drivers.empty())
    {
        std::cout << "[INFO] No drivers registered.\n";
        return;
    }

    std::cout << "\n--- Driver Statistics ---\n";
    std::cout << "Name           | Location        | Status | Completed\n";
    std::cout << "---------------+-----------------+--------+----------\n";

    for (const Driver *driver : drivers)
    {
        // Count completed trips for this driver
        int completedTrips = 0;
        for (const Trip *trip : trips)
        {
            if (trip->getDriver() == driver && trip->getStatus() == COMPLETED)
            {
                completedTrips++;
            }
        }

        std::string status = driver->getIsAvailable() ? "Free" : "Busy";
        printf("%-14s | %-15s | %-6s | %d\n",
               driver->getName().c_str(),
               driver->getCurrentLocation().c_str(),
               status.c_str(),
               completedTrips);
    }
    std::cout << "\n";
}
