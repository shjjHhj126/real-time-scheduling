#ifndef STREAM_H
#define STREAM_H
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define stream_time 10000
#define periodic_job_input "OnlinejobsOfHRT.txt"
#define periodic_task_input "TaskModel.txt"
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

typedef struct list {
    unsigned int count;
    task *head;
} list;

#define en_list(head, node, member)                    \
    do {                                               \
        task **temp = head;                            \
        while(*temp && (*temp)->member < node->member) \
            temp = &(*temp)->next;                     \
        node->next = *temp;                            \
        *temp = node;                                  \
    } while(0)

void list_init(list *p);

int build_periodic_task_hashtable(list *success, list *fail);

void free_list(list *a);

void insert_head(task **head, task *node);

task *get_min(list *a);

#endif
