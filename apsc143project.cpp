// APSC 143 Engineering Programming Project Starter Code
// Feel free to change anything about this file, but do not remove "#include "colours.h".

// Make sure to include all relevant libraries
#include <cstdio>
#include <cstdlib>
#include "colors.h"
#include <iostream>

// You don't need to use these, but they help make your code look more organized
#define PACMAN 'P'
#define GHOST 'G'
#define DOT '.'
#define WALL 'W'
#define EMPTY ' '
#define UP 'w'
#define LEFT 'a'
#define DOWN 's'
#define RIGHT 'd'

//global variable declaration so they can be accessed in any function
//ghost locations intialied to the size of an int as they will be searched for and intialized later
int ghost1Location[] = {sizeof(int),sizeof(int)};
int ghost2Location[] = {sizeof(int),sizeof(int)};
int winCounter = 0;
int playerLocation[] = {sizeof(int),sizeof(int)};
char** map = (char**)malloc(sizeof(char*)*9);
char ghostMap[9][9];
char oneDirection = LEFT;
char twoDirection = UP;

int tempPlayerLocation[2];
int tempGhost1[2];
int tempGhost2[2];
// This function searches through the 2D array and sets the locations of the ghosts and pacman to the desired locations
void locations(){
    int ghost = 1;
    for(int i = 0; i < 9; i ++){
        for(int j = 0; j < 9; j++){
            if((map[i][j] == GHOST) && ghost){
                ghost1Location[0] = i; ghost1Location[1]=j;
                --ghost;
            }else if(map[i][j] == GHOST){
                ghost2Location[0] = i; ghost2Location[1]=j;
            }else if(map[i][j] == PACMAN){
                playerLocation[0] = i; playerLocation[1] = j;
            }
        }
    }
}
// This function prints the map, changing the colour depending on the char
void printMap(){
    colourChange(BLUE);
    printf("W W W W W W W W W W W\n");
    for(int i = 0; i < 9; i++){
        printf("W ");
        for(int j = 0; j < 9; j++){
            if(map[i][j]==GHOST) {
                colourChange(PINK);
                printf("%c ", map[i][j]);
            }else if(map[i][j] == PACMAN){
                colourChange(YELLOW);
                printf("%c ", map[i][j]);
            }else if(map[i][j] == DOT){
                colourChange(WHITE);
                printf("%c ", map[i][j]);
            }
            else{
                colourChange(BLUE);
                printf("%c ", map[i][j]);
            }
        }
        colourChange(BLUE);
        printf("W");
        printf("\n");
    }
    colourChange(BLUE);
    printf("W W W W W W W W W W W");
    printf("\n");
}

//function definition for function that reads the pac-man map.
char **readMap(char fileName[]){
    //use fopen function to open and read function using the pathway to the downloaded map file. Give the file a name (fo)
    FILE *fo = fopen(fileName, "r");
    //if no function is found, print it to the user and end the program
    if(fo == NULL){
        printf("File not found.");
        return 0;
    }
    //use a for loop to allocate memory for each of the rows of the array
    for(int i = 0; i < 9; i++) {
        map[i] = (char*)malloc(sizeof(char)*9);
    }
    //use a for loop to iterate through the rows of the map array and assign each value of the file to the corresponding
    //location in the map array
    for(int i = 0; i < 9;i++) {
        if(i == 0) {
            fscanf(fo, "%c %c %c %c %c %c %c %c %c", &(map)[i][0], &(map)[i][1], &(map)[i][2],
                   &(map)[i][3], &(map)[i][4], &(map)[i][5], &(map)[i][6],
                   &(map)[i][7], &(map)[i][8]);
        }
            //due to a bug with file reading, the first value had to be read twice (reason for duplication of map address [i][0]
        else{
            fscanf(fo, "%c %c %c %c %c %c %c %c %c %c",&(map)[i][0] ,&(map)[i][0], &(map)[i][1], &(map)[i][2],
                   &(map)[i][3], &(map)[i][4], &(map)[i][5], &(map)[i][6],
                   &(map)[i][7], &(map)[i][8]);
        }

    }
    //close the file
    fclose(fo);
    //return the completely built map
    return map;
}

