#include <semaphore.h>
#include <pthread.h>

class LindaSpace
{
private:
//exchange space
    char *space;
    size_t space_size;

//semaphores
    sem_t *sem_editor; //allows only one writer to work
    sem_t *sem_blocking_readers; // blocks entering of further readers
    sem_t *sem_counting_readers; //used to check number of readers
    sem_t *sem_waiting_for_changes; //when when needed tuple is not found we are waiting on this one
    void write()
    {
        /*
        sem_wait(sem_editor);

        sem_wait(sem_blocking_readers);
        int readers_left;
        sem_getvalue(sem_counting_readers, &readers_left);
        while(readers_left != 0)
        {
            //jakoś śpij i sprawdź ponownie
        }

        Wczytywaniedanych();
        dodanie_krotki();
        zapisanie_danych();

        //odblokuj wszystkich czekających na zmiany
        sem_post(sem_waiting_for_changes); //Kilka razy

        sem_post(sem_blocking_readers)
        sem_post(sem_editor)

        */
    }

    void read()
    {
        /*
        sem_wait(sem_blocking_readers);
        sem_post(sem_counting_readers);
        sem_post(sem_blocking_readers);

        bool znaleziono_szukany_element = wczytaj_i_przeszukaj_dane();
        while(!znaleziono_szukany_element)
        {
            if(sem_trywait(sem_counting_readers))
                printuj_errora, który nie powinien mieć miejsca

            sem_wait(sem_waiting_for_changes);
            sem_post(sem_counting_readers);
            znaleziono_szukany_element = wczytaj_i_przeszukaj_dane();
    
        }

        if(sem_trywait(sem_counting_readers))
            printuj_errora, który nie powinien mieć miejsca

        
        */
    }

    void remove()
    {
        /*
        sem_wait(sem_editor);

        sem_wait(sem_blocking_readers);
        int readers_left;
        sem_getvalue(sem_counting_readers, &readers_left);
        while(readers_left != 0)
        {
            //jakoś śpij i sprawdź ponownie
        }

        bool znaleziono_szukany_element = wczytaj_i_przeszukaj_dane();
        while(!znaleziono_szukany_element)
        {
            sem_post(sem_editor);
            sem_post(sem_blocking_readers);

            if(sem_trywait(sem_counting_readers))
                printuj_errora, który nie powinien mieć miejsca

            sem_wait(sem_waiting_for_changes);
            sem_wait(sem_blocking_readers);
            sem_wait(sem_editor);
            sem_post(sem_counting_readers);

            znaleziono_szukany_element = wczytaj_i_przeszukaj_dane();
        }
        usun_element();
        zapisz_nowe_dane();

        //odblokuj wszystkich czekających na zmiany (funkcja for, bo z whilem może być niebezpiecznie)
        sem_post(sem_waiting_for_changes); //Kilka razy

        sem_post(sem_blocking_readers)
        sem_post(sem_editor)
        */
    }

public:
    LindaSpace(/* args */);
    ~LindaSpace();

};

LindaSpace::LindaSpace(/* args */)
{
    space_size = 100;
    space = new char[space_size];

}

LindaSpace::~LindaSpace()
{
}
