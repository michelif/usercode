#define makeControlPlots_cxx
#include "makeControlPlots.h"
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


void makeControlPlots::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L makeControlPlots.C
//      Root > makeControlPlots t
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
  if (!intervals)
    return;

   if (fChain == 0) return;

   Long64_t nentries = fChain->GetEntries();
  
   std::vector<variablesToControl> controls;
   int kIntervals=intervals->numberOfIntervals();
   controls.reserve(kIntervals);

   cout<<"creating variables"<<endl;
   for(int iinterval=0;iinterval<kIntervals;iinterval++){	  
   controls[iinterval].reset();
   }


   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      controls[time_interval-1].counter++;
      int tt=iTT(ieta,iphi,sign);
      controls[time_interval-1].nhitMean[ieta-1][sign]+=nHits;
      controls[time_interval-1].etMean[ieta-1][sign]+=et;
      controls[time_interval-1].etMeanRMS[ieta-1][sign]+=nHits*pow(RMSet,2);
      controls[time_interval-1].etMeanNoCorr[ieta-1][sign]+=etNoCorr;
      controls[time_interval-1].etMeanNoCorrRMS[ieta-1][sign]+=nHits*pow(RMSetNoCorr,2);
      controls[time_interval-1].lcMean[ieta-1][sign]+=lc;
      controls[time_interval-1].lcMeanRMS[ieta-1][sign]+=nHits*pow(RMSlc,2);
      controls[time_interval-1].counterEta[ieta-1][sign]++;
      controls[time_interval-1].nhitTowerMean[tt-1]+=nHits;
      controls[time_interval-1].etTowerMean[tt-1]+=et;
      controls[time_interval-1].etTowerMeanRMS[tt-1]+=nHits*pow(RMSet,2);
      controls[time_interval-1].etTowerMeanNoCorr[tt-1]+=etNoCorr;
      controls[time_interval-1].etTowerMeanNoCorrRMS[tt-1]+=nHits*pow(RMSetNoCorr,2);
      controls[time_interval-1].lcTowerMean[tt-1]+=lc;
      controls[time_interval-1].lcTowerMeanRMS[tt-1]+=nHits*pow(RMSlc,2);
      controls[time_interval-1].counterTower[tt-1]++;
   }

   int counterTotal=0;
   float nInterval[kIntervals];    
   float nIntervalError[kIntervals];    
   //   float nIntervalError[kInterval];
   float etTotal=0;    
   float etTotalNoCorr=0; 

   cout<<"loop ended"<<endl;
   for(int iinterval=0;iinterval<kIntervals;iinterval++){
     if(controls[iinterval].counter!=0){
       for (int i=0;i<kBarlRings;++i)
	 {
	   for(int j=0;j<kSides;++j){
	     controls[iinterval].etMean[i][j]=controls[iinterval].etMean[i][j]/controls[iinterval].counterEta[i][j];
	     controls[iinterval].etMeanRMS[i][j]=sqrt(controls[iinterval].etMeanRMS[i][j])/(controls[iinterval].nhitMean[i][j]);
	     controls[iinterval].etMeanNoCorr[i][j]= controls[iinterval].etMeanNoCorr[i][j]/controls[iinterval].counterEta[i][j];
	     controls[iinterval].etMeanNoCorrRMS[i][j]= sqrt(controls[iinterval].etMeanNoCorrRMS[i][j])/(controls[iinterval].nhitMean[i][j]);
	     controls[iinterval].lcMeanRMS[i][j]= sqrt(controls[iinterval].lcMeanRMS[i][j])/(controls[iinterval].nhitMean[i][j]);
	     controls[iinterval].lcMean[i][j]=controls[iinterval].lcMean[i][j]/controls[iinterval].counterEta[i][j];
	     controls[iinterval].nhitMean[i][j]=controls[iinterval].nhitMean[i][j]/controls[iinterval].counterEta[i][j];
	   }
	 }
       
       for (int i=0;i<kSM*kTowerPerSM;++i)
	 {
	   controls[iinterval].etTowerMean[i]=controls[iinterval].etTowerMean[i]/controls[iinterval].counterTower[i];
	   controls[iinterval].etTowerMeanRMS[i]=sqrt(controls[iinterval].etTowerMeanRMS[i])/(controls[iinterval].nhitTowerMean[i]);// i want eerror on mean
	   controls[iinterval].etTowerMeanNoCorr[i]= controls[iinterval].etTowerMeanNoCorr[i]/controls[iinterval].counterTower[i];
	   controls[iinterval].etTowerMeanNoCorrRMS[i]= sqrt(controls[iinterval].etTowerMeanNoCorrRMS[i])/(controls[iinterval].nhitTowerMean[i]);// i want eerror on mean
	   controls[iinterval].lcTowerMean[i]=controls[iinterval].lcTowerMean[i]/controls[iinterval].counterTower[i];
	   controls[iinterval].lcTowerMeanRMS[i]= sqrt(controls[iinterval].lcTowerMeanRMS[i])/(controls[iinterval].nhitTowerMean[i]);// i want eerror on mean
	   controls[iinterval].nhitTowerMean[i]=controls[iinterval].nhitTowerMean[i]/controls[iinterval].counterTower[i];
	 }
       nInterval[iinterval]=intervals->intervalTime(iinterval);
       nIntervalError[iinterval]=0.;
       //       nIntervalError[iinterval]=0;
     }
   }


   //Arrays to be used in the graph
   float etMeanArray[kIntervals];
   float etMeanRMSArray[kIntervals];
   float etMeanNoCorrArray[kIntervals];
   float etMeanNoCorrRMSArray[kIntervals];
   float lcMeanArray[kIntervals];
   float lcMeanRMSArray[kIntervals];
   float nhitMeanArray[kIntervals];
   
   float etTowerMeanArray[kIntervals];
   float etTowerMeanRMSArray[kIntervals];
   float etTowerMeanNoCorrArray[kIntervals];
   float etTowerMeanNoCorrRMSArray[kIntervals];
   float lcTowerMeanArray[kIntervals];
   float lcTowerMeanRMSArray[kIntervals];
   float nhitTowerMeanArray[kIntervals];


   int historyNormalizationInterval=20;


   TFile *outFile=TFile::Open(outFileName+"_etaRing.root","recreate");

   for (int i=0;i<kBarlRings;++i)
     {
       for(int j=0;j<kSides;++j){
	 std::cout << "Creating history for iring " <<  i+1 << "/85" << " side  "<<j+1<<"/2"<<std::endl;
	 float etref=0;
	 float etNoCorrref=0;
	 float lcref=0;
	 float nhitref=0;
	 int nref=0;
	 
	 for (int iref=-5;iref<6;++iref)
	   {
	     nref++;
	     etref+=controls[historyNormalizationInterval+iref].etMean[i][j];
	     etNoCorrref+=controls[historyNormalizationInterval+iref].etMeanNoCorr[i][j];
	     lcref+=controls[historyNormalizationInterval+iref].lcMean[i][j];
	     nhitref+=controls[historyNormalizationInterval+iref].nhitMean[i][j];
	   }
	 
	 etref=etref/nref;
	 etNoCorrref=etNoCorrref/nref;
	 lcref=lcref/nref;
	 nhitref=nhitref/nref;


	 
	 for(int iinterval=0;iinterval<kIntervals;iinterval++){
	   //	   cout<<controls[iinterval].etMeanNoCorr[i][j]<<" "<<controls[iinterval].etMeanNoCorrRMS[i][j]<<" "<<controls[iinterval].lcMean[i][j]<<" "<<controls[iinterval].lcMeanRMS[i][j]<<endl;
	   //Normalizing to time reference interval
	   float nXtalRing=controls[iinterval].counterEta[i][j];
	   etMeanArray[iinterval]=controls[iinterval].etMean[i][j]/etref;
	   etMeanRMSArray[iinterval]=controls[iinterval].etMeanRMS[i][j]/etref;
	   etMeanNoCorrArray[iinterval]=controls[iinterval].etMeanNoCorr[i][j]/etNoCorrref;
	   etMeanNoCorrRMSArray[iinterval]=controls[iinterval].etMeanNoCorrRMS[i][j]/(etNoCorrref);
	   nhitMeanArray[iinterval]=controls[iinterval].nhitMean[i][j]/nhitref;
	   lcMeanArray[iinterval]=1/(controls[iinterval].lcMean[i][j]/lcref);
	   lcMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcMean[i][j],2)*controls[iinterval].lcMeanRMS[i][j];
	   //Now normalizing other regions to reference region
	   // 	   if (i!=0)
	   // 	     {
	   // 	       etMeanArray[iinterval]=etMeanArray[i][j][iinterval]/etMeanArray[0][iinterval];
	   // 	       etMeanNoCorrArray[i][j][iinterval]=etMeanNoCorrArray[i][j][iinterval]/etMeanNoCorrArray[0][iinterval];
	   // 	       nhitMeanArray[i][j][iinterval]=nhitMeanArray[i][j][iinterval]/nhitMeanArray[0][iinterval];
	   // 	       lcMeanArray[i][j][iinterval]=lcMeanArray[i][j][iinterval]/lcMeanArray[0][iinterval];
	   // 	     }
	 }
	 
	 TGraph* nhitGraph=new TGraph(kIntervals,nInterval,&nhitMeanArray[0]);
	 TString etaLabel="ieta_";
	 TString sideLabel="_side_";
	 etaLabel+=(i+1);
	 sideLabel+=j;
	 
	 nhitGraph->SetName("nHit_"+etaLabel+sideLabel);
	 nhitGraph->SetTitle("nHit_"+etaLabel+sideLabel);
	 nhitGraph->GetYaxis()->SetTitle("nhit");
	 nhitGraph->GetXaxis()->SetTitle("interval");
	 nhitGraph->Write();
       
	 TGraphErrors* etGraph=new TGraphErrors(kIntervals,nInterval,&etMeanArray[0],nIntervalError,&etMeanRMSArray[0]);
	 etGraph->SetName("et_"+etaLabel+sideLabel);
	 etGraph->SetTitle("et_"+etaLabel+sideLabel);
	 etGraph->GetYaxis()->SetTitle("<et>");   
	 etGraph->GetXaxis()->SetTitle("interval");
       
	 etGraph->Write();
       
       
	 TGraphErrors* etNoCorrGraph=new TGraphErrors(kIntervals,nInterval,&etMeanNoCorrArray[0],nIntervalError,&etMeanNoCorrRMSArray[0]);
	 etNoCorrGraph->SetName("etNoCorr_"+etaLabel+sideLabel);
	 etNoCorrGraph->SetTitle("etNoCorr_"+etaLabel+sideLabel);
	 etNoCorrGraph->GetYaxis()->SetTitle("<etNoCorr>");   
	 etNoCorrGraph->GetXaxis()->SetTitle("interval");
       
	 etNoCorrGraph->Write();
       
       
	 TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcMeanArray[0],nIntervalError,&lcMeanRMSArray[0]);
	 lcGraph->SetName("lc_"+etaLabel+sideLabel);
	 lcGraph->SetTitle("lc_"+etaLabel+sideLabel);
	 lcGraph->GetYaxis()->SetTitle("<tl>");   
	 lcGraph->GetXaxis()->SetTitle("interval");
       
	 lcGraph->Write();

	 TGraphErrors* EtNoCorrvsLCGraph=new TGraphErrors((kIntervals-200),&lcMeanArray[200],&etMeanNoCorrArray[200],&lcMeanRMSArray[200],&etMeanNoCorrRMSArray[200]);
	 EtNoCorrvsLCGraph->SetName("EtNoCorrvsLC_"+etaLabel+sideLabel);       
	 EtNoCorrvsLCGraph->SetName("EtNoCorrvsLC_"+etaLabel+sideLabel);       
	 EtNoCorrvsLCGraph->GetYaxis()->SetTitle("<etNoCorr>");   
	 EtNoCorrvsLCGraph->GetXaxis()->SetTitle("<lc>");

	 EtNoCorrvsLCGraph->Write();
       }
     }

   outFile->Write();
   outFile->Close();
   outFile=TFile::Open(outFileName+"_itt.root","recreate");
   for (int i=0;i<kSM*kTowerPerSM;++i)
     {
       std::cout << "Creating history for itt " <<  i+1 << "/" << kSM*kTowerPerSM << std::endl;
       float etref=0;
       float etNoCorrref=0;
       float lcref=0;
       float nhitref=0;
       int nref=0;

       for (int iref=-5;iref<6;++iref)
	 {
	   nref++;
	   etref+=controls[historyNormalizationInterval+iref].etTowerMean[i];
	   etNoCorrref+=controls[historyNormalizationInterval+iref].etTowerMeanNoCorr[i];
	   lcref+=controls[historyNormalizationInterval+iref].lcTowerMean[i];
	   nhitref+=controls[historyNormalizationInterval+iref].nhitTowerMean[i];
	 }

       etref=etref/nref;
       etNoCorrref=etNoCorrref/nref;
       lcref=lcref/nref;
       nhitref=nhitref/nref;

       for(int iinterval=0;iinterval<kIntervals;iinterval++){
	 //Normalizing to time reference interval
	 etTowerMeanArray[iinterval]=controls[iinterval].etTowerMean[i]/etref;
	 etTowerMeanRMSArray[iinterval]=controls[iinterval].etTowerMeanRMS[i]/etref;
	 etTowerMeanNoCorrArray[iinterval]=controls[iinterval].etTowerMeanNoCorr[i]/etNoCorrref;
	 etTowerMeanNoCorrRMSArray[iinterval]=controls[iinterval].etTowerMeanNoCorrRMS[i]/etNoCorrref;
	 nhitTowerMeanArray[iinterval]=controls[iinterval].nhitTowerMean[i]/nhitref;
	 lcTowerMeanArray[iinterval]=1/(controls[iinterval].lcTowerMean[i]/lcref);
	 lcTowerMeanRMSArray[iinterval]=pow(lcref/controls[iinterval].lcTowerMean[i],2)*controls[iinterval].lcTowerMeanRMS[i];
	 //Now normalizing other regions to reference region
	 // 	   if (i!=0)
	 // 	     {
	 // 	       etMeanArray[iinterval]=etMeanArray[iinterval]/etMeanArray[0][iinterval];
	 // 	       etMeanNoCorrArray[iinterval]=etMeanNoCorrArray[iinterval]/etMeanNoCorrArray[0][iinterval];
	 // 	       nhitMeanArray[iinterval]=nhitMeanArray[iinterval]/nhitMeanArray[0][iinterval];
	 // 	       lcMeanArray[iinterval]=lcMeanArray[iinterval]/lcMeanArray[0][iinterval];
	 // 	     }
       }
       TGraph* nhitGraph=new TGraph(kIntervals,nInterval,&nhitTowerMeanArray[0]);
       TString etaLabel="itt_";
       etaLabel+=(i+1);
       
	 nhitGraph->SetName("nHit_"+etaLabel);
	 nhitGraph->SetTitle("nHit_"+etaLabel);
	 nhitGraph->GetYaxis()->SetTitle("nhit");
	 nhitGraph->GetXaxis()->SetTitle("interval");
	 nhitGraph->Write();
	 
	 TGraphErrors* etGraph=new TGraphErrors(kIntervals,nInterval,&etTowerMeanArray[0],nIntervalError,&etTowerMeanRMSArray[0]);
	 etGraph->SetName("et_"+etaLabel);
	 etGraph->SetTitle("et_"+etaLabel);
	 etGraph->GetYaxis()->SetTitle("<et>/<et ieta=1>");   
	 etGraph->GetXaxis()->SetTitle("interval");
	 
	 etGraph->Write();
	 
	 
	 TGraphErrors* etNoCorrGraph=new TGraphErrors(kIntervals,nInterval,&etTowerMeanNoCorrArray[0],nIntervalError,&etTowerMeanNoCorrRMSArray[0]);
	 etNoCorrGraph->SetName("etNoCorr_"+etaLabel);
	 etNoCorrGraph->SetTitle("etNoCorr_"+etaLabel);
	 etNoCorrGraph->GetYaxis()->SetTitle("<etNoCorr>");   
	 etNoCorrGraph->GetXaxis()->SetTitle("interval");
	 
	 etNoCorrGraph->Write();
	 
	 
	 TGraphErrors* lcGraph=new TGraphErrors(kIntervals,nInterval,&lcTowerMeanArray[0],nIntervalError,&lcTowerMeanRMSArray[0]);
	 lcGraph->SetName("lc_"+etaLabel);
	 lcGraph->SetTitle("lc_"+etaLabel);
	 lcGraph->GetYaxis()->SetTitle("<tl>");   
	 lcGraph->GetXaxis()->SetTitle("interval");
	 
	 lcGraph->Write();
	 

	 TGraphErrors* EtNoCorrvsLCGraph=new TGraphErrors((kIntervals-200),&lcTowerMeanArray[200],&etTowerMeanNoCorrArray[200],&lcTowerMeanRMSArray[200],&etTowerMeanNoCorrRMSArray[200]);
	 EtNoCorrvsLCGraph->SetName("EtNoCorrvsLC_"+etaLabel);       
	 EtNoCorrvsLCGraph->SetName("EtNoCorrvsLC_"+etaLabel);       
	 EtNoCorrvsLCGraph->GetYaxis()->SetTitle("<etNoCorr>");   
	 EtNoCorrvsLCGraph->GetXaxis()->SetTitle("<lc>");

	 EtNoCorrvsLCGraph->Write();       
       
     }

   outFile->Write();
   outFile->Close();


}
