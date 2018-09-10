#include <cstdio>
#include <cstdlib>

#include <TApplication.h>
#include <TCanvas.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TStyle.h>

#define NPOINTS 61
#define NCOMPS 7
#define BUFSIZE 32
#define XMIN 0.f
#define XMAX 4.f

int main(int argc, char** argv)
{
	TApplication app("B_spline_base", &argc, argv);

	FILE* f1 = fopen("testData.txt", "r");

	float values[NCOMPS][NPOINTS];

	unsigned int i=0;
	unsigned int j=0;
	char buffer[BUFSIZE];
	for (unsigned int iLine=0; iLine<NPOINTS*NCOMPS; iLine++) {
		fgets(buffer, BUFSIZE, f1);
		values[i][j] = atof(buffer);
		fprintf(stderr, "%d, %d: %f\n", i, j, values[i][j]);
		if (i++ == NCOMPS-1) { i=0; j++; }
	}

	TMultiGraph* mg = new TMultiGraph();

	TGraph* gr[NCOMPS];
	for (unsigned int iComp=0; iComp<NCOMPS; iComp++) {
		gr[iComp] = new TGraph(NPOINTS);
		gr[iComp]->SetMarkerStyle(21);
		gr[iComp]->SetMarkerColor(iComp+1);
		mg->Add(gr[iComp]);
	}

	for (unsigned int iComp=0; iComp<NCOMPS; iComp++) {
		for (unsigned int iPoint=0; iPoint<NPOINTS; iPoint++) {
			float xVal = XMIN + (XMAX-XMIN)*(float)iPoint/(float)(NPOINTS-1);
			gr[iComp]->SetPoint(iPoint, xVal, values[iComp][iPoint]);
		}
	}

	TCanvas* canv1 = new TCanvas("canv1", "canv1", 1200, 800);
	canv1->cd();
	mg->Draw("APL");

	gPad->SetGrid(1, 1);

	app.Run();
	return 0;
}
