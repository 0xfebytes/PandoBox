#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "lcd.h"

/* 
 * DEFINITIONS -------------------------------------------
*/
// user defined points for the input from the atmega
#define PIN_A		2
#define PIN_B		0
#define BUTTON		3

// REMOVE; testing
#define A_LED		1
#define B_LED		4
#define BUT_LED		5

/*
 * VARIABLES ---------------------------------------------
*/
// for the serial interface
int lcd;

// heart of the menu (counter determines cursor placement
static uint8_t counter = 0;
const char *menu[9];

// pointers to the fifo and information files
FILE *fifo;
FILE *info;

// for the timer (determine when to exit menu)
time_t menu_time, cur_time;
int elapsed;

// REMOVE; testing
char buffer[sizeof(int)*8+1];
uint8_t len = sizeof(buffer);

// TO DO:
// create parameters for information

/*
 * FORWARD DECLARATIONS ----------------------------------
*/
void printMenu(uint8_t current_item); // generates menu
void incMenu(void); // increases the counter
void decMenu(void); // decreases the counter
void buttonPress(void); // sends commands to fifo

/*
 * MAIN --------------------------------------------------
 * -------------------------------------------------------
*/
int main(int argc, char* argv[]){
	// menu items
	// item 0 represents no menu, used for displaying
	// current track info
	menu[0] = "";
	menu[1] = "Play/Pause";
	menu[2] = "Play Next";
	menu[3] = "Like Song";
	menu[4] = "Dislike Song";
	menu[5] = "Change Station";
	menu[6] = "";
	menu[7] = "";
	menu[8] = "";

	// open and check serial
	if((lcd = serialOpen("/dev/ttyAMA0", 9600)) < 0 ) {
		printf("Could not open serial device /dev/ttyAMA0\n");
		return 1;
	}

	// setup and check if wiringPi is good
    if(wiringPiSetup() == -1) {
        printf("Unable to start wiringPi\n");
		return 1;
	}

	// ISR for increasing counter
	if(wiringPiISR(PIN_A, INT_EDGE_RISING, &incMenu) < 0 )
	{
		fprintf(stderr, "Could not setup ISR: %s\n", strerror(errno));
		return 1;
	}

	// put back in if needed
	// delay(10);

	// ISR for decreasing counter
	if(wiringPiISR(PIN_B, INT_EDGE_RISING, &decMenu) < 0)
	{
		fprintf(stderr, "Could not setup ISR: %s\n", strerror(errno));
		return 1;
	}

	// put back in if needed
	// delay(10);

	// ISR for select
	if(wiringPiISR(BUTTON, INT_EDGE_RISING, &buttonPress) < 0)
	{
		fprintf(stderr, "Could not setup ISR: %s\n", strerror(errno));
		return 1;
	}

	// REMOVE; testing
	pinMode(A_LED, OUTPUT);
	pinMode(B_LED, OUTPUT);
	pinMode(BUT_LED, OUTPUT);

	// set up internal pulldowns
	pullUpDnControl(PIN_A, PUD_DOWN);
	pullUpDnControl(PIN_B, PUD_DOWN);
	pullUpDnControl(BUTTON, PUD_DOWN);

	// open the fifo and file with track information
	fifo = fopen("/home/hal/.config/pianobar/ctl", "w");
	info = fopen("/home/hal/.config/pianobar/out", "r");

	// clear/init LCD
	clear(lcd);

	//REMOVE THIS; IT IS IMPORTANT THAT YOU REMOVE THIS
	incMenu();

	for(;;){
		// calculate the time in menu
		time(&cur_time);
		elapsed = difftime(cur_time, menu_time);
		if ((int)elapsed > 30) {
			counter = 0;
		}
		printf("elapsed time: %2d, counter: %d\n", (int)elapsed, counter);

		/*
		fgets(temp, 80, info);
		artist = strtok(temp, "\n");
		song = strtok(NULL, "\n");
		album = strtok(NULL, "\n");
		*/

		if (counter == 0){
			clear(lcd);
			setCursor(lcd, 1, 0);
			serialPrintf(lcd, "Will fill in...");
			/*
			serialPrintf(lcd, "<======");
			setCursor(lcd, 1, 14);
			serialPrintf(lcd, ">-%s", time_left);
			setCursor(lcd, 2, 0);
			serialPrintf(lcd, artist);
			setCursor(lcd, 3, 0);
			serialPrintf(lcd, song);
			setCursor(lcd, 4, 0);
			serialPrintf(lcd, album);
			*/
		}

		delay(1000);
	}

	serialClose(lcd);
	fclose(fifo);
	fclose(info);

    return 0;
}

/*
 * FUNCTIONS ---------------------------------------------
*/
// prints the menu with cursor
void printMenu(uint8_t current_item){
	clear(lcd);
	// REMOVE; testing
	setCursor(lcd, 1, 15);
	if(snprintf(buffer, len, "%d", counter) == -1) {
		serialPrintf(lcd,"ERROR");
	}
	serialPrintf(lcd, buffer);
	setCursor(lcd, 1, 0);
	// first page
	if (current_item < 5){
		serialPrintf(lcd, menu[1]);
		setCursor(lcd, 2, 0);
		serialPrintf(lcd, menu[2]);
		setCursor(lcd, 3, 0);
		serialPrintf(lcd, menu[3]);
		setCursor(lcd, 4, 0);
		serialPrintf(lcd, menu[4]);
		setCursor(lcd, current_item, 0);
	}
	// second page
	else if (current_item > 5){
		serialPrintf(lcd, menu[5]);
		setCursor(lcd, 2, 0);
		serialPrintf(lcd, menu[6]);
		setCursor(lcd, 3, 0);
		serialPrintf(lcd, menu[7]);
		setCursor(lcd, 4, 0);
		serialPrintf(lcd, menu[8]);
		setCursor(lcd, current_item - 4, 0);
	}
	serialPrintf(lcd, ">%s", menu[current_item]);
}

// increases the counter
void incMenu(void){
	if (counter < 5)
		counter += 1;
	printMenu(counter);
	// REMOVE; testing
	digitalWrite(A_LED, 1);
	delay(100);
	digitalWrite(A_LED, 0);
	time(&menu_time);
}

// decreases the counter
void decMenu(void){
	if (counter > 1)
		counter -= 1;
	printMenu(counter);
	// REMOVE; testing
	digitalWrite(B_LED, 1);
	delay(100);
	digitalWrite(B_LED, 0);
	time(&menu_time);
}

// sends commands to the fifo
void buttonPress(void){
	// REMOVE; testing
	digitalWrite(BUT_LED, 1);
	delay(100);
	digitalWrite(BUT_LED, 0);
	switch(counter){
	case 1:
		fprintf(fifo, "p");
		break;
	case 2:
		fprintf(fifo, "n");
		break;
	case 3:
		fprintf(fifo, "+");
		break;
	case 4:
		fprintf(fifo, "-");
		break;
	default:
		break;
	}
	fflush(fifo);
	time(&menu_time);
}
