#include "sharedMemoryHandler.h"

void SharedMemoryHandler::create(const char *name)
{
    int openedShm = shm_open(name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (openedShm == -1)
    {
        throw "Cannot create shared memory";
    }
    if (ftruncate(openedShm, sizeof(sh_m)) == -1)
    {
        throw "ftruncate error";
    }
    SharedMemory *shmMapped = (SharedMemory *)mmap(NULL, sizeof(*shmMapped), PROT_READ | PROT_WRITE, MAP_SHARED, openedShm, 0);
    if (shmMapped == MAP_FAILED)
    {
        throw "Cannot map memory";
    }
    if (sem_init(&shmMapped->sem_read, 1, 0) == -1)
    {
        throw "Cannot init sem_read";
    }

    if (sem_init(&shmMapped->sem_write, 1, 1) == -1)
    {
        throw "Cannot init sem_write";
    }
}

SharedMemoryHandler::SharedMemory* SharedMemoryHandler::open(const char *name)
{
    int openedShm = shm_open(name, O_RDWR, 0);
    if (openedShm == -1)
    {
        throw "Cannot open shared memory";
    }
    SharedMemory *shmMapped = (SharedMemory *)mmap(NULL, sizeof(*shmMapped), PROT_READ | PROT_WRITE,
                                                   MAP_SHARED, openedShm, 0);
    if (shmMapped == MAP_FAILED)
    {
        throw "Cannot map memory";
    }
    return shmMapped;
}

