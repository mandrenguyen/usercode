#include "TCanvas.h"
#include "TError.h"
#include "TPad.h"
#include "TString.h"
#include "TRandom.h"
#include "TH1F.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TString.h"
#include "TStyle.h"
#include "TLine.h"
#include "DrawTick.C"
#include "TGraphAsymmErrors.h"
#include "TF1.h"


//---------------------------------------------------------------------
void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns, 
			  const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0., 
			  const Float_t leftMargin=0.2, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);

void plotDE(int cbin = 0,
		 TString infname = "data.root",
		 TString pythia = "pythia.root",
		 TString mix = "mix.root",
		 bool useWeight = true,
		 bool drawXLabel = false,
		 bool drawLeg = false);

void plotBalanceRatio(int cbin = 0,
		 TString infname = "data.root",
		 TString pythia = "pythia.root",
		 TString mix = "mix.root",
		 bool useWeight = true,
		 bool drawXLabel = false,
		 bool drawLeg = false);

void drawText(const char *text, float xp, float yp);
void drawDum(float min, float max, double drawXLabel);

//--------------------------------------------------------------
// drawPatch() is a crazy way of removing 0 in the second and third 
// pad which is partially shown due to no margin between the pads
// if anybody has a better way of doing it let me know! - Andre
//--------------------------------------------------------------
void drawPatch(float x1, float y1, float x2, float y2); 
//---------------------------------------------------------------------

TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2)
{
   h1->Sumw2();
   h2->Sumw2();
   h2->Divide(h1);
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors(h2);
   
   return gEfficiency;
}

void plotDeltaEAllCent(){

  TCanvas *c1 = new TCanvas("c1","",1250,530);

  makeMultiPanelCanvas(c1,3,1,0.0,0.0,0.2,0.15,0.02);

  c1->cd(1);
  plotDE(2,"data.root","pythia.root","mix.root",true,false,false);
  drawText("30-100%",0.76,0.24);
  drawPatch(0.94,0.0972,1.1,0.141);
  drawText("(a)",0.22,0.9);


  c1->cd(2);
  plotDE(1,"data.root","pythia.root","mix.root",true,true,false);
  drawText("10-30%",0.75,0.24);
  drawPatch(-0.0007,0.0972,0.0518,0.141);
  drawPatch(0.94,0.0972,1.1,0.141);
  drawText("(b)",0.02,0.9);

  TLatex tsel;
  tsel.SetNDC();
  tsel.SetTextFont(63);
  tsel.SetTextSize(15);
  tsel.DrawLatex(0.55,0.9,"p_{T,1} > 120 GeV/c");
  tsel.DrawLatex(0.55,0.825,"p_{T,2} > 50 GeV/c");
  tsel.DrawLatex(0.55,0.75,"#Delta#phi_{12} > #frac{2}{3}#pi");


  c1->cd(3);
  plotDE(0,"data.root","pythia.root","mix.root",true,false,true);
  drawText("0-10%",0.75,0.24);
  drawPatch(-0.0007,0.0972,0.0518,0.141);
  drawPatch(0.93,0.0972,1.1,0.141);
  drawText("(c)",0.02,0.9);
/*
  TLatex *cms = new TLatex(0.35,0.1825,"CMS");
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();                                                                                                                                        

  TLatex *lumi = new TLatex(0.73,0.1825,"#intL dt = 6.7 #mub^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw(); 
*/
  c1->Print("./fig/deltaPtOverPt1_all_cent_20101219_v1.gif");
  c1->Print("./fig/deltaPtOverPt1_all_cent_20101219_v1.eps");
  c1->Print("./fig/deltaPtOverPt1_all_cent_20101219_v1.pdf");
  c1->Print("./fig/deltaPtOverPt1_all_cent_20101219_v1.C");

}

