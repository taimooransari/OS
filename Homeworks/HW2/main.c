#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// process control block (PCB)
struct metrics
{
    float throughput_total;
    float response_total;
    float turnaround_total;
    float total_process;
};

typedef struct metrics metrics;

// process control block (PCB)
struct pcb
{
    unsigned int pid;
    char pname[20];
    unsigned int ptimeleft;
    unsigned int ptimearrival;
    int responded;
};

typedef struct pcb pcb;

// queue node
struct dlq_node
{
    struct dlq_node *pfwd;
    struct dlq_node *pbck;
    struct pcb *data;
};

typedef struct dlq_node dlq_node;

// queue
struct dlq
{
    struct dlq_node *head;
    struct dlq_node *tail;
};

typedef struct dlq dlq;

// function to add a pcb to a new queue node
dlq_node *get_new_node(pcb *ndata)
{
    if (!ndata)
        return NULL;

    dlq_node *new = malloc(sizeof(dlq_node));
    if (!new)
    {
        fprintf(stderr, "Error: allocating memory\n");
        exit(1);
    }

    new->pfwd = new->pbck = NULL;
    new->data = ndata;
    return new;
}

// function to add a node to the tail of queue
void add_to_tail(dlq *q, dlq_node *new)
{
    if (!new)
        return;

    if (q->head == NULL)
    {
        if (q->tail != NULL)
        {
            fprintf(stderr, "DLList inconsitent.\n");
            exit(1);
        }
        q->head = new;
        q->tail = q->head;
    }
    else
    {
        new->pfwd = q->tail;
        new->pbck = NULL;
        new->pfwd->pbck = new;
        q->tail = new;
    }
}

// function to remove a node from the head of queue
dlq_node *remove_from_head(dlq *const q)
{
    if (q->head == NULL)
    { // empty
        if (q->tail != NULL)
        {
            fprintf(stderr, "DLList inconsitent.\n");
            exit(1);
        }
        return NULL;
    }
    else if (q->head == q->tail)
    { // one element
        if (q->head->pbck != NULL || q->tail->pfwd != NULL)
        {
            fprintf(stderr, "DLList inconsitent.\n");
            exit(1);
        }

        dlq_node *p = q->head;
        q->head = NULL;
        q->tail = NULL;

        p->pfwd = p->pbck = NULL;
        return p;
    }
    else
    { // normal
        dlq_node *p = q->head;
        q->head = q->head->pbck;
        q->head->pfwd = NULL;

        p->pfwd = p->pbck = NULL;
        return p;
    }
}

// function to print our queue
void print_q(const dlq *q)
{
    dlq_node *n = q->head;
    if (n == NULL)
        return;

    while (n)
    {
        printf("%s(%d),", n->data->pname, n->data->ptimeleft);
        n = n->pbck;
    }
}

// function to check if the queue is empty
int is_empty(const dlq *q)
{
    if (q->head == NULL && q->tail == NULL)
        return 1;
    else if (q->head != NULL && q->tail != NULL)
        return 0;
    else
    {
        fprintf(stderr, "Error: DLL queue is inconsistent.");
        exit(1);
    }
}

// function to sort the queue on completion time
void sort_by_timetocompletion(const dlq *q)
{
    // bubble sort
    dlq_node *start = q->tail;
    dlq_node *end = q->head;

    while (start != end)
    {
        dlq_node *node = start;
        dlq_node *next = node->pfwd;

        while (next != NULL)
        {
            if (node->data->ptimeleft < next->data->ptimeleft)
            {
                // do a swap
                pcb *temp = node->data;
                node->data = next->data;
                next->data = temp;
            }
            node = next;
            next = node->pfwd;
        }
        end = end->pbck;
    }
}

// function to sort the queue on arrival time
void sort_by_arrival_time(const dlq *q)
{
    // bubble sort
    dlq_node *start = q->tail;
    dlq_node *end = q->head;

    while (start != end)
    {
        dlq_node *node = start;
        dlq_node *next = node->pfwd;

        while (next != NULL)
        {
            if (node->data->ptimearrival < next->data->ptimearrival)
            {
                // do a swap
                pcb *temp = node->data;
                node->data = next->data;
                next->data = temp;
            }
            node = next;
            next = node->pfwd;
        }
        end = end->pbck;
    }
}

// function to tokenize the one row of data
pcb *tokenize_pdata(char *buf)
{
    pcb *p = (pcb *)malloc(sizeof(pcb));
    if (!p)
    {
        fprintf(stderr, "Error: allocating memory.\n");
        exit(1);
    }

    char *token = strtok(buf, ":\n");
    if (!token)
    {
        fprintf(stderr, "Error: Expecting token pname\n");
        exit(1);
    }
    strcpy(p->pname, token);

    token = strtok(NULL, ":\n");
    if (!token)
    {
        fprintf(stderr, "Error: Expecting token pid\n");
        exit(1);
    }
    p->pid = atoi(token);

    token = strtok(NULL, ":\n");
    if (!token)
    {
        fprintf(stderr, "Error: Expecting token duration\n");
        exit(1);
    }

    p->ptimeleft = atoi(token);

    token = strtok(NULL, ":\n");
    if (!token)
    {
        fprintf(stderr, "Error: Expecting token arrival time\n");
        exit(1);
    }
    p->ptimearrival = atoi(token);

    token = strtok(NULL, ":\n");
    if (token)
    {
        fprintf(stderr, "Error: Oh, what've you got at the end of the line?\n");
        exit(1);
    }

    return p;
}

