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

#define NJOBS 138.
#define NFILES 3
#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2

using namespace std;

void verifyNorm(){
  TFile f("LCverify.root","recreate");

  //et histos                                                                                                                               
  //  TH2F* LC_vs_nentries=new TH2F("LC_vs_nentries","LC_vs_nentries",4000,0,8000.,100,0.8,1.4);
  TGraph* LC_vs_nentries;

  int counter=0;
  int ieta,iphi,sign,nhits;
  float LC,LC_array[kBarlRings][kBarlWedges][kSides],LC_dim[kBarlRings*kBarlWedges*kSides];
  float nentries[kBarlRings][kBarlWedges][kSides],nentries_dim[kBarlRings*kBarlWedges*kSides];
  std::ifstream LC_barl_in("LC_barl.dat", ios::in);

  if (!LC_barl_in.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }

  while(!LC_barl_in.eof()){
    counter++;
    if(counter%10000==0) cout<<counter<<endl;
    LC_barl_in>> ieta >> iphi >> sign >> LC >> nhits;
    if(LC !=0 && nhits!=0){
      LC_array[ieta][iphi][sign]+=LC;
      nentries[ieta][iphi][sign]+=nhits;
    }
  }

  for(int iieta=0;iieta<kBarlRings;iieta++){
    for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
      for(int iisign=0;iisign<kSides;iisign++){
	if(nentries[iieta][iiphi][iisign]!=0)LC_dim[iieta+iiphi+iisign]=LC_array[iieta][iiphi][iisign]/nentries[iieta][iiphi][iisign];
	nentries_dim[iieta+iiphi+iisign]=nentries[iieta][iiphi][iisign];
      }
    }
  }
  
  LC_vs_nentries=new TGraph(kBarlRings*kBarlWedges*kSides,nentries_dim,LC_dim) ;
  (LC_vs_nentries->GetXaxis())->SetRangeUser(0.8,1.3);
  (LC_vs_nentries->GetXaxis())->SetTitle("nhits");
  (LC_vs_nentries->GetYaxis())->SetTitle("LC");
  TCanvas* dummyCanvas=new TCanvas("dummyCanvas","dummyCanvas",1); 
  dummyCanvas->cd();
  
  LC_vs_nentries->Draw("AP*");
  dummyCanvas->Write();
  dummyCanvas->SaveAs("./plots/png/LC_vs_nentries.png");


  dummyCanvas->Close();

  f.cd();
  //write et histos
  LC_vs_nentries->Write();
  f.Close();
}
