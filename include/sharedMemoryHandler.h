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

class SharedMemoryHandler {
private:
  static SharedMemoryHandler *instance;
  SharedMemoryHandler() = default;

public:
  class SharedMemory {
  public:
    sem_t sem_is_resource_reserved; // blocks entering of further readers
    sem_t sem_counting_readers;     // used to check number of readers
    sem_t sem_waiting_for_changes; // when when needed tuple is not found we are
                                   // waiting on this one
    unsigned char tupleSpace[MAX_TUPLE_NUMBER * MAX_TUPLE_SIZE];
  };
  static SharedMemoryHandler *getInstance();
  static void create(const char *name);
  SharedMemory *open(const char *name);
};

SharedMemoryHandler *SharedMemoryHandler::instance = 0;

#endif