#include <utility>
#include <vector>
#include <algorithm>

class my_point
{
public:
	my_point(UInt_t p_i, Double_t p_x, Double_t p_y) : index(p_i), xcoord(p_x), ycoord(p_y) {}
	~my_point() {}
	friend bool operator< (const my_point& lhs, const my_point& rhs) { return lhs.xcoord < rhs.xcoord; }

public:
	UInt_t index;
	Double_t xcoord;
	Double_t ycoord;

};

void ConstructGraph(TGraph* inGraph);

void memoryManagement()
{
	gStyle->SetMarkerSize(1);
	gStyle->SetMarkerStyle(20);
	gStyle->SetMarkerColor(kRed);

	TCanvas* canvFunc = new TCanvas("canvFunc", "canvFunc", 100, 100, 1000, 600);

	TF1* func = new TF1("func", "sin(10.0*x)", 0., 1.);
	canvFunc->cd();
	func->Draw();
	func->SetLineColor(kBlack);
	gPad->SetGrid(1,1);

	TRandom3* rndGen = new TRandom3();

	TGraph* graphArray[10];
	TCanvas* canvasArray[10];
	std::vector<my_point> data[10];

	for (UInt_t iGraph=0; iGraph<10; iGraph++) {

		graphArray[iGraph] = new TGraph();

		TString canvasName;
		canvasName.Form("canvas_%d", iGraph);
		canvasArray[iGraph] = new TCanvas(canvasName, canvasName);

		for (UInt_t iPoint=0; iPoint<100; iPoint++) {
			Double_t x = rndGen->Rndm();		// from 0.0 to 1.0
			Double_t y = func->Eval(x);
			my_point pointObj(iPoint, x, y);
			data[iGraph].push_back(pointObj);
		} // iPoint

		std::sort(data[iGraph].begin(), data[iGraph].end());
		std::vector<my_point>::iterator iter;
		UInt_t counter=0;
		for (iter = data[iGraph].begin(); iter != data[iGraph].end(); ++iter) {
			graphArray[iGraph]->SetPoint(counter++, iter->xcoord, iter->ycoord);
		}

		graphArray[iGraph]->Draw();

		ConstructGraph(graphArray[iGraph]);
	} // iGraph

	return;
}

void ConstructGraph(TGraph* inGraph)
{

}
