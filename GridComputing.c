/* Matthew Proetsch
 * COP 3502H - BA 212 9:00am
 * GridComputing.c
 * Parse user data from input file "GridComputing.in", which represents
 * anaylzed raw data from satellite probes from NASA
 * Once analyzed, determine discovered planets' statuses and output to
 * "GridComputing.out"
 */

 #include <stdio.h>
 #include <stdlib.h>
 #include <math.h>
 #include <string.h>

 #define FILENAME_IN "GridComputing.in"
 #define FILENAME_OUT "GridComputing.out"

 typedef struct GeocentricCelestialReferenceFrame {
	 int x;
	 int y;
	 int z;
 } GCRF;

 typedef struct DiscoveredPlanet {
	 int usersConfirmed;
	 int type;

	 GCRF *coordinates;
	 struct DiscoveredPlanet *next;
 } DiscPlanet;

 typedef struct VerifiedPlanet {
	 int type;
	 GCRF *coordinates;

	 struct VerifiedPlanet *next;
 } VerPlanet;

 typedef struct UserWorkCompleted {
	 char firstName[20];
	 char lastName[20];
	 int gbAnalyzed;
	 int planetsDiscovered;

	 struct UserWorkCompleted *next;
 } UserWork;

 typedef enum {
	 NONE = 0,
	 GAS = 1,
	 ICE = 2,
	 STORM = 3,
	 BARREN = 4,
	 TEMPERATE = 5,
	 LAVA = 6,
	 OCEANIC = 7,
	 PLASMA = 8,
	 UNKNOWN = 9,
 } Planets;

//Returns the distance (in kiloparsecs, of course) of arg 0 from Eart
double DistanceFromEarth(VerPlanet *);

//Insert arg 1 into the list pointed to by arg 0
void InsertWorker(UserWork **, UserWork *);

//Append arg 1 to the list of discovered planets pointed to by arg 0 in no particular order
void AppendDiscovered(DiscPlanet **, DiscPlanet *);

//Remove arg 1 from the list of discovered planets pointed to by arg 0 in no particular order
void RemoveDiscovered(DiscPlanet **, DiscPlanet *);

//Insert a verified planet arg 1 into the list pointed to by arg 0
void InsertVerified(VerPlanet **, DiscPlanet *);

