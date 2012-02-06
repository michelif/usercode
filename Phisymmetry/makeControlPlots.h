//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Feb  3 16:06:38 2012 by ROOT version 5.27/06b
// from TTree finalTree_barl/tree with et and lc
// found on file: finalOutput_2011A.root
//////////////////////////////////////////////////////////

#ifndef makeControlPlots_h
#define makeControlPlots_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
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


#include "lumiIntervals.h"

#define MAXHITS 1000
#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2
#define kTowerPerSM 68
#define kSM 36


class makeControlPlots {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   lumiIntervals* intervals;

   TString outFileName;
   

   // Declaration of leaf types
   Int_t           time_interval;
   UInt_t          nHits;
   Int_t           ieta;
   Int_t           iphi;
   Int_t           sign;
   Float_t         et;
   Float_t         RMSet;
   Float_t         etNoCorr;
   Float_t         RMSetNoCorr;
   Float_t         lc;
   Float_t         RMSlc;

   // List of branches
   TBranch        *b_timeInterval;   //!
   TBranch        *b_nHits;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_sign;   //!
   TBranch        *b_et;   //!
   TBranch        *b_RMSet;   //!
   TBranch        *b_etNoCorr;   //!
   TBranch        *b_RMSetNoCorr;   //!
   TBranch        *b_lc;   //!
   TBranch        *b_RMSlc;   //!

   int iSM(int eta, int phi, int isign)
   {
     int id = ( phi - 1 ) / 20 + 1;
     if ( isign == 0 ) id += 18;
     return id;
   }

   int iTower(int eta, int phi, int isign)
   {
     int ttEta=(eta-1)/5+1;
     int ttPhi=((phi-1)%20)/5+1;
     return (ttEta-1)*4+ttPhi;
   }

   int iTT(int eta, int phi, int isign)
   {
     return (iSM(eta,phi,isign)-1)*68+iTower(eta,phi,isign);
   }

   struct variablesToControl{
     float etMean[kBarlRings][kSides];
     float etMeanNoCorr[kBarlRings][kSides];     
     float lcMean[kBarlRings][kSides];
     float nhitMean[kBarlRings][kSides];
     int   counterEta[kBarlRings][kSides];  
     float etTowerMean[kSM*kTowerPerSM];
     float etTowerMeanNoCorr[kSM*kTowerPerSM];     
     float lcTowerMean[kSM*kTowerPerSM];
     float nhitTowerMean[kSM*kTowerPerSM];
     int   counterTower[kSM*kTowerPerSM];  
     float   counter;


     void reset(){
       for (int i=0;i<kBarlRings;++i)
	 {
	   for(int j=0;j<kSides;j++){
	     etMean[i][j]=0;	
	     etMeanNoCorr[i][j]=0;
	     lcMean[i][j]=0;	
	     nhitMean[i][j]=0;	
	     counterEta[i][j]=0;     
	   }
	 }
       for (int i=0;i<kSM*kTowerPerSM;++i)
	 {
	   etTowerMean[i]=0;	
	   etTowerMeanNoCorr[i]=0;
	   lcTowerMean[i]=0;	
	   nhitTowerMean[i]=0;	
	   counterTower[i]=0;     
	 }
       counter=0;	
     }

   };

   makeControlPlots(TTree *tree=0);
   virtual ~makeControlPlots();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void setLumiIntervals(const char* file);
   void setOutfile(const char* outFile);

};

#endif

#ifdef makeControlPlots_cxx
makeControlPlots::makeControlPlots(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("finalOutput_2011A.root");
      if (!f) {
         f = new TFile("finalOutput_2011A.root");
      }
      tree = (TTree*)gDirectory->Get("finalTree_barl");

   }
   Init(tree);
}

makeControlPlots::~makeControlPlots()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

void makeControlPlots::setLumiIntervals(const char* file)
{
  intervals=new lumiIntervals(file);
}

void makeControlPlots::setOutfile(const char* file)
{
   outFileName=TString(file);
}


Int_t makeControlPlots::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t makeControlPlots::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (!fChain->InheritsFrom(TChain::Class()))  return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void makeControlPlots::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("time_interval", &time_interval, &b_timeInterval);
   fChain->SetBranchAddress("nHits", &nHits, &b_nHits);
   fChain->SetBranchAddress("ieta", &ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", &iphi, &b_iphi);
   fChain->SetBranchAddress("sign", &sign, &b_sign);
   fChain->SetBranchAddress("et", &et, &b_et);
   fChain->SetBranchAddress("RMSet", &RMSet, &b_RMSet);
   fChain->SetBranchAddress("etNoCorr", &etNoCorr, &b_etNoCorr);
   fChain->SetBranchAddress("RMSetNoCorr", &RMSetNoCorr, &b_RMSetNoCorr);
   fChain->SetBranchAddress("lc", &lc, &b_lc);
   fChain->SetBranchAddress("RMSlc", &RMSlc, &b_RMSlc);
   Notify();
}

Bool_t makeControlPlots::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void makeControlPlots::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t makeControlPlots::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef makeControlPlots_cxx
