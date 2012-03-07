#define kFactorFitter2_cxx
#include "kFactorFitter2.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>


void kFactorFitter2::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L kFactorFitter2.C
//      Root > kFactorFitter2 t
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
   cout<<"nentries "<<nentries<<endl;
   Long64_t nbytes = 0, nb = 0;

   int ibin=0;
   float miscal_vec[kNMiscalBinsEB];

   std::vector<kFactorGraphs> graphs;
   graphs.reserve(kBarlRings);

  TH2F xtalkSumMap("xtalkSumMap","xtalkSumMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalkMeanMap("xtalkMeanMap","xtalkMeanMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalknHitsMap("xtalknHitsMap","xtalknHitsMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalkRatioMap("xtalkRatioMap","xtalkRatioMap",360,0.5,360.5,171,-85.5,85.5);



   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     if(jentry%1000000==0) std::cout<<jentry<<std::endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      miscal_vec[ibin]=miscal;
      if(etsum>0 && nhits>0){
	graphs[ieta-1].etsum_vec[iphi-1][sign][ibin]+=etsum;
	graphs[ieta-1].nhits_vec[iphi-1][sign][ibin]+=nhits;
      }
      if(etsum_first>0 && nhits_first>0){
	graphs[ieta-1].etsum_first_vec[iphi-1][sign][ibin]+=etsum_first;
	graphs[ieta-1].nhits_first_vec[iphi-1][sign][ibin]+=nhits_first;
      }
      if(etsum_second>0 && nhits_second >0){
	graphs[ieta-1].etsum_second_vec[iphi-1][sign][ibin]+=etsum_second;
	graphs[ieta-1].nhits_second_vec[iphi-1][sign][ibin]+=nhits_second;
      }
      //      cout<<miscal_vec[ibin]<<" "<<graphs[ieta-1].etsum_vec[iphi-1][sign][ibin]<<" "<<graphs[ieta-1].nhits_vec[iphi-1][sign][ibin]<<" "<<
      //	graphs[ieta-1].etsum_first_vec[iphi-1][sign][ibin]<<" "<<graphs[ieta-1].nhits_first_vec[iphi-1][sign][ibin]<<" "<<graphs[ieta-1].etsum_second_vec[iphi-1][sign][ibin]<<" "<<graphs[ieta-1].nhits_second_vec[iphi-1][sign][ibin]<<" "<<ibin<<endl;
      ibin++;
      if(ibin==kNMiscalBinsEB) ibin=0;
   }


   TFile* kFactorFitterFile= new TFile("kFactorFitterOut.root","recreate");
   kFactorFitterFile->cd();
   int middlebin =  int (kNMiscalBinsEB/2);
   
   for(int iieta=0;iieta<kBarlRings;++iieta){
     for(int iiphi=0;iiphi<kBarlWedges;++iiphi){
       for(int iisign=0;iisign<kSides;++iisign){
	 if(graphs[iieta].etsum_vec[iiphi][iisign][middlebin]!=0){
	    // cout<<graphs[iieta].etsum_vec[iiphi][iisign][middlebin]<<endl;	   
	   float middleBinEnergySum=(float)graphs[iieta].etsum_vec[iiphi][iisign][middlebin];
	    for(int iibin=0;iibin<kNMiscalBinsEB;++iibin){

	      graphs[iieta].etsum_vec[iiphi][iisign][iibin]=(float)graphs[iieta].etsum_vec[iiphi][iisign][iibin]/middleBinEnergySum-1;
	      //	      cout<<"iibin "<<iibin<<" et norm "<< graphs[iieta].etsum_vec[iiphi][iisign][iibin]<<endl;
	   }

	 stringstream name_et_sum;
	 name_et_sum<<"etsum_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	 graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign]=new TGraph (kNMiscalBinsEB,miscal_vec,graphs[iieta].etsum_vec[iiphi][iisign]);
	 graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign]->SetName(name_et_sum.str().c_str());
	 graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign]->SetTitle(name_et_sum.str().c_str());
	 graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign]->Fit("pol1","Q");
	 int theSign=iisign>0 ? 1 : -1;
	 xtalkSumMap.Fill(iiphi+1,iieta*theSign,graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	 graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign]->Write();

	 delete graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign];


	 
	 for(int iibin=0;iibin<kNMiscalBinsEB;iibin++){
	   graphs[iieta].etsum_vec[iiphi][iisign][iibin]=(float)graphs[iieta].etsum_vec[iiphi][iisign][iibin]/graphs[iieta].nhits_vec[iiphi][iisign][iibin];
	 }
	 // cout<<graphs[iieta].etsum_vec[iiphi][iisign][middlebin]<<endl;	   
	 float middleBinEnergyMean=(float)graphs[iieta].etsum_vec[iiphi][iisign][middlebin];
	 for(int iibin=0;iibin<kNMiscalBinsEB;++iibin){
	   
	   graphs[iieta].etsum_vec[iiphi][iisign][iibin]=(float)graphs[iieta].etsum_vec[iiphi][iisign][iibin]/middleBinEnergyMean-1;
	   //	      cout<<"iibin "<<iibin<<" et norm "<< graphs[iieta].etsum_vec[iiphi][iisign][iibin]<<endl;
	 }
	 
	 stringstream name_et_mean;
	 name_et_mean<<"etmean_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	 graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]=new TGraph (kNMiscalBinsEB,miscal_vec,graphs[iieta].etsum_vec[iiphi][iisign]);
	 graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->SetName(name_et_mean.str().c_str());
	 graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->SetTitle(name_et_mean.str().c_str());
	 graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->Fit("pol1","Q");

	 xtalkMeanMap.Fill(iiphi+1,iieta*theSign,graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	 graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->Write();

	 delete graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign];

	 }

	 if(graphs[iieta].nhits_vec[iiphi][iisign][middlebin]!=0){
	   float middleBinNhits=(float)graphs[iieta].nhits_vec[iiphi][iisign][middlebin];
	   for(int iibin=0;iibin<kNMiscalBinsEB;++iibin){
	     
	     graphs[iieta].nhits_vec[iiphi][iisign][iibin]=(float)graphs[iieta].nhits_vec[iiphi][iisign][iibin]/middleBinNhits-1;
	     //	      cout<<"iibin "<<iibin<<" et norm "<< graphs[iieta].nhits_vec[iiphi][iisign][iibin]<<endl;
	   }
	   
	   stringstream name_nhits;
	   name_nhits<<"nhits_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	   graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]=new TGraph (kNMiscalBinsEB,miscal_vec,graphs[iieta].nhits_vec[iiphi][iisign]);
	   graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->SetName(name_nhits.str().c_str());
	   graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->SetTitle(name_nhits.str().c_str());
	   graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->Fit("pol1","Q");
	   int theSign=iisign>0 ? 1 : -1;
	   xtalknHitsMap.Fill(iiphi+1,iieta*theSign,graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	   graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->Write();
	   
	   delete graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign];
	   
	   

	 }

       }
     }
   }

   xtalkSumMap.Write();
   xtalkMeanMap.Write();
   xtalknHitsMap.Write();
   kFactorFitterFile->Close();

}
