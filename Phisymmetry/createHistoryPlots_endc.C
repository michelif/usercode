#define createHistoryPlots_endc_cxx
#include "createHistoryPlots_endc.h"
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



void createHistoryPlots_endc::Loop()
{
using namespace std;
//   In a ROOT session, you can do:
//      Root > .L createHistoryPlots_endc.C
//      Root > createHistoryPlots_endc t
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
   int timeVar=0,hitVar=0,ixVar=0,iyVar=0,signVar=0;
   float energyVar=0,RMSenergyVar=0,lcVar=0,RMSlcVar=0;
   float energyNoCorrVar=0,RMSenergyNoCorrVar=0;
   outFile->cd();
   TTree* outTree= new TTree("tree_endc","tree_endc");
   outTree->Branch("time_interval",&timeVar,"timeInterval/I");
   outTree->Branch("nHits",&hitVar, "nHits/i");
   outTree->Branch("ix",&ixVar,"ix/I");
   outTree->Branch("iy",&iyVar,"iy/I");
   outTree->Branch("sign",&signVar,"sign/I");
   outTree->Branch("energySum",&energyVar,"energySum/F");
   outTree->Branch("energySquared",&RMSenergyVar,"energySquared/F");
   outTree->Branch("energyNoCorrSum",&energyNoCorrVar,"energyNoCorrSum/F");
   outTree->Branch("energyNoCorrSquared",&RMSenergyNoCorrVar,"energyNoCorrSquared/F");
   outTree->Branch("lcSum",&lcVar,"lcSum/F");
   outTree->Branch("lcSquared",&RMSlcVar,"lcSquared/F");

   cout<<"start looping over entries"<<endl;


   cout<<"start looping over entries"<<endl;
   Long64_t nbytes = 0, nb = 0;
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
       int theSign=sign[ihit];
       int theX=ix[ihit];
       int theY=iy[ihit];
       if(theSign < kSides && theX <=kEndcWedgesX && theInterval>=0 && theInterval <kIntervals && theY <=kEndcWedgesY ){
	 sums[theInterval].energySum[theX-1][theY-1][theSign]+=et_endc[ihit];
	 sums[theInterval].energySquared[theX-1][theY-1][theSign]+=pow(et_endc[ihit],2);
	 sums[theInterval].energyNoCorrSum[theX-1][theY-1][theSign]+=et_endc[ihit]/lc_endc[ihit];
	 sums[theInterval].energyNoCorrSquared[theX-1][theY-1][theSign]+=pow(et_endc[ihit]/lc_endc[ihit],2);
	 sums[theInterval].lasercorrSum[theX-1][theY-1][theSign]+=lc_endc[ihit];
	 sums[theInterval].lasercorrSquared[theX-1][theY-1][theSign]+=pow(lc_endc[ihit],2);
	 sums[theInterval].counter[theX-1][theY-1][theSign]++;
       }
     }

   }//close loop over entries


   cout<<"filling out tree"<<endl;

   for(int iinterval=0;iinterval<kIntervals;iinterval++){
     std::cout << "INTERVAL " << iinterval << "/" << kIntervals-1 << std::endl;
     for(int iisign=0;iisign<kSides;iisign++){
       for(int iix=0;iix<kEndcWedgesX;iix++){
	 for(int iiy=0;iiy<kEndcWedgesY;iiy++){
	   if(sums[iinterval].energySum[iix][iiy][iisign]>0){
	     hitVar= sums[iinterval].counter[iix][iiy][iisign];
	     ixVar=iix+1;
	     iyVar=iiy+1;
	     timeVar=iinterval+1;
	     signVar=iisign;
	     energyVar=sums[iinterval].energySum[iix][iiy][iisign];
	     RMSenergyVar=sums[iinterval].energySquared[iix][iiy][iisign];
	     energyNoCorrVar=sums[iinterval].energyNoCorrSum[iix][iiy][iisign];
	     RMSenergyNoCorrVar=sums[iinterval].energyNoCorrSquared[iix][iiy][iisign];
	     lcVar=sums[iinterval].lasercorrSum[iix][iiy][iisign];
	     RMSlcVar=sums[iinterval].lasercorrSquared[iix][iiy][iisign];
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
