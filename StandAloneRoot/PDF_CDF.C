// ----------------------------------------------------
// This block you need for ACLiC and full compilation.
// It is not necessary if you run this file as a macro from ROOT.

#include <TApplication.h>
#include <TCanvas.h>
#include <TH1D.h>
#include <TF1.h>
#include <TGraph.h>
#include <Math/DistFunc.h>

// ----------------------------------------------------

// This block you need anyway.
void DrawBinomial();
void DrawNormal();
void DrawExp();

// ----------------------------------------------------
// This block is required only for standalone compilation.
#ifndef __CLING__

void PDF_CDF();

int main(int argc, char** argv)
{
	TApplication app("drawStability", &argc, argv);

	PDF_CDF();

	app.Run();
	return 0;
}

#endif // __CLING__
// ----------------------------------------------------

void PDF_CDF()
{
	Double_t xMin = -2.;
	Double_t xMax = 2.;
	UInt_t nPoints = 1000;

	//TH1D* histoPDF = new TH1D("histoPDF", "histoPDF", 10000, xMin, xMax);
	//TH1D* histoCDF = new TH1D("histoCDF", "histoCDF", 10000, xMin, xMax);

	Double_t grX1[nPoints+1];
	Double_t grX2[nPoints+1];
	Double_t grY1[nPoints+1];
	Double_t grY2[nPoints+1];

	for (UInt_t i=0; i<=nPoints; i++) {

		Double_t x = xMin + (xMax-xMin) * (Double_t)i / (Double_t)nPoints;
		//cout << i << "\t" << x << endl;
		Double_t res = 0.;

		res = ROOT::Math::normal_pdf(x);
		grX1[i] = x;
		grY1[i] = res;
		//histoPDF->Fill(res);

		res = ROOT::Math::normal_cdf(x);
		grX2[i] = x;
		grY2[i] = res;
		//histoCDF->Fill(res);

	}


	// ------------------------------------------
/*
	gStyle->SetOptStat(111111);

	TCanvas* canv = new TCanvas("canv", "canv");
	canv->Divide(2);

	canv->cd(1);
	histoPDF->Draw();
	gPad->SetGrid(1, 1);
	//gPad->SetLogy();

	canv->cd(2);
	histoCDF->Draw();
	gPad->SetGrid(1, 1);
	//gPad->SetLogy();
*/
	// ------------------------------------------

	TGraph* grPDF = new TGraph(nPoints, grX1, grY1);
	TGraph* grCDF = new TGraph(nPoints, grX2, grY2);

	TCanvas* canvGraph = new TCanvas("canvGraph", "canvGraph", 1200, 800);
	canvGraph->Divide(2);

	canvGraph->cd(1);
	grPDF->Draw("AL");
	gPad->SetGrid(1, 1);

	canvGraph->cd(2);
	grCDF->Draw("AL");
	gPad->SetGrid(1, 1);

	// ------------------------------------------

	DrawNormal();
	DrawExp();
	DrawBinomial();
}

void DrawNormal()
{
	Double_t xMin = -2.;
	Double_t xMax = 2.;

	TF1* funcPDF = new TF1("funcPDF", "ROOT::Math::normal_pdf(x)", xMin, xMax);
	TF1* funcCDF = new TF1("funcCDF", "ROOT::Math::normal_cdf(x)", xMin, xMax);

	TCanvas* canvNormal = new TCanvas("canvNormal", "canvNormal", 1200, 800);
	canvNormal->Divide(2);

	canvNormal->cd(1);
	funcPDF->Draw();
	gPad->SetGrid(1, 1);

	canvNormal->cd(2);
	funcCDF->Draw();
	gPad->SetGrid(1, 1);
}

void DrawExp()
{
	Double_t xMin = 0.;
	Double_t xMax = 10.;

	TF1* funcPDF = new TF1("funcPDF", "ROOT::Math::exponential_pdf(x, 1.)", xMin, xMax);
	TF1* funcCDF = new TF1("funcCDF", "ROOT::Math::exponential_cdf(x, 1.)", xMin, xMax);

	TCanvas* canvExp = new TCanvas("canvExp", "canvExp", 1200, 800);
	canvExp->Divide(2);

	canvExp->cd(1);
	funcPDF->Draw();
	gPad->SetGrid(1, 1);

	canvExp->cd(2);
	funcCDF->Draw();
	gPad->SetGrid(1, 1);
}

void DrawBinomial()
{
	Double_t xMin = -2.;
	Double_t xMax = 40.;

	TF1* funcPDF = new TF1("funcPDF", "ROOT::Math::binomial_pdf(x, 0.5, 20)", xMin, xMax);
	TF1* funcCDF = new TF1("funcCDF", "ROOT::Math::binomial_cdf(x, 0.5, 20)", xMin, xMax);

	TCanvas* canvBinomial = new TCanvas("canvBinomial", "canvBinomial", 1200, 800);
	canvBinomial->Divide(2);

	canvBinomial->cd(1);
	funcPDF->Draw();
	gPad->SetGrid(1, 1);

	canvBinomial->cd(2);
	funcCDF->Draw();
	gPad->SetGrid(1, 1);
}
