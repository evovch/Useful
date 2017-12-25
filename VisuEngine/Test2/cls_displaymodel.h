#pragma once

#include "support.h"

class cls_myGLwidget;

class cls_DisplayModel
{
public: // methods
    cls_DisplayModel(cls_myGLwidget* p_widget);
    ~cls_DisplayModel();

    void Dump(void);

    void SendToGPU();
    void Draw();

    stc_VandC* GetVandC(void) const { return mVertexAndColorData; }
    unsigned int* GetTriangles(void) const { return mTriangleIndices; }
    unsigned int* GetWires(void) const { return mWireIndices; }

protected: // methods

private: // methods

    void GenerateBox(void);

private: // data members

    unsigned int mNumOfVertices;
    unsigned int mNumOfTriangles;
    unsigned int mNumOfWires;

    stc_VandC* mVertexAndColorData;		// [mNumOfVertices]
    unsigned int* mTriangleIndices;		// [mNumOfTriangles*3]
    unsigned int* mWireIndices;			// [mNumOfWires*2]

    // Pointer to the corresponding widget
    cls_myGLwidget* mWidget;

};
