///*
#include <iostream>
#include <TCanvas.h>
#include <TGraph.h>
#include <TStopwatch.h>
#include <TFile.h>

Double_t FuncL (Double_t inX, Double_t* b, Int_t n);
Double_t FuncLderiv1 (Double_t inX, Double_t* b, Int_t n);
Double_t FuncLderiv2 (Double_t inX, Double_t* b, Int_t n);
Bool_t Comb (Int_t start, Int_t num, Int_t range);
ULong64_t Factorial (Int_t base);
ULong64_t Newton_koef (Int_t base, Int_t top);
ULong64_t MaxSumLen (Int_t n);
//*/
Int_t combNum;
std::vector<Int_t> curComb;
std::vector<Int_t> *combs;

///*
void Lagrange (const Char_t* filename="data/MICEX_120101_121231_.txt", Int_t offset=0);

int main(int argc, char** argv) {
	Lagrange();
	return 0;
}
//*/

void Lagrange (const Char_t* filename/*="data/MICEX_120101_121231_.txt"*/, Int_t offset/*=0*/)
{
	Int_t n;
	Double_t* x;
	Double_t* y;
	Double_t minInpY;
	Double_t maxInpY;

	// ----------------------------------------------------------------------------------
	// Read data from file into memory (n, x[], y[])
	FILE* f;
	f = fopen(filename, "r");

	Char_t line[255];
/*
	Char_t delimiter = ',';
	Char_t* lstr;
	Char_t* rstr;
*/
	fgets(line, 255, f);
	n = atoi(line)-1;

	std::cout << "n=" << n << std::endl;

	x = (Double_t*)calloc(n+1, sizeof(Double_t));
	y = (Double_t*)calloc(n+1, sizeof(Double_t));

	Int_t i;
	for (i=0; i<offset; i++) fgets(line, 255, f);
	for (i=0; i<=n; i++)
	{
		fgets(line, 255, f);
/*		lstr = strtok(line, &delimiter);
		x[i] = atof(lstr);
		rstr = strtok(NULL, &delimiter);
		y[i] = atof(rstr);
*/
		x[i] = i;
		y[i] = atof(line);

		if (i==0) {
			minInpY = y[i];
			maxInpY = y[i];
		} else {
			if (minInpY > y[i]) minInpY = y[i];
			if (maxInpY < y[i]) maxInpY = y[i];
		}

	}

	fclose(f);

	// ----------------------------------------------------------------------------------
	// Print data from memory
	std::cout << "---------------------------------------------------" << std::endl;
	for (i=0; i<=n; i++)
		std::cout << x[i] << "\t\t" << y[i] << std::endl;
	std::cout << "---------------------------------------------------" << std::endl;

	// ----------------------------------------------------------------------------------
	// Create a canvas and draw initial points
	Double_t w = 600.;
	Double_t h = 600.;

	TCanvas* c1 = new TCanvas("c", "c", w, h);
	c1->SetWindowSize(w + (w - c1->GetWw()), h + (h - c1->GetWh()));
//	c1->Divide (1, 2);
//	c1->cd(1);
	TGraph* gr1 = new TGraph(n+1, x, y);
	gr1->SetTitle (filename);
	gr1->SetMinimum (minInpY-10);
	gr1->SetMaximum (maxInpY+10);
	gr1->Draw("AL*");

	// ----------------------------------------------------------------------------------
	// Lagrange interpolation
	// n - number of points -1

	TStopwatch* fTimer;
	fTimer = new TStopwatch();
	fTimer->Start();

	Double_t* s;
	s = (Double_t*)calloc(n+1, sizeof(Double_t));

	Double_t* b;
	b = (Double_t*)calloc(n+1, sizeof(Double_t));

	Int_t j;
	Double_t curSumB;
	Double_t curSumA;
	Double_t curMult;

	// Compute all s[] and very first b[0]
	curSumB = 0.;
	for (i=0; i<=n; i++)
	{
		curMult = 1.;
		for (j=0; j<=n; j++) {
			if (j==i) continue;
			curMult *= (x[i] - x[j]);
		}
		s[i] = y[i] / curMult;
		curSumB += s[i];
	}
	b[0] = curSumB;
	// Compute end +++++++++++++++++++++++

	free (y);			// ---------No more y below this line

	ULong64_t maxLen = MaxSumLen(n);
	std::cout << "Max length of a sum = " << maxLen << std::endl;
	combs = new std::vector<Int_t>[maxLen];

	//std::cout << "--------------------------------------------" << std::endl;
	for (i=1; i<=n; i++)
	{
		curSumB = 0.;
		curComb.clear();
		combNum=0;
		Comb (0, n, i);
		for (Int_t skip=0; skip<=n; skip++)
		{
			curSumA = 0.;
			for (Int_t k=0; k<combNum; k++) {
				//if (i%2) std::cout << " - ";// << std::endl;
				//else std::cout << " + ";// << std::endl;
				curMult = 1.;
				for (std::vector<Int_t>::iterator iter = combs[k].begin(); iter != combs[k].end(); ++iter) {
					if (*iter >= skip) {
						//std::cout << "x[" << (*iter)+1 << "]*";
						curMult *= x[(*iter)+1];
					} else {
						//std::cout << "x[" << *iter << "]*";
						curMult *= x[*iter];
					}
				}
				curSumA += curMult;
			}
			// a[skip,i] = curSumA
			curSumB += curSumA*s[skip];
			//std::cout << std::endl;
		}
		if (i%2)
			b[i] = -curSumB;
		else
			b[i] = curSumB;
		//std::cout << "--------------------------------------------" << std::endl;
		for (Int_t k=0; k<combNum; k++)
			combs[k].clear();
	}

	delete [] combs;
	free (s);			// ---------No more s below this line

	// ----------------------------------------------------------------------------------
	// Newtons method of extremum search.
	Bool_t extrFound;
	Bool_t newExtr;
	Bool_t goodExtr;
	Int_t numOfExtrs = 0;
	Double_t* extrX;
//	Double_t* extrY;
	Int_t numOfMins = 0;
	Double_t* minX;
	Double_t* minY;
	Int_t numOfMaxs = 0;
	Double_t* maxX;
	Double_t* maxY;

	extrX = (Double_t*)calloc(n+1, sizeof(Double_t));
//	extrY = (Double_t*)calloc(n+1, sizeof(Double_t));
	minX = (Double_t*)calloc(n+1, sizeof(Double_t));
	minY = (Double_t*)calloc(n+1, sizeof(Double_t));
	maxX = (Double_t*)calloc(n+1, sizeof(Double_t));
	maxY = (Double_t*)calloc(n+1, sizeof(Double_t));

	for (i=0; i<=n; i++)
	{
		Double_t curX = x[i];
		Double_t epsilonLd = 0.0001;
		Double_t epsilonX = 0.01;
		Int_t numOfIters = 0;
		Int_t itersThreshold = 10000;
		extrFound = kTRUE;
		while (fabs(FuncLderiv1 (curX, b, n)) > epsilonLd) {
			curX -= FuncLderiv1 (curX, b, n) / FuncLderiv2 (curX, b, n);
			if (numOfIters++ > itersThreshold) {
				extrFound = kFALSE;
				break;
			}
		}
		std::cout << "numOfIters=" << numOfIters << "\t\t";
		std::cout << "X=" << curX << ", Y=" << FuncLderiv1 (curX, b, n) << "\t\t";
		if (extrFound) {

			newExtr = kTRUE;

			// Check whether the extremum is new
			for (Int_t secondExtrI = 0; secondExtrI<numOfExtrs; secondExtrI++) {
				if (fabs(extrX[secondExtrI] - curX) < epsilonX) {
					newExtr = kFALSE;
					break;
				}
			}

			if (newExtr) {
				// Check whether the extremum is within the X range

				if (curX >= x[0] && curX <= x[n]) goodExtr = kTRUE;
				else goodExtr = kFALSE;

				if (goodExtr) {
					std::cout << "=====NEW===== index=" << numOfExtrs << "\t";

					if (FuncLderiv2 (curX, b, n) > 0.) {
						std::cout << "min";
						minX[numOfMins] = curX;
						minY[numOfMins] = FuncL (curX, b, n);
						numOfMins++;
					} else {
						std::cout << "max";
						maxX[numOfMaxs] = curX;
						maxY[numOfMaxs] = FuncL (curX, b, n);
						numOfMaxs++;
					}
					extrX[numOfExtrs] = curX;
	//				extrY[numOfExtrs] = FuncL (curX, b, n);
					numOfExtrs++;
				}
			}
		}
		std::cout << std::endl;
	}

	TGraph* grMin = new TGraph(numOfMins, minX, minY);
	grMin->SetMarkerColor(6);
	grMin->Draw("*");

	TGraph* grMax = new TGraph(numOfMaxs, maxX, maxY);
	grMax->SetMarkerColor(7);
	grMax->Draw("*");


	// Linear approximation of mins and maxs
	Double_t Amin; Double_t Bmin;
	Double_t Amax; Double_t Bmax;

	Double_t dataMin[4] = {0., 0., 0., 0.};
	Double_t dataMax[4] = {0., 0., 0., 0.};

	for (Int_t tmp=0; tmp<numOfMins; tmp++) {
		dataMin[0] += minX[tmp];
		dataMin[1] += minY[tmp];
		dataMin[2] += minX[tmp]*minY[tmp];
		dataMin[3] += minX[tmp]*minX[tmp];		
	}

	Amin = (numOfMins*dataMin[2] - dataMin[0]*dataMin[1]) / (numOfMins*dataMin[3] - dataMin[0]*dataMin[0]);
	Bmin = (dataMin[1] - Amin*dataMin[0])/numOfMins;

	for (Int_t tmp=0; tmp<numOfMaxs; tmp++) {
		dataMax[0] += maxX[tmp];
		dataMax[1] += maxY[tmp];
		dataMax[2] += maxX[tmp]*maxY[tmp];
		dataMax[3] += maxX[tmp]*maxX[tmp];		
	}

	Amax = (numOfMaxs*dataMax[2] - dataMax[0]*dataMax[1]) / (numOfMaxs*dataMax[3] - dataMax[0]*dataMax[0]);
	Bmax = (dataMax[1] - Amax*dataMax[0])/numOfMaxs;

	// Draw approximating lines
	Double_t linMinX[2] = {x[0], x[n]};
	Double_t linMinY[2] = {Amin*x[0]+Bmin, Amin*x[n]+Bmin};
	Double_t linMaxX[2] = {x[0], x[n]};
	Double_t linMaxY[2] = {Amax*x[0]+Bmax, Amax*x[n]+Bmax};

	TGraph* grMinLin = new TGraph(2, linMinX, linMinY);
	grMinLin->SetLineColor(6);
	grMinLin->Draw("L");

	TGraph* grMaxLin = new TGraph(2, linMaxX, linMaxY);
	grMaxLin->SetLineColor(7);
	grMaxLin->Draw("L");
	// Draw end	 +++++++++++++++++++++++

	free (maxY); free (maxX);
	free (minY); free (minX);
//	free (extrY);
	free (extrX);

	// Tabulate resulting function L(x) and draw it on the canvas
	Double_t* Lx;
	Double_t* Ly;
//	Double_t* Lyd1;
//	Double_t* Lyd2;

	Lx = (Double_t*)calloc(8*n+1, sizeof(Double_t));
	Ly = (Double_t*)calloc(8*n+1, sizeof(Double_t));
//	Lyd1 = (Double_t*)calloc(8*n+1, sizeof(Double_t));
//	Lyd2 = (Double_t*)calloc(8*n+1, sizeof(Double_t));

	for (i=0; i<=8*n; i++) {
		Lx[i] = x[0] + i*(x[n]-x[0])/(8*n);
		Ly[i] = FuncL (Lx[i], b, n);
//		Lyd1[i] = FuncLderiv1 (Lx[i], b, n);
//		Lyd2[i] = FuncLderiv2 (Lx[i], b, n);
	}

	TGraph* grL = new TGraph(8*n+1, Lx, Ly);
	grL->SetLineColor(2);
	grL->Draw("L");
/*
	c1->cd(2);
	TGraph* grLd1 = new TGraph(8*n+1, Lx, Lyd1);
	grLd1->SetLineColor(3);
	grLd1->Draw("AL");

	TGraph* grLd2 = new TGraph(8*n+1, Lx, Lyd2);
	grLd2->SetLineColor(4);
	grLd2->Draw("L");
*/
//	free(Lyd2);
//	free(Lyd1);
	free(Ly);
	free(Lx);
	// Tabulate end +++++++++++++++++++++++++++++++++++++++++++++

	free (b);			// ---------No more b below this line
	free (x);			// ---------No more x below this line

	fTimer->Stop();
	Double_t rtime = fTimer->RealTime();
	Double_t ctime = fTimer->CpuTime();
	std::cout << "Real time: " << rtime << ", CPU time: " << ctime << std::endl;
	delete fTimer;

	TFile outputFile("demo.root","update");
	c1->Write();
	gr1->Write();
	grMin->Write();
	grMax->Write();
	grMinLin->Write();
	grMaxLin->Write();
/*
	grL->Write();
	grLd1->Write();
	grLd2->Write();
*/
	outputFile.Close();

//	exit();

}

