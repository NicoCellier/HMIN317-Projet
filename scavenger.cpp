#include "scavenger.h"

void Scavenger::collect(int collectable) {
    int value = this->m_backpack + collectable;
    if(value > this->m_backpack_max)
        value = this->m_backpack_max;

    this->m_backpack = value;
}

void Scavenger::yeildTo() {
    // TODO
}

int Scavenger::getBackPack() {
    return this->m_backpack;
}

void Scavenger::setBackPack(int value) {
    if(value > this->m_backpack_max)
        value = this->m_backpack_max;

    this->m_backpack = value;
}
