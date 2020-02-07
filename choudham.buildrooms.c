/*Amena Choudhury
 *2/7/2020
 *Creates a directory called choudham.rooms.<pid>
 *Generates 7 different file names containing 1 room per file
 *in choudham.rooms.<pid> directory
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//assigns roomType to each room
char *roomType[3] = {"START_ROOM", "END_ROOM", "MID-ROOM"};
//hard coded list of Room Names
char *roomNames[10] = {"Azkaban", "Basilisk", "Centaurs", "Dementor", "Evanesco",
			"Firebolt", "Kneazle", "Marauder", "Phoenix", "Pensieve"};

//create bool variable
//https://stackoverflow.com/questions/1921539/using-boolean-values-in-c
typedef enum { false, true } bool;

//define room data type
struct room{
	char *name;
	char *type;
	//int connections[6];
	//pointer to connected rooms
	struct room* connections[6];
	int numberOfConnections;
};

//initialize room
void initializeRoom(struct room *Room, char *name, char *type){
	Room->name = name;
	Room->type = type;
	Room->numberOfConnections = 0;

	int i = 0;
	for(i = 0; i < 7; i++){
		Room->connections[i] = NULL;
		printf("Connections set to NULL\n"); fflush(stdout); //comment out
	}
}

//Create room array and fill with struct name and type	
void createRoom(struct room *roomsArray[], char *roomNames[], char *roomType[]){
	printf("Entering createRoom function\n"); fflush(stdout); //comment out
	int typeOfRoom;
	int nameOfRoom;
	//create array of rooms selected 
	int selectedRooms[7] = {0,0,0,0,0,0,0};	
	printf("Initialized array of rooms\n"); fflush(stdout); //comment out

	//create the 7 rooms for the game
	int i;
	while(i < 7){
		printf("Entered while loop for createRoom function.\n"); fflush(stdout);
		typeOfRoom = -1;
		//roomsArray[i] = malloc(sizeof(struct room));
		//if(roomsArray[i] == NULL){
		//	printf("Memory not allocated.\n"); fflush(stdout);
		//}
		//else
		//	printf("Memory successfully allocated using malloc.\n"); fflush(stdout);
		//1st room is start room. last room is end room.
		//all others are mid room
		if(i == 0){
			typeOfRoom = 0;
		}
		else if(i == 6){
			typeOfRoom = 1;
		}
		else{	
			typeOfRoom = 2;
		}
		printf("typeofRoom set. size of rooms set.\n"); fflush(stdout);
		//select room names randomly
		do{
			nameOfRoom = rand() % 9;
		}while(selectedRooms[nameOfRoom] == 1);
		//update array
		selectedRooms[nameOfRoom] = 1;
		//initialize room
		initializeRoom(roomsArray[i], roomNames[nameOfRoom], roomType[typeOfRoom]);
		i++;
	}
	printf("Exiting createRoom function.\n"); fflush(stdout);
}

//Check each room in array for number of connections.
//Returns true (1) if a room has 3-6 connections.
//Otherwise, returns false (0).
bool IsGraphFull(struct room *roomArray[]){
	printf("Starting check for IsGraphFull\n"); fflush(stdout); //comment out
	int i;
	printf("Entering for loop for IsGraphFull\n"); fflush(stdout); //comment out
	for(i = 0; i < 7; i ++){
		printf("Entered for loop for IsGraphFull\n"); fflush(stdout); //comment out
		printf("number of Connections: %d\n", roomArray[i]->numberOfConnections); fflush(stdout); //comment out
		if(roomArray[i]->numberOfConnections < 4){
			printf("Connections not > 3\n"); fflush(stdout); //comment out
			return false;
		}
	}
	return true;
}

//Returns a random room. Does not validate if connection can be added
//from 2.2 Program Outlining in Program 2
struct room* GetRandomRoom(struct room* roomArray[]){
	printf("Entered GetRandomRoom function\n"); fflush(stdout); //comment out
	int i = rand()%7;
	printf("Room integer is: %d\n", i); fflush(stdout); //comment out
	printf("Exiting GetRandomRoom function.\n"); fflush(stdout); //comment out
	return roomArray[i];
}

//Returns true if a connection can be added from room x (< 6 connections), false otherwise
//from 2.2 Program Outlining in Program 2
bool CanAddConnectionFrom(struct room *x){
	printf("Entered CanAddConnectionFrom function\n"); fflush(stdout); //comment out
	printf("number of Connections: %d\n", x->numberOfConnections); fflush(stdout); //comment out
	if(x->numberOfConnections < 6){
		return true;
	}
	else
		return false;
}

//Returns true if a connection from room x to room y already exists, false otherwise
//from 2.2 Program Outlining in Program 2
bool ConnectionAlreadyExists(struct room* x, struct room* y){
	printf("Entered ConnectionAlreadyExists function.\n"); fflush(stdout);
	int i;
	for(i = 0; i < 6; i++){
		printf("Entered for loop for ConnectionAlreadyExists.\n"); fflush(stdout);
		printf("Room2 name: %s\n", y->name); fflush(stdout);
		printf("Room1 name: %s\n", x->connections[i]);
		if(x->connections[i] == NULL){
			return false;
		}
		else if(strcmp(x->connections[i]->name,y->name) == 0){
			printf("Connection already exists. true.\n"); fflush(stdout);
			return true;
		}
		else{
			printf("Connnection already exists. false.\n"); fflush(stdout);
			return false;
		}
	}
}

//One way connection rooms x and y together. Does not check for valid connection
//from 2.2 Program Outlining in Program 2
void ConnectRoom(struct room *x, struct room *y){
	x->connections[x->numberOfConnections] = y;
	x->numberOfConnections++;
}

//Returns true if rooms x and y are the same room, false otherwise
//from 2.2 Program Outlining in Program 2
bool IsSameRoom(struct room *x, struct room *y){
	printf("Entered IsSameRoom function.\n"); fflush(stdout);
	if(x->name == y->name){
		printf("Exiting IsSameRoom. true.\n"); fflush(stdout);
		return true;
	}
	else{
		printf("Exiting IsSameRoom. false.\n"); fflush(stdout);
		return false;
	}
}

//Adds a random, valid outbound connection from one room to another room
//from 2.2 Program Outlining in Program 2
void AddRandomConnection(struct room *roomArray[]){
	struct room *x;
	struct room *y;
	printf("starting to add a random connection\n"); fflush(stdout);
	//Get a random room that can add a connection
	/*do{
		printf("Entered do 1st while loop for AddRandomConnection\n"); fflush(stdout);
		x = GetRandomRoom(roomArray);
	}while(CanAddConnectionFrom(x) == true);*/
	while(true){
		printf("Entered while loop for AddRandomConnection\n"); fflush(stdout);
		x = GetRandomRoom(roomArray);

		if(CanAddConnectionFrom(x) == true){
			break;
		}
	}

	//Get another room that is not connected to x, can connect to x and is not same
	//room as x. 
	printf("Entering 2nd do while loop in AddRandomConnection\n"); fflush(stdout);
	do{
		y  = GetRandomRoom(roomArray);
	}while(CanAddConnectionFrom(y)== false || IsSameRoom(x,y) == true || ConnectionAlreadyExists(x,y) == true);

	printf("Connecting the rooms\n"); fflush(stdout);
	//connect the rooms
	ConnectRoom(x,y);
	ConnectRoom(y,x);
	printf("AddRandomConnection function complete; rooms connected\n"); fflush(stdout);
}

