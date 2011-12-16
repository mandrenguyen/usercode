#include <iostream>
#include "TCanvas.h"
#include "TError.h"
#include "TPad.h"
#include "TString.h"
#include "TRandom.h"
#include "TH1F.h"

#include "TFile.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TGraphAsymmErrors.h"
#include "TStyle.h"
#include "TLine.h"
#include "DrawTick.C"
#include "weightMix.C"

using namespace std;

TGraphAsymmErrors *divideGraph(TGraphAsymmErrors *a,TGraphAsymmErrors *b)
{
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
   for (int i=0;i<a->GetN();i++)
   {
      double x,y;
      double x2,y2;
      a->GetPoint(i,x,y);
      b->GetPoint(i,x2,y2);
      double errYL = a->GetErrorYlow(i);
      double errYH = a->GetErrorYhigh(i);
      double errYL2 = b->GetErrorYlow(i);
      double errYH2 = b->GetErrorYhigh(i);
      gEfficiency->SetPointError(i,0,0,sqrt(errYL*errYL+errYL2*errYL2),sqrt(errYH*errYH+errYH2*errYH2));
      gEfficiency->SetPoint(i,x,y/y2);
   }
   return gEfficiency;
}

TGraphAsymmErrors *calcEff(TH1* h1, TH1* hCut,double *npart)
{
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
   gEfficiency->BayesDivide(hCut,h1);
   cout <<gEfficiency->GetN()<<endl;
   for (int i=0;i<gEfficiency->GetN();i++)
   {
      double x,y;
      gEfficiency->GetPoint(i,x,y);
      double errYL = gEfficiency->GetErrorYlow(i);
      double errYH = gEfficiency->GetErrorYhigh(i);
      gEfficiency->SetPointError(i,0,0,errYL,errYH);
      gEfficiency->SetPoint(i,npart[h1->FindBin(x)-1],y);
      cout <<x<<" "<<h1->FindBin(x)<<" "<<npart[h1->FindBin(x)-1]<<endl;
   }
   return gEfficiency;
}

TGraphAsymmErrors *calcEffpythia(TH1* h1, TH1* hCut,double *npart)
{
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
   gEfficiency->BayesDivide(hCut,h1);
   cout <<gEfficiency->GetN()<<endl;
   for (int i=0;i<gEfficiency->GetN();i++)
   {
      double x,y;
      gEfficiency->GetPoint(i,x,y);
      double errYL = gEfficiency->GetErrorYlow(i);
      double errYH = gEfficiency->GetErrorYhigh(i);
      gEfficiency->SetPointError(i,0,0,errYL,errYH);
      gEfficiency->SetPoint(i,2,y);
      cout<<" Setting PYTHIA point to npart=2 by hand, Yen-Jie please check -Matt "<<endl;
   }
   return gEfficiency;
}




