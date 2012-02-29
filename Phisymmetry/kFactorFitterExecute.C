{
  gROOT->Reset();
  gROOT->ProcessLine(".L kFactorFitter.C++");
  kFactorFitter t;
  t.FitKFactors();

}
