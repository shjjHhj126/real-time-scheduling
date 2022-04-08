#include "stream.h"

void stream_init(stream *p)
{
    p->head = NULL;
}

int build_periodic_task_hashtable(stream *success, stream *fail)
{
    FILE *fd = fopen(periodic_task_input, "r");
    if(!fd)
        ERROR("open periodic task input file");
    char buffer[4096];
    int n;
    while(fscanf(fd, "%s\n",buffer)) {
        task *node = malloc(sizeof(node));
        node->next = NULL;
        int min_exe;
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
        fscanf(fd, "%s\n\n",buffer);
        if(min_exe > node->exe_time || 
           node->exe_time > node->deadline ||
           !*buffer) {
            insert_head(&fail->head, node);
            continue;
        }
        int job_num = (stream_time - phase) / period;
        node->job = malloc(sizeof(job) * job_num);
        for(int i = 0;i < job_num;i++) {
            char *token;
            if(!(token = strtok(buffer, " ")))
                goto wrong_input;
            node->job[i] = atoi(token);
            token = strtok(NULL, " ");
        }
        en_list(&success[node->utilization * 10].head, node);
        continue;
wrong_input :
        printf("ERROR : wrong number of parameter in periodic task\n");
        fclose(fd);
        free(fd);
        free(node);
        return -1;
    }
    fclose(fd);
    free(fd);
    return 1;
}

void insert_head(task **head, task *node)
{
    node->next = *head;
    *head = node;
}

void en_list(task **head, task *node)
{
    task **temp = head;
    while(*temp && (*temp)->utilization < node->utilization)
        temp = &(*temp)->next;
    node->next = *temp;
    *temp = node;
}

task *get_min(stream *a)
{
    for(int i=0;i < table_number;i++)
        if(a[i].head) {
            task *temp = a[i].head;
            a[i].head = temp->next;
            temp->next = NULL;
            return a[i].head;
        }
    return NULL;
}
