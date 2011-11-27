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

using namespace std;

void LCHistoMakerBarrel(){
  TFile f("LCHistos.root","recreate");

  //et histos                                                                                                                               
  TH1F* LC_barl_vs_eta=new TH1F("LC_barrel_vs_eta","LC_barrel_vs_eta",170,-85., 85.);
  TH1F* LC_barl_vs_phi= new TH1F("LC_barrel_vs_phi","LC_barrel_vs_phi",360,1., 360.);
  TH2F* LC_barl_vs_etaphi=new TH2F("LC_barrel_vs_etaphi","LC_barrel_vs_etaphi",360,1.,360.,170,-85.,85.);

  //histos to normalize
  TH1F* nentries_barl_vs_eta=new TH1F("nentries_barrel_vs_eta","nentries_barrel_vs_eta",170,-85., 85.);
  TH1F* nentries_barl_vs_phi= new TH1F("nentries_barrel_vs_phi","nentries_barrel_vs_phi",360,1., 360.);
  TH2F* nentries_barl_vs_etaphi=new TH2F("nentries_barrel_vs_etaphi","nentries_barrel_vs_etaphi",360,1.,360.,170,-85.,85.);



  int counter=0;
  int ieta,iphi,sign,nhits;
  float LC;
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
    int theSign = sign==1 ? 1:-1;
    LC_barl_vs_eta->Fill((ieta+1)*theSign,LC);
    LC_barl_vs_phi->Fill(iphi+1, LC);
    LC_barl_vs_etaphi->Fill(iphi+1,(ieta+1)*theSign,LC);
    nentries_barl_vs_eta->Fill((ieta+1)*theSign,nhits);
    nentries_barl_vs_phi->Fill(iphi+1, nhits);
    nentries_barl_vs_etaphi->Fill(iphi+1,(ieta+1)*theSign,nhits);
    }
  }


  LC_barl_vs_eta->Divide(nentries_barl_vs_eta);
  LC_barl_vs_phi->Divide(nentries_barl_vs_phi);
  LC_barl_vs_etaphi->Divide(nentries_barl_vs_etaphi);
  (LC_barl_vs_eta->GetXaxis())->SetTitle("i#eta");
  (LC_barl_vs_eta->GetYaxis())->SetTitle("LC");
  (LC_barl_vs_phi->GetXaxis())->SetTitle("i#phi");
  (LC_barl_vs_phi->GetYaxis())->SetTitle("LC");
  (LC_barl_vs_etaphi->GetXaxis())->SetTitle("i#phi");
  (LC_barl_vs_etaphi->GetYaxis())->SetTitle("i#eta");

  (nentries_barl_vs_eta->GetXaxis())->SetTitle("i#eta");
  (nentries_barl_vs_eta->GetYaxis())->SetTitle("nentries");
  (nentries_barl_vs_phi->GetXaxis())->SetTitle("i#phi");
  (nentries_barl_vs_phi->GetYaxis())->SetTitle("nentries");
  (nentries_barl_vs_etaphi->GetXaxis())->SetTitle("i#phi");
  (nentries_barl_vs_etaphi->GetYaxis())->SetTitle("i#eta");

  TCanvas* dummyCanvas=new TCanvas("dummyCanvas","dummyCanvas",1); 
  dummyCanvas->cd();
  
  LC_barl_vs_eta->Draw();
  dummyCanvas->Write();
  dummyCanvas->SaveAs("./plots/png/LC_barl_vs_eta.png");

  LC_barl_vs_phi->Draw();
  dummyCanvas->Write();
  dummyCanvas->SaveAs("./plots/png/LC_barl_vs_phi.png");

  LC_barl_vs_etaphi->Draw("colz");
  dummyCanvas->Write();
  dummyCanvas->SaveAs("./plots/png/LC_barl_vs_etaphi.png");

  nentries_barl_vs_eta->Draw();
  dummyCanvas->Write();
  dummyCanvas->SaveAs("./plots/png/nentries_barl_vs_eta.png");

  nentries_barl_vs_phi->Draw();
  dummyCanvas->Write();
  dummyCanvas->SaveAs("./plots/png/nentries_barl_vs_phi.png");

  nentries_barl_vs_etaphi->Draw("colz");
  dummyCanvas->Write();
  dummyCanvas->SaveAs("./plots/png/nentries_barl_vs_etaphi.png");


  dummyCanvas->Close();

  f.cd();
  //write et histos
  LC_barl_vs_eta->Write();
  LC_barl_vs_phi->Write();
  LC_barl_vs_etaphi->Write();

  f.Close();
}
