#include "sharedMemoryHandler.h"

SharedMemoryHandler *SharedMemoryHandler::instance = nullptr;

SharedMemoryHandler *SharedMemoryHandler::getInstance() {
  if (!instance)
    instance = new SharedMemoryHandler;
  return instance;
}
SharedMemoryHandler::~SharedMemoryHandler() { delete instance; }

void SharedMemoryHandler::create(const char *name) {
  shm_unlink(name);
  int openedShm = shm_open(name, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
  if (openedShm == -1) {
    throw "Cannot create shared memory";
  }
  if (ftruncate(openedShm, sizeof(SharedMemory)) == -1) {
    throw "ftruncate error";
  }
  SharedMemory *shmMapped =
      (SharedMemory *)mmap(NULL, sizeof(*shmMapped), PROT_READ | PROT_WRITE,
                           MAP_SHARED, openedShm, 0);
  if (shmMapped == MAP_FAILED) {
    throw "Cannot map memory";
  }

  if (sem_init(&shmMapped->sem_is_resource_reserved, 1, 1) == -1) {
    throw "Cannot init sem_is_resource_reserved";
  }

  if (sem_init(&shmMapped->sem_counting_readers, 1, 0) == -1) {
    throw "Cannot init sem_counting_readers";
  }

  if (pthread_condattr_init(&shmMapped->attrcond) != 0)
    throw "Cannot init conditional variable attribute";

  if (pthread_condattr_setpshared(&shmMapped->attrcond,
                                  PTHREAD_PROCESS_SHARED) != 0)
    throw "Cannot share conditional variable";

  if (pthread_cond_init(&shmMapped->cond_waiting_for_changes,
                        &shmMapped->attrcond) != 0)
    throw "Cannot init conditional variable";

  //shmMapped->cond_waiting_for_changes = PTHREAD_COND_INITIALIZER;

  if(pthread_mutexattr_init(&shmMapped->attrmutex) != 0)
    throw "Cannot init mutex attribute";
 
  if (pthread_mutexattr_setpshared(&shmMapped->attrmutex,
                                  PTHREAD_PROCESS_SHARED) != 0)
    throw "Cannot share mutex";

  if(pthread_mutex_init(&shmMapped->mutex_waiting_for_changes, &shmMapped->attrmutex) != 0)
    throw "Cannot init mutex";
    
  //shmMapped->mutex_waiting_for_changes = PTHREAD_MUTEX_INITIALIZER;
  /* Clean up. */
  // pthread_cond_destroy(cond_waiting_for_changes);
  // pthread_condattr_destroy(&attrcond);
  // delete shmMapped->cond_waiting_for_changes
}

SharedMemoryHandler::SharedMemory *SharedMemoryHandler::open(const char *name) {
  int openedShm = shm_open(name, O_RDWR, 0);
  if (openedShm == -1) {
    throw "Cannot open shared memory";
  }
  SharedMemory *shmMapped =
      (SharedMemory *)mmap(NULL, sizeof(*shmMapped), PROT_READ | PROT_WRITE,
                           MAP_SHARED, openedShm, 0);
  if (shmMapped == MAP_FAILED) {
    throw "Cannot map memory";
  }
  return shmMapped;
}
