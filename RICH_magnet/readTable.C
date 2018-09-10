/**
	Compile an animated gif from a set of still png images:
	convert -delay 50 -loop 0 *.png B.gif
*/

#include "cppTokenizer.h"

// Number of the first line with data, lines numbering start from 1
#define DATASTART 9

// Number of components in one line
// x, y, z, Bx, By, Bz
#define NCOMP 6

#define SAVEPNGS

void readTable(TString filename="CBMNewServ1Slice2m9_18Grid.data.table", TString outputDir="figures/")
{
	std::ifstream ifs;
	ifs.open(filename.Data(), std::ifstream::in);
	std::string line;

	std::vector<std::string> tokens;

	unsigned int Nx=0, Ny=0, Nz=0;

	double*  x = NULL;
	double*  y = NULL;
	double*  z = NULL;
	double* Bx = NULL;
	double* By = NULL;
	double* Bz = NULL;

	double Xmin=0.;    //TODO deduce from the input data
	double Xmax=3000.; //TODO deduce from the input data
	double Ymin=0.;    //TODO deduce from the input data
	double Ymax=3000.; //TODO deduce from the input data

	// Read the map ===========================================================

	cout << "Reading the map..." << endl;

	unsigned int lineCounter=0;
	while (getline(ifs, line))
	{
		if (lineCounter >= DATASTART-1) {
			//cout << "||" << line << "||" << endl;

			if (Nx == 0 || Ny == 0 || Nz == 0) {
				cerr << "Nx=0 or Ny=0 or Nz=0. Aborting." << endl;
				return;
			}

			tokens = tokenize(line, " ");
			//cout << tokens.size() << endl;
			if (tokens.size() != NCOMP) {
				cerr << "Line " << lineCounter << " contains other than " << NCOMP << " values. Aborting." << endl;
				return;
			}
			//cout << endl;

			//cerr << lineCounter-(DATASTART-1) << endl;
			 x[lineCounter-(DATASTART-1)] = std::stod(tokens.at(0));
			 y[lineCounter-(DATASTART-1)] = std::stod(tokens.at(1));
			 z[lineCounter-(DATASTART-1)] = std::stod(tokens.at(2));
			Bx[lineCounter-(DATASTART-1)] = std::stod(tokens.at(3));
			By[lineCounter-(DATASTART-1)] = std::stod(tokens.at(4));
			Bz[lineCounter-(DATASTART-1)] = std::stod(tokens.at(5));

		} else {
			//cout << "++" << line << "++"  << endl;
			if (lineCounter == 0) {
				tokens = tokenize(line, " ");

				// Yes, reverse order...
				Nz = std::stoi(tokens.at(0));
				Ny = std::stoi(tokens.at(1));
				Nx = std::stoi(tokens.at(2));

				// Allocate memory
				 x = new double[Nx*Ny*Nz];
				 y = new double[Nx*Ny*Nz];
				 z = new double[Nx*Ny*Nz];
				Bx = new double[Nx*Ny*Nz];
				By = new double[Nx*Ny*Nz];
				Bz = new double[Nx*Ny*Nz];
			}
		}

		lineCounter++;
	} // loop over the lines of the input text file

	// Short summary ==========================================================

	cout << "Nx=" << Nx << ",\t" << "Ny=" << Ny << ",\t" << "Nz=" << Nz << endl;
	cout << "Expected   " << Nx*Ny*Nz << " entries." << endl;
	cout << "Discovered " << lineCounter-(DATASTART-1) << " entries." << endl;

	// Draw whatever you want =================================================

	cout << "Producing output figures..." << endl;

	TCanvas* canvBx = new TCanvas("canvBx", "canvBx", 1000, 1000);
	TCanvas* canvBy = new TCanvas("canvBy", "canvBy", 1000, 1000);
	TCanvas* canvBz = new TCanvas("canvBz", "canvBz", 1000, 1000);

	for (unsigned int iz=0; iz<251; iz+=5)
	{
		unsigned int index = Ny*Nz + Nz + iz;

		TString histoNameBx, histoTitleBx, fileNameBx;
		TString histoNameBy, histoTitleBy, fileNameBy;
		TString histoNameBz, histoTitleBz, fileNameBz;

		histoNameBx.Form("Bx_secZ=%011.6f", z[index]); //FIXME
		histoTitleBx.Form("Bx value in the XY plane at Z=%0.6f", z[index]); //FIXME
		fileNameBx = histoNameBx + ".png";
		TH2D* Bx_secZ = new TH2D(histoNameBx, histoTitleBx, Nx, Xmin, Xmax, Ny, Ymin, Ymax);

		histoNameBy.Form("By_secZ=%011.6f", z[index]); //FIXME
		histoTitleBy.Form("By value in the XY plane at Z=%0.6f", z[index]); //FIXME
		fileNameBy = histoNameBy + ".png";
		TH2D* By_secZ = new TH2D(histoNameBy, histoTitleBy, Nx, Xmin, Xmax, Ny, Ymin, Ymax);

		histoNameBz.Form("Bz_secZ=%011.6f", z[index]); //FIXME
		histoTitleBz.Form("Bz value in the XY plane at Z=%0.6f", z[index]); //FIXME
		fileNameBz = histoNameBz + ".png";
		TH2D* Bz_secZ = new TH2D(histoNameBz, histoTitleBz, Nx, Xmin, Xmax, Ny, Ymin, Ymax);

		for (unsigned int ix=0; ix<Nx; ix++) {
			for (unsigned int iy=0; iy<Ny; iy++) {
				index = ix*Ny*Nz + iy*Nz + iz;

				////cerr << index << "\t" << x[index] << "\t" << y[index] << "\t" << z[index] << endl;

				Bx_secZ->SetBinContent(ix+1, iy+1, Bx[index]);
				By_secZ->SetBinContent(ix+1, iy+1, By[index]);
				Bz_secZ->SetBinContent(ix+1, iy+1, Bz[index]);
			}
		}

		canvBx->cd();
		Bx_secZ->GetZaxis()->SetRangeUser(-0.002, 0.002);
		Bx_secZ->GetXaxis()->SetLabelSize(0.02);
		Bx_secZ->GetYaxis()->SetLabelSize(0.02);
		Bx_secZ->GetZaxis()->SetLabelSize(0.02);
		Bx_secZ->Draw("COLZ");
		Bx_secZ->SetStats(kFALSE);
		#ifdef SAVEPNGS
		canvBx->SaveAs(outputDir + "Bx/" + fileNameBx);
		#endif

		canvBy->cd();
		By_secZ->GetZaxis()->SetRangeUser(-0.002, 0.002);
		By_secZ->GetXaxis()->SetLabelSize(0.02);
		By_secZ->GetYaxis()->SetLabelSize(0.02);
		By_secZ->GetZaxis()->SetLabelSize(0.02);
		By_secZ->Draw("COLZ");
		By_secZ->SetStats(kFALSE);
		#ifdef SAVEPNGS
		canvBy->SaveAs(outputDir + "By/" + fileNameBy);
		#endif

		canvBz->cd();
		Bz_secZ->GetZaxis()->SetRangeUser(-0.002, 0.002);
		Bz_secZ->GetXaxis()->SetLabelSize(0.02);
		Bz_secZ->GetYaxis()->SetLabelSize(0.02);
		Bz_secZ->GetZaxis()->SetLabelSize(0.02);
		Bz_secZ->Draw("COLZ");
		Bz_secZ->SetStats(kFALSE);
		#ifdef SAVEPNGS
		canvBz->SaveAs(outputDir + "Bz/" + fileNameBz);
		#endif
	}

	// ========================================================================

	// Free memory
	if  (x) delete [] x;
	if  (z) delete [] z;
	if  (y) delete [] y;
	if (Bx) delete [] Bx;
	if (Bz) delete [] Bz;
	if (By) delete [] By;

	cout << "Done." << endl;
}
