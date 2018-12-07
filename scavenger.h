#ifndef SCAVENGER_H
#define SCAVENGER_H

#include "minion.h"

class Scavenger : public Minion
{
public:
    Scavenger() : Minion() {}

    void collect(int collectable);
    void yeildTo();

    int getBackPack();
    void setBackPack(int value);

private:
    int m_backpack = 0;
    int m_backpack_max = 50;
};

#endif // SCAVENGER_H
