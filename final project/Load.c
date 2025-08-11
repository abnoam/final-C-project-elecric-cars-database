#define _CRT_SECURE_NO_WARNINGS
#include "Load.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "displayError.h"
#define BUFFER_SIZE 250



void loadFiles(CarBST** carBST, StationBST** stationBST) {
	// Open all data files for reading
	FILE* cars = fopen("Cars.txt", "r");
	FILE* ports = fopen("Ports.txt", "r");
	FILE* stations = fopen("Stations.txt", "r");
	FILE* lineOfCars = fopen("LineOfCars.txt", "r");
	char buffer[BUFFER_SIZE];
	fgets(buffer, sizeof(buffer), stations);   // Skip the first line (header)
	*stationBST = loadStations(stations);      // Load stations BST from file
	fclose(stations);
	fgets(buffer, sizeof(buffer), cars);       // Skip the first line (header)
	*carBST = loadCars(cars, *stationBST);     // Load cars BST from file
	fclose(cars);
	fgets(buffer, sizeof(buffer), ports);      // Skip the first line (header)
	loadPorts(ports, *stationBST,*carBST);     // Load ports and associate with stations and cars
	fclose(ports);
	fgets(buffer, sizeof(buffer), lineOfCars); // Skip the first line (header)
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
		sscanf(line, "%8[^,],%d", license, &s_ID);           // Parse car license and station ID from each line
		station = searchStation(stationBST->root, &s_ID, 0); // Find station by ID
		car = searchCar(carBST->root, license);              // Find car by license
		// Add car to station’s waiting queue and increment car count
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
	// Initialize an empty Car BST
	CarBST* bst = initCarBST();
	Car* car;
	Station* station;
	while (fgets(line, sizeof(line), cars)) {
		// Parse each line to get car info, including license, port type, total paid, station ID, port number, in queue status
		sscanf(line, "%8[^,],%[^,],%lf,%d,%d,%d", license, PT, &totalPayed, &s_ID, &portNumber, &inqueue);
		// Map port type string to enum
		if (strcmp(PT, "SLOW") == 0)
			portType = SLOW;
		else if (strcmp(PT, "MID") == 0)
			portType = MID;
		else if (strcmp(PT, "FAST") == 0)
			portType = FAST;
		car = createCar(license, portType, totalPayed, NULL, inqueue); // Create car struct with no assigned port initially
		bst->root = insertTCar(bst->root, createTCar(car));            // Insert car node into the BST keyed by license
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
		// Parse port data including station ID, port number, type, status, last charging time, car license
		sscanf(line, "%d,%d,%[^,],%d,%d,%d,%d,%d,%d,%8[^,]", &stationID, &portNumber, PT, &status, &tin.Year, &tin.Month, &tin.Day, &tin.Hour, &tin.Min, carLicense);
		// Map port type string to enum
		if (strcmp(PT, "SLOW") == 0)
			portType = SLOW;
		else if (strcmp(PT, "MID") == 0)
			portType = MID;
		else if (strcmp(PT, "FAST") == 0)
			portType = FAST;
		if (carLicense[0] == '-') carLicense[0] = '\0';           // If car license field first char is "-" , interpret as empty string (no car assigned)
		station = searchStation(stationBST->root, &stationID, 0); // Find the station by ID
		car = searchCar(carBST->root, carLicense);
		// Create port with or without an assigned car pointer depending on search result
		if (!car) {
			port = createPort(portNumber, portType, status, NULL, tin);
		}
		else {
			port = createPort(portNumber, portType, status, car, tin);
			car->pPort = port;
		}
		// Add port to station's port list, either as first or appended port
		if (!station->portsList)
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
		// Parse station info: id, name, number of ports, and coordinates
		sscanf(line, "%d,%[^,],%d,%lf,%lf", &id, SN, &numOfPorts, &coord.x, &coord.y);
		// Allocate memory for station name dynamically (important to avoid pointer issues)
		stationName = malloc(strlen(SN) + 1);
		if (!stationName) { printf("Mem Allocation failed\n"); stationName = NULL; }
		strcpy(stationName, SN);
		// Create station struct and insert into BST by station ID
		bst->root = insertStation(bst->root, createStation(id, stationName, numOfPorts, &coord.x, &coord.y, 0));
	}
	return bst;
}

