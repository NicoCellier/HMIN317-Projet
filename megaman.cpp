#include "megaman.h"


#include <QVector2D>
#include <QVector3D>
#include <QImage>

#include <iostream>

struct VertexData
{
    QVector3D position;
    QVector2D texCoord;
};

//! [0]
Megaman::Megaman(QString path, QVector2D spawnPosition) :
    indexBuf(QOpenGLBuffer::IndexBuffer),
    position(0, 0)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    position = spawnPosition;

    //std::cout << "Spawning at " << position[0] << ", " << position[1] << std::endl;

    // Initializes cube geometry and transfers it to VBOs

    init();
}

Megaman::~Megaman()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void Megaman::init() {
    int nbV = 2; // nb VertexByRow or Column

    // Init vertices
    VertexData *vertices = new VertexData[nbV*nbV];
    float baseX, baseY, spriteDim, x, y;
    spriteDim = 2.0f/nbV;
    baseX = position[0] - spriteDim/2;
    baseY = position[1] - spriteDim/2;
    for (int i = 0; i < nbV; ++i) {
        x = baseX + i * spriteDim;
        for (int j = 0; j < nbV; ++j) {
            y = baseY + j * spriteDim;
            vertices[i*nbV+j] = {QVector3D(x, y, 0.1f), QVector2D((1.0f/(nbV-1))*i, (1.0f/(nbV-1))*j)};
        }
    }

    GLushort indices[(nbV-1)*(nbV*2+4)];
    for (int i=0;i<nbV-1;i++)
        {
            indices[(nbV*2+4)*i] = nbV*i;
            indices[(nbV*2+4)*i+1] = nbV*i;

            for (int j=2;j<(nbV*2+2);j+=2)
                {
                    indices[(nbV*2+4)*i+j] = nbV*i +(j-2)/2;
                    indices[(nbV*2+4)*i+j+1] = nbV*(i+1) + (j-2)/2;
                }

            indices[(nbV*2+4)*i+(nbV*2+2)] = nbV*(i+1) + nbV-1;
            indices[(nbV*2+4)*i+(nbV*2+3)] = nbV*(i+1) + nbV-1;
    }

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, nbV*nbV * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, (nbV-1)*(nbV*2+4) * sizeof(GLushort));

}

void Megaman::draw(QOpenGLShaderProgram *program)
{

    int nbV = 2;
    // Tell OpenGL which VBOs to use
    arrayBuf.bind();
    indexBuf.bind();

    // Offset for position
    quintptr offset = 0;

    // Tell OpenGL programmable pipeline how to locate vertex position data
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, offset, 3, sizeof(VertexData));

    // Offset for texture coordinate
    offset += sizeof(QVector3D);

    // Tell OpenGL programmable pipeline how to locate vertex texture coordinate data
    int texcoordLocation = program->attributeLocation("a_texcoord");
    program->enableAttributeArray(texcoordLocation);
    program->setAttributeBuffer(texcoordLocation, GL_FLOAT, offset, 2, sizeof(VertexData));

    // Draw cube geometry using indices from VBO 1

    glDrawElements(GL_TRIANGLE_STRIP,(nbV-1)*(nbV*2+4), GL_UNSIGNED_SHORT, 0);
}

