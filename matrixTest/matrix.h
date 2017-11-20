#ifndef MATRIX_H
#define MATRIX_H

//class TH2D;
class TGraph2D;

class matrix
{
public:

  matrix();
  matrix(const matrix* arg);
  ~matrix();

  double GetData(int iCol, int jRow) const { return mData[iCol*4+jRow]; }
  void SetData(int iCol, int jRow, double val) { mData[iCol*4+jRow] = val; }

  double GetAlpha() const { return mAlpha; }
  double GetBeta() const { return mBeta; }
  double GetGamma() const { return mGamma; }
  void SetAngles(double a, double b, double g) { mAlpha=a; mBeta=b; mGamma=g; }

  void InitE();
  void InitTest();

  void InitXrot(double gammaDeg);
  void InitYrot(double betaDeg);
  void InitZrot(double alphaDeg);
  void InitXYZposRow(double x, double y, double z);
  void InitXYZposCol(double x, double y, double z);

  void Multiply(matrix right_op);

/*  void MultiplyXrot(double gammaDeg);
  void MultiplyYrot(double betaDeg);
  void MultiplyZrot(double alphaDeg);*/

  void Dump();
  void DumpAngles();
  void Transpose();

  void ExecuteTXYZ();

  void RunScan();

private:

  double mData[16];

  // these are not reconstructed but manually set correct angles
  double mAlpha;
  double mBeta;
  double mGamma;

  //TH2D* mHisto;
  TGraph2D* mGraph;

};

#endif // MATRIX_H
