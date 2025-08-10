#define _CRT_SECURE_NO_WARNINGS
#include "Load.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "displayError.h"
#define BUFFER_SIZE 250



void loadFiles(CarBST** carBST, StationBST** stationBST) {
	FILE* cars = fopen("Cars.txt", "r");
	FILE* ports = fopen("Ports.txt", "r");
	FILE* stations = fopen("Stations.txt", "r");
	FILE* lineOfCars = fopen("LineOfCars.txt", "r");
	char buffer[BUFFER_SIZE];
	fgets(buffer, sizeof(buffer), stations);// goes down one line
	*stationBST = loadStations(stations);
	fclose(stations);
	fgets(buffer, sizeof(buffer), cars);// goes down one line
	*carBST = loadCars(cars, *stationBST);
	fclose(cars);
	fgets(buffer, sizeof(buffer), ports);// goes down one line
	loadPorts(ports, *stationBST,*carBST);
	fclose(ports);
	fgets(buffer, sizeof(buffer), lineOfCars);// goes down one line
	loadLineOfCars(lineOfCars, *stationBST, *carBST);
	fclose(lineOfCars);
}

void loadLineOfCars(FILE* lineOfCars, StationBST* stationBST, CarBST* carBST) {
	char line[BUFFER_SIZE];
	char license[9];
	int s_ID;
	Station* station;
	Car* car;
	while (fgets(line, sizeof(line), lineOfCars)) {
		sscanf(line, "%8[^,],%d", license, &s_ID);
		station = searchStation(stationBST->root, &s_ID, 0);
		car = searchCar(carBST->root, license);
		enqueueCar(station->carQueue, car);
		station->nCars++;
	}

}

CarBST* loadCars(FILE* cars, StationBST* stationBST) {
	char line[BUFFER_SIZE];
	char license[9];
	char PT[5];
	PortType portType;
	double totalPayed;
	int s_ID, portNumber, inqueue;
	CarBST* bst = initCarBST();
	Car* car;
	Station* station;
	while (fgets(line, sizeof(line), cars)) {
		sscanf(line, "%8[^,],%[^,],%lf,%d,%d,%d", license, PT, &totalPayed, &s_ID, &portNumber, &inqueue);
		if (strcmp(PT, "SLOW") == 0)
			portType = SLOW;
		else if (strcmp(PT, "MID") == 0)
			portType = MID;
		else if (strcmp(PT, "FAST") == 0)
			portType = FAST;
		car = createCar(license, portType, totalPayed, NULL, inqueue);
		bst->root = insertTCar(bst->root, createTCar(car));
	}
	return bst;
}

void loadPorts(FILE* ports, StationBST* stationBST,CarBST* carBST) {
	int stationID, portNumber, status;
	Date tin;
	char line[BUFFER_SIZE];
	char PT[5];
	PortType portType;
	char carLicense[9];
	Station* station;
	Car* car;
	Port* port;
	while (fgets(line, sizeof(line), ports)) {
		sscanf(line, "%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%8[^,]", &stationID, &portNumber, PT, &status, &tin.Year, &tin.Month, &tin.Day, &tin.Hour, &tin.Min, carLicense);
		if (strcmp(PT, "SLOW") == 0)
			portType = SLOW;
		else if (strcmp(PT, "MID") == 0)
			portType = MID;
		else if (strcmp(PT, "FAST") == 0)
			portType = FAST;
		if (carLicense[0] == '-') carLicense[0] = '\0';
		station = searchStation(stationBST->root, &stationID, 0);
		car = searchCar(carBST->root, carLicense);
		if (car == NULL) {
			port = createPort(portNumber, portType, status, NULL, tin);
		}
		else {
			port = createPort(portNumber, portType, status, car, tin);
			car->pPort = port;
		}
		if (station->portsList == NULL)
			station->portsList = port;
		else
			addPort(station->portsList, port);
	}
}

