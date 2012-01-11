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

#include "weightMix.C"

static const bool doFit = 0;
static const int type = 5;
static const bool doRMS = 0;

//---------------------------------------------------------------------
void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns, 
			  const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0., 
			  const Float_t leftMargin=0.2, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);

TGraphAsymmErrors* plotDE(int cbin = 0,
			  TString infname = "data.root",
			  TString pythia = "pythia.root",
			  TString mix = "mix.root",
			  bool useWeight = true,
			  bool drawXLabel = false,
			  bool drawLeg = false,
			  int sys = 0
			  );

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

      errYL2 = 0;
      errYH2 = 0;

      errYL= 0;
      errYH= 0;

      gEfficiency->SetPointError(i,0,0,sqrt(errYL*errYL+errYL2*errYL2)/y2,sqrt(errYH*errYH+errYH2*errYH2)/y2);
      gEfficiency->SetPoint(i,x,y/y2);
    }
  return gEfficiency;
}


TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2, int shift = 0, double* bincenters = 0)
{
   TH1D* hR = (TH1D*)h2->Clone("hR");
   hR->Divide(h2,h1,1,1,"B");
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors(hR);

   double x,y; 
   if(1){
     for(int i = 0; i < gEfficiency->GetN(); ++i){
       gEfficiency->GetPoint(i,x,y);
       x = bincenters[i];
       gEfficiency->SetPoint(i,x,y);
       gEfficiency->SetPointError(i,0,0,h2->GetBinError(i+1)/h1->GetBinContent(i+1),h2->GetBinError(i+1)/h1->GetBinContent(i+1));
     }

   }
  
   return gEfficiency;
}

TGraphAsymmErrors *getMean(TH2D* h, TH2D* hB, double* bincenters = 0){

  TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors(h->GetNbinsX());


  double showerScale = 0.68;
  double sideScale = sideCorrect*(3.1415926536-2.0944)/(sideMax-sideMin)*1;

  double x,y,e;
  for(int i = 0; i < h->GetNbinsX(); ++i){
    x = bincenters[i];
    TH1D* hh = h->ProjectionY("hy",i+1,i+1);
    TH1D* hb = hB->ProjectionY("hby",i+1,i+1);
    hb->Scale(sideScale);

    hh->Add(hb,-1);
    y = hh->GetMean();
    e = hh->GetMeanError();

    if(doRMS){
      y = hh->GetRMS();
      e = hh->GetRMSError();
    }

    gEfficiency->SetPoint(i,x,y);
    gEfficiency->SetPointError(i,0,0,e,e);
  }

  return gEfficiency;

}


