/**
 * @file scheduler.c
 */
#include <stdint.h>
#include "err.h"
#include "systick.h"
#include "scheduler.h"

static struct task * s_tasks[SCHEDULER_MAX_TASKS];
static struct task s_idle_task;
static uint8_t s_num_tasks;
static uint8_t s_current_task;


void idle()
{
    systick_idle();
}

void scheduler_init()
{
    s_idle_task.run = idle;

    // idle task is always task 0
    scheduler_add_task(&s_idle_task);
}

int scheduler_add_task(struct task *t)
{
    if (s_num_tasks >= SCHEDULER_MAX_TASKS)
        return CANNODE_ERROR;

    s_tasks[s_num_tasks++] = t;

    return CANNODE_OK;
}

struct task *scheduler_get_next_task()
{
    s_current_task++;
    if (s_current_task == s_num_tasks)
        s_current_task = 0;

    return s_tasks[s_current_task];
}
