#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TaskQueue.h"

#define task_QUEUE_LOG(LEVEL, TAG, MESSAGE)

int task_queue_init(TaskQueue* queue, size_t size, int autoresize)
{
    queue->size = size;
    queue->autoresize = autoresize;
    queue->min = malloc(queue->size * sizeof(Task));
    queue->max = queue->min + queue->size;
    queue->start = queue->min;
    queue->end = queue->min;
    return 1;
}

int task_queue_resize(TaskQueue* queue, size_t new_size)
{
    Task* old_min = queue->min;
    size_t old_size = queue->size;

    queue->size = new_size;
    queue->min = malloc(queue->size * sizeof(Task));
    queue->max = queue->min + queue->size;
    queue->start += (queue->min - old_min);
    queue->end += (queue->min - old_min);
    memmove(queue->min, old_min, old_size * sizeof(Task));
    return 1;
}

int task_queue_push(TaskQueue* queue, Task func)
{
    if (queue->end >= queue->max)
    {
        if (queue->autoresize)
        {
            task_QUEUE_LOG("I", "TaskQueue", "TaskQueue.push: queue full, resizing.");
            task_queue_resize(queue, queue->size * 2);
        }
        else
        {
            task_QUEUE_LOG("I", "TaskQueue", "! TaskQueue.push: queue full.");
            return 0;
        }
    }
    *queue->end = func;
    queue->end++;
    return 1;
}

int task_queue_unshift(TaskQueue* queue, Task func)
{
    if (queue->end >= queue->max)
    {
        if (queue->autoresize)
        {
            task_QUEUE_LOG("I", "TaskQueue", "TaskQueue.unshift: queue full, resizing.");
            task_queue_resize(queue, queue->size * 2);
        }
        else
        {
            task_QUEUE_LOG("W", "TaskQueue", "TaskQueue.unshift: queue full.");
            return 0;
        }
    }
    if (queue->start <= queue->min)
    {
        task_QUEUE_LOG("I", "TaskQueue", "TaskQueue.unshift: queue at begining, moving data.");
        memmove(queue->start + 1, queue->start, sizeof(Task) * (queue->end - queue->start));
        queue->end++;
    }
    else
    {
        queue->start--;
    }
    *queue->start = func;
    return 1;
}

Task task_queue_shift(TaskQueue* queue)
{
    if (queue->start < queue->min)
    {
        task_QUEUE_LOG("W", "TaskQueue", "TaskQueue.shift: start is not consistent.");
        return 0;
    }
    if (queue->end > queue->max)
    {
        task_QUEUE_LOG("W", "TaskQueue", "TaskQueue.shift: end is not consistent.");
        return 0;
    }
    Task result = *queue->start;
    queue->start++;
    return result;
}

int task_queue_run(TaskQueue* queue)
{
    if (queue->start > queue->end)
    {
        task_QUEUE_LOG("W", "TaskQueue", "TaskQueue.shift: start and end are not consistent.");
        return 0;
    }
    while (queue->start < queue->end)
    {
        (task_queue_shift(queue))();
    }
    return 1;
}
