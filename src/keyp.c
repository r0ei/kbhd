#include <linux/kernel.h>
#include <linux/errno.h> // EINVAL

#include "keyp.h"

/* "translates" integer scancode data to simple ascii */
size_t kbd_char(unsigned int sc, char *buf)
{
	if (sc >= NUM_OF_KEYS)
		return -EINVAL;

	strncpy(buf, kbd_US[sc], strlen(kbd_US[sc]) + 1);

	return SUCCESS;
}

/* mapping of scancodes.
    NOTE: Mapping might be different between different keyboard layouts. */
const char *kbd_US[NUM_OF_KEYS] = {
  	"Error",
	"Escape",
	"1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
	"-", "=",
	"Back Space", "Tab",
	"q", "w", "e", "r", "t", "y", "u", "i", "o", "p",
	"[", "]",
	"Enter", "Left-Ctrl",
	"a", "s", "d", "f", "g", "h", "j", "k", "l",
	";", "\"", "`",
	"Left-Shift",
	"\\",
	"z", "x", "c", "v", "b", "n", "m",
	",", ".", "/",
	"Right-Shift",
	"*",
	"Left-Alt", "Space Bar", "CapsLock",
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10",
	"Num Lock",
	"Scroll Lock",
	"Home Key",
	"Up Arrow",
	"Page Up",
	"-",
	"Left Arrow",
	"Keypad 5",
	"Right Arrow",
	"+",
	"End Key",
	"Down Arrow",
	"Page Down",
	"Insert Key",
	"Delete Key",
	"SysRq",
	"Undefined", /* Cherry G80-0777 keyboard */
	"Undefined", // Telerate keyboard
	"F11",
	"F12"  /* 59-5a-...-7f are less common */
};