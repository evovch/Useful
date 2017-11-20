#include <TApplication.h>

#include "matrix.h"

void matrixTest();

int main(int argc, char** argv)
{
  TApplication app("matrixTest", &argc, argv);

  matrixTest();

  app.Run();
  return 0;
}

void matrixTest()
{
  matrix M1;
  M1.RunScan();
}
