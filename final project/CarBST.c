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
	bst->root = NULL;
	return bst;
}

tCar* createTCar(Car* car) {
	tCar* node = (tCar*)malloc(sizeof(tCar));
	if (!node) { displayError(300); return NULL; }
	node->car = car;
	node->left = NULL;
	node->right = NULL;
	return node;
}
tCar* insertTCar(tCar* root, tCar* newNode) {
	if (root == NULL)
		return newNode;
	int newData = atoi(newNode->car->nLicense);
	int rootData = atoi(root->car->nLicense);
	if (newData <= rootData) //left subtree (<=)
			root->left = insertTCar(root->left, newNode);
	else //right subtree (>)
			root->right = insertTCar(root->right, newNode);
	return root;
}

Car* searchCar(tCar* root, char nLicense[9]){
	if (root == NULL)
		return NULL;
	if(*nLicense == '\0')
		return NULL;
	int rootData = atoi(root->car->nLicense);
	int nLic = atoi(nLicense);
	if (nLic == rootData)
		return root->car;
	if (nLic < rootData)
		return searchCar(root->left, nLicense);
	else
		return searchCar(root->right, nLicense);
}


void chargeCar(StationBST** stationBST, CarBST* carBST) {
	char stationName[MAX_NAME_LEN];
	StationBST* sBST = *stationBST;
	if(!sBST->root){
		printf("No stations!\n");
		return;
	}
	char carLicense[9],checkInput;
	int digitCount = 0, licBool=1, inputType, pt, choiceInput, ID;
	Station* station;
	Port* port;
	while (licBool) {
		carLicenseInput(carLicense);
		if(strcmp(carLicense,"\0") != 0){
			licBool = 0;
		}
	}
	licBool = 1;
	Car* car = searchCar(carBST->root,carLicense);
	if (!car)
	{
		while (licBool) {
			printf("Enter car's port type(FAST[0],MID[1],SLOW[2]): ");
			if (scanf("%d%c", &pt, &checkInput) != 2 || checkInput != '\n') { //checks if users enterd an integer and nothing else {if(scanf scanned amount of values other than 2 || the second value scanned is not an enter)}
				displayError(101);
				while (getchar() != '\n'); //clears inputs
			}
			else if (pt < 0 || pt > 2) { //checks if input is not between [0,2]
				displayError(200);
			}
			else{
				licBool = 0;
				switch (pt)
				{
				case 0:
					car = createCar(carLicense, FAST, 0, NULL, 0);
					break;
				case 1:
					car = createCar(carLicense, MID, 0, NULL, 0);
					break;
				case 2:
					car = createCar(carLicense, SLOW, 0, NULL, 0);
					break;
				default:
					break;
				}
				tCar* tcar = createTCar(car);
				carBST->root = insertTCar(carBST->root, tcar);
			}
		}
	}
	licBool = 1;
	do{
		station = stationInput(sBST);
	} while (!station);
	port = searchAvailbleByPT(station->portsList,car->portType);
	if(!car->pPort){
		if(port) {
			assignCar2port(car, port);
		}
		else {
			car->totalPayed += minutesBetween(car->pPort->tin) * 1.2;
			enqueueCar(station->carQueue, car);
			car->inqueue = 1;
			station->nCars++;
			printf("ports are occupid added to queue\n");
		}
	}
	else{
		station = findStation(sBST->root, car->nLicense);
		printf("Car is already charging in %s station\n",station->name);
	}
}

void checkCarStatus(StationBST* stationBST, CarBST* carBST) {
	char carLicense[9];
	int carsInFront;
	carLicenseInput(carLicense);
	if (strcmp(carLicense, "\0") == 0) {
		return;
	}
	Car* car = searchCar(carBST->root, carLicense);
	if(!car){
		displayError(404);
		return;
	}
	Station* station = findStation(stationBST->root, car->nLicense);
	if (station) {
		if (!car->inqueue) {
			printf("----- Info -----\n");
			printf("Station Name: %s\n", station->name);
			printf("Port number: %d\n", car->pPort->num);
			printf("Charge time: %d[Min]\n", minutesBetween(car->pPort->tin));
		}
		else{
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
	Car* car = searchCar(cBST->root, carLicense);
	if(!car){
		displayError(404);
		return;
	}
	Station* station = findStation(sBST->root, car->nLicense);
	Port* port = car->pPort;
	if(port){
		car->pPort = NULL;
		car->totalPayed += minutesBetween(port->tin) * 1.2;
		port->tin.Min = 0;
		port->tin.Hour = 0;
		port->tin.Day = 0;
		port->tin.Month = 0;
		port->tin.Year = 0;
		port->p2car = NULL;
		port->status = 2;
		printf("The car %s is removed from charger\n", car->nLicense);
	}
	else{
		printf("Car is not charging\n");
		return;
	}
	car = searchCarByPortType(station->carQueue, port->portType);
	if(car){
		assignCar2port(car, port);
		removeCarFromQueue(station->carQueue, car);
		station->nCars--;
		return;
	}
}

void dispTopCustomers(CarBST* cBST) {
	if (cBST->root) {
		Car* carList[5] = { NULL };
		findTopCustumers(cBST->root, carList);
		printTopCustumer(carList);
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
			for (int j = 4; j > i; j--) {	// move every element in list 1 place forward
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
		node = node->left;
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
	else {
		if (root->left == NULL) {
			tCar* temp = root->right;
			free(root);
			return temp;
		}
		else if (root->right == NULL) {
			tCar* temp = root->left;
			free(root);
			return temp;
		}

		tCar* temp = findMinTcar(root->right);
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
	if(car->inqueue == 1){
		printf("Car is in queue\nCannot be removed\n");
		return;
	}
	else if(car->pPort){
		printf("Car is charging\nCannot be removed\n");
		return;
	}
	else{
		cBST->root = removeCar(cBST->root, car->nLicense);
		printf("Customer is removed\n");
	}
}


void freeAllCar(tCar* root) {
	if (!root) {
		return;
	}
	freeAllCar(root->left);
	freeAllCar(root->right);
	free(root->car);
	free(root);
}






