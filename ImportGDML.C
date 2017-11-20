void ImportGDML(TString filename="trapGeom.gdml")
{
	TGeoManager* geoM = new TGeoManager("geoM", "geoM");
	geoM->Import(filename);
}
