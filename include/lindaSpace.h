#include <semaphore.h>
#include <pthread.h>

#include <iostream>
#include <unistd.h>

using namespace std;

class LindaSpace
{
private:
    //exchange space
    char *space;
    size_t space_size;

    //semaphores
    sem_t *sem_is_resource_reserved; // blocks entering of further readers
    sem_t *sem_counting_readers;     //used to check number of readers
    sem_t *sem_waiting_for_changes;  //when when needed tuple is not found we are waiting on this one

    void notify_waiting_for_changes()
    {
        int waiting;
        sem_getvalue(sem_waiting_for_changes, &waiting);
        while (waiting >= 1)
        {
            sem_post(sem_waiting_for_changes);
            sem_getvalue(sem_waiting_for_changes, &waiting);
        }
    }

public:
    LindaSpace(/* args */);
    ~LindaSpace();

    void write()
    {

        sem_wait(sem_is_resource_reserved);
        int readers_left;
        sem_getvalue(sem_counting_readers, &readers_left);
        while (readers_left != 0)
        {
            cout << "sleeping in write\n";
            sleep(1);
        }

        cout << "written\n";

        notify_waiting_for_changes();

        sem_post(sem_is_resource_reserved);
    }

    void read()
    {

        sem_wait(sem_is_resource_reserved);
        sem_post(sem_counting_readers);
        sem_post(sem_is_resource_reserved);

        bool found_searched_element = false;
        //wczytaj_i_przeszukaj_dane();

        while (!found_searched_element)
        {
            if (sem_trywait(sem_counting_readers))
                cout << "ERROR 1\n";

            sem_wait(sem_waiting_for_changes);
            sem_post(sem_counting_readers);
            found_searched_element = true;
        }
        cout << "reading\n";

        if (sem_trywait(sem_counting_readers))
            cout << "ERROR 2\n";
    }

    void remove()
    {
        sem_wait(sem_is_resource_reserved);
        int readers_left;
        sem_getvalue(sem_counting_readers, &readers_left);
        while (readers_left != 0)
        {
            cout << "sleeping in write\n";
            sleep(1);
        }

        bool found_searched_element = false;
        //wczytaj_i_przeszukaj_dane();

        while (!found_searched_element)
        {
            sem_post(sem_is_resource_reserved);

            sem_wait(sem_waiting_for_changes);
            sem_wait(sem_is_resource_reserved);

            found_searched_element = true;
            //wczytaj_i_przeszukaj_dane();
        }
        cout << "deleted\n";

        sem_post(sem_is_resource_reserved);
    }
};

LindaSpace::LindaSpace(/* args */)
{
    space_size = 100;
    space = new char[space_size];
    sem_is_resource_reserved = new sem_t;
    sem_counting_readers = new sem_t;
    sem_waiting_for_changes = new sem_t;

    sem_init(sem_is_resource_reserved, 0, 1);
    sem_init(sem_counting_readers, 0, 0);
    sem_init(sem_waiting_for_changes, 0, 0);
}

LindaSpace::~LindaSpace()
{
}