void plotScaleSys(int cent = 0){
  string sysNames[]={
    "",
  };

  TGraphAsymmErrors* gs0 = plotDE(cent,"data.root","data_pp.root","mix.root",false,false,false,1);
  TGraphAsymmErrors* gs1 = plotDE(cent,"data.root","data_pp.root","mix.root",false,false,false,3);
  TGraphAsymmErrors* gs2 = plotDE(cent,"data.root","data_pp.root","mix.root",false,false,false,5);
  TGraphAsymmErrors* gs3 = plotDE(cent,"data.root","data_pp.root","mix.root",false,false,false,7);

  TGraphAsymmErrors* gs4 = plotDE(cent,"data.root","data_pp.root","mix.root",false,false,false,1+1);
  TGraphAsymmErrors* gs5 = plotDE(cent,"data.root","data_pp.root","mix.root",false,false,false,3+1);
  TGraphAsymmErrors* gs6 = plotDE(cent,"data.root","data_pp.root","mix.root",false,false,false,5+1);
  TGraphAsymmErrors* gs7 = plotDE(cent,"data.root","data_pp.root","mix.root",false,false,false,7+1);

  int ref = 0;
  TGraphAsymmErrors* g0 = plotDE(cent,"data.root","data_pp.root","mix.root",false,false,false,ref);

  TGraphAsymmErrors* gR0 = divideGraph(gs0,g0);
  TGraphAsymmErrors* gR1 = divideGraph(gs1,g0);
  TGraphAsymmErrors* gR2 = divideGraph(gs2,g0);
  TGraphAsymmErrors* gR3 = divideGraph(gs3,g0);
  TGraphAsymmErrors* gR4 = divideGraph(gs4,g0);
  TGraphAsymmErrors* gR5 = divideGraph(gs5,g0);
  TGraphAsymmErrors* gR6 = divideGraph(gs6,g0);
  TGraphAsymmErrors* gR7 = divideGraph(gs7,g0);

  TF1* f0 = new TF1("f0","pol1",120,320);
  TF1* f1 = new TF1("f1","pol1",120,320);
  TF1* f2 = new TF1("f2","pol1",120,320);
  TF1* f3 = new TF1("f3","pol1",120,320);
  TF1* f4 = new TF1("f4","pol1",120,320);
  TF1* f5 = new TF1("f5","pol1",120,320);
  TF1* f6 = new TF1("f6","pol1",120,320);
  TF1* f7 = new TF1("f7","pol1",120,320);

  f1->SetLineColor(4);
  f2->SetLineColor(3);
  f3->SetLineColor(2);
  f5->SetLineColor(4);
  f6->SetLineColor(3);
  f7->SetLineColor(2);

  f4->SetLineStyle(2);
  f5->SetLineStyle(2);
  f6->SetLineStyle(2);
  f7->SetLineStyle(2);

  gR4->SetMarkerStyle(24);
  gR5->SetMarkerStyle(24);
  gR6->SetMarkerStyle(24);
  gR7->SetMarkerStyle(24);

  gR1->SetMarkerColor(4);
  gR1->SetLineColor(4);
  gR2->SetMarkerColor(3);
  gR2->SetLineColor(3);
  gR3->SetMarkerColor(2);
  gR3->SetLineColor(2);

  gR5->SetMarkerColor(4);
  gR5->SetLineColor(4);
  gR6->SetMarkerColor(3);
  gR6->SetLineColor(3);
  gR7->SetMarkerColor(2);
  gR7->SetLineColor(2);

  gR0->Fit(f0);
  gR1->Fit(f1);
  gR2->Fit(f2);
  gR3->Fit(f3);
  gR4->Fit(f4);
  gR5->Fit(f5);
  gR6->Fit(f6);
  gR7->Fit(f7);

  double sl0 = f0->GetParameter(1);
  double sl1 = f1->GetParameter(1);
  double sl2 = f2->GetParameter(1);
  double sl3 = f3->GetParameter(1);

  double av0 = f0->Eval(120)-1;
  double av1 = f1->Eval(120)-1;
  double av2 = f2->Eval(120)-1;
  double av3 = f3->Eval(120)-1;

  if(fabs(f0->Eval(320)-1) > fabs(av0)) av0 = f0->Eval(320)-1;
  if(fabs(f1->Eval(320)-1) > fabs(av1)) av1 = f1->Eval(320)-1;
  if(fabs(f2->Eval(320)-1) > fabs(av2)) av2 = f2->Eval(320)-1;
  if(fabs(f3->Eval(320)-1) > fabs(av3)) av3 = f3->Eval(320)-1;

  TH2D* hPad = new TH2D("hPad",";<p_{T,1}> (GeV/c);Variation/Result",100,120,320,100,0.8,1.2);
  hPad->SetYTitle(Form("<(p_{T,1}-p_{T,2})/p_{T,1}> Ratio"));
  if (type==0) hPad->SetYTitle(Form("<(p_{T}^{1}-p_{T}^{2})> Ratio"));
  if (type==3) hPad->SetYTitle(Form("<A_{J}> Ratio"));

  hPad->GetYaxis()->CenterTitle();
  hPad->GetXaxis()->CenterTitle();

  string centLabels[] = {
    "0-10%",
    "10-20%",
    "20-30%",
    "30-50%",
    "50-70%",
    "70-100%"
  };


  TCanvas* c1 = new TCanvas("cs1","",600,600);
  hPad->Draw();
  gR0->Draw("p same");
  gR1->Draw("p same");
  gR2->Draw("p same");
  gR3->Draw("p same");
  gR4->Draw("p same");
  gR5->Draw("p same");
  gR6->Draw("p same");
  gR7->Draw("p same");

  TLine* line = new TLine(120,1,320,1);
  line->SetLineStyle(2);
  line->Draw();

  TLegend *t3=new TLegend(0.18,0.7,0.75,0.93);
  t3->AddEntry("",Form("%s",centLabels[cent].data()),"");
  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(33);
  t3->Draw();


  c1->Print(Form("ScaleSystematics_deltaPt%d_cent%d_v0.gif",type,cent));
  c1->Print(Form("ScaleSystematics_deltaPt%d_cent%d_v0.eps",type,cent));
  c1->Print(Form("ScaleSystematics_deltaPt%d_cent%d_v0.pdf",type,cent));

}


