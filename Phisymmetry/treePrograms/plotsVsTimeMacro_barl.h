//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Dec  5 16:28:43 2011 by ROOT version 5.27/06b
// from TTree variablesTree_endc/variablesTree_endc
// found on file: root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees3/172947_174137/variablesTree_9_1_Ttq.root
//////////////////////////////////////////////////////////

#ifndef plotsVsTimeMacro_barl_h
#define plotsVsTimeMacro_barl_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class plotsVsTimeMacro_barl {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Float_t         etaBranch;
   Float_t         phiBranch;
   Float_t         lc_barl_Branch;
   Float_t         et_barl_Branch;
   Int_t           unixTime_barl_Branch;

   //endcap
/*    Float_t         xBranch; */
/*    Float_t         yBranch; */
/*    Float_t         lc_endc_Branch; */
/*    Float_t         et_endc_Branch; */
/*    Int_t           unixTime_endc_Branch; */
/*    Float_t         sign_endc_Branch; */

   // List of branches
   TBranch        *b_etaBranch;   //!
   TBranch        *b_phiBranch;   //!
   TBranch        *b_lc_barl_Branch;   //!
   TBranch        *b_et_barl_Branch;   //!
   TBranch        *b_unixTime_barl_Branch;   //!

   //endcap
/*    TBranch        *b_xBranch;   //! */
/*    TBranch        *b_yBranch;   //! */
/*    TBranch        *b_lc_endc_Branch;   //! */
/*    TBranch        *b_et_endc_Branch;   //! */
/*    TBranch        *b_unixTime_endc_Branch;   //! */
/*    TBranch        *b_sign_endc_Branch;   //! */

   plotsVsTimeMacro_barl(TTree *tree=0);
   virtual ~plotsVsTimeMacro_barl();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef plotsVsTimeMacro_barl_cxx
plotsVsTimeMacro_barl::plotsVsTimeMacro_barl(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees3/172947_174137/variablesTree_9_1_Ttq.root");
      if (!f) {
         f = new TFile("root://eoscms//eos/cms/store/caf/user/micheli/PhiSymmetryJobs/jobsWithTrees3/172947_174137/variablesTree_9_1_Ttq.root");
      }
      tree = (TTree*)gDirectory->Get("variablesTree_endc");

   }
   Init(tree);
}

plotsVsTimeMacro_barl::~plotsVsTimeMacro_barl()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t plotsVsTimeMacro_barl::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t plotsVsTimeMacro_barl::LoadTree(Long64_t entry)
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

void plotsVsTimeMacro_barl::Init(TTree *tree)
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

   fChain->SetBranchAddress("etaBranch", &etaBranch, &b_etaBranch);
   fChain->SetBranchAddress("phiBranch", &phiBranch, &b_phiBranch);
   fChain->SetBranchAddress("lc_barl_Branch", &lc_barl_Branch, &b_lc_barl_Branch);
   fChain->SetBranchAddress("et_barl_Branch", &et_barl_Branch, &b_et_barl_Branch);
   fChain->SetBranchAddress("unixTime_barl_Branch", &unixTime_barl_Branch, &b_unixTime_barl_Branch);

/*    fChain->SetBranchAddress("xBranch", &xBranch, &b_xBranch); */
/*    fChain->SetBranchAddress("yBranch", &yBranch, &b_yBranch); */
/*    fChain->SetBranchAddress("lc_endc_Branch", &lc_endc_Branch, &b_lc_endc_Branch); */
/*    fChain->SetBranchAddress("et_endc_Branch", &et_endc_Branch, &b_et_endc_Branch); */
/*    fChain->SetBranchAddress("unixTime_endc_Branch", &unixTime_endc_Branch, &b_unixTime_endc_Branch); */
/*    fChain->SetBranchAddress("sign_endc_Branch", &sign_endc_Branch, &b_sign_endc_Branch); */



   Notify();
}

Bool_t plotsVsTimeMacro_barl::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void plotsVsTimeMacro_barl::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t plotsVsTimeMacro_barl::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef plotsVsTimeMacro_barl_cxx
