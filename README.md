# Electric Cars Database Management System

## Overview

This is a comprehensive **Electric Vehicles Charging Station Management System** built in C. The system manages multiple charging stations, electric vehicles, charging ports (FAST/MID/SLOW), and queuing mechanisms for vehicles waiting to charge.

## Features

### Core Functionality
- ⚡ **Station Management**: Create, store, and manage multiple charging stations with geographic coordinates
- 🚗 **Vehicle Management**: Track electric vehicles with license plates and charging history
- 🔌 **Port Management**: Handle different charging port types (FAST, MID, SLOW) with occupancy tracking
- 📋 **Queue Management**: Implement vehicle queuing system when ports are unavailable
- 💰 **Payment Tracking**: Record total payments per vehicle
- 📊 **Statistical Reports**: Generate station and customer analytics
- 💾 **Data Persistence**: Save and load system state from files

### Menu Options (13 Operations)
1. **Locate Nearest Station** - Find closest charging station by coordinates
2. **Charge Car** - Register a vehicle for charging at a station
3. **Check Status** - View current charging status or queue position
4. **Stop Charge** - End charging session and process payment
5. **Display All Stations** - View all registered stations
6. **Display Cars at Station** - Show vehicles currently charging or in queue
7. **Station Statistics Report** - Generate detailed station analytics
8. **Display Top Customers** - Show top 5 paying customers
9. **Add New Port** - Add charging port to a station
10. **Release Charging Ports** - Free occupied ports
11. **Remove Out-of-Order Ports** - Remove damaged/broken ports
12. **Remove Customer** - Delete customer record
13. **Close Station** - Shut down a station

## System Architecture

### Data Structures

#### Vehicle Management
- **Car**: Stores vehicle license plate, port type requirement, payment history, station assignment
- **qCar**: Queue manager for vehicles waiting to charge
- **carNode**: Individual node in the vehicle queue
- **CarBST**: Binary Search Tree for efficient vehicle lookup by license plate

#### Station Management
- **Station**: Represents a charging station with ID, name, coordinates, ports, and vehicle queue
- **StationBST**: Binary Search Tree organizing stations by ID
- **Coord**: GPS coordinates (x, y) for location services

#### Charging Infrastructure
- **Port**: Individual charging port with type (FAST/MID/SLOW) and occupancy status
- **PortType**: Enum defining charging speeds

### File Structure
```
final-C-project-elecric-cars-database/
├── final project/
│   ├── main.c                 # Program entry point
│   ├── menu.c                 # Interactive menu system
│   ├── Car.c/h                # Vehicle management
│   ├── CarBST.c/h             # Vehicle BST operations
│   ├── Port.c/h               # Charging port management
│   ├── Station.c/h            # Station operations
│   ├── StationBST.c/h         # Station BST operations
│   ├── Load.c/h               # File I/O operations
│   ├── displayError.c/h       # Error handling
│   └── final project.sln      # Visual Studio solution file
└── README.md
```

## Technical Details

### Key Components

#### Binary Search Trees
- **Vehicle BST**: Ordered by license plate number for O(log n) lookup
- **Station BST**: Ordered by station ID for quick access
- Supports insertion, deletion, and searching operations

#### Queue Implementation
- FIFO (First In First Out) queue for vehicle charging requests
- Duplicate prevention based on license plates
- Dynamic node allocation for memory efficiency

#### Port Type System
- **FAST**: High-speed charging ports
- **MID**: Medium-speed charging ports
- **SLOW**: Standard charging ports
- Each station can have multiple ports of each type

### Error Handling System
The system includes comprehensive error codes:
- **Type Errors** (100-103): Invalid input types
- **Value Errors** (200-201): Out of range or invalid values
- **Memory Errors** (300): Allocation failures
- **File Errors** (400): File I/O issues
- **Not Found** (404): Record lookup failures

### Data Persistence
The system automatically saves/loads:
- Station database (stations.txt)
- Vehicle database (cars.txt)
- Charging port configurations (ports.txt)
- Vehicle queue states (lineOfCars.txt)

## Compilation & Execution

### Requirements
- C compiler (GCC, MSVC, or Clang)
- Visual Studio 2019+ (recommended for .sln file)
- 48 KB minimum disk space

### Build Instructions
```bash
# Using Visual Studio
- Open final project.sln
- Build solution (Ctrl+Shift+B)
- Run the executable

# Using GCC
gcc -o electric_cars_db final\ project/*.c -lm
./electric_cars_db
```

### Running the Program
```
Please enter the number of the option you want:
[1] Locate nearest station
[2] Charge Car
[3] Check for status
...
[0] Quit
Option: _
```

## Usage Examples

### Example 1: Register Vehicle for Charging
1. Select option [2] - Charge Car
2. Enter vehicle license plate: 1234567AB
3. Select port type (FAST/MID/SLOW)
4. Choose station by name or ID
5. System assigns port or queues vehicle if unavailable

### Example 2: Check Charging Status
1. Select option [3] - Check Status
2. Enter vehicle license plate
3. View charging port number and time elapsed, or queue position

### Example 3: View Top Customers
1. Select option [8] - Display Top Customers
2. System calculates and displays top 5 paying customers with total expenditure

## Key Features Explained

### Nearest Station Locator
- Calculates distance using GPS coordinates
- Returns the closest available station
- Useful for navigation and load balancing

### Dynamic Queue Management
- Automatically assigns vehicles to appropriate port types
- Matches vehicle requirements with available ports
- Manages overflow with queue system
- Processes queue when ports become available

### Payment Tracking
- Records total amount paid per vehicle
- Enables top customer identification
- Useful for billing and analytics

### Station Statistics
- Port utilization rates
- Queue lengths
- Revenue per station
- Average charging times

## Memory Management
- Dynamic memory allocation for all structures
- Proper deallocation on system exit
- Prevention of memory leaks through structured cleanup

## Future Enhancement Possibilities
- Real-time GPS integration
- Mobile app interface
- Battery percentage tracking
- Reservation system
- Dynamic pricing based on demand
- Multiple vehicle ownership support

## Author
Created as a final C project for electric vehicle infrastructure management.

## License
Open source project

---

**Last Updated**: August 2025
