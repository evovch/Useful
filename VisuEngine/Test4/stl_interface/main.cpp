#include "cls_stl_interface.h"

int main(int /*argc*/, char** /*argv*/)
{
    const char* v_filename = "../input/teapot.stl";

    cls_stl_interface::Import(v_filename);

    return 0;
}
