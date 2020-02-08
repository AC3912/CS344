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
	}
}

//Create room array and fill with struct name and type	
void createRoom(struct room *roomsArray[], char *roomNames[], char *roomType[]){
	int typeOfRoom;
	int nameOfRoom;

	//create array of room names selected 
	int selectedRooms[7] = {0,0,0,0,0,0,0};	
	
	//create the 7 rooms for the game
	int i = 0;
	while(i < 7){
		;
		typeOfRoom = -1;
		roomsArray[i] = malloc(sizeof(struct room));
		
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
	
}

//Check each room in array for number of connections.
//Returns true (1) if a room has 3-6 connections.
//Otherwise, returns false (0).
bool IsGraphFull(struct room *roomArray[]){
	int i;
	for(i = 0; i < 7; i ++){
		if(roomArray[i]->numberOfConnections < 4){
			return false;
		}
	}
	return true;
}

//Returns a random room. Does not validate if connection can be added
//from 2.2 Program Outlining in Program 2
struct room* GetRandomRoom(struct room* roomArray[]){
	int i = rand()%7;
	return roomArray[i];
}

//Returns true if a connection can be added from room x (< 6 connections), false otherwise
//from 2.2 Program Outlining in Program 2
bool CanAddConnectionFrom(struct room *x){
	if(x->numberOfConnections < 6){
		return true;
	}
	else
		return false;
}

//Returns true if a connection from room x to room y already exists, false otherwise
//from 2.2 Program Outlining in Program 2
bool ConnectionAlreadyExists(struct room* x, struct room* y){
	int i;
	for(i = 0; i < 6; i++){
		if(x->connections[i] == NULL){
			return false;
		}
		else if(strcmp(x->connections[i]->name,y->name) == 0){
			return true;
		}
		else{
			return false;
		}
	}
}

//One way connection rooms x and y together. Does not check for valid connection
//from 2.2 Program Outlining in Program 2
void ConnectRoom(struct room *x, struct room *y){
	int c = 0;
	while(x->connections[c] != NULL && c < 6){
		c++;
	}
	x->connections[c] = y;
	x->numberOfConnections++;
}

//Returns true if rooms x and y are the same room, false otherwise
//from 2.2 Program Outlining in Program 2
bool IsSameRoom(struct room *x, struct room *y){
	if(x->name == y->name){
		return true;
	}
	else{
		return false;
	}
}

//Adds a random, valid outbound connection from one room to another room
//from 2.2 Program Outlining in Program 2
void AddRandomConnection(struct room *roomArray[]){
	struct room *x;
	struct room *y;
	
	while(true){
		x = GetRandomRoom(roomArray);
		if(CanAddConnectionFrom(x) == true){
			break;
		}
		
	}

	//Get another room that is not connected to x, can connect to x and is not same
	//room as x. 
	do{
		y  = GetRandomRoom(roomArray);
	}while(CanAddConnectionFrom(y)== false || IsSameRoom(x,y) == true || ConnectionAlreadyExists(x,y) == true);

	//connect the rooms
	ConnectRoom(x,y);
	ConnectRoom(y,x);
}

int main(){
	srand(time(NULL));
	
	//initialize array of rooms
	struct room *roomsArray[7];

	//initialize rooms
	createRoom(roomsArray, roomNames, roomType);
		
	//create graph with connections
	while(IsGraphFull(roomsArray) == false){
		AddRandomConnection(roomsArray);
	}
	
	//create directory
	int processID = getpid();
	char directoryName[32];
	sprintf(directoryName,"choudham.rooms.%d", processID);
	mkdir(directoryName, 0755);
	
	//create file for each room
	FILE *f;
	
	int i = 0;
	while(i < 7){
		char roomFileName[50];
		
		//names the file for each room with the room name
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
