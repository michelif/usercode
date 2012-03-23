#define createHistoryPlots_barl_cxx
#include "createHistoryPlots_barl.h"
#include "TMath.h"
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
#include "TTreeIndex.h"
#include "TChainIndex.h"
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

  if (!intervals)
    return;
  if (fChain == 0) return;
  cout<<"starting"<<endl;
  Long64_t nentries = fChain->GetEntries();
  cout<<"nentries "<<nentries<<endl;
  //######Reading the tree with time intervals###########


  std::vector<histos> sums;
  int kIntervals=intervals->numberOfIntervals();
  sums.reserve(kIntervals);

   cout<<"creating histos"<<endl;
   for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
     sums[iinterval].reset();
   }
   
   //######### creating the output tree ##############
   TFile *outFile=TFile::Open(outFileName,"recreate");
   int timeVar=0,hitVar=0,ietaVar=0,iphiVar=0,signVar=0;
   float energyVar=0,RMSenergyVar=0,lcVar=0,RMSlcVar=0;
   float energyNoCorrVar=0,RMSenergyNoCorrVar=0;
   outFile->cd();
   TTree* outTree= new TTree("tree_barl","tree_barl");
   outTree->Branch("time_interval",&timeVar,"timeInterval/I");
   outTree->Branch("nHits",&hitVar, "nHits/i");
   outTree->Branch("ieta",&ietaVar,"ieta/I");
   outTree->Branch("iphi",&iphiVar,"iphi/I");
   outTree->Branch("sign",&signVar,"sign/I");
   outTree->Branch("energySum",&energyVar,"energySum/F");
   outTree->Branch("energySquared",&RMSenergyVar,"energySquared/F");
   outTree->Branch("energyNoCorrSum",&energyNoCorrVar,"energyNoCorrSum/F");
   outTree->Branch("energyNoCorrSquared",&RMSenergyNoCorrVar,"energyNoCorrSquared/F");
   outTree->Branch("lcSum",&lcVar,"lcSum/F");
   outTree->Branch("lcSquared",&RMSlcVar,"lcSquared/F");

   cout<<"start looping over entries"<<endl;
   Long64_t nbytes = 0, nb = 0;

   //if you want to apply an offline et cut
   bool doOfflineEtCut=true;
   float offlineEtCut=0.25;

   if (doOfflineEtCut==false) offlineEtCut=0;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     if(jentry%100000==0) std::cout<< "EVENT " << jentry<< " RUN " << run << " LS " << lumi << std::endl; 
     //Checking quality of LS
     if (goodLS && !goodLS->isGoodLS(run,lumi))
       continue;
     int theInterval=intervals->intervalNumber(run, lumi);
     
     for (unsigned int ihit=0;ihit<nhit;++ihit){
       int theSign=ieta[ihit]>0 ? 1:0;
       int theEta=TMath::Abs(ieta[ihit]);
       int thePhi=iphi[ihit];
       
       //	 cout<<theEta<<" "<<thePhi<<" "<<theInterval<<" "<<theSign<<endl;
       if(theSign < kSides && thePhi <=kBarlWedges && theInterval>=0 && theInterval <kIntervals && theEta <=kBarlRings && et_barl[ihit]>offlineEtCut){
	 sums[theInterval].energySum[theEta-1][thePhi-1][theSign]+=et_barl[ihit];
	 sums[theInterval].energySquared[theEta-1][thePhi-1][theSign]+=pow(et_barl[ihit],2);
	 sums[theInterval].energyNoCorrSum[theEta-1][thePhi-1][theSign]+=et_barl[ihit]/lc_barl[ihit];
	 sums[theInterval].energyNoCorrSquared[theEta-1][thePhi-1][theSign]+=pow(et_barl[ihit]/lc_barl[ihit],2);
	 sums[theInterval].lasercorrSum[theEta-1][thePhi-1][theSign]+=lc_barl[ihit];
	 sums[theInterval].lasercorrSquared[theEta-1][thePhi-1][theSign]+=pow(lc_barl[ihit],2);
	 sums[theInterval].counter[theEta-1][thePhi-1][theSign]++;
       }
     }
   }//close loop over entries

   cout<<"filling out tree"<<endl;

   for(int iinterval=0;iinterval<kIntervals;iinterval++){
     std::cout << "INTERVAL " << iinterval << "/" << kIntervals-1 << std::endl;
     for(int iisign=0;iisign<kSides;iisign++){
       for(int iieta=0;iieta<kBarlRings;iieta++){
	 for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	   if(sums[iinterval].energySum[iieta][iiphi][iisign]>0){
	     hitVar= sums[iinterval].counter[iieta][iiphi][iisign];
	     ietaVar=iieta+1;
	     iphiVar=iiphi+1;
	     timeVar=iinterval+1;
	     signVar=iisign;
	     energyVar=sums[iinterval].energySum[iieta][iiphi][iisign];
	     RMSenergyVar=sums[iinterval].energySquared[iieta][iiphi][iisign];
	     energyNoCorrVar=sums[iinterval].energyNoCorrSum[iieta][iiphi][iisign];
	     RMSenergyNoCorrVar=sums[iinterval].energyNoCorrSquared[iieta][iiphi][iisign];
	     lcVar=sums[iinterval].lasercorrSum[iieta][iiphi][iisign];
	     RMSlcVar=sums[iinterval].lasercorrSquared[iieta][iiphi][iisign];
	     outTree->Fill();
	   }
	 }
       }
     }
   }
   
   outTree->Write();
   outFile->Write();
   outFile->Close();
}
