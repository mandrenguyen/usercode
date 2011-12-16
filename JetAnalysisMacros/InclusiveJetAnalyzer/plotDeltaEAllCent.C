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

static const bool doFit = 1;
static const int type = 1;


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

TGraphAsymmErrors *calcEff(TH1* h1, TH1* h2, int shift = 0, double* bincenters = 0)
{
   h1->Sumw2();
   h2->Sumw2();
   h2->Divide(h1);
   TGraphAsymmErrors *gEfficiency = new TGraphAsymmErrors(h2);

   double x,y; 
   if(1){
     for(int i = 0; i < gEfficiency->GetN(); ++i){
       gEfficiency->GetPoint(i,x,y);
       x = bincenters[i];
       gEfficiency->SetPoint(i,x,y);
     }

   }
  
   return gEfficiency;
}

void plotDeltaEAllCent(){

  weightMix();
  TCanvas *c1 = new TCanvas("c1","",1250,530);

  makeMultiPanelCanvas(c1,3,1,0.0,0.0,0.2,0.15,0.02);

  c1->cd(1);
  plotDE(2,"data.root","data_pp.root","mix.root",true,false,false);
  drawText("30-100%",0.76,0.24);
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
  plotDE(1,"data.root","data_pp.root","mix.root",true,true,false);
  drawText("10-30%",0.75,0.24);
  drawPatch(-0.0007,0.0972,0.0518,0.141);
  drawPatch(0.94,0.0972,1.1,0.141);
  drawText("(b)",0.02,0.9);

  TLatex tsel;
  tsel.SetNDC();
  tsel.SetTextFont(63);
  tsel.SetTextSize(15);
  tsel.DrawLatex(0.25,0.9,Form("p_{T,1} > %d GeV/c",leadCut));
  tsel.DrawLatex(0.25,0.825,Form("p_{T,2} > %d GeV/c",subleadCut));
  tsel.DrawLatex(0.25,0.75,"#Delta#phi_{12} > #frac{2}{3}#pi");

  c1->cd(3);
  plotDE(0,"data.root","data_pp.root","mix.root",true,false,true);
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
  c1->Print(Form("./fig/deltaPtOverPt%d_all_cent_20111124%s_lead%d_sub%d_v2.gif",type,doFit ? "_fit" : "",leadCut,subleadCut));
  c1->Print(Form("./fig/deltaPtOverPt%d_all_cent_20111124%s_lead%d_sub%d_v2.eps",type,doFit ? "_fit" : "",leadCut,subleadCut));
  c1->Print(Form("./fig/deltaPtOverPt%d_all_cent_20111124%s_lead%d_sub%d_v2.pdf",type,doFit ? "_fit" : "",leadCut,subleadCut));



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
  TString cut="pt1>120&&pt2>30&&abs(dphi)>3.14159265359*2/3&& abs(eta1) < 2 && abs(eta2) < 2";
  TString cutpp="pt1>120&&pt2>30&&abs(dphi)>3.14159265359*2/3&& abs(eta1) < 2 && abs(eta2) < 2";
  TString trigcut = "";
  TString cstring = "";
  TString divide = "";
  //  useWeight = 0;
  
  if (type==0) divide = "/1";
  if (type==1) divide = "/pt1";
  if (type==2) divide = "/pt2";
  if (type==3) divide = "/(pt1+pt2)";
  

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

  /*
  ntMix->SetAlias("pt1","et1");
  ntMix->SetAlias("pt2","et2");
  ntPythia->SetAlias("pt1","et1");
  ntPythia->SetAlias("pt2","et2");
  */

  nt->SetAlias("adphi","acos(cos(phi1-phi2))");
  ntMix->SetAlias("adphi","abs(dphi)");
  ntPythia->SetAlias("adphi","abs(dphi)");
  ntMix->SetAlias("weight",weightString.Data());

  const int nBin = 6;
  //  double m[nBin+1] = {100,110,120,130,140,170,240};
  //  double m[nBin+1] = {120,130,140,170,240};
  double m[nBin+1] = {120,140,160,180,200,230,500};

  double cm0[nBin+1] = {0,0,0,0,0,0,0};
  double cm1[nBin+1] = {0,0,0,0,0,0,0};
  double cm2[nBin+1] = {0,0,0,0,0,0,0};
  double cme0[nBin+1] = {0,0,0,0,0,0,0};
  double cme1[nBin+1] = {0,0,0,0,0,0,0};
  double cme2[nBin+1] = {0,0,0,0,0,0,0};

  TH1D* hDum = new TH1D("hDum","",1000,0,1000);
  for(int i = 0; i < nBin; ++i){
    nt->Draw("pt1>>hDum",Form("%s && pt1 >= %f && pt1 < %f",cut.Data(),m[i],m[i+1]));
    cm0[i] = hDum->GetMean();
    cme0[i] = hDum->GetMeanError();
    ntMix->Draw("pt1>>hDum",Form("%s*(%s && pt1 >= %f && pt1 < %f && bin < %d)",cut.Data(),weightString.Data(),m[i],m[i+1]));
    cm1[i] = hDum->GetMean();
    cme1[i] = hDum->GetMeanError();
    ntPythia->Draw("pt1>>hDum",Form("abs(eta1) < 2 && abs(eta2) < 2 && abs(dphi) > 2.0944&& pt1 >= %f && pt1 < %f",m[i],m[i+1]));
    cm2[i] = hDum->GetMean();
    cme2[i] = hDum->GetMeanError();
  }
  
  TH1D *hTmp = new TH1D("hTmp","",100,m[0],330);
  TH1D *h = new TH1D("h","",nBin,m);
  TH1D *h2 = new TH1D("h2","",nBin,m);

  h->Reset();
  h2->Reset();
  
  nt->Draw("pt1>>h",Form("((pt1-pt2)%s)*(%s)",divide.Data(),cut.Data()));
  nt->Draw("pt1>>h2",Form("%s",cut.Data()));
  TGraphAsymmErrors *g = calcEff(h2,h,0,cm0);
  h->Reset();
  h2->Reset();

  ntPythia->Draw("pt1>>h",Form("((pt1-pt2)%s)*(%s)",divide.Data(),cutpp.Data()));
  ntPythia->Draw("pt1>>h2",Form("%s",cutpp.Data()));
  TGraphAsymmErrors *gPythia = calcEff(h2,h,0,cm2);
  h->Reset();
  h2->Reset();

  if(useWeight){  
    ntMix->Draw("pt1>>h",Form("(((pt1-pt2)%s)*(%s))*weight",divide.Data(),cut.Data()));
    ntMix->Draw("pt1>>h2",Form("(%s)*weight",cut.Data()));

    //    ntMix->Draw("pt1>>h",Form("((pt1-pt2)%s)*(%s*%s)",divide.Data(),cut.Data(),weightString.Data()));
    //    ntMix->Draw("pt1>>h2",Form("(%s)*%s",cut.Data(),weightString.Data()));
    //    cout<<weightString.Data()<<endl;

  }else{
    ntMix->Draw("pt1>>h",Form("((pt1-pt2)%s)*(%s)",divide.Data(),cut.Data()));
    ntMix->Draw("pt1>>h2",Form("%s",cut.Data()));
  }
  TGraphAsymmErrors *gMix = calcEff(h2,h,0,cm1);

  hTmp->SetMaximum(0.6);
  //  if(type == 1) hTmp->SetMaximum(0.5);
  if(type == 3) hTmp->SetMaximum(1.2);
  hTmp->SetMinimum(0.1);

  if (type==0){ 
    hTmp->SetMaximum(150);
    hTmp->SetMinimum(0.);
  }

  if (type==3){ 
    hTmp->SetMaximum(0.5);
    hTmp->SetMinimum(0);
  }

  if (drawXLabel) {
     hTmp->SetXTitle("Leading Jet p_{T} (GeV/c)");
     hTmp->GetXaxis()->CenterTitle();
  } else {
     hTmp->SetXTitle("");
     hTmp->GetXaxis()->CenterTitle();
  }

  hTmp->SetYTitle(Form("<(p_{T,1}-p_{T,2})/p_{T,1}>"));
  if (type==0) hTmp->SetYTitle(Form("<(p_{T}^{1}-p_{T}^{2})> (GeV/c)"));
  if (type==3) hTmp->SetYTitle(Form("<A_{J}>"));

  hTmp->GetYaxis()->CenterTitle();
  hTmp->GetYaxis()->SetTitleOffset(1.5);
  hTmp->GetXaxis()->SetLabelSize(22);
  hTmp->GetXaxis()->SetLabelFont(43);

  hTmp->Draw();

  TGraphAsymmErrors *gFitPythia = (TGraphAsymmErrors*) gPythia->Clone();  
  TGraphAsymmErrors *gFit = (TGraphAsymmErrors*) g->Clone();  

  TF1 *f;
  if (type==1) f = new TF1("f","[0]+[1]*x+[2]*x*x+[3]",100,250);
  else         f = new TF1("f","[0]+[1]*x+[2]*x*x+[3]*x",100,250);
  f->SetLineColor(2);
  f->SetLineStyle(2);
  f->SetLineWidth(1);
  f->FixParameter(3,0);
  f->SetLineColor(1);

  //  gFitPythia->Fit(f,"MR");

  if(type == 1){
  f->SetParameter(0,0.120068);
  f->SetParameter(1,0.00187116);
  f->SetParameter(2,-5.50113e-06);
  }else{
    f->SetParameter(0,-40.0787);
    f->SetParameter(1,0.786485);
    f->SetParameter(2,-0.00162406);
  }

  f->FixParameter(0,f->GetParameter(0));
  f->FixParameter(1,f->GetParameter(1));
  f->FixParameter(2,f->GetParameter(2));
  f->ReleaseParameter(3);

  if(doFit) gFit->Fit(f,"MR");
  if(doFit) gFit->Fit(f,"MR");
  if(doFit) gFit->Fit(f,"MR");

  TF1 *f2;
  if (type==1) f2 = new TF1("f2","[0]+[1]*x+[2]*x*x+[3]/x",100,250);
  else         f2 = new TF1("f2","[0]+[1]*x+[2]*x*x+[3]",100,250);
  f2->SetLineColor(4);
  f2->SetLineWidth(1);
  f2->FixParameter(3,0);
  f2->SetLineColor(1);

  //  gFitPythia->Fit(f,"MR");
  if(type == 1){
    f->SetParameter(0,0.120068);
    f->SetParameter(1,0.00187116);
    f->SetParameter(2,-5.50113e-06);
  }else{
    f->SetParameter(0,-40.0787);
    f->SetParameter(1,0.786485);
    f->SetParameter(2,-0.00162406);
  }

  
  f2->FixParameter(0,f->GetParameter(0));
  f2->FixParameter(1,f->GetParameter(1));
  f2->FixParameter(2,f->GetParameter(2));
  f2->ReleaseParameter(3);

  if(doFit){
  gFit->Fit(f2,"MR");
  gFit->Fit(f2,"MR");
  gFit->Fit(f2,"MR");
  }

  double tickSize;

  tickSize=0.6;

  if(type == 3) tickSize=0.003;
  if (type==1 || type==2) tickSize=0.012;

  
  double erroDEar = 0.02;
  for(int i = 0; i < g->GetN(); ++i){
    double *x = g->GetX();
    double *y = g->GetY();
    DrawTick(y[i],0.082*y[i],0.082*y[i],x[i],tickSize,4,dataColor);
  }
  g->Draw("p same");
  g->SetMarkerSize(1.25);
  g->SetMarkerColor(dataColor);
  g->SetLineColor(dataColor);

  gPythia->SetMarkerSize(1.7);
  gPythia->SetMarkerColor(4);
  gPythia->SetMarkerStyle(29);
  gPythia->SetLineColor(4);
  gMix->SetMarkerColor(mixColor);
  gMix->SetMarkerStyle(25);
  gMix->SetMarkerSize(1.25);
  gMix->SetLineColor(mixColor);

  gMix->SetLineStyle(1);


  g->SetName("g");
  gPythia->SetName("gPythia");
  gMix->SetName("gMix");
  gMix->Draw("p same");
  gPythia->Draw("p same");
  if(doFit){
  f->Draw("same");
  f2->Draw("same");
  }

  TLine* pline = new TLine(m[0],gPythia->GetY()[0],m[nBin],gPythia->GetY()[0]);
  pline->SetLineColor(4);
  pline->SetLineStyle(4);
  pline->Draw();

  if(drawLeg){
    TLegend *t3=new TLegend(0.13,0.76,0.53,0.93); 
    //    t3->AddEntry(g,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","p");
    if(0){
        t3->AddEntry(g,"2011","p");
        t3->AddEntry(gMix,"2010","p");
    }else{
      t3->AddEntry(g,"Pb+Pb 2011","p");
      t3->AddEntry(gMix,"PYTHIA+HYDJET 1.6","p");
    }

    t3->AddEntry(gPythia,"pp #sqrt{s}=2.76 TeV","p");  

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
  nt->Draw("(pt1-pt2)/(pt1+pt2)>>h",Form("(%s)",cut.Data())); 

  useWeight = 0;
  
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
