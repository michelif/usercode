{
  bool doAlsoTTPlots=false;
  TFile *_file0 = TFile::Open("histories_RUN2011ABnew_etaRing.root");
  if (doAlsoTTPlots)TFile *_file1 = TFile::Open("histories_RUN2011ABnew_itt.root");
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

  TGraph* lc=(TGraph*)_file0->Get("lc_ieta_1_side_0");
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

  for (int i=1;i<86;++i)
    {
      for (int j=0;j<2;j++){
	TString etaLabel="ieta_";
	etaLabel+=i;
	TString sideLabel="_side_";       
	sideLabel+=j;
	TString eta="";
	eta+=i;

	TGraph* lc=(TGraph*)_file0->Get("lc_"+etaLabel+sideLabel);
	TGraph* et=(TGraph*)_file0->Get("et_"+etaLabel+sideLabel);
	TGraph* etNoCorr=(TGraph*)_file0->Get("etNoCorr_"+etaLabel+sideLabel);

	for (int ii=0;ii<npoints;++ii)
	  {
	    lcDist[ii]->Fill(*(lc->GetY()+ii));
	    etDist[ii]->Fill(*(et->GetY()+ii));
	    etNoCorrDist[ii]->Fill(*(etNoCorr->GetY()+ii));
	  }

	b.Draw();
	lc->SetMarkerColor(1);
	lc->SetMarkerStyle(20);
	lc->SetMarkerSize(0.5);
	et->SetMarkerColor(kRed);
	et->SetMarkerStyle(20);
	et->SetMarkerSize(0.5);
	etNoCorr->SetMarkerColor(kViolet);
	etNoCorr->SetMarkerStyle(20);
	etNoCorr->SetMarkerSize(0.5);
	lc->Draw("PSAME");
	et->Draw("PSAME");
	etNoCorr->Draw("PSAME");
	TLine* line=new TLine(X0,1,X1,1);
	line->Draw("same");
	TLegend l(0.55,0.75,0.8,0.88);
	l.SetTextSize(0.033);
	l.SetBorderSize(0);
	l.SetFillColor(0);
	l.AddEntry(lc,"1/laser corr i#eta("+eta+")","P");
	l.AddEntry(et,"<et corrected> i#eta("+eta+")","P");
	l.AddEntry(etNoCorr,"<et uncorrected> i#eta("+eta+")","P");
	l.Draw();
	std::cout <<  "monitor_"+etaLabel+sideLabel+".png" << std::endl;
	c1->SaveAs("monitor_"+etaLabel+sideLabel+".png");
      }
    }

  double quantiles[5]={0.01,0.16,0.5,0.84,0.99};

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

  c1->SaveAs("fullHistory.png");

  if(doAlsoTTPlots){
  for (int i=1;i<36*68;++i)
    {
       TString ittLabel="itt_";
       ittLabel+=i;
       TString itt="";
       itt+=i;

       TGraph* lc=(TGraph*)_file1->Get("lc_"+ittLabel);
       TGraph* et=(TGraph*)_file1->Get("et_"+ittLabel);
       TGraph* etNoCorr=(TGraph*)_file1->Get("etNoCorr_"+ittLabel);

       for (int ii=0;ii<npoints;++ii)
	 {
	   lcDist[ii]->Fill(*(lc->GetY()+ii));
	   etDist[ii]->Fill(*(et->GetY()+ii));
	   etNoCorrDist[ii]->Fill(*(etNoCorr->GetY()+ii));
	 }

       b.Draw();
       lc->SetMarkerColor(1);
       lc->SetMarkerStyle(20);
       lc->SetMarkerSize(0.5);
       et->SetMarkerColor(kRed);
       et->SetMarkerStyle(20);
       et->SetMarkerSize(0.5);
       etNoCorr->SetMarkerColor(kViolet);
       etNoCorr->SetMarkerStyle(20);
       etNoCorr->SetMarkerSize(0.5);
       lc->Draw("PSAME");
       et->Draw("PSAME");
       etNoCorr->Draw("PSAME");
       TLegend l(0.45,0.75,0.91,0.88);
       l.SetTextSize(0.033);
       l.SetBorderSize(0);
       l.SetFillColor(0);
       l.AddEntry(lc,"1/laser corr itt("+itt+")","P");
       l.AddEntry(et,"<et corrected> itt("+itt+")","P");
       l.AddEntry(etNoCorr,"<et uncorrected> itt("+itt+")","P");
       l.Draw();
       std::cout <<  "monitor_"+ittLabel+".png" << std::endl;
       c1->SaveAs("monitor_"+ittLabel+".png");
    }

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

  c1->SaveAs("fullHistory_itt.png");
  }
}
