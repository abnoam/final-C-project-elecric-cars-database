#pragma once


typedef struct Car Car;
typedef struct tCar tCar;
typedef struct StationBST StationBST;

// tCar structure (binary tree node struct)
// Car* car - pointer to car
// tCar* right - pointer to the right node
// tCar* left - pointer to the left node
typedef struct tCar{
	Car* car;
	tCar* right;
	tCar* left;
}tCar;

// CarBST structure (binary tree manager struct)
// tCar* root - pointer to the root of the binary tree
typedef struct CarBST{ 
	tCar* root;
}CarBST;


// Initializes a new binary search tree of cars
// returns a pointer to the empty tree (root = NULL)
CarBST* initCarBST();

// Creates a new car node(tCar)
// Returns a pointer to the new node(tCar)
// (Car* car)
tCar* createTCar(Car* car);

// Inserts a new tCar node into the binary search tree (by nLicense)
// Returns the updated root of the tree
// (tCar* root, tCar* newNode)
tCar* insertTCar(tCar* root, tCar* newNode);

// Searches for a tCar node in the tree based on nLicense
// Returns a pointer to the found tCar, or NULL if not found
// (tCar* root, char* nLicense)
Car* searchCar(tCar* root, char* nLicense);

// Handles the charging process of a car:
// Asks user for car license
// Creates a new car if not exists
// Lets user select a station by ID or name
// If a matching port is available, assigns car to port
// Otherwise, adds car to the queue
// (StationBST** stationBST, CarBST* carBST)
void chargeCar(StationBST** stationBST, CarBST* carBST);

// Displays status of a car (charging or waiting):
// Prints station name
// - If charging: shows port number and time
// - If in queue: shows number of cars in front
// (StationBST* stationBST, CarBST* carBST)
void checkCarStatus(StationBST* stationBST, CarBST* carBST);

// Stops charging for a car:
// Frees port
// Calculates payment
// Assigns next matching car from queue (if exists) to the port
// (StationBST* stationBST, CarBST* carBST)
void stopCharge(StationBST* stationBST, CarBST* carBST);

void printTopCustumer(Car* carList[5]);

void findTopCustumers(tCar* root, Car* carList[5]);

void dispTopCustomers(CarBST* cBST);

tCar* findMinTcar(tCar* node);

tCar* removeCar(tCar* root, char nLicense[9]);

void remCustomer(CarBST* cBST);

void freeAllCar(tCar* root);