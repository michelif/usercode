//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar  7 11:45:18 2012 by ROOT version 5.27/06b
// from TTree kFactorTree/kFactorTree
// found on file: /afs/cern.ch/user/m/micheli/scratch0/CMSSW_4_2_7/src/AlCaPhiSym_Run2011A-v1_RAW_160331_161357/PhiSymmetryCalibration_kFactors.root
//////////////////////////////////////////////////////////

#ifndef kFactorFitter2_h
#define kFactorFitter2_h

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
#define  kNMiscalBinsEB 21

#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2


class kFactorFitter2 {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   struct kFactorGraphs{
     TGraph* kFactorGraphsSum_barl[kBarlWedges][kSides];
     TGraph* kFactorGraphsMean_barl[kBarlWedges][kSides];
     TGraph* kFactorGraphsRatio_barl[kBarlWedges][kSides];
     TGraph* kFactorGraphsnHits_barl[kBarlWedges][kSides];
     float etsum_vec[kBarlWedges][kSides][kNMiscalBinsEB],nhits_vec[kBarlWedges][kSides][kNMiscalBinsEB];
     float etsum_first_vec[kBarlWedges][kSides][kNMiscalBinsEB],nhits_first_vec[kBarlWedges][kSides][kNMiscalBinsEB],etsum_second_vec[kBarlWedges][kSides][kNMiscalBinsEB],nhits_second_vec[kBarlWedges][kSides][kNMiscalBinsEB];
   };


   // Declaration of leaf types
   Float_t         miscal;
   Float_t         etsum;
   Int_t           nhits;
   Float_t         etsum_first;
   Int_t           nhits_first;
   Float_t         etsum_second;
   Int_t           nhits_second;
   Int_t           ieta;
   Int_t           sign;
   Int_t           iphi;

   // List of branches
   TBranch        *b_miscal;   //!
   TBranch        *b_etsum;   //!
   TBranch        *b_nhits;   //!
   TBranch        *b_etsum_first;   //!
   TBranch        *b_nhits_first;   //!
   TBranch        *b_etsum_second;   //!
   TBranch        *b_nhits_second;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_sign;   //!
   TBranch        *b_iphi;   //!

   kFactorFitter2(TTree *tree=0);
   virtual ~kFactorFitter2();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef kFactorFitter2_cxx
kFactorFitter2::kFactorFitter2(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/afs/cern.ch/user/m/micheli/scratch0/CMSSW_4_2_7/src/AlCaPhiSym_Run2011A-v1_RAW_160331_161357/PhiSymmetryCalibration_kFactors.root");
      if (!f) {
         f = new TFile("/afs/cern.ch/user/m/micheli/scratch0/CMSSW_4_2_7/src/AlCaPhiSym_Run2011A-v1_RAW_160331_161357/PhiSymmetryCalibration_kFactors.root");
      }
      tree = (TTree*)gDirectory->Get("kFactorTree");

   }
   Init(tree);
}

kFactorFitter2::~kFactorFitter2()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t kFactorFitter2::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t kFactorFitter2::LoadTree(Long64_t entry)
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

void kFactorFitter2::Init(TTree *tree)
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

   fChain->SetBranchAddress("miscal", &miscal, &b_miscal);
   fChain->SetBranchAddress("etsum", &etsum, &b_etsum);
   fChain->SetBranchAddress("nhits", &nhits, &b_nhits);
   fChain->SetBranchAddress("etsum_first", &etsum_first, &b_etsum_first);
   fChain->SetBranchAddress("nhits_first", &nhits_first, &b_nhits_first);
   fChain->SetBranchAddress("etsum_second", &etsum_second, &b_etsum_second);
   fChain->SetBranchAddress("nhits_second", &nhits_second, &b_nhits_second);
   fChain->SetBranchAddress("ieta", &ieta, &b_ieta);
   fChain->SetBranchAddress("sign", &sign, &b_sign);
   fChain->SetBranchAddress("iphi", &iphi, &b_iphi);
   Notify();
}

Bool_t kFactorFitter2::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void kFactorFitter2::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t kFactorFitter2::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef kFactorFitter2_cxx
