#include "RollbackManager.h"
#include "Trip.h"
#include "Driver.h"
#include <iostream>

RollbackManager::RollbackManager() {}

void RollbackManager::recordState(Trip* trip) {
    if (!trip) {
        std::cerr << "Error: Cannot record state of null trip\n";
        return;
    }

    TripAction action;
    action.trip = trip;
    action.oldState = trip->getStatus();
    action.assignedDriver = trip->getDriver();

    actionStack.push_back(action);
}

bool RollbackManager::undoLastOperation() {
    if (actionStack.empty()) {
        std::cerr << "Error: Nothing to undo\n";
        return false;
    }

    TripAction lastAction = actionStack.back();
    actionStack.pop_back();

    Trip* trip = lastAction.trip;
    TripState oldState = lastAction.oldState;
    Driver* assignedDriver = lastAction.assignedDriver;

    // Restore the trip state
    trip->setStatus(oldState);

    // Handle driver state changes
    if (oldState == REQUESTED && trip->getDriver() != nullptr) {
        // Reverting from ASSIGNED/ONGOING/COMPLETED to REQUESTED - free the driver
        trip->getDriver()->setAvailable(true);
        trip->setDriver(nullptr);
    } else if (oldState == ASSIGNED && assignedDriver != nullptr) {
        // Reverting to ASSIGNED state - restore the driver
        trip->setDriver(assignedDriver);
        assignedDriver->setAvailable(false);
    } else if (oldState == CANCELLED) {
        // Reverting from CANCELLED - restore the previous driver assignment
        if (assignedDriver != nullptr) {
            trip->setDriver(assignedDriver);
            assignedDriver->setAvailable(false);
        }
    }

    return true;
}

size_t RollbackManager::getStackSize() const {
    return actionStack.size();
}
