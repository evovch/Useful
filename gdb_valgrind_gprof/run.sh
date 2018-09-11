#!/bin/bash

# Cleanup
rm -fv debug.txt memcheck.txt prof.txt gmon.out

# Debugger
gdb -batch -x cmnds.txt --args ./prog > debug.txt

# Memory leak detector
valgrind --tool=memcheck --leak-check=full ./prog 2> memcheck.txt

# Profiler
gprof ./prog > prof.txt
