#include <semaphore.h>
#include <pthread.h>

class LindaSpace
{
private:
    char *space;
    size_t space_size;

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
