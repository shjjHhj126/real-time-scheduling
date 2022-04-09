#include "schedule.h"

schedule periodic_task_schedule(list *stream)
{
    int odd_n = 0, even_n = 0;
    float odd_utilization = 0., even_utilization = 0.;
    list odd, even;
    list_init(&odd);
    list_init(&even);

    task *node = get_min(stream);
    while(node) {
        if(node->period & 1) {
            int used = 0;
            if(odd_utilization + node->utilization <= 1.) {
                if(check_periodic_schedule(&odd, node)) {
                    en_list(&odd.head, node, deadline);
                    odd.count++;
                    odd_n++;
                    used = 1;
                    odd_utilization += node->utilization;
                }
            }
            if(even_utilization + node->utilization <= 1.) {
                if(check_periodic_schedule(&even, node)) {
                    if(used) {
                        task *n_node = task_cpy(node);
                        en_list(&even.head, n_node, deadline);
                    }
                    else
                        en_list(&even.head, node, deadline);
                    even.count++;
                    even_n++;
                    even_utilization += node->utilization;
                }
            }
        } else if(node->phase & 1) {
            if(odd_utilization + node->utilization <= 1.) {
                if(check_periodic_schedule(&odd, node)) {
                    en_list(&odd.head, node, deadline);
                    odd.count++;
                    odd_n++;
                    odd_utilization += node->utilization;
                }
            }
        } else {
            if(even_utilization + node->utilization <= 1.) {
                if(check_periodic_schedule(&even, node)) {
                    en_list(&even.head, node, deadline);
                    even.count++;
                    even_n++;
                    even_utilization += node->utilization;
                }
            }
        }
        node = get_min(stream);
    }

    schedule plan;
    schedule_init(&plan);

    if(!(odd_n | even_n))
        return plan;

    if(odd_n > even_n ||
        (odd_n == even_n && odd_utilization <= even_utilization)) {
        free_list(&even);
        plan.periodic_task.head = odd.head;
        plan.periodic_task.count = odd.count;
    } else {
        free_list(&odd);
        even_plan.periodic_task.head = even.head;
        even_plan.periodic_task.count = even.count;
    }
    expand_schedule(&plan);
    return plan;
}

void schedule_init(schedule *plan)
{
    INIT_LIST_HEAD(&plan->b_hyperperiod.job_list);
    plan->b_hyperperiod.using_time = plan->b_hyperperiod.total_time = 0;
    plan->hyperperiod = malloc(sizeof(*plan->hyperperiod));
    plan->hyperperiod->using_time = plan->hyperperiod->total_time = 0;
    INIT_LIST_HEAD(&plan->hyperperiod->job_list);
    plan->count = 0;
}

void free_schedule(schedule *a) {
    if(!a)
        return;
    free_list(&a->periodic_task);
    free_list(&a->aperiodic_task);
    free_list(&a->sporadic_task);
    event *node, *safe;
    list_for_each_entry_safe(node, safe, &a->b_hyperperiod.job_list, list)
            free(node);
    for(int i=0;i<a->count; i++)
        list_for_each_entry_safe(node, safe, &a->hyperperiod[i].job_list, list)
            free(node);
    free(a->hyperperiod);
}

int check_periodic_schedule(list *p_list, task *node)
{
}
