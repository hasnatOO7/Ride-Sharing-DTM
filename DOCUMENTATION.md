# Ride-Sharing Dispatch & Trip Management System

A complete C++ implementation of a ride-sharing system with real-time dispatch, driver management, trip tracking, and rollback/undo functionality.

## System Architecture

### Core Components

#### 1. **City (City.h / City.cpp)**
- **Graph-based location network** using custom adjacency lists (no STL maps)
- **Nodes**: Represent physical locations with unique IDs, names, and zone assignments
- **Edges**: Weighted roads between locations representing distances
- **Dijkstra's Algorithm**: Implemented without external libraries to find shortest paths
- **Zone System**: Locations are grouped into zones for penalty-based dispatch logic

#### 2. **Driver (Driver.h / Driver.cpp)**
- Manages driver state and availability
- **Attributes**: ID, name, current location, zone assignment, availability status
- **Methods**: Getters/setters for location updates and availability toggling

#### 3. **Rider (Rider.h / Rider.cpp)**
- Represents passengers requesting rides
- **Attributes**: ID, name, pickup location
- **Methods**: Location management for pickup point

#### 4. **Trip (Trip.h / Trip.cpp)**
- Core entity representing a single ride transaction
- **Enum States**: REQUESTED → ASSIGNED → ONGOING → COMPLETED/CANCELLED
- **Attributes**: ID, rider reference, driver reference, pickup/dropoff locations
- **Method**: `getStateString()` returns human-readable state representation

#### 5. **DispatchEngine (DispatchEngine.h / DispatchEngine.cpp)**
- Intelligent driver assignment algorithm
- **Cost Formula**: `Cost = Distance + Zone Penalty`
  - Distance: Shortest path via Dijkstra between driver location and pickup
  - Zone Penalty: Adds 50 points if driver's zone differs from pickup zone
- **Strategy**: Selects available driver with lowest total cost

#### 6. **RollbackManager (RollbackManager.h / RollbackManager.cpp)**
- Stack-based undo system using `std::vector`
- **TripAction Struct**: Records trip state snapshots (trip, old state, assigned driver)
- **State Restoration**: Properly handles driver availability when reversing state changes
- **Features**:
  - Reverts trip from REQUESTED back to previous state
  - Frees drivers when undoing ASSIGNED/ONGOING/COMPLETED states
  - Re-assigns drivers when undoing CANCELLED state

#### 7. **RideShareSystem (RideShareSystem.h / RideShareSystem.cpp)**
- **Facade pattern** managing all system components
- **Responsibility**:
  - City construction and management
  - Driver/Rider registration
  - Trip lifecycle management
  - Dispatch automation
  - Rollback coordination

### Key Design Decisions

1. **No STL Maps**: Uses linear search in vectors for name-to-ID lookups, simplifying the codebase per requirements
2. **Manual Dijkstra**: Implements shortest path without external graph libraries
3. **No Global Variables**: All state encapsulated in class members
4. **Memory Management**: Explicit new/delete in constructors/destructors
5. **Forward Declarations**: Trip.h uses forward declarations for circular dependencies

## Building & Running

### Prerequisites
- C++11 or later compiler
- CMake 3.10 or later
- Standard build tools (make, ninja, MSVC, etc.)

### Build Instructions

```bash
cd "d:\Ride Sharing DTM\Ride-Sharing-DTM"
mkdir build
cd build
cmake ..
cmake --build .
```

### Run the System

```bash
./RideShareSystem  # On Linux/Mac
RideShareSystem.exe  # On Windows
```

## Test Scenarios

The `main.cpp` file includes comprehensive test cases:

### Scenario 1: Request a Trip
- Rider requests a trip from one location to another
- System automatically finds nearest available driver using dispatch engine
- Trip status transitions from REQUESTED → ASSIGNED
- Driver becomes unavailable

### Scenario 2: Complete a Trip
- Trip status changes from ASSIGNED → COMPLETED
- Driver location updates to dropoff point
- Driver becomes available again

