#include "cls_displaymodel.h"

// Qt
#include <QDebug>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>

// Project
#include "cls_myglwidget.h"

cls_DisplayModel::cls_DisplayModel(cls_myGLwidget* p_widget) :
    mWidget(p_widget),
    mNumOfVertices(0),
    mNumOfTriangles(0),
    mNumOfWires(0),
    mNumOfPoints(0),
    mVertexAndColorData(nullptr),
    mTriangleIndices(nullptr),
    mWireIndices(nullptr),
    mPointIndices(nullptr),
    mConstructed(false)
{
}

cls_DisplayModel::~cls_DisplayModel()
{
    mNumOfVertices = 0;
    mNumOfTriangles = 0;
    mNumOfWires = 0;
    mNumOfPoints = 0;
    if (mVertexAndColorData) delete [] mVertexAndColorData; mVertexAndColorData = nullptr;
    if (mTriangleIndices) delete [] mTriangleIndices; mTriangleIndices = nullptr;
    if (mWireIndices) delete [] mWireIndices; mWireIndices = nullptr;
    if (mPointIndices) delete [] mPointIndices; mPointIndices = nullptr;
    mConstructed = false;
}

void cls_DisplayModel::GenerateBox(void)
{
    mNumOfVertices = 8;
    mNumOfTriangles = 12;
    mNumOfWires = 12;
    mNumOfPoints = 8;

    //// --------------------------------------------------------------------------------------------

    //// Vertices and colors
    mVertexAndColorData = new stc_VandC[8]; //// mNumOfVertices

    //// Only colors
    for (unsigned int i=0; i<8; i++) { //// mNumOfVertices
        mVertexAndColorData[i].c[0] = (float)qrand()/(float)RAND_MAX;
        mVertexAndColorData[i].c[1] = (float)qrand()/(float)RAND_MAX;
        mVertexAndColorData[i].c[2] = (float)qrand()/(float)RAND_MAX;
    }

    //// mNumOfVertices*3
    float v_coords[8*3] = {-0.5,-0.5,-0.5, -0.5,-0.5,0.5, -0.5,0.5,-0.5, -0.5,0.5,0.5, 0.5,-0.5,-0.5, 0.5,-0.5,0.5, 0.5,0.5,-0.5, 0.5,0.5,0.5};

    //// Vertices
    for (unsigned int i=0; i<8; i++) { //// mNumOfVertices
        mVertexAndColorData[i].v[0] = v_coords[i*3+0];
        mVertexAndColorData[i].v[1] = v_coords[i*3+1];
        mVertexAndColorData[i].v[2] = v_coords[i*3+2];
    }

    //// --------------------------------------------------------------------------------------------

    //// Triangles
    mTriangleIndices = new unsigned int[12*3]; //// mNumOfTriangles*3

    //// mNumOfTriangles*3
    unsigned int v_triangIndices[12*3] = {0,1,2, 1,3,2, 6,5,4, 7,5,6, 7,1,5, 7,3,1, 0,6,4, 0,2,6, 2,7,6, 2,3,7, 5,0,4, 5,1,0};

    for (unsigned int i=0; i<12*3; i++) { //// mNumOfTriangles*3
        mTriangleIndices[i] = v_triangIndices[i];
    }

    //// --------------------------------------------------------------------------------------------

    //// Wires
    mWireIndices = new unsigned int[12*2]; //// mNumOfWires*2

    //// mNumOfWires*2
    unsigned int v_wiresIndices[12*2] = {0,1, 0,2, 1,3, 2,3, 4,5, 4,6, 5,7, 6,7, 0,4, 1,5, 3,7, 2,6};

    for (unsigned int i=0; i<12*2; i++) { //// mNumOfWires*2
        mWireIndices[i] = v_wiresIndices[i];
    }

    //// --------------------------------------------------------------------------------------------

    //// Points
    mPointIndices = new unsigned int[8]; //// mNumOfPoints

    for (unsigned int i=0; i<8; i++) { //// mNumOfPoints
        mPointIndices[i] = i;
    }

    //// --------------------------------------------------------------------------------------------

    mConstructed = true;
}

