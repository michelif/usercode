{
TChain inputChain("variablesTree_barl");

std::ifstream fileList("filelist.txt", ios::in);

  if (!fileList.is_open()) {
    cout<<"file not found"<<endl;
    exit(0);
  }
  


int sum=0;
while(!fileList.eof()){
  string nameFile;
  getline(fileList,nameFile);
  inputChain.Add(nameFile.c_str());
  sum++;
  cout<<sum<<endl;
 }


 gROOT->ProcessLine(".L plotsMacro.C+");
 gROOT->ProcessLine("plotsMacro t(&inputChain)");

 gROOT->ProcessLine("t.Loop()");

}
