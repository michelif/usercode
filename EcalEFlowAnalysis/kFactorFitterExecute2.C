{
  gROOT->Reset();
  using namespace std;
  TChain inputChain_barl;
  ostringstream tree_name;
  tree_name<<"kFactorTree_"<<0;
  cout<<tree_name.str()<<endl;
  inputChain_barl=new TChain(tree_name.str().c_str());



  std::ifstream fileList("filelist_kFactors_2012B.txt", ios::in);  

  if (!fileList.is_open() ) {
    cout<<"file not found"<<endl;
    exit(0);
  }
  
  
  
  int sum=0;
  while(!fileList.eof()&& sum <1000){
    string nameFile;
    getline(fileList,nameFile);
    inputChain_barl.Add(nameFile.c_str());
    sum++;
    cout<<sum<<endl;
  }
  
  gROOT->ProcessLine(".L kFactorFitter2.C++");
  kFactorFitter2 t(&inputChain_barl);
  ostringstream file_name;
  file_name<<"kFactorFitterOut_2012B.root"; 
  t.setOutfile(file_name.str().c_str());
  //t.GetCutValues();
  t.Loop();

}
