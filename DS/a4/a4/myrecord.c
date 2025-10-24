#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "myrecord.h"
#include "mysort.h"
/*
 * Define a structure named RECORD to hold a person's name of 20 characters and 
 * the score of float type.
*/

/*
 * Define a structure named STATS containing fields: int count, float mean, 
 * loat stddev (standard deviation), and float median.
*/

/*
 * Define a structure named GRADE to hold a string letter_grade at most 2 letters. 
*/

/*
 * Convert a percentage grade to letter grade defined by percentage ranges
 * A+=[90, 100], A=[85, 90), A-=[80, 85), B+=[77, 80), B=[73, 77) B=[70, 73),
 * C+=[67, 70), C=[63, 77), C-=[60, 63), D+=[57,60),D=[53,57),D=[50,53), F=[0,50).
 * Use binary search algorithm to find the corresponding letter grade.  
 * 
 * @param score -  percetage grade.
 *
 * @return - letter grade wrapped in GRADE structure type.
 */
GRADE grade(float score) {
    GRADE r = {"F"};

    if (score >= 90)
        strcpy(r.letter_grade, "A+");
    else if (score >= 85)
        strcpy(r.letter_grade, "A");
    else if (score >= 80)
        strcpy(r.letter_grade, "A-");
    else if (score >= 77)
        strcpy(r.letter_grade, "B+");
    else if (score >= 73)
        strcpy(r.letter_grade, "B");
    else if (score >= 70)
        strcpy(r.letter_grade, "B-");
    else if (score >= 67)
        strcpy(r.letter_grade, "C+");
    else if (score >= 63)
        strcpy(r.letter_grade, "C");
    else if (score >= 60)
        strcpy(r.letter_grade, "C-");
    else if (score >= 57)
        strcpy(r.letter_grade, "D+");
    else if (score >= 53)
        strcpy(r.letter_grade, "D");
    else if (score >= 50)
        strcpy(r.letter_grade, "D-");
    else
        strcpy(r.letter_grade, "F");

    return r;
}

/*
 *  Import record data from file and store name and store all record entries
 *  in the RECORD array passed by records, return the number of record count.
 *
 *  @param *fp -  FILE pointer to intput file.
 *  @param dataset - array of RECODR type to store record data.
 *  @return   - number of records
 */
int import_data(FILE *fp, RECORD *dataset) {
    if (!fp || !dataset) return 0;

    char line[256];
    int i = 0;

    while (fgets(line, sizeof(line), fp)) {
        char namebuf[64];      
        float score;

        
        if (sscanf(line, " %63[^,] , %f", namebuf, &score) == 2) {
            strncpy(dataset[i].name, namebuf, sizeof(dataset[i].name) - 1);
            dataset[i].name[sizeof(dataset[i].name) - 1] = '\0';

            dataset[i].score = score;
            i++;
        }
    }

    return i; 
}

/*
 *  Take the RECORD data array as input, compute the average score, standard deviation,
 *  median of the score values of the record data, and returns the STATS type value.
 *
 *  @param dataset -  input record data array.
 *  @param count -  the number of data record in dataset array.
 *  @return  -  stats value in STATS type.
 */
STATS process_data(RECORD *dataset, int n) {
    STATS stats = {0};

    if (dataset == NULL || n <= 0)
        return stats;

    float sum = 0.0f;
    for (int i = 0; i < n; i++)
        sum += dataset[i].score;
    float mean = sum / n;

    float var_sum = 0.0f;
    for (int i = 0; i < n; i++) {
        float diff = dataset[i].score - mean;
        var_sum += diff * diff;
    }
    float stddev = sqrt(var_sum / n);

    void *a[n]; 
    for (int i = 0; i < n; i++)
        a[i] = &dataset[i].score;

    select_sort(a, 0, n - 1);  

    float median;
    if (n % 2 == 1)
        median = *(float *)a[n / 2];
    else
        median = (*(float *)a[n / 2 - 1] + *(float *)a[n / 2]) / 2.0f;

    stats.count = n;
    stats.mean = mean;
    stats.stddev = stddev;
    stats.median = median;

    return stats;
}


/*
 *  This function takes output file named outfilename, RECORD array records, 
 *  and stats as inputs, prepare and write report of stats and grade to files.
 *  The records in report file are sorted in decreasing of scores.  
 *
 *  @param *fp -  FILE pointer to output file.
 *  @param *dataset - pointer to dataset array.
 *  @param stats - the stats value to be used in report.
 *  @return - returns 1 if successful; 0 if count < 1
 */
static int cmp2(void *x, void *y) {
    float b = ((RECORD *)x)->score;
    float a = ((RECORD *)y)->score;
    if (a > b) return 1;        
    else if (a < b) return -1;
    else return 0;
}

int report_data(FILE *fp, RECORD *dataset, STATS stats) {
    if (!fp || !dataset || stats.count < 1) return 0;

    int n = stats.count;

    fprintf(fp, "Record count: %d\n", n);
    fprintf(fp, "Average: %.2f\n", stats.mean);
    fprintf(fp, "Stddev: %.2f\n", stats.stddev);
    fprintf(fp, "Median: %.2f\n", stats.median);
    fprintf(fp, "\n");

    RECORD *p[n];
    for (int i = 0; i < n; i++) {
        p[i] = &dataset[i];
    }

    my_sort((void **)p, 0, n - 1, cmp2);

    for (int i = 0; i < n; i++) {
        GRADE g = grade(p[i]->score);
        fprintf(fp, "%s:%.1f,%s\n", p[i]->name, p[i]->score, g.letter_grade);
    }

    return 1;
}
