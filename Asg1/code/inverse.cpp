/*

	CSCI 3280, Introduction to Multimedia Systems
	Spring 2016
	
	Assignment 01 Skeleton

	inverse.cpp
	
*/

#include "stdio.h"
#include "malloc.h"
#include "memory.h"
#include "math.h"
#include "bmp.h"		//	Simple .bmp library

#define MAX_SHADES 8


//
//	***** PUT ALL YOUR CODE INSIDE main() *****
//
int main( int argc, char** argv)
{

	char shades[MAX_SHADES] = {' ', '.', '+', '*', 'X', '%', '#', '@'};
	int 	w, h;
	
	//	Open ASCII Art file ( command line: argv[1] )
	//
	FILE *fin = fopen( argv[1], "r" );
	fscanf( fin, "%d %d", &w, &h );
		
	//	Create a Bitmap object accordingly
	Bitmap myBmp( w, h);

	//
	//	Your code goes here ....
	//
	//	Advice:
	//	Use Bitmap.setColor(x,y,R,G,B) for setting color of pixel at (x,y)
	//	Use fgetc()
	char c;
    int x=0;
    int y=0;
    unsigned char value;
	do{
        c = fgetc (fin);
        for (int i=0; i<MAX_SHADES; i++){
          if (c==shades[i]) {
              value = 255/MAX_SHADES * i;
          }
        }
            myBmp.setColor(x,y,value,value,value);
            x++;
            if (x>w) {
                x=0;
                y++;
            }
    } while (c != EOF);
	//	Save Bitmap file ( command line: argv[2] )	
	myBmp.save( argv[2] );
	fclose(fin);
		
	return 0;

} 
