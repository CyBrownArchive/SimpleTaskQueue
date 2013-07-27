#ifndef __TASKQUEUE__
#define __TASKQUEUE__

#include <stdio.h>

typedef void (*Task) (void);

typedef struct {
    size_t size;
    int autoresize;
    Task* start;
    Task* end;
    Task* min;
    Task* max;
} TaskQueue;

int  task_queue_init    (TaskQueue* queue, size_t size, int autoresize);
int  task_queue_resize  (TaskQueue* queue, size_t new_size);
int  task_queue_push    (TaskQueue* queue, Task func);
int  task_queue_unshift (TaskQueue* queue, Task func);
Task task_queue_shift   (TaskQueue* queue);
int  task_queue_run     (TaskQueue* queue);

#endif
