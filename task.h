#ifndef CANNODE_TASK_H
#define CANNODE_TASK_H

struct task
{
    void (*run)();
    uint32_t period_ms;
};

#endif // CANNODE_TASK_H
