#define _CRT_SECURE_NO_WARNINGS
#include "StationBST.h"
#include "math.h"
#include "CarBST.h"
#include "displayError.h"
#include <float.h>
#include <stdio.h>
#define BLUE "\033[3;34m"
#define CYAN "\033[3;36m"
#define WHITE "\033[3;37m"
#define MAX_NAME_LEN 999


// Generic function pointer type for searching (by ID, Name, or Distance)
typedef Station*(*Search)(Station* root, void* value);


StationBST* initStationBST(){
	StationBST* bst = (StationBST*)malloc(sizeof(StationBST));
	if (!bst) { displayError(300); return; }
	bst->root = NULL;	// Initialize root to NULL
	return bst;	// Return pointer to new BST
}

Station* createStation(int id,char* name, int nPorts,double* x ,double* y,int nCars) {
	Station* node = (Station*)malloc(sizeof(Station));
	if (!node) { displayError(300); return NULL; }
	node->id = id;	// Set station ID
	node->name = name;	// Set station name (pointer assignment)
	node->nPorts = nPorts;	// Number of charging ports
	node->coord.x = *x;	// Set station X coordinate
	node->coord.y = *y;	// Set station Y coordinate
	node->portsList = NULL;	// Initialize ports list as empty
	node->nCars = nCars;	// Number of cars in queue
	node->carQueue = initQCar();	// Initialize queue of cars
	node->left = NULL;	// Left child in BST
	node->right = NULL;	// Right child in BST
	return node;	// Return new station node
}
Station* insertStation(Station* root, Station* newNode){
	if (!root)	
		return newNode;	// Insert here if reached null
	if (newNode->id <= root->id)	// If new ID less or equal, go left subtree
			root->left = insertStation(root->left, newNode);
	else // Else go right subtree
			root->right = insertStation(root->right, newNode);
	return root;	// Return updated root
}



Station* searchByID(Station* root, void* id) {
	if (!root)
		return NULL;	// Not found in empty tree
	int ID = *(int*)id;	// Cast void* to int*
	if (root->id == ID)
		return root;	// Found station by ID
	if (ID < root->id)
		return searchByID(root->left, id);	// Search left subtree
	else
		return searchByID(root->right, id);	// Search right subtree
}

Station* searchByName(Station* root, void* name) {
	if (!root)
		return NULL;	// Empty tree, no station
	char* NAME = (char*)name;
	if (strcmp(root->name,NAME) == 0)
		return root;	// Found station by name
	Station* left = searchByName(root->left, name);	// Search left subtree
	if (left)
		return left;	// Found in left subtree
	return searchByName(root->right, name);	// Search right subtree
}

Station* searchByDist(Station* root, void* coord) {
	if (!root) {
		return NULL;	// No station found
	}
	Coord* target = (Coord*)coord;
	double distRoot = hypot(root->coord.x - target->x, root->coord.y - target->y);

	// Recursively find closest in left and right subtree
	Station* leftClosest = searchByDist(root->left, coord);
	Station* rightClosest = searchByDist(root->right, coord);
	Station* closest = root;
	double minDist = distRoot;
	if (leftClosest) {
		double distLeft = hypot(leftClosest->coord.x - target->x, leftClosest->coord.y - target->y);
		if (distLeft < minDist) {
			minDist = distLeft;	// Update closest if left closer
			closest = leftClosest;
		}
	}
	if (rightClosest) {
		double distRight = hypot(rightClosest->coord.x - target->x, rightClosest->coord.y - target->y);
		if (distRight < minDist) {
			minDist = distRight;	// Update closest if right closer
			closest = rightClosest;
		}
	}
	return closest;	// Return closest station found
}

Station* searchStation(Station* root, void* value, int type) {
	Search search[3] = { searchByID ,searchByName ,searchByDist };
	return search[type](root, value);	// Call appropriate search function
}