int main() {

	FILE *fin = fopen(FILENAME_IN, "r");


	if(fin == NULL) {
		perror(FILENAME_IN);
		return -1;
	}


	char numEntries[5];
    fscanf(fin, "%s", numEntries);
    int intEntries = atoi(numEntries);

	//Initilialize our work list and planet list heads
	//We keep track of our lists with double ptrs
	UserWork **workHead = (UserWork **)malloc(sizeof(UserWork *));
	*workHead = NULL;

	DiscPlanet **discHead = (DiscPlanet **)malloc(sizeof(DiscPlanet *));
	*discHead = NULL;

	VerPlanet **verHead = (VerPlanet **)malloc(sizeof(VerPlanet *));
	*verHead = NULL;

	int i;
	for(i = 0; i < intEntries; i++) {
        //Store every entry in a new UserWork node
		//or update if such a node already exists

		char userLastName[20];
		fscanf(fin, "%s", userLastName);
		char userFirstName[20];
		fscanf(fin, "%s", userFirstName);
		int userGBAnalyzed;
		fscanf(fin, "%d", &userGBAnalyzed);

		int userPlanetType;
		fscanf(fin, "%d", &userPlanetType);


		if(userPlanetType == NONE) {
			//Do nothing; jump down to user info update section

		}

		else if(userPlanetType > 0) {
		    //We have found a planet: First, see if it is one we already know about, and if it is, see if we have verified it
		    //If it isn't one that we know about, add it to the unsorted list of discovered planets

			GCRF *userPlanetCoords = (GCRF *)malloc(sizeof(GCRF));
			int ux, uy, uz;
			fscanf(fin, "%d %d %d", &ux, &uy, &uz);

			userPlanetCoords->x = ux;
			userPlanetCoords->y = uy;
			userPlanetCoords->z = uz;

			int planetFoundInList = 0;
			DiscPlanet *currentDisc;

			for(currentDisc = *discHead; currentDisc != NULL; currentDisc = currentDisc->next) {
				//search for planet in our list of discovered planets

				if( (userPlanetCoords->x == currentDisc->coordinates->x) &&
					(userPlanetCoords->y == currentDisc->coordinates->y) &&
					(userPlanetCoords->z == currentDisc->coordinates->z) ) {

					planetFoundInList = 1;
					currentDisc->usersConfirmed += 1;

					if(currentDisc->usersConfirmed > 2) {

						InsertVerified(verHead, currentDisc);
						RemoveDiscovered(discHead, currentDisc);

					}
				}
			}
			if(!planetFoundInList) {
				//Discovered planet not yet in list of discovered planets

				DiscPlanet *newDisc = (DiscPlanet *)malloc(sizeof(DiscPlanet));

				newDisc->coordinates = userPlanetCoords;
				newDisc->type = userPlanetType;
				newDisc->usersConfirmed = 1;
				newDisc->next = NULL;

				AppendDiscovered(discHead, newDisc);
				}

			}

			UserWork *currentUser = *workHead;
			int userFound = 0;

			for( ; currentUser != NULL; currentUser = currentUser->next) {
				//Check to see if the user exists in the list of workers

				if((strcmp( currentUser->lastName, userLastName )) == 0 &&
				   (strcmp( currentUser->firstName, userFirstName )) == 0) {

					userFound = 1;
					currentUser->gbAnalyzed += userGBAnalyzed;
					if (userPlanetType > 0) {
						currentUser->planetsDiscovered += 1;
					}
				}
			}

			if(!userFound) {
				//User not found in list pointed to by workHead:
				//Fill in appropriate fields and insert into the worker list

				UserWork *newWorker = (UserWork *)malloc(sizeof(UserWork));

				strcpy(newWorker->firstName, userFirstName);
				strcpy(newWorker->lastName, userLastName);
				newWorker->gbAnalyzed = userGBAnalyzed;
				newWorker->next = NULL;

				if(userPlanetType > 0) {
					newWorker->planetsDiscovered = 1;
				} else {
					newWorker->planetsDiscovered = 0;
				}

				InsertWorker(workHead, newWorker);
			}

		}
		
	FILE *fout = fopen(FILENAME_OUT, "w");

	fprintf(fout, "Users:\n");
 
	UserWork *curr;
	for(curr = *workHead; curr != NULL; curr = curr->next) {
		fprintf(fout, "\t%s, %s - Analyzed:%d Discovered:%d\n", curr->lastName, 
																curr->firstName,
																curr->gbAnalyzed,
																curr->planetsDiscovered);

	}

	fprintf(fout, "\nDiscovered planets:\n");

	DiscPlanet *currD;
	for(currD = *discHead; currD != NULL; currD = currD->next) {
        fprintf(fout, "\t(%d,%d,%d) - Confirmed:%d Type:", currD->coordinates->x,
															   currD->coordinates->y,
															   currD->coordinates->z,
															   currD->usersConfirmed);

        if(currD->type == BARREN) fprintf (fout, "BARREN");
        if(currD->type == GAS) fprintf (fout, "GAS");
        if(currD->type == ICE) fprintf(fout, "ICE");
        if(currD->type == LAVA) fprintf(fout, "LAVA");
        if(currD->type == OCEANIC) fprintf(fout, "OCEANIC");
        if(currD->type == PLASMA) fprintf(fout, "PLASMA");
        if(currD->type == STORM) fprintf(fout, "STORM");
        if(currD->type == TEMPERATE) fprintf(fout, "TEMPERATE");
        if(currD->type == UNKNOWN) fprintf(fout, "UNKNOWN");

        fprintf(fout, "\n");
	}

    fprintf(fout, "\nVerified planets:\n");

    VerPlanet *currV;
    for(currV = *verHead; currV != NULL; currV = currV->next) {
        fprintf(fout, "\t(%d,%d,%d) - Type:", currV->coordinates->x,
                                        currV->coordinates->y,
                                        currV->coordinates->z);

        if(currV->type == BARREN) fprintf(fout, "BARREN");
        if(currV->type == GAS) fprintf(fout, "GAS");
        if(currV->type == ICE) fprintf(fout, "ICE");
        if(currV->type == LAVA) fprintf(fout, "LAVA");
        if(currV->type == OCEANIC) fprintf(fout, "OCEANIC");
        if(currV->type == PLASMA) fprintf(fout, "PLASMA");
        if(currV->type == STORM) fprintf(fout, "STORM");
        if(currV->type == TEMPERATE) fprintf(fout, "TEMPERATE");
        if(currV->type == UNKNOWN) fprintf(fout, "UNKNOWN");

        fprintf(fout, "\n");
    }

	fclose(fin);
	fclose(fout);
	
	return 0;
	

}


double DistanceFromEarth(VerPlanet *planet) {
	//Square each element of the planet's GCRF element, add, and take square root

	return sqrt( ( pow((double)planet->coordinates->x, 2) ) +
				 ( pow((double)planet->coordinates->y, 2) ) +
				 ( pow((double)planet->coordinates->z, 2) ) );
}


