#define _CRT_SECURE_NO_WARNINGS
#include "Car.h"
#include "string.h"
#include "stdio.h"
#include "displayError.h"
#include <time.h>
#define CYAN "\033[3;36m"
#define WHITE "\033[3;37m"


qCar* initQCar() {
	qCar* q = (qCar*)malloc(sizeof(qCar));
	if (!q) { displayError(300); return NULL; }
	q->front = NULL;
	q->rear = NULL;
	return q;
}
Car* createCar(char* nLicense, PortType portType, double totalPayed, Port* pPort, int inqueue) {
	Car* car = (Car*)malloc(sizeof(Car));
	if (!car) { displayError(300); return NULL; }
	strcpy(car->nLicense, nLicense);
	car->portType = portType;
	car->totalPayed = totalPayed;
	car->pPort = pPort;
	car->inqueue = inqueue;
	return car;
}

int isEmptyCar(qCar* q) {
	return (q->front) ? 0 : 1;
}
void enqueueCar(qCar* q,Car* car) {
	carNode* carnode = (carNode*)malloc(sizeof(carNode));
	if (!carnode) { displayError(300); return; }
	carnode->car = car;
	carnode->next = NULL;
	if (!searchInCarQ(q, car->nLicense)) {
		if (isEmptyCar(q))
			q->front = carnode;
		else
			q->rear->next = carnode;
		q->rear = carnode;
	}

}

 Car* dequeue(qCar* q) {
	if (isEmptyCar(q))
		return NULL;
	Car* car = q->front->car;
	carNode* node2Del = q->front;
	q->front = q->front->next;
	free(node2Del);
	return car;
}

 void removeCarFromQueue(qCar* q, Car* car) {
	 if (isEmptyCar(q))
		 return;
	 carNode* curr = q->front;
	 carNode* prev = NULL;
	 while (curr) {
		 if (curr->car == car) {
			 if (prev == NULL) {
				 q->front = curr->next;
			 }
			 else {
				 prev->next = curr->next;
			 }
			 if (curr == q->rear) {
				 q->rear = prev;
			 }
			 curr->car->inqueue = 0;
			 free(curr);
			 return;
		 }
		 prev = curr;
		 curr = curr->next;
	 }
 }

carNode* searchInCarQ(qCar* q,char lic[9]) {
	carNode* node = q->front;
    while(node){
		if(strcmp(node->car->nLicense,lic) == 0) {
			return node;
		}
			node = node->next;
	}
	return node;
}

Car* searchCarByPortType(qCar* q,PortType pt){
	carNode* node = q->front;
	while (node) {
		if (node->car->portType == pt) {
			return node->car;
		}
		node = node->next;
	}
	return NULL;
}

int CarsInFront(qCar* q, Car* car){
	int count = 0;
	carNode* node = q->front;
	while(strcmp(car->nLicense,node->car->nLicense) != 0){
		if(car->portType == node->car->portType){
			count++;
		}
		node = node->next;
	}
	return count;
}

void assignCar2port(Car* car, Port* port) {
	if (car->pPort) {
		car->pPort->p2car = NULL;
		car->pPort->status = 2;
		car->totalPayed += minutesBetween(car->pPort->tin) * 1.2;
		car->pPort->tin.Year = 0;
		car->pPort->tin.Month = 0;
		car->pPort->tin.Day = 0;
		car->pPort->tin.Hour = 0;
		car->pPort->tin.Min = 0;
	}
	port->p2car = car;
	port->status = 1;
	car->pPort = port;
	time_t now = time(NULL);
	struct tm* t = localtime(&now);
	port->tin.Year = t->tm_year + 1900;
	port->tin.Month = t->tm_mon + 1;
	port->tin.Day = t->tm_mday;
	port->tin.Hour = t->tm_hour;
	port->tin.Min = t->tm_min;
	printf("The car %s is now charging in port %d\n", car->nLicense, port->num);
}

void carLicenseInput(char* lic) {
	char carLicense[9], checkInput;
	int digitCount = 0;
	printf("Please enter car license: ");
	if (scanf("%8s%c", carLicense, &checkInput) != 2 || checkInput != '\n' || strlen(carLicense) != 8) {
		while (checkInput != '\n' && getchar() != '\n');
		displayError(201);
		strcpy(lic, "\0");
		return;
	}

	else {
		for (int i = 0; i < strlen(carLicense); i++) {
			if (isdigit(carLicense[i])) {
				digitCount++;
			}
		}
		if (digitCount == 8) {
			strcpy(lic, carLicense);
			return;
		}
		else {
			displayError(201);
			strcpy(lic, "\0");
			return;
		}
	}
}

void printCarLic(Car* car){
	printf("%s\n", car->nLicense);
}

void printCarsInQ(qCar* q) {
	carNode* node = q->front;
	printf("~~~~~~~~~~~[ "CYAN"Cars in queue"WHITE" ]~~~~~~~~~~~\n");
	while (node) {
		printf("car license: %s | ", node->car->nLicense);
		printf("Port type: ");
		switch (node->car->portType) {
		case SLOW:  printf("SLOW\n"); break;
		case MID:   printf("MID\n"); break;
		case FAST:  printf("FAST\n"); break;
		default: break;
		}
		printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		node = node->next;
	}
}

void printCarsInPortList(Port* head){
	printf("~~~~~~~~~~~[ "CYAN"Cars in ports"WHITE" ]~~~~~~~~~~~\n");
	while (head) {
		if (head->p2car) {
			printf("car license: %s | ", head->p2car->nLicense);
			printf("Port type: ");
			switch (head->p2car->portType) {
			case SLOW:  printf("SLOW\n"); break;
			case MID:   printf("MID\n"); break;
			case FAST:  printf("FAST\n"); break;
			default: break;
			}
			printf("Time in charge: %d[Min]\n", minutesBetween(head->tin));
			printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
		}
		
		head = head->next;
	}
}