void updateFiles(StationBST* sBST,CarBST* cBST){
	// Open all files for overwriting (w mode)
	FILE* cars = fopen("Cars.txt", "w");
	FILE* ports = fopen("Ports.txt", "w");
	FILE* stations = fopen("Stations.txt", "w");
	FILE* lineOfCars = fopen("LineOfCars.txt", "w");
	// Write header lines to each file for clarity and consistency
	fprintf(cars, "License,PortType,TotalPayed,StationID,PortNumber,InQueue\n");
	fprintf(ports, "StationID,PortNumber,PortType,Status,Year,Month,Day,Hour,Min,CarLicense\n");
	fprintf(stations, "ID,StationName,NumOfPorts,CoordX,CoordY\n");
	fprintf(lineOfCars, "License,StationID\n");
	updateStations(sBST->root, stations, ports, lineOfCars); // Traverse and write station, ports, and queue info recursively
	updateCars(sBST, cBST->root, cars);                      // Traverse and write all cars info recursively
	// Close files after writing
	fclose(stations);
	fclose(cars);
	fclose(ports);
	fclose(lineOfCars);
}

void updateStations(Station* root,FILE* stations,FILE* ports,FILE* lineOfCars){
	if (root)
	{
		// Write station details to stations file
		fprintf(stations,"%d,%s,%d,%f,%f\n",root->id,root->name,root->nPorts,root->coord.x,root->coord.y);
		Port* port = root->portsList;
		while (port) { // Iterate over all ports in station's port list and write their details
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
				// Write car license if port is occupied
				strcpy(carLic, port->p2car->nLicense);
			}
			else{
				// Write "-1" if no car assigned
				strcpy(carLic, "-1");
			}
			// Write port data including status, last charging time, and car license
			fprintf(ports,"%d,%d,%s,%d,%d,%d,%d,%d,%d,%s\n",root->id,port->num,pt,port->status,port->tin.Year, port->tin.Month, port->tin.Day, port->tin.Hour, port->tin.Min,carLic);
			port = port->next;
		}
		carNode* node = root->carQueue->front;
		while(node){ // Write all cars in station's waiting queue to lineOfCars file
		fprintf(lineOfCars,"%s,%d\n",node->car->nLicense,root->id);
		node = node->next;
		}
		// Recursive calls for left and right children in station BST
		updateStations(root->left, stations, ports, lineOfCars);
		updateStations(root->right, stations, ports, lineOfCars);
	}
}

void updateCars(StationBST* sBST,tCar* root,FILE* cars){
	if(root){
		char pt[5];
		switch (root->car->portType) { // Map car's portType enum to string for writing
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
		Station* station = findStation(sBST->root, root->car->nLicense); // Find station where car is located (if any)
		int sID = 0;
		if(station){
			sID = station->id;
		}
		int portNum = 0;
		if(root->car->pPort){ // Find port number if car is charging on a port
			portNum = root->car->pPort->num;
		}
		// Write car info including license, port type, total payment, station ID, port number, and queue status
		fprintf(cars, "%s,%s,%f,%d,%d,%d\n", root->car->nLicense, pt, root->car->totalPayed, sID, portNum, root->car->inqueue);
		// Recursive calls to traverse left and right subtree nodes
		updateCars(sBST, root->left, cars);
		updateCars(sBST, root->right, cars);
	}
}

void exitSys(StationBST* sBST, CarBST* cBST) {
	updateFiles(sBST, cBST); // Before exiting, update all data files with current in-memory data
	freeAll(sBST, cBST);     // Free all dynamically allocated memory for stations and cars
	printf("bye bye\n");     // Final message on program exit

}

void freeAll(StationBST* sBST, CarBST* cBST) {
	freeAllStations(sBST->root); // Recursively free all stations in the BST
	freeAllCar(cBST->root);      // Before exiting, update all data files with current in-memory data
}