//Function definition for function that checks if the player has won the game. The player wins by eating all the candies.
int winCheck(){
    //if loop checks if the player was able to eat all 54 candies, in which case they won, so the if statement returns 1 (win check is true)
    if(winCounter == 54){
        return 1;
    }
        //if they didn't eat all 54 candies, they lost, so the statement returns 0 (win check is false)
    else
        return 0;
}

/* Function definition for function that checks if the player has lost the game. Function operates by checking if either
 * ghost is in the same location as the player, or if either ghost has collided with the player. If any of these conditions
 * are true, the player has lost. The collision conditions are necessary because the player and ghost could swap positions
 * in the array (collide), which would mean the player loses, however the location conditions would not pick it up.
*/
int loseCheck(){
    //four variables below check for collisions and if a ghost and player are in the same spot
    int ghostOne = ghost1Location[0] == playerLocation[0] && ghost1Location[1] == playerLocation[1];
    int ghostTwo = ghost2Location[0] == playerLocation[0] && ghost2Location[1] == playerLocation[1];
    int collison1  = (ghost1Location[0] == tempPlayerLocation[0]) && (ghost1Location[1] == tempPlayerLocation[1]) &&
                     (tempGhost1[0] == playerLocation[0]) && (tempGhost1[1] == playerLocation[1]);
    int collison2  = (ghost2Location[0] == tempPlayerLocation[0]) && (ghost2Location[1] == tempPlayerLocation[1]) &&
                     (tempGhost2[0] == playerLocation[0]) && (tempGhost2[1] == playerLocation[1]);
    //if any of the variables above are true, the player has lost, so the function returns 1 (lose check is true)
    if(ghostOne|| ghostTwo||collison1||collison2){
        return 1;

    }
        //otherwise, the player has not lost so the function returns 0 (lose check is false)
    else
        return 0;
}
/* THe is wall fucntion takes an an array of a location (x and y values) and a direction, based of th position and
 * location, it will return if it is a wall or not. This is done by taking the position, adding to the x or y value
 * depending on direction entered, and checking on the map if that value is a wall. It uses a switch statement to do
 * this and returns 1 if it is a wall and 0 if not.
 */
int isWall(int location[],char direction){

    switch(direction) {
        case (UP): {
            if (location[0] == 0) { return 1; }
            else if (map[(location[0]) - 1][location[1]] == 'W') { return 1; }
            else { return 0; }
        }
        case (LEFT): {
            if (location[1] == 0) { return 1; }
            else if (map[location[0]][location[1] - 1] == 'W') { return 1; }
            else { return 0; }
        }
        case (DOWN): {
            if (location[0] == 8) { return 1; }
            else if (map[location[0] + 1][location[1]] == 'W') { return 1; }
            else { return 0;}
        }
        case (RIGHT): {
            if (location[1] == 8) { return 1; }
            else if (map[location[0]][location[1] + 1] == 'W') { return 1; }
            else { return 0; }
        }
    }
    return 0;
}
/* This function is a void function it takes no arguments and returns nothing. What is does do is take the current player
 * location that has been altered in the move function and replaces it on the map. If there was a dot in the location
 * the player is going it adds to the win counter, and removes the dot from the ghost map that the ghost use to reference
 * what they should leave behind when they move.
 */
