#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <list>
#include <conio.h>
#include <Windows.h>
#include <fstream>

using namespace std;

bool gameOver = false;
bool exitGame = false;

///UP,DOWN,RIGHT,LEFT
char direction;
char oldDirection;
pair <char,char> horizontalOpposites = make_pair('w','s');
pair <char,char> verticalOpposites = make_pair('d','a');
int score=0;

///Dimensions of the box
const short xDimension = 18;
const short yDimension = 18;

///Fruit coordinates
short fruitX;
short fruitY;

///A vector with coordinates of snake's body
list < pair <short, short> > snake;

///Co-ordinates of snake's head
pair <short,short> head;

bool theyAreOpposites(char oldDir, char newDir){///If it's UP and DOWN or RIGHT and LEFT

    bool theyAre = (oldDir==horizontalOpposites.first && newDir==horizontalOpposites.second)
                 ||(oldDir==horizontalOpposites.second && newDir==horizontalOpposites.first)
                 ||(oldDir==verticalOpposites.first && newDir==verticalOpposites.second)
                 ||(oldDir==verticalOpposites.second && newDir==verticalOpposites.first);

    return theyAre;
}
///Initialize snake
void createSnake(){
    snake.clear();
    head = make_pair(xDimension/2,yDimension/2);
    snake.push_back(head);
    snake.push_back(make_pair(head.first-1,head.second));
    snake.push_back(make_pair(head.first-2,head.second));
}

///Check if at co-ordinates (x,y) there is snake
bool thereIsSnake(short x, short y){

    bool thereIs = false;
    list < pair <short,short> > :: iterator it;

    for(it=snake.begin(); it!= snake.end(); ++it){
        if(it->first == x && it->second == y){
            thereIs = true;
        }
    }
    return thereIs;
}

///Check if at co-ordinates (x,y) there is wall
bool thereIsWall(short x, short y){

    bool thereIs = false;
    list < pair <short,short> > :: iterator it;
        if(x<1 || x>xDimension){///Hits on the side walls
            thereIs = true;
        }
        if(y<1 || y>yDimension){///Hits on the upper and lower walls
            thereIs = true;
        }
    return thereIs;
}

///After eating fruit
void ateFruit(short newX, short newY){

    fruitX = (rand() % xDimension) + 1;
    fruitY = (rand() % yDimension) + 1;
    while(thereIsSnake(fruitX,fruitY)){///Don't spawn fruit where there is snake
        fruitX = (rand() % xDimension) + 1;
        fruitY = (rand() % yDimension) + 1;
    }
    snake.push_back(make_pair(newX,newY));
    score = score + snake.size()*0.50;
}

///Snake movement
void moveSnake(char direction){

    short oldX, oldY, tempX, tempY;
    bool crashIntoSelf = false;
    bool crashIntoWall = false;

    list < pair <short,short> > :: iterator it;

    for(it=snake.begin(); it!= snake.end(); ++it){
        if(it->first == head.first && it->second == head.second){///If it's the head

            switch(direction){///if there is snake in this direction or wall
                case 'w':
                    if(thereIsSnake(it->first,it->second-1)){
                        crashIntoSelf = true;
                    }
                    if(thereIsWall(it->first,it->second-1)){
                        crashIntoWall = true;
                    }
                    break;
                case 's':
                    if(thereIsSnake(it->first,it->second+1)){
                        crashIntoSelf = true;
                    }
                    if(thereIsWall(it->first,it->second+1)){
                        crashIntoWall = true;
                    }
                    break;
                case 'a':
                    if(thereIsSnake(it->first-1,it->second)){
                        crashIntoSelf = true;
                    }
                    if(thereIsWall(it->first-1,it->second)){
                        crashIntoWall = true;
                    }
                    break;
                case 'd':
                    if(thereIsSnake(it->first+1,it->second)){
                        crashIntoSelf = true;
                    }
                    if(thereIsWall(it->first+1,it->second)){
                        crashIntoWall = true;
                    }
                    break;
            }
            if(!crashIntoSelf){

                oldX = it->first;
                oldY = it->second;

                switch(direction){///Move according to direction
                    case 'w':
                        if(crashIntoWall){
                            head.second = yDimension;
                            it->second = head.second;
                        }else{
                            head.second = oldY-1;
                            it->second = head.second;
                        }
                        break;
                    case 's':
                        if(crashIntoWall){
                            head.second = 1;
                            it->second = head.second;
                        }else{
                            head.second = oldY+1;
                            it->second = head.second;
                        }
                        break;
                    case 'a':
                        if(crashIntoWall){
                            head.first = xDimension;
                            it->first = head.first;
                        }else{
                            head.first = oldX-1;
                            it->first = head.first;
                        }
                        break;
                    case 'd':
                        if(crashIntoWall){
                            head.first = 1;
                            it->first = head.first;
                        }else{
                            head.first = oldX+1;
                            it->first = head.first;
                        }
                        break;
                }
                if(head.first == fruitX && head.second == fruitY){
                    list < pair <short,short> > :: iterator findTailCoords;
                    advance(findTailCoords = snake.end(), -1);//findTailCoords = snake.end()-1;
                    ateFruit(findTailCoords->first, findTailCoords->second);
                }
            }

        }else{///If it's the rest of the body
            if(!crashIntoSelf){
                tempX = it->first;
                tempY = it->second;
                it->first = oldX;
                it->second = oldY;
                oldX = tempX;
                oldY = tempY;
            }
        }
    }
    if(crashIntoSelf){ gameOver = true;}
}

