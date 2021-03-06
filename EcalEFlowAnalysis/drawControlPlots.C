{
  bool doAlsoTTPlots=false;
  bool doAlsoXtalPlots=false;
  bool quickTest=false;
  bool savePlots=true;
  int nRings=85;
  int nTowers=36*68;
  int nXtals=36*1700;
  if (quickTest)
    {
      nRings=5;
      nTowers=10;
      nXtals=5000;
    }

  TFile *_file0 = TFile::Open("histories_RUN2011A_test_etaRing.root");

  TFile * _file1=0;
  if (doAlsoTTPlots)
    _file1 = TFile::Open("histories_RUN2011ABnew3_itt.root");

  TFile *_file2=0;
  if (doAlsoXtalPlots)
    TFile *_file2 = TFile::Open("histories_RUN2011ABnew3_ixtal.root");

//   TDatime T0(2012,03,01,00,00,00);
//   int X0 = T0.Convert();
//   TDatime T1(2012,09,01,00,00,00);
//   int X1 = T1.Convert();

//2011A boundaries
//  int X0=1300000000+86400*15;
//  int X1=1315200000;

//2011B boundaries
//  int X0=1315200000;
//  int X1=1320000000;

  
  int X0=1300000000+86400*25;
  int X1=1320105600+86400*4;
         
  TH2F b("b","b",10,X0,X1,10,0.95,1.03);
  b.Draw();
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  b.GetXaxis()->SetTitle("Time");
  b.GetXaxis()->SetTimeDisplay(1);
  b.GetXaxis()->SetTimeFormat("%d\/%m");

  gStyle->SetOptFit(111111);
  TH2F a("a","a",10,0.945,1.015,10,0.945,1.015); //CAMBIARE
  //  TH2F a("a","a",10,0.95,1.03,10,0.95,1.03); 

  TGraphErrors* lc=(TGraphErrors*)_file0->Get("lc_ieta_1_side_0");
  int npoints=lc->GetN();
  double X[npoints];
  double errorlX[npoints];
  double errorhX[npoints];

  double lcBand[npoints][5];
  double etBand[npoints][5];
  double etNoCorrBand[npoints][5];

  TH1F* lcDist[npoints];
  TH1F* etDist[npoints];
  TH1F* etNoCorrDist[npoints];

  TH1F ringAlpha("ringAlpha","ringAlpha",240,0.8,2.0);
  TH1F ringAlphaVsEta("ringAlphaVsEta","ringAlphaVsEta",171,-85.5,85.5);
  TH1F ttAlpha("ttAlpha","ttAlpha",240,0.8,2.0);
  TH2F ttAlphaMap("ttAlphaMap","ttAlphaMap",72,0.5,72.5,35,-17.5,17.5);
  TH1F xtalAlpha("xtalAlpha","xtalAlpha",480,0.8,2.0);
  TH2F xtalAlphaMap("xtalAlphaMap","xtalAlphaMap",360,0.5,360.5,171,-85.5,85.5);

  TLine* line=new TLine(X0,1,X1,1);

  TF1 *fa1 = new TF1("fa1","pow(x,[0])",0,1.2);
  fa1->SetParameter(0, 1.52);

  for (int ii=0;ii<npoints;++ii)
    {
      TString label="interval_";
      label+=ii;
      X[ii]=*(lc->GetX()+ii);
      errorlX[ii]=4*3600.;
      errorhX[ii]=4*3600.;
      lcDist[ii]=new TH1F("lcDist_"+label,"lcDist_"+label,200,0.9,1.1);
      etDist[ii]=new TH1F("etDist_"+label,"etDist_"+label,200,0.9,1.1);
      etNoCorrDist[ii]=new TH1F("etNoCorrDist_"+label,"etNoCorrDist_"+label,200,0.9,1.1);
    }

  for (int i=1;i<nRings+1;++i)
    {
      for (int j=0;j<2;j++){
	TString etaLabel="ieta_";
	etaLabel+=i;
	TString sideLabel="_side_";       
	sideLabel+=j;
	TString eta="";
	eta+=i;

	std::cout <<  "monitor_"+etaLabel+sideLabel << std::endl;
	
	TGraphErrors* lc=(TGraphErrors*)_file0->Get("lc_"+etaLabel+sideLabel);
	TGraphErrors* et=(TGraphErrors*)_file0->Get("et_"+etaLabel+sideLabel);
	TGraphErrors* etSumOverRef=(TGraphErrors*)_file0->Get("etSumOverRef_"+etaLabel+sideLabel);
	TGraphErrors* etNoCorr=(TGraphErrors*)_file0->Get("etNoCorr_"+etaLabel+sideLabel);
	TGraphErrors* EtNoCorrvsTL= (TGraphErrors*)_file0->Get("EtNoCorrvsTL_"+etaLabel+sideLabel);

	for (int ii=0;ii<npoints;++ii)
	  {
	    lcDist[ii]->Fill(*(lc->GetY()+ii));
	    etDist[ii]->Fill(*(et->GetY()+ii));
	    etNoCorrDist[ii]->Fill(*(etNoCorr->GetY()+ii));
	  }
// 	EtNoCorrvsTL->Fit(fa1,"R+","",0.9,1.01);
// 	ringAlpha.Fill(fa1->GetParameter(0));
// 	int etaIndex=0;
// 	if (j==0)
// 	  etaIndex=85-i;
// 	else
// 	  etaIndex=86+i;
// 	ringAlphaVsEta.SetBinContent(etaIndex+1,fa1->GetParameter(0));
// 	ringAlphaVsEta.SetBinError(etaIndex+1,fa1->GetParError(0));

	if (savePlots)
	  {
	    b.Draw();
	    
	    lc->SetMarkerColor(1);
	    lc->SetMarkerStyle(20);
	    lc->SetMarkerSize(0.7);
	    lc->SetLineWidth(2);
	    et->SetMarkerColor(kRed);
	    et->SetMarkerStyle(20);
	    et->SetMarkerSize(0.5);
	    etSumOverRef->SetMarkerColor(kRed);
	    etSumOverRef->SetMarkerStyle(20);
	    etSumOverRef->SetMarkerSize(0.5);
	    etNoCorr->SetMarkerColor(kViolet);
	    etNoCorr->SetMarkerStyle(20);
	    etNoCorr->SetMarkerSize(0.5);
	    //	    et->Draw("PSAME");
	    etSumOverRef->Draw("PSAME");
	    // 	    etNoCorr->Draw("PSAME");
	    //	    lc->Draw("PLSAME");
	    
	    line->Draw("same");
	    
	    TLegend l(0.55,0.75,0.8,0.88);
	    
	    l.SetTextSize(0.033);
	    l.SetBorderSize(0);
	    l.SetFillColor(0);
	    
	    //	    l.AddEntry(lc,"1/laser corr i#eta("+eta+")","P");
	    l.AddEntry(etSumOverRef,"<et corrected> i#eta("+eta+")","P");
	    //	    l.AddEntry(etNoCorr,"<et uncorrected> i#eta("+eta+")","P");
	    l.Draw();
	    
	    
	    c1->SaveAs("monitor_"+etaLabel+sideLabel+".png");

	    //	l.Delete();
	    
	    a.Draw();
	    a.GetYaxis()->SetTitle("<et UnCorrected>");
	    a.GetXaxis()->SetTitle("1/<lc>");
	    EtNoCorrvsTL->SetMarkerColor(kViolet);
	    EtNoCorrvsTL->SetMarkerStyle(20);
	    EtNoCorrvsTL->SetMarkerSize(0.5);
	    
	    EtNoCorrvsTL->Draw("PESAME");

	    c1->SaveAs("EtNoCorrvsTL"+etaLabel+sideLabel+".png");
	  }
      }
    }

  gStyle->SetOptStat(1111);
  ringAlpha.GetXaxis()->SetTitle("alpha correction");
  ringAlpha.Draw();
  ringAlpha.SaveAs("ringAlpha.root");
//   if (savePlots)
    c1->SaveAs("ringAlpha.png");

  gStyle->SetOptStat(0);
  ringAlphaVsEta.GetXaxis()->SetTitle("ring index");
  ringAlphaVsEta.GetYaxis()->SetTitle("alpha correction");
  ringAlphaVsEta.SetMaximum(ringAlphaVsEta.GetBinContent(ringAlphaVsEta.GetMaximumBin())*1.2);
  ringAlphaVsEta.SetMinimum(0.8);
  ringAlphaVsEta.SetMarkerStyle(20);
  ringAlphaVsEta.SetMarkerSize(0.7);
  ringAlphaVsEta.Draw("E");
  ringAlphaVsEta.SaveAs("ringAlphaVsEta.root");
//   if (savePlots)
    c1->SaveAs("ringAlphaVsEta.png");

  double quantiles[5]={0.05,0.16,0.5,0.84,0.95};

  double lcBandGraph[5][npoints];
  double etBandGraph[5][npoints];
  double etNoCorrBandGraph[5][npoints];

  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
      etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
      etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
      for (int ij=0;ij<5;++ij)
	{
	  if (ij!=2)
	    {
	      lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
	      etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
	    }
	  else
	    {
	      lcBandGraph[ij][ii]=lcBand[ii][ij];
	      etBandGraph[ij][ii]=etBand[ii][ij];
	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
	    }
	  
	} 
    } 
      
  
  TGraphAsymmErrors * lc68Graph=new TGraphAsymmErrors(npoints,lc->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
  TGraphAsymmErrors * lc95Graph=new TGraphAsymmErrors(npoints,lc->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

  TGraphAsymmErrors * et68Graph=new TGraphAsymmErrors(npoints,et->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
  TGraphAsymmErrors * et95Graph=new TGraphAsymmErrors(npoints,et->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);

  TGraphAsymmErrors * etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
  TGraphAsymmErrors * etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);

  TH2F c("c","c",10,X0,X1,10,0.94,1.03);
  c.Draw();
  c.GetXaxis()->SetTitle("Time");
  c.GetXaxis()->SetTimeDisplay(1);
  c.GetXaxis()->SetTimeFormat("%d\/%m");


  c.Draw();
  lc95Graph->SetFillColor(kYellow);
  lc95Graph->SetFillStyle(1001);
  lc95Graph->Draw("2same");

  lc68Graph->SetFillColor(kGreen);
  lc68Graph->SetFillStyle(1001);
  lc68Graph->SetMarkerColor(kBlack);
  lc68Graph->SetMarkerStyle(20);
  lc68Graph->SetMarkerSize(0.4);
  lc68Graph->Draw("2same");
  lc68Graph->Draw("pxsame");
  if (savePlots)
    c1->SaveAs("lcGraph.png");


  c.Draw();
  et95Graph->SetFillColor(kYellow);
  et95Graph->SetFillStyle(1001);
  et95Graph->Draw("2same");

  et68Graph->SetFillColor(kGreen);
  et68Graph->SetFillStyle(1001);
  et68Graph->SetMarkerColor(kBlack);
  et68Graph->SetMarkerStyle(20);
  et68Graph->SetMarkerSize(0.4);
  et68Graph->Draw("2same");
  et68Graph->Draw("pxsame");

  if (savePlots)
    c1->SaveAs("etGraph.png");


  c.Draw();
  etNoCorr95Graph->SetFillColor(kYellow);
  etNoCorr95Graph->SetFillStyle(1001);
  etNoCorr95Graph->Draw("2same");

  etNoCorr68Graph->SetFillColor(kGreen);
  etNoCorr68Graph->SetFillStyle(1001);
  etNoCorr68Graph->SetMarkerColor(kBlack);
  etNoCorr68Graph->SetMarkerStyle(20);
  etNoCorr68Graph->SetMarkerSize(0.4);
  etNoCorr68Graph->Draw("2same");
  etNoCorr68Graph->Draw("pxsame");

  if (savePlots)
    c1->SaveAs("etNoCorrGraph.png");


  c.Draw();
  lc95Graph->SetFillColor(kYellow+1);
  lc95Graph->SetFillStyle(1001);
  lc95Graph->Draw("2same");

  lc68Graph->SetFillColor(kGreen+3);
  lc68Graph->SetFillStyle(1001);
  lc68Graph->SetMarkerColor(kBlack);
  lc68Graph->SetMarkerStyle(20);
  lc68Graph->SetMarkerSize(0.4);
  lc68Graph->Draw("2same");
  lc68Graph->Draw("pxsame");

  et95Graph->SetFillColor(kYellow);
  et95Graph->SetFillStyle(1001);
  et95Graph->Draw("2same");

  et68Graph->SetFillColor(kGreen);
  et68Graph->SetFillStyle(1001);
  et68Graph->SetMarkerColor(kRed);
  et68Graph->SetMarkerStyle(20);
  et68Graph->SetMarkerSize(0.4);
  et68Graph->Draw("2same");
  et68Graph->Draw("pxsame");

  etNoCorr68Graph->SetMarkerColor(kViolet);
  etNoCorr68Graph->SetMarkerStyle(20);
  etNoCorr68Graph->SetMarkerSize(0.4);
  etNoCorr68Graph->Draw("pxsame");

  line->Draw("same");
  if (savePlots)
    c1->SaveAs("fullHistory.png");

  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->Reset();
      etDist[ii]->Reset();
      etNoCorrDist[ii]->Reset();
    }

  if(doAlsoTTPlots){
  for (int i=1;i<=nTowers;++i)
    {
       TString ittLabel="itt_";
       ittLabel+=i;
       TString itt="";
       itt+=i;

       TGraphErrors* lc=(TGraphErrors*)_file1->Get("lc_"+ittLabel);
       TGraphErrors* et=(TGraphErrors*)_file1->Get("et_"+ittLabel);
       TGraphErrors* etNoCorr=(TGraphErrors*)_file1->Get("etNoCorr_"+ittLabel);
       TGraphErrors* EtNoCorrvsTL= (TGraphErrors*)_file1->Get("EtNoCorrvsTL_"+ittLabel);

       for (int ii=0;ii<npoints;++ii)
	 {
	   lcDist[ii]->Fill(*(lc->GetY()+ii));
	   etDist[ii]->Fill(*(et->GetY()+ii));
	   etNoCorrDist[ii]->Fill(*(etNoCorr->GetY()+ii));
	 }

       EtNoCorrvsTL->Fit(fa1,"R+","",0.9,1.01);
       ttAlpha.Fill(fa1->GetParameter(0));
       int etaIndex=0;
       int phiIndex=0;
       int smIndex=(i-1)/68+1;
       int ttIndex=(i-1)%68+1;
       if (smIndex>18)
	 {
	   etaIndex=17-((ttIndex-1)/4+1)+1;
	 }
       else
	 {
	   etaIndex=17+((ttIndex-1)/4+1)+1;
	 }
       phiIndex=(((smIndex-1)%18)*4)+((ttIndex-1)%4+1);
       std::cout <<  "monitor_"+ittLabel << std::endl;
       std::cout << "i " << " " << smIndex << " " << ttIndex << " " << etaIndex << " " << phiIndex << std::endl;
       ttAlphaMap.SetBinContent(phiIndex,etaIndex,fa1->GetParameter(0));
       ttAlphaMap.SetBinError(phiIndex,etaIndex,fa1->GetParError(0));

       if (savePlots)
	 {
	   b.Draw();
	   lc->SetMarkerColor(1);
	   lc->SetMarkerStyle(20);
	   lc->SetMarkerSize(0.7);
	   lc->SetLineWidth(2);
	   et->SetMarkerColor(kRed);
	   et->SetMarkerStyle(20);
	   et->SetMarkerSize(0.5);
	   etNoCorr->SetMarkerColor(kViolet);
	   etNoCorr->SetMarkerStyle(20);
	   etNoCorr->SetMarkerSize(0.5);
	   et->Draw("PSAME");
	   etNoCorr->Draw("PSAME");
	   lc->Draw("PLSAME");
	   
	   line->Draw("same");
	   TLegend l(0.45,0.75,0.91,0.88);
	   l.SetTextSize(0.033);
	   l.SetBorderSize(0);
	   l.SetFillColor(0);
	   l.AddEntry(lc,"1/laser corr itt("+itt+")","P");
	   l.AddEntry(et,"<et corrected> itt("+itt+")","P");
	   l.AddEntry(etNoCorr,"<et uncorrected> itt("+itt+")","P");
	   l.Draw();
	   c1->SaveAs("monitor_"+ittLabel+".png");
	   a.Draw();
	   a.GetYaxis()->SetTitle("<et UnCorrected>");
	   a.GetXaxis()->SetTitle("1/<lc>");
	   EtNoCorrvsTL->SetMarkerColor(kViolet);
	   EtNoCorrvsTL->SetMarkerStyle(20);
	   EtNoCorrvsTL->SetMarkerSize(0.5);
	   
	   EtNoCorrvsTL->Draw("PESAME");
	   c1->SaveAs("EtNoCorrvsTL"+ittLabel+".png");
	 }
    }

  gStyle->SetOptStat(1111);
  ttAlpha.GetXaxis()->SetTitle("alpha correction");
  ttAlpha.Draw();
  ttAlpha.SaveAs("ttAlpha.root");
//   if (savePlots)
    c1->SaveAs("ttAlpha.png");

  gStyle->SetOptStat(0);
  ttAlphaMap.GetXaxis()->SetTitle("tt phi index");
  ttAlphaMap.GetYaxis()->SetTitle("tt eta index");
  ttAlphaMap.GetZaxis()->SetRangeUser(0.8,1.9);
  ttAlphaMap.Draw("COLZ");
  ttAlphaMap.SaveAs("ttAlphaMap.root");
//   if (savePlots)
    c1->SaveAs("ttAlphaMap.png");

  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
      etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
      etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
      for (int ij=0;ij<5;++ij)
	{
	  if (ij!=2)
	    {
	      lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
	      etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
	    }
	  else
	    {
	      lcBandGraph[ij][ii]=lcBand[ii][ij];
	      etBandGraph[ij][ii]=etBand[ii][ij];
	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
	    }
	  
	} 
    } 
      
  
  lc68Graph=new TGraphAsymmErrors(npoints,lc->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
  lc95Graph=new TGraphAsymmErrors(npoints,lc->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

  et68Graph=new TGraphAsymmErrors(npoints,et->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
  et95Graph=new TGraphAsymmErrors(npoints,et->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);

  etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
  etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);

  c.Draw();
  c.GetXaxis()->SetTitle("Time");
  c.GetXaxis()->SetTimeDisplay(1);
  c.GetXaxis()->SetTimeFormat("%d\/%m");


  c.Draw();
  lc95Graph->SetFillColor(kYellow);
  lc95Graph->SetFillStyle(1001);
  lc95Graph->Draw("2same");

  lc68Graph->SetFillColor(kGreen);
  lc68Graph->SetFillStyle(1001);
  lc68Graph->SetMarkerColor(kBlack);
  lc68Graph->SetMarkerStyle(20);
  lc68Graph->SetMarkerSize(0.4);
  lc68Graph->Draw("2same");
  lc68Graph->Draw("pxsame");

//   if (savePlots)
    c1->SaveAs("lcGraph_itt.png");


  c.Draw();
  et95Graph->SetFillColor(kYellow);
  et95Graph->SetFillStyle(1001);
  et95Graph->Draw("2same");

  et68Graph->SetFillColor(kGreen);
  et68Graph->SetFillStyle(1001);
  et68Graph->SetMarkerColor(kBlack);
  et68Graph->SetMarkerStyle(20);
  et68Graph->SetMarkerSize(0.4);
  et68Graph->Draw("2same");
  et68Graph->Draw("pxsame");

//   if (savePlots)
    c1->SaveAs("etGraph_itt.png");


  c.Draw();
  etNoCorr95Graph->SetFillColor(kYellow);
  etNoCorr95Graph->SetFillStyle(1001);
  etNoCorr95Graph->Draw("2same");

  etNoCorr68Graph->SetFillColor(kGreen);
  etNoCorr68Graph->SetFillStyle(1001);
  etNoCorr68Graph->SetMarkerColor(kBlack);
  etNoCorr68Graph->SetMarkerStyle(20);
  etNoCorr68Graph->SetMarkerSize(0.4);
  etNoCorr68Graph->Draw("2same");
  etNoCorr68Graph->Draw("pxsame");

//   if (savePlots)
    c1->SaveAs("etNoCorrGraph_itt.png");


  c.Draw();
  lc95Graph->SetFillColor(kYellow+1);
  lc95Graph->SetFillStyle(1001);
  lc95Graph->Draw("2same");

  lc68Graph->SetFillColor(kGreen+3);
  lc68Graph->SetFillStyle(1001);
  lc68Graph->SetMarkerColor(kBlack);
  lc68Graph->SetMarkerStyle(20);
  lc68Graph->SetMarkerSize(0.4);
  lc68Graph->Draw("2same");
  lc68Graph->Draw("pxsame");

  et95Graph->SetFillColor(kYellow);
  et95Graph->SetFillStyle(1001);
  et95Graph->Draw("2same");

  et68Graph->SetFillColor(kGreen);
  et68Graph->SetFillStyle(1001);
  et68Graph->SetMarkerColor(kRed);
  et68Graph->SetMarkerStyle(20);
  et68Graph->SetMarkerSize(0.4);
  et68Graph->Draw("2same");
  et68Graph->Draw("pxsame");

  etNoCorr68Graph->SetMarkerColor(kViolet);
  etNoCorr68Graph->SetMarkerStyle(20);
  etNoCorr68Graph->SetMarkerSize(0.4);
  etNoCorr68Graph->Draw("pxsame");

  line->Draw("same");

//   if (savePlots)
    c1->SaveAs("fullHistory_itt.png");
  }

  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->Reset();
      etDist[ii]->Reset();
      etNoCorrDist[ii]->Reset();
    }

  if(doAlsoXtalPlots){
  for (int i=1;i<=nXtals;++i)
    {
       TString ixtalLabel="ixtal_";
       ixtalLabel+=i;
       TString ixtal="";
       ixtal+=i;

       TGraphErrors* lcxtal=(TGraphErrors*)_file2->Get("lc_"+ixtalLabel);
       TGraphErrors* etxtal=(TGraphErrors*)_file2->Get("et_"+ixtalLabel);
       TGraphErrors* etNoCorrxtal=(TGraphErrors*)_file2->Get("etNoCorr_"+ixtalLabel);
       TGraphErrors* EtNoCorrvsTLxtal= (TGraphErrors*)_file2->Get("EtNoCorrvsTL_"+ixtalLabel);

       for (int ii=0;ii<npoints;++ii)
	 {
	   lcDist[ii]->Fill(*(lcxtal->GetY()+ii));
	   etDist[ii]->Fill(*(etxtal->GetY()+ii));
	   etNoCorrDist[ii]->Fill(*(etNoCorrxtal->GetY()+ii));
	 }

       EtNoCorrvsTLxtal->Fit(fa1,"R+","",0.9,1.01);
       xtalAlpha.Fill(fa1->GetParameter(0));
       int etaIndex=(i-1)/360+1;
       int phiIndex=(i-1)%360+1;
       if (etaIndex>85)
	 etaIndex++;
       else
	 etaIndex=86-etaIndex;

       std::cout <<  "monitor_"+ixtalLabel << std::endl;
       std::cout << "i " << i << " " << etaIndex << " " << phiIndex << std::endl;
       xtalAlphaMap.SetBinContent(phiIndex,etaIndex,fa1->GetParameter(0));
       xtalAlphaMap.SetBinError(phiIndex,etaIndex,fa1->GetParError(0));

       if (savePlots)
	 {
	   b.Draw();
	   lcxtal->SetMarkerColor(1);
	   lcxtal->SetMarkerStyle(20);
	   lcxtal->SetMarkerSize(0.7);
	   lcxtal->SetLineWidth(2);
	   etxtal->SetMarkerColor(kRed);
	   etxtal->SetMarkerStyle(20);
	   etxtal->SetMarkerSize(0.5);
	   etNoCorrxtal->SetMarkerColor(kViolet);
	   etNoCorrxtal->SetMarkerStyle(20);
	   etNoCorrxtal->SetMarkerSize(0.5);
	   etxtal->Draw("PSAME");
	   etNoCorrxtal->Draw("PSAME");
	   lcxtal->Draw("PLSAME");
	   line->Draw("same");
	   TLegend l(0.45,0.75,0.91,0.88);
	   l.SetTextSize(0.033);
	   l.SetBorderSize(0);
	   l.SetFillColor(0);
	   l.AddEntry(lcxtal,"1/laser corr ixtal("+ixtal+")","P");
	   l.AddEntry(et,"<et corrected> ixtal("+ixtal+")","P");
	   l.AddEntry(etNoCorr,"<et uncorrected> ixtal("+ixtal+")","P");
	   l.Draw();

	   c1->SaveAs("monitor_"+ixtalLabel+".png");
	   a.Draw();
	   a.GetYaxis()->SetTitle("<et UnCorrected>");
	   a.GetXaxis()->SetTitle("1/<lc>");
	   EtNoCorrvsTLxtal->SetMarkerColor(kViolet);
	   EtNoCorrvsTLxtal->SetMarkerStyle(20);
	   EtNoCorrvsTLxtal->SetMarkerSize(0.5);
	   
	   EtNoCorrvsTLxtal->Draw("PESAME");
	   
	   c1->SaveAs("EtNoCorrvsTL"+ixtalLabel+".png");
	 }

       delete lcxtal;
       delete etxtal;
       delete etNoCorrxtal;
       delete EtNoCorrvsTLxtal;
    }

  gStyle->SetOptStat(1111);
  xtalAlpha.GetXaxis()->SetTitle("alpha correction");
  xtalAlpha.Draw();
  xtalAlpha.SaveAs("xtalAlpha.root");
//   if (savePlots)
    c1->SaveAs("xtalAlpha.png");

  gStyle->SetOptStat(0);
  xtalAlphaMap.GetXaxis()->SetTitle("xtal phi index");
  xtalAlphaMap.GetYaxis()->SetTitle("xtal eta index");
  xtalAlphaMap.GetZaxis()->SetRangeUser(0.8,1.9);
  xtalAlphaMap.Draw("COLZ");
  xtalAlphaMap.SaveAs("xtalAlphaMap.root");
//   if (savePlots)
    c1->SaveAs("xtalAlphaMap.png");

  for (int ii=0;ii<npoints;++ii)
    {
      lcDist[ii]->GetQuantiles(5,&lcBand[ii][0],&quantiles[0]);
      etDist[ii]->GetQuantiles(5,&etBand[ii][0],&quantiles[0]);
      etNoCorrDist[ii]->GetQuantiles(5,&etNoCorrBand[ii][0],&quantiles[0]);
      for (int ij=0;ij<5;++ij)
	{
	  if (ij!=2)
	    {
	      lcBandGraph[ij][ii]=fabs(lcBand[ii][ij]-lcBand[ii][2]);
	      etBandGraph[ij][ii]=fabs(etBand[ii][ij]-etBand[ii][2]);
	      etNoCorrBandGraph[ij][ii]=fabs(etNoCorrBand[ii][ij]-etNoCorrBand[ii][2]);
	    }
	  else
	    {
	      lcBandGraph[ij][ii]=lcBand[ii][ij];
	      etBandGraph[ij][ii]=etBand[ii][ij];
	      etNoCorrBandGraph[ij][ii]=etNoCorrBand[ii][ij];
	    }
	  
	} 
    } 
      
  
  lc68Graph=new TGraphAsymmErrors(npoints,lc->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[1],lcBandGraph[3]);
  lc95Graph=new TGraphAsymmErrors(npoints,lc->GetX(),lcBandGraph[2],errorlX,errorhX,lcBandGraph[0],lcBandGraph[4]);

  et68Graph=new TGraphAsymmErrors(npoints,et->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[1],etBandGraph[3]);
  et95Graph=new TGraphAsymmErrors(npoints,et->GetX(),etBandGraph[2],errorlX,errorhX,etBandGraph[0],etBandGraph[4]);

  etNoCorr68Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[1],etNoCorrBandGraph[3]);
  etNoCorr95Graph=new TGraphAsymmErrors(npoints,etNoCorr->GetX(),etNoCorrBandGraph[2],errorlX,errorhX,etNoCorrBandGraph[0],etNoCorrBandGraph[4]);

  c.Draw();
  c.GetXaxis()->SetTitle("Time");
  c.GetXaxis()->SetTimeDisplay(1);
  c.GetXaxis()->SetTimeFormat("%d\/%m");


  c.Draw();
  lc95Graph->SetFillColor(kYellow);
  lc95Graph->SetFillStyle(1001);
  lc95Graph->Draw("2same");

  lc68Graph->SetFillColor(kGreen);
  lc68Graph->SetFillStyle(1001);
  lc68Graph->SetMarkerColor(kBlack);
  lc68Graph->SetMarkerStyle(20);
  lc68Graph->SetMarkerSize(0.4);
  lc68Graph->Draw("2same");
  lc68Graph->Draw("pxsame");

//   if (savePlots)
    c1->SaveAs("lcGraph_ixtal.png");


  c.Draw();
  et95Graph->SetFillColor(kYellow);
  et95Graph->SetFillStyle(1001);
  et95Graph->Draw("2same");

  et68Graph->SetFillColor(kGreen);
  et68Graph->SetFillStyle(1001);
  et68Graph->SetMarkerColor(kBlack);
  et68Graph->SetMarkerStyle(20);
  et68Graph->SetMarkerSize(0.4);
  et68Graph->Draw("2same");
  et68Graph->Draw("pxsame");

//   if (savePlots)
    c1->SaveAs("etGraph_ixtal.png");


  c.Draw();
  etNoCorr95Graph->SetFillColor(kYellow);
  etNoCorr95Graph->SetFillStyle(1001);
  etNoCorr95Graph->Draw("2same");

  etNoCorr68Graph->SetFillColor(kGreen);
  etNoCorr68Graph->SetFillStyle(1001);
  etNoCorr68Graph->SetMarkerColor(kBlack);
  etNoCorr68Graph->SetMarkerStyle(20);
  etNoCorr68Graph->SetMarkerSize(0.4);
  etNoCorr68Graph->Draw("2same");
  etNoCorr68Graph->Draw("pxsame");

//   if (savePlots)
    c1->SaveAs("etNoCorrGraph_ixtal.png");


  c.Draw();
  lc95Graph->SetFillColor(kYellow+1);
  lc95Graph->SetFillStyle(1001);
  lc95Graph->Draw("2same");

  lc68Graph->SetFillColor(kGreen+3);
  lc68Graph->SetFillStyle(1001);
  lc68Graph->SetMarkerColor(kBlack);
  lc68Graph->SetMarkerStyle(20);
  lc68Graph->SetMarkerSize(0.4);
  lc68Graph->Draw("2same");
  lc68Graph->Draw("pxsame");

  et95Graph->SetFillColor(kYellow);
  et95Graph->SetFillStyle(1001);
  et95Graph->Draw("2same");

  et68Graph->SetFillColor(kGreen);
  et68Graph->SetFillStyle(1001);
  et68Graph->SetMarkerColor(kRed);
  et68Graph->SetMarkerStyle(20);
  et68Graph->SetMarkerSize(0.4);
  et68Graph->Draw("2same");
  et68Graph->Draw("pxsame");

  etNoCorr68Graph->SetMarkerColor(kViolet);
  etNoCorr68Graph->SetMarkerStyle(20);
  etNoCorr68Graph->SetMarkerSize(0.4);
  etNoCorr68Graph->Draw("pxsame");
  line->Draw("same");

//   if (savePlots)
    c1->SaveAs("fullHistory_ixtal.png");
  }
}
