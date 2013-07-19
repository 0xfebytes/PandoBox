/*
 * Requires dbg_putchar
*/
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#define ON				0x41
#define OFF				0x42
#define POSITION		0x45
#define HOME			0x46
#define UNDERLINEON		0x47
#define UNDERLINEOFF	0x48
#define MOVELEFT		0x49
#define MOVERIGHT		0x4A
#define CURSORON		0x4B
#define CURSOROFF		0x4C
#define BACKSPACE		0x4E
#define CLEAR			0x51
#define CONTRAST		0x52
#define BRIGHTNESS		0x53
#define CUSTOMCHAR		0x54
#define SHIFTLEFT		0x55
#define SHIFTRIGHT		0x56
#define CMD				0xFE
#define BAUD			0x61

void cmd(int fd, uint8_t command);
void lcdOn(int fd);
void lcdOff(int fd);
void setCursor(int fd, uint8_t y, uint8_t x);
void home(int fd);
void underLine(int fd, bool turn_on);
void moveLeft(int fd);
void moveRight(int fd);
void cursorBlink(int fd, bool turn_on);
void backspace(int fd);
void clear(int fd);
void setContrast(int fd, uint8_t level);
void setBrightness(int fd, uint8_t level);
void loadCustomChar(int fd, uint8_t location, uint8_t img[8]);
void getCustomChar(int fd, uint8_t location);
void shiftLeft(int fd);
void shiftRight(int fd);
void putChar(int fd, char character);
void changeBaud(int fd, uint32_t rate);
void showBaud(int fd);
void clearLine(int fd, uint8_t y);
