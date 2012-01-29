#define createHistoryPlots_barl_cxx
#include "createHistoryPlots_barl.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include <TChain.h>
#include "TBrowser.h"
#include "TH2.h"
#include "TRandom.h"
#include "TStyle.h"
#include "TCanvas.h"
#include "TLine.h"
#include "TF1.h"
#include "TH1.h"
#include "THStack.h"
#include "TVector3.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

void createHistoryPlots_barl::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L createHistoryPlots_barl.C
//      Root > createHistoryPlots_barl t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();

   //######Reading the tree with time intervals###########

   TFile* f= TFile::Open("readMap_out_barl.root");
   TTree* intervalsTree= (TTree*)f->Get("outTree_barl");


   //   map<pair<int,int>,pair<int,int> > ;
   int fr,lr,fl,ll;

   TBranch *b_firstRun=intervalsTree->GetBranch("firstRun");
   TBranch *b_lastRun=intervalsTree->GetBranch("lastRun");
   TBranch *b_firstLumi=intervalsTree->GetBranch("firstLumi");
   TBranch *b_lastLumi=intervalsTree->GetBranch("lastLumi");
   
   intervalsTree->SetBranchAddress("firstRun", &fr, &b_firstRun);
   intervalsTree->SetBranchAddress("lastRun", &lr, &b_lastRun);
   intervalsTree->SetBranchAddress("firstLumi", &fl, &b_firstLumi);
   intervalsTree->SetBranchAddress("lastLumi", &ll, &b_lastLumi);


   vector<int> frvec;
   vector<int> lrvec;
   vector<int> flvec;
   vector<int> llvec;

   Long64_t nbytes_int = 0, nb_int = 0;
   int nentries_int = intervalsTree->GetEntries();
   for(int jentry=0;jentry<nentries_int;jentry++){
     if(jentry%100000==0) std::cout<<jentry<<std::endl;
     intervalsTree->GetEntry(jentry);
     frvec.push_back(fr);
     lrvec.push_back(lr);
     flvec.push_back(fl);
     llvec.push_back(ll);
     //     cout<<frvec[jentry]<<" "<<lrvec[jentry]<<" "<<flvec[jentry]<<" "<<llvec[jentry]<<endl;

   }

   //######### creating the output tree ##############
   TFile *outFile=TFile::Open("outputForHistory_barl.root");

   int timeVar,hitVar,ietaVar,iphiVar,energyVar,RMSenergyVar,lc,RMSlc;

   TTree* outTree= new TTree("tree_barl","tree_barl");
   outTree->Branch("time_interval",&timeVar,"timeInterval/I");
   outTree->Branch("nHits",&hitVar, "nHits/i");
   outTree->Branch("ieta",&ietaVar,"ieta/I");
   outTree->Branch("iphi",&iphiVar,"iphi/I");
   outTree->Branch("energy",&energyVar,"energy/F");
   outTree->Branch("RMSenergy",&RMSenergyVar,"RMSenergy/F");
   outTree->Branch("lc",&lcVar,"lc/F");
   outTree->Branch("RMSlc",&RMSlcVar,"RMSlc/F");

   //######Loop over ntuples###########

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

   }
}