// Helper function to help populate ready queue
void get_ready_queue(dlq *const p_fq, int *p_time, dlq *ready_queue, char *pname)
{

    dlq_node *temp_head = p_fq->head;
    while (temp_head)
    {

        if (*p_time == temp_head->data->ptimearrival + 1 && strcmp(temp_head->data->pname, pname) != 0)
        {
            add_to_tail(ready_queue, get_new_node(temp_head->data));
        }

        temp_head = temp_head->pbck;
    }

    if (!is_empty(ready_queue) && strcmp(ready_queue->head->data->pname, pname) == 0)
    {
        remove_from_head(ready_queue);
    }
}

// implement the FIFO scheduling code
void sched_FIFO(dlq *const p_fq, int *p_time, metrics *time_data)
{
    *p_time = 1;

    dlq_node *temp_head = p_fq->head;
    sort_by_arrival_time(p_fq);
    dlq ready_queue;
    ready_queue.head = NULL;
    ready_queue.tail = NULL;

    while (*p_time <= temp_head->data->ptimearrival)
    {
        printf("%d:idle:empty:\n", *p_time);
        *p_time += 1;
    }

    dlq_node *nd = p_fq->head;
    while (nd)
    {
        time_data->response_total -= nd->data->ptimearrival;
        nd = nd->pbck;
    }

    while (temp_head)
    {

        if (temp_head->data->ptimeleft > 0)
        {
            get_ready_queue(p_fq, p_time, &ready_queue, temp_head->data->pname);
            printf("%d:%s:", *p_time, temp_head->data->pname);

            if (is_empty(&ready_queue))
            {
                printf("empty:\n");
            }
            else
            {
                print_q(&ready_queue);
                printf(":\n");
            }
            temp_head->data->ptimeleft -= 1;
            *p_time += 1;
        }
        else
        {
            time_data->turnaround_total += *p_time - temp_head->data->ptimearrival - 1;
            time_data->response_total += *p_time;
            temp_head = temp_head->pbck;
            if (!temp_head)
            {
                time_data->response_total -= *p_time;
            }
        }
    }

    time_data->throughput_total = *p_time - 1;
}

// implement the STCF scheduling code
void sched_STCF(dlq *const p_fq, int *p_time, metrics *time_data)
{
    *p_time = 1;

    dlq_node *temp_head = p_fq->head;
    sort_by_arrival_time(p_fq);
    dlq ready_queue;
    ready_queue.head = NULL;
    ready_queue.tail = NULL;

    while (*p_time <= temp_head->data->ptimearrival)
    {
        printf("%d:idle:empty:\n", *p_time);
        *p_time += 1;
    }

    dlq_node *nd = p_fq->head;
    while (nd)
    {
        time_data->response_total -= nd->data->ptimearrival;
        nd = nd->pbck;
    }

    while (temp_head)
    {

        if (temp_head->data->ptimeleft > 0)
        {
            get_ready_queue(p_fq, p_time, &ready_queue, temp_head->data->pname);
            sort_by_timetocompletion(&ready_queue);

            if (is_empty(&ready_queue))
            {
                printf("%d:%s:", *p_time, temp_head->data->pname);
                printf("empty:\n");
            }
            else
            {
                if (temp_head->data->ptimeleft > ready_queue.head->data->ptimeleft)
                {
                    dlq_node *d = remove_from_head(&ready_queue);
                    add_to_tail(&ready_queue, get_new_node(temp_head->data));
                    temp_head = d;
                    sort_by_timetocompletion(&ready_queue);
                }

                printf("%d:%s:", *p_time, temp_head->data->pname);
                print_q(&ready_queue);
                printf(":\n");
            }

            temp_head->data->ptimeleft -= 1;
            if (temp_head->data->responded == 0)
            {
                time_data->response_total += *p_time - 1;
                temp_head->data->responded = 1;
            }
            *p_time += 1;
        }
        else
        {
            time_data->turnaround_total += *p_time - temp_head->data->ptimearrival - 1;

            if (!is_empty(&ready_queue))
            {

                temp_head = ready_queue.head;
            }
            else
            {

                temp_head = temp_head->pbck;
            }
        }
    }
    time_data->throughput_total = *p_time - 1;
}

