#include "matrix.h"

#include <iostream>
#include <iomanip>
using std::cout;
using std::endl;

#include <cmath>

#include <TCanvas.h>
#include <TFile.h>

//#include <TH2D.h>
#include <TGraph2D.h>

#include "support.h"

#define WIDTH 12

matrix::matrix()
{
  InitE();
  //mHisto = new TH2D("mHisto", "mHisto", 720, -360., 360., 720, -360., 360.);
}

matrix::matrix(const matrix* arg)
{
  for (int jRow=0; jRow<4; jRow++) {
    for (int iCol=0; iCol<4; iCol++) {
      this->SetData(iCol, jRow, arg->GetData(iCol, jRow));
    }
  }
  //TODO implement remaining data members
}

matrix::~matrix()
{
/*  if (mHisto) {
    delete mHisto;
    mHisto = NULL;
  }*/
/*  if (mGraph) {
    delete mGraph;
    mGraph = NULL;
  }*/
}

void matrix::InitE()
{
  for (int jRow=0; jRow<4; jRow++) {
    for (int iCol=0; iCol<4; iCol++) {
      if (iCol == jRow) {
        this->SetData(iCol, jRow, 1.);
      } else {
        this->SetData(iCol, jRow, 0.);
      }
    }
  }
}

void matrix::InitTest()
{
  for (int jRow=0; jRow<4; jRow++) {
    for (int iCol=0; iCol<4; iCol++) {
      this->SetData(iCol, jRow, iCol*4+jRow);
    }
  }
}

void matrix::InitXrot(double gammaDeg)
{
  double gamma = gammaDeg * PI / 180.;

  this->SetData(0, 0, 1.);
  this->SetData(0, 1, 0.);
  this->SetData(0, 2, 0.);
  this->SetData(0, 3, 0.);

  this->SetData(1, 0, 0.);
  this->SetData(1, 1, cos(gamma));
  this->SetData(1, 2, sin(gamma));
  this->SetData(1, 3, 0.);

  this->SetData(2, 0, 0.);
  this->SetData(2, 1, -sin(gamma));
  this->SetData(2, 2, cos(gamma));
  this->SetData(2, 3, 0.);

  this->SetData(3, 0, 0.);
  this->SetData(3, 1, 0.);
  this->SetData(3, 2, 0.);
  this->SetData(3, 3, 1.);
}

void matrix::InitYrot(double betaDeg)
{
  double beta = betaDeg * PI / 180.;

  this->SetData(0, 0, cos(beta));
  this->SetData(0, 1, 0.);
  this->SetData(0, 2, -sin(beta));
  this->SetData(0, 3, 0.);

  this->SetData(1, 0, 0.);
  this->SetData(1, 1, 1.);
  this->SetData(1, 2, 0.);
  this->SetData(1, 3, 0.);

  this->SetData(2, 0, sin(beta));
  this->SetData(2, 1, 0.);
  this->SetData(2, 2, cos(beta));
  this->SetData(2, 3, 0.);

  this->SetData(3, 0, 0.);
  this->SetData(3, 1, 0.);
  this->SetData(3, 2, 0.);
  this->SetData(3, 3, 1.);
}

void matrix::InitZrot(double alphaDeg)
{
  double alpha = alphaDeg * PI / 180.;

  this->SetData(0, 0, cos(alpha));
  this->SetData(0, 1, sin(alpha));
  this->SetData(0, 2, 0.);
  this->SetData(0, 3, 0.);

  this->SetData(1, 0, -sin(alpha));
  this->SetData(1, 1, cos(alpha));
  this->SetData(1, 2, 0.);
  this->SetData(1, 3, 0.);

  this->SetData(2, 0, 0.);
  this->SetData(2, 1, 0.);
  this->SetData(2, 2, 1.);
  this->SetData(2, 3, 0.);

  this->SetData(3, 0, 0.);
  this->SetData(3, 1, 0.);
  this->SetData(3, 2, 0.);
  this->SetData(3, 3, 1.);
}

void matrix::InitXYZposRow(double x, double y, double z)
{
  this->InitE();
  this->SetData(0, 3, x);
  this->SetData(1, 3, y);
  this->SetData(2, 3, z);
}

