# Useful

Just collect different useful codes here.

Move_constructors
-----------------

If you have an std collection as a class data member and you use std::swap in the move constructor, you don't need to clear that collection after the swap.

Inheritance_constructors
------------------------

If you have a **Base** and a **Child** class and you implement the move constructor, it is important to use std::move for the base class.
