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


void applyRegressionECALandP(string testingFile,  string weightFilenameECAL, string weightFilenameP, string outFileDirectory, string optionECAL, string optionP) {


  ROOT::Cintex::Cintex::Enable();   

  const GBRForest * forest_p_correction; 
  const GBRForest * forest_p_uncertainty;

  const GBRForest * forest1_1p_correction; 
  const GBRForest * forest1_1p_uncertainty;
  const GBRForest * forest1_2p_correction; 
  const GBRForest * forest1_2p_uncertainty;
  const GBRForest * forest1_3p_correction; 
  const GBRForest * forest1_3p_uncertainty;

  const GBRForest * forest2_1p_correction; 
  const GBRForest * forest2_1p_uncertainty;
  const GBRForest * forest2_2p_correction; 
  const GBRForest * forest2_2p_uncertainty;
  const GBRForest * forest2_3p_correction; 
  const GBRForest * forest2_3p_uncertainty;

  const GBRForest * forest3_1p_correction; 
  const GBRForest * forest3_1p_uncertainty;
  const GBRForest * forest3_2p_correction; 
  const GBRForest * forest3_2p_uncertainty;
  const GBRForest * forest3_3p_correction; 
  const GBRForest * forest3_3p_uncertainty;


					 

  TFile * weight_file_p = new TFile(weightFilenameP.c_str());

  if(optionP=="noSplit"){

  forest_p_correction = (GBRForest *)weight_file_p->Get("Correction");
  forest_p_uncertainty = (GBRForest *)weight_file_p->Get("Uncertainty");
  }else{
  ROOT::Cintex::Cintex::Enable();   

  forest1_1p_correction = (GBRForest *)weight_file_p->Get("Correction1_1");
  forest1_1p_uncertainty = (GBRForest *)weight_file_p->Get("Uncertainty1_1");
  forest1_2p_correction = (GBRForest *)weight_file_p->Get("Correction1_2");
  forest1_2p_uncertainty = (GBRForest *)weight_file_p->Get("Uncertainty1_2");
  forest1_3p_correction = (GBRForest *)weight_file_p->Get("Correction1_3");
  forest1_3p_uncertainty = (GBRForest *)weight_file_p->Get("Uncertainty1_3");

  forest2_1p_correction = (GBRForest *)weight_file_p->Get("Correction2_1");
  forest2_1p_uncertainty = (GBRForest *)weight_file_p->Get("Uncertainty2_1");
  forest2_2p_correction = (GBRForest *)weight_file_p->Get("Correction2_2");
  forest2_2p_uncertainty = (GBRForest *)weight_file_p->Get("Uncertainty2_2");
  forest2_3p_correction = (GBRForest *)weight_file_p->Get("Correction2_3");
  forest2_3p_uncertainty = (GBRForest *)weight_file_p->Get("Uncertainty2_3");

  forest3_1p_correction = (GBRForest *)weight_file_p->Get("Correction3_1");
  forest3_1p_uncertainty = (GBRForest *)weight_file_p->Get("Uncertainty3_1");
  forest3_2p_correction = (GBRForest *)weight_file_p->Get("Correction3_2");
  forest3_2p_uncertainty = (GBRForest *)weight_file_p->Get("Uncertainty3_2");
  forest3_3p_correction = (GBRForest *)weight_file_p->Get("Correction3_3");
  forest3_3p_uncertainty = (GBRForest *)weight_file_p->Get("Uncertainty3_3");
  


  }


  TFile * weight_file_ECAL = new TFile(weightFilenameECAL.c_str());


  if (optionECAL == "V00" || optionECAL == "V01") {
    //  TFile * weight_file = new TFile("photon_2011_no_tk_vars_energy_reg_weights.root");

    const GBRForest * forest_eb_correction = (GBRForest *)weight_file_ECAL->Get("EBCorrection");
    const GBRForest * forest_eb_uncertainty = (GBRForest *)weight_file_ECAL->Get("EBUncertainty");
    const GBRForest * forest_ee_correction = (GBRForest *)weight_file_ECAL->Get("EECorrection");
    const GBRForest * forest_ee_uncertainty = (GBRForest *)weight_file_ECAL->Get("EEUncertainty");



    assert(forest_eb_correction);
    assert(forest_eb_uncertainty);
    assert(forest_ee_correction);
    assert(forest_ee_uncertainty);
  
    if(optionP=="noSplit"){
    assert(forest_p_correction);
    assert(forest_p_uncertainty);
    }else{
    assert(forest1_1p_correction);
    assert(forest1_1p_uncertainty);
    assert(forest1_2p_correction);
    assert(forest1_2p_uncertainty);
    assert(forest1_3p_correction);
    assert(forest1_3p_uncertainty);
    assert(forest2_1p_correction);
    assert(forest2_1p_uncertainty);
    assert(forest2_2p_correction);
    assert(forest2_2p_uncertainty);
    assert(forest2_3p_correction);
    assert(forest2_3p_uncertainty);
    assert(forest3_1p_correction);
    assert(forest3_1p_uncertainty);
    assert(forest3_2p_correction);
    assert(forest3_2p_uncertainty);
    assert(forest3_3p_correction);
    assert(forest3_3p_uncertainty);


    }

    TTree *intree = 0;

    TChain *chainele = new TChain("T1");
    chainele->Add(testingFile.c_str());
    chainele->LoadTree(0);    
    chainele->SetCacheSize(64*1024*1024);
    chainele->SetCacheLearnEntries();
    intree = chainele;

    TTree *intreeVar = 0;

    TChain *chaineleVar = new TChain("T1");
    chaineleVar->Add(testingFile.c_str());
    chaineleVar->LoadTree(0);    
    chaineleVar->SetCacheSize(64*1024*1024);
    chaineleVar->SetCacheLearnEntries();
    intreeVar = chaineleVar;


    GBRApply gbrapply;

    std::vector<string> *varseb = (std::vector<string> *)weight_file_ECAL->Get("varlisteb");
    std::vector<string> *varsee = (std::vector<string> *)weight_file_ECAL->Get("varlistee");
    std::vector<string> *varsp = (std::vector<string> *)weight_file_p->Get("varlist");

    TFile * outFile;

    outFile = new TFile((outFileDirectory + "ElectronEnergyRegression_" + optionECAL +"_"+optionP+ ".root").c_str(), "RECREATE");


    intree->LoadTree(0);
    TTree *intree_eb = gbrapply.ApplyAsFriend(intree, forest_eb_correction, *varseb, "targeteb");
    intree_eb->SetName("targeteb_tree");
    intree_eb->SetTitle("targeteb_tree");

    intreeVar->LoadTree(0);
    TTree *intree_ebvar = gbrapply.ApplyAsFriend(intreeVar, forest_eb_uncertainty, *varseb, "targetebvar");
    intree_ebvar->SetName("targetebvar_tree");
    intree_ebvar->SetTitle("targetebvar_tree");


    intree->LoadTree(0);
    TTree *intree_ee = gbrapply.ApplyAsFriend(intree, forest_ee_correction, *varsee, "targetee");
    intree_ee->SetName("targetee_tree");
    intree_ee->SetTitle("targetee_tree");

    intreeVar->LoadTree(0);
    TTree *intree_eevar = gbrapply.ApplyAsFriend(intreeVar, forest_ee_uncertainty, *varsee, "targeteevar");
    intree_eevar->SetName("targeteevar_tree");
    intree_eevar->SetTitle("targeteevar_tree");


    if (optionP=="noSplit"){
    intree->LoadTree(0);
    TTree *intree_p = gbrapply.ApplyAsFriend(intree, forest_p_correction, *varsp, "targetp");
    intree_p->SetName("targetp_tree");
    intree_p->SetTitle("targetp_tree");

    intreeVar->LoadTree(0);
    TTree *intree_pvar = gbrapply.ApplyAsFriend(intreeVar, forest_p_uncertainty, *varsp, "targetpvar");
    intree_pvar->SetName("targetpvar_tree");
    intree_pvar->SetTitle("targetpvar_tree");
  
    outFile->cd();
    intree_eb->Write();
    intree_ebvar->Write();
    intree_ee->Write();
    intree_eevar->Write();
    intree_p->Write();
    intree_pvar->Write();
    outFile->Write();


    }else{
    intree->LoadTree(0);
    cout<<"1__1"<<endl;
    TTree *intree_p1_1 = gbrapply.ApplyAsFriend(intree, forest1_1p_correction, *varsp, "targetp1_1");
    intree_p1_1->SetName("targetp1_1tree");
    intree_p1_1->SetTitle("targetp1_1tree");
    intreeVar->LoadTree(0);
    TTree *intree_p1_1var = gbrapply.ApplyAsFriend(intreeVar, forest1_1p_uncertainty, *varsp, "targetp1_1var");
    intree_p1_1var->SetName("targetp1_1var_tree");
    intree_p1_1var->SetTitle("targetp1_1var_tree");
    cout<<"1__2"<<endl;
    intree->LoadTree(0);
    TTree *intree_p1_2 = gbrapply.ApplyAsFriend(intree, forest1_2p_correction, *varsp, "targetp1_2");
    intree_p1_2->SetName("targetp1_2tree");
    intree_p1_2->SetTitle("targetp1_2tree");
    intreeVar->LoadTree(0);
    TTree *intree_p1_2var = gbrapply.ApplyAsFriend(intreeVar, forest1_2p_uncertainty, *varsp, "targetp1_2var");
    intree_p1_2var->SetName("targetp1_2var_tree");
    intree_p1_2var->SetTitle("targetp1_2var_tree");
    intree->LoadTree(0);
    TTree *intree_p1_3 = gbrapply.ApplyAsFriend(intree, forest1_3p_correction, *varsp, "targetp1_3");
    intree_p1_3->SetName("targetp1_3tree");
    intree_p1_3->SetTitle("targetp1_3tree");
    intreeVar->LoadTree(0);
    TTree *intree_p1_3var = gbrapply.ApplyAsFriend(intreeVar, forest1_3p_uncertainty, *varsp, "targetp1_3var");
    intree_p1_3var->SetName("targetp1_3var_tree");
    intree_p1_3var->SetTitle("targetp1_3var_tree");
    intree->LoadTree(0);
    TTree *intree_p2_1 = gbrapply.ApplyAsFriend(intree, forest2_1p_correction, *varsp, "targetp2_1");
    intree_p2_1->SetName("targetp2_1tree");
    intree_p2_1->SetTitle("targetp2_1tree");
    intreeVar->LoadTree(0);
    TTree *intree_p2_1var = gbrapply.ApplyAsFriend(intreeVar, forest2_1p_uncertainty, *varsp, "targetp2_1var");
    intree_p2_1var->SetName("targetp2_1var_tree");
    intree_p2_1var->SetTitle("targetp2_1var_tree");
    intree->LoadTree(0);
    TTree *intree_p2_2 = gbrapply.ApplyAsFriend(intree, forest2_2p_correction, *varsp, "targetp2_2");
    intree_p2_2->SetName("targetp2_2tree");
    intree_p2_2->SetTitle("targetp2_2tree");
    intreeVar->LoadTree(0);
    TTree *intree_p2_2var = gbrapply.ApplyAsFriend(intreeVar, forest2_2p_uncertainty, *varsp, "targetp2_2var");
    intree_p2_2var->SetName("targetp2_2var_tree");
    intree_p2_2var->SetTitle("targetp2_2var_tree");
    intree->LoadTree(0);
    TTree *intree_p2_3 = gbrapply.ApplyAsFriend(intree, forest2_3p_correction, *varsp, "targetp2_3");
    intree_p2_3->SetName("targetp2_3tree");
    intree_p2_3->SetTitle("targetp2_3tree");
    intreeVar->LoadTree(0);
    TTree *intree_p2_3var = gbrapply.ApplyAsFriend(intreeVar, forest2_3p_uncertainty, *varsp, "targetp2_3var");
    intree_p2_3var->SetName("targetp2_3var_tree");
    intree_p2_3var->SetTitle("targetp2_3var_tree");
    intree->LoadTree(0);
    TTree *intree_p3_1 = gbrapply.ApplyAsFriend(intree, forest3_1p_correction, *varsp, "targetp3_1");
    intree_p3_1->SetName("targetp3_1tree");
    intree_p3_1->SetTitle("targetp3_1tree");
    intreeVar->LoadTree(0);
    TTree *intree_p3_1var = gbrapply.ApplyAsFriend(intreeVar, forest3_1p_uncertainty, *varsp, "targetp3_1var");
    intree_p3_1var->SetName("targetp3_1var_tree");
    intree_p3_1var->SetTitle("targetp3_1var_tree");
    intree->LoadTree(0);
    TTree *intree_p3_2 = gbrapply.ApplyAsFriend(intree, forest3_2p_correction, *varsp, "targetp3_2");
    intree_p3_2->SetName("targetp3_2tree");
    intree_p3_2->SetTitle("targetp3_2tree");
    intreeVar->LoadTree(0);
    TTree *intree_p3_2var = gbrapply.ApplyAsFriend(intreeVar, forest3_2p_uncertainty, *varsp, "targetp3_2var");
    intree_p3_2var->SetName("targetp3_2var_tree");
    intree_p3_2var->SetTitle("targetp3_2var_tree");
    intree->LoadTree(0);
    TTree *intree_p3_3 = gbrapply.ApplyAsFriend(intree, forest3_3p_correction, *varsp, "targetp3_3");
    intree_p3_3->SetName("targetp3_3tree");
    intree_p3_3->SetTitle("targetp3_3tree");
    intreeVar->LoadTree(0);
    TTree *intree_p3_3var = gbrapply.ApplyAsFriend(intreeVar, forest3_3p_uncertainty, *varsp, "targetp3_3var");
    intree_p3_3var->SetName("targetp3_3var_tree");
    intree_p3_3var->SetTitle("targetp3_3var_tree");




    outFile->cd();
    intree_eb->Write();
    intree_ebvar->Write();
    intree_ee->Write();
    intree_eevar->Write();
    intree_p1_1->Write();
    intree_p1_1var->Write();
    intree_p1_2->Write();
    intree_p1_2var->Write();
    intree_p1_3->Write();
    intree_p1_3var->Write();

    intree_p2_1->Write();
    intree_p2_1var->Write();
    intree_p2_2->Write();
    intree_p2_2var->Write();
    intree_p2_3->Write();
    intree_p2_3var->Write();

    intree_p3_1->Write();
    intree_p3_1var->Write();
    intree_p3_2->Write();
    intree_p3_2var->Write();
    intree_p3_3->Write();
    intree_p3_3var->Write();


    outFile->Write();

    }





  }

  if (optionECAL == "V10" || optionECAL == "V11") {
    //  TFile * weight_file = new TFile("photon_2011_no_tk_vars_energy_reg_weights.root");

    const GBRForest * forest_eb_correction_lowPt = (GBRForest *)weight_file_ECAL->Get("EBCorrection_lowPt");
    const GBRForest * forest_eb_correction_highPt = (GBRForest *)weight_file_ECAL->Get("EBCorrection_highPt");
    const GBRForest * forest_eb_uncertainty_lowPt = (GBRForest *)weight_file_ECAL->Get("EBUncertainty_lowPt");
    const GBRForest * forest_eb_uncertainty_highPt = (GBRForest *)weight_file_ECAL->Get("EBUncertainty_highPt");
    const GBRForest * forest_ee_correction_lowPt = (GBRForest *)weight_file_ECAL->Get("EECorrection_lowPt");
    const GBRForest * forest_ee_correction_highPt = (GBRForest *)weight_file_ECAL->Get("EECorrection_highPt");
    const GBRForest * forest_ee_uncertainty_lowPt = (GBRForest *)weight_file_ECAL->Get("EEUncertainty_lowPt");
    const GBRForest * forest_ee_uncertainty_highPt = (GBRForest *)weight_file_ECAL->Get("EEUncertainty_highPt");

    assert(forest_eb_correction_lowPt);
    assert(forest_eb_correction_highPt);
    assert(forest_eb_uncertainty_lowPt);
    assert(forest_eb_uncertainty_highPt);
    assert(forest_ee_correction_lowPt);
    assert(forest_ee_correction_highPt);
    assert(forest_ee_uncertainty_lowPt);
    assert(forest_ee_uncertainty_highPt);
    assert(forest_p_correction);
    assert(forest_p_uncertainty);

    TTree *intree = 0;

    TChain *chainele = new TChain("T1");
    chainele->Add(testingFile.c_str());
    chainele->LoadTree(0);    
    chainele->SetCacheSize(64*1024*1024);
    chainele->SetCacheLearnEntries();
    intree = chainele;

    TTree *intreeVar = 0;

    TChain *chaineleVar = new TChain("T1");
    chaineleVar->Add(testingFile.c_str());
    chaineleVar->LoadTree(0);    
    chaineleVar->SetCacheSize(64*1024*1024);
    chaineleVar->SetCacheLearnEntries();
    intreeVar = chaineleVar;

    GBRApply gbrapply;

    std::vector<string> *varseb = (std::vector<string> *)weight_file_ECAL->Get("varlisteb");
    std::vector<string> *varsee = (std::vector<string> *)weight_file_ECAL->Get("varlistee");
    std::vector<string> *varsp = (std::vector<string> *)weight_file_p->Get("varlist");

    TFile * outFile = new TFile((outFileDirectory + "ElectronEnergyRegression_" + optionECAL + ".root").c_str(), "RECREATE");


    intree->LoadTree(0);
    TTree *intree_eb_lowPt = gbrapply.ApplyAsFriend(intree, forest_eb_correction_lowPt, *varseb, "targeteb_lowPt");
    
    intree->LoadTree(0);
    TTree *intree_eb_highPt = gbrapply.ApplyAsFriend(intree, forest_eb_correction_highPt, *varseb, "targeteb_highPt");


    intreeVar->LoadTree(0);
    TTree *intree_ebvar_lowPt = gbrapply.ApplyAsFriend(intreeVar, forest_eb_uncertainty_lowPt, *varseb, "targetebvar_lowPt");

    intreeVar->LoadTree(0);
    TTree *intree_ebvar_highPt = gbrapply.ApplyAsFriend(intreeVar, forest_eb_uncertainty_highPt, *varseb, "targetebvar_highPt");

    intree->LoadTree(0);
    TTree *intree_ee_lowPt = gbrapply.ApplyAsFriend(intree, forest_ee_correction_lowPt, *varsee, "targetee_lowPt");

    intree->LoadTree(0);
    TTree *intree_ee_highPt = gbrapply.ApplyAsFriend(intree, forest_ee_correction_highPt, *varsee, "targetee_highPt");

    intreeVar->LoadTree(0);
    TTree *intree_eevar_lowPt = gbrapply.ApplyAsFriend(intreeVar, forest_ee_uncertainty_lowPt, *varsee, "targeteevar_lowPt");

    intreeVar->LoadTree(0);
    TTree *intree_eevar_highPt = gbrapply.ApplyAsFriend(intreeVar, forest_ee_uncertainty_highPt, *varsee, "targeteevar_highPt");

    intree->LoadTree(0);
    TTree *intree_p = gbrapply.ApplyAsFriend(intree, forest_p_correction, *varsp, "targetp");

    intreeVar->LoadTree(0);
    TTree *intree_pvar = gbrapply.ApplyAsFriend(intreeVar, forest_p_uncertainty, *varsp, "targetpvar");


    // Writing trees
    outFile->cd();
    intree_eb_lowPt->SetName("targeteb_lowPt_tree");
    intree_eb_lowPt->SetTitle("targeteb_lowPt_tree");
    intree_eb_lowPt->Write();
 
    intree_eb_highPt->SetName("targeteb_highPt_tree");
    intree_eb_highPt->SetTitle("targeteb_highPt_tree");    
    intree_eb_highPt->Write();
    
    intree_ee_lowPt->SetName("targetee_lowPt_tree");
    intree_ee_lowPt->SetTitle("targetee_lowPt_tree");    
    intree_ee_lowPt->Write();
    intree_ee_highPt->SetName("targetee_highPt_tree");
    intree_ee_highPt->SetTitle("targetee_highPt_tree");    
    intree_ee_highPt->Write();

    intree_ebvar_lowPt->SetName("targetebvar_lowPt_tree");
    intree_ebvar_lowPt->SetTitle("targetebvar_lowPt_tree");
    intree_ebvar_lowPt->Write();
 
    intree_ebvar_highPt->SetName("targetebvar_highPt_tree");
    intree_ebvar_highPt->SetTitle("targetebvar_highPt_tree");    
    intree_ebvar_highPt->Write();
    
    intree_eevar_lowPt->SetName("targeteevar_lowPt_tree");
    intree_eevar_lowPt->SetTitle("targeteevar_lowPt_tree");    
    intree_eevar_lowPt->Write();
    intree_eevar_highPt->SetName("targeteevar_highPt_tree");
    intree_eevar_highPt->SetTitle("targeteevar_highPt_tree");    
    intree_eevar_highPt->Write();

    intree_p->SetName("targetp_tree");
    intree_p->SetTitle("targetp_tree");
    intree_p->Write();
    intree_pvar->SetName("targetpvar_tree");
    intree_pvar->SetTitle("targetpvar_tree");
    intree_pvar->Write();

    outFile->Write();


  }


}