Double_t FuncL (Double_t inX, Double_t* b, Int_t n)
{
	Double_t sum=0.;
	for (Int_t k=0; k<=n; k++) sum += b[k] * pow(inX, (n-k));
	return sum;
}

Double_t FuncLderiv1 (Double_t inX, Double_t* b, Int_t n)
{
	Double_t sum=0.;
	for (Int_t k=0; k<=n-1; k++) sum += (n-k) * b[k] * pow(inX, (n-k-1));
	return sum;
}

Double_t FuncLderiv2 (Double_t inX, Double_t* b, Int_t n)
{
	Double_t sum=0.;
	for (Int_t k=0; k<=n-2; k++) sum += (n-k) * (n-k-1) * b[k] * pow(inX, (n-k-2));
	return sum;
}

// Recursive function generating all possible combinations of
// ints from range [start; start+num] of length range
// Global
// vector<int> curComb
// vector<int> combs[]
// have to be defined
Bool_t Comb (Int_t start, Int_t num, Int_t range)
{
	if (num == 0) {
		return kFALSE;
	}

	for (Int_t i=start; i<start+num; i++)
	{
		//std::cout << i << "\t\t" << num << "\t\t" << range << std::endl;
		curComb.push_back(i);

		if (range > 1) {
			Bool_t rez = Comb (i+1, num-(i-start)-1, range-1);
			if (!rez) {
				//std::cout << "xxx" << range << std::endl;
				curComb.pop_back();
			}
		} else {
			combs[combNum++] = curComb;
			//std::cout << "---" << range << std::endl;
			curComb.pop_back();
		}
	}

	curComb.pop_back();
	return kTRUE;
}

ULong64_t Factorial (Int_t base)
{
	if (base == 0 || base == 1) return 1;
	return base*Factorial(base-1);
}

ULong64_t Newton_koef (Int_t base, Int_t top)
{
	ULong64_t mult = 1;
	Int_t start;
	if (top == base-top) start = top+1;
	else start = base-top;
	for (Int_t i = start; i <= base; i++)
		mult *= (ULong64_t)i;
	return mult/Factorial(base-top);;
}

ULong64_t MaxSumLen (Int_t n)
{
	return Newton_koef(n, floor(n/2));
}

