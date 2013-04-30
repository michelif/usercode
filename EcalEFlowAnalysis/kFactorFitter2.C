#define kFactorFitter2_cxx
#include "kFactorFitter2.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

float bestCut[85];

void kFactorFitter2::GetCutValues(){
  TFile *f=TFile::Open("root://eoscms//eos/cms/store/group/alca_ecalcalib/micheli/phisym/Run2011A_1/variablesTree_49_1_ct5.root");
  TTree* variablesTree_barl=(TTree*)f->Get("variablesTree_barl");
  TH1F* h[85];
  float iieta[85];

  for (int i=1;i<=85;++i)
    {
      TString histoName="h_";
      histoName+=i;
      h[i-1]=new TH1F(histoName,histoName,600,0.,1.5);
      TString drawString="et_barl>>h_";
      drawString+=i;
      TString cutString="ieta==";
      cutString+=i;
      
      variablesTree_barl->Draw(drawString,cutString);
      bestCut[i-1]=-1.;
      iieta[i-1]=0.0175/2 + (i-1)*0.0175;
      for (int ibin=40;ibin<160;++ibin)
        {
          if (h[i-1]->Integral(1,ibin)>0)
            if (fabs(h[i-1]->Integral(ibin+1,h[i-1]->GetNbinsX())/h[i-1]->Integral(1,ibin)-1.)<0.05)
              {
                bestCut[i-1]=(float)ibin/400.;
		//		std::cout << "ieta " << i << " bestCut " << bestCut[i-1] << std::endl;
		std::cout <<"middle_cut["<<i-1<<"]="<<bestCut[i-1]<<";"<<endl;

                break;
              }

        }
    }
  return;
}

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

   for(int i =0 ;i<kBarlRings;i++){

     graphs[i].Reset();
   }

  TH2F xtalkSumMap("xtalkSumMap","xtalkSumMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalkMeanMap("xtalkMeanMap","xtalkMeanMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalChiMeanMap("xtalChiMeanMap","xtalChiMeanMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalknHitsMap("xtalknHitsMap","xtalknHitsMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalkRatioMap("xtalkRatioMap","xtalkRatioMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalkRatioMeanMap("xtalkRatioMeanMap","xtalkRatioMeanMap",360,0.5,360.5,171,-85.5,85.5);
  TH2F xtalknHitsRatioMap("xtalknHitsRatioMeanMap","xtalknHitsRatioMeanMap",360,0.5,360.5,171,-85.5,85.5);

  TH1F mapOneEtaRing("mapOneEtaRing","mapOneEtaRing",100,1.5,2.5);

  float nHitsMeanXtalIeta1=0;
  float nHitsMeanXtalIeta85=0;
  float counter=0;
  int oldEta=-1,oldPhi=-1,oldSign=-1;


   for (Long64_t jentry=0; jentry<nentries;jentry++) {
     if(jentry%1000000==0) std::cout<<jentry<<std::endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      miscal_vec[ibin]=miscal;
      if(ieta!=oldEta || iphi!= oldPhi || oldSign != sign )ibin=0;
      if(etsum>0 && nhits>0 ){
	graphs[ieta-1].etsum_ring[ibin]+=etsum;
	graphs[ieta-1].nhits_ring[ibin]+=nhits;
	graphs[ieta-1].etsum_vec[iphi-1][sign][ibin]+=etsum;
	graphs[ieta-1].nhits_vec[iphi-1][sign][ibin]+=nhits;
	if(ieta==1 && iphi==1 && sign==0 && ibin==0){
	  nHitsMeanXtalIeta1+=nhits;
	  //	  counter++;
	}
	if(ieta==85 && iphi==1 && sign==0 && ibin==0){
	  nHitsMeanXtalIeta85+=nhits;
	  //	  counter++;
	}

      }
      if(etsum_first>0 && nhits_first>0){
	graphs[ieta-1].etsum_first_vec[iphi-1][sign][ibin]+=etsum_first;
	graphs[ieta-1].nhits_first_vec[iphi-1][sign][ibin]+=nhits_first;
      }
      if(etsum_second>0 && nhits_second >0){
	graphs[ieta-1].etsum_second_vec[iphi-1][sign][ibin]+=etsum_second;
	graphs[ieta-1].nhits_second_vec[iphi-1][sign][ibin]+=nhits_second;
      }
      //	cout<<miscal_vec[ibin]<<" "<<graphs[ieta-1].etsum_vec[iphi-1][sign][ibin]<<" "<<graphs[ieta-1].nhits_vec[iphi-1][sign][ibin]<<" "<<
      //	      graphs[ieta-1].etsum_first_vec[iphi-1][sign][ibin]<<" "<<graphs[ieta-1].nhits_first_vec[iphi-1][sign][ibin]<<" "<<graphs[ieta-1].etsum_second_vec[iphi-1][sign][ibin]<<" "<<graphs[ieta-1].nhits_second_vec[iphi-1][sign][ibin]<<" "<<ibin<<endl;

      ibin++;
      oldEta=ieta;
      oldPhi=iphi;
      oldSign=sign;

   }

   //   nHitsMeanXtal=nHitsMeanXtal/counter;
   cout<<"nHitsMean Xtal ieta 1: "<<nHitsMeanXtalIeta1<<endl; 
   cout<<"nHitsMean Xtal ieta 85: "<<nHitsMeanXtalIeta85<<endl; 

   TFile *kFactorFitterFile=TFile::Open(outFileName,"recreate");

   kFactorFitterFile->cd();
   int middlebin =  int (kNMiscalBinsEB/2);
   
   float nsecondVsnfirst=0;
   float counterRatio=0;
   float nsecondVsnfirstHighEta=0;
   float counterRatioHighEta=0;

   TFile *kFactors=TFile::Open("kFactors_xtals.root","recreate");	 
   kFactors->cd();
   int etaTree,phiTree,signTree;
   double kTree;
   TTree* kFactors_xtalsTree=new TTree("kFactors_xtals","kFactors_xtals");
   kFactors_xtalsTree->Branch("eta",&etaTree,"eta/I");
   kFactors_xtalsTree->Branch("phi",&phiTree,"phi/I");
   kFactors_xtalsTree->Branch("sign",&signTree,"sign/I");
   kFactors_xtalsTree->Branch("kFactor",&kTree,"k/D");

   for(int iieta=0;iieta<kBarlRings;++iieta){
	 

	 float middleBinEnergyMean_ring=(float)graphs[iieta].etsum_ring[middlebin];
	 for(int iibin=0;iibin<kNMiscalBinsEB;++iibin){
	   graphs[iieta].etsum_ring[iibin]=(float)graphs[iieta].etsum_ring[iibin]/middleBinEnergyMean_ring-1;
	 }
	 
	 stringstream name_et_mean_ring;
	 name_et_mean_ring<<"etsum_ring_"<<iieta+1;
	 graphs[iieta].kFactorGraph_ring=new TGraph (kNMiscalBinsEB,miscal_vec,graphs[iieta].etsum_ring);
	 graphs[iieta].kFactorGraph_ring->SetName(name_et_mean_ring.str().c_str());
	 graphs[iieta].kFactorGraph_ring->SetTitle(name_et_mean_ring.str().c_str());
	 graphs[iieta].kFactorGraph_ring->Fit("pol1","Q");
	 //	 int theSign=iisign>0 ? 1 : -1;
	 //	 xtalkMeanMap.Fill(iiphi+1,(iieta+1)*theSign,graphs[iieta].kFactorGraph_ring->GetFunction("pol1")->GetParameter(1));
	 //	 xtalChiMeanMap.Fill(iiphi+1,(iieta+1)*theSign,graphs[iieta].kFactorGraph_ring->GetFunction("pol1")->GetChisquare());
	 kFactorFitterFile->cd();
	 graphs[iieta].kFactorGraph_ring->Write();
	 cout<<"kFactor ieta="<<iieta<<" "<<graphs[iieta].kFactorGraph_ring->GetFunction("pol1")->GetParameter(1)<<endl;
	 delete graphs[iieta].kFactorGraph_ring;




	 /* 	 for(int iibin=0;iibin<kNMiscalBinsEB;iibin++){
	   graphs[iieta].etmean_ring[iibin]=(float)graphs[iieta].etsum_ring[iibin]/graphs[iieta].nhits_ring[iibin]-0.25;
	 }

	 float middleBinEnergyMean_ring=(float)graphs[iieta].etmean_ring[middlebin];
	 for(int iibin=0;iibin<kNMiscalBinsEB;++iibin){
	   graphs[iieta].etmean_ring[iibin]=(float)graphs[iieta].etmean_ring[iibin]/middleBinEnergyMean_ring-1;
	 }
	 
	 stringstream name_et_mean_ring;
	 name_et_mean_ring<<"etmean_ring_"<<iieta+1;
	 graphs[iieta].kFactorGraph_ring=new TGraph (kNMiscalBinsEB,miscal_vec,graphs[iieta].etmean_ring);
	 graphs[iieta].kFactorGraph_ring->SetName(name_et_mean_ring.str().c_str());
	 graphs[iieta].kFactorGraph_ring->SetTitle(name_et_mean_ring.str().c_str());
	 graphs[iieta].kFactorGraph_ring->Fit("pol1","Q");
	 //	 int theSign=iisign>0 ? 1 : -1;
	 //	 xtalkMeanMap.Fill(iiphi+1,(iieta+1)*theSign,graphs[iieta].kFactorGraph_ring->GetFunction("pol1")->GetParameter(1));
	 //	 xtalChiMeanMap.Fill(iiphi+1,(iieta+1)*theSign,graphs[iieta].kFactorGraph_ring->GetFunction("pol1")->GetChisquare());
	 graphs[iieta].kFactorGraph_ring->Write();
	 cout<<"kFactor ieta="<<iieta<<" "<<graphs[iieta].kFactorGraph_ring->GetFunction("pol1")->GetParameter(1)<<endl;
	 delete graphs[iieta].kFactorGraph_ring;

	 */

     for(int iiphi=0;iiphi<kBarlWedges;++iiphi){
       for(int iisign=0;iisign<kSides;++iisign){
	 if(graphs[iieta].etsum_vec[iiphi][iisign][middlebin]!=0){
	   /*
	 for(int iibin=0;iibin<kNMiscalBinsEB;iibin++){
	   graphs[iieta].etmean_vec[iiphi][iisign][iibin]=(float)graphs[iieta].etsum_vec[iiphi][iisign][iibin]/graphs[iieta].nhits_vec[iiphi][iisign][iibin]-0.25;
	 }

	 float middleBinEnergyMean=(float)graphs[iieta].etmean_vec[iiphi][iisign][middlebin];
	 for(int iibin=0;iibin<kNMiscalBinsEB;++iibin){
	   
	   graphs[iieta].etmean_vec[iiphi][iisign][iibin]=(float)graphs[iieta].etmean_vec[iiphi][iisign][iibin]/middleBinEnergyMean-1;
	 }
	 
	 stringstream name_et_mean;
	 name_et_mean<<"etmean_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	 graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]=new TGraph (kNMiscalBinsEB,miscal_vec,graphs[iieta].etmean_vec[iiphi][iisign]);
	 graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->SetName(name_et_mean.str().c_str());
	 graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->SetTitle(name_et_mean.str().c_str());
	 graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->Fit("pol1","Q");
	 int theSign=iisign>0 ? 1 : -1;
	 xtalkMeanMap.Fill(iiphi+1,(iieta+1)*theSign,graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	 xtalChiMeanMap.Fill(iiphi+1,(iieta+1)*theSign,graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->GetFunction("pol1")->GetChisquare());
	 graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign]->Write();

	 delete graphs[iieta].kFactorGraphsMean_barl[iiphi][iisign];

	   */

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
	    
	    if(iieta==35) mapOneEtaRing.Fill(graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	    int theSign=iisign>0 ? 1 : -1;
	    double kf=(graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	    //	    cout<<ieta<<" "<<iphi<<" "<<iisign<<" "<<kf<<endl;
	    xtalkSumMap.Fill(iiphi+1,(iieta+1)*theSign,kf);
	    graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign]->Write();

	    etaTree=iieta+1;
	    phiTree=iiphi+1;
	    signTree=iisign;
	    kTree=kf;
	    kFactors_xtalsTree->Fill();

	    delete graphs[iieta].kFactorGraphsSum_barl[iiphi][iisign];
	    


	 }

	 /*
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
	   xtalknHitsMap.Fill(iiphi+1,(iieta+1)*theSign,graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	   graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign]->Write();
	   
	   delete graphs[iieta].kFactorGraphsnHits_barl[iiphi][iisign];
	   
	   

	 }


	 if(graphs[iieta].etsum_first_vec[iiphi][iisign][middlebin]!=0 && graphs[iieta].etsum_second_vec[iiphi][iisign][middlebin]!=0){
	   nsecondVsnfirst+=graphs[iieta].nhits_second_vec[iiphi][iisign][middlebin]/graphs[iieta].nhits_first_vec[iiphi][iisign][middlebin];  
	   counterRatio++;

	   if(iieta==79){
	   nsecondVsnfirstHighEta+=graphs[iieta].nhits_second_vec[iiphi][iisign][middlebin]/graphs[iieta].nhits_first_vec[iiphi][iisign][middlebin];  
	   counterRatioHighEta++;
	   }
	   for(int iibin=0;iibin<kNMiscalBinsEB;iibin++){//ratio of the mean energy
	     graphs[iieta].ratioMean_vec[iiphi][iisign][iibin]=graphs[iieta].etsum_second_vec[iiphi][iisign][iibin]*graphs[iieta].nhits_first_vec[iiphi][iisign][iibin]/(graphs[iieta].etsum_first_vec[iiphi][iisign][iibin]*graphs[iieta].nhits_second_vec[iiphi][iisign][iibin]);
	   }
	   
	   float middleBinRatioMean=(float)graphs[iieta].ratioMean_vec[iiphi][iisign][middlebin];

	   for(int iibin=0;iibin<kNMiscalBinsEB;++iibin){
	   graphs[iieta].ratioMean_vec[iiphi][iisign][iibin]=(float)graphs[iieta].ratioMean_vec[iiphi][iisign][iibin]/middleBinRatioMean-1;
	   }
	   
	   stringstream name_et_ratioMean;
	   name_et_ratioMean<<"etratioMean_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	   graphs[iieta].kFactorGraphsRatioMean_barl[iiphi][iisign]=new TGraph (kNMiscalBinsEB,miscal_vec,graphs[iieta].ratioMean_vec[iiphi][iisign]);
	   graphs[iieta].kFactorGraphsRatioMean_barl[iiphi][iisign]->SetName(name_et_ratioMean.str().c_str());
	   graphs[iieta].kFactorGraphsRatioMean_barl[iiphi][iisign]->SetTitle(name_et_ratioMean.str().c_str());
	   graphs[iieta].kFactorGraphsRatioMean_barl[iiphi][iisign]->Fit("pol1","Q");
	   int theSign=iisign>0 ? 1 : -1;
	   xtalkRatioMeanMap.Fill(iiphi+1,(iieta+1)*theSign,graphs[iieta].kFactorGraphsRatioMean_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	   graphs[iieta].kFactorGraphsRatioMean_barl[iiphi][iisign]->Write();
	   
	   delete graphs[iieta].kFactorGraphsRatioMean_barl[iiphi][iisign];


	   //kfactors nhits_second/nhits_first
	   for(int iibin=0;iibin<kNMiscalBinsEB;iibin++){//ratio of the mean energy
	     graphs[iieta].ratio_nhits_vec[iiphi][iisign][iibin]=graphs[iieta].nhits_second_vec[iiphi][iisign][iibin]/graphs[iieta].nhits_first_vec[iiphi][iisign][iibin];
	   }
	   
	   float middleBinRatioNhits=(float)graphs[iieta].ratio_nhits_vec[iiphi][iisign][middlebin];

	   for(int iibin=0;iibin<kNMiscalBinsEB;++iibin){
	   graphs[iieta].ratio_nhits_vec[iiphi][iisign][iibin]=(float)graphs[iieta].ratio_nhits_vec[iiphi][iisign][iibin]/middleBinRatioNhits-1;
	   }
	   
	   stringstream name_nhits_ratio;
	   name_nhits_ratio<<"nhitsRatio_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	   graphs[iieta].kFactorGraphsnHitsRatio_barl[iiphi][iisign]=new TGraph (kNMiscalBinsEB,miscal_vec,graphs[iieta].ratio_nhits_vec[iiphi][iisign]);
	   graphs[iieta].kFactorGraphsnHitsRatio_barl[iiphi][iisign]->SetName(name_nhits_ratio.str().c_str());
	   graphs[iieta].kFactorGraphsnHitsRatio_barl[iiphi][iisign]->SetTitle(name_nhits_ratio.str().c_str());
	   graphs[iieta].kFactorGraphsnHitsRatio_barl[iiphi][iisign]->Fit("pol1","Q");
	   xtalknHitsRatioMap.Fill(iiphi+1,(iieta+1)*theSign,graphs[iieta].kFactorGraphsnHitsRatio_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	   graphs[iieta].kFactorGraphsnHitsRatio_barl[iiphi][iisign]->Write();
	   
	   delete graphs[iieta].kFactorGraphsnHitsRatio_barl[iiphi][iisign];

	   //kfactor etsum a/etsum b	   
	   for(int iibin=0;iibin<kNMiscalBinsEB;iibin++){
	     graphs[iieta].ratio_vec[iiphi][iisign][iibin]=(float)graphs[iieta].etsum_second_vec[iiphi][iisign][iibin]/graphs[iieta].etsum_first_vec[iiphi][iisign][iibin];
	   }
	   
	   float middleBinEnergyRatio=(float)graphs[iieta].ratio_vec[iiphi][iisign][middlebin];
	   for(int iibin=0;iibin<kNMiscalBinsEB;++iibin){
	     
	   graphs[iieta].ratio_vec[iiphi][iisign][iibin]=(float)graphs[iieta].ratio_vec[iiphi][iisign][iibin]/middleBinEnergyRatio-1;
	   }
	   
	   stringstream name_et_ratio;
	   name_et_ratio<<"etratio_"<<iieta+1<<"_"<<iiphi+1<<"_"<<iisign;
	   graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]=new TGraph (kNMiscalBinsEB,miscal_vec,graphs[iieta].ratio_vec[iiphi][iisign]);
	   graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->SetName(name_et_ratio.str().c_str());
	   graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->SetTitle(name_et_ratio.str().c_str());
	   graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->Fit("pol1","Q");

	   xtalkRatioMap.Fill(iiphi+1,(iieta+1)*theSign,graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->GetFunction("pol1")->GetParameter(1));
	   graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign]->Write();
	   
	   delete graphs[iieta].kFactorGraphsRatio_barl[iiphi][iisign];
	   
	 }
	 */
       }
     }
   }

   cout<<"ratio "<<nsecondVsnfirst/counterRatio<<"+-"<<sqrt(nsecondVsnfirst*(1+nsecondVsnfirst/counterRatio))/counterRatio<<endl;
   cout<<"ratio High Eta "<<nsecondVsnfirstHighEta/counterRatioHighEta<<"+-"<<sqrt(nsecondVsnfirstHighEta*(1+nsecondVsnfirstHighEta/counterRatioHighEta))/counterRatioHighEta<<endl;


   xtalkSumMap.Write();
   xtalkMeanMap.Write();
   xtalChiMeanMap.Write();
   xtalknHitsMap.Write();
   xtalkRatioMap.Write();
   xtalkRatioMeanMap.Write();
   mapOneEtaRing.Write();
   kFactorFitterFile->Close();

   kFactors->cd();
   kFactors_xtalsTree->Write();
   kFactors->Close();

}