void checkAndReplace(){
    if(map[playerLocation[0]][playerLocation[1]] == DOT){
        map[playerLocation[0]][playerLocation[1]] = PACMAN;
        ghostMap[playerLocation[0]][playerLocation[1]] = EMPTY;
        winCounter += 1;
    }else if(map[playerLocation[0]][playerLocation[1]] == GHOST){
        ghostMap[playerLocation[0]][playerLocation[1]] = GHOST;
    }
    else {
        map[playerLocation[0]][playerLocation[1]] = PACMAN;
    }
}
/* The first thing this program does is check set the temp player location equal to the current location, this is used above
 * when checking for collisions as we need to know if the ghost and the player swapped locations. This function passes
 * in a direction, which is direction enterd by the person playing. It then uses a switch statement for the direction
 * entered by the player. It then uses the isWall function with the direction entered to see if it can move. If it
 * can move it removes the pacman from the map, changes the player location, then calls check and replace to change the
 * location of the player on the map. If it cant move, it just breaks and doesn't do anything.
 */
void move(char direction){
    tempPlayerLocation[0] = playerLocation[0];
    tempPlayerLocation[1] = playerLocation[1];
    switch(direction){
        case (UP):
            if(isWall(playerLocation,direction) == 0){
                if(map[playerLocation[0]][playerLocation[1]] == GHOST)
                    map[playerLocation[0]][playerLocation[1]] = GHOST;
                else
                    map[playerLocation[0]][playerLocation[1]] = EMPTY;
                playerLocation[0] -= 1;
                checkAndReplace();
            }
            break;
        case (LEFT):
            if(isWall(playerLocation,direction) == 0){
                if(map[playerLocation[0]][playerLocation[1]] == GHOST)
                    map[playerLocation[0]][playerLocation[1]] = GHOST;
                else
                    map[playerLocation[0]][playerLocation[1]] = EMPTY;
                playerLocation[1] -= 1;
                checkAndReplace();
                break;
            }
        case (DOWN):
            if(isWall(playerLocation,direction) == 0){
                if(map[playerLocation[0]][playerLocation[1]] == GHOST)
                    map[playerLocation[0]][playerLocation[1]] = GHOST;
                else
                    map[playerLocation[0]][playerLocation[1]] = EMPTY;
                playerLocation[0] += 1;
                checkAndReplace();
            }
            break;
        case (RIGHT):
            if(isWall(playerLocation,direction) == 0){
                if(map[playerLocation[0]][playerLocation[1]] == GHOST)
                    map[playerLocation[0]][playerLocation[1]] = GHOST;
                else
                    map[playerLocation[0]][playerLocation[1]] = EMPTY;
                playerLocation[1] += 1;
                checkAndReplace();
            }
           break;
    }
}
/* The no180 function ensures that ghosts are not doing 180s unless it has too. Keeping in mind that this function will
 * only be called if it can no longer move in a straight line, it takes in the direction that the ghost is currently
 * going and ranks the directions it should try and move in. The opposite direction will always be the last direction
 * returned as we don't want it doing 180s unless it has to.
 */
char no180(char direction,int number){
    char one, two, three;
    if(direction == UP){
        one = RIGHT; two = LEFT; three = DOWN;
    }else if(direction == DOWN){
        one = RIGHT; two = LEFT; three = UP;
    }else if(direction == RIGHT){
        one = UP; two = DOWN; three = LEFT;
    }else if(direction == LEFT){
        one = UP; two = DOWN; three = RIGHT;
    }
    switch(number) {
        case 1: return one;
        case 2: return two;
        case 3: return three;
    }
    return 0;
}
/* The two ghost move functions are the exact same, expect for the fact that they have two different positions and
 * directions, both of which are declared as global varibles at the top of the program. The first thing this function
 * does is set the current position equal to a temporary varible, this is used to check for collisions in the loseCheck
 * funtion. It then takes in the current direction, delcared as a global varible, and tries to move in the same
 * direction as it was currenly moving. It uses the isWall varible to check if it can move in that direction, if it can
 * it moves in that direction and replaces the place it just was with the value stored on the ghost map. This is
 * necessary to make sure that dots are replaced after the ghost has moved over. The default case occurs when the current
 * direction being travelled has to be changed due to a wall being in the way. This works by first calling the no180
 * function, this function returns the directions it should travel in, in order to avoid doing 180s and repeatably
 * going back and forth. The default case takes that current direciton and using an if/else statement and the isWall
 * funtion tries to move in the directions given to it, where the 180 direction is always the last try. Once it decides
 * which way to go, it moves in that direction, and sets the current direction equal to that direction.
 */
