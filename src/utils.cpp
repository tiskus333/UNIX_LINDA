#include "utils.h"



void printTuple(const Tuple &tuple)
{
    std::cout << "Tuple = {";
    for (auto &i : tuple)
    {
        std::cout << i << ',';
    }
    std::cout << "\b}\n";
}