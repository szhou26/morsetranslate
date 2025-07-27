/*
 *  decoding.c
 *
 *
 *  length of dot = 1 unit --> 3.33 ms
 *  length of dash = 3 units ---> 3.33 x 3 ms
 *  space between letters = 3 units ---> 3.33 x 3 ms (button not pressed)
 *  space between words = 7 units ---> 3.33 x 3 ms (button not pressed)
 *
*/
#include <MKL46Z4.h>
#include <string.h>
#include "decoding.h"

typedef struct {
	const char *code;  // Morse code
	char letter;       // alphanumeric
} MorseEntry;

const MorseEntry morse_table[] = {
    {".-", 'A'},   {"-...", 'B'}, {"-.-.", 'C'}, {"-..", 'D'},
    {".", 'E'},    {"..-.", 'F'}, {"--.", 'G'},  {"....", 'H'},
    {"..", 'I'},   {".---", 'J'}, {"-.-", 'K'},  {".-..", 'L'},
    {"--", 'M'},   {"-.", 'N'},   {"---", 'O'},  {".--.", 'P'},
    {"--.-", 'Q'}, {".-.", 'R'},  {"...", 'S'},  {"-", 'T'},
    {"..-", 'U'},  {"...-", 'V'}, {".--", 'W'},  {"-..-", 'X'},
    {"-.--", 'Y'}, {"--..", 'Z'}, {".----", '1'}, {"..---",'2'},
	{"...--", '3'}, {"....-",'4'}, {".....",'5'}, {"-....",'6'},
	{"--...",'7'}, {"---..",'8'}, {"----.",'9'}, {"-----",'0'}
};

char decode_morse(const char *code) {
    for (int i = 0; i < sizeof(morse_table) / sizeof(MorseEntry); i++) {
        if (strcmp(code, morse_table[i].code) == 0)
            return morse_table[i].letter;
    }
    return '?';
}
