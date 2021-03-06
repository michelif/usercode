//================================================================================================
//			trainElectronEnergyRegression.C
//
// Given an appropriate electron ntuple, uses the classes GBRTrees etc. in order to train a regression
// to recover the generated electron energy
//
// USAGE
// trainElectronEnergyRegression(char* trainingFile, char* outWeightFile, char* optionChar)
//
// trainingFile		= ntuple file on which to perform the training
// outWeightFile	= output file, to which it will save the weights in a .root file
// optionChar		= denotes which version of the training is being performed
//
// V00	no pT split	no tracker variables
// V01	no pT split	includes tracker variables
// V10	pT split	no tracker variables
// V11	pT split	includes tracker variables
//________________________________________________________________________________________________

#include "TFile.h"
#include "TTree.h"
#include "GBRTrainer.h"
#include "GBRForest.h"
#include "GBRApply.h"
#include "TCut.h"
#include "TH2D.h"
#include "TSystem.h"
#include "TChain.h"
#include "TObjArray.h"
#include "TTreeFormula.h"
#include "TCanvas.h"
#include "Cintex/Cintex.h"

int GetTotalEvents(TChain *chain) {

  int numevents = 0;

  TObjArray *files = chain->GetListOfFiles();
  for (int i=0; i<files->GetEntries(); ++i) {    
    TFile *file = TFile::Open(files->At(i)->GetTitle(),"READ");

    TDirectory *fwkdir = (TDirectory*)file->FindObjectAny("AnaFwkMod");
    TH1D *hevents = (TH1D*)fwkdir->Get("hDAllEvents");

    numevents += hevents->GetEntries();

    file->Close();

  }

  return numevents;

}

