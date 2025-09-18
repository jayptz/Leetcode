#include <ctype.h>
#include "mychar.h"

/**
 * Determine the type of a char character.
 *
 * @param c - char type
 * @return - 0 if c is a digit 
             1 if c is an arithmetic operator
             2 if c is the left parenthsis (
             3 if c is the right parenthsis )
             4 if c is an English letter; 
             otherwise -1.
 */
int mytype(char c) {
    if (isdigit(c)) {
        return 0; 
    } else if (c == '+' || c == '-' || c == '*' || c == '/' || c == '%') {
        return 1;  
    } else if (c == '(') {
        return 2;  
    } else if (c == ')') {
        return 3;  
    } else if (isalpha(c)) {
        return 4;  
    } else {
        return -1; 
    }
}
 

/**
 * Flip the case of an English character.
 *
 * @param c - char type
 * @return -  c's upper/lower case letter if c is a lower/upper case English letter.
 */
char case_flip(char c) {
    if (isupper(c)) {
        return tolower(c);
    } else if (islower(c)) {
        return toupper(c);
    } else {
        return c;  
    }
}

/**
 * Convert digit character to the corresponding integer value.
 *
 * @param c - char type value
 * @return - its corresponding integer value if c is a digit character;
 *           otherwise -1.
 */
int digit_to_int(char c) {
    if (isdigit(c)) {
        return c - '0';  
    } else {
        return -1; 
    }
}
