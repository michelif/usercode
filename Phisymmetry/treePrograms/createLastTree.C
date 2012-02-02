#define createLastTree_cxx
#include "createLastTree.h"
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



#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2
#define kIntervals 382


void createLastTree::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L createLastTree.C
//      Root > createLastTree t
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
   cout<<"starting"<<endl;
   Long64_t nentries = fChain->GetEntries();
   cout<<"nentries "<<nentries<<endl;



   struct histos{
     float energySumVec[kBarlRings][kBarlWedges][kSides];
     float energySquaredVec[kBarlRings][kBarlWedges][kSides];
     float lasercorrSumVec[kBarlRings][kBarlWedges][kSides];
     float lasercorrSquaredVec[kBarlRings][kBarlWedges][kSides];
     int nhit[kBarlRings][kBarlWedges][kSides];
     
     //     int   indexinterval;
   };

   histos* histostruct=(histos*)malloc(kIntervals*sizeof(histos));


   for(int iisign=0;iisign<kSides;++iisign){
     for(int iieta=0;iieta<kBarlRings;++iieta){
       for(int iiphi=0;iiphi<kBarlWedges;++iiphi){
	 for(int iinterval=0;iinterval<kIntervals;iinterval++){
	   histostruct[iinterval].nhit[iieta][iiphi][iisign]=0;
	   histostruct[iinterval].energySumVec[iieta][iiphi][iisign]=0;
	   histostruct[iinterval].energySquaredVec[iieta][iiphi][iisign]=0;
	   histostruct[iinterval].lasercorrSumVec[iieta][iiphi][iisign]=0;
	   histostruct[iinterval].lasercorrSquaredVec[iieta][iiphi][iisign]=0;
	   
	 }
       }
     }
   }
   

   TFile *outFile=TFile::Open("finalOutput.root","recreate");

   int timeVar=0,hitVar=0,ietaVar=0,iphiVar=0,signVar=0;
   float etVar=0,RMSetVar=0,lcVar=0,RMSlcVar=0;
   TTree* newTree=new TTree("finalTree_barl","tree with et and lc");
   newTree->Branch("time_interval",&timeVar,"timeInterval/I");
   newTree->Branch("nHits",&hitVar, "nHits/i");
   newTree->Branch("ieta",&ietaVar,"ieta/I");
   newTree->Branch("iphi",&iphiVar,"iphi/I");
   newTree->Branch("sign",&signVar,"sign/I");
   newTree->Branch("et",&etVar,"et/F");
   newTree->Branch("RMSet",&RMSetVar,"RMSet/F");
   newTree->Branch("lc",&lcVar,"lc/F");
   newTree->Branch("RMSlc",&RMSlcVar,"RMSlc/F");


   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
     if(jentry%100000==0) std::cout<<jentry<<std::endl; 
     int theSign=sign;
     int theEta=ieta;
     int thePhi=iphi;
     int theInterval = time_interval;

     if(theSign < kSides && thePhi <=kBarlWedges && theInterval>=0 && theInterval <=kIntervals && theEta <=kBarlRings ){
       
       histostruct[theInterval-1].energySumVec[theEta-1][thePhi-1][theSign]+=energySum;
       histostruct[theInterval-1].energySquaredVec[theEta-1][thePhi-1][theSign]+=energySquared;
       histostruct[theInterval-1].lasercorrSumVec[theEta-1][thePhi-1][theSign]+=lcSum;
       histostruct[theInterval-1].lasercorrSquaredVec[theEta-1][thePhi-1][theSign]+=lcSquared;
       histostruct[theInterval-1].nhit[theEta-1][thePhi-1][theSign]+=nHits;
       
     }
   }

   for(int iisign=0;iisign<kSides;++iisign){
     for(int iieta=0;iieta<kBarlRings;++iieta){
       if(iieta%10 ==0) cout<<iieta<<endl;
       for(int iiphi=0;iiphi<kBarlWedges;++iiphi){
	 for(int iinterval=0;iinterval<kIntervals;iinterval++){
	   if(histostruct[iinterval].nhit[iieta][iiphi][iisign]>0 ){
	     int N=histostruct[iinterval].nhit[iieta][iiphi][iisign];
	     ietaVar=iieta+1;
	     iphiVar=iiphi+1;
	     timeVar=iinterval+1;
	     signVar=iisign;
	     etVar=histostruct[iinterval].energySumVec[iieta][iiphi][iisign]/histostruct[iinterval].nhit[iieta][iiphi][iisign];
	     RMSetVar=sqrt(TMath::Abs(histostruct[iinterval].energySquaredVec[iieta][iiphi][iisign]/N-pow(etVar,2)));
	     lcVar=histostruct[iinterval].lasercorrSumVec[iieta][iiphi][iisign]/histostruct[iinterval].nhit[iieta][iiphi][iisign];
	     RMSlcVar=sqrt(TMath::Abs(histostruct[iinterval].lasercorrSquaredVec[iieta][iiphi][iisign]/N-pow(lcVar,2)));
	     hitVar=N;
	     newTree->Fill();
	   }
	 }
	 
       }
     }
   }

   cout<<fChain->GetEntries();   

   newTree->Write();
   outFile->Write();
   outFile->Close();
   
}
