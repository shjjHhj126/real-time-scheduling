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
    period *hyperperiod;
} schedule;

typedef struct status {
    unsigned int remain_time,
                 release_time,
                 deadline;
    task *info;
    struct status *next;
} status;

#define remove_head(head, node) \
    do{                         \
        if(head) {              \
            node = head;        \
            head = head->next;  \
        }                       \
    } while(0)

#define remove_node(head, node)    \
    do {                           \
        status **temp = head;      \
        while(*temp != node)       \
            temp = &(*temp)->next; \
        *temp = (*temp)->next;     \
    } while(0)


#define en_status_list(head, node, member)             \
    do {                                               \
        status **temp = head;                          \
        while(*temp && (*temp)->member < node->member) \
            temp = &(*temp)->next;                     \
        node->next = *temp;                            \
        *temp = node;                                  \
    } while(0)

schedule periodic_task_schedule(list *stream);

void schedule_init(schedule *plan);

void free_schedule(schedule *a);

int check_periodic_schedule(list *p_list, unsigned int *hyperperiod,task *node);

void expand_schedule(schedule *plan);

unsigned int cal_hyperperiod(unsigned int a,unsigned int b);

int update_status(status *a, unsigned int now_time, unsigned int cost);
