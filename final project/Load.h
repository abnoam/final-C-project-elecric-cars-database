#pragma once
#include <stdio.h>
#include "CarBST.h"
#include "StationBST.h"

// Loads all system data from text files into memory
// Loads stations, cars, ports, and the line of cars queue
// (CarBST* carBST, StationBST* stationBST)
void loadFiles(CarBST* carBST, StationBST* stationBST);

// Loads all cars from the cars file into the CarBST
// Also assigns them to the appropriate station if relevant
// (FILE* cars, StationBST* stationBST)
// Returns a pointer to the created CarBST
CarBST* loadCars(FILE* cars, StationBST* stationBST);

// Loads all ports from the ports file into their corresponding stations
// If a port is occupied, links it to the correct car
// (FILE* ports, StationBST* stationBST, CarBST* carBST)
void loadPorts(FILE* ports, StationBST* stationBST, CarBST* carBST);

// Loads all stations from the stations file into the StationBST
// (FILE* stations)
// Returns a pointer to the created StationBST
StationBST* loadStations(FILE* stations);

// Loads the queue of cars waiting at stations from file
// Adds each car to the correct station's queue
// (FILE* lineOfCars, StationBST* stationBST, CarBST* carBST)
void loadLineOfCars(FILE* lineOfCars, StationBST* stationBST, CarBST* carBST);


// Updates all data files (stations, cars, ports, queues) based on the current system state
// (StationBST* sBST, CarBST* cBST)
void updateFiles(StationBST* sBST, CarBST* cBST);

// Recursively writes all stations to the stations file
// Writes their ports to the ports file and queues to the lineOfCars file
// (Station* root, FILE* stations, FILE* ports, FILE* lineOfCars)
void updateStations(Station* root, FILE* stations, FILE* ports, FILE* lineOfCars);

// Recursively writes all cars to the cars file
// (StationBST* sBST, tCar* root, FILE* cars)
void updateCars(StationBST* sBST, tCar* root, FILE* cars);

// Exits the system safely
// Updates all files and frees all allocated memory
// (StationBST* sBST, CarBST* cBST)
void exitSys(StationBST* sBST, CarBST* cBST);

// Frees all memory for stations and cars
// (StationBST* sBST, CarBST* cBST)
void freeAll(StationBST* sBST, CarBST* cBST);