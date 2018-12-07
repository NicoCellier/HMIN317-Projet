#include "soldier.h"

void Soldier::attack(Minion &target) {
    printf("tiens bouffe !\n");
    target.recieveDamage(this->m_strength);
}

int Soldier::getArmor() {
    return this->m_armor;
}

int Soldier::getStrength() {
    return this->m_strength;
}
