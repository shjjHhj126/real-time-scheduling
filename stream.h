#include <string.h>

#define stream_time 10000
#define periodic_task_input OnlinejobsOfHRT.txt
#define table_number 10

typedef struct task { // periodic task
    float utilization;
    unsigned int id,
                 phase,
                 period,
                 exe_time,
                 deadline;
    unsigned int *job;
    struct task *next;
} task;

typedef struct stream {
    task *head;
} stream;

void stream_init(stream *p);

int build_periodic_task_hashtable(stream *success, stream *fail);

void insert_head(task **head, task *node);

void en_list(task **head, task *node);

task *get_min(stream *a);
