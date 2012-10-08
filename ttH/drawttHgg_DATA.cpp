#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"
#include "cl95cms.C"

bool separate_mechanism = false;
bool use_all_bkg=false;
bool draw_controlSample=true;

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

  db_stack->set_lumiNormalization(9300.);
  db_stack->set_noStack(false);

  std::string bkg_string="";
  std::string controlSample_string="";
  if(use_all_bkg)bkg_string="all";
  if(draw_controlSample)controlSample_string="wControlSampleNoBTag";//fra
  std::string outputdir_str = "ttHggPlots_" +dataDataset+"_"+ selType + "_" +bkg_string+ "_"+controlSample_string+"_"+ bTaggerType;
  db_nostack->set_outputdir(outputdir_str);
  db_stack->set_outputdir(outputdir_str);

  int signalFillColor = 46;


  //DATA
  std::string dataFileName = "ttHgg_" + dataDataset;
  dataFileName += "_" + selType;
  dataFileName += "_" + bTaggerType;
  dataFileName += ".root";
  TFile* dataFile = TFile::Open(dataFileName.c_str());
  db_stack->add_dataFile( dataFile, "data" );
  db_nostack->add_dataFile( dataFile, "data");

  //Data control sample
  std::string controlSampleFileName = "ttHgg_" + dataDataset+"_"+selType;
  //  std::string controlSampleFileName = "ttHgg_" + dataDataset;
  //controlSampleFileName += "_invertedPhotonID1Phot";
  controlSampleFileName += "_inverted";
  controlSampleFileName += "_" + bTaggerType;
  controlSampleFileName += ".root";
  TFile* controlSampleFile = TFile::Open(controlSampleFileName.c_str());
  if(draw_controlSample){
    db_stack->add_mcFile( controlSampleFile, "controlSample","CS",kRed );
    db_nostack->add_mcFile( controlSampleFile, "controlSample","CS",kRed );
  }

  //signal
  std::string ttHFileName = "ttHgg_TTH_HToGG_M-125_8TeV-pythia6_Summer12-PU_S7_START52_V9-v2";
  ttHFileName += "_" + selType;
  ttHFileName += "_" + bTaggerType;
  ttHFileName += ".root";
  TFile* ttHFile = TFile::Open(ttHFileName.c_str());
  //  db_nostack->add_mcFile( ttHFile, "ttH", "ttH (125)", signalFillColor, 0);
  db_stack->add_mcFile( ttHFile, "ttH", "ttH (125)", signalFillColor, 0);


  if( separate_mechanism ) {
  std::string GluGluHFileName = "ttHgg_GluGluToHToGG_M-125_8TeV-powheg-pythia6";
  GluGluHFileName += "_" + selType;
  GluGluHFileName += "_" + bTaggerType;
  GluGluHFileName += ".root";
  TFile* GluGluHFile = TFile::Open(GluGluHFileName.c_str());
  db_nostack->add_mcFile( GluGluHFile, "GluGluH", "GluGlu H (125)", signalFillColor+1, 0);
  db_stack->add_mcFile( GluGluHFile, "GluGluH", "GluGlu H (125)", signalFillColor+1, 0);
  
  std::string VBFHFileName = "ttHgg_VBF_HToGG_M-125_8TeV-powheg-pythia6";
  VBFHFileName += "_" + selType;
  VBFHFileName += "_" + bTaggerType;
  VBFHFileName += ".root";
  TFile* VBFHFile = TFile::Open(VBFHFileName.c_str());
  db_nostack->add_mcFile( VBFHFile, "VBFH", "VBF H (125)", signalFillColor+2, 0);
  db_stack->add_mcFile( VBFHFile, "VBFH", "VBF H (125)", signalFillColor+2, 0);

  std::string VHFileName = "ttHgg_WH_ZH_HToGG_M-125_8TeV-pythia6";
  VHFileName += "_" + selType;
  VHFileName += "_" + bTaggerType;
  VHFileName += ".root";
  TFile* VHFile = TFile::Open(VHFileName.c_str());
  db_nostack->add_mcFile( VHFile, "VH", "VH (125)", signalFillColor, 0);
  db_stack->add_mcFile( VBFHFile, "VBFH", "VBF H (125)", signalFillColor+2, 0);

  }else{

    // inclusive higgs other mechanism
    std::string HToGGFileName = "ttHgg_HToGG_M-125_8TeV-pythia6";
    HToGGFileName += "_" + selType;
    HToGGFileName += "_" + bTaggerType;
    HToGGFileName += ".root";
    TFile* HToGGFile = TFile::Open(HToGGFileName.c_str());
    db_stack->add_mcFile( HToGGFile, "HToGG", "H (125) (other)", signalFillColor+2, 0);
    //    db_nostack->add_mcFile( HToGGFile, "HToGG", "H (125) (other)", signalFillColor+2, 0);
  }
  std::string DiPhotonFileName = "ttHgg_DiPhoton_8TeV-pythia6";
  DiPhotonFileName += "_" + selType;
  DiPhotonFileName += "_" + bTaggerType;
  DiPhotonFileName += ".root";
  TFile* DiPhotonFile = TFile::Open(DiPhotonFileName.c_str());
  db_nostack->add_mcFile( DiPhotonFile, "DiPhoton", "Diphoton", 29);
  db_stack->add_mcFile( DiPhotonFile, "DiPhoton", "Diphoton", 29);

  std::string GammaJetFileName = "ttHgg_GJet_doubleEMEnriched_TuneZ2star_8TeV-pythia6";
  GammaJetFileName += "_" + selType;
  GammaJetFileName += "_" + bTaggerType;
  GammaJetFileName += ".root";
  TFile* GammaJetFile = TFile::Open(GammaJetFileName.c_str());
  db_nostack->add_mcFile( GammaJetFile, "GammaJet", "#gamma + Jet", 38);//fra
    db_stack->add_mcFile( GammaJetFile, "GammaJet", "#gamma + Jet", 38);


  std::string DiBosonFileName = "ttHgg_VV_8TeV";
  DiBosonFileName += "_" + selType;
  DiBosonFileName += "_" + bTaggerType;
  DiBosonFileName += ".root";
  TFile* DiBosonFile = TFile::Open(DiBosonFileName.c_str());

  std::string TriBosonFileName = "ttHgg_VGG_8TeV";
  TriBosonFileName += "_" + selType;
  TriBosonFileName += "_" + bTaggerType;
  TriBosonFileName += ".root";
  TFile* TriBosonFile = TFile::Open(TriBosonFileName.c_str());

  std::string TTFileName = "ttHgg_TT_8TeV";
  TTFileName += "_" + selType;
  TTFileName += "_" + bTaggerType;
  TTFileName += ".root";
  TFile* TTFile = TFile::Open(TTFileName.c_str());


  std::string QCDFileName = "ttHgg_QCD_doubleEMEnriched_TuneZ2star_8TeV-pythia6";
  QCDFileName += "_" + selType;
  QCDFileName += "_" + bTaggerType;
  QCDFileName += ".root";
  TFile* QCDFile = TFile::Open(QCDFileName.c_str());

  db_stack->add_mcFile( TriBosonFile, "TriBoson", "V+ #gamma#gamma ", 40);
  db_stack->add_mcFile( DiBosonFile, "DiBoson", "Diboson", 39);
  db_stack->add_mcFile( TTFile, "TT", "Top", 44);
  db_stack->add_mcFile( QCDFile, "QCD", "QCD", 41);   

  if(use_all_bkg){
  db_nostack->add_mcFile( DiBosonFile, "DiBoson", "Diboson", 39);
  db_nostack->add_mcFile( TriBosonFile, "TriBoson", "V+ #gamma#gamma ", 40);
  db_nostack->add_mcFile( TTFile, "TT", "Top", 44);

  db_nostack->add_mcFile( QCDFile, "QCD", "QCD", 41); 

  }

  bool log = true;

  
  //stack plots
  //  db_stack->set_rebin(2);

  if(!draw_controlSample){
  db_stack->set_rebin(4);
  db_stack->drawHisto("mgg_prepresel", "DiPhoton Invariant Mass", "GeV");

  //  db_stack->drawHisto("mgg_presel", "DiPhoton Invariant Mass", "GeV");


  //  bool doUL = (selType != "presel" );
  bool doUL= false;


  db_stack->drawHisto("nvertex");
  db_stack->drawHisto("nvertex_PUW");

  db_stack->drawHisto("njets", "Number of Jets", "", "Events","true");
  //  db_stack->set_yAxisMax(800.);
  db_stack->set_xAxisMin(1.5);
  db_stack->set_flags("zoom");
  db_stack->drawHisto("njets", "Number of Jets", "", "Events","true");
  db_stack->reset();
  db_stack->drawHisto("nbjets_loose", "Number of b-Jets (Loose)", "", "Events");
  db_stack->drawHisto("nbjets_medium", "Number of b-Jets (Medium)", "", "Events");


  db_stack->set_rebin(4);
  db_stack->drawHisto("mjj", "Dijet Mass", "GeV");
  db_stack->drawHisto("mjj_0btag", "Dijet Mass", "GeV");
  db_stack->drawHisto("mjj_1btag", "Dijet Mass", "GeV");
  db_stack->drawHisto("mjj_2btag", "Dijet Mass", "GeV");


  db_stack->drawHisto("mgg", "DiPhoton Invariant Mass", "GeV");
  printYields( db_stack, "incl", doUL );

  db_stack->drawHisto("ptphot0", "Lead Photon p_{T}", "GeV");
  db_stack->drawHisto("ptphot1", "Sublead Photon p_{T}", "GeV");
  db_stack->drawHisto("etaphot0", "Lead Photon #eta", "");
  db_stack->drawHisto("etaphot1", "Sublead Photon #eta", "");

  db_stack->drawHisto("ptjet0", "Lead Jet p_{T}", "GeV");
  db_stack->drawHisto("ptjet1", "Sublead Jet p_{T}", "GeV");
  db_stack->drawHisto("ptjet2", "3rd Jet p_{T}", "GeV");
  db_stack->drawHisto("ptjet3", "4th Jet p_{T}", "GeV");
  db_stack->drawHisto("ptjet4", "5th Jet p_{T}", "GeV");
  db_stack->drawHisto("ptjet5", "6th Jet p_{T}", "GeV");
  db_stack->drawHisto("ptjet6", "7th Jet p_{T}", "GeV");
  db_stack->drawHisto("ptjet7", "8th Jet p_{T}", "GeV");
  db_stack->drawHisto("ptjet8", "9th Jet p_{T}", "GeV");
  db_stack->drawHisto("ptjet9", "10th Jet p_{T}", "GeV");

  db_stack->drawHisto("etajet0", "Lead Jet #eta", "");
  db_stack->drawHisto("etajet1", "Sublead Jet #eta", "");
  db_stack->drawHisto("etajet2", "3rd Jet #eta", "");
  db_stack->drawHisto("etajet3", "4th Jet #eta", "");
  db_stack->drawHisto("etajet4", "5th Jet #eta", "");
  db_stack->drawHisto("etajet5", "6th Jet #eta", "");
  db_stack->drawHisto("etajet6", "7th Jet #eta", "");
  db_stack->drawHisto("etajet7", "8th Jet #eta", "");
  db_stack->drawHisto("etajet8", "9th Jet #eta", "");
  db_stack->drawHisto("etajet9", "10th Jet #eta", "");

  db_stack->drawHisto("pfMet","pfMet","GeV");
  db_stack->drawHisto("Ht", "Ht", "GeV");

  db_stack->drawHisto("ptele1", "Lead electron p_{T}", "GeV");
  db_stack->drawHisto("ptele2", "Sublead electron p_{T}", "GeV");

  db_stack->drawHisto("ptmu1", "Lead muon p_{T}", "GeV");
  db_stack->drawHisto("ptmu2", "Sublead muon p_{T}", "GeV");


  db_stack->set_rebin(2);

  db_stack->drawHisto("ptDiphot", "Diphoton p_{T}", "GeV");

  db_stack->drawHisto("deltaPhi", "#Delta#Phi(dijet-diphoton)", "rad");
  db_stack->drawHisto("ptDijet", "Dijet p_{T}", "GeV");
  db_stack->drawHisto("ptRatio", "Dijet p_{T} / Diphoton p_{T}");
  db_stack->drawHisto("ptDifference", "Dijet p_{T} - Diphoton p_{T}", "GeV");

  db_stack->drawHisto("deltaEtaJets", "Jet-Jet #Delta#eta");
  db_stack->drawHisto("deltaFabsEtaJets", "Jet-Jet #Delta|#eta|");

  db_stack->drawHisto("nvertex");
  db_stack->drawHisto("nvertex_PUW");

  db_stack->drawHisto("njets", "Number of Jets", "", "Events","true");
  db_stack->set_yAxisMax(800.);
  db_stack->set_xAxisMin(1.5);
  db_stack->set_flags("zoom");
  db_stack->drawHisto("njets", "Number of Jets", "", "Events","true");
  db_stack->reset();
  db_stack->drawHisto("nbjets_loose", "Number of b-Jets (Loose)", "", "Events");
  db_stack->drawHisto("nbjets_medium", "Number of b-Jets (Medium)", "", "Events");


  db_stack->set_rebin(4);
  db_stack->drawHisto("mjj", "Dijet Mass", "GeV");
  db_stack->drawHisto("mjj_0btag", "Dijet Mass", "GeV");
  db_stack->drawHisto("mjj_1btag", "Dijet Mass", "GeV");
  db_stack->drawHisto("mjj_2btag", "Dijet Mass", "GeV");


}  else{  
  db_nostack->drawHisto("mgg_prepresel", "DiPhoton Invariant Mass", "GeV");
  printYields( db_nostack, "prepresel" );
  //  db_nostack->drawHisto("mgg_presel", "DiPhoton Invariant Mass", "GeV");
  printYields( db_nostack, "presel");

  //  bool doUL = (selType != "presel" );
  bool doUL= false;


  db_nostack->drawHisto("nvertex");
  db_nostack->drawHisto("nvertex_PUW");

  db_nostack->drawHisto("njets", "Number of Jets", "", "Events","true");
  db_nostack->set_yAxisMax(800.);
  db_nostack->set_xAxisMin(1.5);
  db_nostack->set_flags("zoom");
  db_nostack->drawHisto("njets", "Number of Jets", "", "Events","true");
  db_nostack->reset();
  db_nostack->drawHisto("nbjets_loose", "Number of b-Jets (Loose)", "", "Events");
  db_nostack->drawHisto("nbjets_medium", "Number of b-Jets (Medium)", "", "Events");


  db_nostack->set_rebin(4);
  db_nostack->drawHisto("mjj", "Dijet Mass", "GeV");
  db_nostack->drawHisto("mjj_0btag", "Dijet Mass", "GeV");
  db_nostack->drawHisto("mjj_1btag", "Dijet Mass", "GeV");
  db_nostack->drawHisto("mjj_2btag", "Dijet Mass", "GeV");

  db_nostack->set_yAxisMax(150.);
  db_nostack->drawHisto("mgg", "DiPhoton Invariant Mass", "GeV");
  db_nostack->reset();

  db_nostack->drawHisto("ptphot0", "Lead Photon p_{T}", "GeV");
  db_nostack->drawHisto("ptphot1", "Sublead Photon p_{T}", "GeV");
  db_nostack->drawHisto("etaphot0", "Lead Photon #eta", "");
  db_nostack->drawHisto("etaphot1", "Sublead Photon #eta", "");

  db_nostack->drawHisto("ptjet0", "Lead Jet p_{T}", "GeV");
  db_nostack->drawHisto("ptjet1", "Sublead Jet p_{T}", "GeV");
  db_nostack->drawHisto("ptjet2", "3rd Jet p_{T}", "GeV");
  db_nostack->drawHisto("ptjet3", "4th Jet p_{T}", "GeV");
  db_nostack->drawHisto("ptjet4", "5th Jet p_{T}", "GeV");
  db_nostack->drawHisto("ptjet5", "6th Jet p_{T}", "GeV");
  db_nostack->drawHisto("ptjet6", "7th Jet p_{T}", "GeV");
  db_nostack->drawHisto("ptjet7", "8th Jet p_{T}", "GeV");
  db_nostack->drawHisto("ptjet8", "9th Jet p_{T}", "GeV");
  db_nostack->drawHisto("ptjet9", "10th Jet p_{T}", "GeV");

  db_nostack->drawHisto("etajet0", "Lead Jet #eta", "");
  db_nostack->drawHisto("etajet1", "Sublead Jet #eta", "");
  db_nostack->drawHisto("etajet2", "3rd Jet #eta", "");
  db_nostack->drawHisto("etajet3", "4th Jet #eta", "");
  db_nostack->drawHisto("etajet4", "5th Jet #eta", "");
  db_nostack->drawHisto("etajet5", "6th Jet #eta", "");
  db_nostack->drawHisto("etajet6", "7th Jet #eta", "");
  db_nostack->drawHisto("etajet7", "8th Jet #eta", "");
  db_nostack->drawHisto("etajet8", "9th Jet #eta", "");
  db_nostack->drawHisto("etajet9", "10th Jet #eta", "");

  db_nostack->drawHisto("pfMet","pfMet","GeV");
  db_nostack->drawHisto("Ht", "Ht", "GeV");

  db_nostack->drawHisto("ptele1", "Lead electron p_{T}", "GeV");
  db_nostack->drawHisto("ptele2", "Sublead electron p_{T}", "GeV");

  db_nostack->drawHisto("ptmu1", "Lead muon p_{T}", "GeV");
  db_nostack->drawHisto("ptmu2", "Sublead muon p_{T}", "GeV");


  db_nostack->set_rebin(2);

  db_nostack->drawHisto("ptDiphot", "Diphoton p_{T}", "GeV");

  db_nostack->drawHisto("deltaPhi", "#Delta#Phi(dijet-diphoton)", "rad");
  db_nostack->drawHisto("ptDijet", "Dijet p_{T}", "GeV");
  db_nostack->drawHisto("ptRatio", "Dijet p_{T} / Diphoton p_{T}");
  db_nostack->drawHisto("ptDifference", "Dijet p_{T} - Diphoton p_{T}", "GeV");

  db_nostack->drawHisto("deltaEtaJets", "Jet-Jet #Delta#eta");
  db_nostack->drawHisto("deltaFabsEtaJets", "Jet-Jet #Delta|#eta|");
}  

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
  float totalBG_ave = 0.;
  float signal = 0.;

  yieldsFile << std::endl << "Yields (@ 30 fb-1): " << std::endl;
  for( unsigned int ii=0; ii<histos.size(); ++ii ) {
    yieldsFile << db->get_mcFile(ii).datasetName << " " << histos[ii]->Integral(binXmin, binXmax) << std::endl;
    if( db->get_mcFile(ii).datasetName != "HToGG" ) {
      totalBG += histos[ii]->Integral(binXmin, binXmax);
      totalBG_ave += histos[ii]->Integral(1, histos[ii]->GetNbinsX());
    } else {
      foundSignal = true;
      signal = histos[ii]->Integral(binXmin, binXmax);
    }
  }

  totalBG_ave *= (10.)/(histos[0]->GetXaxis()->GetXmax()-histos[0]->GetXaxis()->GetXmin());

  yieldsFile << "Total BG: " << totalBG << " (averaged: " << totalBG_ave << ")" << std::endl;

  float signal_xsec = 2.28E-03*(19.37 + 1.573 + 0.6966 + 0.3943 + 0.1302); 
  float total_signal = signal_xsec*db->get_lumi();
  float effS = signal/total_signal;
  yieldsFile << "Signal efficiency: " << effS << std::endl;

  if( !foundSignal ) std::cout << "WARNING!!! DIDN'T FIND SIGNAL HToGG!" << std::endl; 

  
  if( doUL && foundSignal ) {

    float ul = CLA( db->get_lumi(), 0., effS, 0., totalBG, 0. );
    float ul_bgave = CLA( db->get_lumi(), 0., effS, 0., totalBG_ave, 0. );
    yieldsFile << std::endl << "No error on BG:" << std::endl;
    yieldsFile << "UL: " << ul << "    (average BG): " << ul_bgave << std::endl;
    yieldsFile << "UL/SM: " << ul/signal_xsec << "    (average BG): " << ul_bgave/signal_xsec << std::endl;
    float ul_bgerr = CLA( db->get_lumi(), 0., effS, 0., totalBG, 0.05*totalBG );
    yieldsFile << std::endl << "5\% error on BG:" << std::endl;
    yieldsFile << "UL: " << ul_bgerr << std::endl;
    yieldsFile << "UL/SM: " << ul_bgerr/signal_xsec << std::endl;

  }



}

