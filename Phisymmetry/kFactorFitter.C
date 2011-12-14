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


#define kBarlRings 85
#define kBarlWedges 360
#define kSides 2

using namespace std;

void FitKFactors (){



  TFile* kFactorGraph=TFile::Open("kMultiGraph_barl.root","recreate");
  TMultiGraph* kFactorGraphs_barl[kBarlRings][kBarlWedges][kSides];

   for(int iisign=0;iisign<kSides;iisign++){
     for(int iieta=0;iieta<kBarlRings;iieta++){
       if(iieta%10 ==0)cout<<iieta<<endl;
       for(int iiphi=0;iiphi<kBarlWedges;iiphi++){
	 if(iiphi%10 ==0)cout<<"iiphi "<<iiphi<<endl;
	 int sum=0;
	 stringstream nameK;
	 nameK<<"k_barl_phi_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	 stringstream multinameK;
	 multinameK<<"multi"<<nameK.str();	
   
	 kFactorGraphs_barl[iieta][iiphi][iisign]=new TMultiGraph(multinameK.str().c_str(),multinameK.str().c_str());
	 std::ifstream fileListK("filelist_kFactors.txt", ios::in);
	 
	 if (!fileListK.is_open()) {
	   cout<<"file not found"<<endl;
	   exit(0);
	 }
	 
	 while(!fileListK.eof() && sum <3){
	   string nameFile;
	   getline(fileListK,nameFile);
	   TFile * dummyFile=TFile::Open(nameFile.c_str());
	   //	   cout<<"begin loop for file "<<sum<<endl;
	   
	   //	 cout<<nameK.str().c_str()<<endl;

	   TGraphErrors* dummyGraph=(TGraphErrors*)dummyFile->Get(nameK.str().c_str());
	   ostringstream dummy2;
	   dummy2<<nameK.str()<<sum;

	   dummyGraph->SetTitle(dummy2.str().c_str());
	   dummyGraph->SetName(dummy2.str().c_str());
	   //	 cout<<"get graph"<<endl;

	   kFactorGraphs_barl[iieta][iiphi][iisign]->Add(dummyGraph);

	   //	   delete dummyGraph;
	   delete dummyFile;
	   sum++;
	   //	   	   cout<<sum<<endl;
	 }  
	 kFactorGraph->cd();
	 //	 cout<<"writing"<<endl;
	 kFactorGraphs_barl[iieta][iiphi][iisign]->Write();
	 //	cout<<"write ok"<<endl;
	 delete   kFactorGraphs_barl[iieta][iiphi][iisign];
     }
     }
   }
  //  inputChain_endc.Add(nameFile.c_str());
   kFactorGraph->Write();
   kFactorGraph->Close();
 


}
