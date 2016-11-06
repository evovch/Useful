void root_draw_markers()
{
	TCanvas* canv = new TCanvas("canv", "canv", 500, 500);
	canv->cd();

	gStyle->SetMarkerSize(10);
	gStyle->SetMarkerStyle(20);
	gStyle->SetMarkerColor(kRed);
	TMarker* mark = new TMarker();
	mark->SetX(0.1);
	mark->SetY(0.1);
	mark->Draw();

/*	TArc* circ = new TArc(200./1000., 200./1000., 50./1000.);
	circ->Draw();*/

}
