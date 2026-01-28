#include "Driver.h"

Driver::Driver(int id, const std::string &name, const std::string &carModel,
               const std::string &numberPlate, const std::string &location, int zoneID)
    : id(id), name(name), carModel(carModel), numberPlate(numberPlate),
      currentLocation(location), zoneID(zoneID), isAvailable(true) {}

int Driver::getId() const
{
    return id;
}

const std::string &Driver::getName() const
{
    return name;
}

const std::string &Driver::getCarModel() const
{
    return carModel;
}

const std::string &Driver::getNumberPlate() const
{
    return numberPlate;
}

const std::string &Driver::getCurrentLocation() const
{
    return currentLocation;
}

int Driver::getZoneID() const
{
    return zoneID;
}

bool Driver::getIsAvailable() const
{
    return isAvailable;
}

void Driver::setCurrentLocation(const std::string &location)
{
    currentLocation = location;
}

void Driver::setZoneID(int zone)
{
    zoneID = zone;
}

void Driver::setAvailable(bool available)
{
    isAvailable = available;
}