void ghostMoveTwo(){
    tempGhost2[0] = ghost2Location[0];
    tempGhost2[1] = ghost2Location[1];
    switch(twoDirection) {
        case UP: {if (!isWall(ghost2Location, UP)&&twoDirection==UP) {
                map[ghost2Location[0]][ghost2Location[1]] = ghostMap[ghost2Location[0]][ghost2Location[1]];
                ghost2Location[0] -= 1;
                map[ghost2Location[0]][ghost2Location[1]] = GHOST;
                twoDirection = UP;
                break;
            }}
        case RIGHT: {if (!isWall(ghost2Location, RIGHT)&&twoDirection==RIGHT) {
                map[ghost2Location[0]][ghost2Location[1]] = ghostMap[ghost2Location[0]][ghost2Location[1]];
                ghost2Location[1] += 1;
                map[ghost2Location[0]][ghost2Location[1]] = GHOST;
                twoDirection = RIGHT;
                break;
            }}
        case LEFT: {if (!isWall(ghost2Location, LEFT)&&twoDirection==LEFT) {
                map[ghost2Location[0]][ghost2Location[1]] = ghostMap[ghost2Location[0]][ghost2Location[1]];
                ghost2Location[1] -= 1;
                map[ghost2Location[0]][ghost2Location[1]] = GHOST;
                twoDirection = LEFT;
                break;
            }}
        case DOWN: {if (!isWall(ghost2Location, DOWN)&&twoDirection==DOWN) {
                map[ghost2Location[0]][ghost2Location[1]] = ghostMap[ghost2Location[0]][ghost2Location[1]];
                ghost2Location[0] += 1;
                map[ghost2Location[0]][ghost2Location[1]] = GHOST;
                twoDirection = DOWN;
                break;
            }}
        default: {
            char directionOne = no180(twoDirection, 1);
            char directionTwo = no180(twoDirection, 2);
            char directionThree = no180(twoDirection, 3);
            if (!isWall(ghost2Location, directionOne)) {
                map[ghost2Location[0]][ghost2Location[1]] = ghostMap[ghost2Location[0]][ghost2Location[1]];
                if (directionOne == LEFT) {
                    ghost2Location[1] -= 1;
                } else if (directionOne == RIGHT) {
                    ghost2Location[1] += 1;
                } else if (directionOne == UP) {
                    ghost2Location[0] -= 1;
                } else {
                    ghost2Location[0] += 1;
                }
                map[ghost2Location[0]][ghost2Location[1]] = GHOST;
                twoDirection = directionOne;
            } else if (!isWall(ghost2Location, directionTwo)) {
                map[ghost2Location[0]][ghost2Location[1]] = ghostMap[ghost2Location[0]][ghost2Location[1]];
                if (directionTwo == LEFT) {
                    ghost2Location[1] -= 1;
                } else if (directionTwo == RIGHT) {
                    ghost2Location[1] += 1;
                } else if (directionTwo == UP) {
                    ghost2Location[0] -= 1;
                } else {
                    ghost2Location[0] += 1;
                }
                map[ghost2Location[0]][ghost2Location[1]] = GHOST;
                twoDirection = directionTwo;
            } else {
                map[ghost2Location[0]][ghost2Location[1]] = ghostMap[ghost2Location[0]][ghost2Location[1]];
                if (directionThree == LEFT) {
                    ghost2Location[1] -= 1;
                } else if (directionThree == RIGHT) {
                    ghost2Location[1] += 1;
                } else if (directionThree == UP) {
                    ghost2Location[0] -= 1;
                } else {
                    ghost2Location[0] += 1;
                }
                map[ghost2Location[0]][ghost2Location[1]] = GHOST;
                twoDirection = directionThree;
            }
            break;
        }
    }
}
void ghostMoveOne(){
    tempGhost1[0] = ghost1Location[0];
    tempGhost1[1] = ghost1Location[1];
    switch(oneDirection) {
        case UP: {if (!isWall(ghost1Location, UP)&&oneDirection==UP) {
                map[ghost1Location[0]][ghost1Location[1]] = ghostMap[ghost1Location[0]][ghost1Location[1]];
                ghost1Location[0] -= 1;
                map[ghost1Location[0]][ghost1Location[1]] = GHOST;
                oneDirection = UP;
                break;
            }}
        case RIGHT: {if (!isWall(ghost1Location, RIGHT)&&oneDirection==RIGHT) {
                map[ghost1Location[0]][ghost1Location[1]] = ghostMap[ghost1Location[0]][ghost1Location[1]];
                ghost1Location[1] += 1;
                map[ghost1Location[0]][ghost1Location[1]] = GHOST;
                oneDirection = RIGHT;
                break;
            }}
        case LEFT: {if (!isWall(ghost1Location, LEFT)&&oneDirection==LEFT) {
                map[ghost1Location[0]][ghost1Location[1]] = ghostMap[ghost1Location[0]][ghost1Location[1]];
                ghost1Location[1] -= 1;
                map[ghost1Location[0]][ghost1Location[1]] = GHOST;
                oneDirection = LEFT;
                break;
            }}
        case DOWN: {if (!isWall(ghost1Location, DOWN)&&oneDirection==DOWN) {
                    map[ghost1Location[0]][ghost1Location[1]] = ghostMap[ghost1Location[0]][ghost1Location[1]];
                    ghost1Location[0] += 1;
                    map[ghost1Location[0]][ghost1Location[1]] = GHOST;
                    oneDirection = DOWN;
                break;
            }}
        default: {
            char directionOne = no180(oneDirection, 1);
            char directionTwo = no180(oneDirection, 2);
            char directionThree = no180(oneDirection, 3);
            if (!isWall(ghost1Location, directionOne)) {
                map[ghost1Location[0]][ghost1Location[1]] = ghostMap[ghost1Location[0]][ghost1Location[1]];
                if (directionOne == LEFT) {
                    ghost1Location[1] -= 1;
                } else if (directionOne == RIGHT) {
                    ghost1Location[1] += 1;
                } else if (directionOne == UP) {
                    ghost1Location[0] -= 1;
                } else {
                    ghost1Location[0] += 1;
                }
                map[ghost1Location[0]][ghost1Location[1]] = GHOST;
                oneDirection = directionOne;
            } else if (!isWall(ghost1Location, directionTwo)) {
                map[ghost1Location[0]][ghost1Location[1]] = ghostMap[ghost1Location[0]][ghost1Location[1]];
                if (directionTwo == LEFT) {
                    ghost1Location[1] -= 1;
                } else if (directionTwo == RIGHT) {
                    ghost1Location[1] += 1;
                } else if (directionTwo == UP) {
                    ghost1Location[0] -= 1;
                } else {
                    ghost1Location[0] += 1;
                }
                map[ghost1Location[0]][ghost1Location[1]] = GHOST;
                oneDirection = directionTwo;
            } else {
                map[ghost1Location[0]][ghost1Location[1]] = ghostMap[ghost1Location[0]][ghost1Location[1]];
                if (directionThree == LEFT) {
                    ghost1Location[1] -= 1;
                } else if (directionThree == RIGHT) {
                    ghost1Location[1] += 1;
                } else if (directionThree == UP) {
                    ghost1Location[0] -= 1;
                } else {
                    ghost1Location[0] += 1;
                }
                map[ghost1Location[0]][ghost1Location[1]] = GHOST;
                oneDirection = directionThree;
            }
            break;
        }
    }
}


