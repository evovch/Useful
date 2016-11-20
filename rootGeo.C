void rootGeo()
{
	TGeoManager* geoM = new TGeoManager("geoM", "geoM");


	// Materials and media --------------------------------------------------------------

	TGeoMaterial* vacMat = new TGeoMaterial("vacMat", 0, 0, 0);
	TGeoMedium* vacMed = new TGeoMedium("vacMed", 1, vacMat);

	TGeoMaterial* ledMat = new TGeoMaterial("ledMat", 0, 0, 0);
	TGeoMedium* ledMed = new TGeoMedium("ledMed", 1, ledMat);

	TGeoMaterial* sciMat = new TGeoMaterial("sciMat", 0, 0, 0);
	TGeoMedium* sciMed = new TGeoMedium("sciMed", 1, sciMat);


	// Shapes and volumes ---------------------------------------------------------------

	Double_t xSize = 10.;
	Double_t ySize = 10.;
	Double_t zSize = 50.;
	UInt_t nSlices = 6;

	Double_t sliceZsize = zSize/(Double_t)nSlices;

	// Half dimensions are given as constructor parameters
	TGeoBBox* worldShape = new TGeoBBox("worldShape", 1000., 1000., 1000.);
	TGeoVolume* worldVol = new TGeoVolume("worldVol", worldShape, vacMed);

	TGeoBBox* moduleShape = new TGeoBBox("moduleShape", xSize/2., ySize/2., zSize/2.);
	TGeoVolume* moduleVol = new TGeoVolume("moduleVol", moduleShape, ledMed);

	TGeoBBox* sliceShape = new TGeoBBox("sliceShape", xSize/2., ySize/2., sliceZsize/2.);
	TGeoVolume* sliceVol = new TGeoVolume("sliceVol", sliceShape, sciMed);

	// Volume hierarchy -----------------------------------------------------------------

	TGeoRotation* rotNoRot = new TGeoRotation("rotNoRot", 0., 0., 0.);
	rotNoRot->RegisterYourself();

	UInt_t nX=20;
	UInt_t nY=5;

	TString transName;
	for (UInt_t i=0; i<nX; i++) {
		for (UInt_t j=0; j<nY; j++) {
			transName.Form("trans_module_%d_%d", i, j);
			TGeoCombiTrans* trans1 = new TGeoCombiTrans(transName.Data(),
				-((Double_t)nX/2.)*xSize + xSize/2. + (Double_t)i*xSize,
				-((Double_t)nY/2.)*ySize + ySize/2. + (Double_t)j*ySize,
				0., rotNoRot);

			trans1->RegisterYourself();
			worldVol->AddNode(moduleVol, i*nY+j, trans1);
		}
	}

	for (UInt_t i=0; i<nSlices; i++) {
		transName.Form("trans_slice_%d", i);
		TGeoCombiTrans* trans2 = new TGeoCombiTrans(transName.Data(), 0., 0., -((Double_t)nSlices/2.)*sliceZsize + sliceZsize/2. + (Double_t)i*sliceZsize, rotNoRot);
		trans2->RegisterYourself();
		moduleVol->AddNode(sliceVol, i, trans2);
	}

	// Finalization ---------------------------------------------------------------------

	geoM->SetTopVolume(worldVol);
	geoM->CloseGeometry();
/*
	geoM->CheckOverlaps();
	geoM->CheckGeometry();
	geoM->CheckGeometryFull();
*/
	TBrowser* bro = new TBrowser("bro", "bro");
}