void InsertWorker(UserWork **head, UserWork *newWorker) {
	//Insert a worker alphabetically, last name takes prominence over first name

	UserWork *current = *head;
	UserWork *prev = current;

	if(current == NULL) {
		//Assign newWorker to head of the list
		*head = newWorker;
		return;
	}
	else if(strcmp(newWorker->lastName, (*head)->lastName) < 0) {
		//newWorker comes before head by last name, so it belongs at the front
		newWorker->next = *head;
		*head = newWorker;
		return;
	}
	else if(strcmp(newWorker->lastName, (*head)->lastName) == 0) {
		//determine head of the list by first names
		if(strcmp(newWorker->firstName, (*head)->firstName) < 0) {
			//newWorker comes first
			newWorker->next = *head;
			*head = newWorker;
			return;
		}
	}

	//if newWorker doesn't belong at head, decide where it belongs

	for( ; current->next != NULL; current = current->next) {

		if(strcmp(newWorker->lastName, current->lastName) < 0) {

			newWorker->next = current;
			prev->next = newWorker;
			return;
			
		} else if(strcmp(newWorker->lastName, current->lastName) == 0) {
			//matching last names, so we must sort by first names

			if(strcmp(newWorker->firstName, current->firstName) < 0) {
				//newWorker's first name comes before current's first name

				newWorker->next = current;
				prev->next = newWorker;
				return;

			}
			else if(strcmp(newWorker->firstName, current->firstName) > 0) {
				//unable to place newWorker here, and we don't know how long before
				//newWorker's firstName will come next alphabetically, so we just
				//advance our pointer for now
				continue;
				
			}

			//It should never be the case that both last names and first names match
		}

		//advance prev pointer and continue
		prev = current;
	}
	
	//we're now dealing with the final element in the list
	//since we broke out of the for-loop with condition "current->next == NULL"
	
	if(strcmp(newWorker->lastName, current->lastName) < 0) {
		newWorker->next = current;
		prev->next = newWorker;
		return;
	
	} else if(strcmp(newWorker->lastName, current->lastName) > 0) {
		current->next = newWorker;
		return;
	
	} else {
		if(strcmp(newWorker->firstName, current->firstName) < 0) {
			newWorker->next = current;
			prev->next = newWorker;
			return;
		} else {
			current->next = newWorker;
			return;
		}
		
	}
}


void AppendDiscovered(DiscPlanet **discHead, DiscPlanet *newDisc) {

	DiscPlanet *current = *discHead;

	if(*discHead == NULL) {
		//make discHead point to newDisc in order to make a new list

		*discHead = newDisc;
		return;
	}

	for( ; current->next != NULL; current = current->next) {
		//Do nothing, this loop exists only to progress to the end of the list
	}

	current->next = newDisc;

	return;
}


void RemoveDiscovered(DiscPlanet **discHead, DiscPlanet *remove) {
	//Remove "remove" from list pointed to by "discHead"

	DiscPlanet *current = *discHead;
	DiscPlanet *prev = current;

	if( ((*discHead)->coordinates->x == remove->coordinates->x) &&
		((*discHead)->coordinates->y == remove->coordinates->y) &&
		((*discHead)->coordinates->z == remove->coordinates->z) ) {

		//Remove the head of the list by making the head pointer point to the next element

		*discHead = (*discHead)->next;
		return;
	}

	for( ; current != NULL; current = current->next) {

		if( (current->coordinates->x == remove->coordinates->x) &&
			(current->coordinates->y == remove->coordinates->y) &&
			(current->coordinates->z == remove->coordinates->z) ) {
			//Remove current element

			prev->next = current->next;
			free(current);

			return;

		}

		else {

			prev = current;
		}

	}
}


void InsertVerified(VerPlanet **verHead, DiscPlanet *newVerTemp) {
	//Insert verified planet in appropriate list position in order of distance from Earth
	//first we must convert newVerTemp to a VerPlanet structure

	VerPlanet *newVer = (VerPlanet *)malloc(sizeof(VerPlanet));

	newVer->coordinates = newVerTemp->coordinates;
	newVer->type = newVerTemp->type;
	newVer->next = NULL;

	VerPlanet *current = *verHead;
	VerPlanet *prev = current;

	if(*verHead == NULL) {
		//Make a new list for verified planets

		(*verHead) = newVer;
		return;
	}
	else if(DistanceFromEarth(newVer) < DistanceFromEarth(*verHead)) {
        //Need to replace list head

        newVer->next = *verHead;
        *verHead = newVer;
	}

	else {
		//Insert newVer in the list of verified planets pointed to by verHead
		//according to its distance from the earth

		for(current = *verHead; current->next != NULL; current = current->next) {

			if(DistanceFromEarth(newVer) > DistanceFromEarth(current)) {
				//Do nothing until we reach a planet that is further from earth than newVer

				prev = current;
				continue;
			}

			else if(DistanceFromEarth(newVer) <= DistanceFromEarth(current)) {
				//insert newVer[ifiedPlanet] into the list before that planet
				
                newVer->next = current;
                prev->next = newVer;
				return;
			}

		}
		
		//we are at the final planet in the list
		if(DistanceFromEarth(newVer) < DistanceFromEarth(current)) {
			newVer->next = current;
			prev->next = newVer;
			
		} else {
			current->next = newVer;
		}
	}
	return;
}
