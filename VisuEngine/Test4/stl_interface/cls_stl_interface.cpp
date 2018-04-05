#include "cls_stl_interface.h"

// STD
#include <cstdio>

// Project
#include "cls_stl_file.h"
#include "support.h"

extern FILE* yyin;
extern int yyparse(cls_stl_file*& myStlFile);

cls_stl_interface::cls_stl_interface()
{
}

cls_stl_interface::~cls_stl_interface()
{
}

cls_stl_file* cls_stl_interface::Import(const char* p_filename)
{
    FILE* v_inFile = fopen(p_filename, "r");

    if (!v_inFile) {
        fprintf(stderr, "[ERROR] Failed to open file '%s'\n", p_filename);
        return nullptr;
    } else {
        yyin = v_inFile;
        cls_stl_file* v_stl_file;
        /*int res = */yyparse(v_stl_file);
        fclose(v_inFile);
        return v_stl_file;
    }
}

cls_stl_file* cls_stl_interface::ImportText(const char* /*p_filename*/)
{

}

/**

UINT8[80] – Header
UINT32 – Number of triangles

foreach triangle
REAL32[3] – Normal vector
REAL32[3] – Vertex 1
REAL32[3] – Vertex 2
REAL32[3] – Vertex 3
UINT16 – Attribute byte count
end

**/
cls_stl_file* cls_stl_interface::ImportBinary(const char* p_filename)
{
    FILE* v_inFile = fopen(p_filename, "rb");

    if (!v_inFile) {
        fprintf(stderr, "[ERROR] Failed to open file '%s'\n", p_filename);
        return nullptr;
    } else {

        cls_stl_file* v_resStlFile = new cls_stl_file();

        // Obtain file size
        fseek(v_inFile , 0 , SEEK_END);
        long int v_fileSize = ftell(v_inFile);
        rewind(v_inFile);

        fprintf(stderr, "File size: %ld\n", v_fileSize);

        // Header
        unsigned char v_bufferHeader[81];
        v_bufferHeader[80] = '\0';
        fread(v_bufferHeader, 80, 1, v_inFile);
        fprintf(stderr, "HEADER =========================================================================\n");
        fprintf(stderr, "%s\n", v_bufferHeader);
        fprintf(stderr, "================================================================================\n");

        // Number of triangles
        unsigned char v_bufferN[4];
        fread(v_bufferN, 4, 1, v_inFile);
        unsigned int v_N = FourCharsToUintLE(v_bufferN);
        fprintf(stderr, "%d triangles declared in the header.\n", v_N);

        // Facets
        // 4*3*4 - (normal, vertex, vertex, vertex) * (x, y, z) * (4 bytes each)
        // 2 - Attribute byte count (whatever it is)
        unsigned int v_sizeOfFacet = 4*3*4 + 2;
        unsigned char* v_dataBuffer = new unsigned char[v_N*v_sizeOfFacet];
        fprintf(stderr, "Awaited data size: %d bytes.\n", v_N*v_sizeOfFacet);

        fread(v_dataBuffer, v_N*v_sizeOfFacet, 1, v_inFile);

        struct facet_t* v_lastFacet = nullptr;

        for (unsigned int iFacet=0; iFacet<v_N; iFacet++) {
            unsigned int v_offset = iFacet*v_sizeOfFacet;

            float v_normal[3];
            float v_vertex[3*3];

            v_normal[0] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*0]);
            v_normal[1] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*1]);
            v_normal[2] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*2]);
            v_vertex[0] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*3]);
            v_vertex[1] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*4]);
            v_vertex[2] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*5]);
            v_vertex[3] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*6]);
            v_vertex[4] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*7]);
            v_vertex[5] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*8]);
            v_vertex[6] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*9]);
            v_vertex[7] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*10]);
            v_vertex[8] = FourCharsToFloat(&v_dataBuffer[v_offset + 4*11]);

            /*fprintf(stderr, "facet normal %f %f %f\n", v_normal[0], v_normal[1], v_normal[2]);
            fprintf(stderr, " vertex %f %f %f\n", v_vertex[0], v_vertex[1], v_vertex[2]);
            fprintf(stderr, " vertex %f %f %f\n", v_vertex[3], v_vertex[4], v_vertex[5]);
            fprintf(stderr, " vertex %f %f %f\n", v_vertex[6], v_vertex[7], v_vertex[8]);
            fprintf(stderr, "endfacet\n");*/

            struct normal_t* n = new struct normal_t(v_normal[0], v_normal[1], v_normal[2]);
            struct vertex_t* v1 = new struct vertex_t(v_vertex[0], v_vertex[1], v_vertex[2]);
            struct vertex_t* v2 = new struct vertex_t(v_vertex[3], v_vertex[4], v_vertex[5]);
            struct vertex_t* v3 = new struct vertex_t(v_vertex[6], v_vertex[7], v_vertex[8]);
            struct loop_t* loop = new struct loop_t(v1, v2, v3);
            struct facet_t* facet = new struct facet_t(n, loop);

            if (iFacet == 0) {
                v_resStlFile->SetFirstFacet(facet);
            } else {
                v_lastFacet->SetNext(facet);
            }
            v_lastFacet = facet;

        }

        delete [] v_dataBuffer;

        fclose(v_inFile);

        return v_resStlFile;
    }
}




            /*fprintf(stderr, "%d: %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n", v_offset,
                            v_dataBuffer[v_offset+0],
                            v_dataBuffer[v_offset+1],
                            v_dataBuffer[v_offset+2],
                            v_dataBuffer[v_offset+3],
                            v_dataBuffer[v_offset+4],
                            v_dataBuffer[v_offset+5],
                            v_dataBuffer[v_offset+6],
                            v_dataBuffer[v_offset+7],
                            v_dataBuffer[v_offset+8],
                            v_dataBuffer[v_offset+9],
                            v_dataBuffer[v_offset+10],
                            v_dataBuffer[v_offset+11]);

            fprintf(stderr, "    %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n",
                            v_dataBuffer[v_offset+12],
                            v_dataBuffer[v_offset+13],
                            v_dataBuffer[v_offset+14],
                            v_dataBuffer[v_offset+15],
                            v_dataBuffer[v_offset+16],
                            v_dataBuffer[v_offset+17],
                            v_dataBuffer[v_offset+18],
                            v_dataBuffer[v_offset+19],
                            v_dataBuffer[v_offset+20],
                            v_dataBuffer[v_offset+21],
                            v_dataBuffer[v_offset+22],
                            v_dataBuffer[v_offset+23]);

            fprintf(stderr, "    %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n",
                            v_dataBuffer[v_offset+24],
                            v_dataBuffer[v_offset+25],
                            v_dataBuffer[v_offset+26],
                            v_dataBuffer[v_offset+27],
                            v_dataBuffer[v_offset+28],
                            v_dataBuffer[v_offset+29],
                            v_dataBuffer[v_offset+30],
                            v_dataBuffer[v_offset+31],
                            v_dataBuffer[v_offset+32],
                            v_dataBuffer[v_offset+33],
                            v_dataBuffer[v_offset+34],
                            v_dataBuffer[v_offset+35]);

            fprintf(stderr, "    %02x%02x%02x%02x %02x%02x%02x%02x %02x%02x%02x%02x\n",
                            v_dataBuffer[v_offset+36],
                            v_dataBuffer[v_offset+37],
                            v_dataBuffer[v_offset+38],
                            v_dataBuffer[v_offset+39],
                            v_dataBuffer[v_offset+40],
                            v_dataBuffer[v_offset+41],
                            v_dataBuffer[v_offset+42],
                            v_dataBuffer[v_offset+43],
                            v_dataBuffer[v_offset+44],
                            v_dataBuffer[v_offset+45],
                            v_dataBuffer[v_offset+46],
                            v_dataBuffer[v_offset+47]);

            fprintf(stderr, "    %02x%02x\n",
                            v_dataBuffer[v_offset+48],
                            v_dataBuffer[v_offset+49]);*/
