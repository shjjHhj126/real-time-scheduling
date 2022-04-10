#include "schedule.h"

schedule periodic_task_schedule(list *stream)
{
    float utilization = 0.;
    unsigned int hyperperiod = 0;
    list p_list;
    list_init(&p_list);
    task *node = get_min(stream);
    while(node) {
        if(utilization + node->utilization <= 1.) {
            if(check_periodic_schedule(&p_list, &hyperperiod, node)) {
                en_list(&p_list.head, node, deadline);
                p_list.count++;
                utilization += node->utilization;
            }
        } else
            break;
        node = get_min(stream);
    }

    schedule plan;
    schedule_init(&plan);
    //expand_schedule(&plan);
    return plan;
}

void schedule_init(schedule *plan)
{
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
    for(int i=0;i<a->count; i++)
        list_for_each_entry_safe(node, safe, &a->hyperperiod[i].job_list, list)
            free(node);
    free(a->hyperperiod);
}

int check_periodic_schedule(list *p_list, unsigned int *hyperperiod, task *node)
{
    *hyperperiod = cal_hyperperiod(*hyperperiod, node->period);
    task *temp = p_list->head;
    status *head = NULL;
    for(int i=0; i < p_list->count; i++, temp = temp->next) {
        status *n_node = malloc(sizeof(*n_node));
        n_node->release_time = temp->phase;
        n_node->deadline = temp->deadline + temp->phase;
        n_node->remain_time = temp->exe_time;
        n_node->info = temp;
        n_node->next = NULL;
        en_status_list(&head, n_node, deadline);
    }
    status *n_node = malloc(sizeof(*n_node));
    n_node->release_time = node->phase;
    n_node->deadline = node->deadline + node->phase;
    n_node->remain_time = node->exe_time;
    n_node->info = node;
    n_node->next = NULL;
    en_status_list(&head, n_node, deadline);
    int time = 0;
    while(head) {
        status *now;
        remove_head(head, now);
        if(now->release_time > time) {
            unsigned int diff = now->release_time - time;
            while(diff) {
                status *comp = head;
                while(comp && comp->release_time > time)
                    comp = comp->next;
                if(comp) {
                    int spend = update_status(comp, time, diff);
                    if(spend < 0) {
                        free(comp);
                        while(head) {
                            comp = head;
                            head = head->next;
                            free(comp);
                        }
                        return 0;
                    } else {
                        if(comp->deadline > stream_time) {
                            remove_node(&head, comp);
                            free(comp);
                        }
                        time += spend;
                        diff -= spend;
                    }
                } else
                    diff = 0;
            }
            time = now->release_time;
        }
        int spend = update_status(now, time, now->remain_time);
        time += spend;
        if(now->deadline > stream_time)
            free(now);
        else 
            en_status_list(&head, now, deadline);
    }
    return 1;
}

int update_status(status *a, unsigned int now_time, unsigned int cost)
{
    int spend;
    if(a->deadline + cost - now_time < 0)
        return -1;
    if(a->remain_time <= cost) {
        spend = a->remain_time;
        a->remain_time = a->info->exe_time;
        a->release_time += a->info->period;
        a->deadline += a->info->period;
    } else {
        spend = cost;
        a->remain_time -= cost;
    }
    return spend;
}

unsigned int cal_hyperperiod(unsigned int a,unsigned int b)
{
    if(!a || !b)
        return a | b;
    unsigned int gcd = b;
    while(a % gcd) {
        unsigned int temp = a % gcd;
        a = gcd;
        gcd = temp;
    }
    return a * b / gcd;
}

void expand_schedule(schedule *plan)
{
    
}