void matrix::InitXYZposCol(double x, double y, double z)
{
  this->InitE();
  this->SetData(3, 0, x);
  this->SetData(3, 1, y);
  this->SetData(3, 2, z);
}

void matrix::Multiply(matrix right_op)
{
  matrix left_op(this);
  for (int iCol=0; iCol<4; iCol++) {
    for (int jRow=0; jRow<4; jRow++) {
      double sum = 0.;
      for (int k=0; k<4; k++) {
        sum += left_op.GetData(k, jRow) * right_op.GetData(iCol, k);
      }
      this->SetData(iCol, jRow, sum);
    }
  }
}

/*void matrix::MultiplyXrot(double gammaDeg)
{
  matrix right_op;
  right_op.InitXrot(gammaDeg);
  this->Multiply(right_op);
}

void matrix::MultiplyYrot(double betaDeg)
{
  matrix right_op;
  right_op.InitYrot(betaDeg);
  this->Multiply(right_op);
}

void matrix::MultiplyZrot(double alphaDeg)
{
  matrix right_op;
  right_op.InitZrot(alphaDeg);
  this->Multiply(right_op);
}*/

void matrix::Dump()
{
  cout << "-----------------------------------------------" << endl;
  for (int jRow=0; jRow<4; jRow++) {
    for (int iCol=0; iCol<4; iCol++) {
      cout << std::fixed << this->GetData(iCol, jRow) << "\t";
    }
    cout << endl;
  }
  cout << "-----------------------------------------------" << endl;
}

void matrix::DumpAngles()
{
  cout << "-----------------------------------------------" << endl;
  cout << "alph=" << std::fixed << this->mAlpha << endl;
  cout << "beta=" << std::fixed << this->mBeta << endl;
  cout << "gamm=" << std::fixed << this->mGamma << endl;
  cout << "-----------------------------------------------" << endl;
}

void matrix::Transpose()
{
  for (int iCol=0; iCol<4; iCol++) {
    for (int jRow=iCol+1; jRow<4; jRow++) {
      double tmp;
      tmp = this->GetData(iCol, jRow);
      this->SetData(iCol, jRow, this->GetData(jRow, iCol));
      this->SetData(jRow, iCol, tmp);
    }
  }
}

