#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "lcd.h"
#include <wiringPi.h>
#include <wiringSerial.h>
#include <stdint.h>

uint32_t bauds[] = {0, 300, 1200, 2400, 9600, 14400, 19200, 57600, 115200};

void cmd(int fd, uint8_t command){
	serialPutchar(fd, CMD);
	serialPutchar(fd, command);
}

void lcdOn(int fd){
	cmd(fd, ON);
}

void lcdOff(int fd){
	cmd(fd, OFF);
}

void setCursor(int fd, uint8_t y, uint8_t x){
	uint8_t pos;
	// line one starts at 0x00 line two at 0x40
	if (y == 1)
		pos = 0x00 + x;
	else if (y == 2)
		pos = 0x40 + x;
	else if (y == 3)
		pos = 0x14 + x;
	else if (y == 4)
		pos = 0x54 + x;
	else
		pos = 0x00 + x;
		
	cmd(fd, POSITION);
	serialPutchar(fd, pos);
}

void home(int fd){
	cmd(fd, HOME);
}

void underLine(int fd, bool turn_on){
	if (turn_on)
		cmd(fd, UNDERLINEON);
	else
		cmd(fd, UNDERLINEOFF);
}

void moveLeft(int fd){
	cmd(fd, MOVELEFT);
}

void moveRight(int fd){
	cmd(fd, MOVERIGHT);
}

void cursorBlink(int fd, bool turn_on){
	if (turn_on)
		cmd(fd, CURSORON);
	else
		cmd(fd, CURSOROFF);
}

void backspace(int fd){
	cmd(fd, BACKSPACE);
}

void clear(int fd){
	cmd(fd, CLEAR);
}

// level between 1 and 50: default 40
void setContrast(int fd, uint8_t level){
	cmd(fd, CONTRAST);
	serialPutchar(fd, level);
}

// level between 1 and 8: default 5
void setBrightness(int fd, uint8_t level){
	cmd(fd, BRIGHTNESS);
	serialPutchar(fd, level);
}

void loadCustomChar(int fd, uint8_t location, uint8_t img[8]){
	int i;
	cmd(fd, CUSTOMCHAR);
	
	// address of customchar (1-8)
	serialPutchar(fd, location);
	
	// load 8 bytes top to bottom
	for (i = 0; i < 8; i++){
		serialPutchar(fd, img[i]);
	}
}

void getCustomChar(int fd, uint8_t location){
	cmd(fd, CUSTOMCHAR);
	
	// address of customchar (1-8)
	serialPutchar(fd, location);
}

void shiftLeft(int fd){
	cmd(fd, SHIFTLEFT);
}

void shiftRight(int fd){
	cmd(fd, SHIFTRIGHT);
}

// for wiringpi: use serialPrintf(int fd, char *str)
void write(int fd, char string[]){
	int i = 0;
	while (string[i] != '\0'){
		serialPutchar(fd, string[i]);
		i++;
	}
}

void putChar(int fd, char character){
	serialPutchar(fd, character);
}

void changeBaud(int fd, uint32_t rate){
	int i = 0;
	while (rate != bauds[i]){
		i++;
	}
	cmd(fd, BAUD);
	serialPutchar(fd, i);
}

void showBaud(int fd){
	cmd(fd, 0x71);
}

// needs fixing
void clearLine(int fd, uint8_t y){
	int i;
		
	setCursor(fd, y, 20);
	for (i = 0; i < 19; i++){
		backspace(fd);
	}
}
