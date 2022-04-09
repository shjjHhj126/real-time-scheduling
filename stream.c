#include "stream.h"

void list_init(list *p)
{
    p->head = NULL;
    p->count = 0;
}

int build_periodic_task_hashtable(list *success, list *fail)
{
    FILE *fd = fopen(periodic_task_input, "r");
    if(!fd) {
        printf("ERROR : open periodic task input file\n");
        return -1;
    }
    char buffer[4096];
    int n;
    while(fgets(buffer, 4096, fd)) {
        task *node = malloc(sizeof(node));
        node->next = NULL;
        int min_exe = 0;
        for (n = 0;n<6;n++) {
            char *token;
            if(!(token = strtok(buffer, " ")))
                goto wrong_input;
            switch(n) {
                case 0:
                    node->id = atoi(token);
                    break;
                case 1:
                    node->phase = atoi(token);
                    break;
                case 2:
                    node->period = atoi(token);
                    break;
                case 3:
                    min_exe = atoi(token);
                    break;
                case 4:
                    node->exe_time = atoi(token);
                    break;
                case 5:
                    node->deadline = atoi(token);
                    break;
            }
            token = strtok(NULL, " ");
        }
        if(node->deadline > node->period)
            node->deadline = node->period;

        node->utilization = node->exe_time / node->period;

        memset(buffer, '\0', 4096);
        fgets(buffer, 4096, fd);
        fscanf(fd, "\n");
        if(min_exe > node->exe_time || 
           node->exe_time > node->deadline ||
           !*buffer) {
            insert_head(&fail->head, node);
            continue;
        }
        int job_num = (stream_time - node->phase) / node->period;
        node->job = malloc(sizeof(*node->job) * job_num);
        for(int i = 0;i < job_num;i++) {
            char *token;
            if(!(token = strtok(buffer, " ")))
                goto wrong_input;
            node->job[i] = atoi(token);
            token = strtok(NULL, " ");
        }
        success[(int)(node->utilization * 10)].head = NULL;
        success[(int)(node->utilization * 10)].count++;
        en_list(&success[(int)(node->utilization * 10)].head, node, utilization);
        continue;
wrong_input :
        printf("ERROR : wrong number of parameter in periodic task\n");
        fclose(fd);
        free(node);
        return -1;
    }
    fclose(fd);
    return 1;
}

void free_list(list *a)
{
    if(!a)
        return;
    task *temp = a->head;
    while(temp) {
        a->head = temp->next;
        free(temp->job);
        free(temp);
        temp = a->head;
    }
    a->count = 0;
}

void insert_head(task **head, task *node)
{
    node->next = *head;
    *head = node;
}

task *get_min(list *a)
{
    for(int i=0;i < table_number;i++)
        if(a[i].head) {
            task *temp = a[i].head;
            a[i].head = temp->next;
            temp->next = NULL;
            a[i].count--;
            return a[i].head;
        }
    return NULL;
}

task *task_cpy(const task *a) {
    task *node = malloc(sizeof(*node));
    node->utilization = a->utilization;
    node->id = a->id;
    node->phase = a->phase;
    node->period = a->period;
    node->exe_time = a->exe_time;
    node->deadline = a->deadline;
    node->job = malloc(sizeof(a->job));
    memcpy(node->job, a->job, sizeof(a->job));
    node->next = NULL;
}

