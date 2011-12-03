#define plotsMacro_cxx
#include "plotsMacro.h"
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
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

using namespace std;

void plotsMacro::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L plotsMacro.C
//      Root > plotsMacro t
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

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   TH2F* nentries_barl_vs_etaphi=new TH2F("nentries_barl_vs_etaphi","nentries_barl_vs_etaphi",360,1.,360.,170,-85.,85.);
   TH2F* lc_barl_vs_etaphi=new TH2F("lc_barl_vs_etaphi","lc_barl_vs_etaphi",360,1.,360.,170,-85.,85.);
   TH2F* et_barl_vs_etaphi=new TH2F("et_barl_vs_etaphi","et_barl_vs_etaphi",360,1.,360.,170,-85.,85.);


   TFile* outMap= TFile::Open("MapsOut.root","recreate");

      outMap->cd();
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      // if (Cut(ientry) < 0) continue;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      if(jentry%10000000==0) std::cout<<jentry<<std::endl;

      nentries_barl_vs_etaphi->Fill(phiBranch,etaBranch);
      lc_barl_vs_etaphi->Fill(phiBranch,etaBranch,lcBranch);
      et_barl_vs_etaphi->Fill(phiBranch,etaBranch,etBranch);



   }
      lc_barl_vs_etaphi->Divide(nentries_barl_vs_etaphi);
      et_barl_vs_etaphi->Divide(nentries_barl_vs_etaphi);

      lc_barl_vs_etaphi->Write();
      et_barl_vs_etaphi->Write();

      outMap->Write();
      outMap->Close();

}
