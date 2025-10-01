/*
 --------------------------------------------------
 File:    myword_ptest.c
 About:   public test driver
 Author:  HBF
 Version: 2025-01-21
 --------------------------------------------------
 */

#include <stdio.h>
#include <string.h>
#include "myword.h"

#define DICTIONARY_SIZE 2000
#define MAX_LINE_LEN 1000
#define MAX_WORDS 1000

char dictionary_tests[] = { 'a', 'b', 'c', 'd' };
char word_tests[][30] = { "this", "is", "data", "structure" };
char dictionaary_filename[40] = "common-english-words.txt"; //default stop word file
char testdata_filename[40] = "textdata.txt";   //default input file name

char *statsformat = "%s: %d\n";

void test_dictionary() {
	printf("------------------\n");
	printf("Test: create_dictionary, contain_word\n\n");

	FILE *fp = fopen(dictionaary_filename, "r");
	if (fp == NULL) {
		perror("open input file error");
		return;
	}
	char dictionary[DICTIONARY_SIZE] = { 0 };
	int wc = create_dictionary(fp, dictionary);
	printf("create_dictionary(): %d\n", wc);
    fclose(fp);
	//printf("dictionary(): %s\n", dictionary);
	int count = sizeof(word_tests) / sizeof *word_tests;
	for (int i = 0; i < count; i++) {
		printf("contain_word(%s): %d\n", word_tests[i],
					contain_word(dictionary, word_tests[i]));
	}
	printf("\n");
}


void test_process_words() {
	printf("------------------\n");
	printf("Test: process_words\n\n");

	FILE *fp = fopen(dictionaary_filename, "r");
	if (fp == NULL) {
		perror("open input file error");
		return;
	}
	char dictionary[DICTIONARY_SIZE] = { 0 };
	create_dictionary(fp, dictionary);
	fclose(fp);

	fp = fopen(testdata_filename, "r");
	if (fp == NULL) {
		perror("open input file error");
		return;
	}
	WORD words[MAX_WORDS];
	WORDSTATS ws = process_words(fp, words, dictionary);
	fclose(fp);

	printf(statsformat, "line_count", ws.line_count);
	printf(statsformat, "word_count", ws.word_count);
	printf(statsformat, "keyword_count", ws.keyword_count);

	printf("\n");
	for (int i = 0; i < ws.keyword_count; i++) {
		printf(statsformat, words[i].word, words[i].count);
	}
	printf("\n");
}

int main(int argc, char *args[]) {
	test_dictionary();
	test_process_words();
	return 0;
}

