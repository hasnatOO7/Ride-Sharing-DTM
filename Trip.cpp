#include "Trip.h"
#include "Driver.h"
#include "Rider.h"
#include "City.h"
#include <iostream>
#include <iomanip>
#include <cmath>

<<<<<<< Updated upstream
<<<<<<< Updated upstream
<<<<<<< Updated upstream
Trip::Trip(int id, Rider *rider, const std::string &pickup, const std::string &dropoff)
    : id(id), rider(rider), driver(nullptr), pickup(pickup), dropoff(dropoff), status(REQUESTED), fare(0.0), city(nullptr) {}
=======
=======
>>>>>>> Stashed changes
=======
>>>>>>> Stashed changes
Trip::Trip(int id, Rider *rider, const std::string &pickup, const std::string &dropoff, int driverId) : id(id), rider(rider), driver(nullptr), pickup(pickup), dropoff(dropoff), status(REQUESTED){} : rider(rider), driverId(driverId), driver(nullptr)
    : id(id), rider(rider), driver(nullptr), pickup(pickup), dropoff(dropoff), status(REQUESTED) {}
>>>>>>> Stashed changes

int Trip::getId() const
{
    return id;
}

Rider *Trip::getRider() const
{
    return rider;
}

Driver *Trip::getDriver() const
{
    return driver;
}

const std::string &Trip::getPickup() const
{
    return pickup;
}

const std::string &Trip::getDropoff() const
{
    return dropoff;
}

TripState Trip::getStatus() const
{
    return status;
}

double Trip::getFare() const
{
    return fare;
}

std::string Trip::getStateString() const
{
    switch (status)
    {
    case REQUESTED:
        return "REQUESTED";
    case ASSIGNED:
        return "ASSIGNED";
    case ONGOING:
        return "ONGOING";
    case COMPLETED:
        return "COMPLETED";
    case CANCELLED:
        return "CANCELLED";
    default:
        return "UNKNOWN";
    }
}

void Trip::setDriver(Driver *newDriver)
{
    driver = newDriver;
}

void Trip::setStatus(TripState newStatus)
{
    status = newStatus;
}

void Trip::setCity(City *cityRef)
{
    city = cityRef;
}

double Trip::calculateFare()
{
    if (!city || !driver)
    {
        fare = 0.0;
        return fare;
    }

    // Get distance between pickup and dropoff
    int distance = city->getShortestDistance(pickup, dropoff);
    if (distance <= 0)
    {
        distance = 10; // Default fallback distance in km
    }

    // Convert distance to km (if stored as meters, divide by 1000)
    double distanceKm = distance / 1.0; // Assuming distance is already in km

    // Base fare: 150 PKR
    double baseFare = 150.0;

    // Distance charge: 30 PKR per km
    double distanceCharge = distanceKm * 30.0;

    // Calculate subtotal
    double subtotal = baseFare + distanceCharge;

    // Check if vehicle is a Bike (reduce fare by 40% for bikes)
    std::string carModel = driver->getCarModel();
    if (carModel.find("Bike") != std::string::npos || carModel.find("bike") != std::string::npos)
    {
        subtotal = subtotal * 0.6; // 60% of original (40% reduction)
    }

    fare = subtotal;
    return fare;
}

void Trip::generateReceipt() const
{
    if (!rider || !driver)
    {
        return;
    }

    std::cout << "\n";
    std::cout << "╔════════════════════════════════════════╗\n";
    std::cout << "║       RIDE COMPLETE - RECEIPT          ║\n";
    std::cout << "╚════════════════════════════════════════╝\n";
    std::cout << "\n";
    std::cout << "Passenger:  " << rider->getName() << "\n";
    std::cout << "Driver:     " << driver->getName() << " (" << driver->getCarModel() << ")\n";
    std::cout << "Number Plate: " << driver->getNumberPlate() << "\n";
    std::cout << "\n";
    std::cout << "Route: " << pickup << " → " << dropoff << "\n";

    // Calculate and display distance
    if (city)
    {
        int distance = city->getShortestDistance(pickup, dropoff);
        std::cout << "Distance: " << distance << " km\n";
    }

    std::cout << "\n";
    std::cout << "────────────────────────────────────────\n";
    std::cout << "TOTAL FARE: PKR " << std::fixed << std::setprecision(2) << fare << "\n";
    std::cout << "────────────────────────────────────────\n";
    std::cout << "\n";
    std::cout << "Thank you for using our ride-sharing service!\n";
    std::cout << "\n";
}
