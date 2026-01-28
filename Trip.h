#ifndef TRIP_H
#define TRIP_H

#include <string>

class Driver;
class Rider;

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
    std::string pickup;
    std::string dropoff;
    TripState status;

public:
    Trip(int id, Rider *rider, const std::string &pickup, const std::string &dropoff);

    int getId() const;
    Rider *getRider() const;
    Driver *getDriver() const;
    const std::string &getPickup() const;
    const std::string &getDropoff() const;
    TripState getStatus() const;
    std::string getStateString() const;

    void setDriver(Driver *driver);
    void setStatus(TripState newStatus);
};

#endif // TRIP_H
