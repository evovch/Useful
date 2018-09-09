#include <iostream>

// ROOT
#include <TApplication.h>
#include <TF1.h>
#include <TCanvas.h>

// Project
#include "cls_quadratic_function.hpp"
#include "cls_quartic_function.hpp"

int main(int argc, char** argv)
{
	TApplication app("PolynomialFunctionsTest", &argc, argv);

	double a =  0.1;
	double b = -2.0;
	double c =  3.0;
	double d =  4.0;
	double e =100.0;

	// QUARTIC ================================================================
/*
	TF1* func4Root = new TF1("func4Root", "[0]*x*x*x*x + [1]*x*x*x + [2]*x*x + [3]*x + [4]", -10., 20.);
	func4Root->SetParNames("a","b","c","d","e");
	func4Root->SetParameters(a,b,c,d,e);

	TCanvas* canv4 = new TCanvas("canv4", "canv4", 800, 600);
	canv4->cd();
	func4Root->Draw();
	gPad->SetGrid(1,1);

	cls_quartic_function<double> func4(a,b,c,d,e);
	double v_roots4[4];
	unsigned short nRoots = func4.Solve(v_roots4);
	std::cout << nRoots << " ROOTS:" << std::endl;
	std::cout << "\t" << v_roots4[0] << ", " << func4.Evaluate(v_roots4[0]) << std::endl;
	std::cout << "\t" << v_roots4[1] << ", " << func4.Evaluate(v_roots4[1]) << std::endl;
	std::cout << "\t" << v_roots4[2] << ", " << func4.Evaluate(v_roots4[2]) << std::endl;
	std::cout << "\t" << v_roots4[3] << ", " << func4.Evaluate(v_roots4[3]) << std::endl;
*/

	// QUADRATIC ==============================================================
	TF1* func2Root = new TF1("func2Root", "[0]*x*x + [1]*x + [2]", -10., 20.);
	func2Root->SetParNames("a","b","c");
	func2Root->SetParameters(a,b,c);

	TCanvas* canv2 = new TCanvas("canv2", "canv2", 800, 600);
	canv2->cd();
	func2Root->Draw();
	gPad->SetGrid(1,1);

	cls_quadratic_function<double> func2(a,b,c);
	double v_roots2[2];
	unsigned short nRoots = func2.Solve(v_roots2);
	std::cout << nRoots << " ROOTS:" << std::endl;
	std::cout << "\t" << v_roots2[0] << ", " << func2.Evaluate(v_roots2[0]) << std::endl;
	std::cout << "\t" << v_roots2[1] << ", " << func2.Evaluate(v_roots2[1]) << std::endl;

	app.Run();

	return 0;
}