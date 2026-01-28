#ifndef DISPATCHENGINE_H
#define DISPATCHENGINE_H

#include <vector>
#include <string>

class City;
class Driver;

class DispatchEngine
{
public:
    DispatchEngine();

    // Find the nearest available driver with the lowest cost
    // Cost = Distance + Penalty (50 if driver's zone != pickup zone)
    Driver *findNearestDriver(City &city, std::vector<Driver *> &drivers, const std::string &pickupLocation);

private:
    // Calculate cost for a driver
    int calculateCost(City &city, Driver *driver, const std::string &pickupLocation);
};

#endif // DISPATCHENGINE_H
