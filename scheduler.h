#ifndef CANNODE_SCHEDULER_H
#define CANNODE_SCHEDULER_H

#include "task.h"

#define SCHEDULER_MAX_TASKS 32

void scheduler_init();

int scheduler_add_task(struct task *t);

struct task *scheduler_get_next_task();

#endif // CANNODE_SCHEDULER_H
