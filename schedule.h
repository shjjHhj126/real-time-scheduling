#include <stdio.h>

#define periodic_task_file OnlinejobsOfHRT.txt
#define table_number 10;

typedef struct customer {
    unsigned float utilization; 
    unsigned int id,
                 phase,
                 period,
                 exe_time,
                 deadline;
    unsigned int *job;
    struct customer *next;
} customer;

typedef struct stream {
    struct customer *first;
    int count; // the total number of customer
} stream; 

void build_table(stream *, stream *);

