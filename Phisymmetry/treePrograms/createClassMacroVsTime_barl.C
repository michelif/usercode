{

TChain inputChain_barl("variablesTree_barl");


std::ifstream fileList("filelist.txt", ios::in);

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






 gROOT->ProcessLine(".L plotsVsTimeMacro_barl.C+");
 gROOT->ProcessLine("plotsVsTimeMacro_barl t(&inputChain_barl)");
 gROOT->ProcessLine("t.Loop()");

}
