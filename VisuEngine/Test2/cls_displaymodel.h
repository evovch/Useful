/**

    @class cls_DisplayModel

    An object of this class is something which is sent to the GPU in a straightforward manner.
    This model is ready for visualization.
    It consists of a number of vertices with their colors,
    indices of the triangles,
    indices of the wires, and
    indices of the points.
    It is supposed that each set of indices (triangles, wores, points)
    is processed by the corresponding shader pipeline (aka program).

*/

#pragma once

// Project
#include "support.h"

class cls_myGLwidget;

class cls_DisplayModel
{
public: // methods
    cls_DisplayModel(cls_myGLwidget* p_widget);
    ~cls_DisplayModel();

    void Dump(void) const;

    void SendToGPU(void) const;
    void Draw(void) const;

    //stc_VandC* GetVandC(void) const { return mVertexAndColorData; }
    //unsigned int* GetTriangles(void) const { return mTriangleIndices; }
    //unsigned int* GetWires(void) const { return mWireIndices; }
    //unsigned int* GetPoints(void) const { return mPointIndices; }

    void GenerateBox(void);

private: // data members

    //// Pointer to the corresponding widget
    cls_myGLwidget* mWidget;

    unsigned int mNumOfVertices;
    unsigned int mNumOfTriangles;
    unsigned int mNumOfWires;
    unsigned int mNumOfPoints;

    stc_VandC* mVertexAndColorData; //// [mNumOfVertices]
    unsigned int* mTriangleIndices; //// [mNumOfTriangles*3]
    unsigned int* mWireIndices;     //// [mNumOfWires*2]
    unsigned int* mPointIndices;    //// [mNumofPoints]

    bool mConstructed;

};