// implement the SJF scheduling code
void sched_SJF(dlq *const p_fq, int *p_time, metrics *time_data)
{
    *p_time = 1;

    dlq_node *temp_head = p_fq->head;
    sort_by_arrival_time(p_fq);
    dlq ready_queue;
    ready_queue.head = NULL;
    ready_queue.tail = NULL;

    while (*p_time <= temp_head->data->ptimearrival)
    {
        printf("%d:idle:empty:\n", *p_time);
        *p_time += 1;
    }

    dlq_node *nd = p_fq->head;
    while (nd)
    {
        time_data->response_total -= nd->data->ptimearrival;
        nd = nd->pbck;
    }

    while (temp_head)
    {

        if (temp_head->data->ptimeleft > 0)
        {
            if (temp_head->data->responded == 0)
            {
                time_data->response_total += *p_time - 1;
                temp_head->data->responded = 1;
            }

            get_ready_queue(p_fq, p_time, &ready_queue, temp_head->data->pname);
            sort_by_timetocompletion(&ready_queue);
            printf("%d:%s:", *p_time, temp_head->data->pname);

            if (is_empty(&ready_queue))
            {
                printf("empty:\n");
            }
            else
            {
                print_q(&ready_queue);
                printf(":\n");
            }

            temp_head->data->ptimeleft -= 1;
            *p_time += 1;
        }
        else
        {
            time_data->turnaround_total += *p_time - temp_head->data->ptimearrival - 1;

            if (!is_empty(&ready_queue))
            {
                temp_head = ready_queue.head;
            }
            else
            {
                temp_head = temp_head->pbck;
            }
        }
    }

    time_data->throughput_total = *p_time - 1;
}

void sched_RR(dlq *const p_fq, int *p_time, metrics *time_data)
{

    *p_time = 1;

    sort_by_arrival_time(p_fq);

    dlq_node *temp_head = p_fq->head;
    dlq ready_queue;
    ready_queue.head = NULL;
    ready_queue.tail = NULL;

    while (*p_time <= temp_head->data->ptimearrival)
    {
        printf("%d:idle:empty:\n", *p_time);
        *p_time += 1;
    }

    dlq_node *nd = p_fq->head;
    while (nd)
    {
        nd->data->responded = 0;
        time_data->response_total -= nd->data->ptimearrival;
        nd = nd->pbck;
    }

    get_ready_queue(p_fq, p_time, &ready_queue, temp_head->data->pname);
    while (temp_head->data->ptimeleft > 0 || !is_empty(&ready_queue))
    {

        temp_head->data->ptimeleft -= 1;
        if (temp_head->data->ptimeleft <= 0)
        {
            time_data->turnaround_total += *p_time - temp_head->data->ptimearrival;
        }
        if (temp_head->data->responded == 0)
        {
            time_data->response_total += *p_time - 1;
            temp_head->data->responded = 1;
        }

        if (is_empty(&ready_queue))
        {
            printf("%d:%s:", *p_time, temp_head->data->pname);
            printf("empty:\n");
            *p_time += 1;
            get_ready_queue(p_fq, p_time, &ready_queue, temp_head->data->pname);
        }
        else
        {

            printf("%d:%s:", *p_time, temp_head->data->pname);
            print_q(&ready_queue);
            printf(":\n");

            dlq_node *d = remove_from_head(&ready_queue);
            if (temp_head->data->ptimeleft > 0)
            {
                add_to_tail(&ready_queue, get_new_node(temp_head->data));
            }
            temp_head->data = d->data;
            *p_time += 1;
            get_ready_queue(p_fq, p_time, &ready_queue, temp_head->data->pname);
        }
    }

    time_data->throughput_total = *p_time - 1;
}

int main()
{
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int N = 0;
    char tech[20] = {'\0'};
    char buffer[100] = {'\0'};
    scanf("%d", &N);
    scanf("%s", tech);

    dlq queue;
    queue.head = NULL;
    queue.tail = NULL;
    for (int i = 0; i < N; ++i)
    {
        scanf("%s\n", buffer);
        // printf("%s\n", buffer);
        pcb *p = tokenize_pdata(buffer);
        add_to_tail(&queue, get_new_node(p));
    }
    // print_q(&queue);
    // printf("\n");
    int system_time = 0;
    sort_by_arrival_time(&queue);
    // print_q (&queue);

    metrics time_data;
    time_data.total_process = N;
    time_data.response_total = 0;
    time_data.turnaround_total = 0;
    time_data.throughput_total = 0;

    // run scheduler
    if (!strncmp(tech, "FIFO", 4))
        sched_FIFO(&queue, &system_time, &time_data);
    else if (!strncmp(tech, "SJF", 3))
        sched_SJF(&queue, &system_time, &time_data);
    else if (!strncmp(tech, "STCF", 4))
        sched_STCF(&queue, &system_time, &time_data);
    else if (!strncmp(tech, "RR", 2))
        sched_RR(&queue, &system_time, &time_data);
    else
        fprintf(stderr, "Error: unknown POLICY\n");

    printf("Processes: %f \n", time_data.total_process);
    printf("THROUGHPUT: %f\n", time_data.total_process / time_data.throughput_total);
    printf("TURNAROUND: %f\n", time_data.turnaround_total / time_data.total_process);
    printf("RESPONSE: %f\n", time_data.response_total / time_data.total_process);
    return 0;
}