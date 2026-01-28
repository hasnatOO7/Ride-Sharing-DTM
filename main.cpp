#include <iostream>
#include <string>
#include <vector>
#include <cstdio>
#include <iomanip>
#include <cstdlib>
#include "RideShareSystem.h"
#include "Driver.h"
#include "Rider.h"
#include "Trip.h"

RideShareSystem *g_system = nullptr;

void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void displaySnappyHeader()
{
    std::cout << " =============================================\n";
    std::cout << "           S N A P P Y   R I D E S           \n";
    std::cout << "      Pakistan's #1 Fast Booking System      \n";
    std::cout << " =============================================\n";
}

void setupFleet(RideShareSystem &system)
{
    std::cout << "\n[SETUP] Registering 10 Drivers...\n";

    // Step 1: Add 8 locations with 8 zones
    system.addLocation("Kalma Chowk", 1);
    system.addLocation("Liberty", 1);
    system.addLocation("DHA", 1);
    system.addLocation("Blue Area", 2);
    system.addLocation("F-10", 2);
    system.addLocation("Clifton", 3);
    system.addLocation("Saddar", 3);
    system.addLocation("Murree Road", 4);
    system.addLocation("Committee Chowk", 4);
    system.addLocation("D Ground", 5);
    system.addLocation("Samanabad", 5);
    system.addLocation("Bosan Road", 6);
    system.addLocation("Nishtar Road", 6);
    system.addLocation("Hafizabad Chowk", 7);
    system.addLocation("Sadar Bazar", 7);
    system.addLocation("Okara Bypass", 8);
    system.addLocation("Sadar Okara", 8);

    std::cout << "[OK] Added 17 locations across 8 zones\n";

    // Step 2: Add hardcoded drivers with name, car model, number plate, location, and zone
    std::cout << "Registering drivers...\n";
    system.addDriver("Ahmed", "Toyota Corolla", "LEA-123", "Kalma Chowk", 1);
    system.addDriver("Bilal", "Honda Civic", "ISL-456", "Blue Area", 2);
    system.addDriver("Sara", "Suzuki Alto", "KHI-789", "Clifton", 3);
    system.addDriver("Ali", "Toyota Prius", "RAW-101", "Raja Bazaar", 4);
    system.addDriver("Fatima", "Honda City", "FSB-202", "Clock Tower", 5);
    system.addDriver("Usman", "Hyundai Elantra", "MLT-303", "Chowk Bazaar", 6);
    system.addDriver("Zain", "KIA Picanto", "HFB-404", "City Center", 7);
    system.addDriver("Aisha", "Toyota Fortuner", "OKR-505", "Main Bazaar", 8);
    system.addDriver("Kamran", "Nissan Sunny", "LHR-606", "Liberty", 1);
    system.addDriver("Nida", "Toyota Yaris", "ISL-707", "F-10", 2);

    std::cout << "[OK] Registered 10 drivers\n\n";
}

void displayMainMenu()
{
    std::cout << "\n=========================================\n";
    std::cout << "           MAIN MENU - SELECT ACTION          \n";
    std::cout << "=========================================\n";
    std::cout << " 1. Book a Ride\n";
    std::cout << " 2. View Trip Status\n";
    std::cout << " 3. Cancel a Trip\n";
    std::cout << " 4. Undo Last Action\n";
    std::cout << " 5. Exit\n";
    std::cout << "=========================================\n";
    std::cout << "Enter your choice (1-5): ";
}

