#ifndef kFactorFitter_h
#define kFactorFitter_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include "TMath.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TFile.h"
#include "TTree.h"
#include <TChain.h>
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
#include "TTreeIndex.h"
#include "TChainIndex.h"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "TMultiGraph.h"

#define kBarlRings 1
#define kBarlWedges 360
#define kSides 1


class TMultiGraph;

class kFactorFitter{

 public:
  struct kFactorGraphs{
    TMultiGraph* kFactorGraphs_barl[kBarlWedges][kSides];
    TMultiGraph* kFactorGraphsRatio_barl[kBarlWedges][kSides];
    TMultiGraph* kFactorGraphsnHits_barl[kBarlWedges][kSides];

  };

  //  kFactorFitter();
  //  ~kFactorFitter();
  void FitKFactors();
};

#endif
