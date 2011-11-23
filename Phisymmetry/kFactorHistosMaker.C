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

void kFactorHistoMakerBarrel(){
  TFile f("kFactorHistos.root","recreate");

  //et histos                                                                                                                               
  TH1F* k_phi_barl_vs_eta=new TH1F("k_phi_barrel_vs_eta","k_phi_barrel_vs_eta",170,-85., 85.);
  TH1F* k_phi_barl_vs_phi= new TH1F("k_phi_barrel_vs_phi","k_phi_barrel_vs_phi",360,1., 360.);
  TH2F* k_phi_barl_vs_etaphi=new TH2F("k_phi_barrel_vs_etaphi","k_phi_barrel_vs_etaphi",360,1.,360.,170,-85.,85.);

  int counter=0;
  int ieta,iphi;
  double kFactor;
  std::ifstream k_phi_barl_in("k_phi_barl.dat", ios::in);

  if (!k_phi_barl_in.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }

  while(!k_phi_barl_in.eof()){
    counter++;
    if(counter%10000==0) cout<<counter<<endl;
    k_phi_barl_in>> ieta >> iphi >> kFactor;
    k_phi_barl_vs_eta->Fill(ieta+1, kFactor);
    k_phi_barl_vs_phi->Fill(iphi+1, kFactor);
    k_phi_barl_vs_etaphi->Fill(iphi+1,ieta+1,kFactor);
  }

  //label histos

  k_phi_barl_vs_eta->Scale(1./(360*NJOBS));
  k_phi_barl_vs_phi->Scale(1./(85.*NJOBS));
  k_phi_barl_vs_etaphi->Scale(1./NJOBS);
  (k_phi_barl_vs_eta->GetXaxis())->SetTitle("i#eta");
  (k_phi_barl_vs_eta->GetYaxis())->SetTitle("K");
  (k_phi_barl_vs_phi->GetXaxis())->SetTitle("i#phi");
  (k_phi_barl_vs_phi->GetYaxis())->SetTitle("K");
  (k_phi_barl_vs_etaphi->GetXaxis())->SetTitle("i#phi");
  (k_phi_barl_vs_etaphi->GetYaxis())->SetTitle("i#eta");

  TCanvas* dummyCanvas=new TCanvas("dummyCanvas","dummyCanvas",1); 
  dummyCanvas->cd();
  
  k_phi_barl_vs_eta->Draw();
  dummyCanvas->Write();
  dummyCanvas->SaveAs("./plots/png/k_phi_barl_vs_eta.png");

  k_phi_barl_vs_phi->Draw();
  dummyCanvas->Write();
  dummyCanvas->SaveAs("./plots/png/k_phi_barl_vs_phi.png");

  k_phi_barl_vs_etaphi->Draw("colz");
  dummyCanvas->Write();
  dummyCanvas->SaveAs("./plots/png/k_phi_barl_vs_etaphi.png");

  dummyCanvas->Close();

  f.cd();
  //write et histos
  k_phi_barl_vs_eta->Write();
  k_phi_barl_vs_phi->Write();
  k_phi_barl_vs_etaphi->Write();

  f.Close();
}
