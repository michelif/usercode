{
  gROOT->Reset();
  using namespace std;
  TChain inputChain_barl;
  ostringstream tree_name;
  tree_name<<"kFactorTree_endc";
  cout<<tree_name.str()<<endl;
  inputChain_barl=new TChain(tree_name.str().c_str());


  std::ifstream fileList("filelist_kFactors_2011A_1_endc.txt", ios::in);
  
  if (!fileList.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }
  
  
  
  int sum=0;
  while(!fileList.eof() && sum<1){
    string nameFile;
    getline(fileList,nameFile);
    inputChain_barl.Add(nameFile.c_str());
    sum++;
    cout<<sum<<endl;
  }
  
  gROOT->ProcessLine(".L kFactorFitter_endc.C++");
  kFactorFitter_endc t(&inputChain_barl);
  ostringstream file_name;
  file_name<<"kFactorFitterOut_2011AB_endc.root";
  t.setOutfile(file_name.str().c_str());
  //t.GetCutValues();
  t.Loop();

}
