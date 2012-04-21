#define kFactorFitter_endc_cxx
#include "kFactorFitter_endc.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

void kFactorFitter_endc::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L kFactorFitter_endc.C
//      Root > kFactorFitter_endc t
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

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;

   int ibin=0;
   float miscal_vec[kNMiscalBinsEE];

   kFactorGraphs_endc graphs;
   graphs.Reset();

  TH2F xtalkSumMap("xtalkSumMap","xtalkSumMap",360,0.5,360.5,171,-85.5,85.5);

  int oldRing=-1;

   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      miscal_vec[ibin]=miscal;
      if(ring!=oldRing)ibin=0;
      if(etsum>0){
	graphs.etsum_ring[ring][ibin]+=etsum;
      }
      //      cout<<ibin<<" miscal "<<miscal_vec[ibin]<<" "<<graphs.etsum_ring[ring][ibin]<<" ring "<<ring<<endl;
      ibin++;
      oldRing=ring;
   }

   TFile *kFactorFitterFile=TFile::Open(outFileName,"recreate");

   kFactorFitterFile->cd();
   int ring_index;
   float kfactor_;

   TTree* outTree= new TTree("kFactors_endc","kFactors_endc");
   outTree->Branch("ring",&ring_index,"ring/I");
   outTree->Branch("kfactor",&kfactor_,"kfactor/F");

   int middlebin =  int (kNMiscalBinsEE/2);
   
   for(int iring=0;iring<kRings;++iring){
     float middleBinEtSum=(float)graphs.etsum_ring[iring][middlebin];
     for(int iibin=0;iibin<kNMiscalBinsEE;++iibin){
       graphs.etsum_ring[iring][iibin]=(float)graphs.etsum_ring[iring][iibin]/middleBinEtSum-1;
       cout<<graphs.etsum_ring[iring][iibin]<<endl; 
     }
     
     stringstream name_etsum_ring;
     name_etsum_ring<<"etsum_ring_"<<iring+1;
     graphs.kFactorGraphsSum_endc[iring]=new TGraph (kNMiscalBinsEE,miscal_vec,graphs.etsum_ring[iring]);
     graphs.kFactorGraphsSum_endc[iring]->SetName(name_etsum_ring.str().c_str());
     graphs.kFactorGraphsSum_endc[iring]->SetTitle(name_etsum_ring.str().c_str());
     graphs.kFactorGraphsSum_endc[iring]->Fit("pol1","Q");
     graphs.kFactorGraphsSum_endc[iring]->Write();
     ring_index=iring+1;
     kfactor_=graphs.kFactorGraphsSum_endc[iring]->GetFunction("pol1")->GetParameter(1);
     outTree->Fill();
     cout<<"kFactor iring="<<iring<<" "<<graphs.kFactorGraphsSum_endc[iring]->GetFunction("pol1")->GetParameter(1)<<endl;
     delete graphs.kFactorGraphsSum_endc[iring];
     


   }

   outTree->Write();
   kFactorFitterFile->Close();

}
