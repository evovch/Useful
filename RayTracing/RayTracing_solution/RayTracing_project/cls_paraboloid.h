/**

    @class cls_paraboloid

*/

#ifndef CLS_PARABOLOID_H
#define CLS_PARABOLOID_H

#include "cls_explicit_surface.h" // mother class

class cls_paraboloid : public cls_explicit_surface
{
public:
    cls_paraboloid() :
        cls_explicit_surface()
        , m_a(1.0)
        , m_b(1.0)
    {
    }

    ~cls_paraboloid()
    {
    }

    virtual float EvaluateZ(float p_x, float p_y) const override final
    {
        return (p_x*p_x) / (m_a*m_a) + (p_y*p_y) / (m_b*m_b);
    }

private:
    float m_a;
    float m_b;

};

#endif // CLS_PARABOLOID_H
