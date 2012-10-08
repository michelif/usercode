#include "RedNtpFinalizer_ttHgg.h"

#include "QGLikelihood/interface/QGLikelihoodCalculator.h"

#include "KinematicFit/DiJetKinFitter.h"

#include "HelicityLikelihoodDiscriminant/HelicityLikelihoodDiscriminant.h"




RedNtpFinalizer_ttHgg::RedNtpFinalizer_ttHgg( const std::string& dataset, const std::string& selectionType, const std::string& bTaggerType ) : RedNtpFinalizer( "ttHgg", dataset ) 
{

  bTaggerType_ = bTaggerType;

  setSelectionType(selectionType);

}

RedNtpFinalizer_ttHgg::~RedNtpFinalizer_ttHgg()
{

   outFile_->Close();


   if (!tree_) return;
   delete tree_->GetCurrentFile();
}





void RedNtpFinalizer_ttHgg::finalize()
{


   this->Init();


   std::string fullFlags = selectionType_ + "_" + bTaggerType_;
   this->set_flags(fullFlags); //this is for the outfile name
   this->createOutputFile();

   outFile_->cd();



   TH1D*  h1_nvertex = new TH1D("nvertex", "", 51, -0.5, 50.5);
   h1_nvertex->Sumw2();
   TH1D*  h1_nvertex_PUW = new TH1D("nvertex_PUW", "", 51, -0.5, 50.5);
   h1_nvertex_PUW->Sumw2();

   TH1D*  h1_njets = new TH1D("njets", "", 11, -0.5, 10.5);
   h1_njets->Sumw2();
   TH1D*  h1_nbjets_loose = new TH1D("nbjets_loose", "", 11, -0.5, 10.5);
   h1_nbjets_loose->Sumw2();
   TH1D*  h1_nbjets_medium = new TH1D("nbjets_medium", "", 11, -0.5, 10.5);
   h1_nbjets_medium->Sumw2();
   TH1D*  h1_nbjets_loose_2jets = new TH1D("nbjets_loose_2jets", "", 11, -0.5, 10.5);
   h1_nbjets_loose_2jets->Sumw2();
   TH1D*  h1_nbjets_medium_2jets = new TH1D("nbjets_medium_2jets", "", 11, -0.5, 10.5);
   h1_nbjets_medium_2jets->Sumw2();

   TH1D*  h1_njets_ZbbHgg = new TH1D("njets_ZbbHgg", "", 11, -0.5, 10.5);
   h1_njets_ZbbHgg->Sumw2();
   TH1D*  h1_nbjets_loose_ZbbHgg = new TH1D("nbjets_loose_ZbbHgg", "", 11, -0.5, 10.5);
   h1_nbjets_loose_ZbbHgg->Sumw2();
   TH1D*  h1_nbjets_medium_ZbbHgg = new TH1D("nbjets_medium_ZbbHgg", "", 11, -0.5, 10.5);
   h1_nbjets_medium_ZbbHgg->Sumw2();
   TH1D*  h1_nbjets_loose_nj2_ZbbHgg = new TH1D("nbjets_loose_nj2_ZbbHgg", "", 11, -0.5, 10.5);
   h1_nbjets_loose_nj2_ZbbHgg->Sumw2();
   TH1D*  h1_nbjets_medium_nj2_ZbbHgg = new TH1D("nbjets_medium_nj2_ZbbHgg", "", 11, -0.5, 10.5);
   h1_nbjets_medium_nj2_ZbbHgg->Sumw2();

   TH1D*  h1_posMatchedJet = new TH1D("posMatchedJet", "", 11, -0.5, 10.5);
   h1_posMatchedJet->Sumw2();
   TH1D*  h1_ptMatchedJet = new TH1D("ptMatchedJet", "", 1000, 0., 1000.);
   h1_ptMatchedJet->Sumw2();
   TH1D*  h1_etaMatchedJet = new TH1D("etaMatchedJet", "", 100, -5., 5.);
   h1_etaMatchedJet->Sumw2();
   TH1D*  h1_phiMatchedJet = new TH1D("phiMatchedJet", "", 100, -3.1416, 3.1416);
   h1_phiMatchedJet->Sumw2();


   TH1D* h1_ptphot0 = new TH1D("ptphot0", "",200, 0., 200.);
   h1_ptphot0->Sumw2();
   TH1D* h1_ptphot1 = new TH1D("ptphot1", "", 200, 0., 200.);
   h1_ptphot1->Sumw2();
   TH1D* h1_etaphot0 = new TH1D("etaphot0", "", 100, -5., 5.);
   h1_etaphot0->Sumw2();
   TH1D* h1_etaphot1 = new TH1D("etaphot1", "", 100, -5., 5.);
   h1_etaphot1->Sumw2();


   TH1D* h1_ptjet0 = new TH1D("ptjet0", "", 200, 0., 200.);
   h1_ptjet0->Sumw2();
   TH1D* h1_ptjet1 = new TH1D("ptjet1", "", 200, 0., 200.);
   h1_ptjet1->Sumw2();
   TH1D* h1_etajet0 = new TH1D("etajet0", "", 100, -5., 5.);
   h1_etajet0->Sumw2();
   TH1D* h1_etajet1 = new TH1D("etajet1", "", 100, -5., 5.);
   h1_etajet1->Sumw2();

   //other jets
   int numMaxJets=8;
   TH1D* h1_ptOtherJets[numMaxJets];
   TH1D* h1_etaOtherJets[numMaxJets];
   for(int i =0;i<numMaxJets;i++){
     stringstream ii;
     ii<<i+2;
     h1_ptOtherJets[i] = new TH1D("ptjet"+TString(ii.str()), "", 200, 0., 200.);
     h1_ptOtherJets[i]->Sumw2();
     h1_etaOtherJets[i] = new TH1D("etajet"+TString(ii.str()), "", 100, -5., 5.);
     h1_etaOtherJets[i]->Sumw2();
   }

   //MET
   TH1D* h1_pfMet = new TH1D("pfMet", "", 150, 0., 150.);
   h1_etajet1->Sumw2();

   //HT
   TH1D* h1_Ht = new TH1D("Ht", "", 200, 0., 2000.);
   h1_Ht->Sumw2();

   //leptons
   TH1D* h1_ptele1 = new TH1D("ptele1", "", 100, 0., 120.);
   h1_ptele1->Sumw2();
   TH1D* h1_ptele2 = new TH1D("ptele2", "", 120, 0., 120.);
   h1_ptele2->Sumw2();
   TH1D* h1_ptmu1 = new TH1D("ptmu1", "", 200, 0., 200.);
   h1_ptmu1->Sumw2();
   TH1D* h1_ptmu2 = new TH1D("ptmu2", "", 120, 0., 120.);
   h1_ptmu2->Sumw2();



   TH1D* h1_kinfit_chiSquareProbMax = new TH1D("kinfit_chiSquareProbMax", "", 1000, 0., 1.0001);
   h1_kinfit_chiSquareProbMax->Sumw2();
   TH1D* h1_kinfit_chiSquareProbMax_mjjWindow = new TH1D("kinfit_chiSquareProbMax_mjjWindow", "", 1000, 0., 1.0001);
   h1_kinfit_chiSquareProbMax_mjjWindow->Sumw2();

   TH1D* h1_mjj = new TH1D("mjj", "", 200, 0., 500.);
   h1_mjj->Sumw2();
   TH1D* h1_mjj_0btag = new TH1D("mjj_0btag", "", 200, 0., 500.);
   h1_mjj_0btag->Sumw2();
   TH1D* h1_mjj_1btag = new TH1D("mjj_1btag", "", 200, 0., 500.);
   h1_mjj_1btag->Sumw2();
   TH1D* h1_mjj_2btag = new TH1D("mjj_2btag", "", 200, 0., 500.);
   h1_mjj_2btag->Sumw2();
   TH1D* h1_mjj_qglHI = new TH1D("mjj_qglHI", "", 200, 0., 500.);
   h1_mjj_qglHI->Sumw2();
   TH1D* h1_mjj_qglLO = new TH1D("mjj_qglLO", "", 200, 0., 500.);
   h1_mjj_qglLO->Sumw2();
   TH1D* h1_mjj_correct = new TH1D("mjj_correct", "", 200, 0., 500.);
   h1_mjj_correct->Sumw2();
   TH1D* h1_mjj_incorrect = new TH1D("mjj_incorrect", "", 200, 0., 500.);
   h1_mjj_incorrect->Sumw2();

   TH1D* h1_mgg_prepresel = new TH1D("mgg_prepresel", "", 80, 100., 180.);
   h1_mgg_prepresel->Sumw2();
   TH1D* h1_mgg_presel = new TH1D("mgg_presel", "", 80, 100., 180.);
   h1_mgg_presel->Sumw2();

   TH1D* h1_mgg= new TH1D("mgg", "", 80, 100., 180.);
   h1_mgg->Sumw2();

   TH1D* h1_mgg_0btag = new TH1D("mgg_0btag", "", 80, 100., 180.);
   h1_mgg_0btag->Sumw2();
   TH1D* h1_mgg_1btag = new TH1D("mgg_1btag", "", 80, 100., 180.);
   h1_mgg_1btag->Sumw2();
   TH1D* h1_mgg_2btag = new TH1D("mgg_2btag", "", 80, 100., 180.);
   h1_mgg_2btag->Sumw2();
   TH1D* h1_mgg_2btagmed = new TH1D("mgg_2btagmed", "", 80, 100., 180.);
   h1_mgg_2btagmed->Sumw2();

   TH1D* h1_mgg_0btag_ebeb = new TH1D("mgg_0btag_ebeb", "", 80, 100., 180.);
   h1_mgg_0btag_ebeb->Sumw2();
   TH1D* h1_mgg_1btag_ebeb = new TH1D("mgg_1btag_ebeb", "", 80, 100., 180.);
   h1_mgg_1btag_ebeb->Sumw2();
   TH1D* h1_mgg_2btag_ebeb = new TH1D("mgg_2btag_ebeb", "", 80, 100., 180.);
   h1_mgg_2btag_ebeb->Sumw2();

   TH1D* h1_mgg_0btag_nebeb = new TH1D("mgg_0btag_nebeb", "", 80, 100., 180.);
   h1_mgg_0btag_nebeb->Sumw2();
   TH1D* h1_mgg_1btag_nebeb = new TH1D("mgg_1btag_nebeb", "", 80, 100., 180.);
   h1_mgg_1btag_nebeb->Sumw2();
   TH1D* h1_mgg_2btag_nebeb = new TH1D("mgg_2btag_nebeb", "", 80, 100., 180.);
   h1_mgg_2btag_nebeb->Sumw2();

   TH1D*  h1_ptDiphot = new TH1D("ptDiphot", "", 100, 0., 500.);
   h1_ptDiphot->Sumw2();

   TH1D*  h1_deltaPhi = new TH1D("deltaPhi", "", 100, 0., 3.1416);
   h1_deltaPhi->Sumw2();
   TH1D*  h1_ptDijet = new TH1D("ptDijet", "", 100, 0., 500.);
   h1_ptDijet->Sumw2();
   TH1D*  h1_ptRatio = new TH1D("ptRatio", "", 100, 0., 3.);
   h1_ptRatio->Sumw2();
   TH1D*  h1_ptDifference = new TH1D("ptDifference", "", 100, -200., 200.);
   h1_ptDifference->Sumw2();

   TH1D* h1_deltaEtaJets = new TH1D("deltaEtaJets", "", 100, -10., 10.);
   h1_deltaEtaJets->Sumw2();
   TH1D* h1_deltaFabsEtaJets = new TH1D("deltaFabsEtaJets", "", 100, -5., 5.);
   h1_deltaFabsEtaJets->Sumw2();
   TH1D* h1_zeppen = new TH1D("zeppen", "", 100, -8., 8.);
   h1_zeppen->Sumw2();

   TH1D*  h1_deltaPhi_kinfit = new TH1D("deltaPhi_kinfit", "", 100, 0., 3.1416);
   h1_deltaPhi_kinfit->Sumw2();
   TH1D*  h1_ptDijet_kinfit = new TH1D("ptDijet_kinfit", "", 100, 0., 500.);
   h1_ptDijet_kinfit->Sumw2();
   TH1D*  h1_ptRatio_kinfit = new TH1D("ptRatio_kinfit", "", 100, 0., 3.);
   h1_ptRatio_kinfit->Sumw2();
   TH1D*  h1_ptDifference_kinfit = new TH1D("ptDifference_kinfit", "", 100, -200., 200.);
   h1_ptDifference_kinfit->Sumw2();

   TH1D* h1_deltaEtaJets_kinfit = new TH1D("deltaEtaJets_kinfit", "", 100, -10., 10.);
   h1_deltaEtaJets_kinfit->Sumw2();
   TH1D* h1_deltaFabsEtaJets_kinfit = new TH1D("deltaFabsEtaJets_kinfit", "", 100, -5., 5.);
   h1_deltaFabsEtaJets_kinfit->Sumw2();
   TH1D* h1_zeppen_kinfit = new TH1D("zeppen_kinfit", "", 100, -8., 8.);
   h1_zeppen_kinfit->Sumw2();

   TH1D* h1_qgljet0 = new TH1D("qgljet0", "", 100, 0., 1.0001);
   h1_qgljet0->Sumw2();
   TH1D* h1_qgljet1 = new TH1D("qgljet1", "", 100, 0., 1.0001);
   h1_qgljet1->Sumw2();
   TH1D* h1_qgljet0_0btag = new TH1D("qgljet0_0btag", "", 100, 0., 1.0001);
   h1_qgljet0_0btag->Sumw2();
   TH1D* h1_qgljet1_0btag = new TH1D("qgljet1_0btag", "", 100, 0., 1.0001);
   h1_qgljet1_0btag->Sumw2();

   TH1D* h1_qgljet_1btag = new TH1D("qgljet_1btag", "", 100, 0., 1.0001);
   h1_qgljet_1btag->Sumw2();

   TH1D* h1_qgljet0_correct = new TH1D("qgljet0_correct", "", 100, 0., 1.0001);
   h1_qgljet0_correct->Sumw2();
   TH1D* h1_qgljet1_correct = new TH1D("qgljet1_correct", "", 100, 0., 1.0001);
   h1_qgljet1_correct->Sumw2();
   TH1D* h1_qgljet0_incorrect = new TH1D("qgljet0_incorrect", "", 100, 0., 1.0001);
   h1_qgljet0_incorrect->Sumw2();
   TH1D* h1_qgljet1_incorrect = new TH1D("qgljet1_incorrect", "", 100, 0., 1.0001);
   h1_qgljet1_incorrect->Sumw2();

   TH1D* h1_cosTheta1 = new TH1D("cosTheta1", "", 100, -1.0001, 1.0001);
   h1_cosTheta1->Sumw2();
   TH1D* h1_cosTheta2 = new TH1D("cosTheta2", "", 100, -1.0001, 1.0001);
   h1_cosTheta2->Sumw2();
   TH1D* h1_cosThetaStar = new TH1D("cosThetaStar", "", 100, -1.0001, 1.0001);
   h1_cosThetaStar->Sumw2();
   TH1D* h1_helphi = new TH1D("helphi", "", 100, 0., 3.1416);
   h1_helphi->Sumw2();
   TH1D* h1_helphi1 = new TH1D("helphi1", "", 100, 0., 3.1416);
   h1_helphi1->Sumw2();

   TH1D* h1_mVstar = new TH1D("mVstar", "", 1000, 0., 1000.);
   h1_mVstar->Sumw2();
   TH1D* h1_ptVstar = new TH1D("ptVstar", "", 500, 0., 500.);
   h1_ptVstar->Sumw2();
   TH1D* h1_etaVstar = new TH1D("etaVstar", "", 100, -5., 5.);
   h1_etaVstar->Sumw2();
   TH1D* h1_phiVstar = new TH1D("phiVstar", "", 100, 0., 3.1416);
   h1_phiVstar->Sumw2();

   TH1D* h1_mVstar_kinfit = new TH1D("mVstar_kinfit", "", 1000, 0., 1000.);
   h1_mVstar_kinfit->Sumw2();
   TH1D* h1_ptVstar_kinfit = new TH1D("ptVstar_kinfit", "", 500, 0., 500.);
   h1_ptVstar_kinfit->Sumw2();
   TH1D* h1_etaVstar_kinfit = new TH1D("etaVstar_kinfit", "", 100, -5., 5.);
   h1_etaVstar_kinfit->Sumw2();
   TH1D* h1_phiVstar_kinfit = new TH1D("phiVstar_kinfit", "", 100, 0., 3.1416);
   h1_phiVstar_kinfit->Sumw2();



  
   int btagCategory_t;
   int njets_t;
   int nbjets_loose_t;
   int nbjets_medium_t;
   float ptPhot1_t;
   float ptPhot2_t;
   float etaPhot1_t;
   float etaPhot2_t;
   float mgg_t;
   float ptgg_t;
   float ptJet1_t;
   float ptJet2_t;
   float ptJet3_t=0;
   float ptJet4_t=0;
   float ptJet5_t=0;
   float ptJet6_t=0;
   float ptJet7_t=0;
   float ptJet8_t=0;
   float ptJet9_t=0;
   float ptJet10_t=0;
   float etaJet1_t;
   float etaJet2_t;
   float etaJet3_t=-10;
   float etaJet4_t=-10;
   float etaJet5_t=-10;
   float etaJet6_t=-10;
   float etaJet7_t=-10;
   float etaJet8_t=-10;
   float etaJet9_t=-10;
   float etaJet10_t=-10;
   float Ht_t;
   float qglJet1_t;
   float qglJet2_t;
   bool  btagJet1_t;
   bool  btagJet2_t;
   float mjj_t;
   float ptjj_t;
   float zeppen_t;
   float chiSquareProbMax_t;
   float absCosThetaStar_t;

   float eventWeight = 1.;

   TTree* tree_passedEvents = new TTree();
   tree_passedEvents->SetName("tree_passedEvents");
   tree_passedEvents->Branch( "run", &run, "run/I" );
   tree_passedEvents->Branch( "lumi", &lumi, "lumi/I" );
   tree_passedEvents->Branch( "event", &event, "event/I" );
   tree_passedEvents->Branch( "eventWeight", &eventWeight, "eventWeight/F" );
   tree_passedEvents->Branch( "btagCategory", &btagCategory_t, "btagCategory_t/I" );
   tree_passedEvents->Branch( "njets", &njets_t, "njets_t/I" );
   tree_passedEvents->Branch( "nbjets_xloose", &nbjets_loose_t, "nbjets_loose_t/I" );
   tree_passedEvents->Branch( "nbjets_medium", &nbjets_medium_t, "nbjets_medium_t/I" );
   tree_passedEvents->Branch( "ptPhot1", &ptPhot1_t, "ptPhot1_t/F" );
   tree_passedEvents->Branch( "ptPhot2", &ptPhot2_t, "ptPhot2_t/F" );
   tree_passedEvents->Branch( "etaPhot1", &etaPhot1_t, "etaPhot1_t/F" );
   tree_passedEvents->Branch( "etaPhot2", &etaPhot2_t, "etaPhot2_t/F" );
   tree_passedEvents->Branch( "mgg", &mgg_t, "mgg_t/F" );
   tree_passedEvents->Branch( "ptgg", &ptgg_t, "ptgg_t/F" );
   tree_passedEvents->Branch( "ptJet1", &ptJet1_t, "ptJet1_t/F" );
   tree_passedEvents->Branch( "ptJet2", &ptJet2_t, "ptJet2_t/F" );
   tree_passedEvents->Branch( "ptJet3", &ptJet3_t, "ptJet3_t/F" );
   tree_passedEvents->Branch( "ptJet4", &ptJet4_t, "ptJet4_t/F" );
   tree_passedEvents->Branch( "ptJet5", &ptJet5_t, "ptJet5_t/F" );
   tree_passedEvents->Branch( "ptJet6", &ptJet6_t, "ptJet6_t/F" );
   tree_passedEvents->Branch( "ptJet7", &ptJet7_t, "ptJet7_t/F" );
   tree_passedEvents->Branch( "ptJet8", &ptJet8_t, "ptJet8_t/F" );
   tree_passedEvents->Branch( "ptJet9", &ptJet9_t, "ptJet9_t/F" );
   tree_passedEvents->Branch( "ptJet10", &ptJet10_t, "ptJet10_t/F" );
   tree_passedEvents->Branch( "etaJet1", &etaJet1_t, "etaJet1_t/F" );
   tree_passedEvents->Branch( "etaJet2", &etaJet2_t, "etaJet2_t/F" );
   tree_passedEvents->Branch( "etaJet3", &etaJet3_t, "etaJet3_t/F" );
   tree_passedEvents->Branch( "etaJet4", &etaJet4_t, "etaJet4_t/F" );
   tree_passedEvents->Branch( "etaJet5", &etaJet5_t, "etaJet5_t/F" );
   tree_passedEvents->Branch( "etaJet6", &etaJet6_t, "etaJet6_t/F" );
   tree_passedEvents->Branch( "etaJet7", &etaJet7_t, "etaJet7_t/F" );
   tree_passedEvents->Branch( "etaJet8", &etaJet8_t, "etaJet8_t/F" );
   tree_passedEvents->Branch( "etaJet9", &etaJet9_t, "etaJet9_t/F" );
   tree_passedEvents->Branch( "etaJet10", &etaJet10_t, "etaJet10_t/F" );
   tree_passedEvents->Branch( "Ht", &Ht_t, "Ht_t/F" );
   tree_passedEvents->Branch( "qglJet1", &qglJet1_t, "qglJet1_t/F" );
   tree_passedEvents->Branch( "qglJet2", &qglJet2_t, "qglJet2_t/F" );
   tree_passedEvents->Branch( "btagJet1", &btagJet1_t, "btagJet1_t/O" );
   tree_passedEvents->Branch( "btagJet2", &btagJet2_t, "btagJet2_t/O" );
   tree_passedEvents->Branch( "mjj", &mjj_t, "mjj_t/F" );
   tree_passedEvents->Branch( "ptjj", &ptjj_t, "ptjj_t/F" );
   tree_passedEvents->Branch( "zeppen", &zeppen_t, "zeppen_t/F" );
   tree_passedEvents->Branch( "chiSquareProbMax", &chiSquareProbMax_t, "chiSquareProbMax_t/F" );
   tree_passedEvents->Branch( "absCosThetaStar", &absCosThetaStar_t, "absCosThetaStar_t/F" );
   tree_passedEvents->Branch("ptele1", &ptele1, "ptele1/F");
   tree_passedEvents->Branch("etaele1", &etaele1, "etaele1/F");
   tree_passedEvents->Branch("phiele1", &phiele1, "phiele1/F");
   tree_passedEvents->Branch("ptele2", &ptele2, "ptele2/F");
   tree_passedEvents->Branch("etaele2", &etaele2, "etaele2/F");
   tree_passedEvents->Branch("phiele2", &phiele2, "phiele2/F");
   tree_passedEvents->Branch("ptmu1", &ptmu1, "ptmu1/F");
   tree_passedEvents->Branch("etamu1", &etamu1, "etamu1/F");
   tree_passedEvents->Branch("phimu1", &phimu1, "phimu1/F");
   tree_passedEvents->Branch("ptmu2", &ptmu2, "ptmu2/F");
   tree_passedEvents->Branch("etamu2", &etamu2, "etamu2/F");
   tree_passedEvents->Branch("phimu2", &phimu2, "phimu2/F");
   tree_passedEvents->Branch("epfMet", &epfMet, "epfMet");

   //   std::string qglFileName = "/afs/cern.ch/work/p/pandolf/CMSSW_5_2_5/src/UserCode/pandolf/QGLikelihood/QG_QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6_Summer12-PU_S7_START52_V9-v1.root";
   std::string qglFileName="/afs/cern.ch/work/m/micheli/pand/CMSSW_5_2_4/src/UserCode/pandolf/QGLikelihood/QG_QCD_Pt-15to3000_TuneZ2_Flat_8TeV_pythia6_Summer12-PU_S7_START52_V9-v1.root";
   QGLikelihoodCalculator *qglikeli = new QGLikelihoodCalculator( qglFileName );

   float Zmass = 91.19;
   float Wmass = 80.4;

   DiJetKinFitter* fitter_jetsW = new DiJetKinFitter( "fitter_jetsW", "fitter_jets", Wmass );
   DiJetKinFitter* fitter_jetsZ = new DiJetKinFitter( "fitter_jetsZ", "fitter_jets", Zmass );

   HelicityLikelihoodDiscriminant *helicityDiscriminator = new HelicityLikelihoodDiscriminant();

   int seed = 110;
   TRandom3* coin = new TRandom3(seed);

   float allPairs = 0.;
   float correctPairs_lead = 0.;
   float correctPairs_fancy = 0.;


   if (tree_ == 0) return;

   Long64_t nentries = tree_->GetEntries();

   Long64_t nbytes = 0, nb = 0;

   if( xSection_!=1. && xSection_!=0. ) {
     std::cout << std::endl;
     std::cout << "-> Cross-Section: " << xSection_ << " pb" << std::endl;
     std::cout << "-> # Generated Events: " << nGenEvents_ << std::endl;
     std::cout << "-> Event Weight: " << xSection_/nGenEvents_ << std::endl << std::endl;
   }

   for (Long64_t jentry=0; jentry<nentries;jentry++) {

      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;

      if( ientry % 20000 == 0 ) std::cout << "-> Entry " << ientry << " / " << nentries << std::endl;

      nb = tree_->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;


      bool isMC = ( run<5 );

      


      if( isMC ) {

        if( nGenEvents_<=0 ) {
          std::cout << std::endl << "-> There must be a problem: nGenEvents is not positive!! Exiting" << std::endl;
          exit(99);
        }

        eventWeight = xSection_ / nGenEvents_ ;
 
        h1_nvertex->Fill( nvtx, eventWeight );

        // pu/pt reeventWeighting
        if(dopureeventWeight_) eventWeight *= pu_weight; 
        if(doptreeventWeight_) eventWeight *= pt_weight; 

      }

      h1_nvertex_PUW->Fill( nvtx, eventWeight );


      TLorentzVector diphot;
      diphot.SetPtEtaPhiM( ptggnewvtx, etagg, phigg, massggnewvtx);


      // analysis cuts:

      //if(npu>=30) continue;

      // if(massggnewvtx<90 || massggnewvtx>190) continue;
      if(diphot.M()<100 || diphot.M()>180) continue;

      if((TMath::Abs(etascphot1)>1.4442&&TMath::Abs(etascphot1)<1.566)||(TMath::Abs(etascphot2)>1.4442&&TMath::Abs(etascphot2)<1.566)
         || TMath::Abs(etascphot1)>2.5 || TMath::Abs(etascphot2)>2.5) continue;  // acceptance

      //     if(ptphot1<ptphot1cut) continue; //pt first photon

      bool ebeb = TMath::Abs(etascphot1)<1.4442 && TMath::Abs(etascphot2)<1.4442;


      if(ptphot1 < ptphot1cut_) continue; //pt first photon
      if(ptphot2 < ptphot2cut_) continue; //pt second photon

//       if(ptphot2<ptphot2cut* massggnewvtx/120.) continue; //pt second photon

      //if(ptgg_thresh_>0 && diphot.Pt()< pthiggsmincut_) continue; //pt higgs min


//    if(ptjet1cut>0 && (ptcorrjet1<ptjet1cut_ || TMath::Abs(etajet1)>4.7)) continue; //pt first jet
//    if(ptjet2cut>0 && (ptcorrjet2<ptjet2cut_ || TMath::Abs(etajet2)>4.7)) continue; //pt second jet


       // photon identification

       // r9 categories
       bool isr9phot1(0), isr9phot2(0);
 
       if(TMath::Abs(etascphot1)<1.4442 && r9phot1>.94) isr9phot1 = 1;
       if(TMath::Abs(etascphot2)<1.4442 && r9phot2>.94) isr9phot2 = 1;
       if(TMath::Abs(etascphot1)>1.4442 && r9phot1>.94) isr9phot1 = 1;
       if(TMath::Abs(etascphot2)>1.4442 && r9phot2>.94) isr9phot2 = 1;
 
       //if(r9cat_ == 1) {
       //  if(!isr9phot1 || !isr9phot2) continue;
       //} else if (r9cat_ == 0){
       //  if(isr9phot1 && isr9phot2) continue;
       //} 
 
       // photon id
       bool idphot1(0), idphot2(0), looseidphot1(0), looseidphot2(0), pxlphot1(1), pxlphot2(1);
 
 //       if(pixelseedcut) { 
 // 	pxlphot1 = !pid_haspixelseedphot1;
 // 	pxlphot2 = !pid_haspixelseedphot2;
 //       }

      if(!invert_photonCuts_){       
       idphot1 = (idcicpfphot1 >= photonID_thresh_);
       idphot2 = (idcicpfphot2 >= photonID_thresh_);
      }else{
	idphot1 = (idcicpfphot1 >= photonID_thresh_);
	idphot2 = (idcicpfphot2 < photonID_thresh_);
      }

       if(!cs_){ // photon id no control sample
 
       if(photonID_thresh_>0) {
         if(!(idphot1)) continue;
         if(!(idphot2)) continue;
       }else{
         if(!(idphot1 && pxlphot1)) continue;
         if(!(idphot2 && pxlphot2)) continue;
       }
       
       }else{ // photon id for control sample
        
         looseidphot1 = (idcicpfphot1 > 0 );
         looseidphot2 = (idcicpfphot2 > 0 );
         //	  if( !( (idphot1 && looseidphot2 && !idphot2) || (idphot2 && looseidphot1 && !idphot1) ) ) continue; 
         // Not perfect should be using the same electronVeto wrt CiC selection (now using matchedPromptEle veto)
         if( !( (idphot1 && !idphot2 && !pid_hasMatchedPromptElephot2) || (idphot2 && !idphot1 && !pid_hasMatchedPromptElephot1) ) ) continue; 
 
       }


       h1_mgg_prepresel->Fill( massggnewvtx, eventWeight );


       // jets

       int njets_selected = 0;
       int njets_selected_btagloose = 0;
       int njets_selected_btagmedium = 0;
       std::vector<int> index_MatchedJet;
       std::vector<int> index_selected;
       std::vector<int> index_selected_btagloose;
       std::vector<int> index_selected_btagmedium;

       for( unsigned ijet=0; ijet<njets; ++ijet ) {

//std::cout << ijet << "/" << njets << " pt: " << ptcorrjet[ijet] << " eta: " << etajet[ijet] << std::endl;
         if( ptcorrjet[ijet] < ptjetthresh_count_ ) continue;
         if( fabs(etajet[ijet]) > etajetthresh_count_ ) continue;

         //jet PU ID:
         bool passedPUID = true;
         if(TMath::Abs(etajet[ijet]) < 2.5) {
           if(betastarjet[ijet] > 0.2 * log( nvtx - 0.67 ) ) passedPUID = false;
           if(rmsjet[ijet] > 0.06) passedPUID = false;
         } else if(TMath::Abs(etajet[ijet]) < 3.){
           if(rmsjet[ijet] > 0.05) passedPUID = false;
         } else {
           if(rmsjet[ijet] > 0.055) passedPUID = false;
         }
         if( !passedPUID ) continue;


         if( isMC ) {
           if( partMomPdgIDjet[ijet] == 23 || abs( partMomPdgIDjet[ijet] ) == 24 ) {
             h1_posMatchedJet->Fill( ijet, eventWeight );
             h1_ptMatchedJet->Fill( ptcorrjet[ijet], eventWeight );
             h1_etaMatchedJet->Fill( etajet[ijet], eventWeight );
             h1_phiMatchedJet->Fill( phijet[ijet], eventWeight );
             index_MatchedJet.push_back(ijet);
           }
         }

         if( btagjprobjet[ijet]>0.275 ) {
           njets_selected_btagloose++;
           index_selected_btagloose.push_back(ijet);
         }
         if( btagjprobjet[ijet]>0.545 ) {
           njets_selected_btagmedium++;
           index_selected_btagmedium.push_back(ijet);
         }

         index_selected.push_back(ijet);
         njets_selected++;

         //AnalysisJet thisJet;
         //thisJet.set
         //selectedJets.push_back( thisJet );

       } //for jets


       if(njets_selected<njets_thresh_) continue;
       if(njets_selected_btagloose<nbtagloose_thresh_) continue;
       if(njets_selected_btagmedium<nbtagmedium_thresh_) continue;
       int isLeptonic= (ptele1>0 || ptmu1>0);
       if (isLeptonic<isLeptonic_thresh_) continue;
       if(leptonVeto_){
	 if(isLeptonic==1)continue;

       }

       h1_ptphot0->Fill(ptphot1, eventWeight);
       h1_ptphot1->Fill(ptphot2, eventWeight);

       h1_etaphot0->Fill(etaphot1, eventWeight);
       h1_etaphot1->Fill(etaphot2, eventWeight);


       h1_njets->Fill( njets_selected, eventWeight );
       h1_nbjets_loose->Fill( njets_selected_btagloose, eventWeight );
       h1_nbjets_medium->Fill( njets_selected_btagmedium, eventWeight );

       if( njets_selected>=2 ) {
         h1_nbjets_loose_2jets->Fill( njets_selected_btagloose, eventWeight );
         h1_nbjets_medium_2jets->Fill( njets_selected_btagmedium, eventWeight );
       }

       if( H_event && V_event && Zbb_event ) {
         h1_njets_ZbbHgg->Fill( njets_selected, eventWeight );
         h1_nbjets_loose_ZbbHgg->Fill( njets_selected_btagloose, eventWeight );
         h1_nbjets_medium_ZbbHgg->Fill( njets_selected_btagmedium, eventWeight );
         if( njets_selected>1 ) {
           h1_nbjets_loose_nj2_ZbbHgg->Fill( njets_selected_btagloose, eventWeight );
           h1_nbjets_medium_nj2_ZbbHgg->Fill( njets_selected_btagmedium, eventWeight );
         }
       }


       if( index_selected.size()<2 ) continue;

       h1_mgg_presel->Fill( massggnewvtx, eventWeight );


       // try with simply the two leading ones:
       allPairs += eventWeight;
       if( (partMomPdgIDjet[index_selected[0]] == 23 || abs( partMomPdgIDjet[index_selected[0]] ) == 24)
       && ( partMomPdgIDjet[index_selected[1]] == 23 || abs( partMomPdgIDjet[index_selected[1]] ) == 24) ) correctPairs_lead += eventWeight;
       


       // choose jets as two btagged jets, OR leading ones
       int indexjet0 = index_selected[0];
       int indexjet1 = index_selected[1];
       bool chooseBtaggedJets=true;//be careful with this
       bool firstjet_isbtaggedloose = false;
       bool secondjet_isbtaggedloose = false;


       if(chooseBtaggedJets){
       if( index_selected_btagloose.size()==1 ) {
         if( index_selected_btagloose[0]!=index_selected[0] ) { 
           indexjet1 = index_selected_btagloose[0];
           secondjet_isbtaggedloose = true;
         } else {
           firstjet_isbtaggedloose = true;
         }
       }
       if( index_selected_btagloose.size()>1 ) {
         indexjet0 = index_selected_btagloose[0];
         indexjet1 = index_selected_btagloose[1];
         firstjet_isbtaggedloose = true;
         secondjet_isbtaggedloose = true;
       }
       }
       AnalysisJet jet0;
       jet0.SetPtEtaPhiE( ptcorrjet[indexjet0], etajet[indexjet0], phijet[indexjet0], ecorrjet[indexjet0] );
       AnalysisJet jet1;
       jet1.SetPtEtaPhiE( ptcorrjet[indexjet1], etajet[indexjet1], phijet[indexjet1], ecorrjet[indexjet1] );
       AnalysisJet dijet = jet0 + jet1;


       h1_ptjet0->Fill( jet0.Pt(), eventWeight );
       h1_ptjet1->Fill( jet1.Pt(), eventWeight );

       h1_etajet0->Fill( jet0.Eta(), eventWeight );
       h1_etajet1->Fill( jet1.Eta(), eventWeight );

       float HtotherJets=0;


       //additional jets
       ptJet3_t=0;
       ptJet4_t=0;
       ptJet5_t=0;
       ptJet6_t=0;
       ptJet7_t=0;
       ptJet8_t=0;
       ptJet9_t=0;
       ptJet10_t=0;
       AnalysisJet otherJets[numMaxJets];
       for(int i =2; i<index_selected.size();++i){
	 otherJets[i-2].SetPtEtaPhiE( ptcorrjet[index_selected[i]], etajet[index_selected[i]], phijet[index_selected[i]], ecorrjet[index_selected[i]] );
	 h1_ptOtherJets[i-2]->Fill( otherJets[i-2].Pt(), eventWeight );
	 h1_etaOtherJets[i-2]->Fill( otherJets[i-2].Eta(), eventWeight );
	 HtotherJets+=otherJets[i-2].Pt();
	 if(i==2){
	   ptJet3_t=otherJets[i-2].Pt();
	   etaJet3_t=otherJets[i-2].Eta();
	 }
	 if(i==3){
	   ptJet4_t=otherJets[i-2].Pt();
	   etaJet4_t=otherJets[i-2].Eta();
	 }
	 if(i==4){
	   ptJet5_t=otherJets[i-2].Pt();
	   etaJet5_t=otherJets[i-2].Eta();
	 }
	 if(i==5){
	   ptJet6_t=otherJets[i-2].Pt();
	   etaJet6_t=otherJets[i-2].Eta();
	 }
	 if(i==6){
	   ptJet7_t=otherJets[i-2].Pt();
	   etaJet7_t=otherJets[i-2].Eta();
	 }
	 if(i==7){
	   ptJet8_t=otherJets[i-2].Pt();
	   etaJet8_t=otherJets[i-2].Eta();
	 }

	 if(i==8){
	   ptJet9_t=otherJets[i-2].Pt();
	   etaJet9_t=otherJets[i-2].Eta();
	 }

	 if(i==9){
	   ptJet10_t=otherJets[i-2].Pt();
	   etaJet10_t=otherJets[i-2].Eta();
	 }
       }

       //       std::cout<<HtotherJets<<" "<<ptphot1<<" "<<ptphot2<<" "<<jet0.Pt()<<" "<<jet1.Pt()<<std::endl;
       float Ht=HtotherJets+ptphot1+ptphot2+jet0.Pt()+jet1.Pt();
       if(ptele1>0)Ht+=ptele1;
       if(ptele2>0)Ht+=ptele2;
       if(ptmu1>0)Ht+=ptmu1;
       if(ptmu2>0)Ht+=ptmu2;
       if(epfMet>0)Ht+=epfMet;
       if(Ht<Ht_thresh_)continue;
       h1_Ht->Fill(Ht,eventWeight);


       bool chose_correctPair = (partMomPdgIDjet[indexjet0] == 23 || abs( partMomPdgIDjet[indexjet0] ) == 24)
                             && (partMomPdgIDjet[indexjet1] == 23 || abs( partMomPdgIDjet[indexjet1] ) == 24);


       h1_mjj->Fill( dijet.M(), eventWeight );

       if( chose_correctPair ) {

         correctPairs_fancy += eventWeight;
         h1_mjj_correct->Fill( dijet.M(), eventWeight );

       } else {

         h1_mjj_incorrect->Fill( dijet.M(), eventWeight );

       }



       float zeppen = diphot.Eta() - 0.5*( jet0.Eta() + jet1.Eta() );
       h1_zeppen->Fill( zeppen, eventWeight);



       // perform two kinfits:
       std::pair<TLorentzVector,TLorentzVector> jets_kinfitW = fitter_jetsW->fit(jet0, jet1);
       std::pair<TLorentzVector,TLorentzVector> jets_kinfitZ = fitter_jetsZ->fit(jet0, jet1);
       float chiSquareProbW = TMath::Prob(fitter_jetsW->getS(), fitter_jetsW->getNDF());
       float chiSquareProbZ = TMath::Prob(fitter_jetsZ->getS(), fitter_jetsZ->getNDF());


       TLorentzVector phot0, phot1;
       phot0.SetPtEtaPhiM( ptphot1, etaphot1, phiphot1, 0.);
       phot1.SetPtEtaPhiM( ptphot2, etaphot2, phiphot2, 0.);

       HelicityLikelihoodDiscriminant::HelicityAngles hangles;
       if( coin->Uniform(1.)<0.5 ) hangles = helicityDiscriminator->computeHelicityAngles(phot0, phot1, jet0, jet1);
       else                        hangles = helicityDiscriminator->computeHelicityAngles(phot1, phot0, jet0, jet1);

       
       float cosThetaStar = hangles.helCosThetaStar;
       h1_cosThetaStar->Fill( hangles.helCosThetaStar, eventWeight );
       h1_cosTheta1->Fill( hangles.helCosTheta1, eventWeight );
       h1_cosTheta2->Fill( hangles.helCosTheta2, eventWeight );
       h1_helphi->Fill( hangles.helPhi, eventWeight );
       h1_helphi1->Fill( hangles.helPhi1, eventWeight );

       TLorentzVector jet0_kinfit, jet1_kinfit;
       float chiSquareProbMax=0.;

       if( chiSquareProbW>chiSquareProbZ ) {

         chiSquareProbMax = chiSquareProbW;
         jet0_kinfit = jets_kinfitW.first;
         jet1_kinfit = jets_kinfitW.second;

       } else {

         chiSquareProbMax = chiSquareProbZ;
         jet0_kinfit = jets_kinfitZ.first;
         jet1_kinfit = jets_kinfitZ.second;

       }

       TLorentzVector dijet_kinfit = jet0_kinfit + jet1_kinfit;

       h1_kinfit_chiSquareProbMax->Fill( chiSquareProbMax, eventWeight );
       if( dijet.M()>60. && dijet.M()<120. ) 
         h1_kinfit_chiSquareProbMax_mjjWindow->Fill( chiSquareProbMax, eventWeight );

       
       float zeppen_kinfit = diphot.Eta() - 0.5*( jet0_kinfit.Eta() + jet1_kinfit.Eta() );
       h1_zeppen_kinfit->Fill( zeppen_kinfit, eventWeight );

      



       float qgljet0 = qglikeli->computeQGLikelihoodPU( jet0.Pt(), rhoPF, ntrkjet[indexjet0], nneutjet[indexjet0], ptDjet[indexjet0] );
       float qgljet1 = qglikeli->computeQGLikelihoodPU( jet1.Pt(), rhoPF, ntrkjet[indexjet1], nneutjet[indexjet1], ptDjet[indexjet1] );


       // med-med category for double higgs searches
       // try to be as model independent as possible (no mass or angular cuts)
       // just cut hard on jet pt's
       int btagCategory = -1;
       if( njets_selected_btagmedium==2 && jet0.Pt()>50. && jet1.Pt()>50. && diphot.Pt()>100. ) { 

         btagCategory = 3;

         h1_mgg_2btagmed->Fill( massggnewvtx, eventWeight );



       } else { // SM higgs categories


         btagCategory = (njets_selected_btagloose<=2) ? njets_selected_btagloose : 2;

         if( btagCategory==0 ) {
           if( diphot.Pt() < ptgg_0btag_thresh_ ) continue;
         } else if( btagCategory==1 ) {
           if( diphot.Pt() < ptgg_1btag_thresh_ ) continue;
         } else {
           if( diphot.Pt() < ptgg_2btag_thresh_ ) continue;
         }
         
         

         if( btagCategory==0 ) {
           h1_mjj_0btag->Fill( dijet.M(), eventWeight );
         } else if( btagCategory==1 ) {
           h1_mjj_1btag->Fill( dijet.M(), eventWeight );
         } else {
           h1_mjj_2btag->Fill( dijet.M(), eventWeight );
         }
         
         if( qgljet0>0.8 && qgljet1>0.8 ) h1_mjj_qglHI->Fill( dijet.M(), eventWeight );
         else                             h1_mjj_qglLO->Fill( dijet.M(), eventWeight );

         // old cut logic:
         if( btagCategory==0 ) {
           if( jet0.Pt() < ptjetleadthresh_0btag_ ) continue;
         }

         if( fabs(zeppen)>zeppenfeld_thresh_ ) continue;
         if( dijet.M()<mjj_min_thresh_ || dijet.M()>mjj_max_thresh_ ) continue;

         if( btagCategory==0 ) {
           if( jet1.Pt() < ptjet_0btag_thresh_ ) continue;
           if( fabs(cosThetaStar) > costhetastar_0btag_thresh_ ) continue;
         } else if( btagCategory==1 ) {
           if( jet1.Pt() < ptjet_1btag_thresh_ ) continue;
           if( fabs(cosThetaStar) > costhetastar_1btag_thresh_ ) continue;
         } else {
           if( jet1.Pt() < ptjet_2btag_thresh_ ) continue;
           if( fabs(cosThetaStar) > costhetastar_2btag_thresh_ ) continue;
         }
         


         h1_qgljet0->Fill( qgljet0, eventWeight );
         h1_qgljet1->Fill( qgljet1, eventWeight );
         if( chose_correctPair ) {
           h1_qgljet0_correct->Fill( qgljet0, eventWeight );
           h1_qgljet1_correct->Fill( qgljet1, eventWeight );
         } else {
           h1_qgljet0_incorrect->Fill( qgljet0, eventWeight );
           h1_qgljet1_incorrect->Fill( qgljet1, eventWeight );
         }

         if( btagCategory==0 ) {

           h1_mgg_0btag->Fill( massggnewvtx, eventWeight );
           if( ebeb ) h1_mgg_0btag_ebeb->Fill( massggnewvtx, eventWeight );
           else  h1_mgg_0btag_nebeb->Fill( massggnewvtx, eventWeight );

           h1_qgljet0_0btag->Fill( qgljet0, eventWeight );
           h1_qgljet1_0btag->Fill( qgljet1, eventWeight );

         } else if( btagCategory==1 ) {

           h1_mgg_1btag->Fill( massggnewvtx, eventWeight );
           if( ebeb ) h1_mgg_1btag_ebeb->Fill( massggnewvtx, eventWeight );
           else  h1_mgg_1btag_nebeb->Fill( massggnewvtx, eventWeight );

           // qg only for non-btagged jet:
           if( firstjet_isbtaggedloose ) {
             h1_qgljet_1btag->Fill( qgljet1, eventWeight );
           } else {
             h1_qgljet_1btag->Fill( qgljet0, eventWeight );
           }

         } else {

           h1_mgg_2btag->Fill( massggnewvtx, eventWeight );
           if( ebeb ) h1_mgg_2btag_ebeb->Fill( massggnewvtx, eventWeight );
           else  h1_mgg_2btag_nebeb->Fill( massggnewvtx, eventWeight );

         }


       } // if susy 2 med btag cat
       
       
       //if( njets_selected_btagloose==2 ) {
       //  if( dijet.M()<mjj_min_2btag_thresh_|| dijet.M()>mjj_max_2btag_thresh_ ) continue;
       //} else {
       //  if( dijet.M()<mjj_min_thresh_ || dijet.M()>mjj_max_thresh_ ) continue;
       //}



       h1_ptDiphot->Fill( diphot.Pt(), eventWeight );


       float deltaphi = fabs(dijet.DeltaPhi(diphot));

       h1_deltaPhi->Fill( deltaphi, eventWeight );
       h1_ptDijet->Fill( dijet.Pt(), eventWeight );
       h1_ptRatio->Fill( dijet.Pt()/diphot.Pt(), eventWeight );
       h1_ptDifference->Fill( dijet.Pt()-diphot.Pt(), eventWeight );

       h1_deltaEtaJets->Fill( jet0.Eta()-jet1.Eta(), eventWeight );
       h1_deltaFabsEtaJets->Fill( fabs(jet0.Eta())-fabs(jet1.Eta()), eventWeight );


       float deltaphi_kinfit = fabs(dijet_kinfit.DeltaPhi(diphot));

       h1_deltaPhi_kinfit->Fill( deltaphi_kinfit, eventWeight );
       h1_ptDijet_kinfit->Fill( dijet_kinfit.Pt(), eventWeight );
       h1_ptRatio_kinfit->Fill( dijet_kinfit.Pt()/diphot.Pt(), eventWeight );
       h1_ptDifference_kinfit->Fill( dijet_kinfit.Pt()-diphot.Pt(), eventWeight );

       h1_deltaEtaJets_kinfit->Fill( jet0_kinfit.Eta()-jet1_kinfit.Eta(), eventWeight );
       h1_deltaFabsEtaJets_kinfit->Fill( fabs(jet0_kinfit.Eta())-fabs(jet1_kinfit.Eta()), eventWeight );


       h1_mgg->Fill( massggnewvtx, eventWeight );


       TLorentzVector Vstar = dijet + diphot;
       TLorentzVector Vstar_kinfit = dijet_kinfit + diphot;
       
       h1_mVstar->Fill( Vstar.M(), eventWeight );
       h1_ptVstar->Fill( Vstar.Pt(), eventWeight );
       h1_etaVstar->Fill( Vstar.Eta(), eventWeight );
       h1_phiVstar->Fill( Vstar.Phi(), eventWeight );

       h1_mVstar_kinfit->Fill( Vstar_kinfit.M(), eventWeight );
       h1_ptVstar_kinfit->Fill( Vstar_kinfit.Pt(), eventWeight );
       h1_etaVstar_kinfit->Fill( Vstar_kinfit.Eta(), eventWeight );
       h1_phiVstar_kinfit->Fill( Vstar_kinfit.Phi(), eventWeight );

       //leptons
       h1_ptele1->Fill(ptele1,eventWeight);
       h1_ptele2->Fill(ptele2,eventWeight);
       h1_ptmu1->Fill(ptmu1,eventWeight);
       h1_ptmu2->Fill(ptmu2,eventWeight);

       //MET
       h1_pfMet->Fill(epfMet,eventWeight);
       //Ht
       Ht_t=Ht;

       // set tree vars:
       btagCategory_t = btagCategory;
       njets_t  = njets_selected;
       nbjets_loose_t = njets_selected_btagloose;
       nbjets_medium_t = njets_selected_btagmedium;
       ptPhot1_t = ptphot1;
       ptPhot2_t = ptphot2;
       etaPhot1_t = etaphot1;
       etaPhot2_t = etaphot2;
       mgg_t = diphot.M();
       ptgg_t = diphot.Pt();
       ptJet1_t = jet0.Pt();
       ptJet2_t = jet1.Pt();
       etaJet1_t = jet0.Eta();
       etaJet2_t = jet1.Eta();
       qglJet1_t = qgljet0;
       qglJet2_t = qgljet1;
       btagJet1_t = firstjet_isbtaggedloose;
       btagJet2_t = secondjet_isbtaggedloose;
       mjj_t = dijet.M();
       ptjj_t = dijet.Pt();
       zeppen_t = zeppen;
       chiSquareProbMax_t = chiSquareProbMax;
       absCosThetaStar_t = fabs(cosThetaStar);


       //       std::cout<<Ht<<" "<<ptPhot1_t+ptPhot2_t+ptJet1_t+ptJet2_t+ptJet3_t+ptJet4_t+ptJet5_t+ptJet6_t+ptJet7_t+ptJet8_t+ptJet9_t+ptJet10_t<<std::endl;
       //       std::cout<<ptPhot1_t<<" "<<ptPhot2_t<<" "<<ptJet1_t<<" "<<ptJet2_t<<" "<<ptJet3_t<<" "<<ptJet4_t<<" "<<ptJet5_t<<" "<<ptJet6_t<<" "<<ptJet7_t<<" "<<ptJet8_t<<" "<<ptJet9_t<<" "<<ptJet10_t<<std::endl;
       //       std::cout<<"--------------------size:"<<index_selected.size()<<endl;
       tree_passedEvents->Fill();


   } //for entries


   std::cout << "-> Leading jets: Chose correct jet pair in " << correctPairs_lead/allPairs*100. << "%% of the cases." << std::endl;
   std::cout << "-> Fancy jets: Chose correct jet pair in " << correctPairs_fancy/allPairs*100. << "%% of the cases." << std::endl;

   outFile_->cd();

   tree_passedEvents->Write();

   h1_nvertex->Write();
   h1_nvertex_PUW->Write();

   h1_njets->Write();
   h1_nbjets_loose->Write();
   h1_nbjets_medium->Write();
   h1_nbjets_loose_2jets->Write();
   h1_nbjets_medium_2jets->Write();
   h1_njets_ZbbHgg->Write();
   h1_nbjets_loose_ZbbHgg->Write();
   h1_nbjets_medium_ZbbHgg->Write();
   h1_nbjets_loose_nj2_ZbbHgg->Write();
   h1_nbjets_medium_nj2_ZbbHgg->Write();
   h1_posMatchedJet->Write();
   h1_ptMatchedJet->Write();
   h1_etaMatchedJet->Write();
   h1_phiMatchedJet->Write();

   h1_ptphot0->Write();
   h1_ptphot1->Write();
   h1_etaphot0->Write();
   h1_etaphot1->Write();

   h1_ptjet0->Write();
   h1_ptjet1->Write();
   h1_etajet0->Write();
   h1_etajet1->Write();

   for(int i=0;i<numMaxJets;++i){
     h1_ptOtherJets[i]->Write();
     h1_etaOtherJets[i]->Write();
   }

   h1_pfMet->Write();
   h1_Ht->Write();

   h1_kinfit_chiSquareProbMax->Write();
   h1_kinfit_chiSquareProbMax_mjjWindow->Write();

   h1_mjj->Write();

   h1_mjj_0btag->Write();
   h1_mjj_1btag->Write();
   h1_mjj_2btag->Write();

   h1_mjj_qglHI->Write();
   h1_mjj_qglLO->Write();

   h1_mjj_correct->Write();
   h1_mjj_incorrect->Write();

   h1_ptDiphot->Write();

   h1_deltaPhi->Write();
   h1_ptDijet->Write();
   h1_ptRatio->Write();
   h1_ptDifference->Write();

   h1_deltaEtaJets->Write();
   h1_deltaFabsEtaJets->Write();
   h1_zeppen->Write();

   h1_deltaPhi_kinfit->Write();
   h1_ptDijet_kinfit->Write();
   h1_ptRatio_kinfit->Write();
   h1_ptDifference_kinfit->Write();

   h1_deltaEtaJets_kinfit->Write();
   h1_deltaFabsEtaJets_kinfit->Write();
   h1_zeppen_kinfit->Write();

   h1_mgg_prepresel->Write();
   h1_mgg_presel->Write();
   h1_mgg->Write();
   h1_mgg_0btag->Write();
   h1_mgg_1btag->Write();
   h1_mgg_2btag->Write();
   h1_mgg_2btagmed->Write();

   h1_mgg_0btag_ebeb->Write();
   h1_mgg_1btag_ebeb->Write();
   h1_mgg_2btag_ebeb->Write();

   h1_mgg_0btag_nebeb->Write();
   h1_mgg_1btag_nebeb->Write();
   h1_mgg_2btag_nebeb->Write();

   h1_qgljet0->Write();
   h1_qgljet1->Write();
   h1_qgljet0_0btag->Write();
   h1_qgljet1_0btag->Write();
   h1_qgljet_1btag->Write();

   h1_qgljet0_correct->Write();
   h1_qgljet1_correct->Write();
   h1_qgljet0_incorrect->Write();
   h1_qgljet1_incorrect->Write();

   h1_cosTheta1->Write();
   h1_cosTheta2->Write();
   h1_cosThetaStar->Write();
   h1_helphi->Write();
   h1_helphi1->Write();

   h1_mVstar->Write();
   h1_ptVstar->Write();
   h1_etaVstar->Write();
   h1_phiVstar->Write();

   h1_mVstar_kinfit->Write();
   h1_ptVstar_kinfit->Write();
   h1_etaVstar_kinfit->Write();
   h1_phiVstar_kinfit->Write();

   h1_ptele1->Write();
   h1_ptele2->Write();
   h1_ptmu1->Write();
   h1_ptmu2->Write();

} //finalize


















