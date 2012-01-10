#define plotsVsTimeMacro_barl_cxx
#include "plotsVsTimeMacro_barl.h"
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
#define PRECISION 0.005

using namespace std;

void plotsVsTimeMacro_barl::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L plotsVsTimeMacro_barl.C
//      Root > plotsVsTimeMacro_barl t
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

   TH2F* lc_barl_vs_time[kBarlRings][kBarlWedges][kSides];
   TH2F* et_barl_vs_time[kBarlRings][kBarlWedges][kSides];
   TH2F* et_etaRef_barl_vs_time[kBarlRings][kBarlWedges][kSides];

   TH1F* energy[kBarlRings][kBarlWedges][kSides];
   int counter[kBarlRings][kBarlWedges][kSides];
   TH1F* counterHisto=new TH1F("counterhisto","counterhisto",1000,0,1000);
   TH1F* unixtimeHisto=new TH1F("unixtimehisto","unixtimehisto",10000,1312833000.,1314353000.);

   cout<<"creating histos"<<endl;
   //   int iisign=0;
   for(int iisign=0;iisign<kSides;iisign++){
    for(int iieta=0;iieta<kBarlRings;iieta++){
      if(iieta%10 ==0)cout<<iieta<<endl;
      for(int iiphi=0;iiphi<kBarlWedges;iiphi++){

	  stringstream etName_barl_Stream;
	  etName_barl_Stream<<"etVsTime_barl_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	  et_barl_vs_time[iieta][iiphi][iisign]=new TH2F(etName_barl_Stream.str().c_str(),etName_barl_Stream.str().c_str(),100,1312833000.,1314353000.,100,0,1);

	  stringstream lcName_barl_Stream;
	  lcName_barl_Stream<<"lcVsTime_barl_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;                                        
	  //	  lc_barl_vs_time[iieta][iiphi][iisign]=new TH2F(lcName_barl_Stream.str().c_str(),lcName_barl_Stream.str().c_str(),100,1312833000.,1314353000.,100,0.4,1.6);

	  stringstream energyname;
	  energyname<<"energy_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;                                        
	  energy[iieta][iiphi][iisign]=new TH1F(energyname.str().c_str(),energyname.str().c_str(),1000, 0.,2.);

	  counter[iieta][iiphi][iisign]=0;
      }
    }
  }


   int etaRef=1;
   float etEtaRef=0;
   int nEtaRef=0;
   
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry); 
     nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     if(jentry%100000==0) std::cout<<jentry<<std::endl;


     for (int ihit=0;ihit<nhit;++ihit){
       int theSign=ieta[ihit]>0 ? 1:0;
     int theEta=TMath::Abs(ieta[ihit]);
     int thePhi=iphi[ihit];
     //     cout<<(theEta-1)<<" "<<(thePhi-1)<<" "<<theSign<<endl;
     //     cout<<"lc "<<lc_barl_Branch<<"time "<<unixTime_barl_Branch<<endl;
     unixtimeHisto->Fill(unixtime);
     energy[theEta-1][thePhi-1][theSign]->Fill(et_barl[ihit]);
     counter[theEta-1][thePhi-1][theSign]++;

     if(energy[theEta-1][thePhi-1][theSign]->GetRMS()/energy[theEta-1][thePhi-1][theSign]->GetMean()<PRECISION && counter[theEta-1][thePhi-1][theSign]>10) {

       //     cout<<(theEta)<<" "<<(thePhi)<<" "<<theSign<<endl;
       //     lc_barl_vs_time[theEta-1][thePhi-1][theSign]->Fill(unixtime,lc_barl[ihit]);
     et_barl_vs_time[theEta-1][thePhi-1][theSign]->Fill(unixtimeHisto->GetMean(),energy[theEta-1][thePhi-1][theSign]->GetMean());
     energy[theEta-1][thePhi-1][theSign]->Reset();
     unixtimeHisto->Reset();
     counterHisto->Fill(counter[theEta-1][thePhi-1][theSign]);
     counter[theEta-1][thePhi-1][theSign]=0;
     }
     if(theEta == TMath::Abs(etaRef) && et_barl[ihit] != 0){
       etEtaRef+=et_barl[ihit];
       nEtaRef++;
       //       cout<<(theEta)<<" "<<(thePhi)<<" "<<theSign<<" "<<etEtaRef<< endl;
     }
     }

   }
     
   cout<<"loop ends"<<endl;    
   etEtaRef=(float)etEtaRef/nEtaRef;
   cout<<"etEtaRef "<<etEtaRef<<endl;

   TFile* barlfile=TFile::Open("barrelVsTime.root","recreate");

   cout<<"writing lc histos"<<endl;
   /* for(int iisign=0;iisign<kSides;iisign++){
     for(int iieta=0;iieta<kBarlRings;iieta++){
       if(iieta%10 ==0)cout<<iieta<<endl;
       for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	 lc_barl_vs_time[iieta][iiphi][iisign]->GetYaxis()->SetTitle("LC");
	 lc_barl_vs_time[iieta][iiphi][iisign]->GetXaxis()->SetTitle("time");
	 lc_barl_vs_time[iieta][iiphi][iisign]->GetXaxis()->SetTimeDisplay(1);
	 lc_barl_vs_time[iieta][iiphi][iisign]->GetXaxis()->SetTimeFormat("%d-%m %H:%M");
	 lc_barl_vs_time[iieta][iiphi][iisign]->SetMarkerSize(1.);
	 lc_barl_vs_time[iieta][iiphi][iisign]->SetMarkerColor(4);
	 lc_barl_vs_time[iieta][iiphi][iisign]->SetMarkerStyle(20);

	 
		 lc_barl_vs_time[iieta][iiphi][iisign]->Write();

	 delete	 lc_barl_vs_time[iieta][iiphi][iisign];

       }
     }
   }
   */
   cout<<"creating  histos"<<endl;

   for(int iisign=0;iisign<kSides;iisign++){
     for(int iieta=0;iieta<kBarlRings;iieta++){
      if(iieta%10 ==0)cout<<iieta<<endl;
       for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	 stringstream etEtaRefName_barl_Stream;
	 etEtaRefName_barl_Stream<<"etOverEtaRefVsTime_barl_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	 et_etaRef_barl_vs_time[iieta][iiphi][iisign]=new TH2F(etEtaRefName_barl_Stream.str().c_str(),etEtaRefName_barl_Stream.str().c_str(),100,1312833000.,1314353000.,100,0,1./etEtaRef);
	 

	 for(int iBinX=1;iBinX<=et_barl_vs_time[iieta][iiphi][iisign]->GetXaxis()->GetNbins();iBinX++){
	   for(int iBinY=1;iBinY<=et_barl_vs_time[iieta][iiphi][iisign]->GetYaxis()->GetNbins();iBinY++){
	     et_etaRef_barl_vs_time[iieta][iiphi][iisign]->SetBinContent(iBinX,iBinY,et_barl_vs_time[iieta][iiphi][iisign]->GetBinContent(iBinX,iBinY));
	   }
	 }
	 
	 delete et_barl_vs_time[iieta][iiphi][iisign];
	 
	 et_etaRef_barl_vs_time[iieta][iiphi][iisign]->GetYaxis()->SetTitle("et/et_{|#eta=1|}");
	 et_etaRef_barl_vs_time[iieta][iiphi][iisign]->GetXaxis()->SetTitle("time");
	 et_etaRef_barl_vs_time[iieta][iiphi][iisign]->GetXaxis()->SetTimeDisplay(1);
	 et_etaRef_barl_vs_time[iieta][iiphi][iisign]->GetXaxis()->SetTimeFormat("%d-%m %H:%M");
	 et_etaRef_barl_vs_time[iieta][iiphi][iisign]->SetMarkerSize(1.);
	 et_etaRef_barl_vs_time[iieta][iiphi][iisign]->SetMarkerColor(4);
	 et_etaRef_barl_vs_time[iieta][iiphi][iisign]->SetMarkerStyle(20);
	 
	 et_etaRef_barl_vs_time[iieta][iiphi][iisign]->Write();	 
	 delete et_etaRef_barl_vs_time[iieta][iiphi][iisign];
       }
     }
   }



   counterHisto->Write();
   barlfile->Write();
   barlfile->Close();

}