void bookRide()
{
    if (!g_system)
        return;

    std::cout << "\n+---------- BOOKING SYSTEM ----------+\n";

    // Step 1: Select pickup city
    std::cout << "\nSTEP 1: Select Pickup City\n";
    g_system->displayCities();
    int pickupCityId;
    std::cout << "Enter City Number: ";
    std::cin >> pickupCityId;
    std::cin.ignore();

    const std::vector<CityInfo> &cities = g_system->getAllCities();
    if (pickupCityId < 1 || pickupCityId > (int)cities.size())
    {
        std::cout << "[ERROR] Invalid city selection.\n";
        return;
    }

    // Step 2: Select pickup location
    std::cout << "\nSTEP 2: Select Pickup Location\n";
    g_system->displayLocationsByCity(pickupCityId);
    int pickupLocId;
    std::cout << "Enter Location Number: ";
    std::cin >> pickupLocId;
    std::cin.ignore();

    std::string pickupLocation = g_system->getLocationById(pickupCityId, pickupLocId);
    if (pickupLocation.empty())
    {
        std::cout << "[ERROR] Invalid location selection.\n";
        return;
    }

    // Step 3: Select dropoff city
    std::cout << "\nSTEP 3: Select Dropoff City\n";
    g_system->displayCities();
    int dropoffCityId;
    std::cout << "Enter City Number: ";
    std::cin >> dropoffCityId;
    std::cin.ignore();

    if (dropoffCityId < 1 || dropoffCityId > (int)cities.size())
    {
        std::cout << "[ERROR] Invalid city selection.\n";
        return;
    }

    // Step 4: Select dropoff location
    std::cout << "\nSTEP 4: Select Dropoff Location\n";
    g_system->displayLocationsByCity(dropoffCityId);
    int dropoffLocId;
    std::cout << "Enter Location Number: ";
    std::cin >> dropoffLocId;
    std::cin.ignore();

    std::string dropoffLocation = g_system->getLocationById(dropoffCityId, dropoffLocId);
    if (dropoffLocation.empty())
    {
        std::cout << "[ERROR] Invalid location selection.\n";
        return;
    }

    if (pickupLocation == dropoffLocation)
    {
        std::cout << "[ERROR] Pickup and dropoff must be different!\n";
        return;
    }

    // Step 5: Show available drivers and select one
    std::cout << "\nSTEP 5: Select Driver\n";
    g_system->displayAvailableDriversWithETA(pickupLocation);

    int driverChoice;
    std::cout << "Enter Driver Number (1-10): ";
    std::cin >> driverChoice;
    std::cin.ignore();

    const std::vector<Driver *> &allDrivers = g_system->getAllDrivers();
    if (driverChoice < 1 || driverChoice > (int)allDrivers.size())
    {
        std::cout << "[ERROR] Invalid driver selection.\n";
        return;
    }

    Driver *selectedDriver = allDrivers[driverChoice - 1];
    if (!selectedDriver || !selectedDriver->getIsAvailable())
    {
        std::cout << "[ERROR] Invalid driver selection or driver not available.\n";
        return;
    }

    // Step 6: Create rider and request trip
    std::cout << "\nEnter Rider Name: ";
    std::string riderName;
    std::getline(std::cin, riderName);

    if (riderName.empty())
    {
        std::cout << "[ERROR] Rider name cannot be empty.\n";
        return;
    }

    Rider *rider = g_system->addRider(riderName, pickupLocation);
    Trip *trip = g_system->requestTrip(rider, pickupLocation, dropoffLocation, selectedDriver);

    if (!trip)
    {
        std::cout << "[ERROR] Failed to create trip.\n";
    }
    else
    {
        // Calculate and display estimated fare
        double estimatedFare = trip->calculateFare();
        int etaMinutes = (int)(estimatedFare / 50); // ETA calculation based on fare

        // Display professional ride ticket
        std::cout << "\n";
        std::cout << "------------------------------------------------\n";
        std::cout << "          RIDE CONFIRMED - SUCCESS!             \n";
        std::cout << "------------------------------------------------\n";
        std::cout << "[>] Driver:      Driver " << selectedDriver->getName() << "\n";
        std::cout << "[>] Car Details: " << selectedDriver->getCarModel() << " (" << selectedDriver->getNumberPlate() << ")\n";
        std::cout << "[>] ETA:         " << etaMinutes << " Minutes\n";
        std::cout << "[>] Route:       " << pickupLocation << "  -->  " << dropoffLocation << "\n";
        std::cout << "------------------------------------------------\n";
        std::cout << "     Your ride is on the way! Stay safe.      \n";
        std::cout << "------------------------------------------------\n";
        std::cout << "[INFO] Estimated Fare: PKR " << std::fixed << std::setprecision(2) << estimatedFare << "\n\n";
    }
}

