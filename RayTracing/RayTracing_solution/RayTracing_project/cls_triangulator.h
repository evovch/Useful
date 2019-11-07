#ifndef CLS_TRIANGULATOR_H
#define CLS_TRIANGULATOR_H

#include "cls_vec3.h"

class cls_implicit_surface;

class cls_triangulator
{
public:

    cls_triangulator()
    {
    }

    ~cls_triangulator()
    {
    }

    void Triangulate(cls_implicit_surface const * const p_surface);

};

#endif // CLS_TRIANGULATOR_H
