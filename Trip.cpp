#include "Trip.h"

Trip::Trip(int id, Rider* rider, const std::string& pickup, const std::string& dropoff)
    : id(id), rider(rider), driver(nullptr), pickup(pickup), dropoff(dropoff), status(REQUESTED) {}

int Trip::getId() const {
    return id;
}

Rider* Trip::getRider() const {
    return rider;
}

Driver* Trip::getDriver() const {
    return driver;
}

const std::string& Trip::getPickup() const {
    return pickup;
}

const std::string& Trip::getDropoff() const {
    return dropoff;
}

TripState Trip::getStatus() const {
    return status;
}

std::string Trip::getStateString() const {
    switch (status) {
        case REQUESTED:
            return "REQUESTED";
        case ASSIGNED:
            return "ASSIGNED";
        case ONGOING:
            return "ONGOING";
        case COMPLETED:
            return "COMPLETED";
        case CANCELLED:
            return "CANCELLED";
        default:
            return "UNKNOWN";
    }
}

void Trip::setDriver(Driver* newDriver) {
    driver = newDriver;
}

void Trip::setStatus(TripState newStatus) {
    status = newStatus;
}