void viewTripStatus()
{
    if (!g_system)
        return;

    const std::vector<Trip *> &trips = g_system->getAllTrips();

    if (trips.empty())
    {
        std::cout << "\n[INFO] No trips available.\n";
        return;
    }

    std::cout << "\n=============== TRIP STATUS ===============\n";
    std::cout << "ID | Rider        | From    | To      | Driver | Status\n";
    std::cout << "---+--------------+---------+---------+--------+----------\n";

    for (const Trip *trip : trips)
    {
        const char *driverName = "None";
        if (trip->getDriver())
        {
            driverName = trip->getDriver()->getName().c_str();
        }

        printf("%2d | %-12s | %-7s | %-7s | %-6s | %s\n",
               trip->getId(),
               trip->getRider()->getName().c_str(),
               trip->getPickup().c_str(),
               trip->getDropoff().c_str(),
               driverName,
               trip->getStateString().c_str());
    }

    std::cout << "==========================================\n";

    // Option to complete a trip
    std::cout << "\nDo you want to complete a trip? (Y/N): ";
    char choice;
    std::cin >> choice;
    std::cin.ignore();

    if (choice == 'Y' || choice == 'y')
    {
        std::cout << "\nEnter Trip ID to complete: ";
        int tripId;
        std::cin >> tripId;
        std::cin.ignore();

        for (Trip *trip : trips)
        {
            if (trip->getId() == tripId && trip->getStatus() != COMPLETED && trip->getStatus() != CANCELLED)
            {
                g_system->completeTrip(trip);
                return;
            }
        }
        std::cout << "[ERROR] Invalid trip ID or trip already completed/cancelled.\n";
    }
    std::cout << "\n";
}

void cancelTrip()
{
    if (!g_system)
        return;

    const std::vector<Trip *> &trips = g_system->getAllTrips();

    if (trips.empty())
    {
        std::cout << "\n[ERROR] No trips available to cancel.\n";
        return;
    }

    std::cout << "\n===== ACTIVE TRIPS =====\n";
    int activeCount = 0;
    for (const Trip *trip : trips)
    {
        if (trip->getStatus() != CANCELLED && trip->getStatus() != COMPLETED)
        {
            printf(" %d. Trip #%d: %s (%s)\n",
                   ++activeCount,
                   trip->getId(),
                   trip->getRider()->getName().c_str(),
                   trip->getStateString().c_str());
        }
    }

    if (activeCount == 0)
    {
        std::cout << "[INFO] No active trips to cancel.\n";
        return;
    }

    std::cout << "========================\n";
    int tripChoice;
    std::cout << "Enter Trip Number to Cancel: ";
    std::cin >> tripChoice;
    std::cin.ignore();

    activeCount = 0;
    for (Trip *trip : trips)
    {
        if (trip->getStatus() != CANCELLED && trip->getStatus() != COMPLETED)
        {
            activeCount++;
            if (activeCount == tripChoice)
            {
                g_system->cancelTrip(trip);
                return;
            }
        }
    }

    std::cout << "[ERROR] Invalid trip selection.\n";
}

void undoAction()
{
    if (!g_system)
        return;

    std::cout << "\n";
    g_system->undoLastAction();
}

int main()
{
    std::cout << "\n=========================================\n";
    std::cout << "  RIDE-SHARING SYSTEM - PAKISTAN\n";
    std::cout << "     Numerical Input Based Booking\n";
    std::cout << "=========================================\n";

    RideShareSystem rideSystem;
    g_system = &rideSystem;

    // Initialize Pakistani cities and fleet
    rideSystem.setupPakistaniCities();
    setupFleet(rideSystem);

    // Clear screen and display professional header
    clearScreen();
    displaySnappyHeader();

    int choice = 0;
    while (true)
    {
        displayMainMenu();
        std::cin >> choice;
        std::cin.ignore();

        switch (choice)
        {
        case 1:
            bookRide();
            break;

        case 2:
            viewTripStatus();
            break;

        case 3:
            cancelTrip();
            break;

        case 4:
            undoAction();
            break;

        case 5:
            std::cout << "\n =============================================\n";
            std::cout << "   Thank you for using SNAPPY RIDES!          \n";
            std::cout << " =============================================\n";
            std::cout << "       Total Trips: " << rideSystem.getAllTrips().size() << "\n";
            std::cout << "       Total Drivers: " << rideSystem.getAllDrivers().size() << "\n";
            std::cout << " =============================================\n\n";
            return 0;

        default:
            std::cout << "[ERROR] Invalid choice. Please enter 1-5.\n";
        }
    }

    return 0;
}
