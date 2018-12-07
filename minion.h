#ifndef MINION_H
#define MINION_H

#include <QVector2D>

class Minion
{
public:
    Minion();
    //MinionAt(QVector2D position);

    void recieveDamage(int damage);
    void heal(int heal);

    QVector2D getPosition();
    void setPosition(QVector2D newPosition);

    int getHealth();
    void setHealth(int health);

private:
    QVector2D m_position;

    int m_health_max;
    int m_health;
};

#endif // MINION_H
