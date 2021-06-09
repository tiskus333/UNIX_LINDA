#include <semaphore.h>
#include <pthread.h>

#include <iostream>
#include <unistd.h>

#include "lindaTuples.h"
#include "sharedMemoryHandler.h"

using namespace std;

class LindaSpace
{
private:
    bool debug;

    LindaTuples tuples;
    //exchange space
    char *space;
    size_t space_size;

    //semaphores
    sem_t *sem_is_resource_reserved; // blocks entering of further readers
    sem_t *sem_counting_readers;     //used to check number of readers
    // when when needed tuple is not found we are semaphore_value on this one
    pthread_cond_t *cond_waiting_for_changes;
    pthread_mutex_t *mutex_waiting_for_changes;

    void notify_waiting_for_changes()
    {
        if(pthread_cond_broadcast(cond_waiting_for_changes) != 0){
            std::cout << "bad_brodcast\n";
        }
    }

    bool search_for_data(const RegexTuple &tuple)
    {
        tuples.deserialize(space);
        Tuple result = tuples.read(tuple);
        return Tuple{"waiting"} != result;
    }

    void save_data(const Tuple &tuple)
    {
        if (debug)
        {
            cout << "writing tuple: ";
            printTuple(tuple);
        }
        if (!tuples.deserialize(space))
            throw "Tuple not properly deserialized ";
        if (!tuples.output(tuple))
            throw "Tuple not properly saved ";
        if (!tuples.serialize(space, space_size))
            throw "Tuple not properly serialized ";
    }

public:
    LindaSpace(bool debug);
    LindaSpace(char* mem, sem_t* sem_is_resource_reserved, sem_t* sem_counting_readers, pthread_cond_t* cond_waiting_for_changes, pthread_mutex_t* mutex_waiting_for_c);
    ~LindaSpace();

    void write(const Tuple &tuple)
    {
        sem_wait(sem_is_resource_reserved);
       
        int readers_left;
        sem_getvalue(sem_counting_readers, &readers_left);
        while (readers_left != 0)
        {
            if (debug)
                cout << "waiting in write, readers_left: " << readers_left << "\n";
            sem_getvalue(sem_counting_readers, &readers_left);
        }
        save_data(tuple);

        notify_waiting_for_changes();

        sem_post(sem_is_resource_reserved);
    }

    Tuple read(const RegexTuple &regex, int timeout)
    {

        sem_wait(sem_is_resource_reserved);
        sem_post(sem_counting_readers);
        sem_post(sem_is_resource_reserved);

        bool found_searched_element = search_for_data(regex);
        while (!found_searched_element)
        {
            if (sem_trywait(sem_counting_readers))
                cout << "ERROR 1\n";

            struct timespec ts;
            if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
                perror("clock_gettime");
                exit(EXIT_FAILURE);
            }
            ts.tv_sec += timeout;
            
            int rc = pthread_cond_timedwait(cond_waiting_for_changes,
                              mutex_waiting_for_changes, &ts);

            if (rc == ETIMEDOUT) {

                pthread_mutex_unlock(mutex_waiting_for_changes);
                throw std::runtime_error("Timeout");
            }

            sem_wait(sem_is_resource_reserved);
            sem_post(sem_counting_readers);
            sem_post(sem_is_resource_reserved);
            found_searched_element = search_for_data(regex);
        }
        if (debug)
            cout << "reading: \n";

        Tuple result = tuples.read(regex);

        if (sem_trywait(sem_counting_readers))
            cout
                << "ERROR 2\n";
                
        pthread_mutex_unlock(mutex_waiting_for_changes);

        return result;
    }

    void remove(const RegexTuple &regex, int timeout)
    {
        sem_wait(sem_is_resource_reserved);
        int readers_left;
        sem_getvalue(sem_counting_readers, &readers_left);
        while (readers_left != 0)
        {
            if (debug)
                cout << "waiting in remove, readers_left: " << readers_left << "\n";
            sem_getvalue(sem_counting_readers, &readers_left);
        }

        bool found_searched_element = search_for_data(regex);

        while (!found_searched_element)
        {
            sem_post(sem_is_resource_reserved);

            struct timespec ts;
            if (clock_gettime(CLOCK_REALTIME, &ts) == -1) {
                perror("clock_gettime");
                exit(EXIT_FAILURE);
            }
            ts.tv_sec += timeout;

            int rc = pthread_cond_timedwait(cond_waiting_for_changes,
                              mutex_waiting_for_changes, &ts);

            if (rc == ETIMEDOUT) {

                pthread_mutex_unlock(mutex_waiting_for_changes);
                throw std::runtime_error("Timeout");
            } 

            sem_wait(sem_is_resource_reserved);

            found_searched_element = search_for_data(regex);
        }
        if (debug)
            cout << "deleted \n";
        Tuple result = tuples.input(regex);
        tuples.serialize(space, space_size);

        sem_post(sem_is_resource_reserved);

        pthread_mutex_unlock(mutex_waiting_for_changes);
    }
};

LindaSpace::LindaSpace(bool debug = false)
{
    space_size = MAX_TUPLE_NUMBER * MAX_TUPLE_SIZE;
    space = new char[space_size];
    sem_is_resource_reserved = new sem_t;
    sem_counting_readers = new sem_t;
    cond_waiting_for_changes = new pthread_cond_t;
    *cond_waiting_for_changes = PTHREAD_COND_INITIALIZER;
    mutex_waiting_for_changes = new pthread_mutex_t;
    *mutex_waiting_for_changes = PTHREAD_MUTEX_INITIALIZER;

    sem_init(sem_is_resource_reserved, 0, 1);
    sem_init(sem_counting_readers, 0, 0);
}

LindaSpace::LindaSpace(char* mem, sem_t* sem_is_resource_r, sem_t* sem_counting_r, pthread_cond_t* cond_waiting_for_c, pthread_mutex_t* mutex_waiting_for_c)
{

    space = mem;
    space_size = MAX_TUPLE_NUMBER * MAX_TUPLE_SIZE;
    sem_is_resource_reserved = sem_is_resource_r;
    sem_counting_readers = sem_counting_r;
    cond_waiting_for_changes = cond_waiting_for_c;
    mutex_waiting_for_changes = mutex_waiting_for_c;
}

LindaSpace::~LindaSpace()
{
}
