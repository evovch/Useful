#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>

#include "classes.h"

#include <iostream>
using std::cout;
using std::endl;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::StartProcessing(void)
{
    TDirectory* oldGdir = gDirectory;

    TFile theTreeFile("outputRootTreeFile.root", "RECREATE");
    TTree theTree("theTree", "The ROOT tree of events");

    cls_event* theEvent = new cls_event();

    theTree.Branch("theBranch", "cls_event", theEvent);

    UInt_t nEvents = 1000;
    Double_t meanNhitsPerEvent = 20.;

    TRandom3 rndGen;

    //Specific for GUI application example
    std::vector<Double_t> TsValues;
    std::vector<Double_t> ChValues;
    std::vector<Double_t> AdcValues;

    for (UInt_t i=0; i<nEvents; i++) {

        Double_t nHits = rndGen.Poisson(meanNhitsPerEvent);

        Double_t eventTimestamp = rndGen.Gaus(0., 10.);

        //cout << "Event " << i << "\t\tTs: " << (i+1)*1000.+eventTimestamp << "\tnHits: " << (UInt_t)nHits << endl;

        Double_t tsInPs = (Double_t)(i+1) * 1000. + eventTimestamp;
        theEvent->SetTimestamp(tsInPs);

        for (UInt_t j=0; j<(UInt_t)nHits; j++)
        {
            Double_t hitTs = rndGen.Exp(5000.);
            UInt_t hitCh = rndGen.Integer(128);
            UInt_t hitAdc = rndGen.Integer(1024);

            //Specific for GUI application example
            TsValues.push_back(hitTs);
            ChValues.push_back(hitCh);
            AdcValues.push_back(hitAdc);

            theEvent->AddHit((UInt_t)hitTs, hitCh, hitAdc);

            //cout << "\t\tHit " << j << "\tTs: " << hitTs/1000. << "\tCh: " << hitCh << "\tAdc: " << hitAdc << endl;
        }

        theEvent->NextEvent();
        theTree.Fill();
        theEvent->Clear();

    }

    delete theEvent;

    theTreeFile.Write();
    //theTree.Print();

    gDirectory = oldGdir;

    //Specific for GUI application example
    //cls_HistoWidget* theHistoWidget = new cls_HistoWidget();
    //theHistoWidget->FillFromData(AdcValues);
    //theHistoWidget->show();
    ui->histoWidget1->FillFromData(AdcValues);
}
