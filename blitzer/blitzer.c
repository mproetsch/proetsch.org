#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <xdo.h>


//build with:
//gcc -o blitzer.test `pkg-config --cflags --libs gl` `imlib2-config --libs` -lglut -lxdo blitzer.c

#define DEPTH      4
#define DEPTH8     0x00e0e0c0
#define DEPTH12    0x00f0f0f0
#define DEPTH16    0x00f1f1f1
#define DEPTH24    0x00ffffff
#define DEPTH32    0xffffffff


#define SIDE_LEN 8
#define NUM_GEMS 7
#define WAIT_TIME_uS 4000
#define SLEEP_TIME_uS 0


#define XOFFSET 343
#define YOFFSET 335
#define SPACING 39 //Represents square sides in pixels of the Bejeweled grid

typedef enum GEMCOLOR {
	BLUE = 0,
	ORANGE,
	GREEN,
	WHITE,
	YELLOW,
	RED,
	PURPLE 
} GEMCOLOR;


char** findMatchHorizontal(int [][SIDE_LEN], int);
char** findMatchVertical(int [][SIDE_LEN], int);
int inBounds(int, int);
void get_pixel_color(Display*, Window, int, int, XColor*);
int findGemColor(XColor *c);

void main(int argc, char *argv[]) {
	
	Display *dpy = XOpenDisplay(NULL);
	Window root = DefaultRootWindow(dpy);
	const struct xdo_t *newdo = xdo_new_with_opened_display(dpy, ":0", 0);
	
	while(1) {
	
	int GameGrid[SIDE_LEN][SIDE_LEN];
	
	
	int i = 0, j = 0;
	int y = YOFFSET, x = XOFFSET;
	
	//Move from the corner of the grid to the center of the first square
	//also some tweaks for FF 10.0.2 under ArchLinux 3.2.7-1 YMMV
	y += (SPACING / 2) + 1;
	x += 19;
	
	for(i = 0; i < SIDE_LEN; i++) {
		for(j = 0; j < SIDE_LEN; j++) {
			
			XColor c;
			get_pixel_color(dpy, root, x, y, &c);
			int color = findGemColor(&c);
			GameGrid[i][j] = color;
			printf("(%d, %d, %d) ", c.red, c.green, c.blue);
			
			x += SPACING;
			
		}
		printf("\n\n");
		x = XOFFSET + (SPACING / 2);
		y += SPACING;
	}
	
    
    /* Randomly generated game grid
     * 
    int GameGrid[SIDE_LEN][SIDE_LEN]; 
    int i, j;
    for(i = 0; i < SIDE_LEN; i++) {
        for(j = 0; j < SIDE_LEN; j++) {
            GameGrid[i][j] = rand() % 7;
        }
    }
    **/
    

    
    for(i = 0; i < SIDE_LEN; i++) {
        for(j = 0; j < SIDE_LEN; j++) {
            printf("%d ", GameGrid[i][j]);
        }
        printf("\n");
    }
    
    
    
    char **firstMatchFound;
    firstMatchFound = findMatchHorizontal(GameGrid, SIDE_LEN);
    
    if(firstMatchFound[0][0] == '\0') {
		firstMatchFound = findMatchVertical(GameGrid, SIDE_LEN);
	}
		
	if(firstMatchFound[0][0] != '\0') {
    
	    char matchy[2], matchx[2];
	    matchy[0] = firstMatchFound[0][0];
	    matchy[1] = '\0';
	    
	    matchx[0] = firstMatchFound[0][1];
	    matchx[1] = '\0';
	    
	    int MatchYint = atoi(matchy);
	    int MatchXint = atoi(matchx);
	    
	    printf("Match found! Offset [%d][%d] by moving %s\n", MatchYint, MatchXint,
	                                                          firstMatchFound[1]);
	                                                          
	    xdo_mousemove(newdo, XOFFSET, YOFFSET, 0);
	    xdo_mousemove_relative(newdo, SPACING/2, SPACING/2);
	    xdo_mousemove_relative(newdo, MatchXint*SPACING, MatchYint*SPACING);
	    
	    Window *mWindow = NULL;
	    xdo_mousewindow(newdo, mWindow);
	    xdo_mousedown(newdo, mWindow, 1);
	    
	    if(strcmp(firstMatchFound[1], "up") == 0) {
			int z = 0;
			for(z; z > -10; z--) {
				xdo_mousemove_relative(newdo, 0, z);
				usleep(WAIT_TIME_uS);
			}
			
				
		} else if(strcmp(firstMatchFound[1], "left") == 0) {
			int z = 0;
			for(z; z > -10; z--) {
				xdo_mousemove_relative(newdo, z, 0);
				usleep(WAIT_TIME_uS);
			}
				
		} else if(strcmp(firstMatchFound[1], "down") == 0) {
			int z = 0;
			for(z; z < 10; z++) {
				xdo_mousemove_relative(newdo, 0, z);
				usleep(WAIT_TIME_uS);
			}
				
		} else if(strcmp(firstMatchFound[1], "right") == 0) {
			int z = 0;
			for(z; z < 10; z++) {
				xdo_mousemove_relative(newdo, z, 0);
				usleep(WAIT_TIME_uS);
			}
		}
		xdo_mouseup(newdo, mWindow, 1);	
	}
	

	
	usleep(SLEEP_TIME_uS);
	
	free(firstMatchFound[0]);
	free(firstMatchFound[1]);
	free(firstMatchFound);
			
	}

                                         
}

