#if !defined(__CINT__) || defined(__MAKECINT__)

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
#include "TLine.h"

#include "GraphErrorsBand.h"

#endif


//---------------------------------------------------------------------
void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns, 
			  const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0., 
			  const Float_t leftMargin=0.2, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);

void plotRatio(int cbin = 0,
		 TString infname = "data-smeared.root",
		 TString DataPF = "data.root",
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


TH1D *getErrorBand(TH1* h);

void plotEnergySmearingSystematics(){

   TCanvas *c1 = new TCanvas("c1","",1650,430);

   makeMultiPanelCanvas(c1,5,1,0.0,0.0,0.2,0.15,0.02);


  c1->cd(1);
  plotRatio(4,"data-smeared.root","data.root","data.root",false,false,false);
  drawText("50~100%",0.72,0.24);
  drawPatch(0.976,0.0972,1.1,0.171);
  gPad->SetLeftMargin(0.25);
  gPad->SetBottomMargin(0.18);

  c1->cd(2);
  plotRatio(3,"data-smeared.root","data.root","data.root",false,false,false);
  drawText("30~50%",0.7,0.24);
  drawPatch(-0.00007,0.0972,0.0518,0.171);
  drawPatch(0.976,0.0972,1.1,0.171);
  gPad->SetBottomMargin(0.18);

  c1->cd(3);
  plotRatio(2,"data-smeared.root","data.root","data.root",false,true,false);
  drawText("20~30%",0.7,0.24);
  drawPatch(-0.00007,0.0972,0.0518,0.171);
  drawPatch(0.976,0.0972,1.1,0.171);
  gPad->SetBottomMargin(0.18);

  c1->cd(4);
  plotRatio(1,"data-smeared.root","data.root","data.root",false,false,false);
  drawText("10~20%",0.7,0.24);
  drawPatch(-0.00007,0.0972,0.0518,0.171);
  drawPatch(0.976,0.0972,1.1,0.171);
  gPad->SetBottomMargin(0.18);

  c1->cd(5);
  plotRatio(0,"data-smeared.root","data.root","data.root",false,false,true);
  drawText("0~10%",0.73,0.24);
  drawPatch(-0.00007,0.0972,0.0518,0.171);
  gPad->SetBottomMargin(0.18);

  TLatex *cms = new TLatex(0.086,4.5,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();                                                                                                                                        

  TLatex *lumi = new TLatex(0.15,4.1,"#intL dt = 7 #mub^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw(); 

  c1->Print("./fig/ratio_sys_resolution_20101201_v1.gif");
  c1->Print("./fig/ratio_sys_resolution_20101201_v1.eps");
  c1->Print("./fig/ratio_sys_resolution_20101201_v1.pdf");

}

void plotRatio(int cbin,
		 TString infname,
		 TString DataPF,
		 TString mix,
		 bool useWeight,
		 bool drawXLabel,
		 bool drawLeg)
{
  TString cut="et1>120&& et1<2000 && et2>50 && dphi>2.5&&(et1-et2)/(et1+et2)<10 ";
  TString cstring = "";
  if(cbin==0) {
     cstring = "0-10%";
     cut+=" && bin>=0 && bin<4";
  } else if (cbin==1) {
     cstring = "10-20%";
     cut+=" && bin>=4 && bin<8";
  } else if (cbin==2) {
     cstring = "20-30%";
     cut+=" && bin>=8 && bin<12";
  } else if (cbin==3) {
     cstring = "30-50%";
     cut+=" && bin>=12 && bin<20";
  }
  else {
     cstring = "50-100%";
     cut+=" && bin>=20";
  }


  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");

  // open the DataPF (MC) file
  TFile *infDataPF = new TFile(DataPF.Data());
  TTree *ntDataPF = (TTree*) infDataPF->FindObjectAny("nt");

  // open the datamix file
  TFile *infMix = new TFile(mix.Data());
  TTree *ntMix =(TTree*)infMix->FindObjectAny("nt");

  // Variable Aj
  char *aj = "(et1-et2)/(et1+et2)";

  const int nBin = 14;
  double bins[nBin+1]={0,0.05,0.1,0.15,0.2,0.25,0.3,0.35,0.4,0.45,0.5,0.55,0.6,0.8,1};
  // projection histogram
  TH1D *h = new TH1D("h","",nBin,bins);
  TH1D *hTmp = new TH1D("hTmp","",nBin,bins);
  TH1D *hEmbedded = new TH1D("hEmbedded","",nBin,bins);
  TH1D *hDataMix = new TH1D("hDataMix","",nBin,bins);
  nt->Draw(Form("%s>>h",aj),Form("(%s)",cut.Data())); 
   
  if (useWeight) {
    // use the weight value caluculated by Matt's analysis macro
    ntDataPF->Draw(Form("%s>>hEmbedded",aj),Form("(%s)",cut.Data())); 
    ntMix->Draw(Form("%s>>hDataMix",aj),Form("(%s)*weight",cut.Data())); 
  } else {
    // ignore centrality reweighting
    ntDataPF->Draw(Form("%s>>hEmbedded",aj),Form("(%s)",cut.Data()));
    ntMix->Draw(Form("%s>>hDataMix",aj),Form("(%s)",cut.Data()));  
  }

  // calculate the statistical error and normalize
  h->Sumw2();
  h->Scale(1./h->GetEntries());
  h->SetMarkerStyle(20);

  hEmbedded->Sumw2();
  hEmbedded->Scale(1./hEmbedded->Integral(0,20));
  hEmbedded->SetLineColor(kBlue);
  hEmbedded->SetFillColor(kAzure-8);
  hEmbedded->SetFillStyle(3005);

  hEmbedded->SetStats(0);

  if(drawXLabel) {
     hEmbedded->SetXTitle("A_{J} = (E_{T}^{j1}-E_{T}^{j2})/(E_{T}^{j1}+E_{T}^{j2})");
     h->SetXTitle("A_{J} = (E_{T}^{j1}-E_{T}^{j2})/(E_{T}^{j1}+E_{T}^{j2})");
  }
  h->GetXaxis()->SetLabelSize(20);
  h->GetXaxis()->SetLabelFont(43);
  h->GetXaxis()->SetTitleSize(22);
  h->GetXaxis()->SetTitleFont(43);
  h->GetXaxis()->SetTitleOffset(1.5);
  h->GetXaxis()->CenterTitle();
  h->GetXaxis()->SetNdivisions(905,true);

  h->GetYaxis()->SetLabelSize(20);
  h->GetYaxis()->SetLabelFont(43);
  h->GetYaxis()->SetTitleSize(20);
  h->GetYaxis()->SetTitleFont(43);
  h->GetYaxis()->SetTitleOffset(2.5);
  h->GetYaxis()->CenterTitle();
  

  h->SetAxisRange(0,0.3,"Y");
  h->SetMarkerColor(2);
  
  h->SetYTitle("Ratio");
  hEmbedded->SetYTitle("Ratio");



  hDataMix->Sumw2();
  hDataMix->Scale(1./hDataMix->Integral(0,20));
  hDataMix->SetLineColor(kRed);
  hDataMix->SetFillColor(kRed-9);
  hDataMix->SetFillStyle(3004);
  
  h->SetAxisRange(0,5,"Y");
  hEmbedded->SetAxisRange(0,10,"Y");
  h->Divide(hDataMix);
  hEmbedded->Divide(hDataMix);

  h->Draw("");

  TH1D *hErr = getErrorBand(h);


  double systematicErrorResolution[nBin+1] =
                  {0.25,0.05,0.02,0.02,0.02,0.02,0.03,0.05,0.07,0.11,0.15,0.25};

  double systematicError[nBin];

  for (int b=0;b<nBin;b++)
  {
     double sum=0;
     sum+= systematicErrorResolution[b]*systematicErrorResolution[b];
     systematicError[b]=sqrt(sum);
     hTmp->SetBinContent(b,1);
  }


  TGraph *gErrorBand;

  if (cbin!=0) {
     gErrorBand = GetErrorBand(hTmp,systematicError,systematicError,0.025,12);
  } else {
     gErrorBand = GetErrorBand(hTmp,systematicError,systematicError,0.025,12);
  }
  gErrorBand->Draw("f");
  hEmbedded->SetMarkerStyle(4);
  hEmbedded->SetMarkerColor(4);
  hEmbedded->SetLineColor(4);
  hEmbedded->SetLineStyle(2);
  
  //hEmbedded->Draw("same");
  //h->Fit("pol0","","",0,1);
  h->Draw("same");
  TLine *l = new TLine(0,1,1,1);
  l->SetLineStyle(2);
  l->Draw();

  if(drawLeg){
    TLegend *t3=new TLegend(0.07,0.67,0.49,0.84); 
    t3->AddEntry(h,"Data smeared / Data","pl");
    //t3->AddEntry(hEmbedded,"Data PF / PYQUEN + Data","l");  
    //t3->AddEntry(hDataMix,"unquenched PYQUEN + Data","lf");
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

  hdum->SetYTitle("Ratio");

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

TH1D *getErrorBand(TH1* h)
{
   TH1D *hErr = (TH1D*) h->Clone();
   hErr->SetName("hErr");
   for (int i=0;i<h->GetNbinsX();i++) {
      double var = h->GetBinContent(i);
      double varErr = h->GetBinError(i);
      hErr->SetBinContent(i,var);
      hErr->SetBinError(i,var*0.1);
   }
   return hErr;
}
