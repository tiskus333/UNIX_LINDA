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

#include <iostream>
#include <pthread.h>

class SharedMemoryHandler {
private:
  static SharedMemoryHandler *instance;
  SharedMemoryHandler() = default;
  ~SharedMemoryHandler();
  const char *shm_name;

public:
  class SharedMemory {
  public:
    sem_t sem_is_resource_reserved; // blocks entering of further readers
    sem_t sem_counting_readers;     // used to check number of readers
    pthread_cond_t
        *cond_waiting_for_changes; // waiting on this one for new tuples
    pthread_condattr_t attrcond;
    unsigned char tupleSpace[MAX_TUPLE_NUMBER * MAX_TUPLE_SIZE];
  };
  static SharedMemoryHandler *getInstance();
  void create(const char *name);
  SharedMemory *open(const char *name);
};

#endif