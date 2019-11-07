#include "cls_plane_surface.h"
#include "cls_ray.h"
#include "cls_intersection_finder.h"
#include "cls_point.h"

int main2(int argc, char** argv)
{
    cls_plane_surface plane1(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);

    cls_ray ray1(-2.0f, -2.0f, -2.0f, 1.0f, 1.0f, 1.0f);

    cls_intersection_finder inter_finder1;

    //cls_point intersection1 = inter_finder1.Intersect(&plane1, &ray1);

    return 0;
}
