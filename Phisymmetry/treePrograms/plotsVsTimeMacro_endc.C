#define plotsVsTimeMacro_endc_cxx
#include "plotsVsTimeMacro_endc.h"
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
#define  kEndcWedgesX  100
#define  kEndcWedgesY  100
#define NHITS 900

using namespace std;

void plotsVsTimeMacro_endc::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L plotsVsTimeMacro_endc.C
//      Root > plotsVsTimeMacro_endc t
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
   cout<<"nentries "<<nentries<<endl;

   Long64_t nbytes = 0, nb = 0;


   TH2F* lc_endc_vs_time[kEndcWedgesX][kEndcWedgesY][kSides];
   TH2F* et_endc_vs_time[kEndcWedgesX][kEndcWedgesY][kSides];
   TH2F* et_etaRef_endc_vs_time[kEndcWedgesX][kEndcWedgesY][kSides];

   TH1F* energy[kEndcWedgesX][kEndcWedgesY][kSides];
   TH1F* lc[kEndcWedgesX][kEndcWedgesY][kSides];
   TH1F* counterHisto=new TH1F("counterhisto","counterhisto",500,0,2000);
   int counter[kEndcWedgesX][kEndcWedgesY][kSides];
   float unixtimeMean[kEndcWedgesX][kEndcWedgesY][kSides];


   cout<<"creating histos"<<endl;
   //   int iisign=0;


   for(int iisign=0;iisign<kSides;iisign++){
	  for (int iix=0; iix<kEndcWedgesX; iix++) {
	    for (int iiy=0; iiy<kEndcWedgesY; iiy++) {
	      stringstream etName_endc_Stream;
	      etName_endc_Stream<<"etVsTime_endc_"<<iix+1<<"_"<<iiy+1<<"_"<<iisign;
	      et_endc_vs_time[iix][iiy][iisign]=new TH2F(etName_endc_Stream.str().c_str(),etName_endc_Stream.str().c_str(),100,1312833000.,1314353000.,100,0,1);
	      stringstream lcName_endc_Stream;
	      lcName_endc_Stream<<"lcVsTime_endc_"<<iix+1<<"_"<<iiy+1<<"_"<<iisign;
	           lc_endc_vs_time[iix][iiy][iisign]=new TH2F(lcName_endc_Stream.str().c_str(),lcName_endc_Stream.str().c_str(),100,1312833000.,1314353000.,100,0.3,1.7);

	      stringstream energyname;
	      energyname<<"energy_"<<iix+1<<"_"<<iiy+1<<"_"<<iisign;                                        
	      energy[iix][iiy][iisign]=new TH1F(energyname.str().c_str(),energyname.str().c_str(),1000, 0.,2.);
	      
	      stringstream lcname;
	      lcname<<"lc_"<<iix+1<<"_"<<iiy+1<<"_"<<iisign;                                        
	      lc[iix][iiy][iisign]=new TH1F(lcname.str().c_str(),lcname.str().c_str(),1000, 0.,2.);
	      
	      
	      counter[iix][iiy][iisign]=0;
	      
	      unixtimeMean[iix][iiy][iisign]=0;


	      
	    }
	  }
   }
	  

   float oldtime=0;
   
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     if(jentry%100000==0) std::cout<<jentry<<std::endl;

     for (int ihit=0;ihit<nhit;++ihit){

       int theSign=sign[ihit];
     int theX=ix[ihit];
     int theY=iy[ihit];
     //                cout<<(theX)<<" "<<(theY)<<" "<<theSign<<endl;
     //        cout<<"lc "<<lc_endc[ihit]<<"time "<<unixtime<<endl;
     lc_endc_vs_time[theX-1][theY-1][theSign]->Fill(unixtime,lc_endc[ihit]);
     et_endc_vs_time[theX-1][theY-1][theSign]->Fill(unixtime,et_endc[ihit]);
     unixtimeMean[theX-1][theY-1][theSign]+=unixtime;
     energy[theX-1][theY-1][theSign]->Fill(et_endc[ihit]);
     lc[theX-1][theY-1][theSign]->Fill(lc_endc[ihit]);
     counter[theX-1][theY-1][theSign]++;

     //check if it is a different inter fill
     if  (TMath::Abs(unixtime-oldtime)>3600) {
       energy[theX-1][theY-1][theSign]->Reset();
       lc[theX-1][theY-1][theSign]->Reset();
       unixtimeMean[theX-1][theY-1][theSign]=0;
       counter[theX-1][theY-1][theSign]=0;
       //       cout<<"time"<<endl;
     }
     



     if( counter[theX-1][theY-1][theSign]== NHITS) {
       // cout<<counter[theX-1][theY-1][theSign]<<" ";
       //            cout<<(theX)<<" "<<(theY)<<" "<<theSign<<endl;
       //     lc_barl_vs_time[theX-1][theY-1][theSign]->Fill(unixtime,lc_barl[ihit]);
       //       cout<<unixtimeMean[theX-1][theY-1][theSign]/counter[theX-1][theY-1][theSign]<<endl;
       //       cout<<lc[theX-1][theY-1][theSign]->GetMean()<<endl;
            et_endc_vs_time[theX-1][theY-1][theSign]->Fill(unixtimeMean[theX-1][theY-1][theSign]/counter[theX-1][theY-1][theSign],energy[theX-1][theY-1][theSign]->GetMean());
          lc_endc_vs_time[theX-1][theY-1][theSign]->Fill(unixtimeMean[theX-1][theY-1][theSign]/counter[theX-1][theY-1][theSign],lc[theX-1][theY-1][theSign]->GetMean());
     energy[theX-1][theY-1][theSign]->Reset();
     lc[theX-1][theY-1][theSign]->Reset();
     //     unixtimeHisto->Reset();
     unixtimeMean[theX-1][theY-1][theSign]=0;
     counterHisto->Fill(counter[theX-1][theY-1][theSign]);
     
     counter[theX-1][theY-1][theSign]=0;
     }

     }
     oldtime=unixtime;
   }
     
   cout<<"loop ends"<<endl;    

   TFile* endcfile=TFile::Open("endcVsTime.root","recreate");

   cout<<"writing lc histos"<<endl;
      for(int iisign=0;iisign<kSides;iisign++){
     for(int iix=0;iix<kEndcWedgesX;iix++){
       if(iix%10 ==0)cout<<iix<<endl;
       for(int iiy=0;iiy<kEndcWedgesY;iiy++){
	 lc_endc_vs_time[iix][iiy][iisign]->GetYaxis()->SetTitle("LC");
	 lc_endc_vs_time[iix][iiy][iisign]->GetXaxis()->SetTitle("time");
	 lc_endc_vs_time[iix][iiy][iisign]->GetXaxis()->SetTimeDisplay(1);
	 lc_endc_vs_time[iix][iiy][iisign]->GetXaxis()->SetTimeFormat("%d-%m %H:%M");
	 lc_endc_vs_time[iix][iiy][iisign]->SetMarkerSize(1.);
	 lc_endc_vs_time[iix][iiy][iisign]->SetMarkerColor(4);
	 lc_endc_vs_time[iix][iiy][iisign]->SetMarkerStyle(20);

	 
	 lc_endc_vs_time[iix][iiy][iisign]->Write();

	 delete	 lc_endc_vs_time[iix][iiy][iisign];

       }
     }
   }

   cout<<"creating  histos"<<endl;


   float etEtaRef=0.2316323;



   for(int iisign=0;iisign<kSides;iisign++){
     for(int iix=0;iix<kEndcWedgesX;iix++){
      if(iix%10 ==0)cout<<iix<<endl;
       for(int iiy=0;iiy<kEndcWedgesY;iiy++){
	 stringstream etEtaRefName_endc_Stream;
	 etEtaRefName_endc_Stream<<"etOverEtaRefVsTime_endc_"<<iix+1<<"_"<<iiy+1<<"_"<<iisign;
	 et_etaRef_endc_vs_time[iix][iiy][iisign]=new TH2F(etEtaRefName_endc_Stream.str().c_str(),etEtaRefName_endc_Stream.str().c_str(),100,1312833000.,1314353000.,100,0,1./etEtaRef);
	 

	 for(int iBinX=1;iBinX<=et_endc_vs_time[iix][iiy][iisign]->GetXaxis()->GetNbins();iBinX++){
	   for(int iBinY=1;iBinY<=et_endc_vs_time[iix][iiy][iisign]->GetYaxis()->GetNbins();iBinY++){
	     et_etaRef_endc_vs_time[iix][iiy][iisign]->SetBinContent(iBinX,iBinY,et_endc_vs_time[iix][iiy][iisign]->GetBinContent(iBinX,iBinY));
	   }
	 }
	 
	 delete et_endc_vs_time[iix][iiy][iisign];
	 
	 et_etaRef_endc_vs_time[iix][iiy][iisign]->GetYaxis()->SetTitle("et/et_{|#eta=1|}");
	 et_etaRef_endc_vs_time[iix][iiy][iisign]->GetXaxis()->SetTitle("time");
	 et_etaRef_endc_vs_time[iix][iiy][iisign]->GetXaxis()->SetTimeDisplay(1);
	 et_etaRef_endc_vs_time[iix][iiy][iisign]->GetXaxis()->SetTimeFormat("%d-%m %H:%M");
	 et_etaRef_endc_vs_time[iix][iiy][iisign]->SetMarkerSize(1.);
	 et_etaRef_endc_vs_time[iix][iiy][iisign]->SetMarkerColor(4);
	 et_etaRef_endc_vs_time[iix][iiy][iisign]->SetMarkerStyle(20);
	 
	 et_etaRef_endc_vs_time[iix][iiy][iisign]->Write();	 
	 delete et_etaRef_endc_vs_time[iix][iiy][iisign];
       }
     }
   }

   

   counterHisto->Write();  
   endcfile->Write();
   endcfile->Close();

}
