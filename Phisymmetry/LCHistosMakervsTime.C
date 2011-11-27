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
#define NJOBS_MAX 34
#define NJOBS NJOBS_MAX-NJOBS_MIN+1
#define kBarlRings 85//change!
#define kBarlWedges 360
#define kSides 2
#define NROWSJOB kBarlRings*kBarlWedges*kSides

using namespace std;

void LCHistoMakerBarrelvsTime(){
  // cout<<"start"<<endl; 

  
  stringstream nameFileStream;
  nameFileStream<<"LCHistosvsTime_"<<NJOBS_MIN<<"_"<<NJOBS_MAX<<".root";


  TFile f(nameFileStream.str().c_str(),"recreate");

  int counter=0,counterJob=0;
  int ieta,iphi,sign,nhits;
  float LC;
 
  float LCmap[kBarlRings][kBarlWedges][kSides];
  int nhitsMap[kBarlRings][kBarlWedges][kSides];
  float LCvsTime[kBarlRings][kBarlWedges][kSides][NJOBS];
  TGraph* graphLCvsTime[kBarlRings][kBarlWedges][kSides];

  float avgTime[NJOBS];

  //  cout<<"start"<<endl;
  //initializing variables
    for(int iieta=0;iieta<kBarlRings;iieta++){
      for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	for(int iisign=0;iisign<kSides;iisign++){
	  LCmap[iieta][iiphi][iisign]=0.;
	  nhitsMap[iieta][iiphi][iisign]=0;
	  for(int iJob=0;iJob<NJOBS;iJob++){
	    LCvsTime[iieta][iiphi][iisign][iJob]=0.;
	    //	    cout<<"iieta "<<iieta<<" iiphi "<<iiphi<< " iisign "<<iisign<<" iJob "<<iJob<<endl; 
	  }
	}
      }
    }

  std::ifstream LC_barl_in("LC_barl.dat", ios::in);
  std::ifstream avgTime_barl_in("avg_time_barl.dat", ios::in);

  if (!LC_barl_in.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }

  if (!avgTime_barl_in.is_open()) {
    cout<<"time file not found"<<endl;
    exit(0);
  }

  while(!LC_barl_in.eof() && counterJob< NJOBS){
    //    cout<<"entering"<<endl;
    int counterRow=0;

    while(counterRow<NROWSJOB){
      LC_barl_in>> ieta >> iphi >> sign >> LC >> nhits;
      counter++;
      if(counter%20000==0)     cout<<"counter= "<<counter<<endl;
      counterRow++;
      if(LC !=0 && nhits!=0){
	LCmap[ieta][iphi][sign]=LC;
	nhitsMap[ieta][iphi][sign]=nhits;
      }
    }

    cout<<"counterRow "<<counterRow<<"counterJob "<<counterJob<<endl;
    counterRow=0;
    avgTime_barl_in>>avgTime[counterJob];
    for(int iieta=0;iieta<kBarlRings;iieta++){
      for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	for(int iisign=0;iisign<kSides;iisign++){
	  	  if(nhitsMap[iieta][iiphi][iisign] !=0) LCvsTime[iieta][iiphi][iisign][counterJob]= (float)LCmap[iieta][iiphi][iisign]/nhitsMap[iieta][iiphi][iisign];

		  //cout<<LCvsTime[iieta][iiphi][iisign][counterJob]<<endl;
	  LCmap[iieta][iiphi][iisign]=0.;  
	  nhitsMap[iieta][iiphi][iisign]=0;  
	}
      }
    }

 
    counterJob++;

  }


  f.cd();
  for(int iieta=0;iieta<kBarlRings;iieta++){
    cout<<"iieta "<<iieta<<endl;
    for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
      for(int iisign=0;iisign<kSides;iisign++){

	graphLCvsTime[iieta][iiphi][iisign]= new TGraph(NJOBS,avgTime,LCvsTime[iieta][iiphi][iisign]);  
	string dummyString;
	stringstream dummyStream;
	dummyStream<<"LCvsEta_"<<iieta<<"_"<<iiphi<<"_"<<iisign;
	dummyString=dummyStream.str();
	graphLCvsTime[iieta][iiphi][iisign]->GetYaxis()->SetTitle("LC");
	graphLCvsTime[iieta][iiphi][iisign]->GetXaxis()->SetTitle("unixtime");
	graphLCvsTime[iieta][iiphi][iisign]->SetName(dummyString.c_str());
	graphLCvsTime[iieta][iiphi][iisign]->SetTitle(dummyString.c_str());
	graphLCvsTime[iieta][iiphi][iisign]->Write();
      }
    }
  }

  f.Write();
  f.Close();

}
