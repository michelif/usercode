//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Feb  1 10:09:08 2012 by ROOT version 5.27/06b
// from TTree tree_barl/tree_barl
// found on file: 2011A/outputForHistory_barl_2011A_1.root
//////////////////////////////////////////////////////////

#ifndef createLastTree_h
#define createLastTree_h

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


class createLastTree {
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
   Float_t         energySum;
   Float_t         energySquared;
   Float_t         energyNoCorrSum;
   Float_t         energyNoCorrSquared;
   Float_t         lcSum;
   Float_t         lcSquared;

   Float_t kFactorRing[kBarlRings];
     
   // List of branches
   TBranch        *b_time_interval;   //!
   TBranch        *b_nHits;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_sign;   //!
   TBranch        *b_energySum;   //!
   TBranch        *b_energySquared;   //!
   TBranch        *b_energyNoCorrSum;   //!
   TBranch        *b_energyNoCorrSquared;   //!
   TBranch        *b_lcSum;   //!
   TBranch        *b_lcSquared;   //!

   struct histosLastTree{
     float energySum[kBarlRings][kBarlWedges][kSides];
     float energySquared[kBarlRings][kBarlWedges][kSides];
     float energyNoCorrSum[kBarlRings][kBarlWedges][kSides];
     float energyNoCorrSquared[kBarlRings][kBarlWedges][kSides];
     float lasercorrSum[kBarlRings][kBarlWedges][kSides];
     float lasercorrSquared[kBarlRings][kBarlWedges][kSides];
     int nhit[kBarlRings][kBarlWedges][kSides];



    void reset()
    {
      for(int iisign=0;iisign<kSides;iisign++){
	for(int iieta=0;iieta<kBarlRings;iieta++){
	  for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	    energySum[iieta][iiphi][iisign]=0;
	    energySquared[iieta][iiphi][iisign]=0;
	    energyNoCorrSum[iieta][iiphi][iisign]=0;
	    energyNoCorrSquared[iieta][iiphi][iisign]=0;
	    lasercorrSum[iieta][iiphi][iisign]=0;
	    lasercorrSquared[iieta][iiphi][iisign]=0;
	  }
	}
      }
    }
     
   };

   createLastTree(TTree *tree=0);
   virtual ~createLastTree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void setLumiIntervals(const char* file);
   void setOutfile(const char* outFile);
   void setKFactorRing();
     };

#endif

#ifdef createLastTree_cxx
createLastTree::createLastTree(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("2011A/outputForHistory_barl_2011A_1.root");
      if (!f) {
         f = new TFile("2011A/outputForHistory_barl_2011A_1.root");
      }
      tree = (TTree*)gDirectory->Get("tree_barl");

   }
   Init(tree);
}

createLastTree::~createLastTree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}


#endif

void createLastTree::setLumiIntervals(const char* file)
{
  intervals=new lumiIntervals(file);
}

void createLastTree::setOutfile(const char* file)
{
   outFileName=TString(file);
}

