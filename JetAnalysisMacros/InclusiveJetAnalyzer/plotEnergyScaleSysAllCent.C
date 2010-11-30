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

#endif


//---------------------------------------------------------------------
void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns, 
			  const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0., 
			  const Float_t leftMargin=0.2, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);

void plotEnergyScaleSys(int cbin = 0,
		 TString infname = "data.root",
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

void plotEnergyScaleSysAllCent(){

  TCanvas *c1 = new TCanvas("c1","",1250,530);

  makeMultiPanelCanvas(c1,3,1,0.0,0.0,0.2,0.15,0.02);

  c1->cd(1);
  plotEnergyScaleSys(2,"data.root",true,false,false);
  drawText("30-100%",0.75,0.34);
  drawPatch(0.976,0.0972,1.1,0.141);

  c1->cd(2);
  plotEnergyScaleSys(1,"data.root",true,true,false);
  drawText("10-30%",0.75,0.34);
  drawPatch(-0.00007,0.0972,0.0518,0.141);
  drawPatch(0.976,0.0972,1.1,0.141);

  c1->cd(3);
  plotEnergyScaleSys(0,"data.root",true,false,true);
  drawText("0-10%",0.75,0.34);
  drawPatch(-0.00007,0.0972,0.0518,0.141);

  TLatex *cms = new TLatex(0.30,0.18,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();                                                                                                                                        

  TLatex *lumi = new TLatex(0.68,0.18,"#intL dt = 3 #mub^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw(); 

  c1->Print("./fig/EnergyScaleSystematics_all_cent_20101127_v2.gif");
  c1->Print("./fig/EnergyScaleSystematics_all_cent_20101127_v2.eps");
  c1->Print("./fig/EnergyScaleSystematics_all_cent_20101127_v2.pdf");

}

void plotEnergyScaleSys(int cbin,
		 TString infname,
		 bool useWeight,
		 bool drawXLabel,
		 bool drawLeg)
{
  TString cut="et1>120 && et2>35 && dphi>2.5";
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

  // projection histogram
  TH1D *h = new TH1D("h","",20,0,1);
  TH1D *hSys1 = new TH1D("hSys1","",20,0,1);
  TH1D *hSys2 = new TH1D("hSys2","",20,0,1);
  nt->Draw("(et1-et2)/(et1+et2)>>h",Form("(%s)",cut.Data())); 
   
  nt->Draw("(et1-et2)/(et1+et2)>>h",Form("(%s)",cut.Data())); 
  nt->Draw("(et1*(1-unc1*0)-et2*(1-unc2*1))/(et1*(1-unc1*0)+et2*(1-unc2*1))>>hSys1",Form("(%s)",cut.Data())); 
  nt->Draw("(et1*(1-unc1*(et1-120)/(35-120))-et2*(1-unc2*(et2-120)/(35-120)))/(et1*(1-unc1*(et1-120)/(35-120))+et2*(1-unc2*(et2-120)/(35-120)))>>hSys2",Form("(%s)",cut.Data())); 

  // calculate the statistical error and normalize
  h->Sumw2();
  h->Scale(1./h->GetEntries());
  h->SetMarkerStyle(20);

  hSys1->Scale(1./hSys1->Integral(0,20));
  hSys1->SetLineColor(kBlue);
  hSys1->SetFillColor(kAzure-8);
  hSys1->SetFillStyle(0);

  hSys1->SetStats(0);

  hSys1->SetYTitle("1/N_{leading jet} dN/dA_{J}");
  hSys1->Draw("hist");


  hSys1->GetXaxis()->SetLabelSize(20);
  hSys1->GetXaxis()->SetLabelFont(43);
  hSys1->GetXaxis()->SetTitleSize(22);
  hSys1->GetXaxis()->SetTitleFont(43);
  hSys1->GetXaxis()->SetTitleOffset(1.5);
  hSys1->GetXaxis()->CenterTitle();

  hSys1->GetXaxis()->SetNdivisions(905,true);
  
  if(drawXLabel)hSys1->SetXTitle("A_{J} #equiv (E_{T}^{j1}-E_{T}^{j2})/(E_{T}^{j1}+E_{T}^{j2})");

  hSys1->GetYaxis()->SetLabelSize(20);
  hSys1->GetYaxis()->SetLabelFont(43);
  hSys1->GetYaxis()->SetTitleSize(20);
  hSys1->GetYaxis()->SetTitleFont(43);
  hSys1->GetYaxis()->SetTitleOffset(2.5);
  hSys1->GetYaxis()->CenterTitle();
  

  hSys1->SetAxisRange(0,0.2,"Y");


  hSys2->Scale(1./hSys2->Integral(0,20));
  hSys2->SetLineColor(kRed);
  hSys2->SetFillColor(kRed-9);
  hSys2->SetFillStyle(0);
  hSys2->Draw("same");
  
  h->Draw("same");

  if(drawLeg){
    TLegend *t3=new TLegend(0.26,0.63,0.80,0.88); 
    t3->AddEntry(h,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","pl");
    t3->AddEntry(hSys1,"Data 30-100%, 2^{nd} Jet E_{T} scaled","l");
    t3->AddEntry(hSys2,"Data 30-100%, tile JEC","l");
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

  if(drawXLabel)   hdum->SetXTitle("A_{J} #equiv (E_{T}^{j1}-E_{T}^{j2})/(E_{T}^{j1}+E_{T}^{j2})");
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
