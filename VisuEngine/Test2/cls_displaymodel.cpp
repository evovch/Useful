#include "cls_displaymodel.h"

#include <QDebug>

cls_DisplayModel::cls_DisplayModel()
{
    // Allocate memory inside this GenerateBox() method; free in the destructor
    this->GenerateBox();
}

cls_DisplayModel::~cls_DisplayModel()
{
    if (mVertexAndColorData) delete [] mVertexAndColorData; mVertexAndColorData = nullptr;
    if (mTriangleIndices) delete [] mTriangleIndices; mTriangleIndices = nullptr;
    if (mWireIndices) delete [] mWireIndices; mWireIndices = nullptr;
}

void cls_DisplayModel::GenerateBox(void)
{
    mNumOfVertices = 8;
    mNumOfTriangles = 12;
    mNumOfWires = 12;

    // Vertices and colors
    mVertexAndColorData = new stc_VandC[8]; // mNumOfVertices

    // Only colors
    for (unsigned int i=0; i<8; i++) { // mNumOfVertices
        mVertexAndColorData[i].c[0] = 0.5;
        mVertexAndColorData[i].c[1] = 0.5;
        mVertexAndColorData[i].c[2] = 0.5;
    }

    // mNumOfVertices*3
    float v_coords[8*3] = {-0.5,-0.5,-0.5, -0.5,-0.5,0.5, -0.5,0.5,-0.5, -0.5,0.5,0.5, 0.5,-0.5,-0.5, 0.5,-0.5,0.5, 0.5,0.5,-0.5, 0.5,0.5,0.5};

    // Vertices
    for (unsigned int i=0; i<8; i++) { // mNumOfVertices
        mVertexAndColorData[i].v[0] = v_coords[i*3+0];
        mVertexAndColorData[i].v[1] = v_coords[i*3+1];
        mVertexAndColorData[i].v[2] = v_coords[i*3+2];
    }

    // Triangles
    mTriangleIndices = new unsigned int[12*3]; // mNumOfTriangles*3

    // mNumOfTriangles*3
    unsigned int v_triangIndices[12*3] = {0,1,2, 1,3,2, 6,5,4, 7,5,6, 7,1,5, 7,3,1, 0,6,4, 0,2,6, 2,7,6, 2,3,7, 5,0,4, 5,1,0};

    for (unsigned int i=0; i<12*3; i++) { // mNumOfTriangles*3
        mTriangleIndices[i] = v_triangIndices[i];
    }

    // Wires
    mWireIndices = new unsigned int[12*2]; // mNumOfWires*2

    // mNumOfWires*2
    unsigned int v_wiresIndices[12*2] = {0,1, 0,2, 1,3, 2,3, 4,5, 4,6, 5,7, 6,7, 0,4, 1,5, 3,7, 2,6};

    for (unsigned int i=0; i<12*2; i++) { // mNumOfWires*2
        mWireIndices[i] = v_wiresIndices[i];
    }
}

void cls_DisplayModel::Dump(void)
{
    qDebug().nospace() << "-----------------------------------------------------------------------";

    qDebug().nospace() << "mNumOfVertices=" << mNumOfVertices;
    qDebug().nospace() << "mNumOfTriangles=" << mNumOfTriangles;
    qDebug().nospace() << "mNumOfWires=" << mNumOfWires;

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

    qDebug().nospace() << "-----------------------------------------------------------------------";
}

void cls_DisplayModel::SendToGPU()
{

}

void cls_DisplayModel::Draw()
{

}