void plotDeltaEAllCentSys(int sys = 4){

  /*
  //sys 

0 - nothing
1 - Jet energy scale down
2 - Jet energy scale up
3 - Jet smear
4 - Jet efficiency
5 - 
6 - 
7  - Evt plane up
8  - Evt plane down
9  - Evt plane up MC
10 - Evt plane down MC
  */

  string sysNames[]={
    "",
    "Energy Scale Up",
    "Energy Scale Down",
    "Energy Scale Up",
    "Energy Scale Down",
    "Energy Scale Up",
    "Energy Scale Down",
    "Energy Scale Up",
    "Energy Scale Down",
    "Energy Resolution",
    "",
    "",
    "",
    "Jet Efficiency",
    "",
    "",
    "Event plane aligned",
    "Event plane perp",
    "",
    ""
  };

  int centrality = 2;

  TGraphAsymmErrors* gs0 = plotDE(0,"data.root","data_pp.root","mix.root",false,false,false,sys);
  TGraphAsymmErrors* gs1 = plotDE(2,"data.root","data_pp.root","mix.root",false,false,false,sys);
  TGraphAsymmErrors* gs2 = plotDE(5,"data.root","data_pp.root","mix.root",false,false,false,sys);

  int ref = 0;
  if(sys == 7) ref = 8;
  if(sys == 9) ref = 10;
  TGraphAsymmErrors* g0 = plotDE(0,"data.root","data_pp.root","mix.root",false,false,false,ref);
  TGraphAsymmErrors* g1 = plotDE(2,"data.root","data_pp.root","mix.root",false,false,false,ref);
  TGraphAsymmErrors* g2 = plotDE(5,"data.root","data_pp.root","mix.root",false,false,false,ref);


  TGraphAsymmErrors* gR0 = divideGraph(gs0,g0);
  TGraphAsymmErrors* gR1 = divideGraph(gs1,g1);
  TGraphAsymmErrors* gR2 = divideGraph(gs2,g2);

  TF1* f0 = new TF1("f0","pol1",120,320);
  TF1* f1 = new TF1("f1","pol1",120,320);
  TF1* f2 = new TF1("f2","pol1",120,320);

  f1->SetLineColor(2);
  f2->SetLineColor(4);

  gR2->SetMarkerColor(4);
  gR2->SetLineColor(4);
  gR1->SetMarkerColor(2);
  gR1->SetLineColor(2);

  if(0){
  gR0->Fit(f0);
  gR1->Fit(f1);
  gR2->Fit(f2);
  }

  double sl0 = f0->GetParameter(1);
  double sl1 = f1->GetParameter(1);
  double sl2 = f2->GetParameter(1);

  double av0 = f0->Eval(120)-1;
  double av1 = f1->Eval(120)-1;
  double av2 = f2->Eval(120)-1;

  if(fabs(f0->Eval(320)-1) > fabs(av0)) av0 = f0->Eval(320)-1;
  if(fabs(f1->Eval(320)-1) > fabs(av1)) av1 = f1->Eval(320)-1;
  if(fabs(f2->Eval(320)-1) > fabs(av2)) av2 = f2->Eval(320)-1;

  TH2D* hPad = new TH2D("hPad",";<p_{T,1}> (GeV/c);Variation/Result",100,120,320,100,0.95,1.05);

  /*
  if(sys == 1) hPad = new TH2D("hPad",";<p_{T,1}> (GeV/c);Variation/Result",100,120,320,100,0.8,1.5);
  if(sys == 2) hPad = new TH2D("hPad",";<p_{T,1}> (GeV/c);Variation/Result",100,120,320,100,0.94,1.13);
  if(sys == 4) hPad = new TH2D("hPad",";<p_{T,1}> (GeV/c);Variation/Result",100,120,320,100,0.94,1.13);
  */

  hPad->SetYTitle(Form("<(p_{T,1}-p_{T,2})/p_{T,1}> Ratio"));
  if (type==0) hPad->SetYTitle(Form("<(p_{T}^{1}-p_{T}^{2})> Ratio"));
  if (type==3) hPad->SetYTitle(Form("<A_{J}> Ratio"));
  //  if (type==5) hPad->SetYTitle(Form("<(p_{T}^{1}-p_{T}^{2})> Ratio"));

  hPad->GetYaxis()->CenterTitle();
  hPad->GetXaxis()->CenterTitle();

  TCanvas* c1 = new TCanvas("cs1","",600,600);
  hPad->Draw();
  gR0->SetMarkerStyle(24);
  gR1->SetMarkerStyle(24);
  gR2->SetMarkerStyle(24);

  gR0->SetMarkerSize(1.5);
  gR1->SetMarkerSize(1.5);
  gR2->SetMarkerSize(1.5);

  gR0->Draw("p same");
  gR1->Draw("p same");
  gR2->Draw("p same");

  TLine* line = new TLine(120,1,320,1);
  line->SetLineStyle(2);
  line->Draw();

  TLegend *t3=new TLegend(0.18,0.7,0.75,0.93);
  t3->AddEntry("",Form("%s Systematics",sysNames[sys].data()),"");
  t3->AddEntry(gR0,Form("0-10%s Max : %s%0.1f, Slope : %s%0.1f/20(GeV/c)","%","%",(100.*av0),"%",(2000.*sl0)),"p");
  t3->AddEntry(gR1,Form("20-30%s Max : %s%0.1f, Slope : %s%0.1f/20(GeV/c)","%","%",(100.*av1),"%",(2000.*sl1)),"p");
  t3->AddEntry(gR2,Form("70-100%s Max : %s%0.1f, Slope : %s%0.1f/20(GeV/c)","%","%",(100.*av2),"%",(2000.*sl2)),"p");

  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(43);
  t3->SetTextSize(13);
  t3->Draw();

  c1->Print(Form("Systematics_deltaPt%d_sys%d_v0.gif",type,sys));
  c1->Print(Form("Systematics_deltaPt%d_sys%d_v0.eps",type,sys));
  c1->Print(Form("Systematics_deltaPt%d_sys%d_v0.pdf",type,sys));

}


void plotDeltaEAllCentAna(){

  TCanvas *c1 = new TCanvas("c1","",1050,700);
  makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.2,0.2,0.02);

  c1->cd(1);
  plotDE(5,"data.root","data_pp.root","mix.root",true,false,false);
  drawText("70-100%",0.68,0.089);
  drawPatch(0.94,0.0972,1.1,0.141);
  drawText("(a)",0.22,0.9);

  TLatex *cms = new TLatex(0.31,0.92,"CMS Preliminary");
  cms->SetNDC();
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();  

  TLatex *lumi = new TLatex(0.31,0.85,Form("%s #mub^{-1}",LUM));
  lumi->SetNDC();
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw();


  c1->cd(2);
  plotDE(4,"data.root","data_pp.root","mix.root",true,true,false);
  drawText("50-70%",0.68,0.089);
  drawPatch(-0.0007,0.0972,0.0518,0.141);
  drawPatch(0.94,0.0972,1.1,0.141);
  drawText("(b)",0.02,0.9);

  TLatex tsel;
  tsel.SetNDC();
  tsel.SetTextFont(63);
  tsel.SetTextSize(15);
  tsel.DrawLatex(0.15,0.25,Form("p_{T,1} > %d GeV/c",leadCut));
  tsel.DrawLatex(0.15,0.15,Form("p_{T,2} > %d GeV/c",subleadCut));
  tsel.DrawLatex(0.15,0.05,"#Delta#phi_{12} > #frac{2}{3}#pi");

  c1->cd(3);
  plotDE(3,"data.root","data_pp.root","mix.root",true,false,true);
  drawText("30-50%",0.68,0.089);
  drawPatch(-0.0007,0.0972,0.0518,0.141);
  drawPatch(0.93,0.0972,1.1,0.141);
  drawText("(c)",0.02,0.9);

  c1->cd(4);
  plotDE(2,"data.root","data_pp.root","mix.root",true,false,false);
  drawText("20-30%",0.75,0.24);
  drawPatch(-0.0007,0.0972,0.0518,0.141);
  drawPatch(0.93,0.0972,1.1,0.141);
  drawText("(d)",0.22,0.9);

  c1->cd(5);
  plotDE(1,"data.root","data_pp.root","mix.root",true,true,false);
  drawText("10-20%",0.75,0.24);
  drawPatch(-0.0007,0.0972,0.0518,0.141);
  drawPatch(0.93,0.0972,1.1,0.141);
  drawText("(e)",0.02,0.9);

  c1->cd(6);
  plotDE(0,"data.root","data_pp.root","mix.root",true,false,false);
  drawText("0-10%",0.75,0.24);
  drawPatch(-0.0007,0.0972,0.0518,0.141);
  drawPatch(0.93,0.0972,1.1,0.141);
  drawText("(f)",0.02,0.9);


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
  c1->Print(Form("./fig/deltaPtOverPt%d_lead%d_sub%d%s%s_20120103.gif",type,leadCut,subleadCut,doFit ? "_fit" : "",doRMS?"_rms":""));
  c1->Print(Form("./fig/deltaPtOverPt%d_lead%d_sub%d%s%s_20120103.pdf",type,leadCut,subleadCut,doFit ? "_fit" : "",doRMS?"_rms":""));
  c1->Print(Form("./fig/deltaPtOverPt%d_lead%d_sub%d%s%s_20120103.eps",type,leadCut,subleadCut,doFit ? "_fit" : "",doRMS?"_rms":""));


}

