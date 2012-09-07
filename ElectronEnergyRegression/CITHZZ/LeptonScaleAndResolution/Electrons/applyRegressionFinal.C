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
#include "TCanvas.h"
#include <string.h>


void applyRegressionFinal(string testingFile, string testingVarFile, string weightFilenameECALandP,string targetFile,  string outFileDirectory, string option) {

  ROOT::Cintex::Cintex::Enable();   

  TFile * weight_file_ECALandP = new TFile(weightFilenameECALandP.c_str());


  if (option == "V00" || option == "V01") {
    //  TFile * weight_file = new TFile("photon_2011_no_tk_vars_energy_reg_weights.root");

    const GBRForest * forest_eb_correction = (GBRForest *)weight_file_ECALandP->Get("EBCorrection");
    const GBRForest * forest_eb_uncertainty = (GBRForest *)weight_file_ECALandP->Get("EBUncertainty");
    const GBRForest * forest_ee_correction = (GBRForest *)weight_file_ECALandP->Get("EECorrection");
    const GBRForest * forest_ee_uncertainty = (GBRForest *)weight_file_ECALandP->Get("EEUncertainty");



    assert(forest_eb_correction);
    assert(forest_eb_uncertainty);
    assert(forest_ee_correction);
    assert(forest_ee_uncertainty);



    TTree *intree = 0;
    TChain *chainele = new TChain("T1");
    chainele->Add(testingFile.c_str());
    chainele->LoadTree(0);    
    chainele->SetCacheSize(64*1024*1024);
    chainele->SetCacheLearnEntries();
    //adding friends
    TFile *fileTarget = TFile::Open(targetFile.c_str());

    chainele->AddFriend("targeteb_tree",fileTarget);
    chainele->AddFriend("targetebvar_tree",fileTarget);
    chainele->AddFriend("targetee_tree",fileTarget);
    chainele->AddFriend("targeteevar_tree",fileTarget);
    chainele->AddFriend("targetp_tree",fileTarget);
    chainele->AddFriend("targetpvar_tree",fileTarget);

    intree = chainele;

    TTree *intreeVar = 0;
    TChain *chaineleVar = new TChain("T1");
    chaineleVar->Add(testingVarFile.c_str());
    chaineleVar->LoadTree(0);    
    chaineleVar->SetCacheSize(64*1024*1024);
    chaineleVar->SetCacheLearnEntries();



    chaineleVar->AddFriend("targeteb_tree",fileTarget);
    chaineleVar->AddFriend("targetebvar_tree",fileTarget);
    chaineleVar->AddFriend("targetee_tree",fileTarget);
    chaineleVar->AddFriend("targeteevar_tree",fileTarget);
    chaineleVar->AddFriend("targetp_tree",fileTarget);
    chaineleVar->AddFriend("targetpvar_tree",fileTarget);

    intreeVar = chaineleVar;


    GBRApply gbrapply;

    std::vector<string> *varseb = (std::vector<string> *)weight_file_ECALandP->Get("varlisteb");
    std::vector<string> *varsee = (std::vector<string> *)weight_file_ECALandP->Get("varlistee");


    TFile * outFile = new TFile((outFileDirectory + "ElectronEnergyRegression_Final_" + option + ".root").c_str(), "RECREATE");



    intree->LoadTree(0);
    TTree *intree_eb = gbrapply.ApplyAsFriend(intree, forest_eb_correction, *varseb, "targetebECALP");
    intree_eb->SetName("targetebECALP_tree");
    intree_eb->SetTitle("targetebECALP_tree");

    intreeVar->LoadTree(0);
    TTree *intree_ebvar = gbrapply.ApplyAsFriend(intreeVar, forest_eb_uncertainty, *varseb, "targetebECALPvar");
    intree_ebvar->SetName("targetebECALPvar_tree");
    intree_ebvar->SetTitle("targetebECALPvar_tree");


    intree->LoadTree(0);
    TTree *intree_ee = gbrapply.ApplyAsFriend(intree, forest_ee_correction, *varsee, "targeteeECALP");
    intree_ee->SetName("targeteeECALP_tree");
    intree_ee->SetTitle("targeteeECALP_tree");

    intreeVar->LoadTree(0);
    TTree *intree_eevar = gbrapply.ApplyAsFriend(intreeVar, forest_ee_uncertainty, *varsee, "targeteeECALPvar");
    intree_eevar->SetName("targeteeECALPvar_tree");
    intree_eevar->SetTitle("targeteeECALPvar_tree");


    outFile->cd();
    intree_eb->Write();
    intree_ebvar->Write();
    intree_ee->Write();
    intree_eevar->Write();
    outFile->Write();

  }

  if (option == "V10" || option == "V11") {
    //  TFile * weight_file = new TFile("photon_2011_no_tk_vars_energy_reg_weights.root");

    const GBRForest * forest_eb_correction_lowPt = (GBRForest *)weight_file_ECALandP->Get("EBCorrection_lowPt");
    const GBRForest * forest_eb_correction_highPt = (GBRForest *)weight_file_ECALandP->Get("EBCorrection_highPt");
    const GBRForest * forest_eb_uncertainty_lowPt = (GBRForest *)weight_file_ECALandP->Get("EBUncertainty_lowPt");
    const GBRForest * forest_eb_uncertainty_highPt = (GBRForest *)weight_file_ECALandP->Get("EBUncertainty_highPt");
    const GBRForest * forest_ee_correction_lowPt = (GBRForest *)weight_file_ECALandP->Get("EECorrection_lowPt");
    const GBRForest * forest_ee_correction_highPt = (GBRForest *)weight_file_ECALandP->Get("EECorrection_highPt");
    const GBRForest * forest_ee_uncertainty_lowPt = (GBRForest *)weight_file_ECALandP->Get("EEUncertainty_lowPt");
    const GBRForest * forest_ee_uncertainty_highPt = (GBRForest *)weight_file_ECALandP->Get("EEUncertainty_highPt");

    assert(forest_eb_correction_lowPt);
    assert(forest_eb_correction_highPt);
    assert(forest_eb_uncertainty_lowPt);
    assert(forest_eb_uncertainty_highPt);
    assert(forest_ee_correction_lowPt);
    assert(forest_ee_correction_highPt);
    assert(forest_ee_uncertainty_lowPt);
    assert(forest_ee_uncertainty_highPt);


    TTree *intree = 0;

    TChain *chainele = new TChain("T1");
    chainele->Add(testingFile.c_str());
    chainele->LoadTree(0);    
    chainele->SetCacheSize(64*1024*1024);
    chainele->SetCacheLearnEntries();
    intree = chainele;

    TTree *intreeVar = 0;

    TChain *chaineleVar = new TChain("T1");
    chaineleVar->Add(testingVarFile.c_str());
    chaineleVar->LoadTree(0);    
    chaineleVar->SetCacheSize(64*1024*1024);
    chaineleVar->SetCacheLearnEntries();
    intreeVar = chaineleVar;

    GBRApply gbrapply;

    std::vector<string> *varseb = (std::vector<string> *)weight_file_ECALandP->Get("varlisteb");
    std::vector<string> *varsee = (std::vector<string> *)weight_file_ECALandP->Get("varlistee");


    TFile * outFile = new TFile((outFileDirectory + "ElectronEnergyRegression_Final_" + option + ".root").c_str(), "RECREATE");


    intree->LoadTree(0);
    TTree *intree_eb_lowPt = gbrapply.ApplyAsFriend(intree, forest_eb_correction_lowPt, *varseb, "targetebECALP_lowPt");
    
    intree->LoadTree(0);
    TTree *intree_eb_highPt = gbrapply.ApplyAsFriend(intree, forest_eb_correction_highPt, *varseb, "targetebECALP_highPt");


    intreeVar->LoadTree(0);
    TTree *intree_ebvar_lowPt = gbrapply.ApplyAsFriend(intreeVar, forest_eb_uncertainty_lowPt, *varseb, "targetebECALPvar_lowPt");

    intreeVar->LoadTree(0);
    TTree *intree_ebvar_highPt = gbrapply.ApplyAsFriend(intreeVar, forest_eb_uncertainty_highPt, *varseb, "targetebECALPvar_highPt");

    intree->LoadTree(0);
    TTree *intree_ee_lowPt = gbrapply.ApplyAsFriend(intree, forest_ee_correction_lowPt, *varsee, "targeteeECALP_lowPt");

    intree->LoadTree(0);
    TTree *intree_ee_highPt = gbrapply.ApplyAsFriend(intree, forest_ee_correction_highPt, *varsee, "targeteeECALP_highPt");

    intreeVar->LoadTree(0);
    TTree *intree_eevar_lowPt = gbrapply.ApplyAsFriend(intreeVar, forest_ee_uncertainty_lowPt, *varsee, "targeteeECALPvar_lowPt");

    intreeVar->LoadTree(0);
    TTree *intree_eevar_highPt = gbrapply.ApplyAsFriend(intreeVar, forest_ee_uncertainty_highPt, *varsee, "targeteeECALPvar_highPt");



    // Writing trees
    outFile->cd();
    intree_eb_lowPt->SetName("targetebECALP_lowPt_tree");
    intree_eb_lowPt->SetTitle("targetebECALP_lowPt_tree");
    intree_eb_lowPt->Write();
 
    intree_eb_highPt->SetName("targetebECALP_highPt_tree");
    intree_eb_highPt->SetTitle("targetebECALP_highPt_tree");    
    intree_eb_highPt->Write();
    
    intree_ee_lowPt->SetName("targeteeECALP_lowPt_tree");
    intree_ee_lowPt->SetTitle("targeteeECALP_lowPt_tree");    
    intree_ee_lowPt->Write();
    intree_ee_highPt->SetName("targeteeECALP_highPt_tree");
    intree_ee_highPt->SetTitle("targeteeECALP_highPt_tree");    
    intree_ee_highPt->Write();

    intree_ebvar_lowPt->SetName("targetebECALPvar_lowPt_tree");
    intree_ebvar_lowPt->SetTitle("targetebECALPvar_lowPt_tree");
    intree_ebvar_lowPt->Write();
 
    intree_ebvar_highPt->SetName("targetebECALPvar_highPt_tree");
    intree_ebvar_highPt->SetTitle("targetebECALPvar_highPt_tree");    
    intree_ebvar_highPt->Write();
    
    intree_eevar_lowPt->SetName("targeteeECALPvar_lowPt_tree");
    intree_eevar_lowPt->SetTitle("targeteeECALPvar_lowPt_tree");    
    intree_eevar_lowPt->Write();
    intree_eevar_highPt->SetName("targeteeECALPvar_highPt_tree");
    intree_eevar_highPt->SetTitle("targeteeECALPvar_highPt_tree");    
    intree_eevar_highPt->Write();


    outFile->Write();


  }


}
