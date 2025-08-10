#define RED "\033[0;31m"
#define WHITE "\033[3;37m"

void displayError(int errorCode) {
	printf(RED); //change error text color to red
	switch (errorCode)
	{
	case 100:
		printf("Type Error.\n");
		break;
	case 101:
		printf("Type Error: expected an integer.\n");
		break;
	case 102:
		printf("Type Error: expected a double.\n");
		break;
	case 103:
		printf("Type Error: expected a char.\n");
		break;
	case 200:
		printf("Value Error: out of range.\n");
		break;
	case 201:
		printf("Value Error: improper license format.\n");
		break;
	case 300:
		printf("Memory Error: Allocation failed.\n");
		break;
	case 400:
		printf("File Error: Failed to load File.\n");
		break;
	case 404:
		printf("404 Error: Not found.\n");
		break;
	default:
		break;
	}
	printf(WHITE);
}