void cls_DisplayModel::Dump(void) const
{
    if (!mConstructed) {
        qDebug().nospace() << "cls_DisplayModel::Draw(): Display model is not yet constructed.";
        return;
    }

    qDebug().nospace() << "-----------------------------------------------------------------------";

    qDebug().nospace() << "mNumOfVertices=" << mNumOfVertices;
    qDebug().nospace() << "mNumOfTriangles=" << mNumOfTriangles;
    qDebug().nospace() << "mNumOfWires=" << mNumOfWires;
    qDebug().nospace() << "mNumOfPoints=" << mNumOfPoints;

    qDebug().nospace() << "Vertices and colors ===================================================";

    for (unsigned int i=0; i<mNumOfVertices; i++) {
        qDebug().nospace() << "x=" << mVertexAndColorData[i].v[0] << "\t"
                           << "y=" << mVertexAndColorData[i].v[1] << "\t"
                           << "z=" << mVertexAndColorData[i].v[2] << "\t"
                           << "r=" << mVertexAndColorData[i].c[0] << "\t"
                           << "g=" << mVertexAndColorData[i].c[1] << "\t"
                           << "b=" << mVertexAndColorData[i].c[2];
    }

    qDebug().nospace() << "Triangles =============================================================";

    for (unsigned int i=0; i<mNumOfTriangles; i++) {
        qDebug().nospace() << mTriangleIndices[i*3+0] << ", " << mTriangleIndices[i*3+1] << ", " << mTriangleIndices[i*3+2];
    }

    qDebug().nospace() << "Wires =================================================================";

    for (unsigned int i=0; i<mNumOfWires; i++) {
        qDebug().nospace() << mWireIndices[i*2+0] << ", " << mWireIndices[i*2+1];
    }

    qDebug().nospace() << "Points ================================================================";

    for (unsigned int i=0; i<mNumOfPoints; i++) {
        qDebug().nospace() << mPointIndices[i];
    }

    qDebug().nospace() << "-----------------------------------------------------------------------";
}

void cls_DisplayModel::SendToGPU(void) const
{
    //qDebug().nospace() << "cls_DisplayModel::SendToGPU";

    if (!mConstructed) {
        qDebug().nospace() << "cls_DisplayModel::Draw(): Display model is not yet constructed.";
        return;
    }

    mWidget->mVAO->bind();
    mWidget->mVBO->bind();

    mWidget->glBufferData(GL_ARRAY_BUFFER, mNumOfVertices*sizeof(stc_VandC), mVertexAndColorData, GL_STATIC_DRAW);
    mWidget->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, v));
    mWidget->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(stc_VandC), (void*)offsetof(stc_VandC, c));
    mWidget->glEnableVertexAttribArray(0);
    mWidget->glEnableVertexAttribArray(1);

    mWidget->mIBOshading->bind();
    mWidget->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfTriangles*3*sizeof(unsigned int), mTriangleIndices, GL_STATIC_DRAW);

    mWidget->mIBOwire->bind();
    mWidget->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfWires*2*sizeof(unsigned int), mWireIndices, GL_STATIC_DRAW);

    mWidget->mIBOpoints->bind();
    mWidget->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mNumOfPoints*sizeof(unsigned int), mPointIndices, GL_STATIC_DRAW);
}

void cls_DisplayModel::Draw(void) const
{
    //qDebug().nospace() << "cls_DisplayModel::Draw";

    if (!mConstructed) {
        qDebug().nospace() << "cls_DisplayModel::Draw(): Display model is not yet constructed.";
        return;
    }

    //// Shading
    mWidget->mProgShading->bind();
    mWidget->mIBOshading->bind();
    mWidget->glDrawElements(GL_TRIANGLES, mNumOfTriangles*3, GL_UNSIGNED_INT, NULL);

    //// Wire
    mWidget->mProgWire->bind();
    mWidget->mIBOwire->bind();
    mWidget->glDrawElements(GL_LINES, mNumOfWires*2, GL_UNSIGNED_INT, NULL);

    //// Points
    mWidget->mProgPoints->bind();
    mWidget->mIBOpoints->bind();
    mWidget->glDrawElements(GL_POINTS, mNumOfPoints, GL_UNSIGNED_INT, NULL);
}