TGraphAsymmErrors* plotDE(  int cbin,
		 TString infname,
		 TString pythia,
		 TString mix,
		 bool useWeight,
		 bool drawXLabel,
	      bool drawLeg,
	      int sys
	      )
{		

  string pt1name = "pt1";
  string pt2name = "pt2";
  if(sys >= 1 && sys < 20){
    pt1name = Form("pt1sys%d",sys);
    pt2name = Form("pt2sys%d",sys);
  }

  gStyle->SetErrorX(0); 
  TString cut="pt1>120&&pt2>30&&abs(dphi)>3.14159265359*2/3&& abs(eta1) < 1.5 && abs(eta2) < 1.5";
  TString cutpp="pt1>120&&pt2>30&&abs(dphi)>3.14159265359*2/3&& abs(eta1) < 1.5 && abs(eta2) < 1.5";
  TString cutmc = cut;

  TString side=Form("pt1>%d && pt2>%d && abs(dphi)>%f && abs(dphi)<%f && abs(eta1) < 1.5 && abs(eta2) < 1.5",leadCut,subleadCut, sideMin, sideMax);
  TString sidepp=Form("pt1>%d && pt2>%d && abs(dphi)>%f && abs(dphi)<%f && abs(eta1) < 1.5 && abs(eta2) < 1.5",leadCut,subleadCut, sideMin, sideMax);
  TString cutNorm=Form("pt1>%d && abs(eta1) < 2",leadCut);

  double sideScale = sideCorrect*(3.1415926536-2.0944)/(sideMax-sideMin);

  if(sys > 0){
    cut=Form("%s>120&&%s>30&&abs(dphi)>3.14159265359*2/3&& abs(eta1) < 2 && abs(eta2) < 2",pt1name.data(),pt2name.data());
  }

  if(sys == 13){
    pt1name = "pt1";
    cut=Form("%s>120&&%s>30&&abs(dphisys13)>3.14159265359*2/3&& abs(eta1) < 2 && abs(eta2sys13) < 2",pt1name.data(),pt2name.data());

  }

  TString trigcut = "";
  TString cstring = "";
  TString divide = "";
  //  useWeight = 0;
  
  if (type==0) divide = Form("(%s-%s)",pt1name.data(),pt2name.data());
  if (type==1) divide = Form("(%s-%s)/%s",pt1name.data(),pt2name.data(),pt1name.data());
  if (type==2) divide = Form("(%s-%s)/%s",pt1name.data(),pt2name.data(),pt2name.data());
  if (type==3) divide = Form("(%s-%s)/(%s+%s)",pt1name.data(),pt2name.data(),pt1name.data(),pt2name.data());
  if (type==5) divide = Form("%s/%s",pt2name.data(),pt1name.data());
   
  if(cbin==-1) {
    cstring = "0-100%";
    cut+=" && bin>=0 && bin<40";
    cutmc+=" && bin>=0 && bin<40";
    side+=" && bin>=0 && bin<40";
    cutNorm+=" && bin>=0 && bin<40";
  }
  else if(cbin==0) {
    cstring = "0-10%";
    cut+=" && bin>=0 && bin<4";
    cutmc+=" && bin>=0 && bin<4"; 
    side+=" && bin>=0 && bin<4";
    cutNorm+=" && bin>=0 && bin<4";

  } else if (cbin==1) {
    cstring = "10-20%";
    cut+=" && bin>=4 && bin<8";
    cutmc+=" && bin>=4 && bin<8"; 
    side+=" && bin>=4 && bin<8";
    cutNorm+=" && bin>=4 && bin<8";

  } else if (cbin==2) {
    cstring = "20-30%";
    cut+=" && bin>=8 && bin<12";
    cutmc+=" && bin>=8 && bin<12";
    side+=" && bin>=8 && bin<12";
    cutNorm+=" && bin>=8 && bin<12";

  } else if (cbin==3) {
    cstring = "30-50%";
    cut+=" && bin>=12 && bin<20";
    cutmc+=" && bin>=12 && bin<20";
    side+=" && bin>=12 && bin<20";
    cutNorm+=" && bin>=12 && bin<20";

  }else if (cbin==4) {
    cstring = "50-70%";
    cut+=" && bin>=20 && bin<28";
    cutmc+=" && bin>=20 && bin<28";
    side+=" && bin>=20 && bin<28";
    cutNorm+=" && bin>=20 && bin<28";

  }else if (cbin==5) {
    cstring = "70-100%";
    cut+=" && bin>=28";
    cutmc+=" && bin>=28";
    side+=" && bin>=28";
    cutNorm+=" && bin>=28";

  }

  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");
  //  TChain* nt = new TChain("nt");
  //  nt->Add("/Users/yetkinyilmaz/analysis/data2011/dijet20111215/Hi*.root");

  // open the pythia (MC) file
  TFile *infPythia = new TFile(pythia.Data());
  TTree *ntPythia = (TTree*) infPythia->FindObjectAny("nt");

  // open the datamix file
  TFile *infMix = new TFile(mix.Data());
  TTree *ntMix =(TTree*)infMix->FindObjectAny("nt");

  TFile *infW = new TFile("weights.root");
  TTree *ntw =(TTree*)infW->FindObjectAny("ntw");
  ntMix->AddFriend(ntw);

  const int nBin = 10;
  //  double m[nBin+1] = {100,110,120,130,140,170,240};
  //  double m[nBin+1] = {120,130,140,170,240};
  //  double m[nBin+1] =     {120,150,180,220,260,300,1500};
  //  double m[nBin+1] = {120,140,160,180,200,230,1500};
  double m[nBin+1] =     {120,130,140,150,160,180,200,230,260,300,1500};

  double cm0[nBin+1] = {0,0,0,0,0,0,0};
  double cm1[nBin+1] = {0,0,0,0,0,0,0};
  double cm2[nBin+1] = {0,0,0,0,0,0,0};
  double cme0[nBin+1] = {0,0,0,0,0,0,0};
  double cme1[nBin+1] = {0,0,0,0,0,0,0};
  double cme2[nBin+1] = {0,0,0,0,0,0,0};

  TH1D* hDum = new TH1D("hDum","",1000,0,1000);

  for(int i = 0; i < nBin; ++i){
    nt->Draw("pt1>>hDum",Form("noise < 0 && %s && pt1 >= %f && pt1 < %f",cut.Data(),m[i],m[i+1]));
    cm0[i] = hDum->GetMean();
    cme0[i] = hDum->GetMeanError();

    if(sys ==0 || sys ==9 || sys == 10){
      ntMix->Draw("pt1>>hDum",Form("weight*(%s && pt1 >= %f && pt1 < %f)",cutmc.Data(),m[i],m[i+1]));
      cm1[i] = hDum->GetMean();
      cme1[i] = hDum->GetMeanError();
      ntPythia->Draw("pt1>>hDum",Form("abs(eta1) < 2 && abs(eta2) < 2 && abs(dphi) > 2.0944&& pt1 >= %f && pt1 < %f",m[i],m[i+1]));
      cm2[i] = hDum->GetMean();
      cme2[i] = hDum->GetMeanError();
    }
  }
  
  double dmax = 200;
  if(type > 0) dmax = 1.;

  const int Ndbins = 5000;
  double dbins[Ndbins+1];

  for(int i = 0; i < Ndbins+1; ++i){
    dbins[i] = i*(dmax/Ndbins);
  }

  TH1D *hTmp = new TH1D("hTmp","",100,m[0],370);
  TH2D *h = new TH2D("h","",nBin,m,Ndbins,dbins);
  TH2D *hb = new TH2D("hb","",nBin,m,Ndbins,dbins);

  TGraphAsymmErrors *g, *gPythia, *gMix;

  nt->Draw(Form("(%s):%s>>h",divide.Data(),pt1name.data()),Form("noise < 0 && %s",cut.Data()));
  nt->Draw(Form("(%s):%s>>hb",divide.Data(),pt1name.data()),Form("noise < 0 && %s",side.Data()));

  g = getMean(h,hb,cm0);

  if(sys == 0 || sys == 119 || sys == 210){
    ntPythia->Draw(Form("(%s):%s>>h",divide.Data(),pt1name.data()),Form("%s",cutpp.Data()));
    ntPythia->Draw(Form("(%s):%s>>hb",divide.Data(),pt1name.data()),Form("%s",sidepp.Data()));
    gPythia = getMean(h,hb,cm2);
    
    ntMix->Draw(Form("(%s):%s>>h",divide.Data(),pt1name.data()),Form("weight*(%s)",cut.Data()));
    ntMix->Draw(Form("(%s):%s>>hb",divide.Data(),pt1name.data()),Form("weight*(%s)",side.Data()));
    gMix = getMean(h,hb,cm1);

  }  
  hTmp->SetMaximum(0.6);
  //  if(type == 1) hTmp->SetMaximum(0.5);
  hTmp->SetMinimum(0.1);

  if(type == 3) hTmp->SetMaximum(1.2);

  if(type == 5){ 
    hTmp->SetMaximum(.88);
    hTmp->SetMinimum(0.46);
  }

  if (type==0){ 
    hTmp->SetMaximum(150);
    hTmp->SetMinimum(0.);
  }

  if (type==3){ 
    hTmp->SetMaximum(0.5);
    hTmp->SetMinimum(0);
  }

  if (type==1){
    hTmp->SetMaximum(1.);
    hTmp->SetMinimum(0);
  }

  if (drawXLabel) {
     hTmp->SetXTitle("p_{T,1} (GeV/c)");
     hTmp->GetXaxis()->CenterTitle();
  } else {
     hTmp->SetXTitle("");
     hTmp->GetXaxis()->CenterTitle();
  }

  hTmp->SetYTitle(Form("<(p_{T,1}-p_{T,2})/p_{T,1}>"));
  if (type==0) hTmp->SetYTitle(Form("<(p_{T}^{1}-p_{T}^{2})> (GeV/c)"));
  if (type==3) hTmp->SetYTitle(Form("<A_{J}>"));
  if (type==5) hTmp->SetYTitle(Form("<p_{T}^{2}/p_{T}^{1}>"));



  hTmp->GetYaxis()->CenterTitle();
  hTmp->GetYaxis()->SetTitleOffset(2.2);
  hTmp->GetYaxis()->SetTitleSize(28);
  hTmp->GetXaxis()->SetTitleSize(28);
  hTmp->GetXaxis()->SetTitleOffset(2.2);

  hTmp->GetXaxis()->SetLabelSize(22);
  hTmp->GetXaxis()->SetLabelFont(43);
  hTmp->GetXaxis()->SetNdivisions(8);
  hTmp->GetYaxis()->SetNdivisions(5);

  hTmp->GetYaxis()->SetLabelSize(22);
  hTmp->GetYaxis()->SetLabelFont(43);

  hTmp->GetXaxis()->SetTitleFont(43);
  hTmp->GetYaxis()->SetTitleFont(43);

  hTmp->Draw();

  TF1 *f, *f2;
  TGraphAsymmErrors *gFitMix, *gFit;

  //  TF1* fBase = new TF1("fBase","sqrt(pow([0],2)+pow([1]*x,2)+pow([2]*x*x,2))",100,220);
  TF1* fBase = new TF1("fBase","pol2(0)",100,350);
  fBase->SetLineColor(mixColor);

  //  fBase->SetParLimits(0,0.0001,10);
  //  fBase->SetParLimits(1,0.0001,10);
  //  fBase->SetParLimits(2,0.0001,10);
  
  if(sys == 0 || sys == 99 || sys == 910){
    gFitMix = (TGraphAsymmErrors*) gMix->Clone();  
    gFit = (TGraphAsymmErrors*) g->Clone();  

    //    if (type==1) f = new TF1("f","sqrt(pow([0],2)+pow([1]*x,2)+pow([2]*x*x,2)+pow([3],2))",100,220);
    //    else         f = new TF1("f","sqrt(pow([0],2)+pow([1]*x,2)+pow([2]*x*x,2)+pow([3]*x,2))",100,220);

    if (type==1) f = new TF1("f","sqrt(pow(pol2(0),2)+pow([3],2))",100,350);
    else if (type==5) f = new TF1("f","sqrt(pow(pol2(0),2)+pow([3],2))",100,350);
    else f = new TF1("f","sqrt(pow(pol2(0),2)+pow([3]*x,2))",100,350);

    //    f->SetParLimits(0,0.0001,1000);
    //    f->SetParLimits(1,0.0001,1000);
    //    f->SetParLimits(2,0.0001,1000);

  f->SetLineColor(2);
  f->SetLineStyle(2);
  f->SetLineWidth(2);
  f->FixParameter(3,0);
  f->SetLineColor(1);

  gFitMix->Fit(f,"MR");

  f->FixParameter(0,f->GetParameter(0));
  f->FixParameter(1,f->GetParameter(1));
  f->FixParameter(2,f->GetParameter(2));
  f->ReleaseParameter(3);

  fBase->SetParameter(0,f->GetParameter(0));
  fBase->SetParameter(1,f->GetParameter(1));
  fBase->SetParameter(2,f->GetParameter(2));

  if(doFit) gFit->Fit(f,"MR");
  if(doFit) gFit->Fit(f,"MR");
  if(doFit) gFit->Fit(f,"MR");

  //  if (type==1) f2 = new TF1("f2","sqrt(pow([0],2)+pow([1]*x,2)+pow([2]*x*x,2)+pow([3]/x,2))",100,220);
  //  else         f2 = new TF1("f2","sqrt(pow([0],2)+pow([1]*x,2)+pow([2]*x*x,2)+pow([3],2))",100,220);

  if (type==1) f2 = new TF1("f2","sqrt(pow(pol2(0),2)+pow([3]/x,2))",100,350);
  else if (type==5) f2 = new TF1("f2","sqrt(pow(pol2(0),2)+pow((1-([3]/x)),2))",100,350);
  else f2 = new TF1("f2","sqrt(pow(pol2(0),2)+pow([3],2))",100,350);

  f2->SetLineColor(4);
  f2->SetLineWidth(2);
  f2->FixParameter(3,0);
  f2->SetLineColor(1);

  gFitMix->Fit(f2,"MR");

  f2->FixParameter(0,f->GetParameter(0));
  f2->FixParameter(1,f->GetParameter(1));
  f2->FixParameter(2,f->GetParameter(2));
  f2->ReleaseParameter(3);

  if(doFit){
  gFit->Fit(f2,"MR");
  gFit->Fit(f2,"MR");
  gFit->Fit(f2,"MR");
  }
  }

  double tickSize;

  tickSize=0.6;

  if(type == 3) tickSize=0.003;
  if (type==1 || type==2) tickSize=0.003;
  if(type == 5) tickSize=0.003;

  double errScale = 0.03;
  double errRes = 0.01;


  double erroDEar = 0.02;
  TF1* fScaleErrorUp = new TF1("fScaleErrorUp","pol1",120,320);
  TF1* fScaleErrorDown = new TF1("fScaleErrorDown","pol1",120,320);
  fScaleErrorUp->SetParameter(0,0.04921);
  fScaleErrorUp->SetParameter(1,0.000828283);

  fScaleErrorDown->SetParameter(0,0.01983);
  fScaleErrorDown->SetParameter(1,-0.000618589);

  TF1* fScaleError[6];
  //  double scalePar0[] = {1.01609,1.00763,0.97669,1.02068,1.03241,1.00917};
  //  double scalePar1[] = {3.9531e-05,0.000106956,0.000292879,8.6518e-06,-1.24429e-05,-3.51463e-05};

  double scalePar0[] = {1.03,1.03,1.03,1.03,1.03,1.03};
  double scalePar1[] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

  double oldError = 0.082;

  TF1* fBkg[6];

  for(int i = 0; i < 6; ++i){

    fScaleError[i] = new TF1(Form("fScaleError%d",i),"pol1",120,320);
    fScaleError[i]->SetParameter(0,scalePar0[i]-1);
    fScaleError[i]->SetParameter(1,scalePar1[i]);

    fBkg[i] = new TF1(Form("fBkg%d",i),"([4]+[5]*x+[6]*x*x+[7]*x-([0]+[1]*x+[2]*x*x+[3]*x))/x",0,500);
  }

  fBkg[0]->SetParameter(0,-3.62148);
  fBkg[0]->SetParameter(1,0.251335);
  fBkg[0]->SetParameter(2, -0.000507048);
  fBkg[0]->SetParameter(3,0.14988);
  fBkg[0]->SetParameter(4,-36.918);
  fBkg[0]->SetParameter(5,0.409696);
  fBkg[0]->SetParameter(6, -0.00166035);
  fBkg[0]->SetParameter(7,0.409696);

  for(int i = 1; i < 6; ++i){
    fBkg[i]->SetParameter(0,fBkg[0]->GetParameter(0));
    fBkg[i]->SetParameter(1,fBkg[0]->GetParameter(1));
    fBkg[i]->SetParameter(2,fBkg[0]->GetParameter(2));
    fBkg[i]->SetParameter(3,fBkg[0]->GetParameter(3));
  }

  fBkg[1]->SetParameter(4,-12.5155);
  fBkg[1]->SetParameter(5,0.268893);
  fBkg[1]->SetParameter(6,-0.000972584);
  fBkg[1]->SetParameter(7,0.268893);

  fBkg[2]->SetParameter(4, 13.9065);
  fBkg[2]->SetParameter(5, 0.1172);
  fBkg[2]->SetParameter(6,-0.000154972);
  fBkg[2]->SetParameter(7,0.1172);

  fBkg[3]->SetParameter(4,-36.042);
  fBkg[3]->SetParameter(5,0.378475);
  fBkg[3]->SetParameter(6,-0.0014416);
  fBkg[3]->SetParameter(7,0.378475);

  fBkg[4]->SetParameter(4,-15.0462);
  fBkg[4]->SetParameter(5,0.271918);
  fBkg[4]->SetParameter(6,-0.000980983);
  fBkg[4]->SetParameter(7,0.271918);

  fBkg[5]->SetParameter(4,-7.24296);
  fBkg[5]->SetParameter(5,0.221782);
  fBkg[5]->SetParameter(6,-0.000705732);
  fBkg[5]->SetParameter(7,0.221782);

  for(int i = 0; i < g->GetN(); ++i){
    double *x = g->GetX();
    double *y = g->GetY();

    // Systematic uncertainties

    double errEff = 0.015;
    if(type == 5) errEff = 0.01;
    if(i>5) errEff = 0;
	//  errRes = fBkg[cbin]->Eval(x[i]);
   
	  errRes = 0.08;
	  
	  if (cbin == 1) errRes = 0.06;
	  if (cbin == 2) errRes = 0.05;
	  if (cbin == 3) errRes = 0.04;
	  if (cbin == 4) errRes = 0.03;
	  if (cbin == 5) errRes = 0.03;

	  if(type == 5) errRes /= 1.5;
	  
	  double errScaleUp = fabs(fScaleErrorUp->Eval(x[i]));
    double errScaleDown = fabs(fScaleErrorUp->Eval(x[i]));

    errScaleUp = fabs(fScaleError[cbin]->Eval(x[i]));

    if(type != 5) errScaleUp *= 1.5;

    errScaleDown = errScaleUp;
    double finalErrUp = sqrt(errScaleUp*errScaleUp+errRes*errRes+errEff*errEff);
    double finalErrDown = sqrt(errScaleUp*errScaleUp+errRes*errRes+errEff*errEff);

    if((cbin == 0 || cbin == 2) && (i == 0 || i == 8 || i == 9)){
      cout<<"Scale error cbin : "<<cbin<<" pt : "<<x[i]<<"    :     "<<(100.*errScaleUp)<<endl;
      cout<<"Background error cbin : "<<cbin<<" pt : "<<x[i]<<"    :     "<<(100.*errRes)<<endl; 
     cout<<"Final error cbin : "<<cbin<<" pt : "<<x[i]<<"    :     "<<(100.*finalErrUp)<<endl;
    }
    DrawTick(y[i],finalErrUp*y[i],finalErrDown*y[i],x[i],tickSize,4,dataColor);
  }
  g->Draw("p same");
  g->SetMarkerSize(1.25);
  g->SetMarkerColor(dataColor);
  g->SetLineColor(dataColor);

  if(sys == 0 || sys == 99 || sys == 910){
    gPythia->SetMarkerSize(1.7);
    gPythia->SetMarkerColor(4);
    gPythia->SetMarkerStyle(29);
    gPythia->SetLineColor(4);
    gMix->SetMarkerColor(mixColor);
    gMix->SetMarkerStyle(25);
    gMix->SetMarkerSize(1.25);
    gMix->SetLineColor(mixColor);
    
    gMix->SetLineStyle(1);
    
    gPythia->SetName("gPythia");
    gMix->SetName("gMix");
    gMix->Draw("p same");

    if(doFit) fBase->Draw("same");
  }

  g->SetName("g");

  if(sys == 0){
    gMix->Draw("p same");
    gPythia->Draw("p same");
  
    if(doFit){        
      f->Draw("same");
      f2->Draw("same");
    }
    
    TLine* pline = new TLine(m[0],gPythia->GetY()[0],m[nBin],gPythia->GetY()[0]);
    pline->SetLineColor(4);
    pline->SetLineStyle(4);
    //    pline->Draw();
    
    if(drawLeg){
      TLegend *t3=new TLegend(0.1,0.03,0.43,0.32); 
      //    t3->AddEntry(g,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","p");
      t3->AddEntry(g,"Pb+Pb 2011","p");
      if(sys == 0){
	t3->AddEntry(gMix,"PYTHIA+HYDJET 1.8","p");
	
	t3->AddEntry(gPythia,"pp #sqrt{s}=2.76 TeV","p");  
      }
      if(doFit){
	t3->AddEntry(f,"Constant Ratio","l");
	t3->AddEntry(f2,"Constant Difference","l");
      }
      
      t3->SetFillColor(0);
      t3->SetBorderSize(0);
      t3->SetFillStyle(0);
      t3->SetTextFont(63);
      t3->SetTextSize(15);
      t3->Draw();
      
    }
    //  drawText(cstring.Data(),0.76,0.64);
    
  }


  inf->Close();
  infMix->Close();
  infPythia->Close();
  
  if(sys == 99 || sys == 910) return gMix;
  return g;

}


