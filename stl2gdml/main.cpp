#include "stlFile.h"

#include <iostream>

//TODO check units (m, mm)
//TODO English comments

int main(int argc, char** argv)
{
  if (argc != 3) {
    std::cerr << "Wrong number of arguments. Please provide input and output files names." << std::endl;
    std::cerr << "Usage ./stl2gdml <input.stl> <output.gdml>" << std::endl;
    exit(1);
  }

  stlFile *file = new stlFile(argv[1], argv[2]);

  file->ReadFile();
  //file->displayCoordinates();
  file->searchUnicVertex();
  file->printTemplateStlFile();

  delete file;
}
