#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include "RideShareSystem.h"
#include "Driver.h"
#include "Rider.h"
#include "Trip.h"

// Global system reference for interactive functions
RideShareSystem *g_system = nullptr;
std::vector<Rider *> g_riders;

// ===========================
// Setup Function
// ===========================
void setupSystem(RideShareSystem &system)
{
    std::cout << "\n=== Initializing Ride-Sharing System ===\n";
    std::cout << "Setting up city locations...\n";

    // Step 1: Add 8 locations with 3 zones
    // Zone 1: City Center, Downtown areas
    system.addLocation("CityCenter", 1);
    system.addLocation("Mall", 1);
    system.addLocation("RailwayStation", 1);

    // Zone 2: Tech and Business areas
    system.addLocation("TechPark", 2);
    system.addLocation("University", 2);

    // Zone 3: Periphery areas
    system.addLocation("Airport", 3);
    system.addLocation("Hospital", 3);
    system.addLocation("Suburb", 3);

    std::cout << "[OK] Added 8 locations across 3 zones\n";

    // Step 2: Add roads connecting locations logically
    std::cout << "Setting up road network...\n";

    // Zone 1 internal connections
    system.addRoad("CityCenter", "Mall", 8);
    system.addRoad("CityCenter", "RailwayStation", 6);
    system.addRoad("Mall", "RailwayStation", 10);

    // Zone 2 internal connections
    system.addRoad("TechPark", "University", 12);

    // Zone 3 internal connections
    system.addRoad("Airport", "Hospital", 18);
    system.addRoad("Hospital", "Suburb", 15);
    system.addRoad("Airport", "Suburb", 22);

    // Inter-zone connections
    system.addRoad("CityCenter", "TechPark", 20);
    system.addRoad("CityCenter", "Airport", 25);
    system.addRoad("RailwayStation", "Airport", 28);
    system.addRoad("Mall", "University", 5);
    system.addRoad("TechPark", "Airport", 30);
    system.addRoad("University", "Suburb", 35);
    system.addRoad("RailwayStation", "Hospital", 32);

    std::cout << "[OK] Road network established with 15 connections\n";

    // Step 3: Add 5 drivers at different locations
    std::cout << "Registering drivers...\n";
    system.addDriver("Driver Ali", "Airport", 3);
    system.addDriver("Driver Bob", "Mall", 1);
    system.addDriver("Driver Charlie", "TechPark", 2);
    system.addDriver("Driver Diana", "CityCenter", 1);
    system.addDriver("Driver Eva", "Hospital", 3);

    std::cout << "[OK] 5 drivers registered\n";
    std::cout << "\n=== System Initialization Complete ===\n";
    std::cout << "Ready to accept ride requests!\n\n";
}

// ===========================
// Validation Functions
// ===========================
bool isValidLocation(const std::string &location)
{
    // Check against all valid locations (Zone 1, 2, 3)
    const std::vector<std::string> validLocations = {
        "CityCenter", "Mall", "RailwayStation",
        "TechPark", "University",
        "Airport", "Hospital", "Suburb"};

    for (const std::string &loc : validLocations)
    {
        if (loc == location)
        {
            return true;
        }
    }
    return false;
}

void displayValidLocations()
{
    std::cout << "\nValid locations:\n";
    std::cout << "  Zone 1: CityCenter, Mall, RailwayStation\n";
    std::cout << "  Zone 2: TechPark, University\n";
    std::cout << "  Zone 3: Airport, Hospital, Suburb\n\n";
}

// ===========================
// Interactive Menu Functions
// ===========================
void bookRide()
{
    if (!g_system)
        return;

    std::cout << "\n========== BOOK A RIDE ==========\n";

    // Step 1: Display registered riders
    g_system->displayAllRiders();

    std::string riderName;
    std::cout << "Enter rider name: ";
    std::getline(std::cin, riderName);

    if (riderName.empty())
    {
        std::cout << "[ERROR] Rider name cannot be empty.\n";
        return;
    }

    // Step 2: Display valid locations
    g_system->displayAllLocations();

    std::string pickupLocation;
    std::cout << "Enter pickup location: ";
    std::getline(std::cin, pickupLocation);

    if (!isValidLocation(pickupLocation))
    {
        std::cout << "[ERROR] Location '" << pickupLocation << "' not found.\n";
        displayValidLocations();
        return;
    }

    std::string dropoffLocation;
    std::cout << "Enter dropoff location: ";
    std::getline(std::cin, dropoffLocation);

    if (!isValidLocation(dropoffLocation))
    {
        std::cout << "[ERROR] Location '" << dropoffLocation << "' not found.\n";
        displayValidLocations();
        return;
    }

    if (pickupLocation == dropoffLocation)
    {
        std::cout << "[ERROR] Pickup and dropoff locations must be different.\n";
        return;
    }

    // Step 3: Create rider and request trip
    Rider *rider = g_system->addRider(riderName, pickupLocation);
    g_riders.push_back(rider);

    Trip *trip = g_system->requestTrip(rider, pickupLocation, dropoffLocation);

    if (trip)
    {
        std::cout << "\n==============================\n";
        std::cout << "[OK] Trip #" << trip->getId() << " booked successfully!\n";
        std::cout << "  Rider: " << riderName << "\n";
        std::cout << "  Route: " << pickupLocation << " -> " << dropoffLocation << "\n";
        std::cout << "  Status: " << trip->getStateString() << "\n";

        if (trip->getDriver())
        {
            std::cout << "  Driver: " << trip->getDriver()->getName() << "\n";
            std::cout << "  Zone: " << trip->getDriver()->getZoneID() << "\n";
        }
        else
        {
            std::cout << "  [WARNING] No drivers available\n";
        }
        std::cout << "==============================\n\n";
    }
    else
    {
        std::cout << "[ERROR] Failed to create trip.\n";
    }
}

