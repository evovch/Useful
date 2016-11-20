// Use input ROOT file from the Analysis5/sums/ directory

void draw_laser()
{
   TFile* f1 = new TFile("/home/evovch/Documents/Analysis_Sep2016/Analysis5/sums/analysis_sum_laser_calib_corr.root");

   TH1D* h1 = NULL; h1 = (TH1D*)f1->Get("LeadingEdgeDiff_ID1_00/fhLeadingEdgeDiff_ID1_00_ID2_08");
   if (h1 == NULL) {
      printf ("Histogram 1 not found.\n");
      return;
   }

   TH1D* h2 = NULL; h2 = (TH1D*)f1->Get("fhLeadingEdgeDiffOnePADIWAGood_3");
   if (h2 == NULL) {
      printf ("Histogram 2 not found.\n");
      return;
   }

   TH1D* h3 = NULL; h3 = (TH1D*)f1->Get("fhLeadingEdgeDiffOnePMTGood_1");
   if (h3 == NULL) {
      printf ("Histogram 3 not found.\n");
      return;
   }

   TH1D* h4 = NULL; h4 = (TH1D*)f1->Get("fhLeadingEdgeDiffTopRightQuarterGood");
   if (h4 == NULL) {
      printf ("Histogram 4 not found.\n");
      return;
   }

   TCanvas *c1 = new TCanvas("c1","Time precision",10,10,1200,800);

   gStyle->SetOptStat(111111);   //TODO switch on or off
   gPad->SetGrid(1, 1);
   //gPad->SetLogy();

   h4->SetLineColor(kBlack);
   h4->SetLineWidth(2);
   h4->Draw("");
   h4->SetTitle("Time precision");
   h4->SetName("Top-right quarter");
   h4->GetXaxis()->SetRangeUser(-5., 5.);
   h4->GetXaxis()->SetTitle("ns");
   h4->GetYaxis()->SetTitle("Entries");
   gPad->Update();
   TPaveStats *ps4 = (TPaveStats*)h4->GetListOfFunctions()->FindObject("stats");
   ps4->SetX1NDC(0.80); ps4->SetX2NDC(0.95);
   ps4->SetTextColor(kBlack);
   gPad->Modified();

   h3->Scale(13.5);

   h3->SetLineColor(kGreen);
   h3->SetLineWidth(2);
   h3->Draw("][sames");
   h3->SetTitle("Time precision");
   h3->SetName("One PMT");
   h3->GetXaxis()->SetRangeUser(-5., 5.);
   h3->GetXaxis()->SetTitle("ns");
   h3->GetYaxis()->SetTitle("Entries");
   gPad->Update();
   TPaveStats *ps3 = (TPaveStats*)h3->GetListOfFunctions()->FindObject("stats");
   ps3->SetX1NDC(0.65); ps3->SetX2NDC(0.80);
   ps3->SetTextColor(kGreen);
   gPad->Modified();

   h2->Scale(530.);

   h2->SetLineColor(kRed);
   h2->SetLineWidth(2);
   h2->Draw("][sames");
   h2->SetTitle("Time precision");
   h2->SetName("One PADIWA");
   h2->GetXaxis()->SetRangeUser(-5., 5.);
   h2->GetXaxis()->SetTitle("ns");
   h2->GetYaxis()->SetTitle("Entries");
   gPad->Update();
   TPaveStats *ps2 = (TPaveStats*)h2->GetListOfFunctions()->FindObject("stats");
   ps2->SetX1NDC(0.30); ps2->SetX2NDC(0.45);
   ps2->SetTextColor(kRed);
   gPad->Modified();

   h1->Scale(10385.);

   h1->SetLineColor(kBlue);
   h1->SetLineWidth(2);
   h1->Draw("][sames");
   h1->SetTitle("Time precision");
   h1->SetName("One pair of channels");
   h1->GetXaxis()->SetRangeUser(-5., 5.);
   h1->GetXaxis()->SetTitle("ns");
   h1->GetYaxis()->SetTitle("Entries");
   gPad->Update();
   TPaveStats *ps1 = (TPaveStats*)h1->GetListOfFunctions()->FindObject("stats");
   ps1->SetX1NDC(0.15); ps1->SetX2NDC(0.30);
   ps1->SetTextColor(kBlue);
   gPad->Modified();

   //c1->SaveAs("WLS_time_profile_1.eps");
}