void matrix::ExecuteTXYZ()
{
  //cout << "|||||||||||||||||||||||||  ExecuteTXYZ  |||||||||||||||||||||||||" << endl;
  //this->Dump();

  // A = sin(beta) / (cos(beta)*cos(beta))
  double A = this->GetData(0,2) / (1. - this->GetData(0,2) * this->GetData(0,2));

  // B = sin(alpha) * cos(gamma)
  double B = this->GetData(1,0) + A * this->GetData(1,2) * this->GetData(0,0);

  // C = cos(alpha) * sin(gamma)
  double C = this->GetData(2,1) + A * this->GetData(2,2) * this->GetData(0,1);

  // D = cos(alpha) * cos(gamma)
  double D = this->GetData(1,1) + A * this->GetData(1,2) * this->GetData(0,1);

  // F = sin(alpha) * sin(gamma)
  double F = this->GetData(2,0) + A * this->GetData(2,2) * this->GetData(0,0);

  // G = sin(alpha+gamma)
  double G = B+C;
  // J = cos(alpha+gamma)
  double J = D-F;

  // H = sin(alpha-gamma)
  double H = B-C;
  // K = cos(alpha-gamma)
  double K = D+F;

  // L = sin(gamma-alpha)
  double L = C-B;
  // M = cos(gamma-alpha)
  double M = D+F;

  /*cout << "A=" << A << endl;
  cout << "B=" << B << endl;
  cout << "C=" << C << endl;
  cout << "D=" << D << endl;
  cout << "F=" << F << endl;
  cout << "G=" << G << endl;
  cout << "H=" << H << endl;
  cout << "J=" << J << endl;
  cout << "K=" << K << endl;*/

  // alpha+gamma ver1
  double a_plus_g_1 = myAsin(G, J) * 180. / PI;
  // alpha+gamma ver2
  //double a_plus_g_2 = acos(J) * 180. / PI;

  // alpha-gamma ver1
  double a_minus_g_1 = myAsin(H, K) * 180. / PI;
  // alpha-gamma ver2
  //double a_minus_g_2 = acos(K) * 180. / PI;

  double g_minus_a_1 = myAsin(L, M) * 180. / PI;

  //double apg_diff = a_plus_g_1 - a_plus_g_2;
  //double amg_diff = a_minus_g_1 - a_minus_g_2;

  double recAlpha1 = (a_plus_g_1 + a_minus_g_1) / 2.;
  double recGamma1 = (a_plus_g_1 - a_minus_g_1) / 2.;

  double recAlpha2 = (a_plus_g_1 + g_minus_a_1) / 2.;
  double recGamma2 = (a_plus_g_1 - g_minus_a_1) / 2.;

  // tg(alpha) ver1
  double tga1 = B/D;
  // tg(alpha) ver2
  //double tga2 = F/C;

  // tg(gamma) ver1
  double tgg1 = F/B;
  // tg(gamma) ver2
  //double tgg2 = C/D;

  //double tga_diff = tga1-tga2;
  //double tgg_diff = tgg1-tgg2;

  double recAlpha;
  double recGamma;

  if (tga1 < tgg1) {
    recAlpha = recAlpha2;
    recGamma = recGamma2;
  } else {
    recAlpha = recAlpha1;
    recGamma = recGamma1;
  }

  double alphaDiff = recAlpha - this->GetAlpha();
  double gammaDiff = recGamma - this->GetGamma();

  // --------------------------------------------------------------------------------------------------------

  cout << "beta=" << std::fixed << std::setw(WIDTH) << std::right << this->GetBeta() << "\t";
  cout << " A = " << std::fixed << std::setw(WIDTH) << std::right << this->GetAlpha() << "\t";
  cout << "rA1= " << std::fixed << std::setw(WIDTH) << std::right << recAlpha << "\t";
  cout << " G = " << std::fixed << std::setw(WIDTH) << std::right << this->GetGamma() << "\t";
  cout << "rG1= " << std::fixed << std::setw(WIDTH) << std::right << recGamma << "\t";

  //cout << (signed int)(alphaDiff*1000000.) << "\t";

  ///*
  //if (((signed int)(alphaDiff*1000000.) != 0) &&
  //    ((signed int)(alphaDiff*1000000.) != 360000000) &&
  //    ((signed int)(alphaDiff*1000000.) !=-360000000))
  {
    if ((signed int)(alphaDiff*1000000.) == 360000000) {
      alphaDiff -= 360.;
    } else if ((signed int)(alphaDiff*1000000.) ==-360000000) {
      alphaDiff += 360.;
    }

    cout << "difa=" << std::fixed << std::setw(WIDTH) << std::right << alphaDiff << "\t";
  }
  //*/

  ///*
  //if (((signed int)(gammaDiff*1000000.) != 0) &&
  //    ((signed int)(gammaDiff*1000000.) != 360000000) &&
  //    ((signed int)(gammaDiff*1000000.) !=-360000000))
  {
    if ((signed int)(gammaDiff*1000000.) == 360000000) {
      gammaDiff -= 360.;
    } else if ((signed int)(gammaDiff*1000000.) ==-360000000) {
      gammaDiff += 360.;
    }

    cout << "difg=" << std::fixed << std::setw(WIDTH) << std::right << gammaDiff << "\t";
  }
  //*/

  //cout << "difg=" << std::fixed << std::setw(WIDTH) << std::right << gammaDiff << "\t";

  //if (((signed int)(apg_diff*100000.) != 0) || ((signed int)(amg_diff*100000.) != 0))
  {
    //cout << "apg1=" << std::fixed << std::setw(WIDTH) << std::right << a_plus_g_1 << "\t";
    //cout << "diff=" << std::fixed << std::setw(WIDTH) << std::right <<
      //a_plus_g_1 - (this->GetAlpha() + this->GetGamma()) << "\t";
    //cout << "apg2=" << std::fixed << std::setw(WIDTH) << std::right << a_plus_g_2 << "\t";
    //cout << std::fixed << "amg1= " << a_minus_g_1 << "\t";
    //cout << std::fixed << "amg2= " << a_minus_g_2 << "\t";
    //cout << std::fixed << "apg_d=" << apg_diff;
    //cout << std::fixed << "amg_d=" << amg_diff;
    cout << endl;
  }
  //else
  {
    //cout << endl;
  }

  //if (((signed int)(tga_diff*100000.) != 0) || ((signed int)(tgg_diff*100000.) != 0))
  {
    //cout << "tga1= " << tga1 << "\t";
    //cout << "tga2= " << tga2 << "\t";
    //cout << "tgg1= " << tgg1 << "\t";
    //cout << "tgg2= " << tgg2 << "\t";
    //cout << "tga_d=" << tga_diff << "\t";
    //cout << "tgg_d=" << tgg_diff << "\t";
    //cout << endl;
  }
  //else
  {
    //cout << endl;
  }

}

