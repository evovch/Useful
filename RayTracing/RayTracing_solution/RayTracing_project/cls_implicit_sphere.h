#ifndef CLS_IMPLICIT_SPHERE_H
#define CLS_IMPLICIT_SPHERE_H

#include "cls_implicit_surface.h" // mother class

#include "cls_vec3.h"

class cls_implicit_sphere : public cls_implicit_surface
{
public:

    cls_implicit_sphere(void) :
        cls_implicit_surface()
      , m_a(1.0f)
      , m_b(1.0f)
      , m_c(1.0f)
    {
    }

    virtual ~cls_implicit_sphere()
    {
    }

    virtual float Evalute(const cls_vec3& p_x) const override final
    {
        return (p_x.x()*p_x.x())/(m_a*m_a) + (p_x.y()*p_x.y())/(m_b*m_b) + (p_x.z()*p_x.z())/(m_c*m_c);
    }

    virtual const cls_vec3& Evaluate_deriv(const cls_vec3& p_x) const override final
    {
        return cls_vec3((2.0f*p_x.x())/(m_a*m_a),
                        (2.0f*p_x.y())/(m_b*m_b),
                        (2.0f*p_x.z())/(m_c*m_c));
    }

private:

    float m_a;
    float m_b;
    float m_c;

};

#endif // CLS_IMPLICIT_SPHERE_H
