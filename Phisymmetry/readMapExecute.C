{
  TFile* f= TFile::Open("map_barl2011AB.root");
  TTree* intree= (TTree*)f->Get("mapTree_barl");
  gSystem->Load("libJSON.so");
  gROOT->ProcessLine(".L readMap.C++");
  readMap t(intree);
  t.setJSON("Cert_160404-180252_7TeV_PromptReco_Collisions11_JSON.txt");
  t.setOutfile("readMap_out_barl_2011AB.root");
  t.Loop();
}
