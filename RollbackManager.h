#ifndef ROLLBACKMANAGER_H
#define ROLLBACKMANAGER_H

#include <vector>
#include <cstddef>
#include "Trip.h"

class Driver;

struct TripAction {
    Trip* trip;
    TripState oldState;
    Driver* assignedDriver;
};

class RollbackManager {
private:
    std::vector<TripAction> actionStack;

public:
    RollbackManager();

    // Record the current state of a trip before making changes
    void recordState(Trip* trip);

    // Undo the last operation
    bool undoLastOperation();

    // Get the size of the undo stack
    size_t getStackSize() const;
};

#endif // ROLLBACKMANAGER_H
