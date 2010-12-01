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

using namespace std;

TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2,double *npart)
{
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors();
   gEfficiency->BayesDivide(h2,h1);
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



void plotRJ(  double ajCut=0.24,
	      int cbin = 0,
		 TString infname = "/d101/yetkin/data/data.root",
		 TString pythia = "/d101/yetkin/data/pythia.root",
		 TString mix = "/d101/yetkin/data/mix.root",
		 bool useWeight = true,
		 bool drawXLabel = false,
		 bool drawLeg = true)
{		
  gStyle->SetErrorX(0); 
  TString cut="et1>120 && et2>50 && dphi>2.5";
  TString cutpp="et1>120 && et2>50 && dphi>2.5";
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
  TNtuple *ntOut = new TNtuple("ntOut","","a:b");

  const int nBin = 4;
  double m[nBin+1] = {-1.5,-0.5,3.5,11.5, 40.5};
  double npart0[nBin] = {2,355.788,163.34,46.6685}; // Flat Averages
  double npart[nBin] = {2,358.623,232.909,97.9521};
  
  
 /*
  double npart[40];
  npart[0] = 393.633;
  npart[1] = 368.819;
  npart[2] = 343.073;
  npart[3] = 317.625;
  npart[4] = 292.932;
  npart[5] = 271.917;
  npart[6] = 249.851;
  npart[7] = 230.72;
  npart[8] = 212.465;
  npart[9] = 194.752;
  npart[10] = 178.571;
  npart[11] = 163.23;
  npart[12] = 149.187;
  npart[13] = 136.011;
  npart[14] = 123.414;
  npart[15] = 111.7;
  npart[16] = 100.831;
  npart[17] = 90.7831;
  npart[18] = 80.9823;
  npart[19] = 72.6236;
  npart[20] = 64.1508;
  npart[21] = 56.6284;
  npart[22] = 49.9984;
  npart[23] = 43.3034;
  npart[24] = 37.8437;
  npart[25] = 32.6659;
  npart[26] = 27.83;
  npart[27] = 23.7892;
  npart[28] = 20.1745;
  npart[29] = 16.8453;
  npart[30] = 14.0322;
  npart[31] = 11.602;
  npart[32] = 9.52528;
  npart[33] = 7.6984;
  npart[34] = 6.446;
  npart[35] = 4.96683;
  npart[36] = 4.23649;
  npart[37] = 3.50147;
  npart[38] = 3.16107;
  npart[39] = 2.7877;
  */

  TH1D *hTmp = new TH1D("hTmp","",100,0,400);
  TH1D *h = new TH1D("h","",nBin,m);
  TH1D *h2 = new TH1D("h2","",nBin,m);
  
  nt->Draw("bin>>h",Form("(et1-et2)/(et1+et2)>%f&&%s",ajCut,cut.Data()));
  nt->Draw("bin>>h2",Form("%s",cut.Data()));
  TGraphAsymmErrors *g = calcEff(h2,h,npart);

  ntPythia->Draw("bin>>h",Form("(et1-et2)/(et1+et2)>%f&&%s",ajCut,cut.Data()));
  ntPythia->Draw("bin>>h2",Form("%s",cut.Data()));
  TGraphAsymmErrors *gPythia = calcEff(h2,h,npart);
  
  ntMix->Draw("bin>>h",Form("(et1-et2)/(et1+et2)>%f&&%s",ajCut,cut.Data()));
  ntMix->Draw("bin>>h2",Form("%s",cut.Data()));
  TGraphAsymmErrors *gMix = calcEff(h2,h,npart);

  TCanvas *c = new TCanvas("c","",500,500);
  //  hTmp->SetMaximum(g->GetY()[0]*2.2);
  hTmp->SetMaximum(0.7);
  hTmp->SetMinimum(0.15);

  hTmp->SetXTitle("N_{part}");
  hTmp->SetYTitle(Form("R_{A>%0.2f}",ajCut));
  hTmp->GetXaxis()->CenterTitle();
  hTmp->GetYaxis()->CenterTitle();
  hTmp->GetYaxis()->SetTitleOffset(1.5);
  hTmp->Draw();

  g->Draw("p same");
  gPythia->SetMarkerColor(4);
  gPythia->SetLineColor(4);
  gMix->SetMarkerColor(2);
  gMix->SetLineColor(2);
  gMix->Draw("p same");
  gPythia->Draw("p same");

  TLine* pline = new TLine(0,gPythia->GetY()[0],400,gPythia->GetY()[0]);
  pline->SetLineColor(4);
  pline->SetLineStyle(4);
  pline->Draw();

  if(drawLeg){
    TLegend *t3=new TLegend(0.5,0.72,0.9,0.88); 
    t3->AddEntry(g,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","pl");
    t3->AddEntry(gPythia,"PYTHIA","pl");  
    t3->AddEntry(gMix,"embedded PYTHIA","pl");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
  }
  

  TLatex *cms = new TLatex(0.20,0.88,"CMS Preliminary");
  cms->SetNDC();
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();                                                                                                                                        
  TLatex *lumi = new TLatex(0.20,0.81,"#intL dt = 3 #mub^{-1}");
  lumi->SetNDC();
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw(); 

  c->Print(Form("Rj_vs_Npart.eps",ajCut));
  c->Print(Form("Rj_vs_Npart.C",ajCut));
  c->Print(Form("Rj_vs_Npart.gif",ajCut));

}


void doAll(){
  plotRJ(0.2);
  plotRJ(0.3);
  plotRJ(0.4);
}



