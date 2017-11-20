#include "stlFile.h"

#include <iostream>
#include <fstream>
using std::cout;
using std::cerr;
using std::endl;

#include "cppTokenizer.h"

stlFile::stlFile(std::string fileNameInput_, std::string fileNameOutput_)
  : fileNameInput(fileNameInput_),
    fileNameOutput(fileNameOutput_),
    amountCoordinates(0)
{
}

int stlFile::ReadFile() {
  std::ifstream infile(fileNameInput); //создаем входной файл
  if (!infile) {
    cout << "Error!" << endl;
    exit(1);
  }

  std::string curLine;
  int i = 0;
  while (!infile.eof()) { //цикл до конца файла EOF
    getline(infile, curLine);
    if (curLine.find("vertex") != std::string::npos) { //TODO check!!! -1 is wrong
      //cout << curLine << endl;
      getCoordinates(curLine);
      i++;
    }
  }
  cout << "(number of vertices found in the input stl file) i=" << i << endl;
  amountCoordinates = i; //количество координат в файле
  return 0;
}

void stlFile::getCoordinates(std::string str) {
  std::string delims(" \t");
  std::vector<std::string> tokens;
  tokens = tokenize(str, delims);

  //// Kept for debugging
  //cout << str << endl;
  //for (auto iter: tokens) {
  //  cout << iter << endl;
  //}

  if (tokens.size() != 4) {
    //TODO specify input file line number
    cerr << "Unable to extract coordinates of the vertex in the input file." << endl;
    exit(1);
  }

  //TODO implement checks and corresponding messages
  if (tokens.at(0) != std::string("vertex")) {
    cerr << "Something went wrong." << endl;
  }

  // Remove carriage return character created by Windows
  if (tokens.at(3).back() == '\r') {
    tokens.at(3).pop_back();
  }

  vec_x.push_back(tokens.at(1));
  vec_y.push_back(tokens.at(2));
  vec_z.push_back(tokens.at(3));
}

void stlFile::displayCoordinates() const {
  int N = vec_x.size();
  cout << "Coordinates:x,y,z:" << endl;
  for (int i=0; i<N; i++) {
    cout << vec_x[i] << "\t" << vec_y[i] << "\t" << vec_z[i] << endl;
  }
}

void stlFile::printTemplateStlFile() {
  std::ofstream outfile(fileNameOutput);
  if (!outfile) {
    cout << "Error!" << endl;
    exit(1);
  }

  // Start <define> section
  outfile << "<define>" << endl;

  // Populate <define> section
  for (int i=0; i<amRealCoor; i++) {
    outfile << "\t<position name = \"v" << i << "\" unit = \"m"
            << "\" x =\"" << x[i]
            << "\" y =\"" << y[i]
            << "\" z =\"" << z[i]
            << "\" />" << endl;
  }

  // End <define> section
  outfile << "</define>" << endl;

  // Blank line separator between the sections
  outfile << endl;

  // Start <tessellated> section
  outfile << "<tessellated aunit = \"degree\" lunit = \"mm\" name = \"tessel\">" << endl;

  // Populate <tessellated> section
  int vertNum1, vertNum2, vertNum3;
  for (int i=0; i<amountCoordinates; i+=3) {
    vertNum1 = getIndexVertex(i);
    vertNum2 = getIndexVertex(i+1);
    vertNum3 = getIndexVertex(i+2);
    outfile << "<\ttriangular vertex1 = \"v" << vertNum1
                       << "\" vertex2 = \"v" << vertNum2
                       << "\" vertex3 = \"v" << vertNum3
                       << "\" />" << endl;
  }

  // End <tessellated> section
  outfile << "</tessellated>" << endl;

  // Blank line at the end of the file
  outfile << endl;
}

void stlFile::searchUnicVertex() { //подсчет уникальных элементов в массиве
  x.push_back(vec_x[0]);
  y.push_back(vec_y[0]);
  z.push_back(vec_z[0]);
  int k = 0;
  bool flag = true;
  for (int i=1; i<amountCoordinates; i++) {
    for (unsigned int j = 0; j < x.size(); j++) {
      if (x[j]==vec_x[i] && y[j] == vec_y[i] && z[j] == vec_z[i]) {
        flag = false;
      }
    }
    if (flag) {
      x.push_back(vec_x[i]);
      y.push_back(vec_y[i]);
      z.push_back(vec_z[i]);
      k++;
    }
    flag = true;
  }

  amRealCoor = k+1;
  /*
  for (int i = 0; i < amRealCoor; i++) {
    cout << x[i] << endl;
    cout << y[i] << endl;
    cout << z[i] << endl;
    cout << endl;
  }
  */
}

int stlFile::getIndexVertex(int i) {
  for (int j=0; j<amRealCoor; j++) {
    if (x[j] == vec_x[i] && y[j] == vec_y[i] && z[j] == vec_z[i])
      return j;
  }
  return -1;
}
