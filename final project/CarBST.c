#define _CRT_SECURE_NO_WARNINGS
#include "CarBST.h"
#include "Car.h"
#include "StationBST.h"
#include "displayError.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#define CYAN "\033[3;36m"
#define WHITE "\033[3;37m"
#define MAX_NAME_LEN 999


CarBST* initCarBST(){
	CarBST* bst = (CarBST*)malloc(sizeof(CarBST));
	if (!bst) { displayError(300); return NULL; }
	bst->root = NULL;	// Initialize root pointer to NULL (empty tree)
	return bst;
}

tCar* createTCar(Car* car) {
	tCar* node = (tCar*)malloc(sizeof(tCar));
	if (!node) { displayError(300); return NULL; }
	node->car = car;	// Store pointer to Car in this node
	node->left = NULL;
	node->right = NULL;
	return node;
}
tCar* insertTCar(tCar* root, tCar* newNode) {
	if (root == NULL)
		return newNode;	// Insert here if reached leaf
	int newData = atoi(newNode->car->nLicense);
	int rootData = atoi(root->car->nLicense);
	if (newData <= rootData) // If new license is smaller or equal
			root->left = insertTCar(root->left, newNode);	// Insert to left subtree
	else 
			root->right = insertTCar(root->right, newNode);	// Insert to right subtree
	return root;
}

Car* searchCar(tCar* root, char nLicense[9]){
	if (root == NULL)
		return NULL;	// Not found
	if(*nLicense == '\0')
		return NULL;	// Empty license string, invalid input
	int rootData = atoi(root->car->nLicense);
	int nLic = atoi(nLicense);
	if (nLic == rootData)
		return root->car;	// Found matching car
	if (nLic < rootData)
		return searchCar(root->left, nLicense);	// Search left subtree
	else
		return searchCar(root->right, nLicense);	// Search right subtree
}


void chargeCar(StationBST** stationBST, CarBST* carBST) {
	char stationName[MAX_NAME_LEN];
	StationBST* sBST = *stationBST;
	if(!sBST->root){
		printf("No stations!\n");	// No stations to charge at
		return;
	}
	char carLicense[9],checkInput;
	int digitCount = 0, inputType, pt, choiceInput, ID;
	Station* station;
	Port* port;
	 do{
		carLicenseInput(carLicense);	// Get license input from user
	} while (strcmp(carLicense, "\0") == 0);	// Repeat until non-empty input
	Car* car = searchCar(carBST->root,carLicense);	// Search for car in BST
	if (!car)
	{
		do {
			printf("Enter car's port type(FAST[0],MID[1],SLOW[2]): ");
			if (scanf("%d%c", &pt, &checkInput) != 2 || checkInput != '\n') { //checks if users enterd an integer and nothing else {if(scanf scanned amount of values other than 2 || the second value scanned is not an enter)}
				displayError(101);
				while (getchar() != '\n'); //clears inputs
			}
			else if (pt < 0 || pt > 2) { //checks if input is not between [0,2]
				displayError(200);
			}
			else{
				switch (pt)
				{
				case 0:
					car = createCar(carLicense, FAST, 0, NULL, 0);	// Create FAST type car
					break;
				case 1:
					car = createCar(carLicense, MID, 0, NULL, 0);	// Create MID type car
					break;
				case 2:
					car = createCar(carLicense, SLOW, 0, NULL, 0);	// Create SLOW type car
					break;
				default:
					break;
				}
				tCar* tcar = createTCar(car);	// insert car into tree node
				carBST->root = insertTCar(carBST->root, tcar);	// Insert into BST
			}
		} while (!car);
	}
	do{
		station = stationInput(sBST);	// Ask user for station selection
	} while (!station);
	port = searchAvailbleByPT(station->portsList,car->portType);	// Find available port by type	
	if (isCarInStation(station, car->nLicense)) {
		printf("Car is already in this station\n");
	}
	else if(!car->pPort){	// Car is not currently charging
		Station* oldSt = findStation(sBST->root, car->nLicense);	// Check if car is in another station
			if(oldSt){
				oldSt->nCars--;	// Reduce car count in old station
				removeCarFromQueue(oldSt->carQueue, car);	// Remove from old queue
			}
		if(port) {
			assignCar2port(car, port);	// Assign to free port
		}
		else {
			enqueueCar(station->carQueue, car);	// Add to station queue
			car->inqueue = 1;
			station->nCars++;
			printf("ports are occupid added to queue\n");
		}
	}
	else{
		station = findStation(sBST->root, car->nLicense);	// Find station where car is charging
		printf("Car is already charging in %s station\n",station->name);
	}
}

