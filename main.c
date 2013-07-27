#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <assert.h>

#include "taskqueue.h"

TaskQueue queue;

int counter = 0;

void add5()
{
    counter += 5;
}

void mul3()
{
    counter *= 3;
}
void mul4()
{
    counter *= 4;
}

void add2_mul3later()
{
    counter += 2;
    task_queue_push(&queue, mul3);
}

void add7_mul4now()
{
    counter += 7;
    task_queue_unshift(&queue, mul4);
}

void mul2()
{
    counter *= 2;
}


int main(void)
{
    task_queue_init(&queue, 1, 1);
    task_queue_push(&queue, add5);
    task_queue_push(&queue, mul2);
    task_queue_run(&queue); // 5, 10
    assert(counter == 10);

    task_queue_push(&queue, add5);
    task_queue_run(&queue); // 15
    assert(counter == 15);

    task_queue_push(&queue, add2_mul3later); // 17
    task_queue_push(&queue, add7_mul4now); // 17
    task_queue_run(&queue); // 17, 24, 96, 288
    assert(counter == 288);

    puts("SUCCESS ! All assertions passed !");
    return 0;
}