void cancelTrip()
{
    if (!g_system)
        return;

    std::cout << "\n--- Cancel a Trip ---\n";

    const std::vector<Trip *> &allTrips = g_system->getAllTrips();

    if (allTrips.empty())
    {
        std::cout << "[ERROR] No trips available to cancel.\n";
        return;
    }

    std::cout << "\nActive Trips:\n";
    for (const Trip *trip : allTrips)
    {
        std::cout << "  Trip #" << trip->getId() << ": " << trip->getRider()->getName()
                  << " (" << trip->getPickup() << " -> " << trip->getDropoff() << ") "
                  << "Status: " << trip->getStateString() << "\n";
    }

    int tripId;
    std::cout << "\nEnter trip ID to cancel: ";
    std::cin >> tripId;
    std::cin.ignore();

    // Find trip by ID (linear search, no STL map)
    Trip *selectedTrip = nullptr;
    for (Trip *trip : allTrips)
    {
        if (trip->getId() == tripId)
        {
            selectedTrip = trip;
            break;
        }
    }

    if (!selectedTrip)
    {
        std::cout << "[ERROR] Trip #" << tripId << " not found.\n";
        return;
    }

    g_system->cancelTrip(selectedTrip);
    std::cout << "[OK] Trip #" << tripId << " has been cancelled.\n";
}

void undoLastAction()
{
    if (!g_system)
        return;

    std::cout << "\n--- Undo Last Action ---\n";
    bool success = g_system->undoLastAction();

    if (!success)
    {
        std::cout << "[ERROR] No actions to undo.\n";
    }
}

void viewSystemStatus()
{
    if (!g_system)
        return;

    std::cout << "\n========== SYSTEM STATUS ==========\n";

    const std::vector<Trip *> &allTrips = g_system->getAllTrips();

    // Display active trips
    std::cout << "\n--- ACTIVE TRIPS ---\n";
    if (allTrips.empty())
    {
        std::cout << "[INFO] No active trips.\n";
    }
    else
    {
        std::cout << "ID | Rider            | Pickup to Dropoff       | Status     | Driver\n";
        std::cout << "---+------------------+-------------------------+------------+----------------\n";
        for (const Trip *trip : allTrips)
        {
            std::string status = trip->getStateString();
            std::string driverName = trip->getDriver() ? trip->getDriver()->getName() : "Unassigned";

            printf("%2d | %-16s | %-13s to %-7s | %-10s | %s\n",
                   trip->getId(),
                   trip->getRider()->getName().c_str(),
                   trip->getPickup().c_str(),
                   trip->getDropoff().c_str(),
                   status.c_str(),
                   driverName.c_str());
        }
    }

    // Display driver stats
    g_system->displayDriverStats();

    std::cout << "=====================================\n\n";
}

void displayMainMenu()
{
    std::cout << "\n==========================================\n";
    std::cout << "     RIDE-SHARING SYSTEM MENU\n";
    std::cout << "==========================================\n";
    std::cout << "1. Book a Ride\n";
    std::cout << "2. Cancel a Trip\n";
    std::cout << "3. Undo Last Action\n";
    std::cout << "4. View System Status\n";
    std::cout << "5. Exit\n";
    std::cout << "==========================================\n";
    std::cout << "Enter your choice (1-5): ";
}

// ===========================
// Main Function
// ===========================
int main()
{
    std::cout << "\n=========================================\n";
    std::cout << "  RIDE-SHARING DISPATCH & TRIP SYSTEM\n";
    std::cout << "  Pre-Loaded Data + Interactive Menu\n";
    std::cout << "=========================================\n";

    // Create system and setup
    RideShareSystem system;
    g_system = &system;

    setupSystem(system);

    // Interactive menu loop
    int choice = 0;
    while (true)
    {
        displayMainMenu();
        std::cin >> choice;
        std::cin.ignore(); // Clear input buffer

        switch (choice)
        {
        case 1:
            bookRide();
            break;

        case 2:
            cancelTrip();
            break;

        case 3:
            undoLastAction();
            break;

        case 4:
            viewSystemStatus();
            break;

        case 5:
            std::cout << "\n=========================================\n";
            std::cout << "  Thank you for using Ride-Sharing!\n";
            std::cout << "  Final System Summary:\n";
            std::cout << "  - Total Trips: " << system.getAllTrips().size() << "\n";
            std::cout << "  - Total Drivers: " << system.getAllDrivers().size() << "\n";
            std::cout << "=========================================\n\n";
            return 0;

        default:
            std::cout << "[ERROR] Invalid choice. Please enter 1-5.\n";
        }
    }

    return 0;
}