/* Function definition for recursive function that checks the pacman is directly below the ghost with no walls between them
 * and therefore in sight of the ghost. The function depends on the x and y coordinates (the variables for which are flipped)
 * of the array location for the ghost. Function operates by checking an array location and then iterating downwards one
 * row until the last row has been checked, a wall has been reached, or pacman has been found.
*/
int checkDown(int x,int y){
    //if the array location being checked is out of the bounds of the map, the pacman can not be in sight so the function
    //returns 2 (not true)
    if (x == 9 || x == -1 || y == 9 || y == -1) {
        return 2;
        //if the array location being checked is a wall, the pacman can not be in sight so the function returns 2 (not true)
    }else if (map[x][y] == WALL) {
        return 2;
    }
    //if the array location being checked is pacman, the pacman is in sight of the ghost so the function returns 1 (true)
    if(map[x][y]== PACMAN){
        return 1;
    }
    //function returns the location one row further down, to iterate through the rows
    return checkDown(x+1,y);
}

/* Function definition for recursive function that checks the pacman is directly to the right the ghost with no walls between them
 * and therefore in sight of the ghost. The function depends on the x and y coordinates (the variables for which are flipped)
 * of the array location for the ghost. Function operates by checking an array location and then iterating to the right one
 * column until the last column has been checked, a wall has been reached, or pacman has been found.
 */
