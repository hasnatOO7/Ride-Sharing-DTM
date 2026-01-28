#ifndef RIDESHARESYSTEM_H
#define RIDESHARESYSTEM_H

#include <vector>
#include <string>
#include "City.h"
#include "DispatchEngine.h"
#include "RollbackManager.h"

class Driver;
class Rider;
class Trip;

struct CityInfo
{
    int cityId;
    std::string cityName;
    std::vector<std::string> locations;
};

class RideShareSystem
{
private:
    City city;
    DispatchEngine dispatchEngine;
    RollbackManager rollbackManager;
    std::vector<Driver *> drivers;
    std::vector<Rider *> riders;
    std::vector<Trip *> trips;
    std::vector<CityInfo> cities;
    int nextDriverId;
    int nextRiderId;
    int nextTripId;

public:
    RideShareSystem();
    ~RideShareSystem();

    // City operations
    void addLocation(const std::string &name, int zoneID);
    void addRoad(const std::string &locationA, const std::string &locationB, int distance);

    // Driver operations
    Driver *addDriver(const std::string &name, const std::string &carModel,
                      const std::string &numberPlate, const std::string &location, int zoneID);

    // Rider operations
    Rider *addRider(const std::string &name, const std::string &location);

    // Trip operations
    Trip *requestTrip(Rider *rider, const std::string &pickup, const std::string &dropoff, Driver *manualDriver = nullptr);
    bool completeTrip(Trip *trip);
    bool cancelTrip(Trip *trip);
    bool undoLastAction();

    // Pakistani Cities Setup
    void setupPakistaniCities();
    void displayCities() const;
    void displayLocationsByCity(int cityId) const;
    void displayAvailableDrivers() const;
    void displayAvailableDriversWithETA(const std::string &pickupLocation) const;
    Driver *getDriverById(int driverId) const;
    std::string getLocationById(int cityId, int locationId) const;

    // Getter methods
    const std::vector<Trip *> &getAllTrips() const;
    const std::vector<Driver *> &getAllDrivers() const;
    const std::vector<Rider *> &getAllRiders() const;
<<<<<<< Updated upstream
    const std::vector<CityInfo> &getAllCities() const;
=======
    void bookRide(int pickupCityId, int pickupLocationId, int dropoffCityId, int dropoffLocationId, int driverId);
    void showAvailableDrivers();

    // Display helper methods
    void displayAllLocations() const;
    void displayAllRiders() const;
    void displayDriverStats() const;

    // Utility method
    Trip *findRiderActiveTrip(Rider *rider) const;
>>>>>>> Stashed changes
};

#endif // RIDESHARESYSTEM_H
