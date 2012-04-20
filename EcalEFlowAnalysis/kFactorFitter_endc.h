//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Apr 20 16:01:32 2012 by ROOT version 5.27/06b
// from TTree kFactorTree_endc/kFactorTree_endc
// found on file: root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/kFactors_endc/AlCaPhiSym/Run2011A-v1/RAW/160331-161357/PhiSymmetryCalibration_kFactors_19_1_q7i.root
//////////////////////////////////////////////////////////

#ifndef kFactorFitter_endc_h
#define kFactorFitter_endc_h

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
#define  kNMiscalBinsEE 11

#define kSides 2
#define kEndcWedgesX 100
#define kEndcWedgesY 100
#define kRings 39

class kFactorFitter_endc {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   TString outFileName;

   struct kFactorGraphs_endc{
     TGraph* kFactorGraphsSum_endc[kRings];
     float etsum_ring[kRings][kNMiscalBinsEE];

     void Reset(){
       for(int i =0; i<kRings;++i){
	 for(int j=0;j<kNMiscalBinsEE;++j){
	   etsum_ring[i][j]=0.;
	 }
       }
     }
     
   };
   
   // Declaration of leaf types
   Float_t         miscal;
   Float_t         etsum;
   Int_t           ring;

   // List of branches
   TBranch        *b_miscal;   //!
   TBranch        *b_etsum;   //!
   TBranch        *b_ring;   //!

   kFactorFitter_endc(TTree *tree=0);
   virtual ~kFactorFitter_endc();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void setOutfile(const char* outFile);
};

#endif

#ifdef kFactorFitter_endc_cxx
kFactorFitter_endc::kFactorFitter_endc(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/kFactors_endc/AlCaPhiSym/Run2011A-v1/RAW/160331-161357/PhiSymmetryCalibration_kFactors_19_1_q7i.root");
      if (!f) {
         f = new TFile("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/kFactors_endc/AlCaPhiSym/Run2011A-v1/RAW/160331-161357/PhiSymmetryCalibration_kFactors_19_1_q7i.root");
      }
      tree = (TTree*)gDirectory->Get("kFactorTree_endc");

   }
   Init(tree);
}

kFactorFitter_endc::~kFactorFitter_endc()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t kFactorFitter_endc::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t kFactorFitter_endc::LoadTree(Long64_t entry)
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

void kFactorFitter_endc::Init(TTree *tree)
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
   fChain->SetBranchAddress("ring", &ring, &b_ring);
   Notify();
}

Bool_t kFactorFitter_endc::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void kFactorFitter_endc::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t kFactorFitter_endc::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

void kFactorFitter_endc::setOutfile(const char* file)
{
   outFileName=TString(file);
}


#endif // #ifdef kFactorFitter_endc_cxx
