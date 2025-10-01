/* myword.c */
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "myword.h"
#include "mystring.h"   /* for str_lower(char*) */

#ifndef MAX_LINE_LEN
#define MAX_LINE_LEN 1000
#endif

#ifndef MAX_WORDS
#define MAX_WORDS 1000
#endif

/*
 * Load word data from file, and insert words a directory represented by char array.
 * 
 * @param  FILE *fp -   file pointer to an opened text file
 * @param *dictionary - char pointer to a char array where dictionary words are stored. 
 *                      It's up to your design on how to store words in the char array.
 * @return - the number of words added into the dictionary.   
 */
int create_dictionary(FILE *fp, char *dictionary) {
    char line[MAX_LINE_LEN];
    int count = 0;

    while (fgets(line, sizeof(line), fp)) {
        char *tok = strtok(line, " ,.\n\t\r");
        while (tok) {
            str_lower(tok);

            strcat(dictionary, tok);
            strcat(dictionary, " ");

            count++;
            tok = strtok(NULL, " ,.\n\t\r");
        }
    }
    return count;
}

/*
 * Determine if a given word is contained in the given dictionary.  
 * 
 * @param *dictionary -  char pointer to a char array of given dictionary.
 * @param *word  -  pointer to a given word.  
 *                     
 * @return - TRUE if the word is in the dictionary, FALSE otherwise.   
 */
BOOLEAN contain_word(char *dictionary, char *word) {
    char temp[21];
    strncpy(temp, word, 20);
    temp[20] = '\0';
    str_lower(temp);

    char needle[24];
    snprintf(needle, sizeof(needle), "%s ", temp);

    return (strstr(dictionary, needle) != NULL) ? TRUE : FALSE;
}

/*
 * Process text data from a file for word statistic information of line count, word count, keyword count, 
 * and frequency of keyword.   
 * 
 * @param *fp -  FILE pointer of input text data file.
 * @param *words  -  WORD array for keywords and their frequencies.
 * @param *dictionary  -  stop-word/common-word dictionary.    
 *                     
 * @return - WORDSTATS value of processed word stats information.   
 */
WORDSTATS process_words(FILE *fp, WORD *words, char *dictionary) {
    WORDSTATS stats = (WORDSTATS){0,0,0};
    char line[MAX_LINE_LEN];

    while (fgets(line, sizeof(line), fp)) {
        stats.line_count++;

        char *tok = strtok(line, " ,.\n\t\r");
        while (tok) {
            stats.word_count++;

            str_lower(tok);

            if (!contain_word(dictionary, tok)) {
                int found = 0;
                for (int i = 0; i < stats.keyword_count; i++) {
                    if (strcmp(words[i].word, tok) == 0) {
                        words[i].count++;
                        found = 1;
                        break;
                    }
                }
                if (!found && stats.keyword_count < MAX_WORDS) {
                    strncpy(words[stats.keyword_count].word, tok, sizeof(words[stats.keyword_count].word) - 1);
                    words[stats.keyword_count].word[sizeof(words[stats.keyword_count].word) - 1] = '\0';
                    words[stats.keyword_count].count = 1;
                    stats.keyword_count++;
                }
            }

            tok = strtok(NULL, " ,.\n\t\r");
        }
    }
    return stats;
}
