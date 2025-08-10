#pragma once
#include "Port.h"


typedef struct Port Port;

// Car Structure
// char nLicense[9] - license number of the car (MUST BE STRING IN LENGTH 9)
// PortType portType - (FAST, MID, SLOW)
// double totalPayed
// Port* pPort - pointer to the port the car is at
// int inqueue - (0 - not in line, 1 - in line)
typedef struct Car {
	char nLicense[9];
	PortType portType;
	double totalPayed;
	Port* pPort;
	int inqueue;
}Car;

// Car node structure
// Car* car - pointer to car
// struct carNode* next - pointer to the next car in queue
typedef struct carNode {
	Car* car;
	struct carNode* next;
}carNode;

// qCar structure (queue manager struct)
// carNode* front - pointer to the car in front of the queue
// carNode* rear - pointer to the car in rear of the queue
typedef struct qCar {
	carNode* front;
	carNode* rear;
}qCar;

// Initializes a new queue of cars
// returns a pointer to the empty queue  (front = NULL, rear = NULL)
qCar* initQCar();

// Creates a new car
// Returns a pointer to the new car
// (char* nLicense, PortType portType, double totalPayed, Port* pPort, int inqueue)
Car* createCar(char* nLicense, PortType portType, double totalPayed, Port* pPort, int inqueue);

// Checks if the queue is empty
// (qCar* q)
int isEmptyCar(qCar* q);

// Enqueues a car to the queue
// (qCar* q, Car* car)
void enqueueCar(qCar* q, Car* car);

// Removes a car from the queue
// (qCar* q)
Car* dequeue(qCar* q);

// searches if a car is already in the queue based on its license
// (qCar* q, char lic[9])
carNode* searchInCarQ(qCar* q, char lic[9]);

// Assigns a car to a charging port and updates the port/car information
// (Car* car, Port* port)
void assignCar2port(Car* car, Port* port);

// Reads car license from user input and validates it
// (char* lic)
void carLicenseInput(char* lic);

// Counts the number of cars with the same port type that are in front of the car in the queue
// (qCar* q, Car* car)
int CarsInFront(qCar* q, Car* car);

// Searches for the first car in the queue that matches a given port type
// returns car address if found (else returns NULL)
// (qCar* q, PortType pt)
Car* searchCarByPortType(qCar* q, PortType pt);

// Removes a specific car from the queue (if it exists)
// (qCar* q, Car* car)
void removeCarFromQueue(qCar* q, Car* car);

// Prints the license number of a car
// (Car* car)
void printCarLic(Car* car);

// Prints all cars in the queue with their port types
// (qCar* q)
void printCarsInQ(qCar* q);

// Prints all cars currently connected to ports(license,Port type,Charging time)
// (Port* head)
void printCarsInPortList(Port* head);