void plotRB(  double ajCut=0.24,
	      double ajCut2 = 0.15,
		 TString infname = "data.root",
		 TString pythia = "data_pp.root",
		 TString mix = "mix.root",
		 TString titleForComparison = "PYTHIA+HYDJET 1.6",
		 TString titleForFile = "Result",
		 bool useWeight = false,
		 bool drawXLabel = false,
		 bool drawLeg = true)
{		


  weightMix();
  int threshold1 = 100;
  int threshold2 = 120;
  gStyle->SetErrorX(0); 
  TString cut1=Form("pt1>%d",threshold1);
  TString cut2=Form("pt1>%d",threshold2);

  cut1=cut2;
  cout <<cut1.Data()<<endl;
  cout <<cut2.Data()<<endl;

  TString trigcut = "";
  TString cstring = "";

  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");

  // open the pythia (MC) file
  TFile *infPythia = new TFile(pythia.Data());
  TTree *ntPythia = (TTree*) infPythia->FindObjectAny("nt");

  // open the datamix file
  TFile *infMix = new TFile(mix.Data());
  TTree *ntMix =(TTree*)infMix->FindObjectAny("nt");

  // open output
  TFile *outfile = new TFile("output.root","recreate");
  TNtuple *ntOut = new TNtuple("ntOut","","npart");

  const int nBin = 5;
  double m[] = {-0.5,3.5,7.5,11.5,20.5,40.5};
  double npart[] = {358.623,264.245,189.482,113.091,38.6095,2};
  double npart2[] = {358.623,264.245,189.482,113.091,38.6095,2};
  
  double npartValue[40];
  npartValue[0] = 393.633;
  npartValue[1] = 368.819;
  npartValue[2] = 343.073;
  npartValue[3] = 317.625;
  npartValue[4] = 292.932;
  npartValue[5] = 271.917;
  npartValue[6] = 249.851;
  npartValue[7] = 230.72;
  npartValue[8] = 212.465;
  npartValue[9] = 194.752;
  npartValue[10] = 178.571;
  npartValue[11] = 163.23;
  npartValue[12] = 149.187;
  npartValue[13] = 136.011;
  npartValue[14] = 123.414;
  npartValue[15] = 111.7;
  npartValue[16] = 100.831;
  npartValue[17] = 90.7831;
  npartValue[18] = 80.9823;
  npartValue[19] = 72.6236;
  npartValue[20] = 64.1508;
  npartValue[21] = 56.6284;
  npartValue[22] = 49.9984;
  npartValue[23] = 43.3034;
  npartValue[24] = 37.8437;
  npartValue[25] = 32.6659;
  npartValue[26] = 27.83;
  npartValue[27] = 23.7892;
  npartValue[28] = 20.1745;
  npartValue[29] = 16.8453;
  npartValue[30] = 14.0322;
  npartValue[31] = 11.602;
  npartValue[32] = 9.52528;
  npartValue[33] = 7.6984;
  npartValue[34] = 6.446;
  npartValue[35] = 4.96683;
  npartValue[36] = 4.23649;
  npartValue[37] = 3.50147;
  npartValue[38] = 3.16107;
  npartValue[39] = 2.7877;

  TH1D *hTmp = new TH1D("hTmp","",100,-10,400);
  TH1D *h = new TH1D("h","",nBin,m);
  TH1D *hCut = new TH1D("hCut","",nBin,m);

  TH1D *hMix = new TH1D("hMix","",nBin,m);
  TH1D *hCutMix = new TH1D("hCutMix","",nBin,m);
  TH1D *hWeightMix = new TH1D("hWeightMix","",nBin,m);

  TH1D *h2 = new TH1D("h2","",nBin,m);
  TH1D *h2Cut = new TH1D("h2Cut","",nBin,m);

  TH1D *hStat = new TH1D("hStat","",nBin,m);
  TH1D *hNpartSum = new TH1D("hNpartSum","",nBin,m);
  TH1D *hStat2 = new TH1D("hStat2","",nBin,m);
  TH1D *hNpartSum2 = new TH1D("hNpartSum2","",nBin,m);
  
  //  nt->Draw("bin>>h",Form("(pt1-pt2)/(pt1+pt2)<%f&&dphi>3.14159*2/3&&%s",ajCut,cut1.Data()));
  //  nt->Draw("bin>>hCut",Form("%s",cut1.Data()));
  TGraphAsymmErrors *g = calcEff(hCut,h,npart);
  g->SetMarkerSize(1.25);

  cout <<cut2.Data()<<endl;
  nt->Draw("bin>>h2",Form("(pt1-pt2)/(pt1+pt2)<%f&&dphi>3.14159*2/3&&%s",ajCut2,cut2.Data()));
  nt->Draw("bin>>h2Cut",Form("%s",cut2.Data()));
  TGraphAsymmErrors *g2 = calcEff(h2Cut,h2,npart2);
  g2->SetMarkerSize(1.25);

  ntPythia->Draw("bin>>h",Form("(pt1-pt2)/(pt1+pt2)<%f&&dphi>3.14159*2/3&&%s",ajCut2,cut2.Data()));
  ntPythia->Draw("bin>>hCut",Form("%s",cut2.Data()));
  cout<<" pythia "<<endl;
  TGraphAsymmErrors *gPythia = calcEffpythia(hCut,h,npart);
  gPythia->SetMarkerSize(1.7);


  //  ntMix->SetAlias("weight",weightString.Data());
  new TCanvas("ccc3","",300,300);  

  ntMix->Draw("bin>>hMix",Form("((pt1-pt2)/(pt1+pt2)<%f&&dphi>3.14159*2/3&&%s)",ajCut2,cut2.Data()));

  //  ntMix->Draw("bin>>hMix",Form("weight*((pt1-pt2)/(pt1+pt2)<%f&&dphi>3.14159*2/3&&%s)",ajCut2,cut2.Data()));
  //  ntMix->Draw("bin>>h",Form("((pt1-pt2)/(pt1+pt2)<%f&&dphi>3.14159*2/3&&%s) * %s",ajCut2,cut2.Data(),weightString.Data()));

  //  ntMix->Draw("bin>>hMix",Form("( ( ( (pt1-pt2) / (pt1+pt2) ) < %f ) *(dphi>(3.14159*2/3))*%s)*%s",ajCut2,cut2.Data(),weightString.Data()));
  new TCanvas("ccc4","",300,300);

  //  ntMix->Draw("bin>>hCutMix",Form("weight*(%s)",cut2.Data()));
  //  ntMix->Draw("bin>>hCutMix",Form("%s*(%s)",weightString.Data(),cut2.Data()));

  ntMix->Draw("bin>>hWeightMix",Form("%s",weightString.Data()));
  ntMix->Draw("bin>>hCutMix",Form("%s",cut2.Data()));

  //  hCutMix->Multiply(hWeightMix);

  new TCanvas("ccc5","",300,300);

  TGraphAsymmErrors *gMix = calcEff(hCutMix,hMix,npart);
  gMix->SetMarkerSize(1.25);


  TCanvas *c = new TCanvas("c","",500,500);
  //  hTmp->SetMaximum(g->GetY()[0]*2.2);
  //hTmp->SetMaximum(1.4*gPythia->GetY()[0]);
  hTmp->SetMaximum(0.4);
  hTmp->SetMinimum(0.);

  hTmp->SetXTitle("N_{part}");
  hTmp->SetYTitle(Form("R_{B}(A_{J} < %.2f)",ajCut));
  hTmp->GetXaxis()->CenterTitle();
  hTmp->GetYaxis()->CenterTitle();
  hTmp->GetXaxis()->SetTitleSize(26);
  hTmp->GetYaxis()->SetTitleFont(43);
  hTmp->GetYaxis()->SetTitleSize(26);

  hTmp->GetYaxis()->SetTitleOffset(1.3);
  hTmp->Draw();

  double errorbar = 0.02;
/*
  for(int i = 0; i < g->GetN(); ++i){
    double *x = g->GetX();
    double *y = g->GetY();
//    DrawTick(y[i],0.18*y[i],0.18*y[i],x[i],0.012,8.1,16);
  }
  g->Draw("p same");
  g2->SetMarkerStyle(4);
  */
  for(int i = 0; i < g2->GetN(); ++i){
    double *x = g2->GetX();
    double *y = g2->GetY();
    double errBck = 1-(0.4848-(6.581e-05)*x[i])/0.5;
    double err = sqrt(0.063*0.063+0.048*0.048+errBck*errBck);
//    err= 0.14;
    DrawTick(y[i],err*y[i],err*y[i],x[i],0.012,8.1,1);
  }
  gPythia->SetMarkerColor(4);
  gPythia->SetLineColor(4);
  gPythia->SetMarkerStyle(29);
  gMix->SetMarkerColor(2);
  gMix->SetLineColor(2);
  //  gMix->Draw("p same");
  gMix->SetMarkerStyle(21);
  gPythia->Draw("p same");
  g2->Draw("p same");

  TLine* pline = new TLine(0,gPythia->GetY()[0],400,gPythia->GetY()[0]);
  pline->SetLineColor(4);
  pline->SetLineStyle(4);
  pline->Draw();

  if(drawLeg){
    TLegend *t3=new TLegend(0.5,0.77,0.9,0.93); 
    t3->AddEntry(g,"PbPb  #sqrt{s}_{_{NN}}=2.76 TeV","p");
    t3->AddEntry(gPythia,"pp #sqrt{s}=2.76 TeV","p");  
    //    t3->AddEntry(gMix,titleForComparison.Data(),"p");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
  }


  TLatex *cms = new TLatex(0.20,0.88,"CMS");
  cms->SetNDC();
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();                                                                                                                                        
  TLatex tsel;
  tsel.SetNDC();
  tsel.SetTextFont(63);
  tsel.SetTextSize(15);
  tsel.DrawLatex(0.25,0.375,"p_{T,1} > 120 GeV/c");
//  tsel.DrawLatex(0.25,0.275,"p_{T,2} > 50 GeV/c");
  tsel.DrawLatex(0.25,0.275,"#Delta#phi_{12} > #frac{2}{3}#pi");

  TLatex *lumi = new TLatex(0.20,0.81,"#intL dt = 6.7 #mub^{-1}");
  lumi->SetNDC();
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw(); 

  c->Print(Form("fig/RB_%d_%s_vs_Npart.eps",(int)(100*ajCut),titleForFile.Data()));
  c->Print(Form("fig/RB_%d_%s_vs_Npart.C",(int)(100*ajCut),titleForFile.Data()));
  c->Print(Form("fig/RB_%d_%s_vs_Npart.gif",(int)(100*ajCut),titleForFile.Data()));

  /*
  TCanvas *c1 = new TCanvas("c1","",500,500);

  gMix->Draw("ap");
  gMix->Fit("pol1");
*/
  TCanvas *c2 = new TCanvas("c2","",500,500);

  TGraphAsymmErrors *gRatio = divideGraph(g2,gMix);
  gRatio->Draw("ap");
  gRatio->Fit("pol1");
  gRatio->Fit("pol0");

  c2->Print(Form("fig/RB_Ratio_%f_%s_vs_Npart.eps",ajCut,titleForFile.Data()));
  c2->Print(Form("fig/RB_Ratio_%f_%s_vs_Npart.C",ajCut,titleForFile.Data()));
  c2->Print(Form("fig/RB_Ratio_%f_%s_vs_Npart.gif",ajCut,titleForFile.Data()));
  
}


// 6.3 % for smearing
// 4.8% for energy correction



