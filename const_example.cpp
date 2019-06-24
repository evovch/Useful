/**
 * Example of const specifier usage.
 * There are 8 possible combinations, only some of them are allowed:
 * VAR/CONST pointer to a VAR/CONST data (pointing actually to a VAR/CONST data)
 */

int main(int argc, char** argv)
{
    int a=0; // variable data
    a=10; // it's fine to change variables
    int aa=0; // (another) variable data
    aa=11; // it's fine to change variables

    const int b=0; // const data
    //b=20; // error // you can not change const data
    const int bb=0; // (another) const data
    //bb=21; // error // you can not change const data

    // =========================================================================

    // variable pointer to a variable data (pointing actually to a variable data)
    int * pa = &a;
    *pa = 1; // you can change the data
    pa = &aa; // you can change the pointer

    // variable pointer to a variable data (pointing actually to a const data)
    //int * pb = &b; // error // this would be a 'constantness-loosing' cast
    // 2

    // =========================================================================

    // variable pointer to a const data (pointing actually to a variable data)
    const int * cipa = &a;
    int const * icpa = &a; // "const int *"  ==  "int const *"
    //*cipa = 3; // error // you can not change the underlying data because it is seen as const (though it is variable by its definition)
    //*icpa = 3; // error // you can not change the underlying data because it is seen as const (though it is variable by its definition)
    cipa = &aa; // you can redirect the pointer to another variable data
    icpa = &aa; // you can redirect the pointer to another variable data

    // variable pointer to a const data (pointing actually to a const data)
    const int * cipb = &b;
    int const * icpb = &b; // "const int *"  ==  "int const *"
    //*cipb = 4; // error // you can not change the underlying data because it is both const by definition and seen as const
    //*icpb = 4; // error // you can not change the underlying data because it is both const by definition and seen as const
    cipb = &bb; // you can redirect the pointer to another const data
    icpb = &bb; // you can redirect the pointer to another const data

    // =========================================================================
    //int * const ipca0; // error // you must initialize a const pointer as there must be some value which you will not be able to change later on
    // =========================================================================

    // const pointer to a variable data (pointing actually to a variable data)
    int * const ipca = &a;
    *ipca = 5;
    //ipca = &aa; // error // you can not redirect a constant pointer

    // const pointer to a variable data (pointing actually to a const data)
    //int * const ipcb = &b; // error // this would be a 'constantness-loosing' cast
    // 6

    // =========================================================================
    //const int * const cipca0; // error // you must initialize a const pointer as there must be some value which you will not be able to change later on
    // =========================================================================

    // const pointer to a const data (pointing actually to a variable data)
    const int * const cipca = &a;
    int const * const icpca = &a; // "const int *"  ==  "int const *"

    //*cipca = 7; // error // you can not change the underlying data because it is seen as const (though it is variable by its definition)
    //*icpca = 7; // error // you can not change the underlying data because it is seen as const (though it is variable by its definition)
    //cipca = &aa; // error // you can not redirect a constant pointer
    //icpca = &aa; // error // you can not redirect a constant pointer

    // const pointer to a const data (pointing actually to a const data)
    const int * const cipcb = &b;
    int const * const icpcb = &b; // "const int *"  ==  "int const *"

    //*cipcb = 8; // error // you can not change the underlying data because it is both const by definition and seen as const
    //*icpcb = 8; // error // you can not change the underlying data because it is both const by definition and seen as const
    //cipcb = &bb; // error // you can not redirect a constant pointer
    //icpcb = &bb; // error // you can not redirect a constant pointer

    // =========================================================================

    // Duplicate equivalent const specifiers - makes no sense
    // Again, as the two definitions are equvalent ("const int *"  ==  "int const *")
    // putting const both before the type and after makes no sense.
    //const int const * cicp;
    //const int const * const cicpc;

    // =========================================================================

    // All the principles of the examples below are similar to the examples above, so I skip some comments.

    // Further on I also skip the second notation (int const *) and use only (const int *)

    // =========================================================================

    // Just to make everything as clear as possible, I use static memory here

    // variable 1d arrays
    int array0v[5] = {  0,  1,  2,  3,  4 };
    int array1v[5] = { 10, 11, 12, 13, 14 };
    int array2v[5] = { 20, 21, 22, 23, 24 };

    // const 1d arrays
    const int array0c[5] = {  0,  1,  2,  3,  4 };
    const int array1c[5] = { 10, 11, 12, 13, 14 };
    const int array2c[5] = { 20, 21, 22, 23, 24 };

    // 2d arrays in C/C++ are represented as 1d arrays of pointers to 1d arrays.

    // 2d array as an array of variable pointers to variables
    int * arrayvv[3] = { array0v, array1v, array2v };

    // 2d array as an array of variable pointers to constants
    const int * arrayvc[3] = { array0c, array1c, array2c };

    // 2d array as an array of const pointers to variables
    int * const arraycv[3] = { array0v, array1v, array2v };

    // 2d array as an array of const pointers to constants
    const int * const arraycc[3] = { array0c, array1c, array2c };

    // =========================================================================
    // Have fun!
    // =========================================================================


    // The outermost poiters are reassignable
    int ** ippm0;
    int ** ippm1 = arrayvv;
    //int ** ippm2 = arrayvc; // error // this would be a 'constantness-loosing' cast
    //int ** ippm3 = arraycv; // error // this would be a 'constantness-loosing' cast
    //int ** ippm4 = arraycc; // error // this would be a 'constantness-loosing' cast

    // The outermost poiters are not reassignable
    //int ** const ippcm0; // error // need initialization
    int ** const ippcm1 = arrayvv;
    //int ** const ippcm2 = arrayvc; // error // this would be a 'constantness-loosing' cast
    //int ** const ippcm3 = arraycv; // error // this would be a 'constantness-loosing' cast
    //int ** const ippcm4 = arraycc; // error // this would be a 'constantness-loosing' cast


    // The outermost poiters are reassignable
    const int ** cippm0;
    //const int ** cippm1 = arrayvv;
    const int ** cippm2 = arrayvc;
    //const int ** cippm3 = arraycv;
    //const int ** cippm4 = arraycc;

    // The outermost poiters are not reassignable
    //const int ** const cippcm0; // error // need initialization
    //const int ** const cippcm1 = arrayvv;
    const int ** const cippcm2 = arrayvc;
    //const int ** const cippcm3 = arraycv;
    //const int ** const cippcm4 = arraycc;


    // The outermost poiters are reassignable
    int * const * ipcpm0;
    int * const * ipcpm1 = arrayvv;
    //int * const * ipcpm2 = arrayvc;
    int * const * ipcpm3 = arraycv;
    //int * const * ipcpm4 = arraycc;

    // The outermost poiters are not reassignable
    //int * const * const ipcpcm0; // error // need initialization
    int * const * const ipcpcm1 = arrayvv;
    //int * const * const ipcpcm2 = arrayvc;
    int * const * const ipcpcm3 = arraycv;
    //int * const * const ipcpcm4 = arraycc;


    // The outermost poiters are reassignable
    const int * const * cipipm0;
    const int * const * cipipm1 = arrayvv;
    const int * const * cipipm2 = arrayvc;
    const int * const * cipipm3 = arraycv;
    const int * const * cipipm4 = arraycc;

    // The outermost poiters are not reassignable
    //const int * const * const cipcpcm0; // error // need initialization
    const int * const * const cipcpcm1 = arrayvv;
    const int * const * const cipcpcm2 = arrayvc;
    const int * const * const cipcpcm3 = arraycv;
    const int * const * const cipcpcm4 = arraycc;


    return 0;
}
