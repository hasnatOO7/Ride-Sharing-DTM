#ifndef TRIP_H
#define TRIP_H

#include <string>

class Driver;
class Rider;
class City;

enum TripState
{
    REQUESTED,
    ASSIGNED,
    ONGOING,
    COMPLETED,
    CANCELLED
};

class Trip
{
private:
    int id;
    Rider *rider;
    Driver *driver;
    int driverId;
    std::string pickup;
    std::string dropoff;
    TripState status;
    double fare;
    City *city;

public:
    Trip(int id, Rider *rider, const std::string &pickup, const std::string &dropoff, int driverId);

    int getId() const;
    Rider *getRider() const;
    Driver *getDriver() const;
    const std::string &getPickup() const;
    const std::string &getDropoff() const;
    TripState getStatus() const;
    std::string getStateString() const;
    double getFare() const;

    void setDriver(Driver *driver);
    void setStatus(TripState newStatus);
    void setCity(City *cityRef);

    // Fare and receipt methods
    double calculateFare();
    void generateReceipt() const;
};

#endif // TRIP_H
