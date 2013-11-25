/*
 * splat.c: Displays a spinning cursor.
 *
 * Authors: Tianyi Huang <tianyih@andrew.cmu.edu>
 *          Zixuan Liu <zixuanl@andrew.cmu.edu>
 *  	    Jianan Lu <jiananl@andrew.cmu.edu>
 * Date:    11/5/2013
 */

#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	write(1,"Start!\n",7);
	while(1)
	{
		printf("\b \b|");
		sleep(200);
		printf("\b \b/");
		sleep(200);
		printf("\b \b-");
		sleep(200);
		printf("\b \b\\");
		sleep(200);
	}
	/* Add code here -- put your names at the top. */
	return 0;
}