### Scenario 3: Undo Completion
- System reverts trip back to ASSIGNED state
- Driver location remains at dropoff point
- Driver becomes unavailable again

### Scenario 4: Request Another Trip
- Demonstrates dispatch with zone penalties
- Shows how cost formula affects driver selection

### Scenario 5: Cancel a Trip
- Trip status changes to CANCELLED
- Assigned driver becomes available
- Rider can request another trip

### Scenario 6: Undo Cancellation
- System restores trip to ASSIGNED state
- Original driver is re-assigned
- Driver becomes unavailable

## Algorithm Details

### Dijkstra's Algorithm Implementation

```
1. Initialize distance array with INT_MAX except start node (0)
2. Mark all nodes as unvisited
3. For each node in graph:
   a. Find unvisited node with minimum distance
   b. Mark it as visited
   c. Update distances to all unvisited neighbors
   d. If new distance < old distance, update
```

**Time Complexity**: O(V²) where V = number of locations
**Space Complexity**: O(V + E) where E = number of roads

### Dispatch Algorithm

```
1. Get pickup location's zone
2. For each available driver:
   a. Calculate distance via Dijkstra
   b. Add zone penalty if needed
   c. Track driver with minimum total cost
3. Return best driver or null if none available
```

**Time Complexity**: O(D × V²) where D = drivers, V = locations
**Space Complexity**: O(V) for distance arrays

## File Structure

```
Ride-Sharing-DTM/
├── README.md                 # This file
├── CMakeLists.txt           # Build configuration
├── City.h / City.cpp        # Graph implementation
├── Driver.h / Driver.cpp    # Driver management
├── Rider.h / Rider.cpp      # Rider management
├── Trip.h / Trip.cpp        # Trip state machine
├── DispatchEngine.h / cpp   # Dispatch logic
├── RollbackManager.h / cpp  # Undo system
├── RideShareSystem.h / cpp  # Facade/Coordinator
└── main.cpp                 # Test harness
```

## Example Output

```
=== Ride-Sharing Dispatch & Trip Management System ===

Setting up city...
City setup complete with locations and roads

Adding drivers...
Drivers added: Alice, Bob, Charlie, Diana

Adding riders...
Riders added: John, Sarah

=== TEST SCENARIO 1: Request a Trip ===
Trip #0 requested by John. Assigned to Driver: Alice
Trip Status: ASSIGNED
Assigned Driver: Alice
Driver Available: No

=== TEST SCENARIO 2: Complete a Trip ===
Trip #0 completed. Status: COMPLETED
Trip Status: COMPLETED
Driver Available: Yes
Driver Location: Airport

=== TEST SCENARIO 3: Undo the Completion ===
Undo operation completed successfully
Trip Status: ASSIGNED
Driver Available: No

...
```

## Extension Points

1. **Persistence Layer**: Add file I/O for trip history
2. **Rating System**: Track driver and rider ratings
3. **Pricing Engine**: Calculate dynamic fares based on demand
4. **Real-time Updates**: Integrate with WebSocket for live tracking
5. **Advanced Matching**: ML-based driver selection beyond distance
6. **Multi-hop Routing**: Support intermediate stops
7. **Surge Pricing**: Implement zone-based surge multipliers

## Constraints & Limitations

- **Linear Name Lookups**: O(n) for finding locations/drivers by name
- **Quadratic Dijkstra**: O(V²) without priority queue optimization
- **Memory Growth**: No trip history cleanup (use database in production)
- **Single-threaded**: No concurrent request handling
- **Primitive Penalty System**: Fixed zone penalty doesn't scale with distance

## Testing

Run the compiled binary to see all test scenarios execute automatically. Output includes:
- Location setup and road creation
- Driver creation with zone assignment
- Trip request with automatic dispatch
- Trip completion with driver state updates
- Undo operations with state restoration
- Final system summary with all entities

---

**Author**: C++ Expert Implementation
**Version**: 1.0
**Standard**: C++11 or later