void plotBalanceRatio(int cbin,
		 TString infname,
		 TString pythia,
		 TString mix,
		 bool useWeight,
		 bool drawXLabel,
		 bool drawLeg)
{

  //  useWeight = 0;

  TString cut="pt1>120 && pt2>50 && abs(dphi)>2.5";
  TString cutpp="pt1>120 && pt2>50 && abs(dphi)>2.5";
  TString cstring = "";
  TString side=Form("pt1>%d && pt2>%d && abs(dphi)>%f && abs(dphi)<%f && abs(eta1) < 2 && abs(eta2) < 2",leadCut,subleadCut, sideMin, sideMax);
  TString cutNorm=Form("pt1>%d && abs(eta1) < 2",leadCut);

  double sideScale = sideCorrect*(3.1415926536-2.0944)/(sideMax-sideMin);

  if(cbin==-1) {
    cstring = "0-100%";
    cut+=" && bin>=0 && bin<40";
    side+=" && bin>=0 && bin<40";
    cutNorm+=" && bin>=0 && bin<40";
  }
  else if(cbin==0) {
    cstring = "0-10%";
    cut+=" && bin>=0 && bin<4";
    side+=" && bin>=0 && bin<4";
    cutNorm+=" && bin>=0 && bin<4";

  } else if (cbin==1) {
    cstring = "10-20%";
    cut+=" && bin>=4 && bin<8";
    side+=" && bin>=4 && bin<8";
    cutNorm+=" && bin>=4 && bin<8";

  } else if (cbin==2) {
    cstring = "20-30%";
    cut+=" && bin>=8 && bin<12";
    side+=" && bin>=8 && bin<12";
    cutNorm+=" && bin>=8 && bin<12";

  } else if (cbin==3) {
    cstring = "30-50%";
    cut+=" && bin>=12 && bin<20";
    side+=" && bin>=12 && bin<20";
    cutNorm+=" && bin>=12 && bin<20";

  }else if (cbin==4) {
    cstring = "50-70%";
    cut+=" && bin>=20 && bin<28";
    side+=" && bin>=20 && bin<28";
    cutNorm+=" && bin>=20 && bin<28";

  }else if (cbin==5) {
    cstring = "70-100%";
    cut+=" && bin>=28";
    side+=" && bin>=28";
    cutNorm+=" && bin>=28";

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
  nt->Draw("(pt1-pt2)/(pt1+pt2)>>h",Form("(%s)",cut.Data())); 

  if (useWeight) {
    // use the weight value caluculated by Matt's analysis macro
    ntMix->Draw("(pt1-pt2)/(pt1+pt2)>>hDataMix",Form("(%s)*weight",cut.Data())); 
  } else {
    // ignore centrality reweighting
    ntMix->Draw("(pt1-pt2)/(pt1+pt2)>>hDataMix",Form("(%s)",cut.Data()));  
  }
  ntPythia->Draw("(pt1-pt2)/(pt1+pt2)>>hPythia",Form("(%s)",cutpp.Data()));

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
  hPythia->GetXaxis()->SetTitleSize(26);
  hPythia->GetXaxis()->SetTitleFont(43);
  hPythia->GetXaxis()->SetTitleOffset(1.7);
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
    //    t3->AddEntry(h,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","p");
    //    t3->AddEntry(hPythia,"PYTHIA","lf");  
    //    t3->AddEntry(hDataMix,"embedded PYTHIA","lf");

    t3->AddEntry(h,"2011","p");
    t3->AddEntry(hPythia,"2010","lf");
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


void plotDeltaEAllCent(){

  TH1::SetDefaultSumw2();

  if(0){
  plotScaleSys(0);
  plotScaleSys(1);
  plotScaleSys(2);
  plotScaleSys(3);
  plotScaleSys(4);
  plotScaleSys(5);
  }

  //  plotDeltaEAllCentSys(13);
  //  plotDeltaEAllCentSys(2);
  //  plotDeltaEAllCentSys(4);
  
  plotDeltaEAllCentAna();

}
