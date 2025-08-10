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

void findClosestStation(Station* root, Coord userCoord, Station** closest, double* minDist);
typedef Station*(*Search)(Station* root, void* value);


StationBST* initStationBST(){
	StationBST* bst = (StationBST*)malloc(sizeof(StationBST));
	if (!bst) { displayError(300); return; }
	bst->root = NULL;
	return bst;
}

Station* createStation(int id,char* name, int nPorts,double* x ,double* y,int nCars) {
	Station* node = (Station*)malloc(sizeof(Station));
	if (!node) { displayError(300); return NULL; }
	node->id = id;
	node->name = name;
	node->nPorts = nPorts;
	node->coord.x = *x;
	node->coord.y = *y;
	node->portsList = NULL;
	node->nCars = nCars;
	node->carQueue = initQCar();
	node->left = NULL;
	node->right = NULL;
	return node;
}
Station* insertStation(Station* root, Station* newNode){
	if (!root)
		return newNode;
	if (newNode->id <= root->id)//left subtree (<=)
			root->left = insertStation(root->left, newNode);
	else //right subtree (>)
			root->right = insertStation(root->right, newNode);
	return root;
}



Station* searchByID(Station* root, void* id) {
	if (!root)
		return NULL;
	int ID = *(int*)id;
	if (root->id == ID)
		return root;
	if (ID < root->id)
		return searchByID(root->left, id);
	else
		return searchByID(root->right, id);
}

Station* searchByName(Station* root, void* name) {
	if (!root)
		return NULL;
	char* NAME = (char*)name;
	if (strcmp(root->name,NAME) == 0)
		return root;
	Station* left = searchByName(root->left, name);
	if (left)
		return left;
	return searchByName(root->right, name);
}

Station* searchByDist(Station* root, void* coord) {
	if (!root) {
		return NULL;
	}
	Coord* target = (Coord*)coord;
	double distRoot = hypot(root->coord.x - target->x, root->coord.y - target->y);
	Station* leftClosest = searchByDist(root->left, coord);
	Station* rightClosest = searchByDist(root->right, coord);
	Station* closest = root;
	double minDist = distRoot;
	if (leftClosest) {
		double distLeft = hypot(leftClosest->coord.x - target->x, leftClosest->coord.y - target->y);
		if (distLeft < minDist) {
			minDist = distLeft;
			closest = leftClosest;
		}
	}
	if (rightClosest) {
		double distRight = hypot(rightClosest->coord.x - target->x, rightClosest->coord.y - target->y);
		if (distRight < minDist) {
			minDist = distRight;
			closest = rightClosest;
		}
	}
	return closest;
}

Station* searchStation(Station* root, void* value, int type) {
	Search search[3] = { searchByID ,searchByName ,searchByDist };
	return search[type](root, value);
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
		displayError(102);
		while (getchar() != '\n'); 
		return;
	}
	printf("y:");
	if (scanf("%lf%c", &coord.y, &checkInput) != 2 || checkInput != '\n') { //checks if users enterd an double and nothing else
		displayError(102);
		while (getchar() != '\n');
		return;
	}
	Station* closest = searchStation(sBST->root, &coord,2);
	printf("=======[ "CYAN"Station details"WHITE" ]=======\n");
	printf("station name: %s\n", closest->name);
	printf("station ID: %d\n", closest->id);
	printf("distance from station: %.2f[km]\n\n", hypot(closest->coord.x - coord.x, closest->coord.y - coord.y));
	Port *port = closest->portsList;
	for (int i = 0; i < closest->nPorts; i++)
	{
		if (port->status != 3) {
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
			if (port->status == 1) {
				occupiedCount++;
			}
		}
		port = port->next;
	}
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
			return 1;
		}
		head = head->next;
	}
	while(carnode){
		if(strcmp(carnode->car->nLicense,lic) == 0){
			return 1;
		}
		carnode = carnode->next;
	}
	return 0;
}