void plotDE(  int cbin,
		 TString infname,
		 TString pythia,
		 TString mix,
		 bool useWeight,
		 bool drawXLabel,
		 bool drawLeg)
{		
  gStyle->SetErrorX(0); 
  TString cut="et1>120&&et2>50&&dphi>3.14159265359*2/3";
  TString cutpp="et1>120&&et2>50&&dphi>3.14159265359*2/3";
  TString trigcut = "";
  TString cstring = "";
  TString divide = "";
  int type = 1;

  
  if (type==1) divide = "/et1";
  if (type==2) divide = "/et2";
  

  if(cbin==0) {
    cstring = "0-10%";
    cut+=" && bin>=0 && bin<4";
  } else if (cbin==1) {
    cstring = "10-30%";
    cut+=" && bin>=4 && bin<12";
  } else if (cbin==2){
    cstring = "30-100%";
    cut+=" && bin>=12 && bin<40";
  } else if (cbin==3) {
    cstring = "0-100%";
  }


  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");

  // open the pythia (MC) file
  TFile *infPythia = new TFile(pythia.Data());
  TTree *ntPythia = (TTree*) infPythia->FindObjectAny("nt");

  // open the datamix file
  TFile *infMix = new TFile(mix.Data());
  TTree *ntMix =(TTree*)infMix->FindObjectAny("nt");

  const int nBin = 4;
//  double m[nBin+1] = {100,110,120,130,140,170,240};
//  double m[nBin+1] = {120,130,140,170,240};
    double m[nBin+1] = {120,140,160,180,240};
  
  

  TH1D *hTmp = new TH1D("hTmp","",100,m[0],m[nBin]);
  TH1D *h = new TH1D("h","",nBin,m);
  TH1D *h2 = new TH1D("h2","",nBin,m);

  
  nt->Draw("et1>>h",Form("(et1-et2)%s*(%s)",divide.Data(),cut.Data()));
  nt->Draw("et1>>h2",Form("%s",cut.Data()));
  TGraphAsymmErrors *g = calcEff(h2,h);

  ntPythia->Draw("et1>>h",Form("(et1-et2)%s*(%s)",divide.Data(),cutpp.Data()));
  ntPythia->Draw("et1>>h2",Form("%s",cutpp.Data()));
  TGraphAsymmErrors *gPythia = calcEff(h2,h);
  
  ntMix->Draw("et1>>h",Form("(et1-et2)%s*(%s)",divide.Data(),cut.Data()));
  ntMix->Draw("et1>>h2",Form("%s",cut.Data()));
  TGraphAsymmErrors *gMix = calcEff(h2,h);

  //  hTmp->SetMaximum(g->GetY()[0]*2.2);
  hTmp->SetMaximum(0.6);
  if (type==0) hTmp->SetMaximum(100);
  hTmp->SetMinimum(0.1);

  if (drawXLabel) {
     hTmp->SetXTitle("Leading Jet p_{T} (GeV/c)");
     hTmp->GetXaxis()->CenterTitle();
  } else {
     hTmp->SetXTitle("");
     hTmp->GetXaxis()->CenterTitle();
  }

  hTmp->SetYTitle(Form("<(p_{T,1}-p_{T,2})/p_{T,1}>"));
  if (type==0) hTmp->SetYTitle(Form("<(p_{T}^{1}-p_{T}^{2})> (GeV/c)"));
  hTmp->GetYaxis()->CenterTitle();
  hTmp->GetYaxis()->SetTitleOffset(1.5);
  hTmp->GetXaxis()->SetLabelSize(22);
  hTmp->GetXaxis()->SetLabelFont(43);

  hTmp->Draw();

  TGraphAsymmErrors *gFitPythia = (TGraphAsymmErrors*) gMix->Clone();  
  TGraphAsymmErrors *gFit = (TGraphAsymmErrors*) g->Clone();  

  TF1 *f;
  if (type==1) f = new TF1("f","[0]+[1]*x+[2]*x*x+[3]",100,220);
  else         f = new TF1("f","[0]+[1]*x+[2]*x*x+[3]*x",100,220);
  f->SetLineColor(2);
  f->SetLineStyle(2);
  f->SetLineWidth(1);
  f->FixParameter(3,0);
//  gFitPythia->Fit("f");
//  gFitPythia->Fit("f");
//  gFitPythia->Fit("f");
  f->SetLineColor(1);
  
  f->FixParameter(0,f->GetParameter(0));
  f->FixParameter(1,f->GetParameter(1));
  f->FixParameter(2,f->GetParameter(2));
  f->ReleaseParameter(3);

  //gFit->Fit("f");

  TF1 *f2;
  if (type==1) f2 = new TF1("f2","[0]+[1]*x+[2]*x*x+[3]/x",100,220);
  else         f2 = new TF1("f2","[0]+[1]*x+[2]*x*x+[3]",100,220);
  f2->SetLineColor(4);
  f2->SetLineWidth(1);
  f2->FixParameter(3,0);
//  gFitPythia->Fit("f2");
//  gFitPythia->Fit("f2");
//  gFitPythia->Fit("f2");
  f2->SetLineColor(1);
  
  f2->FixParameter(0,f2->GetParameter(0));
  f2->FixParameter(1,f2->GetParameter(1));
  f2->FixParameter(2,f2->GetParameter(2));
  f2->ReleaseParameter(3);



  //gFit->Fit("f2");

  double tickSize;
  if (type==1) tickSize=0.012;
  else tickSize=0.6;
  
  double erroDEar = 0.02;
  for(int i = 0; i < g->GetN(); ++i){
    double *x = g->GetX();
    double *y = g->GetY();
    DrawTick(y[i],0.082*y[i],0.082*y[i],x[i],tickSize,4,1);
  }
  g->Draw("p same");
  g->SetMarkerSize(1.25);
  gPythia->SetMarkerSize(1.7);
  gPythia->SetMarkerColor(4);
  gPythia->SetMarkerStyle(29);
  gPythia->SetLineColor(4);
  gMix->SetMarkerColor(2);
  gMix->SetMarkerStyle(21);
  gMix->SetMarkerSize(1.25);
  gMix->SetLineColor(2);
  g->SetName("g");
  gPythia->SetName("gPythia");
  gMix->SetName("gMix");
  gMix->Draw("p same");
  gPythia->Draw("p same");
  f->Draw("same");
  f2->Draw("same");
  
  TLine* pline = new TLine(m[0],gPythia->GetY()[0],m[nBin],gPythia->GetY()[0]);
  pline->SetLineColor(4);
  pline->SetLineStyle(4);
  pline->Draw();

  if(drawLeg){
    TLegend *t3=new TLegend(0.43,0.76,0.93,0.93); 
    t3->AddEntry(g,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","p");
    t3->AddEntry(gPythia,"PYTHIA","p");  
    t3->AddEntry(gMix,"embedded PYTHIA","p");
    //t3->AddEntry(f,"Constant Ratio","l");
    //t3->AddEntry(f2,"Constant Difference","l");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
  TLatex *cms = new TLatex(0.15,0.88,"CMS");
  cms->SetNDC();
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();                                                                                                                                        
  TLatex *lumi = new TLatex(0.15,0.81,"#intL dt = 6.7 #mub^{-1}");
  lumi->SetNDC();
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw(); 
  }
//  drawText(cstring.Data(),0.76,0.64);

}


void plotBalanceRatio(int cbin,
		 TString infname,
		 TString pythia,
		 TString mix,
		 bool useWeight,
		 bool drawXLabel,
		 bool drawLeg)
{
  TString cut="et1>120 && et2>50 && dphi>2.5";
  TString cutpp="et1>120 && et2>50 && dphi>2.5";
  TString cstring = "";
  if(cbin==0) {
    cstring = "0-10%";
    cut+=" && bin>=0 && bin<4";
  } else if (cbin==1) {
    cstring = "10-30%";
    cut+=" && bin>=4 && bin<12";
  } else {
    cstring = "30-100%";
    cut+=" && bin>=12 && bin<40";
  }

  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");

  // open the pythia (MC) file
  TFile *infPythia = new TFile(pythia.Data());
  TTree *ntPythia = (TTree*) infPythia->FindObjectAny("nt");

  // open the datamix file
  TFile *infMix = new TFile(mix.Data());
  TTree *ntMix =(TTree*)infMix->FindObjectAny("nt");


  // projection histogram
  TH1D *h = new TH1D("h","",20,0,1);
  TH1D *hPythia = new TH1D("hPythia","",20,0,1);
  TH1D *hDataMix = new TH1D("hDataMix","",20,0,1);
  nt->Draw("(et1-et2)/(et1+et2)>>h",Form("(%s)",cut.Data())); 
  
  if (useWeight) {
    // use the weight value caluculated by Matt's analysis macro
    ntMix->Draw("(et1-et2)/(et1+et2)>>hDataMix",Form("(%s)*weight",cut.Data())); 
  } else {
    // ignore centrality reweighting
    ntMix->Draw("(et1-et2)/(et1+et2)>>hDataMix",Form("(%s)",cut.Data()));  
  }
  ntPythia->Draw("(et1-et2)/(et1+et2)>>hPythia",Form("(%s)",cutpp.Data()));

  // calculate the statistical error and normalize
  h->Sumw2();
  h->Scale(1./h->GetEntries());
  h->SetMarkerStyle(20);
  hDataMix->Sumw2();
  hPythia->Sumw2();

  hPythia->Scale(1./hPythia->Integral(0,20));
  hPythia->SetLineColor(kBlue);
  hPythia->SetFillColor(kAzure-8);
  hPythia->SetFillStyle(3005);

  hPythia->SetStats(0);
  hPythia->Draw("hist");

  if(drawXLabel) hPythia->SetXTitle("A_{J} = (E_{T}^{j1}-E_{T}^{j2})/(E_{T}^{j1}+E_{T}^{j2})");

  hPythia->GetXaxis()->SetLabelSize(20);
  hPythia->GetXaxis()->SetLabelFont(43);
  hPythia->GetXaxis()->SetTitleSize(22);
  hPythia->GetXaxis()->SetTitleFont(43);
  hPythia->GetXaxis()->SetTitleOffset(1.5);
  hPythia->GetXaxis()->CenterTitle();

  hPythia->GetXaxis()->SetNdivisions(905,true);
  
  hPythia->SetYTitle("Event Fraction");

  hPythia->GetYaxis()->SetLabelSize(20);
  hPythia->GetYaxis()->SetLabelFont(43);
  hPythia->GetYaxis()->SetTitleSize(20);
  hPythia->GetYaxis()->SetTitleFont(43);
  hPythia->GetYaxis()->SetTitleOffset(2.5);
  hPythia->GetYaxis()->CenterTitle();
  

  hPythia->SetAxisRange(0,0.2,"Y");

  hDataMix->Scale(1./hDataMix->Integral(0,20));
  hDataMix->SetLineColor(kRed);
  hDataMix->SetFillColor(kRed-9);
  hDataMix->SetFillStyle(3004);
  hDataMix->Divide(hPythia);
  hDataMix->SetAxisRange(0,2,"Y");
  hDataMix->Draw("p");

  if(drawLeg){
    TLegend *t3=new TLegend(0.31,0.675,0.85,0.88); 
    t3->AddEntry(h,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","p");
    t3->AddEntry(hPythia,"PYTHIA","lf");  
    t3->AddEntry(hDataMix,"embedded PYTHIA","lf");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
  }

}

void drawPatch(float x1, float y1, float x2, float y2){
  TLegend *t1=new TLegend(x1,y1,x2,y2);
  t1->SetFillColor(kWhite);
  t1->SetBorderSize(0);
  t1->SetFillStyle(1001);
  t1->Draw("");
}

void drawText(const char *text, float xp, float yp){
  TLatex *tex = new TLatex(xp,yp,text);
  tex->SetTextFont(63);
  tex->SetTextSize(22);
  //tex->SetTextSize(0.05);
  tex->SetTextColor(kBlack);
  tex->SetLineWidth(1);
  tex->SetNDC();
  tex->Draw();
}

void drawDum(float min, float max, double drawXLabel){

  TH1D *hdum = new TH1D("hdum","",20,0,1);
  hdum->SetMaximum(max);

  hdum->SetStats(0);

  if(drawXLabel) hdum->SetXTitle("A_{J} = (E_{T}^{j1}-E_{T}^{j2})/(E_{T}^{j1}+E_{T}^{j2})");
  hdum->GetXaxis()->SetLabelSize(20);
  hdum->GetXaxis()->SetLabelFont(43);
  hdum->GetXaxis()->SetTitleSize(22);
  hdum->GetXaxis()->SetTitleFont(43);
  hdum->GetXaxis()->SetTitleOffset(1.5);
  hdum->GetXaxis()->CenterTitle();

  hdum->GetXaxis()->SetNdivisions(905,true);

  hdum->SetYTitle("Event Fraction");

  hdum->GetYaxis()->SetLabelSize(20);
  hdum->GetYaxis()->SetLabelFont(43);
  hdum->GetYaxis()->SetTitleSize(20);
  hdum->GetYaxis()->SetTitleFont(43);
  hdum->GetYaxis()->SetTitleOffset(2.5);
  hdum->GetYaxis()->CenterTitle();

  hdum->SetAxisRange(0,0.2,"Y");

  hdum->Draw("");

}

void makeMultiPanelCanvas(TCanvas*& canv,
                          const Int_t columns,
                          const Int_t rows,
                          const Float_t leftOffset,
                          const Float_t bottomOffset,
                          const Float_t leftMargin,
                          const Float_t bottomMargin,
                          const Float_t edge) {
   if (canv==0) {
      Error("makeMultiPanelCanvas","Got null canvas.");
      return;
   }
   canv->Clear();
   
   TPad* pad[columns][rows];

   Float_t Xlow[columns];
   Float_t Xup[columns];
   Float_t Ylow[rows];
   Float_t Yup[rows];
   Float_t PadWidth = 
   (1.0-leftOffset)/((1.0/(1.0-leftMargin)) +
   (1.0/(1.0-edge))+(Float_t)columns-2.0);
   Float_t PadHeight =
   (1.0-bottomOffset)/((1.0/(1.0-bottomMargin)) +
   (1.0/(1.0-edge))+(Float_t)rows-2.0);
   Xlow[0] = leftOffset;
   Xup[0] = leftOffset + PadWidth/(1.0-leftMargin);
   Xup[columns-1] = 1;
   Xlow[columns-1] = 1.0-PadWidth/(1.0-edge);

   Yup[0] = 1;
   Ylow[0] = 1.0-PadHeight/(1.0-edge);
   Ylow[rows-1] = bottomOffset;
   Yup[rows-1] = bottomOffset + PadHeight/(1.0-bottomMargin);

   for(Int_t i=1;i<columns-1;i++) {
      Xlow[i] = Xup[0] + (i-1)*PadWidth;
      Xup[i] = Xup[0] + (i)*PadWidth;
   }
   Int_t ct = 0;
   for(Int_t i=rows-2;i>0;i--) {
      Ylow[i] = Yup[rows-1] + ct*PadHeight;
      Yup[i] = Yup[rows-1] + (ct+1)*PadHeight;
      ct++;
   }

   TString padName;
   for(Int_t i=0;i<columns;i++) {
      for(Int_t j=0;j<rows;j++) {
         canv->cd();
         padName = Form("p_%d_%d",i,j);
         pad[i][j] = new TPad(padName.Data(),padName.Data(),
            Xlow[i],Ylow[j],Xup[i],Yup[j]);
         if(i==0) pad[i][j]->SetLeftMargin(leftMargin);
         else pad[i][j]->SetLeftMargin(0);

         if(i==(columns-1)) pad[i][j]->SetRightMargin(edge);
         else pad[i][j]->SetRightMargin(0);

         if(j==0) pad[i][j]->SetTopMargin(edge);
         else pad[i][j]->SetTopMargin(0);

         if(j==(rows-1)) pad[i][j]->SetBottomMargin(bottomMargin);
         else pad[i][j]->SetBottomMargin(0);

         pad[i][j]->Draw();
         pad[i][j]->cd();
         pad[i][j]->SetNumber(columns*j+i+1);
      }
   }
}