void trainElectronEnergyRegression_ECAL(char* trainingFile, char* outWeightFile, char* optionChar, int nTrees) {


  
  // Setting up training option
  std::string optionStr(optionChar);

  // ******** If option is V00, V01, V02, etc. ********* //
  if (optionStr == "V00" || optionStr == "V01") {

    GBRTrainer *traineb = new GBRTrainer;
    GBRTrainer *trainebvar = new GBRTrainer;
    GBRTrainer *trainee = new GBRTrainer;
    GBRTrainer *traineevar = new GBRTrainer;

    TTree *intree = 0;

    cout << "Training on file " << trainingFile << " with version " << optionChar << endl;
    TChain *chainele = new TChain("eleIDdir/T1");
    chainele->Add(trainingFile);
    chainele->LoadTree(0);    
    chainele->SetCacheSize(64*1024*1024);
    chainele->SetCacheLearnEntries();
    intree = chainele;

    traineb->AddTree(chainele);
    trainebvar->AddTree(chainele);
    trainee->AddTree(chainele);
    traineevar->AddTree(chainele);

    TCut traincut = "pt>0";////////////////////////////////

    TCut evtcut;
    TCut evtcutvar;
    TCut statusenergycut;

    //if you want to train also energy variance
    evtcut = "event%2==0 ";
    evtcutvar = "event%2==1 ";


    statusenergycut="(GeneratedEnergyStatus3-GeneratedEnergyStatus1)/GeneratedEnergyStatus3<0.01 && GeneratedEnergyStatus3>=GeneratedEnergyStatus1";

    traineb->SetTrainingCut(std::string(traincut && evtcut && statusenergycut && "abs(eta)<1.479 && mcmatch==1"));
    trainee->SetTrainingCut(std::string(traincut && evtcut && statusenergycut && "abs(eta)>1.479 && abs(eta)<2.5 && mcmatch==1"));
    //turn this off for now

    trainebvar->SetTrainingCut(std::string(traincut && evtcutvar && statusenergycut && "abs(eta)<1.479 && mcmatch==1"));
    traineevar->SetTrainingCut(std::string(traincut && evtcutvar && statusenergycut && "abs(eta)>1.479 && abs(eta)<2.5 && mcmatch==1"));
   

    const double maxsig = 3.0;
    const double shrinkage = 0.1;

    traineb->SetMinEvents(200);
    traineb->SetShrinkage(shrinkage);  
    traineb->SetMinCutSignificance(maxsig);

    trainebvar->SetMinEvents(200);
    trainebvar->SetShrinkage(shrinkage);  
    trainebvar->SetMinCutSignificance(maxsig);  

    trainee->SetMinEvents(200);
    trainee->SetShrinkage(shrinkage);  
    trainee->SetMinCutSignificance(maxsig);  

    traineevar->SetMinEvents(200);
    traineevar->SetShrinkage(shrinkage);  
    traineevar->SetMinCutSignificance(maxsig);    

    traineb->SetTargetVar("energyMC/rawEnergySCEle");
    trainebvar->SetTargetVar("abs( targeteb - energyMC/rawEnergySCEle) ");
    trainee->SetTargetVar("energyMC/(rawEnergySCEle*(1+PreShowerOverRaw))");
    traineevar->SetTargetVar("abs( targetee - energyMC/(rawEnergySCEle*(1+PreShowerOverRaw))) ");

    std::vector<std::string> *varsf = new std::vector<std::string>;
    varsf->push_back("rawEnergySCEle");
    varsf->push_back("etaSCEle");
    varsf->push_back("phiSCEle");
    varsf->push_back("R9Ele");  
    varsf->push_back("e5x5SCEle/rawEnergySCEle");  
    varsf->push_back("etaWidthSC");
    varsf->push_back("phiWidthSC");  
    varsf->push_back("clusterSizeSCEle");
    varsf->push_back("hOverEEle");
    varsf->push_back("rho");
    varsf->push_back("nPV");  
    varsf->push_back("etaBC-etaSCEle");
    varsf->push_back("atan2(sin(phiBC-phiSCEle),cos(phiBC-phiSCEle))");
    varsf->push_back("energyBCEle/rawEnergySCEle");
    varsf->push_back("e3x3SCEle/energyBCEle");
    varsf->push_back("e5x5SCEle/energyBCEle");
    varsf->push_back("sigmaIEtaIEtaBC");   
    varsf->push_back("sigmaIPhiIPhiBC");   
    //    varsf->push_back("sep");
    varsf->push_back("energyMaxBC/energyBCEle");
    varsf->push_back("energy2ndBC/energyBCEle");
    varsf->push_back("energyTopBC/energyBCEle");
    varsf->push_back("energyBottomBC/energyBCEle");
    varsf->push_back("energyLeftBC/energyBCEle");
    varsf->push_back("energyRightBC/energyBCEle");
    varsf->push_back("energy2x5MaxBC/energyBCEle");
    varsf->push_back("energy2x5TopBC/energyBCEle");
    varsf->push_back("energy2x5BottomBC/energyBCEle");
    varsf->push_back("energy2x5LeftBC/energyBCEle");
    varsf->push_back("energy2x5RightBC/energyBCEle");


    std::vector<std::string> *varseb = new std::vector<std::string>(*varsf);
    std::vector<std::string> *varsee = new std::vector<std::string>(*varsf);

    varseb->push_back("iEtaBC");
    varseb->push_back("iPhiBC");
    varseb->push_back("iEtaBC%5");
    varseb->push_back("iPhiBC%2");       
    varseb->push_back("(abs(iEtaBC)<=25)*(iEtaBC%25) + (abs(iEtaBC)>25)*((iEtaBC-25*abs(iEtaBC)/iEtaBC)%20)");
    varseb->push_back("iPhiBC%20"); 
    varseb->push_back("etaCryBC");
    varseb->push_back("phiCryBC");

    varsee->push_back("esEnergySCEle/rawEnergySCEle");


    for (int i=0; i<varseb->size(); ++i) {
      cout << "var " << i << " = " << varseb->at(i) << endl;
      traineb->AddInputVar(varseb->at(i));
      trainebvar->AddInputVar(varseb->at(i));
    }

    for (int i=0; i<varsee->size(); ++i) {
      cout << "var " << i << " = " << varsee->at(i) << endl;
      trainee->AddInputVar(varsee->at(i));
      traineevar->AddInputVar(varsee->at(i));
    }

    ROOT::Cintex::Cintex::Enable();   

    //  TFile *ftmp = new TFile("tmpfile.root","RECREATE");    
    GBRApply gbrapply;


    //Train Barrel Energy Regression
    intree->LoadTree(0);  
    const GBRForest *foresteb = traineb->TrainForest(nTrees);
    delete traineb;

    //Apply Barrel Energy Regression
    intree->LoadTree(0);  
    gbrapply.ApplyAsFriend(intree, foresteb, *varseb, "targeteb");


    //Train Barrel Variance Regression
    intree->LoadTree(0);
    const GBRForest *forestebvar = trainebvar->TrainForest(nTrees);
    delete trainebvar;

    //Train Endcap Energy Regression
    intree->LoadTree(0);
    const GBRForest *forestee = trainee->TrainForest(nTrees);
    delete trainee;

    //Apply Endcap Energy Regression
    intree->LoadTree(0);
    gbrapply.ApplyAsFriend(intree, forestee, *varsee, "targetee");

    //Train Endcap Variance Regression
    intree->LoadTree(0);  
    const GBRForest *foresteevar = traineevar->TrainForest(nTrees);
    delete traineevar;  

    TString fname;

    fname = outWeightFile;



    TFile *fout = new TFile(fname,"RECREATE");  
    cout << "Saving weights to file " << fname << endl;

    fout->WriteObject(foresteb,"EBCorrection");
    fout->WriteObject(forestebvar,"EBUncertainty");
    fout->WriteObject(forestee,"EECorrection");
    fout->WriteObject(foresteevar,"EEUncertainty");

    fout->WriteObject(varseb, "varlisteb");
    fout->WriteObject(varsee, "varlistee");

    //  ftmp->Close();  
    //  fout->Close();
  }  



  // ******** If option is V10, V11, V12, etc. ******** //
  // *** Do training separately for low and high pT *** //
  if (optionStr == "V10" || optionStr == "V11") {

    GBRTrainer *traineb_lowPt = new GBRTrainer;
    GBRTrainer *traineb_highPt = new GBRTrainer;
    GBRTrainer *trainebvar_lowPt = new GBRTrainer;
    GBRTrainer *trainebvar_highPt = new GBRTrainer;
    GBRTrainer *trainee_lowPt = new GBRTrainer;
    GBRTrainer *trainee_highPt = new GBRTrainer;
    GBRTrainer *traineevar_lowPt = new GBRTrainer;
    GBRTrainer *traineevar_highPt = new GBRTrainer;

    TTree *intree = 0;

    cout << "Training on file " << trainingFile << " with version " << optionChar << endl;
    TChain *chainele = new TChain("eleIDdir/T1");
    chainele->Add(trainingFile);
    chainele->LoadTree(0);    
    chainele->SetCacheSize(64*1024*1024);
    chainele->SetCacheLearnEntries();
    intree = chainele;

    traineb_lowPt->AddTree(chainele);
    trainebvar_lowPt->AddTree(chainele);
    trainee_lowPt->AddTree(chainele);
    traineevar_lowPt->AddTree(chainele);
    traineb_highPt->AddTree(chainele);
    trainebvar_highPt->AddTree(chainele);
    trainee_highPt->AddTree(chainele);
    traineevar_highPt->AddTree(chainele);
    
    TCut traincut_lowPt = "pt>7 && pt<15";
    TCut traincut_highPt = "pt>=15";
    TCut statusenergycut;
    

    TCut evtcut;
    TCut evtcutvar;

    //if you want to train also energy variance
    evtcut = "event%2==0 ";
    evtcutvar = "event%2==1 ";

    statusenergycut="(energyMC-GeneratedEnergyStatus1)/energyMC<0.01 && energyMC>=GeneratedEnergyStatus1";


    traineb_lowPt->SetTrainingCut(std::string(traincut_lowPt && evtcut && statusenergycut && "abs(eta)<1.479 && mcmatch==1"));
    trainee_lowPt->SetTrainingCut(std::string(traincut_lowPt && evtcut && statusenergycut && "abs(eta)>1.479 && abs(eta)<2.5 && mcmatch==1"));
    traineb_highPt->SetTrainingCut(std::string(traincut_highPt && evtcut && statusenergycut && "abs(eta)<1.479 && mcmatch==1"));	
    trainee_highPt->SetTrainingCut(std::string(traincut_highPt && evtcut && statusenergycut && "abs(eta)>1.479 && abs(eta)<2.5&& mcmatch==1"));


    //turn this off for now
    trainebvar_lowPt->SetTrainingCut(std::string(traincut_lowPt && evtcutvar && statusenergycut && "abs(eta)<1.479 && mcmatch==1"));		       
    traineevar_lowPt->SetTrainingCut(std::string(traincut_lowPt && evtcutvar && statusenergycut && "abs(eta)>1.479 && abs(eta)<2.5 && mcmatch==1")); 
    trainebvar_highPt->SetTrainingCut(std::string(traincut_highPt && evtcutvar && statusenergycut && "abs(eta)<1.479 && mcmatch==1"));	       
    traineevar_highPt->SetTrainingCut(std::string(traincut_highPt && evtcutvar && statusenergycut && "abs(eta)>1.479 && abs(eta)<2.5&& mcmatch==1"));

    const double maxsig = 3.0;
    const double shrinkage = 0.1;

    traineb_lowPt->SetMinEvents(200);
    traineb_lowPt->SetShrinkage(shrinkage);  
    traineb_lowPt->SetMinCutSignificance(maxsig);
    traineb_highPt->SetMinEvents(200);
    traineb_highPt->SetShrinkage(shrinkage);  
    traineb_highPt->SetMinCutSignificance(maxsig);

    trainebvar_lowPt->SetMinEvents(200);
    trainebvar_lowPt->SetShrinkage(shrinkage);  
    trainebvar_lowPt->SetMinCutSignificance(maxsig);  
    trainebvar_highPt->SetMinEvents(200);
    trainebvar_highPt->SetShrinkage(shrinkage);  
    trainebvar_highPt->SetMinCutSignificance(maxsig);  

    trainee_lowPt->SetMinEvents(200);
    trainee_lowPt->SetShrinkage(shrinkage);  
    trainee_lowPt->SetMinCutSignificance(maxsig);  
    trainee_highPt->SetMinEvents(200);
    trainee_highPt->SetShrinkage(shrinkage);  
    trainee_highPt->SetMinCutSignificance(maxsig);  

    traineevar_lowPt->SetMinEvents(200);
    traineevar_lowPt->SetShrinkage(shrinkage);  
    traineevar_lowPt->SetMinCutSignificance(maxsig);    
    traineevar_highPt->SetMinEvents(200);
    traineevar_highPt->SetShrinkage(shrinkage);  
    traineevar_highPt->SetMinCutSignificance(maxsig);    

    traineb_lowPt->SetTargetVar("energyMC/rawEnergySCEle");
    traineb_highPt->SetTargetVar("energyMC/rawEnergySCEle");
    trainebvar_lowPt->SetTargetVar("abs( targeteb_lowPt - energyMC/rawEnergySCEle) ");
    trainebvar_highPt->SetTargetVar("abs( targeteb_highPt - energyMC/rawEnergySCEle) ");
    trainee_lowPt->SetTargetVar("energyMC/(rawEnergySCEle*(1+PreShowerOverRaw))");
    trainee_highPt->SetTargetVar("energyMC/(rawEnergySCEle*(1+PreShowerOverRaw))");
    traineevar_lowPt->SetTargetVar("abs( targetee_lowPt - energyMC/(rawEnergySCEle*(1+PreShowerOverRaw))) ");
    traineevar_highPt->SetTargetVar("abs( targetee_highPt - energyMC/(rawEnergySCEle*(1+PreShowerOverRaw))) ");

    std::vector<std::string> *varsf = new std::vector<std::string>;
    varsf->push_back("rawEnergySCEle");
    varsf->push_back("etaSCEle");
    varsf->push_back("phiSCEle");
    varsf->push_back("R9Ele");  
    varsf->push_back("e5x5SCEle/rawEnergySCEle");  
    varsf->push_back("etaWidthSC");
    varsf->push_back("phiWidthSC");  
    varsf->push_back("clusterSizeSCEle");
    varsf->push_back("hOverEEle");
    varsf->push_back("rho");
    varsf->push_back("nPV");  

    varsf->push_back("etaBC-etaSCEle");
    varsf->push_back("atan2(sin(phiBC-phiSCEle),cos(phiBC-phiSCEle))");
    varsf->push_back("energyBCEle/rawEnergySCEle");
    varsf->push_back("e3x3SCEle/energyBCEle");
    varsf->push_back("e5x5SCEle/energyBCEle");
    varsf->push_back("sigmaIEtaIEtaBC");   
    varsf->push_back("sigmaIPhiIPhiBC");   
    //    varsf->push_back("sep");
    varsf->push_back("energyMaxBC/energyBCEle");
    varsf->push_back("energy2ndBC/energyBCEle");
    varsf->push_back("energyTopBC/energyBCEle");
    varsf->push_back("energyBottomBC/energyBCEle");
    varsf->push_back("energyLeftBC/energyBCEle");
    varsf->push_back("energyRightBC/energyBCEle");
    varsf->push_back("energy2x5MaxBC/energyBCEle");
    varsf->push_back("energy2x5TopBC/energyBCEle");
    varsf->push_back("energy2x5BottomBC/energyBCEle");
    varsf->push_back("energy2x5LeftBC/energyBCEle");
    varsf->push_back("energy2x5RightBC/energyBCEle");


    std::vector<std::string> *varseb = new std::vector<std::string>(*varsf);
    std::vector<std::string> *varsee = new std::vector<std::string>(*varsf);

    varseb->push_back("iEtaBC");
    varseb->push_back("iPhiBC");
    varseb->push_back("iEtaBC%5");
    varseb->push_back("iPhiBC%2");       
    varseb->push_back("(abs(iEtaBC)<=25)*(iEtaBC%25) + (abs(iEtaBC)>25)*((iEtaBC-25*abs(iEtaBC)/iEtaBC)%20)");
    varseb->push_back("iPhiBC%20"); 
    varseb->push_back("etaCryBC");
    varseb->push_back("phiCryBC");

    varsee->push_back("PreShowerOverRaw");


    for (int i=0; i<varseb->size(); ++i) {
      cout << "var " << i << " = " << varseb->at(i) << endl;
      traineb_lowPt->AddInputVar(varseb->at(i));
      trainebvar_lowPt->AddInputVar(varseb->at(i));
    }

    for (int i=0; i<varseb->size(); ++i) {
      cout << "var " << i << " = " << varseb->at(i) << endl;
      traineb_highPt->AddInputVar(varseb->at(i));
      trainebvar_highPt->AddInputVar(varseb->at(i));
    }

    for (int i=0; i<varsee->size(); ++i) {
      cout << "var " << i << " = " << varsee->at(i) << endl;
      trainee_lowPt->AddInputVar(varsee->at(i));
      traineevar_lowPt->AddInputVar(varsee->at(i));
    }

    for (int i=0; i<varsee->size(); ++i) {
      cout << "var " << i << " = " << varsee->at(i) << endl;
      trainee_highPt->AddInputVar(varsee->at(i));
      traineevar_highPt->AddInputVar(varsee->at(i));
    }

    ROOT::Cintex::Cintex::Enable();   

    //  TFile *ftmp = new TFile("tmpfile.root","RECREATE");    
    GBRApply gbrapply;

    //Train Barrel Energy Regression (low pT)
    intree->LoadTree(0);  
    const GBRForest *foresteb_lowPt = traineb_lowPt->TrainForest(nTrees);
    delete traineb_lowPt;

    //Apply Barrel Energy Regression (low pT)
    intree->LoadTree(0);  
    gbrapply.ApplyAsFriend(intree, foresteb_lowPt, *varseb, "targeteb_lowPt");

    //Train Barrel Energy Regression (high pT)
    intree->LoadTree(0);  
    const GBRForest *foresteb_highPt = traineb_highPt->TrainForest(nTrees);
    delete traineb_highPt;

    //Apply Barrel Energy Regression (high pT)
    intree->LoadTree(0);  
    gbrapply.ApplyAsFriend(intree, foresteb_highPt, *varseb, "targeteb_highPt");

    //Train Barrel Variance Regression (low pT)
    intree->LoadTree(0);
    const GBRForest *forestebvar_lowPt = trainebvar_lowPt->TrainForest(nTrees);
    delete trainebvar_lowPt;

    //Train Barrel Variance Regression (high pT)
    intree->LoadTree(0);
    const GBRForest *forestebvar_highPt = trainebvar_highPt->TrainForest(nTrees);
    delete trainebvar_highPt;

    //Train Endcap Energy Regression (low pT)
    intree->LoadTree(0);
    const GBRForest *forestee_lowPt = trainee_lowPt->TrainForest(nTrees);
    delete trainee_lowPt;

    //Apply Endcap Energy Regression
    intree->LoadTree(0);
    gbrapply.ApplyAsFriend(intree, forestee_lowPt, *varsee, "targetee_lowPt");

    //Train Endcap Energy Regression (high pT)
    intree->LoadTree(0);
    const GBRForest *forestee_highPt = trainee_highPt->TrainForest(nTrees);
    delete trainee_highPt;

    //Apply Endcap Energy Regression
    intree->LoadTree(0);
    gbrapply.ApplyAsFriend(intree, forestee_highPt, *varsee, "targetee_highPt");

    //Train Endcap Variance Regression (low pT)
    intree->LoadTree(0);  
    const GBRForest *foresteevar_lowPt = traineevar_lowPt->TrainForest(nTrees);
      delete traineevar_lowPt;  

    //Train Endcap Variance Regression (high pT)
      intree->LoadTree(0);  
      const GBRForest *foresteevar_highPt = traineevar_highPt->TrainForest(nTrees);
      delete traineevar_highPt;  

    TString fname;
    fname = outWeightFile;


    TFile *fout = new TFile(fname,"RECREATE");  
    cout << "Saving weights to file " << fname << endl;

    fout->WriteObject(foresteb_lowPt,"EBCorrection_lowPt");
    fout->WriteObject(foresteb_highPt,"EBCorrection_highPt");
    fout->WriteObject(forestebvar_lowPt,"EBUncertainty_lowPt");
    fout->WriteObject(forestebvar_highPt,"EBUncertainty_highPt");
    fout->WriteObject(forestee_lowPt,"EECorrection_lowPt");
    fout->WriteObject(forestee_highPt,"EECorrection_highPt");
    fout->WriteObject(foresteevar_lowPt,"EEUncertainty_lowPt");
    fout->WriteObject(foresteevar_highPt,"EEUncertainty_highPt");

    fout->WriteObject(varseb, "varlisteb");
    fout->WriteObject(varsee, "varlistee");

    //  ftmp->Close();  
    //  fout->Close();
  }  
}
