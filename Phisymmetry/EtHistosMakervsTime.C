#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
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

#define NJOBS_MIN 1
#define NJOBS_MAX 46
#define NJOBS NJOBS_MAX-NJOBS_MIN+1
#define NFILES 3
#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2
#define NROWSJOB kBarlRings*kBarlWedges*kSides
#define ETAREF 1


using namespace std;

void EtHistoMakerBarrelvsTime(){
  // cout<<"start"<<endl; 
  std::ifstream etsum_barl_in("etsum_barl.dat", ios::in);
  std::ifstream avgTime_barl_in("avg_time_barl.dat", ios::in);

if (!etsum_barl_in.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }

  if (!avgTime_barl_in.is_open()) {
    cout<<"time file not found"<<endl;
    exit(0);
  }


  for (int iFile =0; iFile<NFILES;iFile++){

  stringstream nameFileStream;
  nameFileStream<<"EtHistosvsTime_"<<iFile<<".root";


  TFile f(nameFileStream.str().c_str(),"recreate");


  int counter=0,counterJob=0;
  int ieta,iphi,sign,nhits;
  float et,dummy;
 
  float etMap[kBarlRings][kBarlWedges][kSides];
  int nhitsMap[kBarlRings][kBarlWedges][kSides];
  float etvsTime[kBarlRings][kBarlWedges][kSides][NJOBS];
  TGraph* graphEtvsTime[kBarlRings][kBarlWedges][kSides];

  float avgTime[NJOBS];
  float  etEtaRingRef[NJOBS];

  //  cout<<"start"<<endl;
  //initializing variables
    for(int iieta=0;iieta<kBarlRings;iieta++){
      for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	for(int iisign=0;iisign<kSides;iisign++){
	  etMap[iieta][iiphi][iisign]=0.;
	  nhitsMap[iieta][iiphi][iisign]=0;
	  for(int iJob=0;iJob<NJOBS;iJob++){
	    etvsTime[iieta][iiphi][iisign][iJob]=0.;
	    //	    cout<<"iieta "<<iieta<<" iiphi "<<iiphi<< " iisign "<<iisign<<" iJob "<<iJob<<endl; 
	  }
	}
      }
    }


  
  while(counterJob< NJOBS){
    //    cout<<"entering"<<endl;
    int counterRow=0;

    while(counterRow<NROWSJOB){
      etsum_barl_in>> dummy>>ieta >> iphi >> sign >> et >> nhits;
      counter++;
      if(counter%20000==0)     cout<<"counter= "<<counter<<endl;
      counterRow++;
      if(et !=0 && nhits!=0){
	etMap[ieta][iphi][sign]=et;
	nhitsMap[ieta][iphi][sign]=nhits;
      }
    }

    cout<<"counterRow "<<counterRow<<"counterJob "<<counterJob<<endl;
    counterRow=0;
    avgTime_barl_in>>avgTime[counterJob];
    for(int iieta=0;iieta<kBarlRings;iieta++){
      for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	for(int iisign=0;iisign<kSides;iisign++){
	  if(nhitsMap[iieta][iiphi][iisign] !=0) etvsTime[iieta][iiphi][iisign][counterJob]= (float)etMap[iieta][iiphi][iisign]/nhitsMap[iieta][iiphi][iisign];
	
	  etMap[iieta][iiphi][iisign]=0.;  
	  nhitsMap[iieta][iiphi][iisign]=0;  
	}
      }
    }

    //calculate et mean in the eta ring=1
    int nEtaRingRef=0;
    etEtaRingRef[counterJob]=0.;
    for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
      for(int iisign=0;iisign<kSides;iisign++){
	if(etvsTime[ETAREF-1][iiphi][iisign][counterJob] !=0){
	etEtaRingRef[counterJob]+=etvsTime[ETAREF-1][iiphi][iisign][counterJob];

	nEtaRingRef++;
	}
      }
    } 

    if (nEtaRingRef!=0)etEtaRingRef[counterJob]=(float)etEtaRingRef[counterJob]/nEtaRingRef;    
    cout<<"etEtaRingRef "<<etEtaRingRef[counterJob]<<endl;	
    counterJob++;

  }

  //calculate energy ratio beetween e_i/e_ref
  for(int iJob=0;iJob<NJOBS;iJob++){
    for(int iieta=0;iieta<kBarlRings;iieta++){
      for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	for(int iisign=0;iisign<kSides;iisign++){
	  if(etEtaRingRef[iJob]!=0)etvsTime[iieta][iiphi][iisign][iJob]=(float)etvsTime[iieta][iiphi][iisign][iJob]/etEtaRingRef[iJob];
	}
      }
    }
  }
  

  f.cd();
  for(int iieta=0;iieta<kBarlRings;iieta++){
    cout<<"iieta "<<iieta<<endl;
    for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
      for(int iisign=0;iisign<kSides;iisign++){
	
	graphEtvsTime[iieta][iiphi][iisign]= new TGraph(NJOBS,avgTime,etvsTime[iieta][iiphi][iisign]);  
	string dummyString;
	stringstream dummyStream;
	dummyStream<<"EtvsEta_"<<iFile<<"_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	dummyString=dummyStream.str();
	graphEtvsTime[iieta][iiphi][iisign]->GetYaxis()->SetTitle("et/et_{|#eta=1|}");
	graphEtvsTime[iieta][iiphi][iisign]->GetXaxis()->SetTitle("unixtime");
	graphEtvsTime[iieta][iiphi][iisign]->GetXaxis()->SetTimeDisplay(1);
	graphEtvsTime[iieta][iiphi][iisign]->GetXaxis()->SetTimeFormat("%d-%m %H:%M");
	graphEtvsTime[iieta][iiphi][iisign]->SetMarkerSize(1.);
	graphEtvsTime[iieta][iiphi][iisign]->SetMarkerColor(4);
	graphEtvsTime[iieta][iiphi][iisign]->SetMarkerStyle(20);
	graphEtvsTime[iieta][iiphi][iisign]->SetName(dummyString.c_str());
	graphEtvsTime[iieta][iiphi][iisign]->SetTitle(dummyString.c_str());
	graphEtvsTime[iieta][iiphi][iisign]->Write();
	for(int iiJob=0;iiJob<NJOBS;iiJob++){
	  etvsTime[iieta][iiphi][iisign][iiJob]=0.;
	}
	delete graphEtvsTime[iieta][iiphi][iisign];
      }
    }
  }
  
  f.Write();
  f.Close();
  }

}
