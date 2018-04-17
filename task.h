#ifndef CANNODE_TASK_H
#define CANNODE_TASK_H

struct task
{
    void (*run)();
};

#endif // CANNODE_TASK_H
