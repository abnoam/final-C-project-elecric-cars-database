#pragma once
#include <stdlib.h>




typedef struct Car Car;

typedef enum PortType
{
	FAST, MID, SLOW
}PortType;

// Date structure
// int Year
// int Month
// int Day
// int Hour
// int Min
typedef struct Date {
	int Year;
	int Month;
	int Day;
	int Hour;
	int Min;
}Date;

// Port structure 
// int num - port number
// PortType portType - (FAST, MID, SLOW)
// int status - (1 - occupied, 2 - free, 3 - out of order)
// Car* p2car - pointer to car in port
// Date tin - date of start of charging
// Port* next - pointer to the next port in linked list
typedef struct Port {
	int num;
	PortType portType;
	int status; 
	Car* p2car;
	Date tin;
	struct Port* next;
}Port;

// Creates a new port
// Returns a pointer to the new port
// (int num, PortType portType, int status, Car* p2car, Date date)
Port* createPort(int num, PortType portType, int status, Car* p2car, Date date);

// Adds a port to the linked list of ports
// (Port* head, Port* newPort)
void addPort(Port* head, Port* newPort);

// Removes a port from the linked list of ports by port number
// Returns a pointer to the linked list head
// (Port* head, int num)
void removePort(Port** head, int num);

// searches available port by the port type - (FAST, MID, SLOW)
// returns port if found and available (status: free)
// (Port* head, PortType portType)
Port* searchAvailbleByPT(Port* head, PortType portType);

// Calculates the number of minutes between the given date and the current system time
// Returns Difference in minutes
// (Date inputDate)
int minutesBetween(Date inputDate);

// Prints the list of all ports and their details(Port number, type, status)
// If occupied: prints the car license and charging start time
// Also counts and displays total number of charging cars
// (Port* head)
void printPorts(Port* head);

void printPort(Port* port);
