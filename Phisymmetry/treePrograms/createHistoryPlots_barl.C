#define createHistoryPlots_barl_cxx
#include "createHistoryPlots_barl.h"
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



#define kBarlRings 2
#define kBarlWedges 360
#define kSides 1
#define kIntervals 382

#define etaStart 1
#define phiStart 1
#define sideStart 1

#define startFromZero true
/*
#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2
#define kIntervals 22

#define etaStart 1
#define phiStart 1
#define sideStart 1
#define startFromZero true
*/

vector<unsigned int> frvec;
vector<unsigned int> lrvec;
vector<unsigned int> flvec;
vector<unsigned int> llvec;



int  createHistoryPlots_barl::GetInterval(unsigned int r, unsigned int ls){
  int interval=-100;
  int i=0;
  while( i <frvec.size()){
    
    if(r>frvec[i] && r<lrvec[i]){
      
 
      interval =i;
      i=frvec.size();
      //      cout<<"in interval "<<interval<<endl;
    } else  if(r ==frvec[i]){
      if(ls>= flvec[i] && ls< flvec[i] ){
	//        cout<<run<<" "<<ls<< " frvec "<<frvec[i]<<" lrvec "<<lrvec[i]<< " flvec "<<flvec[i]<<" llvec "<<llvec[i]<<endl;  
	interval=i;
	i=frvec.size();
      }
    }
    i++;
  }
  return interval;

}