Int_t RedNtpFinalizer_ttHgg::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!tree_) return 0;
   return tree_->GetEntry(entry);
}
Long64_t RedNtpFinalizer_ttHgg::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!tree_) return -5;
   Long64_t centry = tree_->LoadTree(entry);
   if (centry < 0) return centry;
   if (tree_->GetTreeNumber() != fCurrent) {
      fCurrent = tree_->GetTreeNumber();
   }
   return centry;
}

void RedNtpFinalizer_ttHgg::Init()
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   fCurrent = -1;
   tree_->SetMakeClass(1);

   tree_->SetBranchAddress("run", &run, &b_run);
   tree_->SetBranchAddress("event", &event, &b_event);
   tree_->SetBranchAddress("lumi", &lumi, &b_lumi);
   tree_->SetBranchAddress("H_event", &H_event, &b_H_event);
   tree_->SetBranchAddress("V_event", &V_event, &b_V_event);
   tree_->SetBranchAddress("Vqq_event", &Vqq_event, &b_Vqq_event);
   tree_->SetBranchAddress("Zbb_event", &Zbb_event, &b_Zbb_event);
   tree_->SetBranchAddress("rhoPF", &rhoPF, &b_rhoPF);
   tree_->SetBranchAddress("massgg", &massgg, &b_massgg);
   tree_->SetBranchAddress("ptgg", &ptgg, &b_ptgg);
   tree_->SetBranchAddress("ptggnewvtx", &ptggnewvtx, &b_ptggnewvtx);
   tree_->SetBranchAddress("phigg", &phigg, &b_phigg);
   tree_->SetBranchAddress("etagg", &etagg, &b_etagg);
   tree_->SetBranchAddress("massggnewvtx", &massggnewvtx, &b_massggnewvtx);
   tree_->SetBranchAddress("ptphot1", &ptphot1, &b_ptphot1);
   tree_->SetBranchAddress("ptphot2", &ptphot2, &b_ptphot2);
   tree_->SetBranchAddress("deltaRToTrackphot1", &deltaRToTrackphot1, &b_deltaRToTrackphot1);
   tree_->SetBranchAddress("deltaRToTrackphot2", &deltaRToTrackphot2, &b_deltaRToTrackphot2);
   tree_->SetBranchAddress("timephot1", &timephot1, &b_timephot1);
   tree_->SetBranchAddress("timephot2", &timephot2, &b_timephot2);
   tree_->SetBranchAddress("etaphot1", &etaphot1, &b_etaphot1);
   tree_->SetBranchAddress("etaphot2", &etaphot2, &b_etaphot2);
   tree_->SetBranchAddress("phiphot1", &phiphot1, &b_phiphot1);
   tree_->SetBranchAddress("phiphot2", &phiphot2, &b_phiphot2);
   tree_->SetBranchAddress("etascphot1", &etascphot1, &b_etascphot1);
   tree_->SetBranchAddress("etascphot2", &etascphot2, &b_etascphot2);
   tree_->SetBranchAddress("phiscphot1", &phiscphot1, &b_phiscphot1);
   tree_->SetBranchAddress("phiscphot2", &phiscphot2, &b_phiscphot2);
   tree_->SetBranchAddress("E1phot1", &E1phot1, &b_E1phot1);
   tree_->SetBranchAddress("E1phot2", &E1phot2, &b_E1phot2);
   tree_->SetBranchAddress("E9phot1", &E9phot1, &b_E9phot1);
   tree_->SetBranchAddress("E9phot2", &E9phot2, &b_E9phot2);
   tree_->SetBranchAddress("energyErrphot1", &energyErrphot1, &b_energyErrphot1);
   tree_->SetBranchAddress("energyErrphot2", &energyErrphot2, &b_energyErrphot2);
   tree_->SetBranchAddress("energySmearingphot1", &energySmearingphot1, &b_energySmearingphot1);
   tree_->SetBranchAddress("energySmearingphot2", &energySmearingphot2, &b_energySmearingphot2);
   tree_->SetBranchAddress("r9phot1", &r9phot1, &b_r9phot1);
   tree_->SetBranchAddress("r9phot2", &r9phot2, &b_r9phot2);
   tree_->SetBranchAddress("isemEGphot1", &isemEGphot1, &b_isemEGphot1);
   tree_->SetBranchAddress("isemEGphot2", &isemEGphot2, &b_isemEGphot2);
   tree_->SetBranchAddress("promptGamma", &promptGamma, &b_promptGamma);
   tree_->SetBranchAddress("LOGamma", &LOGamma, &b_LOGamma);
   tree_->SetBranchAddress("ISRGamma", &ISRGamma, &b_ISRGamma);
   tree_->SetBranchAddress("FSRGamma", &FSRGamma, &b_FSRGamma);
   tree_->SetBranchAddress("idmvaphot1", &idmvaphot1, &b_idmvaphot1);
   tree_->SetBranchAddress("idmvaphot2", &idmvaphot2, &b_idmvaphot2);
   tree_->SetBranchAddress("idcicphot1", &idcicphot1, &b_idcicphot1);
   tree_->SetBranchAddress("idcicphot2", &idcicphot2, &b_idcicphot2);
   tree_->SetBranchAddress("idcicnoelvetophot1", &idcicnoelvetophot1, &b_idcicnoelvetophot1);
   tree_->SetBranchAddress("idcicnoelvetophot2", &idcicnoelvetophot2, &b_idcicnoelvetophot2);
   tree_->SetBranchAddress("idcicpfphot1", &idcicpfphot1, &b_idcicpfphot1);
   tree_->SetBranchAddress("idcicpfphot2", &idcicpfphot2, &b_idcicpfphot2);
   tree_->SetBranchAddress("idcicpfnoelvetophot1", &idcicpfnoelvetophot1, &b_idcicpfnoelvetophot1);
   tree_->SetBranchAddress("idcicpfnoelvetophot2", &idcicpfnoelvetophot2, &b_idcicpfnoelvetophot2);
   tree_->SetBranchAddress("idelephot1", &idelephot1, &b_idelephot1);
   tree_->SetBranchAddress("idelephot2", &idelephot2, &b_idelephot2);
   tree_->SetBranchAddress("pid_isEMphot1", &pid_isEMphot1, &b_pid_isEMphot1);
   tree_->SetBranchAddress("pid_isEMphot2", &pid_isEMphot2, &b_pid_isEMphot2);
   tree_->SetBranchAddress("pid_haspixelseedphot1", &pid_haspixelseedphot1, &b_pid_haspixelseedphot1);
   tree_->SetBranchAddress("pid_haspixelseedphot2", &pid_haspixelseedphot2, &b_pid_haspixelseedphot2);
   tree_->SetBranchAddress("pid_jurECALphot1", &pid_jurECALphot1, &b_pid_jurECALphot1);
   tree_->SetBranchAddress("pid_jurECALphot2", &pid_jurECALphot2, &b_pid_jurECALphot2);
   tree_->SetBranchAddress("pid_twrHCALphot1", &pid_twrHCALphot1, &b_pid_twrHCALphot1);
   tree_->SetBranchAddress("pid_twrHCALphot2", &pid_twrHCALphot2, &b_pid_twrHCALphot2);
   tree_->SetBranchAddress("pid_HoverEphot1", &pid_HoverEphot1, &b_pid_HoverEphot1);
   tree_->SetBranchAddress("pid_HoverEphot2", &pid_HoverEphot2, &b_pid_HoverEphot2);
   tree_->SetBranchAddress("pid_hlwTrackphot1", &pid_hlwTrackphot1, &b_pid_hlwTrackphot1);
   tree_->SetBranchAddress("pid_hlwTrackphot2", &pid_hlwTrackphot2, &b_pid_hlwTrackphot2);
   tree_->SetBranchAddress("pid_etawidphot1", &pid_etawidphot1, &b_pid_etawidphot1);
   tree_->SetBranchAddress("pid_etawidphot2", &pid_etawidphot2, &b_pid_etawidphot2);
   tree_->SetBranchAddress("pid_hlwTrackNoDzphot1", &pid_hlwTrackNoDzphot1, &b_pid_hlwTrackNoDzphot1);
   tree_->SetBranchAddress("pid_hlwTrackNoDzphot2", &pid_hlwTrackNoDzphot2, &b_pid_hlwTrackNoDzphot2);
   tree_->SetBranchAddress("pid_hasMatchedConvphot1", &pid_hasMatchedConvphot1, &b_pid_hasMatchedConvphot1);
   tree_->SetBranchAddress("pid_hasMatchedConvphot2", &pid_hasMatchedConvphot2, &b_pid_hasMatchedConvphot2);
   tree_->SetBranchAddress("pid_hasMatchedPromptElephot1", &pid_hasMatchedPromptElephot1, &b_pid_hasMatchedPromptElephot1);
   tree_->SetBranchAddress("pid_hasMatchedPromptElephot2", &pid_hasMatchedPromptElephot2, &b_pid_hasMatchedPromptElephot2);
   tree_->SetBranchAddress("pid_sminphot1", &pid_sminphot1, &b_pid_sminphot1);
   tree_->SetBranchAddress("pid_sminphot2", &pid_sminphot2, &b_pid_sminphot2);
   tree_->SetBranchAddress("pid_smajphot1", &pid_smajphot1, &b_pid_smajphot1);
   tree_->SetBranchAddress("pid_smajphot2", &pid_smajphot2, &b_pid_smajphot2);
   tree_->SetBranchAddress("pid_ntrkphot1", &pid_ntrkphot1, &b_pid_ntrkphot1);
   tree_->SetBranchAddress("pid_ntrkphot2", &pid_ntrkphot2, &b_pid_ntrkphot2);
   tree_->SetBranchAddress("pid_ptisophot1", &pid_ptisophot1, &b_pid_ptisophot1);
   tree_->SetBranchAddress("pid_ptisophot2", &pid_ptisophot2, &b_pid_ptisophot2);
   tree_->SetBranchAddress("pid_ntrkcsphot1", &pid_ntrkcsphot1, &b_pid_ntrkcsphot1);
   tree_->SetBranchAddress("pid_ntrkcsphot2", &pid_ntrkcsphot2, &b_pid_ntrkcsphot2);
   tree_->SetBranchAddress("pid_ptisocsphot1", &pid_ptisocsphot1, &b_pid_ptisocsphot1);
   tree_->SetBranchAddress("pid_ptisocsphot2", &pid_ptisocsphot2, &b_pid_ptisocsphot2);
   tree_->SetBranchAddress("pid_ecalisophot1", &pid_ecalisophot1, &b_pid_ecalisophot1);
   tree_->SetBranchAddress("pid_ecalisophot2", &pid_ecalisophot2, &b_pid_ecalisophot2);
   tree_->SetBranchAddress("pid_hcalisophot1", &pid_hcalisophot1, &b_pid_hcalisophot1);
   tree_->SetBranchAddress("pid_hcalisophot2", &pid_hcalisophot2, &b_pid_hcalisophot2);
   tree_->SetBranchAddress("njets", &njets, &b_njets);
   tree_->SetBranchAddress("ptjet", ptjet, &b_ptjet);
   tree_->SetBranchAddress("ptcorrjet", ptcorrjet, &b_ptcorrjet);
   tree_->SetBranchAddress("ecorrjet", ecorrjet, &b_ecorrjet);
   tree_->SetBranchAddress("etajet", etajet, &b_etajet);
   tree_->SetBranchAddress("phijet", phijet, &b_phijet);
   tree_->SetBranchAddress("betajet", betajet, &b_betajet);
   tree_->SetBranchAddress("betastarjet", betastarjet, &b_betastarjet);
   tree_->SetBranchAddress("btagvtxjet", btagvtxjet, &b_btagvtxjet);
   tree_->SetBranchAddress("btagtrkjet", btagtrkjet, &b_btagtrkjet);
   tree_->SetBranchAddress("btagjprobjet", btagjprobjet, &b_btagjprobjet);
   tree_->SetBranchAddress("ptDjet", ptDjet, &b_ptDjet);
   tree_->SetBranchAddress("rmsjet", rmsjet, &b_rmsjet);
   tree_->SetBranchAddress("ntrkjet", ntrkjet, &b_ntrkjet);
   tree_->SetBranchAddress("nneutjet", nneutjet, &b_nneutjet);
   tree_->SetBranchAddress("jetIdSimple_mvajet", jetIdSimple_mvajet, &b_jetIdSimple_mvajet);
   tree_->SetBranchAddress("jetIdFull_mvajet", jetIdFull_mvajet, &b_jetIdFull_mvajet);
   tree_->SetBranchAddress("jetId_dR2Meanjet", jetId_dR2Meanjet, &b_jetId_dR2Meanjet);
   tree_->SetBranchAddress("jetId_betaStarClassicjet", jetId_betaStarClassicjet, &b_jetId_betaStarClassicjet);
   tree_->SetBranchAddress("jetId_frac01jet", jetId_frac01jet, &b_jetId_frac01jet);
   tree_->SetBranchAddress("jetId_frac02jet", jetId_frac02jet, &b_jetId_frac02jet);
   tree_->SetBranchAddress("jetId_frac03jet", jetId_frac03jet, &b_jetId_frac03jet);
   tree_->SetBranchAddress("jetId_frac04jet", jetId_frac04jet, &b_jetId_frac04jet);
   tree_->SetBranchAddress("jetId_frac05jet", jetId_frac05jet, &b_jetId_frac05jet);
   tree_->SetBranchAddress("jetId_betajet", jetId_betajet, &b_jetId_betajet);
   tree_->SetBranchAddress("jetId_betaStarjet", jetId_betaStarjet, &b_jetId_betaStarjet);
   tree_->SetBranchAddress("jetIdCutBased_wpjet", jetIdCutBased_wpjet, &b_jetIdCutBased_wpjet);
   tree_->SetBranchAddress("jetIdSimple_wpjet", jetIdSimple_wpjet, &b_jetIdSimple_wpjet);
   tree_->SetBranchAddress("jetIdFull_wpjet", jetIdFull_wpjet, &b_jetIdFull_wpjet);
   tree_->SetBranchAddress("assjet", assjet, &b_assjet);
   tree_->SetBranchAddress("partPdgIDjet", partPdgIDjet, &b_partPdgIDjet);
   tree_->SetBranchAddress("partMomPdgIDjet", partMomPdgIDjet, &b_partMomPdgIDjet);
   tree_->SetBranchAddress("deltaeta", &deltaeta, &b_deltaeta);
   tree_->SetBranchAddress("zeppenjet", &zeppenjet, &b_zeppenjet);
   tree_->SetBranchAddress("deltaphi", &deltaphi, &b_deltaphi);
   tree_->SetBranchAddress("deltaphinewvtx", &deltaphinewvtx, &b_deltaphinewvtx);
   tree_->SetBranchAddress("deltaphigg", &deltaphigg, &b_deltaphigg);
   tree_->SetBranchAddress("invmassjet", &invmassjet, &b_invmassjet);
   tree_->SetBranchAddress("invmass2g1j", &invmass2g1j, &b_invmass2g1j);
   tree_->SetBranchAddress("invmass2g2j", &invmass2g2j, &b_invmass2g2j);
   tree_->SetBranchAddress("pt2g2j", &pt2g2j, &b_pt2g2j);
   tree_->SetBranchAddress("eta2j", &eta2j, &b_eta2j);
   tree_->SetBranchAddress("phi2j", &phi2j, &b_phi2j);
   tree_->SetBranchAddress("pt2j", &pt2j, &b_pt2j);
   tree_->SetBranchAddress("nvtx", &nvtx, &b_nvtx);
   tree_->SetBranchAddress("vtxId", &vtxId, &b_vtxId);
   tree_->SetBranchAddress("vtxPos_x", &vtxPos_x, &b_vtxPos_x);
   tree_->SetBranchAddress("vtxPos_y", &vtxPos_y, &b_vtxPos_y);
   tree_->SetBranchAddress("vtxPos_z", &vtxPos_z, &b_vtxPos_z);
   tree_->SetBranchAddress("vtxIdMVA", &vtxIdMVA, &b_vtxIdMVA);
   tree_->SetBranchAddress("vtxIdEvtProb", &vtxIdEvtProb, &b_vtxIdEvtProb);
   tree_->SetBranchAddress("sMet", &sMet, &b_sMet);
   tree_->SetBranchAddress("eMet", &eMet, &b_eMet);
   tree_->SetBranchAddress("phiMet", &phiMet, &b_phiMet);
   tree_->SetBranchAddress("signifMet", &signifMet, &b_signifMet);
   tree_->SetBranchAddress("eSmearedMet", &eSmearedMet, &b_eSmearedMet);
   tree_->SetBranchAddress("phiSmearedMet", &phiSmearedMet, &b_phiSmearedMet);
   tree_->SetBranchAddress("eShiftedMet", &eShiftedMet, &b_eShiftedMet);
   tree_->SetBranchAddress("phiShiftedMet", &phiShiftedMet, &b_phiShiftedMet);
   tree_->SetBranchAddress("eShiftedScaledMet", &eShiftedScaledMet, &b_eShiftedScaledMet);
   tree_->SetBranchAddress("phiShiftedScaledMet", &phiShiftedScaledMet, &b_phiShiftedScaledMet);
   tree_->SetBranchAddress("eSmearedShiftedMet", &eSmearedShiftedMet, &b_eSmearedShiftedMet);
   tree_->SetBranchAddress("phiSmearedShiftedMet", &phiSmearedShiftedMet, &b_phiSmearedShiftedMet);
   tree_->SetBranchAddress("eShiftedScaledMetPUcorr", &eShiftedScaledMetPUcorr, &b_eShiftedScaledMetPUcorr);
   tree_->SetBranchAddress("phiShiftedScaledMetPUcorr", &phiShiftedScaledMetPUcorr, &b_phiShiftedScaledMetPUcorr);
   tree_->SetBranchAddress("eSmearedShiftedMePUcorrt", &eSmearedShiftedMePUcorrt, &b_eSmearedShiftedMetPUcorr);
   tree_->SetBranchAddress("phiSmearedShiftedMetPUcorr", &phiSmearedShiftedMetPUcorr, &b_phiSmearedShiftedMetPUcorr);
   tree_->SetBranchAddress("sCorrMet", &sCorrMet, &b_sCorrMet);
   tree_->SetBranchAddress("eCorrMet", &eCorrMet, &b_eCorrMet);
   tree_->SetBranchAddress("phiCorrMet", &phiCorrMet, &b_phiCorrMet);
   tree_->SetBranchAddress("signifCorrMet", &signifCorrMet, &b_signifCorrMet);
   tree_->SetBranchAddress("smuCorrMet", &smuCorrMet, &b_smuCorrMet);
   tree_->SetBranchAddress("emuCorrMet", &emuCorrMet, &b_emuCorrMet);
   tree_->SetBranchAddress("phimuCorrMet", &phimuCorrMet, &b_phimuCorrMet);
   tree_->SetBranchAddress("signifmuCorrMet", &signifmuCorrMet, &b_signifmuCorrMet);
   tree_->SetBranchAddress("sNoHFMet", &sNoHFMet, &b_sNoHFMet);
   tree_->SetBranchAddress("eNoHFMet", &eNoHFMet, &b_eNoHFMet);
   tree_->SetBranchAddress("phiNoHFMet", &phiNoHFMet, &b_phiNoHFMet);
   tree_->SetBranchAddress("signifNoHFMet", &signifNoHFMet, &b_signifNoHFMet);
   tree_->SetBranchAddress("stcMet", &stcMet, &b_stcMet);
   tree_->SetBranchAddress("etcMet", &etcMet, &b_etcMet);
   tree_->SetBranchAddress("phitcMet", &phitcMet, &b_phitcMet);
   tree_->SetBranchAddress("signiftcMet", &signiftcMet, &b_signiftcMet);
   tree_->SetBranchAddress("sglobalPfMet", &sglobalPfMet, &b_sglobalPfMet);
   tree_->SetBranchAddress("eglobalPfMet", &eglobalPfMet, &b_eglobalPfMet);
   tree_->SetBranchAddress("phiglobalPfMet", &phiglobalPfMet, &b_phiglobalPfMet);
   tree_->SetBranchAddress("signifglobalPfMet", &signifglobalPfMet, &b_signifglobalPfMet);
   tree_->SetBranchAddress("scentralPfMet", &scentralPfMet, &b_scentralPfMet);
   tree_->SetBranchAddress("ecentralPfMet", &ecentralPfMet, &b_ecentralPfMet);
   tree_->SetBranchAddress("phicentralPfMet", &phicentralPfMet, &b_phicentralPfMet);
   tree_->SetBranchAddress("signifcentralPfMet", &signifcentralPfMet, &b_signifcentralPfMet);
   tree_->SetBranchAddress("eassocPfMet", &eassocPfMet, &b_eassocPfMet);
   tree_->SetBranchAddress("phiassocPfMet", &phiassocPfMet, &b_phiassocPfMet);
   tree_->SetBranchAddress("signifassocPfMet", &signifassocPfMet, &b_signifassocPfMet);
   tree_->SetBranchAddress("eassocOtherVtxPfMet", &eassocOtherVtxPfMet, &b_eassocOtherVtxPfMet);
   tree_->SetBranchAddress("phiassocOtherVtxPfMet", &phiassocOtherVtxPfMet, &b_phiassocOtherVtxPfMet);
   tree_->SetBranchAddress("signifassocOtherVtxPfMet", &signifassocOtherVtxPfMet, &b_signifassocOtherVtxPfMet);
   tree_->SetBranchAddress("etrkPfMet", &etrkPfMet, &b_etrkPfMet);
   tree_->SetBranchAddress("phitrkPfMet", &phitrkPfMet, &b_phitrkPfMet);
   tree_->SetBranchAddress("signiftrkPfMet", &signiftrkPfMet, &b_signiftrkPfMet);
   tree_->SetBranchAddress("ecleanPfMet", &ecleanPfMet, &b_ecleanPfMet);
   tree_->SetBranchAddress("phicleanPfMet", &phicleanPfMet, &b_phicleanPfMet);
   tree_->SetBranchAddress("signifcleanPfMet", &signifcleanPfMet, &b_signifcleanPfMet);
   tree_->SetBranchAddress("ecleanedSaclayPfMet", &ecleanedSaclayPfMet, &b_ecleanedSaclayPfMet);
   tree_->SetBranchAddress("phicleanedSaclayPfMet", &phicleanedSaclayPfMet, &b_phicleanedSaclayPfMet);
   tree_->SetBranchAddress("signifcleanedSaclayPfMet", &signifcleanedSaclayPfMet, &b_signifcleanedSaclayPfMet);
   tree_->SetBranchAddress("eminTypeICleanSaclayPfMet", &eminTypeICleanSaclayPfMet, &b_eminTypeICleanSaclayPfMet);
   tree_->SetBranchAddress("phiminTypeICleanSaclayPfMet", &phiminTypeICleanSaclayPfMet, &b_phiminTypeICleanSaclayPfMet);
   tree_->SetBranchAddress("signifminTypeICleanSaclayPfMet", &signifminTypeICleanSaclayPfMet, &b_signifminTypeICleanSaclayPfMet);
   tree_->SetBranchAddress("globalPfSums", &globalPfSums, &b_globalPfSums);
   tree_->SetBranchAddress("spfMet", &spfMet, &b_spfMet);
   tree_->SetBranchAddress("epfMet", &epfMet, &b_epfMet);
   tree_->SetBranchAddress("phipfMet", &phipfMet, &b_phipfMet);
   tree_->SetBranchAddress("signifpfMet", &signifpfMet, &b_signifpfMet);
   tree_->SetBranchAddress("spfMetType1", &spfMetType1, &b_spfMetType1);
   tree_->SetBranchAddress("epfMetType1", &epfMetType1, &b_epfMetType1);
   tree_->SetBranchAddress("phipfMetType1", &phipfMetType1, &b_phipfMetType1);
   tree_->SetBranchAddress("signifpfMetType1", &signifpfMetType1, &b_signifpfMetType1);
   tree_->SetBranchAddress("sMetGen", &sMetGen, &b_sMetGen);
   tree_->SetBranchAddress("eMetGen", &eMetGen, &b_eMetGen);
   tree_->SetBranchAddress("phiMetGen", &phiMetGen, &b_phiMetGen);
   tree_->SetBranchAddress("signifMetGen", &signifMetGen, &b_signifMetGen);
   tree_->SetBranchAddress("sMetGen2", &sMetGen2, &b_sMetGen2);
   tree_->SetBranchAddress("eMetGen2", &eMetGen2, &b_eMetGen2);
   tree_->SetBranchAddress("phiMetGen2", &phiMetGen2, &b_phiMetGen2);
   tree_->SetBranchAddress("npu", &npu, &b_npu);
   tree_->SetBranchAddress("NtotEvents", &NtotEvents, &b_NtotEvents);
   tree_->SetBranchAddress("xsection", &xsection, &b_xsection);
   tree_->SetBranchAddress("EquivLumi", &EquivLumi, &b_EquivLumi);
   tree_->SetBranchAddress("SampleID", &SampleID, &b_SampleID);
   tree_->SetBranchAddress("pu_weight", &pu_weight, &b_pu_weight);
   tree_->SetBranchAddress("pt_weight", &pt_weight, &b_pt_weight);
   tree_->SetBranchAddress("gen_custom_processId", &gen_custom_processId, &b_gen_custom_processId);
   tree_->SetBranchAddress("gen_pt_gamma1", &gen_pt_gamma1, &b_gen_pt_gamma1);
   tree_->SetBranchAddress("gen_pt_gamma2", &gen_pt_gamma2, &b_gen_pt_gamma2);
   tree_->SetBranchAddress("gen_eta_gamma1", &gen_eta_gamma1, &b_gen_eta_gamma1);
   tree_->SetBranchAddress("gen_eta_gamma2", &gen_eta_gamma2, &b_gen_eta_gamma2);
   tree_->SetBranchAddress("gen_phi_gamma1", &gen_phi_gamma1, &b_gen_phi_gamma1);
   tree_->SetBranchAddress("gen_phi_gamma2", &gen_phi_gamma2, &b_gen_phi_gamma2);
   tree_->SetBranchAddress("gen_pt_genjet1", &gen_pt_genjet1, &b_gen_pt_genjet1);
   tree_->SetBranchAddress("gen_pt_genjet2", &gen_pt_genjet2, &b_gen_pt_genjet2);
   tree_->SetBranchAddress("gen_eta_genjet1", &gen_eta_genjet1, &b_gen_eta_genjet1);
   tree_->SetBranchAddress("gen_eta_genjet2", &gen_eta_genjet2, &b_gen_eta_genjet2);
   tree_->SetBranchAddress("gen_phi_genjet1", &gen_phi_genjet1, &b_gen_phi_genjet1);
   tree_->SetBranchAddress("gen_phi_genjet2", &gen_phi_genjet2, &b_gen_phi_genjet2);
   tree_->SetBranchAddress("gen_mass_diphoton", &gen_mass_diphoton, &b_gen_mass_diphoton);
   tree_->SetBranchAddress("gen_pt_diphoton", &gen_pt_diphoton, &b_gen_pt_diphoton);
   tree_->SetBranchAddress("gen_eta_diphoton", &gen_eta_diphoton, &b_gen_eta_diphoton);
   tree_->SetBranchAddress("gen_phi_diphoton", &gen_phi_diphoton, &b_gen_phi_diphoton);
   tree_->SetBranchAddress("gen_mass_dijet", &gen_mass_dijet, &b_gen_mass_dijet);
   tree_->SetBranchAddress("gen_pt_dijet", &gen_pt_dijet, &b_gen_pt_dijet);
   tree_->SetBranchAddress("gen_eta_dijet", &gen_eta_dijet, &b_gen_eta_dijet);
   tree_->SetBranchAddress("gen_phi_dijet", &gen_phi_dijet, &b_gen_phi_dijet);
   tree_->SetBranchAddress("gen_zeppenfeld", &gen_zeppenfeld, &b_gen_zeppenfeld);
   tree_->SetBranchAddress("gen_pt_lep1", &gen_pt_lep1, &b_gen_pt_lep1);
   tree_->SetBranchAddress("gen_pt_lep2", &gen_pt_lep2, &b_gen_pt_lep2);
   tree_->SetBranchAddress("gen_eta_lep1", &gen_eta_lep1, &b_gen_eta_lep1);
   tree_->SetBranchAddress("gen_eta_lep2", &gen_eta_lep2, &b_gen_eta_lep2);
   tree_->SetBranchAddress("gen_phi_lep1", &gen_phi_lep1, &b_gen_phi_lep1);
   tree_->SetBranchAddress("gen_phi_lep2", &gen_phi_lep2, &b_gen_phi_lep2);
   tree_->SetBranchAddress("gen_pid_lep1", &gen_pid_lep1, &b_gen_pid_lep1);
   tree_->SetBranchAddress("gen_pid_lep2", &gen_pid_lep2, &b_gen_pid_lep2);
   tree_->SetBranchAddress("ptele1", &ptele1, &b_ptele1);
   tree_->SetBranchAddress("ptele2", &ptele2, &b_ptele2);
   tree_->SetBranchAddress("etaele1", &etaele1, &b_etaele1);
   tree_->SetBranchAddress("etaele2", &etaele2, &b_etaele2);
   tree_->SetBranchAddress("phiele1", &phiele1, &b_phiele1);
   tree_->SetBranchAddress("phiele2", &phiele2, &b_phiele2);
   tree_->SetBranchAddress("eneele1", &eneele1, &b_eneele1);
   tree_->SetBranchAddress("eneele2", &eneele2, &b_eneele2);
   tree_->SetBranchAddress("sIeIeele1", &sIeIeele1, &b_sIeIeele1);
   tree_->SetBranchAddress("sIeIeele2", &sIeIeele2, &b_sIeIeele2);
   tree_->SetBranchAddress("dphiele1", &dphiele1, &b_dphiele1);
   tree_->SetBranchAddress("dphiele2", &dphiele2, &b_dphiele2);
   tree_->SetBranchAddress("detaele1", &detaele1, &b_detaele1);
   tree_->SetBranchAddress("detaele2", &detaele2, &b_detaele2);
   tree_->SetBranchAddress("mhitsele1", &mhitsele1, &b_mhitsele1);
   tree_->SetBranchAddress("mhitsele2", &mhitsele2, &b_mhitsele2);
   tree_->SetBranchAddress("d0ele1", &d0ele1, &b_d0ele1);
   tree_->SetBranchAddress("d0ele2", &d0ele2, &b_d0ele2);
   tree_->SetBranchAddress("dzele1", &dzele1, &b_dzele1);
   tree_->SetBranchAddress("dzele2", &dzele2, &b_dzele2);
   tree_->SetBranchAddress("invMassele1g1", &invMassele1g1, &b_invMassele1g1);
   tree_->SetBranchAddress("invMassele1g2", &invMassele1g2, &b_invMassele1g2);
   tree_->SetBranchAddress("invMassele2g1", &invMassele2g1, &b_invMassele2g1);
   tree_->SetBranchAddress("invMassele2g2", &invMassele2g2, &b_invMassele2g2);
   tree_->SetBranchAddress("oEmoPele1", &oEmoPele1, &b_oEmoPele1);
   tree_->SetBranchAddress("oEmoPele2", &oEmoPele2, &b_oEmoPele2);
   tree_->SetBranchAddress("mvanotrigele1", &mvanotrigele1, &b_mvanotrigele1);
   tree_->SetBranchAddress("mvanotrigele2", &mvanotrigele2, &b_mvanotrigele2);
   tree_->SetBranchAddress("mvatrigele1", &mvatrigele1, &b_mvatrigele1);
   tree_->SetBranchAddress("mvatrigele2", &mvatrigele2, &b_mvatrigele2);
   tree_->SetBranchAddress("matchconvele1", &matchconvele1, &b_matchconvele1);
   tree_->SetBranchAddress("matchconvele2", &matchconvele2, &b_matchconvele2);
   tree_->SetBranchAddress("chHadIso03ele1", &chHadIso03ele1, &b_chHadIso03ele1);
   tree_->SetBranchAddress("chHadIso03ele2", &chHadIso03ele2, &b_chHadIso03ele2);
   tree_->SetBranchAddress("nHadIso03ele1", &nHadIso03ele1, &b_nHadIso03ele1);
   tree_->SetBranchAddress("nHadIso03ele2", &nHadIso03ele2, &b_nHadIso03ele2);
   tree_->SetBranchAddress("photIso03ele1", &photIso03ele1, &b_photIso03ele1);
   tree_->SetBranchAddress("photIso03ele2", &photIso03ele2, &b_photIso03ele2);
   tree_->SetBranchAddress("pteleloose1", &pteleloose1, &b_pteleloose1);
   tree_->SetBranchAddress("pteleloose2", &pteleloose2, &b_pteleloose2);
   tree_->SetBranchAddress("etaeleloose1", &etaeleloose1, &b_etaeleloose1);
   tree_->SetBranchAddress("etaeleloose2", &etaeleloose2, &b_etaeleloose2);
   tree_->SetBranchAddress("phieleloose1", &phieleloose1, &b_phieleloose1);
   tree_->SetBranchAddress("phieleloose2", &phieleloose2, &b_phieleloose2);
   tree_->SetBranchAddress("eneeleloose1", &eneeleloose1, &b_eneeleloose1);
   tree_->SetBranchAddress("eneeleloose2", &eneeleloose2, &b_eneeleloose2);
   tree_->SetBranchAddress("sIeIeeleloose1", &sIeIeeleloose1, &b_sIeIeeleloose1);
   tree_->SetBranchAddress("sIeIeeleloose2", &sIeIeeleloose2, &b_sIeIeeleloose2);
   tree_->SetBranchAddress("dphieleloose1", &dphieleloose1, &b_dphieleloose1);
   tree_->SetBranchAddress("dphieleloose2", &dphieleloose2, &b_dphieleloose2);
   tree_->SetBranchAddress("detaeleloose1", &detaeleloose1, &b_detaeleloose1);
   tree_->SetBranchAddress("detaeleloose2", &detaeleloose2, &b_detaeleloose2);
   tree_->SetBranchAddress("mhitseleloose1", &mhitseleloose1, &b_mhitseleloose1);
   tree_->SetBranchAddress("mhitseleloose2", &mhitseleloose2, &b_mhitseleloose2);
   tree_->SetBranchAddress("d0eleloose1", &d0eleloose1, &b_d0eleloose1);
   tree_->SetBranchAddress("d0eleloose2", &d0eleloose2, &b_d0eleloose2);
   tree_->SetBranchAddress("dzeleloose1", &dzeleloose1, &b_dzeleloose1);
   tree_->SetBranchAddress("dzeleloose2", &dzeleloose2, &b_dzeleloose2);
   tree_->SetBranchAddress("invMasseleloose1g1", &invMasseleloose1g1, &b_invMasseleloose1g1);
   tree_->SetBranchAddress("invMasseleloose1g2", &invMasseleloose1g2, &b_invMasseleloose1g2);
   tree_->SetBranchAddress("invMasseleloose2g1", &invMasseleloose2g1, &b_invMasseleloose2g1);
   tree_->SetBranchAddress("invMasseleloose2g2", &invMasseleloose2g2, &b_invMasseleloose2g2);
   tree_->SetBranchAddress("oEmoPeleloose1", &oEmoPeleloose1, &b_oEmoPeleloose1);
   tree_->SetBranchAddress("oEmoPeleloose2", &oEmoPeleloose2, &b_oEmoPeleloose2);
   tree_->SetBranchAddress("mvanotrigeleloose1", &mvanotrigeleloose1, &b_mvanotrigeleloose1);
   tree_->SetBranchAddress("mvanotrigeleloose2", &mvanotrigeleloose2, &b_mvanotrigeleloose2);
   tree_->SetBranchAddress("mvatrigeleloose1", &mvatrigeleloose1, &b_mvatrigeleloose1);
   tree_->SetBranchAddress("mvatrigeleloose2", &mvatrigeleloose2, &b_mvatrigeleloose2);
   tree_->SetBranchAddress("matchconveleloose1", &matchconveleloose1, &b_matchconveleloose1);
   tree_->SetBranchAddress("matchconveleloose2", &matchconveleloose2, &b_matchconveleloose2);
   tree_->SetBranchAddress("chHadIso03eleloose1", &chHadIso03eleloose1, &b_chHadIso03eleloose1);
   tree_->SetBranchAddress("chHadIso03eleloose2", &chHadIso03eleloose2, &b_chHadIso03eleloose2);
   tree_->SetBranchAddress("nHadIso03eleloose1", &nHadIso03eleloose1, &b_nHadIso03eleloose1);
   tree_->SetBranchAddress("nHadIso03eleloose2", &nHadIso03eleloose2, &b_nHadIso03eleloose2);
   tree_->SetBranchAddress("photIso03eleloose1", &photIso03eleloose1, &b_photIso03eleloose1);
   tree_->SetBranchAddress("photIso03eleloose2", &photIso03eleloose2, &b_photIso03eleloose2);
   tree_->SetBranchAddress("ptmu1", &ptmu1, &b_ptmu1);
   tree_->SetBranchAddress("ptmu2", &ptmu2, &b_ptmu2);
   tree_->SetBranchAddress("etamu1", &etamu1, &b_etamu1);
   tree_->SetBranchAddress("etamu2", &etamu2, &b_etamu2);
   tree_->SetBranchAddress("phimu1", &phimu1, &b_phimu1);
   tree_->SetBranchAddress("phimu2", &phimu2, &b_phimu2);
   tree_->SetBranchAddress("enemu1", &enemu1, &b_enemu1);
   tree_->SetBranchAddress("enemu2", &enemu2, &b_enemu2);
   tree_->SetBranchAddress("pixhitsmu1", &pixhitsmu1, &b_pixhitsmu1);
   tree_->SetBranchAddress("pixhitsmu2", &pixhitsmu2, &b_pixhitsmu2);
   tree_->SetBranchAddress("trkhitsmu1", &trkhitsmu1, &b_trkhitsmu1);
   tree_->SetBranchAddress("trkhitsmu2", &trkhitsmu2, &b_trkhitsmu2);
   tree_->SetBranchAddress("hitsmu1", &hitsmu1, &b_hitsmu1);
   tree_->SetBranchAddress("hitsmu2", &hitsmu2, &b_hitsmu2);
   tree_->SetBranchAddress("chi2mu1", &chi2mu1, &b_chi2mu1);
   tree_->SetBranchAddress("chi2mu2", &chi2mu2, &b_chi2mu2);
   tree_->SetBranchAddress("matchmu1", &matchmu1, &b_matchmu1);
   tree_->SetBranchAddress("matchmu2", &matchmu2, &b_matchmu2);
   tree_->SetBranchAddress("d0mu1", &d0mu1, &b_d0mu1);
   tree_->SetBranchAddress("d0mu2", &d0mu2, &b_d0mu2);
   tree_->SetBranchAddress("dzmu1", &dzmu1, &b_dzmu1);
   tree_->SetBranchAddress("dzmu2", &dzmu2, &b_dzmu2);
   tree_->SetBranchAddress("chHadmu1", &chHadmu1, &b_chHadmu1);
   tree_->SetBranchAddress("chHadmu2", &chHadmu2, &b_chHadmu2);
   tree_->SetBranchAddress("nHadmu1", &nHadmu1, &b_nHadmu1);
   tree_->SetBranchAddress("nHadmu2", &nHadmu2, &b_nHadmu2);
   tree_->SetBranchAddress("photmu1", &photmu1, &b_photmu1);
   tree_->SetBranchAddress("photmu2", &photmu2, &b_photmu2);
   tree_->SetBranchAddress("puptmu1", &puptmu1, &b_puptmu1);
   tree_->SetBranchAddress("puptmu2", &puptmu2, &b_puptmu2);
   tree_->SetBranchAddress("ptmuloose1", &ptmuloose1, &b_ptmuloose1);
   tree_->SetBranchAddress("ptmuloose2", &ptmuloose2, &b_ptmuloose2);
   tree_->SetBranchAddress("etamuloose1", &etamuloose1, &b_etamuloose1);
   tree_->SetBranchAddress("etamuloose2", &etamuloose2, &b_etamuloose2);
   tree_->SetBranchAddress("phimuloose1", &phimuloose1, &b_phimuloose1);
   tree_->SetBranchAddress("phimuloose2", &phimuloose2, &b_phimuloose2);
   tree_->SetBranchAddress("enemuloose1", &enemuloose1, &b_enemuloose1);
   tree_->SetBranchAddress("enemuloose2", &enemuloose2, &b_enemuloose2);
   tree_->SetBranchAddress("pixhitsmuloose1", &pixhitsmuloose1, &b_pixhitsmuloose1);
   tree_->SetBranchAddress("pixhitsmuloose2", &pixhitsmuloose2, &b_pixhitsmuloose2);
   tree_->SetBranchAddress("trkhitsmuloose1", &trkhitsmuloose1, &b_trkhitsmuloose1);
   tree_->SetBranchAddress("trkhitsmuloose2", &trkhitsmuloose2, &b_trkhitsmuloose2);
   tree_->SetBranchAddress("hitsmuloose1", &hitsmuloose1, &b_hitsmuloose1);
   tree_->SetBranchAddress("hitsmuloose2", &hitsmuloose2, &b_hitsmuloose2);
   tree_->SetBranchAddress("chi2muloose1", &chi2muloose1, &b_chi2muloose1);
   tree_->SetBranchAddress("chi2muloose2", &chi2muloose2, &b_chi2muloose2);
   tree_->SetBranchAddress("matchmuloose1", &matchmuloose1, &b_matchmuloose1);
   tree_->SetBranchAddress("matchmuloose2", &matchmuloose2, &b_matchmuloose2);
   tree_->SetBranchAddress("d0muloose1", &d0muloose1, &b_d0muloose1);
   tree_->SetBranchAddress("d0muloose2", &d0muloose2, &b_d0muloose2);
   tree_->SetBranchAddress("dzmuloose1", &dzmuloose1, &b_dzmuloose1);
   tree_->SetBranchAddress("dzmuloose2", &dzmuloose2, &b_dzmuloose2);
   tree_->SetBranchAddress("chHadmuloose1", &chHadmuloose1, &b_chHadmuloose1);
   tree_->SetBranchAddress("chHadmuloose2", &chHadmuloose2, &b_chHadmuloose2);
   tree_->SetBranchAddress("nHadmuloose1", &nHadmuloose1, &b_nHadmuloose1);
   tree_->SetBranchAddress("nHadmuloose2", &nHadmuloose2, &b_nHadmuloose2);
   tree_->SetBranchAddress("photmuloose1", &photmuloose1, &b_photmuloose1);
   tree_->SetBranchAddress("photmuloose2", &photmuloose2, &b_photmuloose2);
   tree_->SetBranchAddress("puptmuloose1", &puptmuloose1, &b_puptmuloose1);
   tree_->SetBranchAddress("puptmuloose2", &puptmuloose2, &b_puptmuloose2);
}




