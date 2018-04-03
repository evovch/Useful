.PHONY: all clean

all:
	flex --header-file=stl_parsing.flex.h -o stl_parsing.flex.cpp stl_parsing.l
	bison --defines=stl_parsing.tab.h -o stl_parsing.tab.cpp stl_parsing.y
	g++ main.cpp stl_parsing.flex.cpp stl_parsing.tab.cpp ../cls_stl_file.cpp -o parsing

clean:
	rm -fv stl_parsing.flex.h stl_parsing.flex.cpp stl_parsing.tab.h stl_parsing.tab.cpp
	rm -fv parsing
