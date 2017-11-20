void trapLoop()
{
	TGeoManager* geoM = new TGeoManager("geoM", "geoM");

	// Materials
	TGeoMaterial* vacMat = new TGeoMaterial("vacMat", 0, 0, 0);
	TGeoMedium* vacMed = new TGeoMedium("vacMed", 1, vacMat);

	TGeoMaterial* ledMat = new TGeoMaterial("ledMat", 0, 0, 0);
	TGeoMedium* ledMed = new TGeoMedium("ledMed", 1, ledMat);

	// Top volume
	TGeoBBox* worldShape = new TGeoBBox("worldShape", 1000., 1000., 1000.);
	TGeoVolume* worldVol = new TGeoVolume("worldVol", worldShape, vacMed);

	// Trap volume
	TGeoTrap* trapShape = new TGeoTrap("trapShape", 20., 0., 0., 30., 70., 70., 0., 30., 40., 40., 0.);
	TGeoVolume* trapVolume = new TGeoVolume("trapVolume", trapShape, ledMed);


	TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
	rotNoRot->RegisterYourself();

	TGeoCombiTrans* combitrans = new TGeoCombiTrans("combitrans", 0., 0., 0., rotNoRot);
	combitrans->RegisterYourself();

	worldVol->AddNode(trapVolume, 1, combitrans);

	geoM->SetTopVolume(worldVol);
	geoM->CloseGeometry();

	TCanvas* canv = new TCanvas("canv", "canv");

	worldVol->Draw("ogl");

	Int_t counter = 0;
	TString outputFilename;
	outputFilename.Form("%s_%d.png", "trap", counter);
	canv->SaveAs(outputFilename);
}
