#pragma once

#include "support.h"

class cls_DisplayModel
{
public: // methods
    cls_DisplayModel();
    ~cls_DisplayModel();

    void Dump(void);
    void SendToGPU();
    void Draw();

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

};
