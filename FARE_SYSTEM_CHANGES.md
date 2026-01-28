# Dynamic Fare & Receipt System - Implementation Summary

## Overview
A comprehensive fare calculation and receipt generation system has been added to the Ride-Sharing application. This system automatically calculates trip fares based on distance and vehicle type, and generates professional receipts upon trip completion.

## Changes Made

### 1. **Trip.h** - Header File Updates
Added new data members and methods to the `Trip` class:

**New Members:**
- `double fare` - Stores the calculated trip fare
- `City *city` - Reference to City object for distance calculations

**New Methods:**
- `getFare()` - Returns the calculated fare
- `setCity(City *cityRef)` - Sets the City reference
- `calculateFare()` - Calculates trip fare based on distance and vehicle type
- `generateReceipt()` - Displays a formatted receipt upon trip completion

### 2. **Trip.cpp** - Implementation
Implemented the fare calculation and receipt generation logic:

**calculateFare() Method:**
- Base Fare: **150 PKR**
- Distance Charge: **30 PKR per km**
- Vehicle Type Factor: **40% discount for Bikes** (60% of base price)
- Formula: `Base Fare + (Distance × 30) × Vehicle Factor`
- Example: 10 km car ride = 150 + (10 × 30) = **450 PKR**
- Example: 10 km bike ride = (150 + 300) × 0.6 = **270 PKR**

**generateReceipt() Method:**
- Displays a formatted receipt box with:
  - Passenger name
  - Driver name and car model
  - Number plate
  - Route (Pickup → Dropoff)
  - Distance in km
  - **Total Fare in PKR**
  - Professional box formatting with borders

### 3. **RideShareSystem.cpp** - Integration
Modified the `RideShareSystem` class to support fare calculation:

**In requestTrip():**
- Passes `City` reference to each Trip: `newTrip->setCity(&city)`
- Enables distance calculations for fare computation

**In completeTrip():**
- Calculates fare automatically: `trip->calculateFare()`
- Generates and displays receipt: `trip->generateReceipt()`
- Marks trip as COMPLETED and shows receipt to user

### 4. **main.cpp** - User Interface Updates
Enhanced the booking and completion flow:

**After Booking (in bookRide()):**
- Automatically calculates estimated fare: `trip->calculateFare()`
- Displays: `[INFO] Estimated Fare: PKR [Amount]`

**Trip Status View (in viewTripStatus()):**
- Shows all trips with status
- Added option to complete trips
- User can select a trip to complete
- Receipt is automatically displayed upon completion

**Header Additions:**
- Added `#include <iomanip>` for fare formatting

## Usage Flow

### 1. **Booking a Ride**
```
1. Select Pickup City
2. Select Pickup Location
3. Select Dropoff City
4. Select Dropoff Location
5. Select Driver
6. Enter Rider Name
→ Trip is booked
→ Estimated Fare is displayed
```

### 2. **Completing a Trip**
```
1. Choose "View Trip Status" from menu
2. Review all trips
3. Select "Complete trip" option
4. Enter Trip ID
→ Trip marked as COMPLETED
→ Professional receipt is generated and displayed
```

## Receipt Sample
```
╔════════════════════════════════════════╗
║       RIDE COMPLETE - RECEIPT          ║
╚════════════════════════════════════════╝

Passenger:  John Doe
Driver:     Driver Ahmed (Toyota Corolla)
Number Plate: LEA-123

Route: Kalma Chowk → Blue Area
Distance: 10 km

────────────────────────────────────────
TOTAL FARE: PKR 450.00
────────────────────────────────────────

Thank you for using our ride-sharing service!
```

## Technical Details

### Fare Calculation Algorithm
1. Get distance between pickup and dropoff using `City::getShortestDistance()`
2. Calculate subtotal: `Base Fare (150) + (Distance × 30)`
3. Apply vehicle type factor:
   - If car model contains "Bike": multiply by 0.6 (40% discount)
   - Otherwise: keep full price
4. Return final fare

### Distance Calculation
- Uses Dijkstra's algorithm (existing City implementation)
- Shortest path distance between two locations
- Returns distance in km

### Output Formatting
- Receipts use Unicode box-drawing characters for professional appearance
- Fares formatted with 2 decimal places using `std::setprecision(2)`
- All amounts in Pakistani Rupees (PKR)

## Testing Checklist
- [x] Build compiles without errors
- [x] Trip creation with City reference works
- [x] Fare calculation for cars works (150 + 30×distance)
- [x] Fare calculation for bikes applies 40% discount
- [x] Receipt displays all required information
- [x] Estimated fare shown after booking
- [x] Final receipt shown after trip completion
- [x] Formatting and output look professional

## Future Enhancements (Optional)
- Surge pricing based on demand
- Promo codes and discounts
- Dynamic base fare based on time of day
- Tax calculations
- Payment gateway integration
- Email/SMS receipt delivery
