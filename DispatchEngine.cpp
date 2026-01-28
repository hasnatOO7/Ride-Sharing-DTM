#include "DispatchEngine.h"
#include "City.h"
#include "Driver.h"
#include <climits>
#include <iostream>

DispatchEngine::DispatchEngine() {}

Driver *DispatchEngine::findNearestDriver(City &city, std::vector<Driver *> &drivers, const std::string &pickupLocation)
{
    Driver *bestDriver = nullptr;
    int bestCost = INT_MAX;

    int pickupZone = city.getZoneByName(pickupLocation);
    if (pickupZone == -1)
    {
        std::cerr << "Error: Pickup location not found in city\n";
        return nullptr;
    }

    for (Driver *driver : drivers)
    {
        if (!driver->getIsAvailable())
        {
            continue;
        }

        int cost = calculateCost(city, driver, pickupLocation);
        if (cost < bestCost)
        {
            bestCost = cost;
            bestDriver = driver;
        }
    }

    return bestDriver;
}

int DispatchEngine::calculateCost(City &city, Driver *driver, const std::string &pickupLocation)
{
    int distance = city.getShortestDistance(driver->getCurrentLocation(), pickupLocation);
    if (distance == -1)
    {
        distance = INT_MAX / 2; // Unreachable location, but not maximum to avoid overflow
    }

    int pickupZone = city.getZoneByName(pickupLocation);
    int penalty = 0;

    // Add penalty if driver is in different zone
    if (driver->getZoneID() != pickupZone)
    {
        penalty = 50;
    }

    return distance + penalty;
}
