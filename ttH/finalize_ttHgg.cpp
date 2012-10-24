#include "RedNtpFinalizer_ttHgg.h"
#include "TMath.h"
#include <iostream>




void finalize_oneDataset( const std::string& dataset, const std::string& selectionType, const std::string& bTaggerType, std::vector<std::string> *datasets );
void do_haddCommand( const std::string& dataset, std::vector<std::string> *datasets, const std::string& selectionType, const std::string& bTaggerType );
std::string getSingleFileName( const std::string& dataset, const std::string& selectionType, const std::string& bTaggerType );

bool runOnMC=true;


int main( int argc, char* argv[] ) {

  if( argc!=3 && argc!=4 ) {
    std::cout << "USAGE: ./finalize_ttHgg [dataset] [selectionType] [bTaggerType=\"JP\"]" <<std::endl;
    return 13;
  }


  std::string dataset(argv[1]);
  std::string selectionType(argv[2]);

  std::string bTaggerType="JP";
  if( argc>3 ) {
    std::string bTaggerType_str(argv[3]);
    bTaggerType = bTaggerType_str;
  }


  std::vector<std::string> *datasets = new std::vector<std::string>;


  if( dataset=="DATA_Run2011_FULL" ) {
   
    finalize_oneDataset("DoubleMu_Run2011A_FULL", selectionType, bTaggerType, datasets); //first muons! important!
    finalize_oneDataset("DoubleMu_Run2011B_v2", selectionType, bTaggerType, datasets); //first muons! important!
    finalize_oneDataset("DoubleElectron_Run2011A_FULL", selectionType, bTaggerType, datasets);
    finalize_oneDataset("DoubleElectron_Run2011B_v2", selectionType, bTaggerType, datasets);
    finalize_oneDataset("MuEG_Run2011A_FULL", selectionType, bTaggerType, datasets);
    finalize_oneDataset("MuEG_Run2011B", selectionType, bTaggerType, datasets);

  }else if( dataset=="DATA_Run2012_FULL" ) {
    runOnMC=false;
    finalize_oneDataset("DoublePhoton-Run2012C-13Jul2012-v1_b", selectionType, bTaggerType, datasets); 
    finalize_oneDataset("DoublePhoton-Run2012B-13Jul2012-v1", selectionType, bTaggerType, datasets); 
    finalize_oneDataset("DoublePhoton-Run2012C-13Jul2012-v2", selectionType, bTaggerType, datasets); 
    finalize_oneDataset("Photon-Run2012A-13Jul2012-v1", selectionType, bTaggerType, datasets); 

  } else if( dataset=="DiPhotonBox_8TeV-pythia6" ) {

    finalize_oneDataset("DiPhotonBox_Pt-10To25_8TeV-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("DiPhotonBox_Pt-250ToInf_8TeV-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("DiPhotonBox_Pt-25To250_8TeV-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);

  } else if( dataset=="DiPhoton_8TeV-pythia6" ) {

    finalize_oneDataset("DiPhotonBox_Pt-10To25_8TeV-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("DiPhotonBox_Pt-250ToInf_8TeV-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("DiPhotonBox_Pt-25To250_8TeV-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("DiPhotonJets_8TeV-madgraph-tarball-v2_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);

  } else if( dataset=="V_8TeV") {
  //} else if( dataset=="DYJetsToLL_8TeV-madgraph" ) {

    finalize_oneDataset("DYJetsToLL_M-10To50filter_8TeV-madgraph_Summer12-PU_S7_START52_V9-v1" , selectionType, bTaggerType, datasets);
    finalize_oneDataset("DYJetsToLL_M-50_TuneZ2Star_8TeV-madgraph-tarball_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("WJetsToLNu_TuneZ2Star_8TeV-madgraph-tarball_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);

  } else if( dataset=="GJet_doubleEMEnriched_TuneZ2star_8TeV-pythia6" ) {

    finalize_oneDataset("GJet_Pt-20to40_doubleEMEnriched_TuneZ2star_8TeV-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("GJet_Pt40_doubleEMEnriched_TuneZ2star_8TeV-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);

  } else if( dataset=="QCD_doubleEMEnriched_TuneZ2star_8TeV-pythia6") {

    finalize_oneDataset("QCD_Pt-40_doubleEMEnriched_TuneZ2star_8TeV-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("QCD_Pt-30to40_doubleEMEnriched_TuneZ2star_8TeV-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);

  } else if( dataset=="TT_8TeV" ) {

    //    finalize_oneDataset("TTbarGG_0Jet_S1-cmkuo-TTGG_525_RECO_s46_v1" , selectionType, bTaggerType, datasets);//different from pandolfi
    finalize_oneDataset("TTbarGG_0Jet_Summer12-PU_S7_START52_V9-v1" , selectionType, bTaggerType, datasets);
    finalize_oneDataset("TTGJets_8TeV-madgraph_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("TTJets_TuneZ2star_8TeV-madgraph-tauola_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);

  //} else if( dataset=="VG_8TeV" ) {

  } else if( dataset=="VV_8TeV" ) {

    //finalize_oneDataset("WWJetsTo2L2Nu_TuneZ2star_8TeV-madgraph-tauola");
    finalize_oneDataset("WW_TuneZ2star_8TeV_pythia6_tauola_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("ZZ_TuneZ2star_8TeV_pythia6_tauola_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("WZ_TuneZ2star_8TeV_pythia6_tauola_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("WGToLNuG_TuneZ2star_8TeV-madgraph-tauola_Summer12-PU_S7_START52_V9-v2", selectionType, bTaggerType, datasets);
    finalize_oneDataset("ZGToLLG_8TeV-madgraph_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);

  } else if( dataset=="VGG_8TeV" ) {

    finalize_oneDataset("WmGG", selectionType, bTaggerType, datasets);
    finalize_oneDataset("WpGG", selectionType, bTaggerType, datasets);
    finalize_oneDataset("ZGG", selectionType, bTaggerType, datasets);

  } else if( dataset=="HToGG_M-125_8TeV-pythia6" ) {

    finalize_oneDataset("WH_ZH_HToGG_M-125_8TeV-pythia6_Summer12-PU_S7_START52_V9-v2", selectionType, bTaggerType, datasets);
    finalize_oneDataset("GluGluToHToGG_M-125_8TeV-powheg-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    finalize_oneDataset("VBF_HToGG_M-125_8TeV-powheg-pythia6_Summer12-PU_S7_START52_V9-v1", selectionType, bTaggerType, datasets);
    //    finalize_oneDataset("TTH_HToGG_M-125_8TeV-pythia6", selectionType, bTaggerType, datasets);

  } else if(dataset=="TTH_HToGG_M-125_8TeV-pythia6"){

    finalize_oneDataset("TTH_HToGG_M-125_8TeV-pythia6_Summer12-PU_S7_START52_V9-v2", selectionType, bTaggerType, datasets);

  }else {
  
    finalize_oneDataset( dataset, selectionType, bTaggerType, datasets );

  }


  do_haddCommand( dataset, datasets, selectionType, bTaggerType );

  return 0;

}




void finalize_oneDataset( const std::string& dataset, const std::string& selectionType, const std::string& bTaggerType, std::vector<std::string> *datasets ) {

  std::cout << std::endl << "-> Finalizing " << dataset << std::endl;
  std::cout << "-> Selection: " << selectionType << std::endl;
  std::cout << "-> b-Tagger: " << bTaggerType << std::endl << std::endl;

  RedNtpFinalizer_ttHgg* rf = new RedNtpFinalizer_ttHgg( dataset, selectionType, bTaggerType );
  //rf->set_redNtpDir("/xrootdfs/cms/local/pandolf/HiggsGammaGamma/reduced/redntp.52xv5_VH_feasibility_signalOnly.cicpfloose.regrPho_eCorr_20062012.VH_feasibility_v0/merged");
  //  rf->set_redNtpDir("/xrootdfs/cms/local/pandolf/HiggsGammaGamma/reduced/redntp.52xv5_VH_feasibility.cicpfloose.regrPho_eCorr_20062012.VH_feasibility_v0/merged"); 

  if (runOnMC){
        rf->set_redNtpDir("/xrootdfs/cms/local/micheli/Higgs/reduced/redntp.52xv5.cicpfloose.scales-Lisbon-Hgg.ttH_feasibility_v1/merged"); //MC
    //    rf->set_redNtpDir("/xrootdfs/cms/local/micheli/Higgs/reduced/redntp.52xv5.preselectionCS.scales-Lisbon-Hgg.ttH_feasibility_v1/merged"); //MC
  }else{
       rf->set_redNtpDir("/xrootdfs/cms/local/micheli/Higgs/reduced/redntp.53xv1_data.cicpfloose.scales-Lisbon-Hgg.ttH_feasibility_v1/merged/"); //Data
    //rf->set_redNtpDir("/xrootdfs/cms/local/micheli/Higgs/reduced/redntp.53xv1_data.preselectionCS.scales-Lisbon-Hgg.ttH_feasibility_v1/merged/"); //Data
  }
  rf->addFile(dataset);
  rf->finalize();
  delete rf;

  datasets->push_back(dataset);

}



void do_haddCommand( const std::string& dataset, std::vector<std::string> *datasets, const std::string& selectionType, const std::string& bTaggerType ) {


  if( datasets->size()<=1 ) return;

  
  std::string hadd_command = "hadd -f " + getSingleFileName( dataset, selectionType, bTaggerType );

  std::string suffix = "";
  for( unsigned i=0; i<datasets->size(); ++i )
    suffix = suffix + " " + getSingleFileName( datasets->at(i), selectionType, bTaggerType );

  hadd_command += suffix;
  std::string rm_command = "rm " + suffix;
  system(hadd_command.c_str());
  if( dataset!="HToGG_M-125_8TeV-pythia6" ) //keep also separate signal processes:
    system(rm_command.c_str());

}



std::string getSingleFileName( const std::string& dataset, const std::string& selectionType, const std::string& bTaggerType ) {

  std::string fileName = "ttHgg_" + dataset + "_" +selectionType + "_" + bTaggerType + ".root";
  return fileName;

}