void createLastTree::setKFactorRing()
{
  kFactorRing[0]=1./0.245028;
  kFactorRing[1]=1./0.26128;
  kFactorRing[2]=1./0.273089;
  kFactorRing[3]=1./0.2732;
  kFactorRing[4]=1./0.283083;
  kFactorRing[5]=1./0.259672;
  kFactorRing[6]=1./0.272157;
  kFactorRing[7]=1./0.283421;
  kFactorRing[8]=1./0.279282;
  kFactorRing[9]=1./0.287642;
  kFactorRing[10]=1./0.257156;
  kFactorRing[11]=1./0.26619;
  kFactorRing[12]=1./0.274317;
  kFactorRing[13]=1./0.273775;
  kFactorRing[14]=1./0.284879;
  kFactorRing[15]=1./0.272593;
  kFactorRing[16]=1./0.273589;
  kFactorRing[17]=1./0.262903;
  kFactorRing[18]=1./0.262583;
  kFactorRing[19]=1./0.260766;
  kFactorRing[20]=1./0.281547;
  kFactorRing[21]=1./0.2765;
  kFactorRing[22]=1./0.283145;
  kFactorRing[23]=1./0.279516;
  kFactorRing[24]=1./0.274313;
  kFactorRing[25]=1./0.273071;
  kFactorRing[26]=1./0.283549;
  kFactorRing[27]=1./0.286864;
  kFactorRing[28]=1./0.289093;
  kFactorRing[29]=1./0.291506;
  kFactorRing[30]=1./0.281464;
  kFactorRing[31]=1./0.284257;
  kFactorRing[32]=1./0.289215;
  kFactorRing[33]=1./0.288749;
  kFactorRing[34]=1./0.290137;
  kFactorRing[35]=1./0.29138;
  kFactorRing[36]=1./0.289088;
  kFactorRing[37]=1./0.290615;
  kFactorRing[38]=1./0.288974;
  kFactorRing[39]=1./0.285018;
  kFactorRing[40]=1./0.289581;
  kFactorRing[41]=1./0.290635;
  kFactorRing[42]=1./0.291807;
  kFactorRing[43]=1./0.289271;
  kFactorRing[44]=1./0.280238;
  kFactorRing[45]=1./0.287869;
  kFactorRing[46]=1./0.286005;
  kFactorRing[47]=1./0.287438;
  kFactorRing[48]=1./0.288334;
  kFactorRing[49]=1./0.279735;
  kFactorRing[50]=1./0.279621;
  kFactorRing[51]=1./0.278579;
  kFactorRing[52]=1./0.273631;
  kFactorRing[53]=1./0.27313;
  kFactorRing[54]=1./0.264403;
  kFactorRing[55]=1./0.264274;
  kFactorRing[56]=1./0.271175;
  kFactorRing[57]=1./0.269813;
  kFactorRing[58]=1./0.270147;
  kFactorRing[59]=1./0.266088;
  kFactorRing[60]=1./0.253557;
  kFactorRing[61]=1./0.255196;
  kFactorRing[62]=1./0.246645;
  kFactorRing[63]=1./0.246722;
  kFactorRing[64]=1./0.243231;
  kFactorRing[65]=1./0.252291;
  kFactorRing[66]=1./0.243408;
  kFactorRing[67]=1./0.238192;
  kFactorRing[68]=1./0.238795;
  kFactorRing[69]=1./0.224387;
  kFactorRing[70]=1./0.243234;
  kFactorRing[71]=1./0.239513;
  kFactorRing[72]=1./0.232539;
  kFactorRing[73]=1./0.231337;
  kFactorRing[74]=1./0.217976;
  kFactorRing[75]=1./0.223219;
  kFactorRing[76]=1./0.230454;
  kFactorRing[77]=1./0.227976;
  kFactorRing[78]=1./0.231087;
  kFactorRing[79]=1./0.229022;
  kFactorRing[80]=1./0.222095;
  kFactorRing[81]=1./0.225252;
  kFactorRing[82]=1./0.221834;
  kFactorRing[83]=1./0.220603;
  kFactorRing[84]=1./0.200071; 
}


Int_t createLastTree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t createLastTree::LoadTree(Long64_t entry)
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

void createLastTree::Init(TTree *tree)
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

   fChain->SetBranchAddress("time_interval", &time_interval, &b_time_interval);
   fChain->SetBranchAddress("nHits", &nHits, &b_nHits);
   fChain->SetBranchAddress("ieta", &ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", &iphi, &b_iphi);
   fChain->SetBranchAddress("sign", &sign, &b_sign);
   fChain->SetBranchAddress("energySum", &energySum, &b_energySum);
   fChain->SetBranchAddress("energySquared", &energySquared, &b_energySquared);
   fChain->SetBranchAddress("energyNoCorrSum", &energyNoCorrSum, &b_energyNoCorrSum);
   fChain->SetBranchAddress("energyNoCorrSquared", &energyNoCorrSquared, &b_energyNoCorrSquared);
   fChain->SetBranchAddress("lcSum", &lcSum, &b_lcSum);
   fChain->SetBranchAddress("lcSquared", &lcSquared, &b_lcSquared);
   Notify();
}

Bool_t createLastTree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void createLastTree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t createLastTree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}

