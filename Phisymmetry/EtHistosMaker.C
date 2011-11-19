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

using namespace std;

void EtHistoMakerBarrel(){
  TFile f("EtHistos.root","recreate");

  //et histos
  TH1F* etsum_barl_vs_eta=new TH1F("etsum_barrel_vs_eta","etsum_barrel_vs_eta",170,-85., 85.);
  TH1F* etsum_barl_vs_phi= new TH1F("etsum_barrel_vs_phi","etsum_barrel_vs_phi",360,1., 360.);
  TH2F* etsum_barl_vs_etaphi=new TH2F("etsum_barrel_vs_etaphi","etsum_barrel_vs_etaphi",360,1.,360.,170,-85.,85.);
  TH1F* etsum_barl_vs_phi_vec[4];
  //xtal histos
  TH1F* nXtal_vs_eta=new TH1F("nXtal_vs_eta","nXtal_vs_eta",170,-85., 85.);
  TH1F* nXtal_vs_phi=new TH1F("nXtal_vs_phi","nXtal_vs_phi",360,1., 360.);
  TH2F* nXtal_vs_etaphi=new TH2F("nXtal_vs_etaphi","nXtal_vs_etaphi",360,1.,360.,170,-85.,85.);

  int ieta,iphi,sign;
  unsigned int nhits;
  double et;
  string histoName("etsum_barl_vs_phi_vec_");


  for(int kEtaBin=0;kEtaBin<4;kEtaBin++){
    std::stringstream out;
    out<<kEtaBin;
    string dummyString=histoName+out.str();
    //    cout<<dummyString<<endl;
    etsum_barl_vs_phi_vec[kEtaBin]=new TH1F(dummyString.c_str(), dummyString.c_str(),360,1.,360.);
  }
  
  std::ifstream etsum_barl_in("etsummary_barl.dat", ios::in);
  
  while(!etsum_barl_in.eof()){
    etsum_barl_in>> ieta >> iphi >> sign >> et >> nhits;
    int theSign = sign==1 ? 1:-1;
    if(et != 0){
      nXtal_vs_eta->Fill((ieta+1)*theSign);
      nXtal_vs_phi->Fill(iphi+1);
      nXtal_vs_etaphi->Fill(iphi+1,(ieta+1)*theSign);
    }
    etsum_barl_vs_eta->Fill((ieta+1)*theSign, et);
    etsum_barl_vs_phi->Fill(iphi+1, et);
    etsum_barl_vs_etaphi->Fill(iphi+1,(ieta+1)*theSign,et);
    for(int kEtaBin=0;kEtaBin<3;kEtaBin++){
      if(TMath::Abs(ieta+1)>=kEtaBin*20 && TMath::Abs(ieta+1)<(kEtaBin+1)*20) {
	etsum_barl_vs_phi_vec[kEtaBin]->Fill(iphi+1, et);
      }else if (TMath::Abs(ieta+1)<=85){
	etsum_barl_vs_phi_vec[3]->Fill(iphi+1, et);
      }
    }
  }  

  //normalization of etsum to the number of Xtals
  etsum_barl_vs_eta->Divide(nXtal_vs_eta);
  etsum_barl_vs_phi->Divide(nXtal_vs_phi);
  etsum_barl_vs_etaphi->Divide(nXtal_vs_etaphi);

  //label histos
  (etsum_barl_vs_eta->GetXaxis())->SetTitle("i#eta");
  (etsum_barl_vs_eta->GetYaxis())->SetTitle("E_{T}(GeV)");
  (etsum_barl_vs_phi->GetXaxis())->SetTitle("i#phi");
  (etsum_barl_vs_phi->GetYaxis())->SetTitle("E_{T}(GeV)");
  (etsum_barl_vs_etaphi->GetXaxis())->SetTitle("i#phi");
  (etsum_barl_vs_etaphi->GetYaxis())->SetTitle("i#eta");

  (nXtal_vs_eta->GetXaxis())->SetTitle("i#eta");	  
  (nXtal_vs_eta->GetYaxis())->SetTitle("NXtal");
  (nXtal_vs_phi->GetXaxis())->SetTitle("i#phi");	  
  (nXtal_vs_phi->GetYaxis())->SetTitle("NXtal");
  (nXtal_vs_etaphi->GetXaxis())->SetTitle("i#phi");
  (nXtal_vs_etaphi->GetYaxis())->SetTitle("i#eta");

  f.cd();
  //write et histos
  etsum_barl_vs_eta->Write();
  etsum_barl_vs_phi->Write();
  etsum_barl_vs_etaphi->Write();
  for(int kEtaBin=0;kEtaBin<4;kEtaBin++){
    (etsum_barl_vs_phi_vec[kEtaBin]->GetYaxis())->SetTitle("E_{T}(GeV)");
    (etsum_barl_vs_phi_vec[kEtaBin]->GetXaxis())->SetTitle("i#phi");
    etsum_barl_vs_phi_vec[kEtaBin]->Write();
  }
  //write nXtal Histos
  nXtal_vs_eta->Write();
  nXtal_vs_phi->Write();
  nXtal_vs_etaphi->Write();
  
  f.Close();
  

}
