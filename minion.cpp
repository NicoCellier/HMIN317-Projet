#include "minion.h"

Minion::Minion()
{
    this->m_health_max = 100;
    this->m_health = this->m_health_max;
    this->m_position = QVector2D(0, 0);
}

//Minion::MinionAt(QVector2D position) {
//    this->m_position = position;
//}

void Minion::recieveDamage(int damage) {
    printf("ahlala je reçois des dégats...\n");

    int newHealth = this->m_health - damage;
    if(newHealth <= 0)
        printf("mince je suis mort\n");

    this->m_health = newHealth;
}

void Minion::heal(int heal) {
    int newHealth = this->m_health + heal;

    if(newHealth > this->m_health_max)
        this->m_health = this->m_health_max;
    else
        this->m_health = newHealth;
}

QVector2D Minion::getPosition() {
    return this->m_position;
}

void Minion::setPosition(QVector2D newPosition) {
    this->m_position = newPosition;
}

int Minion::getHealth() {
    return this->m_health;
}

void Minion::setHealth(int health) {
    this->m_health = health;
}
