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
#include "kFactorFitter.h"

using namespace std;

void kFactorFitter::FitKFactors (){
  TH2F xtalkMap("xtalkMap","xtalkMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalkRatioMap("xtalkRatioMap","xtalkRatioMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalknHitsMap("xtalknHitsMap","xtalknHitsMap",360,0.5,360.5,171,-85.5,85.5);



  TFile* kFactorGraph=TFile::Open("kMultiGraph_barl.root","recreate");

  /* struct kFactorGraphs{
    TMultiGraph* kFactorGraphs_barl[kBarlWedges][kSides];
    TMultiGraph* kFactorGraphsRatio_barl[kBarlWedges][kSides];
    TMultiGraph* kFactorGraphsnHits_barl[kBarlWedges][kSides];

    };*/


  std::vector<kFactorGraphs> graphs;
  graphs.reserve(kBarlRings);


  for(int iisign=0;iisign<kSides;iisign++){
    for(int iieta=0;iieta<kBarlRings;iieta++){
      if(iieta%10 ==0)cout<<iieta<<endl;
      for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	if(iiphi%10 ==0)cout<<"iiphi "<<iiphi<<endl;
	int sum=0;
	//	cout<<"starting"<<endl;
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


	graphs[iieta].kFactorGraphs_barl[iiphi][iisign]=new TMultiGraph(multinameK.str().c_str(),multinameK.str().c_str());
	//	cout<<"end"<<endl;
	graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]=new TMultiGraph(multinameKRatio.str().c_str(),multinameKRatio.str().c_str());
	graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]=new TMultiGraph(multinameKnHits.str().c_str(),multinameKnHits.str().c_str());


	std::ifstream fileListK("filelist_kFactors_2011A_2.txt", ios::in);
	
	if (!fileListK.is_open()) {
	  cout<<"file not found"<<endl;
	  exit(0);
	}
	
	while(!fileListK.eof() && sum <80 ){
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
	      
	      graphs[iieta].kFactorGraphs_barl[iiphi][iisign]->Add(dummyGraph);
	    }

	    if(dummyGraphRatio){
	      dummyGraphRatio->SetTitle(dummy2Ratio.str().c_str());
	      dummyGraphRatio->SetName(dummy2Ratio.str().c_str());
	      
	      graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->Add(dummyGraphRatio);
	    }


	    if(dummyGraphnHits){
	      dummyGraphnHits->SetTitle(dummy2nHits.str().c_str());
	      dummyGraphnHits->SetName(dummy2nHits.str().c_str());
	      
	      graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->Add(dummyGraphnHits);
	    }

	  }
	  //   delete dummyGraph;
	  //	  delete dummyFile;
	  sum++;
	  //	    cout<<sum<<endl;
	} 

 
	kFactorGraph->cd();
	cout<<iieta<<" "<<iiphi<<" "<<iisign<<endl;

	TCanvas* c = new TCanvas("c", "c");
	c->cd();
	cout<<"writing"<<endl;
	graphs[iieta].kFactorGraphs_barl[iiphi][iisign]->Draw("AP");
	graphs[iieta].kFactorGraphs_barl[iiphi][iisign]->Fit("pol1","Q","");
	
	//cout<<"write ok"<<endl;
	int theSign=iisign>0 ? 1 : -1;
	xtalkMap.Fill(iiphi+1,iieta*theSign,graphs[iieta].kFactorGraphs_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	graphs[iieta].kFactorGraphs_barl[iiphi][iisign]->Write();

	c->Close();

	TCanvas* c1 = new TCanvas("c1", "c1");
	c1->cd();
	//	cout<<"writing"<<endl;
	graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->Draw("AP");
	graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->Fit("pol1","Q","");
	xtalkRatioMap.Fill(iiphi+1,iieta*theSign,graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));	
	cout<<graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1)<<endl;
	//cout<<"write ok"<<endl;


	graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->Write();
	c1->Close();

	TCanvas* c2 = new TCanvas("c2", "c2");
	c2->cd();
	//	cout<<"writing"<<endl;
	graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->Draw("AP");
	graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->Fit("pol1","Q","");
	xtalknHitsMap.Fill(iiphi+1,iieta*theSign,graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));		
	//cout<<"write ok"<<endl;

	graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->Write();
	c2->Close();


	delete   graphs[iieta].kFactorGraphs_barl[iiphi][iisign];
	delete   graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign];
	delete   graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign];
      }
    }
  }
  //  inputChain_endc.Add(nameFile.c_str());
  xtalkMap.Write();
  xtalkRatioMap.Write();
  xtalknHitsMap.Write();
  kFactorGraph->Write();
  kFactorGraph->Close();
 


}
