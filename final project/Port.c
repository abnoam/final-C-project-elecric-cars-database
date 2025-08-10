#define _CRT_SECURE_NO_WARNINGS
#include "Port.h"
#include <time.h>
#include <stdio.h>
#include "displayError.h"
#define GREEN "\033[3;32m"
#define WHITE "\033[3;37m"




Port* createPort(int num, PortType portType, int status, Car* p2car,Date date) {
	Port* port = (Port*)malloc(sizeof(Port));
	if (!port) { displayError(300); return NULL; }
	port->num = num;
	port->portType = portType;
	port->status = status;
	port->p2car = p2car;
	port->tin = date;
	port->next = NULL;
	return port;
}

void addPort(Port* head, Port* newPort){
	while (head->next)
	{
		head = head->next;
	}
	head->next = newPort;
}

void removePort(Port* head, int num) {
	Port* prev = head;
	Port* curr = head->next;
	Port* newTemp;
	if (head == NULL)
		return;
	if (head->num == num) {
		Port* temp = head;
		head = head->next;
		free(temp);
		newTemp = head;
	}
	else {
		while (curr != NULL && curr->num != num) {
			prev = curr;
			curr = curr->next;
		}
		if (curr == NULL) {
			return;
		}
		prev->next = curr->next;
		free(curr);
		newTemp = prev->next;
	}
	while(newTemp){
		newTemp->num--;
		newTemp = newTemp->next;
	}
}

Port* searchAvailbleByPT(Port* head, PortType portType) {
	while(head){
		if(head->portType == portType && head->status == 2){
			return head;
		}
		head = head->next;
	}
	return NULL;
}

int minutesBetween(Date inputDate) {
	struct tm tmInput = { 0 };                     // Initialize a struct tm with all fields set to 0
	tmInput.tm_year = inputDate.Year - 1900;       // tm_year is years since 1900, so subtract 1900 from input year
	tmInput.tm_mon = inputDate.Month - 1;          // tm_mon is months since January (0-11), so subtract 1 from input month
	tmInput.tm_mday = inputDate.Day;               // Day of the month (1-31)
	tmInput.tm_hour = inputDate.Hour;              // Hours (0-23)
	tmInput.tm_min = inputDate.Min;                // Minutes (0-59)
	tmInput.tm_isdst = -1;						   // day light save time
	time_t inputTime = mktime(&tmInput);           // Convert struct tm to time_t (seconds since Epoch)
	time_t now = time(NULL);                       // Get current time as time_t (seconds since Epoch)
	double diffSeconds = difftime(now, inputTime); // Calculate difference in seconds between now and inputTime
	int diffMinutes = (int)(diffSeconds / 60);     // Convert seconds difference to whole minutes by dividing by 60
	return diffMinutes;                            // Return the difference in minutes
}

void printPort(Port* port){
	printf("~~~~~~~~~~~~~~~~~[ "GREEN"Port #%d"WHITE" ]~~~~~~~~~~~~~~~~~\n", port->num);
	printf("Port Type    : ");
	switch (port->portType) {
	case SLOW:
		printf("SLOW\n");
		break;
	case MID:
		printf("MID\n");
		break;
	case FAST:
		printf("FAST\n");
		break;
	default:
		break;
	}
	printf("Status       : ");
	switch (port->status) {
	case 1:
		printf("Occupied\n"); 
		break;
	case 2:
		printf("Free\n");
		break;
	case 3:
		printf("Out of Order\n");
		break;
	default:
		break;
	}
	if (port->p2car) {
		printf("Car in Port  : ");
		printCarLic(port->p2car);

		printf("Start Time   : %02d/%d/%d %02d:%02d\n",port->tin.Day, port->tin.Month, port->tin.Year,port->tin.Hour, port->tin.Min);
	}
}

void printPorts(Port* head){
	int countCarsInCharge = 0;
	if (!head) {
		printf("No ports!\n");
		return;
	}
	while (head) {
		printPort(head);
		if (head->p2car) {
			countCarsInCharge++;
		}
		head = head->next;
	}
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("Cars Currently Charging: %d\n", countCarsInCharge);
}


