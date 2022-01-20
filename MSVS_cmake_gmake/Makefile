.PHONY: clean all UserLibrary UserLibrary2 UserLibraryClient

MINGWDIR="/c/Program\ Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin"
COMPILER_CPP="$(MINGWDIR)/g++.exe"
DEFGENERATOR="$(MINGWDIR)/gendef.exe"
BLDDIR=build_gmake
INSDIR=install_gmake

FLAGS_DLL=-shared -fPIC -O3
FLAGS_EXE=-O3

all: UserLibrary UserLibrary2 UserLibraryClient

UserLibrary: $(INSDIR)/UserLibrary.dll $(INSDIR)/UserLibrary.def
UserLibrary2: $(INSDIR)/UserLibrary2.dll $(INSDIR)/UserLibrary2.def
UserLibraryClient: $(INSDIR)/UserLibraryClient.exe

clean:
	rm -f $(BLDDIR)/UserLibrary/*.o
	rm -f $(BLDDIR)/UserLibrary2/*.o
	rm -f $(BLDDIR)/UserLibraryClient/*.o
	rm -f $(INSDIR)/UserLibrary.dll
	rm -f $(INSDIR)/UserLibrary.def
	rm -f $(INSDIR)/UserLibrary.lib
	rm -f $(INSDIR)/UserLibrary2.dll
	rm -f $(INSDIR)/UserLibrary2.def
	rm -f $(INSDIR)/UserLibrary2.lib
	rm -f $(INSDIR)/UserLibraryClient.exe

# UserLibrary

$(INSDIR)/UserLibrary.def: $(INSDIR)/UserLibrary.dll
	$(DEFGENERATOR) - $^ > $@ 2> /dev/null

$(INSDIR)/UserLibrary.dll: $(BLDDIR)/UserLibrary/TheClass.o
	$(COMPILER_CPP) $(FLAGS_DLL) -o $@ $^

$(BLDDIR)/UserLibrary/TheClass.o: UserLibrary/source/TheClass.cpp
	$(COMPILER_CPP) $(FLAGS_DLL) -o $@ $^ -c -IUserLibrary/include -DUserLibrary_EXPORTS


# UserLibrary2

$(INSDIR)/UserLibrary2.def: $(INSDIR)/UserLibrary2.dll
	$(DEFGENERATOR) - $^ > $@ 2> /dev/null

$(INSDIR)/UserLibrary2.dll: $(BLDDIR)/UserLibrary2/TheClass2.o
	$(COMPILER_CPP) $(FLAGS_DLL) -o $@ $^

$(BLDDIR)/UserLibrary2/TheClass2.o: UserLibrary2/source/TheClass2.cpp
	$(COMPILER_CPP) $(FLAGS_DLL) -o $@ $^ -c -IUserLibrary2/include -DUserLibrary2_EXPORTS


# UserLibraryClient

$(INSDIR)/UserLibraryClient.exe: $(BLDDIR)/UserLibraryClient/UserLibraryClient.o
	$(COMPILER_CPP) $(FLAGS_EXE) -o $@ $^ -lUserLibrary -lUserLibrary2

$(BLDDIR)/UserLibraryClient/UserLibraryClient.o: UserLibraryClient/source/UserLibraryClient.cpp
	$(COMPILER_CPP) $(FLAGS_EXE) -o $@ $^ -c -IUserLibrary/include -IUserLibrary2/include