void checkCarStatus(StationBST* stationBST, CarBST* carBST) {
	char carLicense[9];
	int carsInFront;
	carLicenseInput(carLicense);	// Get car license from user
	if (strcmp(carLicense, "\0") == 0) {
		return;
	}
	Car* car = searchCar(carBST->root, carLicense);	// Search for car
	if(!car){
		displayError(404);
		return;
	}
	Station* station = findStation(stationBST->root, car->nLicense);	// Locate station
	if (station) {
		if (!car->inqueue) {	// Car is charging
			printf("----- Info -----\n");
			printf("Station Name: %s\n", station->name);
			printf("Port number: %d\n", car->pPort->num);
			printf("Charge time: %d[Min]\n", minutesBetween(car->pPort->tin));
		}
		else{	// Car is in queue
			printf("----- Info -----\n");
			printf("Station Name: %s\n", station->name);
			printf("Cars in front of the line: %d\n", CarsInFront(station->carQueue,car));
		}
	}
	else{
		printf("Car is not in any station\n");
	}
}

void stopCharge(StationBST* sBST, CarBST* cBST){
	char carLicense[9];
	if (!sBST->root) {
		printf("No stations!\n");
		return;
	}
	carLicenseInput(carLicense);
	if (strcmp(carLicense, "\0") == 0) {
		return;
	}
	Car* car = searchCar(cBST->root, carLicense);	// Search for car
	if(!car){
		displayError(404);
		return;
	}
	Station* station = findStation(sBST->root, car->nLicense);
	Port* port = car->pPort;
	if(port){	// Car is currently charging
		car->pPort = NULL;	// Unassign port from car
		car->totalPayed += minutesBetween(port->tin) * 1.2;	// Add payment
		port->tin.Min = 0;	// Reset time
		port->tin.Hour = 0;
		port->tin.Day = 0;
		port->tin.Month = 0;
		port->tin.Year = 0;
		port->p2car = NULL;
		port->status = 2;	// Set port to free
		printf("The car %s is removed from charger\n", car->nLicense);
	}
	else{
		printf("Car is not charging\n");
		return;
	}
	car = searchCarByPortType(station->carQueue, port->portType);	// Get next car in queue of same type
	if(car){
		assignCar2port(car, port);	// Assign new car to port
		removeCarFromQueue(station->carQueue, car);	// Remove from queue
		station->nCars--;
		return;
	}
}

void dispTopCustomers(CarBST* cBST) {
	if (cBST->root) {
		Car* carList[5] = { NULL };	// Array for top 5 customers
		findTopCustumers(cBST->root, carList);	// Fill list
		printTopCustumer(carList);	// Print results
	}
	else{
		printf("No customers\n");
	}
}

void findTopCustumers(tCar* root,Car* carList[5]) {
	if (root == NULL)
		return;
	findTopCustumers(root->left, carList);
	for (int i = 0; i < 5; i++)
	{
		if (carList[i] == NULL || root->car->totalPayed > carList[i]->totalPayed) {
			for (int j = 4; j > i; j--) {	// move every element in list 1 place forward to make space
				carList[j] = carList[j - 1];
			}
			carList[i] = root->car;
			break;
		}
	}
	findTopCustumers(root->right, carList);
}

void printTopCustumer(Car* carList[5]) {
	printf("~~~~~~~~[ "CYAN"Top 5 custumers"WHITE" ]~~~~~~~~\n");
	for (int i = 0; i < 5; i++)
	{
		printf("Car: %s  |  Total Pay: %.2f\n", carList[i]->nLicense, carList[i]->totalPayed);
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}


tCar* findMinTcar(tCar* node) {
	while (node && node->left)
		node = node->left;	// Move to leftmost node
	return node;
}

tCar* removeCar(tCar* root, char nLicense[9]) {
	if (root == NULL)
		return NULL;

	int nLic = atoi(nLicense);
	int rootData = atoi(root->car->nLicense);

	if (nLic < rootData) {
		root->left = removeCar(root->left, nLicense);
	}
	else if (nLic > rootData) {
		root->right = removeCar(root->right, nLicense);
	}
	else {	// Found node to delete
		if (root->left == NULL) {	// One or zero children
			tCar* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			tCar* temp = root->left;
			free(root);
			return temp;
		}

		tCar* temp = findMinTcar(root->right);	// Find successor
		root->car = temp->car;
		root->right = removeCar(root->right, temp->car->nLicense);
	}
	return root;
}

void remCustomer(CarBST* cBST){
	char carLicense[9];
	carLicenseInput(carLicense);
	if (strcmp(carLicense, "\0") == 0) {
		return;
	}
	Car* car = searchCar(cBST->root, carLicense);
	if (!car) {
		displayError(404);
		return;
	}
	if(car->inqueue){	// Car is in queue
		printf("Car is in queue\nCannot be removed\n");
		return;
	}
	else if(car->pPort){	// Car is charging
		printf("Car is charging\nCannot be removed\n");
		return;
	}
	else{
		cBST->root = removeCar(cBST->root, car->nLicense);	// Remove from tree
		printf("Customer is removed\n");
	}
}


void freeAllCar(tCar* root) {
	if (!root) {
		return;
	}
	freeAllCar(root->left);	// Free left subtree
	freeAllCar(root->right);	// Free right subtree
	free(root->car);	// Free car struct
	free(root);	// Free node
}