int checkRight(int x,int y){
    //if the array location being checked is out of the bounds of the map, the pacman can not be in sight so the function
    //returns 2 (not true)
    if(x == 9 || x==-1 || y== 9 || y == -1){
        return 2;
    }
        //if the array location being checked is a wall, the pacman can not be in sight so the function returns 2 (not true)
    else if(map[x][y]== WALL){
        return 2;
    }
    //if the array location being checked is pacman, the pacman is in sight of the ghost so the function returns 1 (true)
    if(map[x][y]== PACMAN){
        return 1;
    }
    //function returns the location one column further right, to iterate through the columns
    return checkRight(x,y+1);
}

/* Function definition for recursive function that checks the pacman is directly to the left the ghost with no walls between them
 * and therefore in sight of the ghost. The function depends on the x and y coordinates (the variables for which are flipped)
 * of the array location for the ghost. Function operates by checking an array location and then iterating to the left one
 * column until the last column has been checked, a wall has been reached, or pacman has been found.
*/
int checkLeft(int x,int y){
    //if the array location being checked is out of the bounds of the map, the pacman can not be in sight so the function
    //returns 2 (not true)
    if(x == 9 || x==-1 || y== 9 || y == -1){
        return 2;
    }
        //if the array location being checked is a wall, the pacman can not be in sight so the function returns 2 (not true)
    else if(map[x][y]== WALL){
        return 2;
    }
    //if the array location being checked is pacman, the pacman is in sight of the ghost so the function returns 1 (true)
    if(map[x][y]== PACMAN){
        return 1;
    }
    //function returns the location one column further left, to iterate through the columns
    return checkLeft(x,y-1);
}

/* Function definition for recursive function that checks the pacman is directly above the ghost with no walls between them
 * and therefore in sight of the ghost. The function depends on the x and y coordinates (the variables for which are flipped)
 * of the array location for the ghost. Function operates by checking an array location and then iterating upwards one
 *row until the last row has been checked, a wall has been reached, or pacman has been found.
*/
int checkUp(int x,int y){
    //if the array location being checked is out of the bounds of the map, the pacman can not be in sight so the function
    //returns 2 (not true)
    if(x == 9 || x==-1 || y== 9 || y == -1){
        return 2;
    }
        //if the array location being checked is a wall, the pacman can not be in sight so the function returns 2 (not true)
    else if(map[x][y]== WALL){
        return 2;
    }
    //if the array location being checked is pacman, the pacman is in sight of the ghost so the function returns 1 (true)
    if(map[x][y]== PACMAN){
        return 1;
    }
    //function returns the location one row further up, to iterate through the rows
    return checkUp(x-1,y);
}