void createHistoryPlots_barl::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L createHistoryPlots_barl.C
//      Root > createHistoryPlots_barl t
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
   cout<<"starting"<<endl;
   Long64_t nentries = fChain->GetEntries();
   cout<<"nentries "<<nentries<<endl;
   //######Reading the tree with time intervals###########


   cout<<"reading the map"<<endl;
   TFile* f= TFile::Open("readMap_out_barl_2011A.root","r");
   TTree* intervalsTree= (TTree*)f->Get("outTree_barl");


   //   map<pair<int,int>,pair<int,int> > ;
   int fr,lr,fl,ll;

   TBranch *b_firstRun=intervalsTree->GetBranch("firstRun");
   TBranch *b_lastRun=intervalsTree->GetBranch("lastRun");
   TBranch *b_firstLumi=intervalsTree->GetBranch("firstLumi");
   TBranch *b_lastLumi=intervalsTree->GetBranch("lastLumi");
   
   intervalsTree->SetBranchAddress("firstRun", &fr, &b_firstRun);
   intervalsTree->SetBranchAddress("lastRun", &lr, &b_lastRun);
   intervalsTree->SetBranchAddress("firstLumi", &fl, &b_firstLumi);
   intervalsTree->SetBranchAddress("lastLumi", &ll, &b_lastLumi);



   //   Long64_t nbytes_int = 0, nb_int = 0;
   int nentries_int = intervalsTree->GetEntries();
   for(int jentry=0;jentry<nentries_int;++jentry){
     if(jentry%100000==0) std::cout<<jentry<<std::endl;
     intervalsTree->GetEntry(jentry);
     frvec.push_back(fr);
     lrvec.push_back(lr);
     flvec.push_back(fl);
     llvec.push_back(ll);

     //     cout<<frvec[jentry]<<" "<<lrvec[jentry]<<" "<<flvec[jentry]<<" "<<llvec[jentry]<<endl;

   }



   float energySum[kBarlRings][kBarlWedges][kSides][kIntervals];
   float energySquared[kBarlRings][kBarlWedges][kSides][kIntervals];
   float lasercorrSum[kBarlRings][kBarlWedges][kSides][kIntervals];
   float lasercorrSquared[kBarlRings][kBarlWedges][kSides][kIntervals];
   int counter[kBarlRings][kBarlWedges][kSides][kIntervals];

   /*  
       float   energy[kBarlRings][kBarlWedges][kSides][kIntervals][MAXHITS];
   float   lasercorr[kBarlRings][kBarlWedges][kSides][kIntervals];
   */
   cout<<"creating histos"<<endl;
   for(int iisign=0;iisign<kSides;iisign++){
     for(int iieta=0;iieta<kBarlRings;iieta++){
       if(iieta%10 ==0) cout<<iieta<<endl;
       for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	 //	 if(iiphi%30 ==0) cout<<"phi"<<iiphi<<endl;
	 for(int iinterval=0;iinterval<kIntervals;iinterval++){
	  
	   /*   stringstream energyname;
	   energyname<<"energy_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign<<"_"<<iinterval;                                        
	   energy[iieta][iiphi][iisign][iinterval]=new TH1F(energyname.str().c_str(),energyname.str().c_str(),1000, 0.,2.);
	   
	   stringstream lcname;
	   lcname<<"lc_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign<<"_"<<iinterval;                           
	   lasercorr[iieta][iiphi][iisign][iinterval]=new TH1F(lcname.str().c_str(),lcname.str().c_str(),1000, 0.,3.);
	   */

	   counter[iieta][iiphi][iisign][iinterval]=0;
	   energySum[iieta][iiphi][iisign][iinterval]=0;
	   energySquared[iieta][iiphi][iisign][iinterval]=0;
	   lasercorrSum[iieta][iiphi][iisign][iinterval]=0;
	   lasercorrSquared[iieta][iiphi][iisign][iinterval]=0;

	   // lc[iieta][iiphi][iisign]=0; 
	   //	 energy[iieta][iiphi][iisign]=0; 
	 }
       }
     }
     


   }


   //######### creating the output tree ##############
   TFile *outFile=TFile::Open("outputForHistory_barl_2011A_1_7.root","recreate");



   
   int timeVar=0,hitVar=0,ietaVar=0,iphiVar=0,signVar=0;
   float energyVar=0,RMSenergyVar=0,lcVar=0,RMSlcVar=0;
   outFile->cd();
   TTree* outTree= new TTree("tree_barl","tree_barl");
   outTree->Branch("time_interval",&timeVar,"timeInterval/I");
   outTree->Branch("nHits",&hitVar, "nHits/i");
   outTree->Branch("ieta",&ietaVar,"ieta/I");
   outTree->Branch("iphi",&iphiVar,"iphi/I");
   outTree->Branch("sign",&signVar,"sign/I");
   outTree->Branch("energySum",&energyVar,"energySum/F");
   outTree->Branch("energySquared",&RMSenergyVar,"energySquared/F");
   outTree->Branch("lcSum",&lcVar,"lcSum/F");
   outTree->Branch("lcSquared",&RMSlcVar,"lcSquared/F");

   //######Loop over ntuples###########
   //   float lc[kBarlRings][kBarlWedges][kSides];
   //   float et[kBarlRings][kBarlWedges][kSides];
  
   
   
   cout<<"start looping over entries"<<endl;
   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     Long64_t ientry = LoadTree(jentry);
     if (ientry < 0) break;
     nb = fChain->GetEntry(jentry);   nbytes += nb;
     // if (Cut(ientry) < 0) continue;
     if(jentry%100000==0) std::cout<<jentry<<std::endl; 

     //       cout<<"entering if"<<endl;
       int interval=GetInterval(run, lumi);
       int theInterval=-1;
       if(interval >=0)	 theInterval=interval;


       for (int ihit=0;ihit<nhit;++ihit){
	 int theSign=ieta[ihit]>0 ? 1:0;
	 int theEta=TMath::Abs(ieta[ihit]);
	 int thePhi=iphi[ihit];
	 if(theSign < kSides && thePhi <=kBarlWedges && theInterval>=0 && theEta <=kBarlRings){
	   //	   cout<<et_barl[ihit];
	 energySum[theEta-1][thePhi-1][theSign][theInterval]+=et_barl[ihit];
	 energySquared[theEta-1][thePhi-1][theSign][theInterval]+=pow(et_barl[ihit],2);
	 lasercorrSum[theEta-1][thePhi-1][theSign][theInterval]+=lc_barl[ihit];
	 lasercorrSquared[theEta-1][thePhi-1][theSign][theInterval]+=pow(lc_barl[ihit],2);

	 counter[theEta-1][thePhi-1][theSign][theInterval]++;
	 }
       }

      



       
   }//close loop over entries
   
   cout<<"filling out tree"<<endl;
   for(int iisign=0;iisign<kSides;iisign++){
     for(int iieta=0;iieta<kBarlRings;iieta++){
       if(iieta%10 ==0) cout<<iieta<<endl;
       for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	 for(int iinterval=0;iinterval<kIntervals;iinterval++){
	   if(energySum[iieta][iiphi][iisign][iinterval]>0){
	     hitVar=counter[iieta][iiphi][iisign][iinterval];
	     ietaVar=iieta+1;
	     iphiVar=iiphi+1;
	     timeVar=iinterval+1;
	     signVar=iisign;
	     energyVar=energySum[iieta][iiphi][iisign][iinterval];
	     RMSenergyVar=energySquared[iieta][iiphi][iisign][iinterval];
	     lcVar=lasercorrSum[iieta][iiphi][iisign][iinterval];
	     RMSlcVar=lasercorrSquared[iieta][iiphi][iisign][iinterval];
	     //	     lcVar=lasercorr[iieta][iiphi][iisign][iinterval]->GetMean();
	     //	     RMSlcVar=lasercorr[iieta][iiphi][iisign][iinterval]->GetMeanError();
	     outTree->Fill();
	     //	     delete energy[iieta][iiphi][iisign][iinterval];   
	     //	     delete lasercorr[iieta][iiphi][iisign][iinterval];   
	   }
	 }
       }
     }
   }
   
   outTree->Write();
   outFile->Write();
   outFile->Close();
}
