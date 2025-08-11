#pragma once
#include "Car.h"
#include "Port.h"



// Coordinates struct
//double x
//double y
typedef struct Coord {
	double x;
	double y;
}Coord;

// Station structure
// int id - station id number
// char* name - station name (MUST BE A DYNAMICALLY ALLOCATED STRING)
// int nPorts - number of ports in station
// Coord coord - station coordinates
// Port* portsList - linked list of the station ports
// int nCars - number of cars in queue
// qCar* carQueue - car queue head
// Station* right - pointer to the right node
// Station* left - pointer to the left node
typedef struct Station {
	int id;
	char* name;
	int nPorts;
	Coord coord;
	Port* portsList;
	int nCars;
	qCar* carQueue;
	struct Station* right;
	struct Station* left;
}Station;


// StationBST structure (binary tree manager struct)
// Station* root - pointer to the root of the binary tree
typedef struct StationBST {
	Station* root;
}StationBST;

// Initializes a new binary search tree of stations
// returns a pointer to the empty tree (root = NULL)
StationBST* initStationBST();

// Creates a new station
// Returns a pointer to the new station
// (int id, char* name, int nPorts, double* x, double* y, int nCars)
Station* createStation(int id, char* name, int nPorts, double* x, double* y, int nCars);

// Inserts a new station node into the binary search tree (by ID)
// Returns the updated root of the tree
// (Station* root, Station* newNode)
Station* insertStation(Station* root, Station* newNode);

// Searches for a station in the tree based on a value and search type
// search by ID: 0,  search by NAME: 1, search by DISTANCE: 2
// Returns a pointer to the found station, or NULL if not found
// (Station* root, void* value, int type)
Station* searchStation(Station* root, void* value, int type);

// Finds and displays the station that is closest in location
// to user coordinates
// (StationBST* bst)
void locNearSt(StationBST* bst);

// Checks if a car with given license is currently 
// in a station
// Returns 1 if found, 0 otherwise
// (Station* station, char lic[9])
int isCarInStation(Station* station, char lic[9]);

// Prints the details of all stations in the BST
// (Station* root)
void printAllStations(Station* root);

// Displays all station names and basic info
// (StationBST* stationBST)
void dispAllSt(StationBST* stationBST);

// Displays all cars currently at all stations
// (StationBST* sBST)
void dispCarsAtSt(StationBST* sBST);

// Prompts the user to select a station by name or ID
// Returns a pointer to the selected station, or NULL if not found
// (StationBST* sBST)
Station* stationInput(StationBST* sBST);

// Generates and displays a status report for station
// (StationBST* sBST)
void reportStStat(StationBST* sBST);

// Adds a new port to a specified station
// (StationBST* sBST)
void addNewPort(StationBST* sBST);

// Prompts user to input a station ID and returns the matching station
// Returns NULL if station is not found
// (StationBST* sBST)
Station* stationInputByID(StationBST* sBST);

// Releases all currently occupied for over 10 hours ports in all stations
// (StationBST* sBST)
void releasePorts(StationBST* sBST);

// Removes all ports marked as out of order from all stations
// (StationBST* sBST)
void remOutOrderPort(StationBST* sBST);

// Closes a station and removes it from the BST
// Frees all allocated memory for that station
// (StationBST* sBST)
void closeSt(StationBST* sBST);

// Finds the station with the minimum ID in a subtree
// Returns a pointer to that station
// (Station* node)
Station* findMinStation(Station* node);

// Removes a station by ID from the BST
// Returns the updated root node
// (Station* root, int ID)
Station* removeStation(Station* root, int ID);

// Frees memory allocated for all ports in a station
// (Station* station)
void freePortsInStation(Station* station);

// Frees memory allocated for the car queue in a station
// (Station* station)
void freeCarQ(Station* station);

// Searches for a station containing a specific car license
// Returns a pointer to that station if found, otherwise NULL
// (Station* root, char lic[9])
Station* findStation(Station* root, char lic[9]);

// Frees all memory for all stations in the BST
// (Station* root)
void freeAllStations(Station* root);