void locNearSt(StationBST* sBST)	{
	if (!sBST->root) {
		printf("No stations!\n");
		return;
	}
	Coord coord;
	char checkInput; //used to check if input has more than just a double
	int portCount = 0,slowCount = 0,midCount = 0, fastCount = 0,occupiedCount = 0;
	printf("please enter location coordinates:\nx:");
	if (scanf("%lf%c", &coord.x,&checkInput) != 2 || checkInput != '\n') { //checks if users enterd an double and nothing else
		displayError(102);	// Input error if not double only
		while (getchar() != '\n'); 
		return;
	}
	printf("y:");
	if (scanf("%lf%c", &coord.y, &checkInput) != 2 || checkInput != '\n') { //checks if users enterd an double and nothing else
		displayError(102);	// Input error if not double only
		while (getchar() != '\n');
		return;
	}
	Station* closest = searchStation(sBST->root, &coord,2);	// Find closest station by coordinates
	
	// Print station details and distance
	printf("=======[ "CYAN"Station details"WHITE" ]=======\n");
	printf("station name: %s\n", closest->name);
	printf("station ID: %d\n", closest->id);
	printf("distance from station: %.2f[km]\n\n", hypot(closest->coord.x - coord.x, closest->coord.y - coord.y));
	Port *port = closest->portsList;
	for (int i = 0; i < closest->nPorts; i++)
	{
		if (port->status != 3) {	// Skip out of order ports
			portCount++;
			switch (port->portType)
			{
			case SLOW:
				slowCount++;
				break;
			case MID:
				midCount++;
				break;
			case FAST:
				fastCount++;
				break;
			}
			if (port->status == 1) {	// Count occupied ports
				occupiedCount++;
			}
		}
		port = port->next;	// Move to next port
	}

	// Print port counts and cars in queue
	printf("working ports in Station: %d\n",portCount);
	printf("|slow ports: %d|\n",slowCount);
	printf("|mid ports:  %d|\n",midCount);
	printf("|fast ports: %d|\n",fastCount);
	printf("occupied ports: %d\n",occupiedCount);
	printf("cars in queue: %d\n", closest->nCars);
	printf("==================================\n");
}

void printStation(Station* station) {
	if (!station) return; 

	printf("- Station Info -\n");
	printf("ID: %d\n", station->id);
	printf("Name: %s\n", station->name);
	printf("Coordinates: (%.2f, %.2f)\n", station->coord.x, station->coord.y);
	printf("Number of Ports: %d\n", station->nPorts);
	printf("Number of Cars in Queue: %d\n", station->nCars);
}

int isCarInStation(Station* station, char lic[9]) {
	Port* head = station->portsList;
	carNode* carnode = station->carQueue->front;
	while(head){
		if(head->p2car && strcmp(head->p2car->nLicense,lic) == 0 ){
			return 1;	// Car found in port
		}
		head = head->next;
	}
	while(carnode){
		if(strcmp(carnode->car->nLicense,lic) == 0){
			return 1;	// Car found in queue
		}
		carnode = carnode->next;
	}
	return 0;	// Car not found
}

Station* findStation(Station* root, char lic[9]){
	if (!root)
		return NULL;
	if (isCarInStation(root, lic))
		return root;	// Station containing the car found
	Station* left = findStation(root->left, lic);
	if (left)
		return left;
	return findStation(root->right, lic);
}

void dispAllSt(StationBST* stationBST){
	if (!stationBST->root){
		printf("No stations!\n");
		return;
	}
	printAllStations(stationBST->root);
}

void printAllStations(Station* root){
	if(!root){
		return;
	}
	printAllStations(root->left);
	printf( "===============[ "CYAN"Station: %s"WHITE" ]===============\n" , root->name);
	printf("Station ID     : %d\n", root->id);
	printPorts(root->portsList);	// Print ports of the station
	printf("Cars in Queue  : %d\n", root->nCars);
	printf("=============================================\n\n");
	printAllStations(root->right);
}

void dispCarsAtSt(StationBST* sBST) {
	if (!sBST->root) {
		printf("No stations!\n");
		return;
	}
	int choiceInput, ID;
	char checkInput, stationName[9];
	Station* station = stationInput(sBST);
	if (station) {
		printCarsInQ(station->carQueue);	// Print cars waiting in queue
		printCarsInPortList(station->portsList);	// Print cars charging in ports
	}
}
Station* stationInputByID(StationBST* sBST) {
	int ID;
	char checkInput;
	printf("Enter station ID: ");
	if (scanf("%d%c", &ID, &checkInput) != 2 || checkInput != '\n') { //checks if users enterd an integer and nothing else {if(scanf scanned amount of values other than 2 || the second value scanned is not an enter)}
		displayError(101);	// Invalid input error
		while (getchar() != '\n'); //clears inputs
		return NULL;
	}
	Station* station = searchStation(sBST->root, &ID, 0);	// Search by ID
	if (!station) {
		displayError(404);	// Station not found
		return NULL;
	}
	return station;
}

