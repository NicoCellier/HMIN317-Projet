#ifndef MEGAMAN_H
#define MEGAMAN_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Megaman : protected QOpenGLFunctions
{
public:
    Megaman(QString path, QVector2D spawnPosition);
    virtual ~Megaman();

    void draw(QOpenGLShaderProgram *program);

private:
    void init();

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;

    QVector2D position;
};

#endif // MEGAMAN_H
