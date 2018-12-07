#ifndef SOLDIER_H
#define SOLDIER_H

#include "minion.h"

class Soldier : public Minion
{
public:
    Soldier() : Minion() {this->setHealth(120);}
    //SoldierAt(QVector2D position);

    void attack(Minion &target);

    int getArmor();
    int getStrength();

private:
    int m_armor;
    int m_strength = 10;
};

#endif // SOLDIER_H
