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
// (int id, char* name, int nPorts, int x, int y, int nCars)
Station* createStation(int id, char* name, int nPorts, int x, int y, int nCars);

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

// Checks if a car with given license is currently connected
// to a port in the given port list
// Returns 1 if found, 0 otherwise
// (Port* head, char lic[9])
int isCarInStationPort(Port* head, char lic[9]);

// Prints the details of all stations in the BST
// (Station* root)
void printAllStations(Station* root);

// Displays all station names and basic info
// (StationBST* stationBST)
void dispAllSt(StationBST* stationBST);

// Displays all cars currently at all stations
// (StationBST* sBST)
void dispCarsAtSt(StationBST* sBST);

Station* stationInput(StationBST* sBST);

void reportStStat(StationBST* sBST);

void addNewPort(StationBST* sBST);

Station* stationInputByID(StationBST* sBST);

void releasePorts(StationBST* sBST);

void remOutOrderPort(StationBST* sBST);

void closeSt(StationBST* sBST);

Station* findMinStation(Station* node);

Station* removeStation(Station* root, int ID);

void freePortsInStation(Station* station);

void freeCarQ(Station* station);

Station* findStation(Station* root, char lic[9]);
void freeAllStations(Station* root);