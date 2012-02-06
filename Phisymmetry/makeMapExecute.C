{
  gROOT->Reset();
  TChain inputChain_barl("variablesTree_barl");
  

  std::ifstream fileList("filelist2011B_3.txt", ios::in);
  
  if (!fileList.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }
  
  
  
  int sum=0;
  while(!fileList.eof()){
    string nameFile;
    getline(fileList,nameFile);
  inputChain_barl.Add(nameFile.c_str());
  
  sum++;
  cout<<sum<<endl;
  }
  
  
  

  
  
  gROOT->ProcessLine(".L makeMap.C+");
  makeMap t(&inputChain_barl);
  t.setOutfile("map_barl2011B_.root");
  t.Loop();
  
}
