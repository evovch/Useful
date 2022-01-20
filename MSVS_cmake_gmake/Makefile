.PHONY: clean all UserLibrary UserLibraryClient

MINGWDIR="/c/Program\ Files/mingw-w64/x86_64-8.1.0-posix-seh-rt_v6-rev0/mingw64/bin"
COMPILER_CPP="$(MINGWDIR)/g++.exe"
DEFGENERATOR="$(MINGWDIR)/gendef.exe"
BLDDIR=build_gmake
INSDIR=install_gmake

FLAGS_DLL=-shared -fPIC -O3
FLAGS_EXE=-O3

clean:
	rm -f $(BLDDIR)/UserLibrary/*.o
	rm -f $(BLDDIR)/UserLibraryClient/*.o
	rm -f $(INSDIR)/UserLibrary.dll
	rm -f $(INSDIR)/UserLibrary.def
	rm -f $(INSDIR)/UserLibraryClient.exe

all: UserLibrary UserLibraryClient

UserLibrary: $(INSDIR)/UserLibrary.dll $(INSDIR)/UserLibrary.def

UserLibraryClient: $(INSDIR)/UserLibraryClient.exe

# UserLibrary

$(INSDIR)/UserLibrary.def: $(INSDIR)/UserLibrary.dll
	$(DEFGENERATOR) - $^ > $@ 2> /dev/null

$(INSDIR)/UserLibrary.dll: $(BLDDIR)/UserLibrary/TheClass.o
	$(COMPILER_CPP) $(FLAGS_DLL) -o $@ $^

$(BLDDIR)/UserLibrary/TheClass.o: UserLibrary/source/TheClass.cpp
	$(COMPILER_CPP) $(FLAGS_DLL) -o $@ $^ -c -IUserLibrary/include -DUserLibrary_EXPORTS

# UserLibraryClient

$(INSDIR)/UserLibraryClient.exe: $(BLDDIR)/UserLibraryClient/UserLibraryClient.o
	$(COMPILER_CPP) $(FLAGS_EXE) -o $@ $^ -lUserLibrary

$(BLDDIR)/UserLibraryClient/UserLibraryClient.o: UserLibraryClient/source/UserLibraryClient.cpp
	$(COMPILER_CPP) $(FLAGS_EXE) -o $@ $^ -c -IUserLibrary/include
