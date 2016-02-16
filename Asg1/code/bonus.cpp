/*

	CSCI 3280, Introduction to Multimedia Systems
	Spring 2015
	
	Assignment 01 Skeleton
    SID: 1155032377

	ascii.cpp
	
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
	printf("argv[1] = %s\n", argv[1]);

	char shades[MAX_SHADES] = {' ', '.', '+', '*', 'X', '%', '#', '@'};
	int		w, h;

	//	Read in image data ( command line: argv[1] )
	//
	Bitmap myBmp( argv[1] );
	
	w = myBmp.getWidth();
	h = myBmp.getHeight();

	//	Prepare output TXT file ( command line: argv[2] )
	//
	FILE *fout = fopen( argv[2], "w" );
	fprintf( fout, "%d %d", w, h );
	
	//
	//	Your code goes here ....
	//	
	//	Advice:
	//	Use Bitmap.getColor(x,y,R,G,B) for getting color of pixel at (x,y)
	//	Use fputc() to read character and "\n" to end your line.
	//
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char value;
    for (int y=0; y<h; y++) {
        printf("\n");
        fputc('\n', fout);
        for (int x=0; x<w; x++) {
            	myBmp.getColor(x,y,R,G,B);
                value = 0.299 * R + 0.587 * G + 0.114 * B;
                if (value!=0) {
                    value = value / (255 / MAX_SHADES);
                }
                if (value>=MAX_SHADES) {
                    value = MAX_SHADES-1;
                }
                printf("%c", shades[value]);
                fputc(shades[value], fout);
        }
    }

	//	close ASCII file
	fclose(fout);

	return 0;

} 