int main(){
	printf("Starting main\n");fflush(stdout);  //comment out 
	srand(time(NULL));
	struct stat st;
	//initialize array of rooms
	struct room *roomsArray[7];
	int a;
	for(a = 0; a < 7; a++){
		roomsArray[a]= (struct room *)malloc(sizeof(struct room));
	}	
	printf("array of rooms created\n"); fflush(stdout); //comment out 
	//initialize rooms
	createRoom(roomsArray, roomNames, roomType);
		
	printf("createRoom function finished running\n"); fflush(stdout); //comment out 
	//create graph with connections
	while(IsGraphFull(roomsArray) == false){
		AddRandomConnection(roomsArray);
	}
	printf("IsGraphFull function finished running\n"); fflush(stdout); //comment out

	//create directory
	int processID = getpid();
	char directoryName[32];
	sprintf(directoryName,"choudham.rooms.%d", processID);
	mkdir(directoryName, 0755);
	//int result = mkdir(directoryName, 0755);
	//if(stat(directoryName, &st) == -1){
	//	mkdir(directoryName, 0755);
	//}
	printf("Directory created\n"); fflush(stdout);

	//create file for each room
	FILE *f;
	
	int i = 0;
	while(i < 7){
		char roomFileName[50];
		//memset(directoryName, 0, sizeof(directoryName));
		//names the file for each room with the room name
		//sprintf(directoryName, "./choudham.rooms.%d/%s", processID, roomsArray[i]->name);
		sprintf(directoryName, "./choudham.rooms.%d/%s", processID, roomsArray[i]->name);
		//open the created file
		f = fopen(directoryName, "w+");
		//check to make sure file open
		if (f == NULL){
			printf("File could not be opened.");
		}
		//print room name to file
		fprintf(f, "ROOM NAME: %s\n", roomsArray[i]->name);
		//print all connections to room to file
		int k = 1;
		while(k < roomsArray[i]->numberOfConnections){
			fprintf(f, "CONNECTION %d: %s\n", k,roomsArray[i]->connections[k]->name);
		k++;
		}
		//print room type to file
		fprintf(f, "ROOM TYPE: %s", roomsArray[i]->type);
		i++;
		fclose(f);
	}
	return 0;
}
