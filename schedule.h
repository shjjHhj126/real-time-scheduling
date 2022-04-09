#include "stream.h"
#include "list.h"
typedef enum task_type {
    PERIODIC = 1,
    APERIODIC = 2,
    SPORADIC = 4
} task_type;

typedef struct event {
    task_type type;
    unsigned int start_time, end_time, id;
    list_head list;
} event;

typedef struct period {
    unsigned int using_time, total_time;
    list_head job_list;
} period;

typedef struct schedule {
    unsigned int count;
    list periodic_task,
         aperiodic_task,
         sporadic_task;
    period b_hyperperiod, *hyperperiod;
} schedule;

schedule periodic_task_schedule(list *stream);

void schedule_init(schedule *plan);

void free_schedule(schedule *a);

int check_periodic_schedule(list *p_list, task *node);

void expand_schedule(schedule *plan); 
