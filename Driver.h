#ifndef DRIVER_H
#define DRIVER_H

#include <string>

class Driver
{
private:
    int id;
    std::string name;
    std::string carModel;
    std::string numberPlate;
    std::string currentLocation;
    int zoneID;
    bool isAvailable;

public:
    Driver(int id, const std::string &name, const std::string &carModel,
           const std::string &numberPlate, const std::string &location, int zoneID);

    int getId() const;
    const std::string &getName() const;
    const std::string &getCarModel() const;
    const std::string &getNumberPlate() const;
    const std::string &getCurrentLocation() const;
    int getZoneID() const;
    bool getIsAvailable() const;

    void setCurrentLocation(const std::string &location);
    void setZoneID(int zone);
    void setAvailable(bool available);
};

#endif // DRIVER_H
