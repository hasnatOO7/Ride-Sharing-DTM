#ifndef DRIVER_H
#define DRIVER_H

#include <string>

class Driver
{
private:
    int id;
    std::string name;
    std::string currentLocation;
    int zoneID;
    bool isAvailable;

public:
    Driver(int id, const std::string &name, const std::string &location, int zoneID);

    int getId() const;
    const std::string &getName() const;
    const std::string &getCurrentLocation() const;
    int getZoneID() const;
    bool getIsAvailable() const;

    void setCurrentLocation(const std::string &location);
    void setZoneID(int zone);
    void setAvailable(bool available);
};

#endif // DRIVER_H
