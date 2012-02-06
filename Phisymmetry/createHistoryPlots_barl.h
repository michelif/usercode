//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Jan 20 17:09:12 2012 by ROOT version 5.27/06b
// from TTree variablesTree_barl/variablesTree_barl
// found on file: root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees5/172947_174137/variablesTree_100_1_pps.root
//////////////////////////////////////////////////////////

#ifndef createHistoryPlots_barl_h
#define createHistoryPlots_barl_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <lumiIntervals.h>
#include <JSON.h>

#define MAXHITS 1000
#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2

class createHistoryPlots_barl {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   lumiIntervals* intervals;
   JSON* goodLS;

   TString outFileName;

   // Declaration of leaf types
   UInt_t          nhit;
   Int_t           ieta[MAXHITS];   //[nhit]
   Int_t           iphi[MAXHITS];   //[nhit]
   Float_t         lc_barl[MAXHITS];   //[nhit]
   Float_t         et_barl[MAXHITS];   //[nhit]
   UInt_t          unixtime;
   UInt_t          run;
   UInt_t          lumi;
   UInt_t          eventid;


   // List of branches
   TBranch        *b_nhit;   //!
   TBranch        *b_ieta;   //!
   TBranch        *b_iphi;   //!
   TBranch        *b_lc_barl;   //!
   TBranch        *b_et_barl;   //!
   TBranch        *b_unixtime;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_eventid;   //!

  struct histos{
    float energySum[kBarlRings][kBarlWedges][kSides];
    float energySquared[kBarlRings][kBarlWedges][kSides];
    float energyNoCorrSum[kBarlRings][kBarlWedges][kSides];
    float energyNoCorrSquared[kBarlRings][kBarlWedges][kSides];
    float lasercorrSum[kBarlRings][kBarlWedges][kSides];
    float lasercorrSquared[kBarlRings][kBarlWedges][kSides];
    int counter[kBarlRings][kBarlWedges][kSides];

    void reset()
    {
      for(int iisign=0;iisign<kSides;iisign++){
	for(int iieta=0;iieta<kBarlRings;iieta++){
	  for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	    counter[iieta][iiphi][iisign]=0;
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
 

   createHistoryPlots_barl(TTree *tree=0);
   virtual ~createHistoryPlots_barl();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void setLumiIntervals(const char* file);
   void setJSON(const char* jsonFile);
   void setOutfile(const char* jsonFile);
};

#endif

#ifdef createHistoryPlots_barl_cxx
createHistoryPlots_barl::createHistoryPlots_barl(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees5/172947_174137/variablesTree_100_1_pps.root");
      if (!f) {
         f = new TFile("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees5/172947_174137/variablesTree_100_1_pps.root");
      }
      tree = (TTree*)gDirectory->Get("variablesTree_barl");

   }
   Init(tree);
   intervals=0;
   outFileName=TString("outputBarl.root");
}

void createHistoryPlots_barl::setLumiIntervals(const char* file)
{
  intervals=new lumiIntervals(file);
}

void createHistoryPlots_barl::setJSON(const char* file)
{
  goodLS=new JSON(file);
}

void createHistoryPlots_barl::setOutfile(const char* file)
{
   outFileName=TString(file);
}

createHistoryPlots_barl::~createHistoryPlots_barl()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t createHistoryPlots_barl::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t createHistoryPlots_barl::LoadTree(Long64_t entry)
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

void createHistoryPlots_barl::Init(TTree *tree)
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

   fChain->SetBranchAddress("nhit", &nhit, &b_nhit);
   fChain->SetBranchAddress("ieta", ieta, &b_ieta);
   fChain->SetBranchAddress("iphi", iphi, &b_iphi);
   fChain->SetBranchAddress("lc_barl", lc_barl, &b_lc_barl);
   fChain->SetBranchAddress("et_barl", et_barl, &b_et_barl);
   fChain->SetBranchAddress("unixtime", &unixtime, &b_unixtime);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("eventid", &eventid, &b_eventid);


   Notify();
}

Bool_t createHistoryPlots_barl::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void createHistoryPlots_barl::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t createHistoryPlots_barl::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef createHistoryPlots_barl_cxx
