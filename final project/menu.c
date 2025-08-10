#define _CRT_SECURE_NO_WARNINGS
#define YELLOW "\033[3m\033[6;33m"
#define GREEN "\033[3;32m"
#define WHITE "\033[0m\033[3;37m"

#include "CarBST.h"
#include "StationBST.h"
#include "displayError.h"
#include "Load.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// prints menu
void Menu()
{
	printf(WHITE);
	printf("please enter the number of the option you want\n"
		"[1] Locate nearest station\n"
		"[2] Charge Car\n"
		"[3] Check for status\n"
		"[4] Stop charge\n"
		"[5] Display all stations\n"
		"[6] Display cars at station\n"
		"[7] Report of stations statistics\n"
		"[8] Display Top customers\n"
		"[9] Add new port\n"
		"[10] Release Charging Ports\n"
		"[11] Remove out of order ports\n"
		"[12] Remove Customer\n"
		"[13] Close station\n"
		"[0] Quit\n"
		GREEN"option: "YELLOW);
}

// actual menu
// operates menu
// reads input from user and shows the function based on the input
void runMenu()
{
	StationBST* sBST = initStationBST();
	CarBST* cBST = initCarBST();
	loadFiles(&cBST, &sBST); // loading files
	int run = 1;
	int showMenu = 1;
	int option = NULL;
	char optionCheck;
	while (run) {
		if (showMenu) {
			Menu();
			showMenu = 0;
			// Input validation for menu selection
			if (scanf("%d%c", &option,&optionCheck) != 2 || optionCheck != '\n') { //checks if users enterd an integer and nothing else {if(scanf scanned amount of values other than 2 || the second value scanned is not an enter)}
				system("cls");
				displayError(101);
				while (getchar() != '\n'); //clears inputs
				showMenu = 1;
			}
			else if (option < 0 || option > 13) { //checks if input is not between [0,13]
				system("cls");
				displayError(200);
				showMenu = 1;
			}

			printf(WHITE);
		}

		else {
			system("cls");
			// Handles the chosen menu option
			switch (option)
			{
			case 0:
				exitSys(sBST,cBST);
				run = 0;
				break;
			case 1:
				locNearSt(sBST);
				break;
			case 2:
				chargeCar(&sBST, cBST);
				break;
			case 3:
				checkCarStatus(sBST, cBST);
				break;
			case 4:
				stopCharge(sBST, cBST);
				break;
			case 5:
				dispAllSt(sBST);
				break;
			case 6:
				dispCarsAtSt(sBST);
				break;
			case 7:
				reportStStat(sBST);
				break;
			case 8:
				dispTopCustomers(cBST);
				break;
			case 9:
				addNewPort(sBST);
				break;
			case 10:
				releasePorts(sBST);
				break;
			case 11:
				remOutOrderPort(sBST);
				break;
			case 12:
				remCustomer(cBST);
				break;
			case 13:
				closeSt(sBST);
				break;
			default:
				break;
			}
			if (option) {
				updateFiles(sBST, cBST);
				printf(YELLOW"Press ENTER to go back"WHITE);
				while (getchar() != '\n');
				system("cls");
				showMenu = 1;
				option = NULL;
			}
		}
	}
}



