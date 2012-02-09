//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb  9 12:17:24 2012 by ROOT version 5.27/06b
// from TTree variablesTree_endc/variablesTree_endc
// found on file: root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/Run2011A_1/variablesTree_10_1_L4B.root
//////////////////////////////////////////////////////////

#ifndef createHistoryPlots_endc_h
#define createHistoryPlots_endc_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <lumiIntervals.h>
#include <JSON.h>

#define MAXHITS 1000
#define kEndcWedgesX 100
#define kEndcWedgesY 100
#define kSides 2

class createHistoryPlots_endc {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   lumiIntervals* intervals;
   JSON* goodLS;

   TString outFileName;


   // Declaration of leaf types
   UInt_t          nhit;
   Int_t           ix[MAXHITS];   //[nhit]
   Int_t           iy[MAXHITS];   //[nhit]
   UInt_t          sign[MAXHITS];   //[nhit]
   Float_t         lc_endc[MAXHITS];   //[nhit]
   Float_t         et_endc[MAXHITS];   //[nhit]
   UInt_t          unixtime;
   UInt_t          run;
   UInt_t          lumi;
   UInt_t          eventid;

   // List of branches
   TBranch        *b_nhit;   //!
   TBranch        *b_ix;   //!
   TBranch        *b_iy;   //!
   TBranch        *b_sign;   //!
   TBranch        *b_lc_endc;   //!
   TBranch        *b_et_endc;   //!
   TBranch        *b_unixtime;   //!
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_eventid;   //!

  struct histos{
    float energySum[kEndcWedgesX][kEndcWedgesY][kSides];
    float energySquared[kEndcWedgesX][kEndcWedgesY][kSides];
    float energyNoCorrSum[kEndcWedgesX][kEndcWedgesY][kSides];
    float energyNoCorrSquared[kEndcWedgesX][kEndcWedgesY][kSides];
    float lasercorrSum[kEndcWedgesX][kEndcWedgesY][kSides];
    float lasercorrSquared[kEndcWedgesX][kEndcWedgesY][kSides];
    int counter[kEndcWedgesX][kEndcWedgesY][kSides];

    void reset()
    {
      for(int iisign=0;iisign<kSides;iisign++){
	for(int iix=0;iix<kEndcWedgesX;iix++){
	  for(int iiy=0;iiy<kEndcWedgesY;iiy++){
	    counter[iix][iiy][iisign]=0;
	    energySum[iix][iiy][iisign]=0;
	    energySquared[iix][iiy][iisign]=0;
	    energyNoCorrSum[iix][iiy][iisign]=0;
	    energyNoCorrSquared[iix][iiy][iisign]=0;
	    lasercorrSum[iix][iiy][iisign]=0;
	    lasercorrSquared[iix][iiy][iisign]=0;
	  }
	}
      }
    }
  };
 



   createHistoryPlots_endc(TTree *tree=0);
   virtual ~createHistoryPlots_endc();
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

#ifdef createHistoryPlots_endc_cxx
createHistoryPlots_endc::createHistoryPlots_endc(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/Run2011A_1/variablesTree_10_1_L4B.root");
      if (!f) {
         f = new TFile("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/Run2011A_1/variablesTree_10_1_L4B.root");
      }
      tree = (TTree*)gDirectory->Get("variablesTree_endc");

   }
   Init(tree);
   intervals=0;
   outFileName=TString("outputEndc.root");
}

void createHistoryPlots_endc::setLumiIntervals(const char* file)
{
  intervals=new lumiIntervals(file);
}

void createHistoryPlots_endc::setJSON(const char* file)
{
  goodLS=new JSON(file);
}

void createHistoryPlots_endc::setOutfile(const char* file)
{
   outFileName=TString(file);
}

createHistoryPlots_endc::~createHistoryPlots_endc()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t createHistoryPlots_endc::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t createHistoryPlots_endc::LoadTree(Long64_t entry)
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

void createHistoryPlots_endc::Init(TTree *tree)
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
   fChain->SetBranchAddress("ix", ix, &b_ix);
   fChain->SetBranchAddress("iy", iy, &b_iy);
   fChain->SetBranchAddress("sign", sign, &b_sign);
   fChain->SetBranchAddress("lc_endc", lc_endc, &b_lc_endc);
   fChain->SetBranchAddress("et_endc", et_endc, &b_et_endc);
   fChain->SetBranchAddress("unixtime", &unixtime, &b_unixtime);
   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("eventid", &eventid, &b_eventid);
   Notify();
}

Bool_t createHistoryPlots_endc::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void createHistoryPlots_endc::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t createHistoryPlots_endc::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef createHistoryPlots_endc_cxx
