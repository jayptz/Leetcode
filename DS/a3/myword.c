
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mystring.h"
#include "myword.h"

#define MAX_LINE_LEN 1000
#define MAX_WORDS 1000

/*
 * Define enumeration type BOOLEAN with value FALSE = 0 and TRUE 1.
 */

/*
 * Define structure type WORD consists of char word[20] to store a word, int count to hold frequency of the word.
 */

/*
 * Define structure type WORDSTATS consisting of int line_count, int word_count, and int keyword_count to represent
 * to represent the number of lines, number of all words, and the number of different non-common-word in text data.
 */


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
    int word_count = 0;
    int dict_pos = 0;
    
    while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
        str_trim(line);
        str_lower(line);
        
        char *word = strtok(line, " \t\n");
        while (word != NULL) {
            // Check if word is not already in dictionary
            if (!contain_word(dictionary, word)) {
                // Add word to dictionary (assuming dictionary is a simple concatenated string)
                if (dict_pos > 0) {
                    dictionary[dict_pos++] = '|'; // separator
                }
                strcpy(dictionary + dict_pos, word);
                dict_pos += strlen(word);
                word_count++;
            }
            word = strtok(NULL, " \t\n");
        }
    }
    
    dictionary[dict_pos] = '\0';
    return word_count;
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
    if (dictionary == NULL || word == NULL || strlen(dictionary) == 0) {
        return FALSE;
    }
    
    char *dict_copy = malloc(strlen(dictionary) + 1);
    strcpy(dict_copy, dictionary);
    
    char *dict_word = strtok(dict_copy, "|");
    while (dict_word != NULL) {
        if (strcmp(dict_word, word) == 0) {
            free(dict_copy);
            return TRUE;
        }
        dict_word = strtok(NULL, "|");
    }
    
    free(dict_copy);
    return FALSE;
}

/*
 * Process text data from a file for word statistic information of line count, word count, keyword count, and frequency of keyword.   
 * 
 * @param *fp -  FILE pointer of input text data file. .
 * @param *words  -  WORD array for keywords and their frequencies.
 * @param *dictionary  -  stop-word/common-word dictionary.    
 *                     
 * @return - WORDSTATS value of processed word stats information.   
 */
WORDSTATS process_words(FILE *fp, WORD *words, char *dictionary) {
    WORDSTATS stats = {0, 0, 0};
    char line[MAX_LINE_LEN];
    int word_index = 0;
    
    while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
        stats.line_count++;
        str_trim(line);
        str_lower(line);
        
        char *word = strtok(line, " \t\n");
        while (word != NULL) {
            stats.word_count++;
            
            // Check if word is not in dictionary (i.e., it's a keyword)
            if (!contain_word(dictionary, word)) {
                // Check if word already exists in words array
                int found = 0;
                for (int i = 0; i < word_index; i++) {
                    if (strcmp(words[i].word, word) == 0) {
                        words[i].count++;
                        found = 1;
                        break;
                    }
                }
                
                // If word not found, add it to words array
                if (!found && word_index < MAX_WORDS) {
                    strcpy(words[word_index].word, word);
                    words[word_index].count = 1;
                    word_index++;
                    stats.keyword_count++;
                }
            }
            
            word = strtok(NULL, " \t\n");
        }
    }
    
    return stats;
}
