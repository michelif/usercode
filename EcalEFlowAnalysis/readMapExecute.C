{
  TFile* f= TFile::Open("map_barl2012_190456-190539.root");
  TTree* intree= (TTree*)f->Get("mapTree_barl");
  gSystem->Load("libJSON.so");
  gROOT->ProcessLine(".L readMap.C++");
  readMap t(intree);
  t.setJSON("analyzed_AlCaPhiSym_Run2012A-v1_RAW.json");
  t.setOutfile("readMap_out_barl_2012_190456-190539.root");
  t.Loop();
}
