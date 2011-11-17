#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>


#define MELE 0.000510998918
#define MZ 91.187
#define HITMIN 10
#define ENERGYMIN 20
#define CHIMAX 1.2
#define SUMPT 0.2
#define FBREMMAX 0.1



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

  TH1F* etsum_barl_vs_eta=new TH1F("etsum_barrel_vs_eta","etsum_barrel_vs_eta",170,-85., 85.);
  int ieta,iphi,sign;
  unsigned int nhits;
  double et;

  std::ifstream etsum_barl_in("etsummary_barl.dat", ios::in);
  
  while(!etsum_barl_in.eof()){
    etsum_barl_in>> ieta >> iphi >> sign >> et >> nhits;
    int theSign = sign==1 ? 1:-1;
    etsum_barl_vs_eta->Fill((ieta+1)*theSign, et);

  }

  f.cd();
  etsum_barl_vs_eta->Write();
  f.Close();
  
}
