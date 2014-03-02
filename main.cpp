//yo this will ostream the characters 'H' and 'L' to control coffee production
#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

using namespace std;

void spinner(int num) {
    if (num < 1000) {
		putchar('0');
	}
	if (num < 100) {
		putchar('0');
	}
	if (num < 10) {
		putchar('0');
	}
	cout << num;
	putchar(' ');
    fflush(stdout);
}


const int NUM_SECONDS = 30;

int main(int argC, char* argV[])
{

	if (argC < 2) {
		cerr << "NOT ENOUGH ARGUMENTS!" << endl;
		return 1;
	}
	
	int cups = atoi(argV[1]);
	if (cups > 10) {
		cerr << "TOO MANY CUPS!" << endl;
		return 1;
	}
	double minutes;
	FILE *file;
	int desc = 0;
	minutes=(double)cups+0.5;
	cout<< "making "<<cups<< " cups of coffee."<<endl;
	cout<< "Coffee maker will take " << minutes
	<<" minutes to steep coffee."<<endl;
	file = fopen("/dev/rfcomm0", "w+");
	if (!file) {
		cerr << "Cannot open file" << endl;
		cout << "Cannot open file" << endl;
		return(1);
	}
	desc = fileno(file);
	write(desc, "H", 1);
	double time_counter = 0;
	double spin_time = 0;
	double print_time = 0;

    time_t beginning_time;
    time(&beginning_time);
    
    time_t this_time;
    time(&this_time);

    while((time_counter/60) < minutes)
    {
        time(&this_time);
        
        if (time_counter >= spin_time) {
			spinner(minutes * 60 - (time_counter + 1));
			spin_time += 1;
		}

        time_counter = difftime(this_time, beginning_time);

        
        if(time_counter > print_time)
        {
            write(desc, "H", 1);
            print_time += 30;
            
        }
        
        usleep(1000);
    }
    write(desc, "L", 1);
	return 0;
}