Station* stationInput(StationBST* sBST){
	int choiceInput,ID;
    char checkInput, stationName[MAX_NAME_LEN];
	Station* station;
    printf("Enter choice (ID[0],Name[1]): ");
    if (scanf("%d%c", &choiceInput, &checkInput) != 2 || checkInput != '\n') { //checks if users enterd an integer and nothing else {if(scanf scanned amount of values other than 2 || the second value scanned is not an enter)}
        displayError(101);
        while (getchar() != '\n'); //clears inputs
        return NULL;
    }
    else if (choiceInput < 0 || choiceInput > 1) { //checks if input is not between [0,1]
        displayError(200);
        return NULL;
    }
    if (choiceInput == 0) {
		station = stationInputByID(sBST);	// Input by ID
		return station;
    }
    else {
        printf("Enter station name: ");
        scanf("%[^\n]s", stationName);	// Read full line for name
		getchar() != '\n';	// Clear newline
        station = searchStation(sBST->root, stationName, 1);	// Search by name
		if (!station) {
			displayError(404);	// Station not found
			return NULL;
		}
    }
	return station;
}

void reportStStat(StationBST* sBST){
	if (!sBST->root) {
		printf("No stations!\n");
		return;
	}
	int TakenPortsCount=0,OORportsCount=0;
	Station* station = stationInput(sBST);
	if (!station) return;
	Port* port = station->portsList;
	while(port){
		if(port->status == 1){
			TakenPortsCount++;	// Count occupied ports
		}
		else if(port->status == 3){
			OORportsCount++;	// Count out of order ports
		}
		port = port->next;
	}
	double uRate =((double)TakenPortsCount/ (double)station->nPorts)*100;	// Usage %
	double oorRate =((double)OORportsCount / (double)station->nPorts)*100;	// Out of order %
	double loadRate;
	if (station->nCars) {
		loadRate = (double)TakenPortsCount / (double)station->nCars;	// Load ratio
	}
	printf("Usage rate of charging ports in station: %0.1f%%\n", uRate);
	printf("precentage of out of order ports in station: %0.1f%%\n", oorRate);
	printf("Station load level: ");
	if(!station->nCars || loadRate > 1){
		printf("No load\n");
	}
	else if(loadRate == 1){
		printf("Balanced\n");
	}
	else if (loadRate < 1 && loadRate > 0.2) {
		printf("Busy\n");
	}
	else{
		printf("Very Busy\n");
	}

}

void addNewPort(StationBST* sBST) {
	if (!sBST->root) {
		printf("No stations!\n");
		return;
	}
	int pt;
	char checkInput;
	Station* station = stationInput(sBST);
	if (!station) {
		return;
	}
	Port* port = station->portsList;
	Date date = { 0 };
	printf("Enter port type(FAST[0],MID[1],SLOW[2]):");
	if (scanf("%d%c", &pt, &checkInput) != 2 || checkInput != '\n') { //checks if users enterd an integer and nothing else {if(scanf scanned amount of values other than 2 || the second value scanned is not an enter)}
		displayError(101);
		while (getchar() != '\n'); //clears inputs
		return;
	}
	else if (pt < 0 || pt > 2) { //checks if input is not between [0,2]
		displayError(200);
		return;
	}
	station->nPorts++;	// Increment number of ports
	switch (pt)
	{
	case 0:
		port = createPort(station->nPorts, FAST, 2, NULL, date);	// Create FAST port
		break;
	case 1:
		port = createPort(station->nPorts, MID, 2, NULL, date);	// Create MID port
		break;
	case 2:
		port = createPort(station->nPorts, SLOW, 2, NULL, date);	// Create SLOW port
		break;
	default:
		break;
	}
	addPort(station->portsList, port);	// Add port to station's port list
	Car* car = searchCarByPortType(station->carQueue, port->portType);	// Find car waiting for this port type
	if (car) {
		assignCar2port(car, port);	// Assign car to newly added port
		removeCarFromQueue(station->carQueue, car);	// Remove car from queue
		station->nCars--;	// Decrease cars in queue count
	}
}

void releasePorts(StationBST* sBST){
	if (!sBST->root) {
		printf("No stations!\n");
		return;
	}
	Station* station = stationInputByID(sBST);
	if (!station) {
		return;
	}
	Port* head = station->portsList;
	Car* car;
	int freePortCount = 0;
	int mb;
	while(head){
		mb = minutesBetween(head->tin);	// Calculate minutes charged
		if(mb > 600){ //charging for over 10 hours
			printf("The car %s is removed from charger\n", head->p2car->nLicense);
			head->p2car->totalPayed += mb * 1.2;	// Update payment
			head->p2car->pPort = NULL;	// Remove car's port reference
			head->p2car = NULL;	// Remove car from port
			head->status = 2;	// Set port status to free
			head->tin.Year = 0;	// Reset port's start time
			head->tin.Month = 0;
			head->tin.Day = 0;
			head->tin.Hour = 0;
			head->tin.Min = 0;
			car = searchCarByPortType(station->carQueue, head->portType);	// Find waiting car
			freePortCount++;
			if(car){
				assignCar2port(car, head);	// Assign waiting car to freed port
				removeCarFromQueue(station->carQueue, car);	// Remove from queue
				station->nCars--; //decrease number of cars
			}
		}
		head = head->next; //move to next port
	}
	if (!freePortCount) {
		printf("No cars charging for over 10 hours\n");
	}
}

