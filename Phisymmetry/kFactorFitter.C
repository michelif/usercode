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
#include "TMultiGraph.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define kBarlRings 1
#define kBarlWedges 1
//#define kBarlRings 85
//#define kBarlWedges 360
#define kSides 2

using namespace std;

void FitKFactors (){
  TFile* kFactorGraph=TFile::Open("kMultiGraph_barl.root","recreate");
  TMultiGraph* kFactorGraphs_barl[kBarlRings][kBarlWedges][kSides];
  TMultiGraph* kFactorGraphsRatio_barl[kBarlRings][kBarlWedges][kSides];
  TMultiGraph* kFactorGraphsnHits_barl[kBarlRings][kBarlWedges][kSides];

  for(int iisign=0;iisign<kSides;iisign++){
    for(int iieta=0;iieta<kBarlRings;iieta++){
      if(iieta%10 ==0)cout<<iieta<<endl;
      for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	if(iiphi%10 ==0)cout<<"iiphi "<<iiphi<<endl;
	int sum=0;
	stringstream nameK;
	nameK<<"k_barl_phi_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;

	stringstream nameKRatio;
	nameKRatio<<"k_barl_ratio_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
		
	stringstream nameKnHits;
	nameKnHits<<"k_barl_nhits_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	
	stringstream multinameK;
	multinameK<<"multi"<<nameK.str();
   
	stringstream multinameKRatio;
	multinameKRatio<<"multi"<<nameKRatio.str();

	stringstream multinameKnHits;
	multinameKnHits<<"multi"<<nameKnHits.str();


	kFactorGraphs_barl[iieta][iiphi][iisign]=new TMultiGraph(multinameK.str().c_str(),multinameK.str().c_str());
	kFactorGraphsRatio_barl[iieta][iiphi][iisign]=new TMultiGraph(multinameKRatio.str().c_str(),multinameKRatio.str().c_str());
	kFactorGraphsnHits_barl[iieta][iiphi][iisign]=new TMultiGraph(multinameKnHits.str().c_str(),multinameKnHits.str().c_str());

	std::ifstream fileListK("filelist_kFactors.txt", ios::in);
	
	if (!fileListK.is_open()) {
	  cout<<"file not found"<<endl;
	  exit(0);
	}
	
	while(!fileListK.eof() ){
	  string nameFile;
	  getline(fileListK,nameFile);
	  TFile * dummyFile;
	  if(nameFile.size()!=0) {
	    dummyFile=TFile::Open(nameFile.c_str());
	    //   cout<<"begin loop for file "<<sum<<endl;
	    
	    TGraphErrors* dummyGraph;
	    dummyFile->GetObject(nameK.str().c_str(),dummyGraph);//get object check if graph exists
	    ostringstream dummy2;
	    dummy2<<nameK.str()<<sum;
	    
	    TGraphErrors* dummyGraphRatio;
	    dummyFile->GetObject(nameKRatio.str().c_str(),dummyGraphRatio);
	    ostringstream dummy2Ratio;
	    dummy2Ratio<<nameKRatio.str()<<sum;
	
	    TGraphErrors* dummyGraphnHits;
	    dummyFile->GetObject(nameKnHits.str().c_str(),dummyGraphnHits);
	    ostringstream dummy2nHits;
	    dummy2nHits<<nameKnHits.str()<<sum;
	    

	    if(dummyGraph){
	      dummyGraph->SetTitle(dummy2.str().c_str());
	      dummyGraph->SetName(dummy2.str().c_str());
	      
	      kFactorGraphs_barl[iieta][iiphi][iisign]->Add(dummyGraph);
	    }

	    if(dummyGraphRatio){
	      dummyGraphRatio->SetTitle(dummy2Ratio.str().c_str());
	      dummyGraphRatio->SetName(dummy2Ratio.str().c_str());
	      
	      kFactorGraphsRatio_barl[iieta][iiphi][iisign]->Add(dummyGraphRatio);
	    }


	    if(dummyGraphnHits){
	      dummyGraphnHits->SetTitle(dummy2nHits.str().c_str());
	      dummyGraphnHits->SetName(dummy2nHits.str().c_str());
	      
	      kFactorGraphsnHits_barl[iieta][iiphi][iisign]->Add(dummyGraphnHits);
	    }

	  }
	  //   delete dummyGraph;
	  //	  delete dummyFile;
	  sum++;
	  //	  cout<<sum<<endl;
	} 

 
	kFactorGraph->cd();
	cout<<iieta<<" "<<iiphi<<" "<<iisign<<endl;

	TCanvas* c = new TCanvas("c", "c");
	c->cd();
	cout<<"writing"<<endl;
	kFactorGraphs_barl[iieta][iiphi][iisign]->Draw("AP");
	kFactorGraphs_barl[iieta][iiphi][iisign]->Fit("pol1","Q","");
	
	//cout<<"write ok"<<endl;

	kFactorGraphs_barl[iieta][iiphi][iisign]->Write();
	c->Close();

	TCanvas* c1 = new TCanvas("c1", "c1");
	c1->cd();
	cout<<"writing"<<endl;
	kFactorGraphsRatio_barl[iieta][iiphi][iisign]->Draw("AP");
	kFactorGraphsRatio_barl[iieta][iiphi][iisign]->Fit("pol1","Q","");
	
	//cout<<"write ok"<<endl;


	kFactorGraphsRatio_barl[iieta][iiphi][iisign]->Write();
	c1->Close();

	TCanvas* c2 = new TCanvas("c2", "c2");
	c2->cd();
	cout<<"writing"<<endl;
	kFactorGraphsnHits_barl[iieta][iiphi][iisign]->Draw("AP");
	kFactorGraphsnHits_barl[iieta][iiphi][iisign]->Fit("pol1","Q","");
	
	//cout<<"write ok"<<endl;

	kFactorGraphsnHits_barl[iieta][iiphi][iisign]->Write();
	c2->Close();


	delete   kFactorGraphs_barl[iieta][iiphi][iisign];
	delete   kFactorGraphsRatio_barl[iieta][iiphi][iisign];
	delete   kFactorGraphsnHits_barl[iieta][iiphi][iisign];
      }
    }
  }
  //  inputChain_endc.Add(nameFile.c_str());
  kFactorGraph->Write();
  kFactorGraph->Close();
 


}
