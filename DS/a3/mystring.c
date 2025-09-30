#include "mystring.h"
#include <ctype.h>

/**
 * Count the number words of given simple string. A word starts with an English charactor end with a charactor of space, tab, comma, or period.  
 *
 * @param s - char pointer to a string
 * @return - return the number of words. 
 */
int str_words(char *s) {
    int count = 0;
    int in_word = 0;
    
    while (*s) {
        if (isalpha(*s)) {
            if (!in_word) {
                count++;
                in_word = 1;
            }
        } else if (*s == ' ' || *s == '\t' || *s == ',' || *s == '.') {
            in_word = 0;
        }
        s++;
    }
    
    return count;
}

/**
 * Change every upper case English letter to its lower case of string passed by s.
 *
 * @param s - char pointer to a string
 * @return - return the number of actual flips.   
 */
int str_lower(char *s) {
    int flips = 0;
    
    while (*s) {
        if (isupper(*s)) {
            *s = tolower(*s);
            flips++;
        }
        s++;
    }
    
    return flips;
}

/**
 * Remove unnecessary space characters in a simple string passed by `s`
 *
 * @param s - char pointer to a string
 */
void str_trim(char *s) {
    char *start = s;
    char *end;
    
    // Skip leading whitespace
    while (isspace(*start)) {
        start++;
    }
    
    // If all characters are whitespace
    if (*start == '\0') {
        *s = '\0';
        return;
    }
    
    // Find end of string
    end = start;
    while (*end) {
        end++;
    }
    end--; // Move to last character
    
    // Skip trailing whitespace
    while (end > start && isspace(*end)) {
        end--;
    }
    
    // Move characters to beginning of string
    char *dest = s;
    while (start <= end) {
        *dest = *start;
        dest++;
        start++;
    }
    *dest = '\0';
}