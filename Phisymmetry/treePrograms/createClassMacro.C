{

TChain inputChain_barl("variablesTree_barl");
TChain inputChain_endc("variablesTree_endc");

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
  inputChain_endc.Add(nameFile.c_str());
  sum++;
  cout<<sum<<endl;
 }




 bool doBarlPlots=true;

 gROOT->ProcessLine(".L plotsMacro.C+");
 if(doBarlPlots){
 gROOT->ProcessLine("plotsMacro t(&inputChain_barl)");
 }else{
 gROOT->ProcessLine("plotsMacro t(&inputChain_endc)");
 }
 gROOT->ProcessLine("t.Loop(doBarlPlots)");

}
