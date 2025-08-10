#pragma once
#include <stdio.h>
#include "CarBST.h"
#include "StationBST.h"

void loadFiles(CarBST* carBST, StationBST* stationBST);
CarBST* loadCars(FILE* cars, StationBST* stationBST);
void loadPorts(FILE* ports, StationBST* stationBST, CarBST* carBST);
StationBST* loadStations(FILE* stations);
void loadLineOfCars(FILE* lineOfCars, StationBST* stationBST, CarBST* carBST);
void updateFiles(StationBST* sBST, CarBST* cBST);
void updateStations(Station* root, FILE* stations, FILE* ports, FILE* lineOfCars);
void updateCars(StationBST* sBST, tCar* root, FILE* cars);
void exitSys(StationBST* sBST, CarBST* cBST);
void freeAll(StationBST* sBST, CarBST* cBST);