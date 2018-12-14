#ifndef MEGAMAN_H
#define MEGAMAN_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include <QObject>
#include <QBasicTimer>
#include <QTimerEvent>

class Megaman : public QOpenGLWidget, protected QOpenGLFunctions
{
    //Q_OBJECT
public:
    Megaman(QString path, QVector2D spawnPosition);
    ~Megaman();

    void draw(QOpenGLShaderProgram *program);
    void moveTo(QVector2D destination);

protected:
    void timerEvent(QTimerEvent *e) override;

private:
    void updatePosition();

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

    QVector2D position;
    QVector2D destination;

    QBasicTimer timer;
};

#endif // MEGAMAN_H
