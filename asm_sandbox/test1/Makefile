COMPILER=gcc

prog: functions_c.o functions_asm.o main.o
	$(COMPILER) $^ -o $@

functions_asm.o: functions_asm.s
	as $^ -o $@

functions_c.o: functions_c.c
	$(COMPILER) -c $^ -o $@

main.o: main.c
	$(COMPILER) -c $^ -o $@

.PHONY: clean

clean:
	rm -f *.o
	rm -f prog
