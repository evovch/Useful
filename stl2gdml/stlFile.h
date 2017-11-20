#ifndef STLFILE_H
#define STLFILE_H

#include <vector>
#include <string>

class stlFile
{
public:
  stlFile(std::string fileNameInput_, std::string fileNameOutput_);
  ~stlFile() {}

  int ReadFile();
  void getCoordinates(std::string str);
  void displayCoordinates() const;
  void printTemplateStlFile();
  void searchUnicVertex();
  int getIndexVertex(int i);

private:

  std::string fileNameInput;
  std::string fileNameOutput;
  int amountCoordinates; // ���������� ��������� � �����
  std::vector<std::string> vec_x;
  std::vector<std::string> vec_y;
  std::vector<std::string> vec_z;
  // ���������� ��� v1,v2.v0...
  std::vector<std::string> x;
  std::vector<std::string> y;
  std::vector<std::string> z;
  int amRealCoor;
};

#endif // STLFILE_H
