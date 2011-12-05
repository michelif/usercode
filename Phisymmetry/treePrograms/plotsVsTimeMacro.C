#define plotsVsTimeMacro_cxx
#include "plotsVsTimeMacro.h"
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


#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2
#define  kEndcWedgesX = 100
#define  kEndcWedgesY = 100

void plotsVsTimeMacro::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L plotsVsTimeMacro.C
//      Root > plotsVsTimeMacro t
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

   TH2F* lc_barl_vs_time[kBarlRings][kBarlWedges][kSides];
   TH2F* et_barl_vs_time[kBarlRings][kBarlWedges][kSides];

   TH2F* nentries_endc_vs_time[kBarlRings][kBarlWedges][kSides];
   TH2F* lc_endc_vs_time=[kBarlRings][kBarlWedges][kSides];
   TH2F* et_endc_vs_time=[kBarlRings][kBarlWedges][kSides];

   TH2F* nentries_endc_vs_time_positive=[kBarlRings][kBarlWedges][kSides];
   TH2F* lc_endc_vs_time_positive=[kBarlRings][kBarlWedges][kSides];
   TH2F* et_endc_vs_time_positive=[kBarlRings][kBarlWedges][kSides];

   TH2F* nentries_endc_vs_time_negative=[kBarlRings][kBarlWedges][kSides];
   TH2F* lc_endc_vs_time_negative=[kBarlRings][kBarlWedges][kSides];
   TH2F* et_endc_vs_time_negative=[kBarlRings][kBarlWedges][kSides];

   cout<<"creating histos"<<endl;

   for(int iisign=0;iisign<kSides;iisign++){
    for(int iieta=0;iieta<kBarlRings;iieta++){
      for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	
	  stringstream etName_barl_Stream;
	  etName_barl_Stream<<"etVsTime_barl_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	  et_barl_vs_time[iieta][iiphi][iisign]=new TH2F(etName_barl_Stream.str().c_str(),,360,1.,360.,170,-85.,85.);
	  
	  stringstream lcName_barl_Stream;
	  lcName_barl_Stream<<"lcVsTime_barl_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	  lc_barl_vs_time[iieta][iiphi][iisign]=new TH2F(lcName_barl_Stream.str().c_str(),,360,1.,360.,170,-85.,85.);

	  for (int ix=0; ix<kEndcWedgesX; ix++) {
	    for (int iy=0; iy<kEndcWedgesY; iy++) {
	      stringstream etName_endc_Stream;
	      etName_endc_Stream<<"etVsTime_endc_"<<ix+1<<"_"<<iy+1<<"_"<<iisign;
	      et_endc_vs_time[ix][iy][iisign]=new TH2F(etName_endc_Stream.str().c_str(),,360,1.,360.,170,-85.,85.);
	      
	      stringstream lcName_endc_Stream;
	      lcName_endc_Stream<<"lcVsTime_endc_"<<ix+1<<"_"<<iy+1<<"_"<<iisign;
	      lc_endc_vs_time[ix][iy][iisign]=new TH2F(lcName_endc_Stream.str().c_str(),,360,1.,360.,170,-85.,85.);
	      
	      stringstream etName_endc_positive_Stream;
	      etName_endc_positive_Stream<<"etVsTime_endc_positive_"<<ix+1<<"_"<<iy+1<<"_"<<iisign;
	      et_endc_positive_vs_time_positive[ix][iy][iisign]=new TH2F(etName_endc_positive_Stream.str().c_str(),,360,1.,360.,170,-85.,85.);
	      
	      stringstream lcName_endc_positive_Stream;
	      lcName_endc_positive_Stream<<"lcVsTime_endc_positive_"<<ix+1<<"_"<<iy+1<<"_"<<iisign;
	      lc_endc_positive_vs_time_positive[ix][iy][iisign]=new TH2F(lcName_endc_positive_Stream.str().c_str(),,360,1.,360.,170,-85.,85.);
	      
	      stringstream etName_endc_negative_Stream;
	      etName_endc_negative_Stream<<"etVsTime_endc_negative_"<<ix+1<<"_"<<iy+1<<"_"<<iisign;
	      et_endc_negative_vs_time_negative[ix][iy][iisign]=new TH2F(etName_endc_negative_Stream.str().c_str(),,360,1.,360.,170,-85.,85.);
	      
	      stringstream lcName_endc_negative_Stream;
	      lcName_endc_negative_Stream<<"lcVsTime_endc_negative_"<<ix+1<<"_"<<iy+1<<"_"<<iisign;
	      lc_endc_negative_vs_time_negative[ix][iy][iisign]=new TH2F(lcName_endc_negative_Stream.str().c_str(),,360,1.,360.,170,-85.,85.);
	      

	    }
	  }

	  

      }
    }
   }    
   
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     if(jentry%10000000==0) std::cout<<jentry<<std::endl;
     int theSign=etaBranch>0 ? 1:0;
     lc_barl_vs_time[etaBranch-1][phiBranch-1][theSign]->Fill(lc_barl_Branch,unixTime_barl_Branch);
     //     et_barl_vs_time[etaBranch-1][phiBranch-1][theSign]->Fill(et_barl_Branch,unixTime_barl_Branch);
     
     //     lc_endc_vs_time[etaBranch-1][phiBranch-1][theSign]->Fill(xBranch,yBranch,lc_endc_Branch);
     //     et_endc_vs_time[etaBranch-1][phiBranch-1][theSign]->Fill(xBranch,yBranch,et_endc_Branch);
     
     if(sign_endc_Branch>0){
       //   lc_endc_vs_time_positive->Fill(xBranch,yBranch,lc_endc_Branch);
       // et_endc_vs_time_positive->Fill(xBranch,yBranch,et_endc_Branch);
       
     }else{
       
       //lc_endc_vs_time_negative->Fill(xBranch,yBranch,lc_endc_Branch);
       //et_endc_vs_time_negative->Fill(xBranch,yBranch,et_endc_Branch);
       
     }
     
     
   }
   
   TFile* lcFile=TFile::Open("lcVsTime.root","recreate");
   for(int iisign=0;iisign<kSides;iisign++){
     for(int iieta=0;iieta<kBarlRings;iieta++){
       for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	 lc_barl_vs_time[iieta][iiphi][iisign]->Write();	 
       }
     }
   }
  
   lcFile->Write();
   lcFile->Close();

}
