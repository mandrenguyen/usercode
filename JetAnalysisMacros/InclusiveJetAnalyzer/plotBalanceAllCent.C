#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
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

#endif


//---------------------------------------------------------------------
void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns, 
			  const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0., 
			  const Float_t leftMargin=0.2, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);

void plotBalance(int cbin = 0,
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

void plotBalanceAllCent(){

   TCanvas *c1 = new TCanvas("c1","",1050,700);
   makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.2,0.2,0.02);

 
  c1->cd(1);
  plotBalance(-1,"data.root","pythia.root","mix.root",true,false,false);
  drawText("(a)",0.22,0.9);
  drawText(" 0-100%",0.70,0.1);
  drawPatch(0.05,0.0,0.2,0.05);

  c1->cd(2);
  plotBalance(4,"data.root","pythia.root","mix.root",true,false,false);
  drawText("50-100%",0.7,0.1);
  drawText("(b)",0.02,0.9);

  c1->cd(3);
  plotBalance(3,"data.root","pythia.root","mix.root",true,false,true);
  drawText("30-50%",0.7,0.1);
  drawText("(c)",0.02,0.9);

  TLatex *cms = new TLatex(0.17,0.2125,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();                                                                                                                                        


  TLatex *lumi = new TLatex(0.66,0.2125,"#intL dt = 6.7 #mub^{-1}");

  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw(); 




  c1->cd(4);
  plotBalance(2,"data.root","pythia.root","mix.root",true,false,false);
  drawText("20-30%",0.7,0.3);
  drawText("(d)",0.22,0.93);
  drawPatch(0.05,0.972,0.2,1.1);
  drawPatch(0.9,0.0972,1.12,0.191);

  //  gPad->SetBottomMargin(0.22);                                                                                                                     

  c1->cd(5);
  plotBalance(1,"data.root","pythia.root","mix.root",true,true,false);
  drawText("10-20%",0.7,0.3);
  drawText("(e)",0.02,0.93);
  drawPatch(-0.00007,0.0972,0.128,0.195);
  drawPatch(0.9,0.0972,1.12,0.191);
  drawPatch(0.976,0.0972,1.1,0.141);
  //  gPad->SetBottomMargin(0.22);                                                                                                                     

  c1->cd(6);
  plotBalance(0,"data.root","pythia.root","mix.root",true,false,false);
  drawText("0-10%",0.7,0.3);
  drawText("(f)",0.02,0.93);
  drawPatch(-0.00007,0.0972,0.0518,0.141);
  drawPatch(-0.00007,0.0972,0.128,0.195);
        

  c1->Print("./fig/dijet_imbalance_all_cent_20101126_v0.gif");
  c1->Print("./fig/dijet_imbalance_all_cent_20101126_v0.eps");
  c1->Print("./fig/dijet_imbalance_all_cent_20101126_v0.pdf");

}

void plotBalance(int cbin,
		 TString infname,
		 TString pythia,
		 TString mix,
		 bool useWeight,
		 bool drawXLabel,
		 bool drawLeg)
{

  TString cut="et1>120 && et2>50 && dphi>2.0944";
  TString cutpp="et1>120 && et2>50 && dphi>2.0944";

  TString cstring = "";
  if(cbin==-1) {
     cstring = "0-100%";
     cut+=" && bin>=0 && bin<40";
  }
  else if(cbin==0) {
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

  hPythia->Scale(1./hPythia->Integral(0,20));
  hPythia->SetLineColor(kBlue);
  hPythia->SetFillColor(kAzure-8);
  hPythia->SetFillStyle(3005);

  hDataMix->Scale(1./hDataMix->Integral(0,20));
  hDataMix->SetLineColor(kRed);
  hDataMix->SetFillColor(kRed-9);
  hDataMix->SetFillStyle(3004);


  hDataMix->SetStats(0);

  hDataMix->GetXaxis()->SetLabelSize(22);
  hDataMix->GetXaxis()->SetLabelFont(43);
  hDataMix->GetXaxis()->SetTitleSize(24);
  hDataMix->GetXaxis()->SetTitleFont(43);
  hDataMix->GetXaxis()->SetTitleOffset(1.4);
  hDataMix->GetXaxis()->CenterTitle();
  
  
  hDataMix->GetYaxis()->SetLabelSize(22);
  hDataMix->GetYaxis()->SetLabelFont(43);
  hDataMix->GetYaxis()->SetTitleSize(22);
  hDataMix->GetYaxis()->SetTitleFont(43);
  hDataMix->GetYaxis()->SetTitleOffset(2.4);
  hDataMix->GetYaxis()->CenterTitle();
  
  if(drawXLabel) hDataMix->SetXTitle("A_{J} = (p_{T}^{j1}-p_{T}^{j2})/(p_{T}^{j1}+p_{T}^{j2})");  
  hDataMix->SetYTitle("Event Fraction");
  hDataMix->SetAxisRange(0,0.24,"Y");
  hDataMix->GetXaxis()->SetNdivisions(905,true);
  hDataMix->GetYaxis()->SetNdivisions(505,true);
  
  hDataMix->SetTitleOffset(2,"X");
  hDataMix->SetTitleOffset(3,"Y");

  hDataMix->Draw("hist");
  //hPythia->Draw("hist");  
  h->Draw("same");

  cout<<" mean value of data "<<h->GetMean()<<endl;

  if(drawLeg){
    TLegend *t3=new TLegend(0.35,0.635,0.89,0.84); 
    t3->AddEntry(h,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","pl");
    //t3->AddEntry(hPythia,"PYTHIA","lf");  
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

  if(drawXLabel) hdum->SetXTitle("A_{J} #equiv (E_{T}^{j1}-E_{T}^{j2})/(E_{T}^{j1}+E_{T}^{j2})");
  /*
  hdum->GetXaxis()->SetLabelSize(20);
  hdum->GetXaxis()->SetLabelFont(43);
  hdum->GetXaxis()->SetTitleSize(22);
  hdum->GetXaxis()->SetTitleFont(43);
  hdum->GetXaxis()->SetTitleOffset(1.5);
  hdum->GetXaxis()->CenterTitle();
  */
  hdum->GetXaxis()->SetNdivisions(905,true);

  hdum->SetYTitle("Event Fraction");
  /*
  hdum->GetYaxis()->SetLabelSize(20);
  hdum->GetYaxis()->SetLabelFont(43);
  hdum->GetYaxis()->SetTitleSize(20);
  hdum->GetYaxis()->SetTitleFont(43);
  hdum->GetYaxis()->SetTitleOffset(2.5);
  hdum->GetYaxis()->CenterTitle();
  */
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
