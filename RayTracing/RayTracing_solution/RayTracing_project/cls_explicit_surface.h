#ifndef CLS_EXPLICIT_SURFACE_H
#define CLS_EXPLICIT_SURFACE_H

class cls_explicit_surface
{
public:

    cls_explicit_surface()
    {
    }

    virtual ~cls_explicit_surface()
    {
    }

    virtual float EvaluateZ(float p_x, float p_y) const = 0;

};

#endif // CLS_EXPLICIT_SURFACE_H
