#include "Rider.h"

Rider::Rider(int id, const std::string &name, const std::string &location)
    : id(id), name(name), location(location) {}

int Rider::getId() const
{
    return id;
}

const std::string &Rider::getName() const
{
    return name;
}

const std::string &Rider::getLocation() const
{
    return location;
}

void Rider::setLocation(const std::string &newLocation)
{
    location = newLocation;
}
