#include  "schedule.h"

//void print_periodic_info(schedule *plan, list *fail);

int main() {
    list *success, p_fail;
    success = malloc(sizeof(*success) * table_number);
    for(int i=0;i < table_number;i++)
        list_init(&success[i]);
    list_init(&p_fail);
    build_periodic_task_hashtable(success, &p_fail);
    schedule plan = periodic_task_schedule(success);
    for(int i=0;i < table_number;i++)
        free_list(&success[i]);
    //print_periodic_info(&plan, &fail);
    /* 
       add aperiodic and sporadic implement here
       */
    free(success);
    return 0;
}