void remOutOrderPort(StationBST* sBST){
	if (!sBST->root) {
		printf("No stations!\n");
		return;
	}
	char checkInput;
	int portNum,oorCount = 0;
	Station* station = stationInputByID(sBST);
	if (!station) {
		return;
	}
	Port* head = station->portsList;
	while(head){
		if(head->status == 3){	// Count out of order ports
			printPort(head);
			oorCount++;
		}
		head = head->next;
	}
	if(!oorCount){
		printf("No out of order ports in station\n");
		return;
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("Enter port to remove: ");
	if (scanf("%d%c", &portNum, &checkInput) != 2 || checkInput != '\n') { //checks if users enterd an double and nothing else
		displayError(101);
		while (getchar() != '\n');
		return;
	}
	head = station->portsList;
	while (head) {
		if (head->status == 3 && head->num == portNum) {	// Find requested port to remove
			removePort(station->portsList,portNum);	// Remove port from list
			station->nPorts--;	// Decrease port count
			printf("Port was removed\n");
			return;
		}
		head = head->next;
	}
	displayError(404);	// Port not found
}

void closeSt(StationBST* sBST){
	if (!sBST->root) {
		printf("No stations!\n");
		return;
	}
	Station* station = stationInput(sBST);
	if(!station){
		return;
	}
	freePortsInStation(station);	// Free all ports in station
	printf("Cars removed from ports\nAll ports deleted\nCars total payed updated\n");
	freeCarQ(station);	// Free all cars in queue
	printf("Freed cars from queue\n");
	sBST->root = removeStation(sBST->root, station->id);	// Remove station from BST
	printf("Station was deleted\n");
}


Station* findMinStation(Station* node) {
	while (node && node->left)	// Go left until the leftmost (smallest) node is found
		node = node->left;
	return node;	// Return the node with the minimum ID
}

Station* removeStation(Station* root, int ID) {
	if (!root)
		return NULL;	// Station not found, return NULL

	if (ID < root->id) {
		root->left = removeStation(root->left, ID);	// Search in left subtree
	}
	else if (ID > root->id) {
		root->right = removeStation(root->right, ID);	// Search in right subtree
	}
	else {
		// Found the station to remove
		if (!root->left) {	// No left child: replace node with right child
			Station* temp = root->right;
			free(root->name);	// Free the station name
			free(root);	// Free the station itself
			return temp;	// Return the right child to link in tree
		}
		else if (!root->right) {	// No right child: replace node with left child
			Station* temp = root->left;
			free(root->name);
			free(root);
			return temp;	// Return the left child
		}
	
		// Node has two children: find minimum node in right subtree
		Station* temp = findMinStation(root->right);
		// Copy data from temp node to current node
		root->carQueue = temp->carQueue;
		root->coord = temp->coord;
		root->id = temp->id;
		root->name = temp->name;
		root->nCars = temp->nCars;
		root->nPorts = temp->nPorts;
		root->portsList = temp->portsList;
		// Remove the temp node from right subtree to avoid duplication
		root->right = removeStation(root->right, temp->id);
	}
	return root;	// Return updated root
}

void freePortsInStation(Station* station){
	Port* port = station->portsList;
	Port* temp;
	if (port) {
		while (port) {
			temp = port->next;	// Save next port before freeing current
			if (port->p2car) {	// If a car is charging, update total payment
				port->p2car->totalPayed += minutesBetween(port->tin) * 1.2;
				port->p2car->pPort = NULL;	// Remove port reference from car
			}
			free(port);	// Free the current port
			port = temp;	// Move to next port
		}
	}
}

void freeCarQ(Station* station) {
	carNode* node = station->carQueue->front;
	carNode* temp;
	if (node) {
		while (node) {
			temp = node->next;	// Save next node before freeing current
			node->car->inqueue = 0;	// Mark car as not in queue anymore
			free(node);	// Free the queue node
			node = temp;	// Move to next node
		}	
	}
	free(station->carQueue);	// Free the queue structure itself
	station->carQueue = NULL;	// Nullify the pointer
}

void freeAllStations(Station* root){
	if(!root){
		return;	// Empty tree, nothing to free
	}
	freeAllStations(root->left);	// Free left subtree recursively
	freeAllStations(root->right);	// Free right subtree recursively
	freePortsInStation(root);	// Free all ports of this station
	freeCarQ(root);	// Free all cars in queue for this station
	free(root->name);	// Free the station’s name string
	free(root);	// Free the station node itself
}