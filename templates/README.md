An example of using C++ templates
=================================

There are three implementations of the same functionality - base class, child class which inherits from the base class and the main function which creates an object of a child class and calls for two methods.

Compilation
-----------

Just run make.

The tree implementations
------------------------

**main1.cpp** which produces prog1 just implements everything without templates but with a concrete data type (int) within one file.

**main2.cpp** which produces prog2 implements the same using templates, also within one file.

**baseClass.h/cpp** and **childClass.h/cpp** are compiled into a shared object libClasses.so (aka dynamic library). These classes are absolutely equivalent to the ones implemented in main2.cpp but this time everything is split across header and source files. **main3.cpp** is compiled into prog3 which uses libClasses.so

The two methods
---------------

*method1()* is implemented in the child class while in the base class it is declared as pure virtual.

*method2()* is implemented only in the base class and the child class knows nothing about it.

*method1()* is called from the method2().

Thus, if you create an object of a child class you can use both *method1()* and *method2()*.
In this case using *method1()* is somewhat natural - it is implemented in the child class and you call via the child class object. More interesting is to call method2() which is literally written only in the base class but it is inherited by the child class. Moreover, method2() calls for the *method1()*.

Things to learn
---------------

1) If you use templates you have to use 'this' keyword to access data members and methods of the mother class. Compare **main1.cpp** and **main2.cpp**.

2) If you separate template declarations and implementation using standard header/source ideology, you have to somehow specify which concrete implementations should be generated from the template. Compare **main2.cpp** and **main3.cpp** (and classes' h/cpp).
