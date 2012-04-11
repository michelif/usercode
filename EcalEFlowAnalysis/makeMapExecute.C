{
  gROOT->Reset();
  TChain inputChain_barl("variablesTree");
  

  std::ifstream fileList("filelist2012_190456-190539.txt", ios::in);
  
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
  t.setOutfile("map_barl2012_190456-190539.root");
  t.Loop();
  
}
