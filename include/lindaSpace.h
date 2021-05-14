#include <semaphore.h>
#include <pthread.h>

#include <iostream>
#include <unistd.h>

using namespace std;

class LindaSpace
{
private:
    bool debug;
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
        pthread_cond_broadcast(cond_waiting_for_changes);
    }

    bool search_for_data(char num) //TODO search for tuple and instead of dealing with chars edit tuples
    {
        if (space[0] == num)
        {
            if (debug)
                cout << "data" << (int)num << " found\n";
            return true;
        }
        if (debug)
            cout << "data " << (int)num << " not found\n";
        return false;
    }

    void save_data(char num)
    {
        if (debug)
            cout << "writing " << (int)num << "\n";
        space[0] = num;
    }

public:
    LindaSpace(bool debug);
    ~LindaSpace();

    void write(char num)
    {

        sem_wait(sem_is_resource_reserved);
        int readers_left;
        sem_getvalue(sem_counting_readers, &readers_left);
        while (readers_left != 0)
        {
            if (debug)
                cout << "sleeping in write\n";
            sleep(1);
            sem_getvalue(sem_counting_readers, &readers_left);
        }
        save_data(num);

        notify_waiting_for_changes();

        sem_post(sem_is_resource_reserved);
    }

    void read(char num)
    {

        sem_wait(sem_is_resource_reserved);
        sem_post(sem_counting_readers);
        sem_post(sem_is_resource_reserved);

        bool found_searched_element = search_for_data(num);
        while (!found_searched_element)
        {
            if (sem_trywait(sem_counting_readers))
                cout << "ERROR 1\n";

            pthread_cond_wait(cond_waiting_for_changes,
                              mutex_waiting_for_changes);
            sem_wait(sem_is_resource_reserved);
            sem_post(sem_counting_readers);
            sem_post(sem_is_resource_reserved);
            found_searched_element = search_for_data(num);
        }
        if (debug)
            cout << "reading: " << int(num) << "\n";

        if (sem_trywait(sem_counting_readers))
            cout << "ERROR 2\n";
    }

    void remove(char num)
    {
        sem_wait(sem_is_resource_reserved);
        int readers_left;
        sem_getvalue(sem_counting_readers, &readers_left);
        while (readers_left != 0)
        {
            if (debug)
                cout << "sleeping in remove\n";
            sleep(1); //TODO remove sleeps
            sem_getvalue(sem_counting_readers, &readers_left);
        }

        bool found_searched_element = search_for_data(num);

        while (!found_searched_element)
        {
            sem_post(sem_is_resource_reserved);

            pthread_cond_wait(cond_waiting_for_changes,
                              mutex_waiting_for_changes);
            sem_wait(sem_is_resource_reserved);

            found_searched_element = search_for_data(num);
        }
        if (debug)
            cout << "deleted" << int(num) << "\n";
        save_data(0);

        sem_post(sem_is_resource_reserved);
    }
};

LindaSpace::LindaSpace(bool debug = false)
{
    space_size = 100;
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

LindaSpace::~LindaSpace()
{
}