int inBounds(int a, int b) {
    if (a < SIDE_LEN && b < SIDE_LEN && a > 0 && b > 0) return 1;
    else return 0;
}


char** findMatchHorizontal(int grid[SIDE_LEN][SIDE_LEN], int size) {
	
    //Grid diagrams in comments always match for x being the gem we are searching for
    //i.e. in the following diagram x is the gem we look for and needs to be moved from
    //[2][0] to [1][0] so we store the initial position in switchMe and the direction
    //in direction
    /*    j012
     *   i ___ 
     *   0|ooo
     *   1|oxx
     *   2|xoo
     */

     
    int current = 0;
    int i, j;
 
    
    char *switchMe = (char *)malloc(sizeof(char) * 3); //for grid coords
    char *direction = (char *)malloc(sizeof(char) * 6); //for move direction
    
	sprintf(switchMe, "\0\0");
	sprintf(direction, "\0\0\0\0\0\0");
    
    char **keyValuePair = (char **)calloc(sizeof(char*), 2);
    
    
    keyValuePair[0] = switchMe;
    keyValuePair[1] = direction;

    //Horizontal passes only! (for now)
    for(current; current < NUM_GEMS; current++) {
        
        for(i = 0; i < size; i++) {
            
            for(j = 0; j < size; j++) {
                //Column-by-column
                if(grid[i][j] == current) {
                    
                    if(inBounds(i, j+2)) {
                        if(grid[i][j+2] == current) {
                                                        
                            /* we will match these grid sections: (where x is current)
                             * 
                             * oxo      ooo 
                             * xox  and xox
                             * ooo      oxo */
                             
                             if(inBounds(i-1, j+1)) {
                                 if(grid[i-1][j+1] == current) {
                                     /* oxo
                                      * xox
                                      * ooo */
                                     sprintf(switchMe, "%d%d",
                                             i-1,
                                             j+1);
                                     sprintf(direction, "down");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                 }
							}
                             
                            if(inBounds(i+1, j+1)) {
                                if(grid[i+1][j+1] == current) {
                                    /* ooo
                                     * xox
                                     * oxo */
                                     sprintf(switchMe, "%d%d",
                                             i+1,
                                             j+1);
                                     sprintf(direction, "up");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }
                        }
                    }
                      
                    if(inBounds(i, j+1)) {
                        if(grid[i][j+1] == current) {
                            /* Match grids that have two gems in a row
                             * 
                             * xoo oooo ooo  the first three cases deal with
                             * oxx xoxx oxx  j-1s or j-2s, or where the far-left gem
                             * ooo oooo xoo  must be moved to match the other two
                             * 
                             * oox oooo ooo  the second three cases deal with j+1s
                             * xxo xxox xxo  and j+2s, or where the far-right gem
                             * ooo oooo oox  must be moved in order to match
                             **/
                               
                            if(inBounds(i-1, j-1)) {
                                if(grid[i-1][j-1] == current) {
                                    /* xoo
                                     * oxx
                                     * ooo */
                                     sprintf(switchMe, "%d%d",
                                             i-1,
                                             j-1);
                                     sprintf(direction, "down");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }   
                               
                            if(inBounds(i, j-2)) {
                                if(grid[i][j-2] == current) {
                                    /* oooo
                                     * xoxx
                                     * oooo */
                                     sprintf(switchMe, "%d%d",
                                             i,
                                             j-2);
                                     sprintf(direction, "right");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }
                                
                            if(inBounds(i+1, j-1)) {
                                if(grid[i+1][j-1] == current) {
                                    /* ooo
                                     * oxx
                                     * xoo */
                                     sprintf(switchMe, "%d%d",
                                             i+1,
                                             j-1);
                                     sprintf(direction, "up");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }
                                 
                            if(inBounds(i-1, j+2)) {
                                if(grid[i-1][j+2] == current) {
                                    /* oox
                                     * xxo
                                     * ooo */
                                     sprintf(switchMe, "%d%d",
                                             i-1,
                                             j+2);
                                     sprintf(direction, "down");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }
                                  
                            if(inBounds(i, j+3)) {
                                if(grid[i][j+3] == current) {
                                    /* oooo
                                     * xxox
                                     * oooo */
                                     sprintf(switchMe, "%d%d",
                                             i,
                                             j+3);
                                     sprintf(direction, "left");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                     }
                            }
                                   
                            if(inBounds(i+1, j+2)) {
                                if(grid[i+1][j+2] == current) {
                                    /* ooo
                                     * xxo
                                     * oox */
                                     sprintf(switchMe, "%d%d",
                                             i+1,
                                             j+2);
                                     sprintf(direction, "up");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return keyValuePair;
}

char** findMatchVertical(int grid[SIDE_LEN][SIDE_LEN], int size) {
	
    //Grid diagrams in comments always match for x being the gem we are searching for
    //i.e. in the following diagram x is the gem we look for and needs to be moved from
    //[2][0] to [1][0] so we store the initial position in switchMe and the direction
    //in direction
    /*    j012
     *   i ___ 
     *   0|ooo
     *   1|oxx
     *   2|xoo
     */

     
    int current = 0;
    int i, j;
 
    
    char *switchMe = (char *)malloc(sizeof(char) * 3); //for grid coords
    char *direction = (char *)malloc(sizeof(char) * 6); //for move direction
    
	sprintf(switchMe, "\0\0");
	sprintf(direction, "\0\0\0\0\0\0");
    
    char **keyValuePair = (char **)calloc(sizeof(char*), 2);
    
    
    keyValuePair[0] = switchMe;
    keyValuePair[1] = direction;

    //Vertical passes time!!
    for(current; current < NUM_GEMS; current++) {
        
        for(i = 0; i < size; i++) {
            
            for(j = 0; j < size; j++) {
                //Column-by-column
                if(grid[i][j] == current) {
                    
                    if(inBounds(i-2, j)) {
                        if(grid[i-2][j] == current) {
                                                        
                            /* we will match these grid sections: (where x is current)
                             * 
                             * oxo      oxo 
                             * xoo  and oox
                             * oxo      oxo */
                             
                             if(inBounds(i-1, j-1)) {
                                 if(grid[i-1][j-1] == current) {
                                     /* oxo
                                      * xoo
                                      * oxo */
                                     sprintf(switchMe, "%d%d",
                                             i-1,
                                             j-1);
                                     sprintf(direction, "right");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                 }
							}
                             
                            if(inBounds(i-1, j+1)) {
                                if(grid[i-1][j+1] == current) {
                                    /* oxo
                                     * oox
                                     * oxo */
                                     sprintf(switchMe, "%d%d",
                                             i-1,
                                             j+1);
                                     sprintf(direction, "left");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }
                        }
                    }
                      
                    if(inBounds(i-1, j)) {
                        if(grid[i-1][j] == current) {
                            /* Match grids that have two gems in a row
                             * 
                             * xoo oxo oox 
                             * oxo ooo oxo  
                             * oxo oxo oxo  
                             *     oxo
                             **/
                               
                            if(inBounds(i-2, j-1)) {
                                if(grid[i-2][j-1] == current) {
                                    /* xoo
                                     * oxo
                                     * oxo */
                                     sprintf(switchMe, "%d%d",
                                             i-2,
                                             j-1);
                                     sprintf(direction, "right");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }   
                               
                            if(inBounds(i-3, j)) {
                                if(grid[i-3][j] == current) {
                                    /* oxo
                                     * ooo
                                     * oxo
                                     * oxo */
                                     sprintf(switchMe, "%d%d",
                                             i-3,
                                             j);
                                     sprintf(direction, "down");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }
                                
                            if(inBounds(i-2, j+1)) {
                                if(grid[i-2][j+1] == current) {
                                    /* oox
                                     * oxo
                                     * oxo */
                                     sprintf(switchMe, "%d%d",
                                             i-2,
                                             j+1);
                                     sprintf(direction, "left");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }
						}
					}
					
					if(inBounds(i+1, j)) {
						if(grid[i+1][j] == current) {
							 /* Match grids that have two gems in a row
                             * 
                             * oxo oxo oxo 
                             * oxo oxo oxo  
                             * xoo ooo oox  
                             *     oxo      */
                             
                            if(inBounds(i+2, j-1)) {
                                if(grid[i+2][j-1] == current) {
                                    /* oxo
                                     * oxo
                                     * xoo */
                                     sprintf(switchMe, "%d%d",
                                             i+2,
                                             j-1);
                                     sprintf(direction, "right");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }
                                  
                            if(inBounds(i+3, j)) {
                                if(grid[i+3][j] == current) {
                                    /* oxo
                                     * oxo
                                     * ooo
                                     * oxo */
                                     sprintf(switchMe, "%d%d",
                                             i+3,
                                             j);
                                     sprintf(direction, "up");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                     }
                            }
                                   
                            if(inBounds(i+2, j+1)) {
                                if(grid[i+2][j+1] == current) {
                                    /* oxo
                                     * oxo
                                     * oox */
                                     sprintf(switchMe, "%d%d",
                                             i+2,
                                             j+1);
                                     sprintf(direction, "left");
                                     printf("CURRENT = %d\n", current);
                                     return keyValuePair;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return keyValuePair;
}

void get_pixel_color(Display *d, Window root, int x, int y, XColor *color) {
	  
	XImage *img =XGetImage(d, root, x, y, 1, 1, DEPTH24, XYPixmap);
	
	color->pixel = XGetPixel(img, 0, 0);
	XFree(img);
	XQueryColor(d, DefaultColormap(d, DefaultScreen(d)), color);
}

int findGemColor(XColor *c) {
	
	GEMCOLOR myGem;
	

	int red = c->red, green = c->green, blue = c->blue;

	/***************************
	 ===============BLUE
	 **************************/
	if(blue > 40000 && 
	   ((float)(blue) / (float)(green) >= 1.5 ) &&
	   red < 10000) {
		myGem = BLUE;
		return myGem;
	}
	
	/***************************
	 ===============GREEN
	 **************************/
	if(green > 35000 &&
	   (green - red) > 25000 &&
	   (green - blue) > 20000) {
		myGem = GREEN;
		return myGem;
	}

	/***************************
	 ===============WHITE
	 **************************/
	if(red == green && green == blue) {
		myGem = WHITE;
		return myGem;
	}
	
	else if(red > 60000 && green > 60000 && blue > 60000) {
		myGem = WHITE;
		return myGem;
	}
	
	/***************************
	 ===============YELLOW
	 **************************/
	if((red > 40000 && green > 50000 && blue < 11000 &&
	   ((float)green / (float)blue >= 2.5))
	   ||
	   (red > 60000 && green > 60000 && blue < 12000)
	   ||
	   (red > 42000 && red < 53000 &&
	   green > 29000 && green < 37000 &&
	   blue > 2400 && blue < 3500)
	   ||
	   (red > 60000 && green > 45000 && blue < 10000)
	   ||
	   (red > 65000 && green > 65000 && blue < 12500)) {
	    
		myGem = YELLOW;
		return myGem;
	}
	
	/***************************
	 ===============RED
	 **************************/
	if(red > 62000 && green < 10000 && blue < 20000) {
		myGem = RED;
		return myGem;
	}
	     
	/***************************
	 ===============PURPLE
	 **************************/
	if((red == blue && green < 10000)
	   ||
	   (red >= 3072 && green <= 2056 && blue <= 3072)
	   ||
	   (red >= 5000 && green <= 4000 && blue <= 4500)
	   ||
	   (red >= 12500 && green <= 9000 && blue <= 7500)) {
		myGem = PURPLE;
		return myGem;
	}
	     
	/***************************
	 ===============ORANGE
	 **************************/
	 
	 //This one's shitty and hard to define on RGB terms
	 //So I left it for last!
	 
	if(red > 55000 &&
	   green > 18000 &&
	   ((float)(green) / (float)(blue) > 1.8)
	   ||
	   (red > 60000 && green > 55000 && blue < 20000)) {
		myGem = ORANGE;
		return myGem;
	}
	
	return 9;
	
}
