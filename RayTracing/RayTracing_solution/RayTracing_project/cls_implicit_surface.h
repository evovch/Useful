#ifndef CLS_IMPLICIT_SURFACE_H
#define CLS_IMPLICIT_SURFACE_H

class cls_vec3;

class cls_implicit_surface
{
public:

    cls_implicit_surface(void)
    {
    }

    virtual ~cls_implicit_surface(void)
    {
    }

    virtual float Evalute(const cls_vec3& p_x) const = 0;

    virtual const cls_vec3& Evaluate_deriv(const cls_vec3& p_x) const = 0;

};

#endif // CLS_IMPLICIT_SURFACE_H