StationBST* loadStations(FILE* stations) {
	StationBST* bst = initStationBST();
	int id, numOfPorts;
	char SN[101];
	char* stationName;
	Coord coord;
	char line[BUFFER_SIZE];
	while (fgets(line, sizeof(line), stations)) {
		sscanf(line, "%d,%[^,],%d,%lf,%lf", &id, SN, &numOfPorts, &coord.x, &coord.y);
		stationName = malloc(strlen(SN) + 1);
		if (!stationName) { printf("Mem Allocation failed\n"); stationName = NULL; }
		strcpy(stationName, SN);
		bst->root = insertStation(bst->root, createStation(id, stationName, numOfPorts, &coord.x, &coord.y, 0));
	}
	return bst;
}

void updateFiles(StationBST* sBST,CarBST* cBST){
	FILE* cars = fopen("Cars.txt", "w");
	FILE* ports = fopen("Ports.txt", "w");
	FILE* stations = fopen("Stations.txt", "w");
	FILE* lineOfCars = fopen("LineOfCars.txt", "w");
	fprintf(cars, "License,PortType,TotalPayed,StationID,PortNumber,InQueue\n");
	fprintf(ports, "StationID,PortNumber,PortType,Status,Year,Month,Day,Hour,Min,CarLicense\n");
	fprintf(stations, "ID,StationName,NumOfPorts,CoordX,CoordY\n");
	fprintf(lineOfCars, "License,StationID\n");
	updateStations(sBST->root, stations, ports, lineOfCars);
	updateCars(sBST, cBST->root, cars);
	fclose(stations);
	fclose(cars);
	fclose(ports);
	fclose(lineOfCars);
}

void updateStations(Station* root,FILE* stations,FILE* ports,FILE* lineOfCars){
	if (root)
	{
		fprintf(stations,"%d,%s,%d,%f,%f\n",root->id,root->name,root->nPorts,root->coord.x,root->coord.y);
		Port* port = root->portsList;
		while (port) {
			char pt[5];
			switch (port->portType){
			case SLOW:
				strcpy(pt,"SLOW");
				break;
			case MID:
				strcpy(pt, "MID");
				break;
			case FAST:
				strcpy(pt, "FAST");
				break;
			default:
				break;
			}
			char carLic[9];
			if(port->p2car){
				strcpy(carLic, port->p2car->nLicense);
			}
			else{
				strcpy(carLic, "-1");
			}
			fprintf(ports,"%d,%d,%s,%d,%d,%d,%d,%d,%d,%s\n",root->id,port->num,pt,port->status,port->tin.Year, port->tin.Month, port->tin.Day, port->tin.Hour, port->tin.Min,carLic);
			port = port->next;
		}
		carNode* node = root->carQueue->front;
		while(node){
		fprintf(lineOfCars,"%s,%d\n",node->car->nLicense,root->id);
		node = node->next;
		}
		updateStations(root->left, stations, ports, lineOfCars);
		updateStations(root->right, stations, ports, lineOfCars);
	}
}

void updateCars(StationBST* sBST,tCar* root,FILE* cars){
	if(root){
		char pt[5];
		switch (root->car->portType) {
		case SLOW:
			strcpy(pt, "SLOW");
			break;
		case MID:
			strcpy(pt, "MID");
			break;
		case FAST:
			strcpy(pt, "FAST");
			break;
		default:
			break;
		}
		Station* station = findStation(sBST->root, root->car->nLicense);
		int sID = 0;
		if(station){
			sID = station->id;
		}
		int portNum = 0;
		if(root->car->pPort){
			portNum = root->car->pPort->num;
		}
		fprintf(cars, "%s,%s,%f,%d,%d,%d\n", root->car->nLicense, pt, root->car->totalPayed, sID, portNum, root->car->inqueue);
		updateCars(sBST, root->left, cars);
		updateCars(sBST, root->right, cars);
	}
}

void exitSys(StationBST* sBST, CarBST* cBST) {
	updateFiles(sBST, cBST);
	freeAll(sBST, cBST);
	printf("bye bye\n");
}

void freeAll(StationBST* sBST, CarBST* cBST) {
	freeAllStations(sBST->root);
	freeAllCar(cBST->root);
}