Station* findStation(Station* root, char lic[9]){
	if (!root)
		return NULL;
	if (isCarInStation(root, lic))
		return root;
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
	printPorts(root->portsList);
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
		printCarsInQ(station->carQueue);
		printCarsInPortList(station->portsList);
	}
}
Station* stationInputByID(StationBST* sBST) {
	int ID;
	char checkInput;
	printf("Enter station ID: ");
	if (scanf("%d%c", &ID, &checkInput) != 2 || checkInput != '\n') { //checks if users enterd an integer and nothing else {if(scanf scanned amount of values other than 2 || the second value scanned is not an enter)}
		displayError(101);
		while (getchar() != '\n'); //clears inputs
		return NULL;
	}
	Station* station = searchStation(sBST->root, &ID, 0);
	if (!station) {
		displayError(404);
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
		station = stationInputByID(sBST);
		return station;
    }
    else {
        printf("Enter station name: ");
        scanf("%[^\n]s", stationName);
		getchar() != '\n';
        station = searchStation(sBST->root, stationName, 1);
		if (!station) {
			displayError(404);
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
			TakenPortsCount++;
		}
		else if(port->status == 3){
			OORportsCount++;
		}
		port = port->next;
	}
	double uRate =((double)TakenPortsCount/ (double)station->nPorts)*100;
	double oorRate =((double)OORportsCount / (double)station->nPorts)*100;
	double loadRate;
	if (station->nCars) {
		loadRate = (double)TakenPortsCount / (double)station->nCars;
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
	station->nPorts++;
	switch (pt)
	{
	case 0:
		port = createPort(station->nPorts, FAST, 2, NULL, date);
		break;
	case 1:
		port = createPort(station->nPorts, MID, 2, NULL, date);
		break;
	case 2:
		port = createPort(station->nPorts, SLOW, 2, NULL, date);
		break;
	default:
		break;
	}
	addPort(station->portsList, port);
	Car* car = searchCarByPortType(station->carQueue, port->portType);
	if (car) {
		assignCar2port(car, port);
		removeCarFromQueue(station->carQueue, car);
		station->nCars--;
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
	int mb;
	while(head){
		mb = minutesBetween(head->tin);
		if(mb > 600){ //charging for over 10 hours
			printf("The car %s is removed from charger\n", head->p2car->nLicense);
			head->p2car->totalPayed += mb * 1.2;
			head->p2car->pPort = NULL;
			head->p2car->totalPayed += minutesBetween(head->tin) * 1.2;
			head->p2car = NULL;
			head->status = 2;
			head->tin.Year = 0;
			head->tin.Month = 0;
			head->tin.Day = 0;
			head->tin.Hour = 0;
			head->tin.Min = 0;
			car = searchCarByPortType(station->carQueue, head->portType);
			if(car){
				assignCar2port(car, head);
				removeCarFromQueue(station->carQueue, car);
				station->nCars--;
			}
		}
		head = head->next;
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
		if(head->status == 3){
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
		if (head->status == 3 && head->num == portNum) {
			removePort(station->portsList,portNum);
			station->nPorts--;
			printf("Port was removed\n");
			return;
		}
		head = head->next;
	}
	displayError(404);
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
	freePortsInStation(station);
	printf("Cars removed from ports\nAll ports deleted\nCars total payed updated\n");
	freeCarQ(station);
	printf("Freed cars from queue\n");
	sBST->root = removeStation(sBST->root, station->id);
	printf("Station was deleted\n");
}


Station* findMinStation(Station* node) {
	while (node && node->left)
		node = node->left;
	return node;
}

Station* removeStation(Station* root, int ID) {
	if (!root)
		return NULL;

	if (ID < root->id) {
		root->left = removeStation(root->left, ID);
	}
	else if (ID > root->id) {
		root->right = removeStation(root->right, ID);
	}
	else {
		if (!root->left) {
			Station* temp = root->right;
			free(root->name);
			free(root);
			return temp;
		}
		else if (!root->right) {
			Station* temp = root->left;
			free(root->name);
			free(root);
			return temp;
		}
	
		Station* temp = findMinStation(root->right);
		root->carQueue = temp->carQueue;
		root->coord = temp->coord;
		root->id = temp->id;
		root->name = temp->name;
		root->nCars = temp->nCars;
		root->nPorts = temp->nPorts;
		root->portsList = temp->portsList;
		root->right = removeStation(root->right, temp->id);
	}
	return root;
}

void freePortsInStation(Station* station){
	Port* port = station->portsList;
	Port* temp;
	if (port) {
		while (port) {
			temp = port->next;
			if (port->p2car) {
				port->p2car->totalPayed += minutesBetween(port->tin) * 1.2;
				port->p2car->pPort = NULL;
			}
			free(port);
			port = temp;
		}
	}
}

void freeCarQ(Station* station) {
	carNode* node = station->carQueue->front;
	carNode* temp;
	if (node) {
		while (node) {
			temp = node->next;
			node->car->inqueue = 0;
			free(node);
			node = temp;
		}
	}
	free(station->carQueue);
	station->carQueue = NULL;
}

void freeAllStations(Station* root){
	if(!root){
		return;
	}
	freeAllStations(root->left);
	freeAllStations(root->right);
	freePortsInStation(root);
	freeCarQ(root);
	free(root->name);
	free(root);
}