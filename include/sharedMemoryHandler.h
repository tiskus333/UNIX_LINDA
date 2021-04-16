#ifndef SHARED_MEMORY_HANDLER_H
#define SHARED_MEMORY_HANDLER_H

#define MAX_TUPLE_SIZE 128
#define MAX_TUPLE_NUMBER 10
#include <stdio.h>
#include <stdlib.h>

#include <semaphore.h>
#include <sys/types.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/mman.h>

#include <algorithm>
#include <string.h>

class SharedMemoryHandler
{
private:
    class SharedMemory
    {
        public:
        sem_t sem_read;
        sem_t sem_write;
        unsigned char tupleSpace[MAX_TUPLE_NUMBER * MAX_TUPLE_SIZE];
    } sh_m;

public:
    void create(const char *name);
};

#endif