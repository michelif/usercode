#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"
#include "cl95cms.C"


void printYields( DrawBase* db, const std::string& suffix, bool doUL=false );


int main(int argc, char* argv[]) {

  if(  argc != 2 && argc != 3 ) {
    std::cout << "USAGE: ./drawttHgg [(string)selType] [bTaggerType=\"JP\"]" << std::endl;
    exit(23);
  }


  std::string selType(argv[1]);


  std::string bTaggerType = "JP";
  if( argc>=3 ) {
    std::string bTaggerType_str(argv[2]);
    bTaggerType = bTaggerType_str;
  }


  std::string dataDataset = "DATA_Run2012_FULL";

  DrawBase* db_nostack = new DrawBase("ttHgg_nostack");
  DrawBase* db_stack = new DrawBase("ttHgg_stack");

  db_nostack->set_lumiOnRightSide();
  db_stack->set_lumiOnRightSide();

  db_nostack->set_shapeNormalization();


  db_stack->set_noStack(false);

  std::string bkg_string="";
  std::string controlSample_string="";
  std::string outputdir_str = "ttHggPlots_" +dataDataset+"_"+ selType + "_" +bkg_string+ "_"+controlSample_string+"_"+ bTaggerType;
  db_nostack->set_outputdir(outputdir_str);
  db_stack->set_outputdir(outputdir_str);

  int signalFillColor = 46;

  bool doUL= true;
  bool doPlot=false;

  //signal
  std::string ttHFileName = "ttHgg_TTH_HToGG_M-125_8TeV-pythia6_Summer12-PU_S7_START52_V9-v2";
  ttHFileName += "_" + selType;
  ttHFileName += "_" + bTaggerType;
  ttHFileName += ".root";
  TFile* ttHFile;
  if(!doPlot) {
    ttHFile = TFile::Open(ttHFileName.c_str());
    db_stack->set_lumiNormalization(9300.);
  }else {
    ttHFile = TFile::Open("ttH_rescaled.root");
    db_stack->set_lumiNormalization(1.);
  }
  //  db_nostack->add_mcFile( ttHFile, "ttH", "ttH (125)", signalFillColor, 0);
  db_stack->add_mcFile( ttHFile, "ttH", "ttH (125)", signalFillColor, 0);

  //TTFile
  TFile* TTFile = TFile::Open("top_rescaled.root");
  db_stack->add_mcFile( TTFile, "TT", "Top", 44);


  //DATA
  std::string dataFileName = "ttHgg_" + dataDataset;
  dataFileName += "_" + selType;
  dataFileName += "_" + bTaggerType;
  dataFileName += ".root";
  TFile* dataFile = TFile::Open(dataFileName.c_str());
  db_stack->add_dataFile( dataFile, "data" );
  //  db_nostack->add_dataFile( dataFile, "data");

  //Data control sample
  std::string controlSampleFileName = "controlSample.root";

  TFile* controlSampleFile = TFile::Open(controlSampleFileName.c_str());
  db_stack->add_mcFile( controlSampleFile, "controlSample","CS",kRed );
  //  db_nostack->add_mcFile( controlSampleFile, "controlSample","CS",kRed );



  bool log = true;

  
  //stack plots
  db_stack->set_rebin(4);
  

  db_stack->drawHisto("mgg", "DiPhoton Invariant Mass", "GeV");
  printYields( db_stack, "incl", doUL );
  

  return 0;


}




void printYields( DrawBase* db, const std::string& suffix, bool doUL ) {

  std::string yieldsFileName = db->get_outputdir() + "/yields_" + suffix + ".txt";
  ofstream yieldsFile(yieldsFileName.c_str());


  //float xMin = 122.;
  //float xMax = 128.;
  float xMin = 120.;
  float xMax = 130.;

  std::vector<TH1D*> histos = db->get_lastHistos_mc();

  int binXmin = histos[0]->FindBin(xMin);
  int binXmax = histos[0]->FindBin(xMax) -1;
  std::cout <<  binXmin << std::endl;
  std::cout <<  binXmax << std::endl;

  bool foundSignal = false;
  float totalBG = 0.;
  float totalBG_err=0.;
  float totalBG_ave = 0.;
  float signal = 0.;

  yieldsFile << std::endl << "Yields (@ 9.3 fb-1): " << std::endl;
  for( unsigned int ii=0; ii<histos.size(); ++ii ) {
    yieldsFile << db->get_mcFile(ii).datasetName << " " << histos[ii]->Integral(binXmin, binXmax) << std::endl;
    if( db->get_mcFile(ii).datasetName != "ttH" ) {
      totalBG += histos[ii]->Integral(binXmin, binXmax);
      totalBG_ave += histos[ii]->Integral(1, histos[ii]->GetNbinsX());
    } else {
      foundSignal = true;
      signal = histos[ii]->Integral(binXmin, binXmax);
    }
  }

  totalBG=totalBG/db->get_lumi();
  totalBG_ave *= (10.)/(histos[0]->GetXaxis()->GetXmax()-histos[0]->GetXaxis()->GetXmin());
  totalBG_ave=totalBG_ave/db->get_lumi();



  //  float signal_xsec = 2.28E-03*(19.37 + 1.573 + 0.6966 + 0.3943 + 0.1302); 
  float signal_xsec = (0.1302)*2.28E-03;
  float total_signal = signal_xsec*db->get_lumi();
  float effS = signal/total_signal;
  yieldsFile << "Signal efficiency: " << effS << std::endl;
  totalBG_err=sqrt(totalBG/3.2); 

  yieldsFile << "Total BG: " << totalBG << "+-"<<totalBG_err<<" (averaged: " << totalBG_ave << ")" << std::endl;

  if( !foundSignal ) std::cout << "WARNING!!! DIDN'T FIND SIGNAL HToGG!" << std::endl; 

  
  if( doUL && foundSignal ) {

    //    float ul = CLA( db->get_lumi(), 0., effS, 0., totalBG, 0. );
    float ul = CL95( db->get_lumi(), 0., effS, 0., totalBG, totalBG_err,signal );
    float ul_bgave = CL95( db->get_lumi(), 0., effS, 0., totalBG_ave, 0.,signal );
    if (totalBG_err==0)    yieldsFile << std::endl << "No error on BG:" << std::endl;
    yieldsFile << "UL: " << ul << "    (average BG): " << ul_bgave << std::endl;
    yieldsFile << "UL/SM: " << ul/signal_xsec << "    (average BG): " << ul_bgave/signal_xsec << std::endl;
    /*    float ul_bgerr = CLA( db->get_lumi(), 0., effS, 0., totalBG, 0.05*totalBG );
    yieldsFile << std::endl << "5\% error on BG:" << std::endl;
    yieldsFile << "UL: " << ul_bgerr << std::endl;
    yieldsFile << "UL/SM: " << ul_bgerr/signal_xsec << std::endl;
    */
  }



}

