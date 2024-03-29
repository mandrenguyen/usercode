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
			  const Float_t leftMargin=0.35, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);

void plotEnergyScaleSys(int cbin = 0,
		 TString infname = "data.root",
		 bool useWeight = true,
		 bool drawXLabel = false,
		 bool drawLeg = false);

void drawText(const char *text, float xp, float yp);


void plotEnergyScaleSysAllCent(){

  TCanvas *c1 = new TCanvas("c1","",1050,700);

  makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.15,0.15,0.02);   

  c1->cd(1);
  plotEnergyScaleSys(5,"data.root",true,false,false);
  drawText("70-100%",0.76,0.24);

  c1->cd(2);
  plotEnergyScaleSys(4,"data.root",true,false,false);
  drawText("50-70%",0.75,0.24);

  c1->cd(3);
  plotEnergyScaleSys(3,"data.root",true,true,true);
  drawText("30-50%",0.75,0.24);

  c1->cd(4);
  plotEnergyScaleSys(2,"data.root",true,false,false);
  drawText("20-30%",0.75,0.24);

  c1->cd(5);
  plotEnergyScaleSys(1,"data.root",true,false,false);
  drawText("10-20%",0.75,0.24);

  c1->cd(6);
  plotEnergyScaleSys(0,"data.root",true,false,false);
  drawText("0-10%",0.75,0.24);

  TLatex *cms = new TLatex(0.20,0.18,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(22);
  cms->Draw();                                                                                                                                        

  TLatex *lumi = new TLatex(0.68,0.18,"#intL dt = 84 #mub^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw(); 

  c1->Print("./fig/EnergyScaleSystematics_all_cent_2011_v0.gif");
  c1->Print("./fig/EnergyScaleSystematics_all_cent_2011_v0.eps");
  c1->Print("./fig/EnergyScaleSystematics_all_cent_2011_v0.pdf");

}

void plotEnergyScaleSys(int cbin,
			TString infname,
			bool useWeight,
			bool drawXLabel,
			bool drawLeg)
{
  TString cut="pt1>120&& pt1<2000 && pt2>30 && dphi>3.1415926/3*2&&(pt1-pt2)/(pt1+pt2)<10 ";
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
  } else if (cbin==4) {
    cstring = "50-70%";
    cut+=" && bin>=20 && bin<28";
  }
  else {
    cstring = "70-100%";
    cut+=" && bin>=28";
  }
  
  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");
  
  nt->AddFriend("rand = ntRandom","random.root");
  
  // projection histogram
  TH1D *h,*hSys1,*hSys2;


  if(cbin==0){
    h = new TH1D("h","",20,0.0001,1.0);
    hSys1 = new TH1D("hSys1","",20,0.0001,1.0);
    hSys2 = new TH1D("hSys2","",20,0.0001,1.0);
  }
  else if(cbin==2){
    h = new TH1D("h","",20,0.0,0.9999);
    hSys1 = new TH1D("hSys1","",20,0.0,0.9999);
    hSys2 = new TH1D("hSys2","",20,0.0,0.9999);
  }
  else{
    h = new TH1D("h","",20,0.0001,0.9999);
    hSys1 = new TH1D("hSys1","",20,0.0001,0.9999);
    hSys2 = new TH1D("hSys2","",20,0.0001,0.9999);
  }
  nt->Draw("(pt1-pt2)/(pt1+pt2)>>h",Form("(%s)",cut.Data())); 
  
  nt->Draw("(pt1-pt2)/(pt1+pt2)>>h",Form("(%s)",cut.Data())); 
  nt->Draw("(pt1*(1-0.07*rand.r1*0)-pt2*(1-0.07*rand.r2*1))/(pt1*(1-0.07*rand.r1*0)+pt2*(1-0.07*rand.r2*1))>>hSys1",Form("(%s)",cut.Data())); 
  nt->Draw("(pt1*(1-0.07*rand.r1*(pt1-120)/(35-120))-pt2*(1-0.07*rand.r2*(pt2-120)/(35-120)))/(pt1*(1-0.07*rand.r1*(pt1-120)/(35-120))+pt2*(1-0.07*rand.r2*(pt2-120)/(35-120)))>>hSys2",Form("(%s)",cut.Data())); 

  // calculate the statistical error and normalize
  h->Sumw2();
  h->Scale(1./h->GetEntries());
  h->SetMarkerStyle(20);

  hSys1->Scale(1./hSys1->Integral(0,20));
  hSys1->SetLineColor(kBlue);
  hSys1->SetFillColor(kAzure-8);
  hSys1->SetFillStyle(0);

  hSys1->SetStats(0);

  //hSys1->SetYTitle("1/N_{leading jet} dN/dA_{J}");
  hSys1->SetYTitle("Event Fraction");

  hSys1->Draw("hist");


  hSys1->GetXaxis()->SetLabelSize(22);
  hSys1->GetXaxis()->SetLabelFont(43);
  hSys1->GetXaxis()->SetTitleSize(24);
  hSys1->GetXaxis()->SetTitleFont(43);
  hSys1->GetXaxis()->SetTitleOffset(1.4);
  hSys1->GetXaxis()->CenterTitle();

  hSys1->GetXaxis()->SetNdivisions(905,true);
  hSys1->GetYaxis()->SetNdivisions(505,true);
  
  if(drawXLabel)hSys1->SetXTitle("A_{J} #equiv (p_{T}^{j1}-p_{T}^{j2})/(p_{T}^{j1}+p_{T}^{j2})");

  hSys1->GetYaxis()->SetLabelSize(22);
  hSys1->GetYaxis()->SetLabelFont(43);
  hSys1->GetYaxis()->SetTitleSize(22);
  hSys1->GetYaxis()->SetTitleFont(43);
  hSys1->GetYaxis()->SetTitleOffset(2.4);
  hSys1->GetYaxis()->CenterTitle();
  

  if(cbin==5)hSys1->SetAxisRange(0.0001,0.2,"Y");
  else hSys1->SetAxisRange(0,0.1999,"Y");
  


  hSys2->Scale(1./hSys2->Integral(0,20));
  hSys2->SetLineColor(kRed);
  hSys2->SetFillColor(kRed-9);
  hSys2->SetFillStyle(0);
  hSys2->Draw("same");
  
  h->Draw("same");

  if(drawLeg){
    TLegend *t3=new TLegend(0.25,0.5,0.60,0.95); 
    t3->AddEntry(h,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","pl");
    t3->AddEntry(hSys1,"Data, 2^{nd} Jet E_{T} scaled","l");
    t3->AddEntry(hSys2,"Data, tile JEC","l");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
  }

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
