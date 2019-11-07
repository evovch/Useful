#include "cls_surfacepoint_solver.h"
#include "cls_implicit_sphere.h"
#include "cls_vec3.h"

int main(int argc, char** argv)
{
    cls_implicit_sphere sph1;

    cls_surfacepoint_solver solver1;

    cls_vec3 q(4.0f, 4.0f, 4.0f);

    solver1.SetStartPoint(q);
    solver1.Calculate(&sph1);
    
    return 0;
}
