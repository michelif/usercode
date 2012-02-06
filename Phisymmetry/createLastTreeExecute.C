{
  gROOT->Reset();
  TFile* f=TFile::Open("createHistoryOut_2011AB.root");
  TTree* inputTree=(TTree*)f->Get("tree_barl");
  // gSystem->Load("libJSON.so");
  gROOT->ProcessLine(".L lumiIntervals.C++");
  gROOT->ProcessLine(".L createLastTree.C++");
  createLastTree t(inputTree);
  t.setLumiIntervals("readMap_out_barl_2011AB.root");  
  t.setOutfile("finalOutput_2011AB.root");
  t.Loop();
  cout<<"all done"<<endl;
  
}