/* Function definition for function that uses the output of the four previous functions, which checked if the pacman is
 * in the line of sight of the ghost, to determine the ghosts next move. If any of the check functions returned 1 (true)
 * the line of sight function will automatically assign the ghosts next move to follow the pacman. If none of the check
 * functions returned 1, then the line of sight function returns 0 and is irrelevant. In that case, the ghosts will simply
 * continue on their previous path.
*/
char lineOfSight(int x,int y){
    //check to see if the pacman is above
    if(checkUp(x,y)==1){
        return UP;
        //check to see if the pacman is to the left
    }else if(checkLeft(x,y) == 1){
        return LEFT;
        //check to see if the pacman is to the right
    }else if(checkRight(x,y)==1){
        return RIGHT;
        //check to see if the pacman is below
    }else if(checkDown(x,y)==1){
        return DOWN;
        //if none of the previous cases are true, return 0
    }else {
        return 0;
    }
}

//function definition for function used to create a secondary map of where the dots are, to account for the moving
//ghosts that do not "eat" the dots, so their locations must be replaced with dots as they move.
void ghostMapping() {
    //for loop to iterate through the rows
    for (int i = 0; i < 9; i++) {
        //for loop to iterate through the columns
        for (int j = 0; j < 9; j++) {
            //if the array location has a ghost, assign a dot to the location
            if (map[i][j] == GHOST)
                ghostMap[i][j] = DOT;
                //if the array location has pacman, the location should be empty
            else if (map[i][j] == PACMAN)
                ghostMap[i][j] = EMPTY;
                //for any case where there isn't a ghost or a pacman, the location is the same as the main map
            else
                ghostMap[i][j] = map[i][j];
        }
    }
}



int main() {
    // Call the read map function with the filepath
    map = readMap("/Users/jordancorbett/Downloads/map.txt");
    // Map where the dots are on the map, this will be changed everytime a dot is ate by the pacman
    ghostMapping();
    // Figure out the locations of the pacman and ghosts on the map, this allows for differnet maps to be used
    locations();
    // Callling the printmap function to print the map
    printMap();
    // This funtion will run repeatably until the player either wins or loses at which time it will break
    while(true) {
        // Get input from user, set equal to char variable chart. GETCH() did not work, after research it seems like
        // many people have had this issue on Mac OS.
        char chart = getchar();
        // Flush the input for good measure
        fflush(stdin);
        // Calling ghost movement functins, these move before the player due to the need to replace the dots behind them.
        ghostMoveOne();
        ghostMoveTwo();
        // Calling the move function, which moves the player in the direction specified
        move(chart);
        // Calling the lineOfSight function to check if the player is in the line of sight of the ghost, this function
        // will return 0 if there is no pacman in the line of sight and will return the direction the player is to the
        // ghost if it is in the line of sight.
        char check = lineOfSight(ghost1Location[0], ghost1Location[1]);
        char check2 = lineOfSight(ghost2Location[0],ghost2Location[1]);
        // If the player is in the line of sight of a ghost, the ghost direciton is changed so it will move in that
        // direction.
        if (check!=0) {
            oneDirection = check;
        }
        if(check2!=0) {
            twoDirection = check2;
        }
        // Print the map
        printMap();
        // Check for loss and for win, if so print required statement and break the while loop
        if(winCheck()){
            printf("Congratulations! You win! Press any key to exit the game");
            break;
        }
        if(loseCheck()){
            printf("Sorry, you lose. Press any key to exit the game");
            break;
        }
    }
    // Waiting for input frm the user to end the game, at which time the program will end
    char temp = getchar();
    // Freeing prevously allocated memory for the double pointer of map (2d arrays)
    free(map);
    // Freeing the memory allocated for the single pointer (1d arrays)
    for(int i = 0; i <9; i ++){
        free(map[i]);
    }


    return 0;
}



