#include <linux/string.h>

#define SUCCESS         0
#define MAX_SIZE        20
#define NUM_OF_KEYS		89 /* approximately */

extern const char *kbd_US[NUM_OF_KEYS];

size_t kbd_char(unsigned int sc, char *buf);