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

#include "mainwidget.h"

#include <QMouseEvent>

#include <math.h>

#include <iostream>

MainWidget::MainWidget(QWidget *parent) :
    QOpenGLWidget(parent),
    geometries(0),
    megaman(0),
    grassTexture(0),
    megamanTexture(0),
    cam(0.0,0.0,-1.5),
    camMove(0.0,0.0),
    xMargin(50),
    yMargin(40),
    maxSpeed(0.025)
{
    setMouseTracking(true);
}

MainWidget::~MainWidget()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete grassTexture;
    delete megamanTexture;
    delete geometries;
    delete megaman;
    doneCurrent();
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());
    std::cout << "MousePosition " << mousePressPosition[0] << " " << mousePressPosition[1] << std::endl;
}

void MainWidget::mouseReleaseEvent(QMouseEvent *e)
{
    // Mouse release position - mouse press position
    update();

}
//! [0]

void MainWidget::mouseMoveEvent(QMouseEvent *e){
    // Save mouse press position
    mousePressPosition = QVector2D(e->localPos());

    camMove = QVector2D(0, 0);
    if(mousePressPosition[0] > this->width()-50){
        camMove[0] = -0.1*1/(this->width()-mousePressPosition[0]);
    }
    else if(mousePressPosition[0] < 50){
        camMove[0] = 0.1*1/(mousePressPosition[0]);
    }

    if(mousePressPosition[1] > this->height()-40){
        camMove[1] = 0.1*1/(this->height()-mousePressPosition[1]);
    }
    else if(mousePressPosition[1] < 40){
        camMove[1] = -0.1*1/(mousePressPosition[1]);
    }

    if(camMove[0] > maxSpeed)
        camMove[0] = maxSpeed;
    else if(camMove[0] < -maxSpeed)
        camMove[0] = -maxSpeed;

    if(camMove[1] > maxSpeed)
        camMove[1] = maxSpeed;
    else if(camMove[1] < -maxSpeed)
        camMove[1] = -maxSpeed;

    update();
}

//! [1]
void MainWidget::timerEvent(QTimerEvent *)
{
    cam[0] += camMove[0];
    cam[1] += camMove[1];
    // Request an update
    update();
}
//! [1]

void MainWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0, 0, 0, 1);

    initShaders();
    initTextures();

//! [2]
    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);
//! [2]

    geometries = new Terrain(20, 20);
    megaman = new Megaman("../HMIN317-Projet/megaman.png", QVector2D(-0.5, -0.5));

    // Use QBasicTimer because its faster than QTimer
    timer.start(10, this);

}

//! [3]
void MainWidget::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}
//! [3]

//! [4]
void MainWidget::initTextures()
{
    // Load grass.png image
    grassTexture = new QOpenGLTexture(QImage("../HMIN317-Projet/grass.png"));
    // Set nearest filtering mode for texture minification
    grassTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    // Set bilinear filtering mode for texture magnification
    grassTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    grassTexture->setWrapMode(QOpenGLTexture::Repeat);


    // Load megaman.png image
    megamanTexture = new QOpenGLTexture(QImage("../HMIN317-Projet/megaman.png"));
    // Set nearest filtering mode for texture minification
    megamanTexture->setMinificationFilter(QOpenGLTexture::Nearest);
    // Set bilinear filtering mode for texture magnification
    megamanTexture->setMagnificationFilter(QOpenGLTexture::Linear);
    // Wrap texture coordinates by repeating
    megamanTexture->setWrapMode(QOpenGLTexture::Repeat);
}
//! [4]

//! [5]
void MainWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 50.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}
//! [5]

void MainWidget::paintGL()
{
    // Clear color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    grassTexture->bind(0);
    // Use texture unit 0 which contains grass.png
    program.setUniformValue("texture", 0);
    // Draw the terrain
    geometries->draw(&program);

    megamanTexture->bind(1);
    program.setUniformValue("texture", 1);
    megaman->draw(&program);


//! [6]
    // Calculate model view transformation
    QMatrix4x4 matrix;
    // Uncomment this to be able to move with the mouse
    //matrix.translate(cam[0], cam[1], cam[2]);
    // So that Y+ is upwards and the images are on their head...
    matrix.rotate(180, 0, 0, 1);
    matrix.translate(0, 0, -20.0f);

    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projection * matrix);
}
