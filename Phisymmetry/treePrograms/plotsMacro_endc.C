#define plotsMacro_endc_cxx
#include "plotsMacro_endc.h"
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

void plotsMacro_endc::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L plotsMacro_endc.C
//      Root > plotsMacro_endc t
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

   Long64_t nentries = fChain->GetEntries();

   Long64_t nbytes = 0, nb = 0;


   TH2F* nentries_endc_vs_xy=new TH2F("nentries_endc_vs_xy","nentries_endc_vs_xy",100,1.,100.,100,1.,100.);
   TH2F* lc_endc_vs_xy=new TH2F("lc_endc_vs_xy","lc_endc_vs_xy",100,1.,100.,100,1.,100.);
   TH2F* et_endc_vs_xy=new TH2F("et_endc_vs_xy","et_endc_vs_xy",100,1.,100.,100,1.,100.);

   TH2F* nentries_endc_vs_xy_positive=new TH2F("nentries_endc_vs_xy_positive","nentries_endc_vs_xy_positive",100,1.,100.,100,1.,100.);
   TH2F* lc_endc_vs_xy_positive=new TH2F("lc_endc_vs_xy_positive","lc_endc_vs_xy_positive",100,1.,100.,100,1.,100.);
   TH2F* et_endc_vs_xy_positive=new TH2F("et_endc_vs_xy_positive","et_endc_vs_xy_positive",100,1.,100.,100,1.,100.);

   TH2F* nentries_endc_vs_xy_negative=new TH2F("nentries_endc_vs_xy_negative","nentries_endc_vs_xy_negative",100,1.,100.,100,1.,100.);
   TH2F* lc_endc_vs_xy_negative=new TH2F("lc_endc_vs_xy_negative","lc_endc_vs_xy_negative",100,1.,100.,100,1.,100.);
   TH2F* et_endc_vs_xy_negative=new TH2F("et_endc_vs_xy_negative","et_endc_vs_xy_negative",100,1.,100.,100,1.,100.);

   

    TFile* outMapEndc= TFile::Open("MapsOutEndc.root","recreate");
   

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     // if (Cut(ientry) < 0) continue;
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   
     nbytes += nb;
     if(jentry%10000000==0) std::cout<<jentry<<std::endl;
     
    
       for (int ihit=0;ihit<nhit;++ihit){
       nentries_endc_vs_xy->Fill(ix[ihit],iy[ihit]);
       lc_endc_vs_xy->Fill(ix[ihit],iy[ihit],lc_endc[ihit]);
       et_endc_vs_xy->Fill(ix[ihit],iy[ihit],et_endc[ihit]);

       if(sign[ihit]>0){
       nentries_endc_vs_xy_positive->Fill(ix[ihit],iy[ihit]);
       lc_endc_vs_xy_positive->Fill(ix[ihit],iy[ihit],lc_endc[ihit]);
       et_endc_vs_xy_positive->Fill(ix[ihit],iy[ihit],et_endc[ihit]);
       
       }else{

       nentries_endc_vs_xy_negative->Fill(ix[ihit],iy[ihit]);
       lc_endc_vs_xy_negative->Fill(ix[ihit],iy[ihit],lc_endc[ihit]);
       et_endc_vs_xy_negative->Fill(ix[ihit],iy[ihit],et_endc[ihit]);

       }
      
       }
   }	
   




     lc_endc_vs_xy->Divide(nentries_endc_vs_xy);
     et_endc_vs_xy->Divide(nentries_endc_vs_xy);

     lc_endc_vs_xy_positive->Divide(nentries_endc_vs_xy_positive);
     et_endc_vs_xy_positive->Divide(nentries_endc_vs_xy_positive);

     lc_endc_vs_xy_negative->Divide(nentries_endc_vs_xy_negative);
     et_endc_vs_xy_negative->Divide(nentries_endc_vs_xy_negative);

     cout<<"start writing"<<endl;

     outMapEndc->cd();
     
     lc_endc_vs_xy->Write();
     et_endc_vs_xy->Write();
     




     cout<<"maps done"<<endl;
     //     TFile* outMapEndcPosNeg= TFile::Open("MapsOutEndcPosNeg.root","recreate");
     // outMapEndcPosNeg->cd();
     lc_endc_vs_xy_positive->Write();
     et_endc_vs_xy_positive->Write();

     cout<<"positive done"<<endl;

     lc_endc_vs_xy_negative->Write();
     et_endc_vs_xy_negative->Write();

     //outMapEndcPosNeg->Write();
     // outMapEndcPosNeg->Close();
     cout<<"all done"<<endl;

     outMapEndc->Write();
     outMapEndc->Close();

      
}
