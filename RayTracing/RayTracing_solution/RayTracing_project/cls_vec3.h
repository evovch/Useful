#ifndef CLS_VEC3_H
#define CLS_VEC3_H

#include <cmath>

class cls_vec3
{
public:

    cls_vec3(void) :
        m_x(0.0f), m_y(0.0f), m_z(0.0f)
    {
    }

    cls_vec3(float p_x, float p_y, float p_z) :
        m_x(p_x), m_y(p_y), m_z(p_z)
    {
    }

    ~cls_vec3(void)
    {
    }

    // vector+vector
    cls_vec3 operator+(const cls_vec3& rhs) const
    {
        cls_vec3 rez;
        rez.m_x = this->m_x + rhs.m_x;
        rez.m_y = this->m_y + rhs.m_y;
        rez.m_z = this->m_z + rhs.m_z;
        return rez;
    }

    // vector-vector
    cls_vec3 operator-(const cls_vec3& rhs) const
    {
        cls_vec3 rez;
        rez.m_x = this->m_x - rhs.m_x;
        rez.m_y = this->m_y - rhs.m_y;
        rez.m_z = this->m_z - rhs.m_z;
        return rez;
    }

    // Cross-product of two vectors
    float operator*(const cls_vec3& rhs) const
    {
        float rez = 0.0f;
        rez += this->m_x * rhs.m_x;
        rez += this->m_y * rhs.m_y;
        rez += this->m_z * rhs.m_z;
        return rez;
    }

    // vector*scalar
    cls_vec3 operator*(float rhs) const
    {
        cls_vec3 rez;
        rez.m_x = this->m_x * rhs;
        rez.m_y = this->m_y * rhs;
        rez.m_z = this->m_z * rhs;
        return rez;
    }

    float LengthSquared(void) const {
        return m_x*m_x + m_y*m_y + m_z*m_z;
    }

    float Length(void) const {
        return sqrt(this->LengthSquared());
    }

    float Distance(const cls_vec3& other) const
    {
        cls_vec3 diff = *this - other;
        return diff.Length();
    }

    float x(void) const { return m_x; }
    float y(void) const { return m_y; }
    float z(void) const { return m_z; }

private:

    float m_x;
    float m_y;
    float m_z;

};

#endif // CLS_VEC3_H
