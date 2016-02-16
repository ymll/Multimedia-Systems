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

    //html header
    fprintf(fout, "<html><head><title>CSCI 3280 Asg1</title></head><body><body style=\"background-color:#000\"><pre>");

	//  From ascii.cpp
    unsigned char R;
    unsigned char G;
    unsigned char B;
    unsigned char value;
    for (int y=0; y<h; y++) {
        fputc('\n', fout);
        for (int x=0; x<w; x++) {
            	myBmp.getColor(x,y,R,G,B);
                //unsigned long rgb = (%04xR<<16)|(G<<8)|B; 
                fprintf(fout, "<font color=#%02x%02x%02x>", R,G,B);
                value = 0.299 * R + 0.587 * G + 0.114 * B;
                if (value!=0) {
                    value = value / (255 / MAX_SHADES);
                }
                if (value>=MAX_SHADES) {
                    value = MAX_SHADES-1;
                }
                fputc(shades[value], fout);
                fprintf(fout, "</font>");
        }
    }

    //html footer
	fprintf(fout, "</pre></body></html>");

	//	close ASCII file
	fclose(fout);

	return 0;

} 