void matrix::RunScan()
{
  double alphaMin = -90.;
  double alphaMax = 90.;
  double betaMin = -90.;
  double betaMax = 90.;
  double gammaMin = -90.;
  double gammaMax = 90.;

  double alphaStep = 10.;
  double betaStep = 30.;
  double gammaStep = 10.;

  unsigned int iPoint = 1;
  unsigned int nPoints = (1+(alphaMax-alphaMin)/alphaStep) +
                         (1+(betaMax-betaMin)/betaStep) +
                         (1+(gammaMax-gammaMin)/gammaStep);

  mGraph = new TGraph2D(nPoints);
  mGraph->SetName("mGraph");

  //double beta = 30.;
  for (double beta = betaMin; beta <= betaMax; beta += betaStep)
  {
    //double alpha = 30.;
    for (double alpha = alphaMin; alpha <= alphaMax; alpha += alphaStep)
    {
      //double gamma = 30.;
      for (double gamma = gammaMin; gamma <= gammaMax; gamma += gammaStep)
      {
        // cout << "alph=" << std::fixed << std::setw(WIDTH) << std::right << alpha << "\t";
        // cout << "beta=" << std::fixed << std::setw(WIDTH) << std::right << beta << "\t";
        // cout << "gamm=" << std::fixed << std::setw(WIDTH) << std::right << gamma << "\t";
        // cout << "a+g= " << std::fixed << std::setw(WIDTH) << std::right << alpha+gamma << "\t";
        // cout << "a-g= " << std::fixed << std::setw(WIDTH) << std::right << alpha-gamma << "\t";
        // cout << endl;

        matrix Mz;
        Mz.InitZrot(alpha);
        cout << std::fixed << "Mz (alph=" << alpha << ")" << endl;
        Mz.Dump();

        matrix My;
        My.InitYrot(beta);
        cout << std::fixed << "My (beta=" << beta << ")" << endl;
        My.Dump();

        matrix Mx;
        Mx.InitXrot(gamma);
        cout << std::fixed << "Mx (gamm=" << gamma << ")" << endl;
        Mx.Dump();

        matrix Mtr;
        Mtr.InitXYZposRow(3.3, 4.4, 5.5);
        cout << std::fixed << "Mtr (x=3.3, y=4.4, z=5.5)" << endl;
        Mtr.Dump();

        matrix Mtxyz;

        Mtxyz.SetAngles(alpha, beta, gamma);

        Mtxyz.InitE();
        cout << "Mtxyz = E" << endl;
        Mtxyz.Dump();

        Mtxyz.Multiply(Mz);
        cout << "Mtxyz *= Mz" << endl;
        Mtxyz.Dump();

        Mtxyz.Multiply(My);
        cout << "Mtxyz *= My" << endl;
        Mtxyz.Dump();

        Mtxyz.Multiply(Mx);
        cout << "Mtxyz *= Mx" << endl;
        Mtxyz.Dump();

        Mtxyz.Multiply(Mtr);
        cout << "Mtxyz *= Mtr" << endl;
        Mtxyz.Dump();


        Mtxyz.ExecuteTXYZ();

        mGraph->SetPoint(iPoint, alpha, beta, gamma);
        iPoint++;
      }
    }
    cout << endl;
  }

  TFile* outFile = new TFile("res.root", "RECREATE");
  mGraph->Write();
  outFile->Close();

  //TCanvas* canv = new TCanvas("canv", "canv");
  //canv->cd();
  //mHisto->Draw();
  //gPad->SetGrid(1, 1);
  //mGraph->Draw();
}
