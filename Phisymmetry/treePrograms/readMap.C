#define readMap_cxx
#include "readMap.h"
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

#define NXTALS 61000
#define NHITSMAX 1000
#define NTRIGGERMAX NXTALS*NHITSMAX
#define MAXSTOPHOURS 12 
#define MAXSTOPTIME 3600*12

using namespace std;

void readMap::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L readMap.C
//      Root > readMap t
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

   unsigned long int unixtimeTot=0;
   long int unixtimeMean=0;
   int firstRun=0,lastRun=0,firstLumi=0, lastLumi=0, counter=0;
   float firstUnixTime=0;


   double nhitTot=0;
   unsigned long int oldunixtimeTot=0;
   long int oldunixtimeMean=0;
   int oldfirstRun=0,oldlastRun=0,oldfirstLumi=0, oldlastLumi=0, oldcounter=0;
   float dummyLastRun=0,dummyLastLumi=0;

   bool flagFirstLoop,flagTime=false;

   TTree* outTree= new TTree("outTree_barl","outTree_barl");
   outTree->Branch("firstRun",&oldfirstRun,"firstRun/I");
   outTree->Branch("lastRun",&oldlastRun,"lastRun/I");
   outTree->Branch("firstLumi",&oldfirstLumi,"firstLumi/I");
   outTree->Branch("lastLumi",&oldlastLumi,"lastLumi/I");
   outTree->Branch("unixtimeMean",&oldunixtimeMean,"unixtimeMean/i");


   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if(nhitTot==0){

	firstRun=run;
	//	cout<<" firstrun "<<firstRun<<endl;
	firstLumi=ls;
	firstUnixTime=unixtime;
      }

      nhitTot+=nhit;
      unixtimeTot+=unixtime;
      counter++;
      if(nhit!=0) {
	dummyLastRun=run;
	dummyLastLumi=ls;
      }

      if(oldfirstRun==0){
	flagFirstLoop=true;
      }else{
	flagFirstLoop=false;
      }
      //	cout<<nhitTot<<endl;      
      if( (unixtime -firstUnixTime)>MAXSTOPTIME){
	if(nhitTot<=NTRIGGERMAX/2){
	  flagTime=false;
		

	  //	  cout<<" resetting"<<endl;
	}else{
	  //	  cout<<" flag time "<<lastRun<<endl;
	  flagTime=true;

	}

	nhitTot+=NTRIGGERMAX;
	lastRun=dummyLastRun;
	lastLumi=dummyLastLumi;
	
	//	cout<<"in time"<<endl;
      }

      if(nhitTot>=NTRIGGERMAX ){
	//	cout<<" firstrun "<<firstRun<<"nhittot "<<nhitTot<<endl;
	lastRun=run;
	lastLumi=ls;
	unixtimeMean=unixtimeTot/counter;

	if(flagFirstLoop){
	oldfirstRun=firstRun;
	oldfirstLumi=firstLumi;
	oldlastRun=lastRun;
	oldlastLumi=lastLumi;
	oldunixtimeTot=unixtimeTot;
	oldcounter=counter;
	oldunixtimeMean=unixtimeMean;
	//	    cout<<" firstRun "<<oldfirstRun<<" oldlastRun "<<oldlastRun<<" fistLumi "<<oldfirstLumi<<" oldlastLumi "<<oldlastLumi<<" unix time "<<oldunixtimeMean<<" entry "<<jentry<<endl;

	}else{
	  if(!flagTime){
	    oldlastRun=lastRun;
	    oldlastLumi=lastLumi;
	    oldunixtimeMean=(oldunixtimeTot+unixtimeTot)/(oldcounter+counter);
	    outTree->Fill();
	    //cout<<" firstRun "<<oldfirstRun<<" oldlastRun "<<oldlastRun<<" fistLumi "<<oldfirstLumi<<" oldlastLumi "<<oldlastLumi<<" unix time "<<oldunixtimeMean<<" entry "<<jentry<<endl;
	    oldfirstRun=0;
	    oldfirstLumi=0;
	    oldlastRun=0;
	    oldlastLumi=0;
	    oldunixtimeTot=0;
	    oldcounter=0;
	    oldunixtimeMean=0;
	  }else{
	    oldfirstRun=firstRun;
	    oldfirstLumi=firstLumi;
	    oldlastRun=lastRun;
	    oldlastLumi=lastLumi;
	    outTree->Fill();
	    //	    cout<<" firstRun "<<oldfirstRun<<" oldlastRun "<<oldlastRun<<" fistLumi "<<oldfirstLumi<<" oldlastLumi "<<oldlastLumi<<" unix time "<<oldunixtimeMean<<" entry "<<jentry<<endl;
	    oldfirstRun=0;
	    oldfirstLumi=0;
	    oldlastRun=0;
	    oldlastLumi=0;
	    oldunixtimeTot=0;
	    oldcounter=0;
	    oldunixtimeMean=0;
	  }

	}


	nhitTot=0;
	firstRun=0;
	firstLumi=0;
	lastRun=0;
	lastLumi=0;
	unixtimeTot=0;
	unixtimeMean=0;
	counter=0;
	firstUnixTime=0;
      }
      if(jentry == (nentries -1)){
	lastRun=run;
	lastLumi=ls;
	unixtimeMean=unixtimeTot/counter;
	oldfirstRun=firstRun;
	oldfirstLumi=firstLumi;
	oldlastRun=lastRun;
	oldlastLumi=lastLumi;
	oldunixtimeTot=unixtimeTot;
	oldcounter=counter;
	oldunixtimeMean=unixtimeMean;
	outTree->Fill();
	   
      }
   }

   TFile* outFile= TFile::Open("readMap_out_barl.root","recreate");
   outFile->cd();
   outTree->Write();
   outFile->Write();
   outFile->Close();
																		  
}