///Draw "graphics"
void draw(){

    system("CLS");
    for(short i=1; i!= xDimension+3; i++){
        cout<<"=";
    }
    for(short y=1; y!=yDimension+1; y++){
        cout<<"\n|";
        for(short x=1; x!=xDimension+1; x++){
            if(thereIsSnake(x,y)){
                if(x == head.first && y == head.second){
                    cout<<"O";
                }else{
                    cout<<"o";
                }
            }else{
                if(x==fruitX && y==fruitY){
                    cout<<"*";
                }else{
                    cout<<" ";
                }
            }
        }
        cout<<"|";
    }
    cout<<"\n";
    for(short j=1; j!= xDimension+3; j++){
        cout<<"=";
    }
    cout<<"\nSCORE: "<<score<<endl;
}

///Initialize game
void startGame(){
    score = 0;
    gameOver = false;
    createSnake();
    fruitX = (rand() % xDimension) + 1;
    fruitY = (rand() % yDimension) + 1;
    while(thereIsSnake(fruitX,fruitY)){///Don't spawn fruit where there is snake
        fruitX = (rand() % xDimension) + 1;
        fruitY = (rand() % yDimension) + 1;
    }
    draw();
    direction = getch();

    if(direction == 'e'){///Exit when 'e' is pressed
        exitGame = true;
    }
    while(direction == 'a'){///On starting point snake can't move on the left
        direction = getch();
        if(direction == 'e'){///Exit when 'e' is pressed
            exitGame = true;
        }
    }
    moveSnake(direction);
}

int main()
{
    do{
        startGame();

        while(!gameOver && !exitGame){

            ///Automatic Movement
            if(!(_kbhit())){
                moveSnake(direction);
            }else{
                oldDirection = direction;
                direction = getch();
                if(direction == 'e'){
                    exitGame = true;
                }else{
                    if(theyAreOpposites(oldDirection, direction)){///if you want to move on the opposite direction
                        direction = oldDirection;///Don't do anything, just keep on moving
                        moveSnake(direction);
                    }else{
                        moveSnake(direction);
                    }
                }
            }
            Sleep(100);///Reduce Speed
            draw();

            /*///Manual Movement
            if(direction == 'e'){
                exitGame = true;
            }else{
                if(theyAreOpposites(oldDirection, direction)){///if you want to move on the opposite direction
                    //direction = oldDirection;///Don't do anything, just keep on moving
                    //moveSnake(direction);
                    //direction = getch();
                }else{
                    draw();
                    oldDirection = direction;
                    direction = getch();
                    moveSnake(direction);
                }
            }*/
        }
        Sleep(1000);///Wait a bit before restarting
    }while(!exitGame);
}
