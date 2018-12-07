/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the QtCore module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "terrain.h"

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
Terrain::Terrain() :
    indexBuf(QOpenGLBuffer::IndexBuffer),
    nbOfVertices(0)
{
    initializeOpenGLFunctions();

    // Generate 2 VBOs
    arrayBuf.create();
    indexBuf.create();

    // Number of vertices in the rendering
    nbOfVertices = 24;

    // Initializes cube geometry and transfers it to VBOs
    init();
}

Terrain::~Terrain()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}
//! [0]

void Terrain::init() {
    float startingPoint = 10.f;

    // Init vertices
    VertexData *vertices = new VertexData[nbOfVertices*nbOfVertices];
    float x, y, step;
    step = startingPoint*2/nbOfVertices;
    for (int i = 0; i < nbOfVertices; ++i) {
        x = -startingPoint + i * step ;
        for (int j = 0; j < nbOfVertices; ++j) {
            y = -startingPoint + j * step;
            vertices[i*nbOfVertices+j] = {QVector3D(x, y, 0), QVector2D((1.0f/(nbOfVertices-1))*i, (1.0f/(nbOfVertices-1))*j)};
            std::cout << "Vertex position: (" << x << ", " << y << ")" << std::endl;
        }
    }

    GLushort indices[(nbOfVertices-1)*(nbOfVertices*2+4)];
    for (int i=0;i<nbOfVertices-1;i++)
        {
            indices[(nbOfVertices*2+4)*i] = nbOfVertices*i;
            indices[(nbOfVertices*2+4)*i+1] = nbOfVertices*i;

            for (int j=2;j<(nbOfVertices*2+2);j+=2)
                {
                    indices[(nbOfVertices*2+4)*i+j] = nbOfVertices*i +(j-2)/2;
                    indices[(nbOfVertices*2+4)*i+j+1] = nbOfVertices*(i+1) + (j-2)/2;
                }

            indices[(nbOfVertices*2+4)*i+(nbOfVertices*2+2)] = nbOfVertices*(i+1) + nbOfVertices-1;
            indices[(nbOfVertices*2+4)*i+(nbOfVertices*2+3)] = nbOfVertices*(i+1) + nbOfVertices-1;
    }

    // Transfer vertex data to VBO 0
    arrayBuf.bind();
    arrayBuf.allocate(vertices, nbOfVertices*nbOfVertices * sizeof(VertexData));

    // Transfer index data to VBO 1
    indexBuf.bind();
    indexBuf.allocate(indices, (nbOfVertices-1)*(nbOfVertices*2+4) * sizeof(GLushort));

}

void Terrain::draw(QOpenGLShaderProgram *program)
{
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

    glDrawElements(GL_TRIANGLE_STRIP,(nbOfVertices-1)*(nbOfVertices*2+4), GL_UNSIGNED_SHORT, 0);
}