void RedNtpFinalizer_ttHgg::setSelectionType( const std::string& selectionType ) {

  selectionType_ = selectionType;


  // default values 
  
  dopureeventWeight_ = true;
  doptreeventWeight_ = true;
  r9cat_ = 1;
  photonID_thresh_ = 4;
  cs_ = false;

  ptphot1cut_ = 30.;
  ptphot2cut_ = 20.;

  ptgg_0btag_thresh_ = 0.;
  ptgg_1btag_thresh_ = 0.;
  ptgg_2btag_thresh_ = 0.;

  ptjetthresh_count_ = 20.;
  etajetthresh_count_ = 2.4;

  ptjet_0btag_thresh_ = 20.;
  ptjet_1btag_thresh_ = 20.;
  ptjet_2btag_thresh_ = 20.;

  // old cut logic, kept for backwards compatibility:
  ptjetleadthresh_0btag_ = 40.;

  zeppenfeld_thresh_ = 1000.;

  costhetastar_0btag_thresh_ = 2.;
  costhetastar_1btag_thresh_ = 2.;
  costhetastar_2btag_thresh_ = 2.;

  mjj_min_thresh_ = 0.;
  mjj_max_thresh_ = 10000.;

  //threshold added for ttH
  njets_thresh_=0;
  nbtagloose_thresh_=0;
  nbtagmedium_thresh_=0;
  isLeptonic_thresh_=0;
  Ht_thresh_=0;
  leptonVeto_=false;
  invert_photonCuts_=false;

  if( selectionType=="presel" ) {

    // leave all cuts to default

  } else if( selectionType=="sel0" ) {

   ptphot1cut_ = 60.;
   ptphot2cut_ = 25.;

   mjj_min_thresh_ = 60.;
   mjj_max_thresh_ = 120.;

  } else if( selectionType=="sel1" ) {

   ptphot1cut_ = 60.;
   ptphot2cut_ = 25.;

   ptgg_0btag_thresh_ = 50.;
   ptgg_1btag_thresh_ = 50.;
   ptgg_2btag_thresh_ = 50.;

   ptjetleadthresh_0btag_ = 40.;

   mjj_min_thresh_ = 60.;
   mjj_max_thresh_ = 120.;

  } else if( selectionType=="optsel1" ) {

   ptphot1cut_ = 60.;
   ptphot2cut_ = 25.;

   ptgg_0btag_thresh_ = 117.;
   ptgg_1btag_thresh_ = 103.;
   ptgg_2btag_thresh_ = 93.;

   ptjet_0btag_thresh_ = 36.;
   ptjet_1btag_thresh_ = 23.;
   ptjet_2btag_thresh_ = 25.;

   costhetastar_0btag_thresh_ = 0.74;
   costhetastar_1btag_thresh_ = 0.52;
   costhetastar_2btag_thresh_ = 0.58;

   mjj_min_thresh_ = 60.;
   mjj_max_thresh_ = 120.;

  } else if( selectionType=="ddrsel" ) {

   ptphot1cut_ = 62.5;
   ptphot2cut_ = 40.;

   ptjetthresh_count_ = 25.;
   ptjetleadthresh_0btag_ = 40.; //this is not accurate

   zeppenfeld_thresh_ = 1.5;

   mjj_min_thresh_ = 55.;
   mjj_max_thresh_ = 115.;
  } else if ( selectionType=="ttHsel" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    //    njets_thresh_=3;
  } else if ( selectionType=="ttHsel2Jets" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=2;
  } else if ( selectionType=="ttHsel2Jets1BTagLoose" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=2;
    nbtagloose_thresh_=1;

  } else if ( selectionType=="ttHsel5JetsNoBTag" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=5;


  }else if ( selectionType=="ttHsel5JetsNoBTag_inverted" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=5;
    invert_photonCuts_=true;

  }else if ( selectionType=="ttHsel4JetsNoBTagLeptonVeto_inverted" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=4;
    invert_photonCuts_=true;
    leptonVeto_=true;

  } else if ( selectionType=="ttHsel4JetsNoBTagLeptonVeto" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=4;
    leptonVeto_=true;

  } else if ( selectionType=="ttHselNJets" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=3;

  } else if ( selectionType=="ttHsel4JetsBTagLoose" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=4;
    nbtagloose_thresh_=1;

  }   else if ( selectionType=="ttHsel4JetsBTagMedium" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=4;
    nbtagmedium_thresh_=1;

  } else if ( selectionType=="ttHsel4JetsNoBTag" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=4;
    leptonVeto_=true;

  } else if ( selectionType=="ttHsel3Jets1Lepton" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=3;
    isLeptonic_thresh_=1;    
  } else if ( selectionType=="ttHsel3Jets1Lepton1BtagMedium" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=3;
    isLeptonic_thresh_=1;    

    nbtagmedium_thresh_=1;

  } else if ( selectionType=="ttHsel5JetsBtagMediumLeptonVeto" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=5;

    nbtagmedium_thresh_=1;
    leptonVeto_=true;

  } else if ( selectionType=="ttHsel3JetsHtCut" ){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;

    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;

    njets_thresh_=3;
    Ht_thresh_=340;

  }else if ( selectionType=="invertedPhotonID1Phot"){

    ptphot1cut_ = 33.;
    ptphot2cut_ = 25.;


    ptjetthresh_count_ = 25.;
    photonID_thresh_ = 4;
    njets_thresh_=2;

    invert_photonCuts_=true;


  } else {
    std::cout << std::endl << std::endl << "Selection '" << selectionType << "' currently not implemented. Exiting." << std::endl;
    exit(12345);

  }


}


