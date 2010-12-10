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

void plotBal_vs_Imbal(int cbin = 0,
		  TString infname = "data.root",
	          bool useWeight = true,
		  bool drawXLabel = false,
		      bool drawLeg = false,
		      int kind = 0    // kind = 0 for data, 1 for mix 2 for pythia
		      );

void drawText(const char *text, float xp, float yp);
void drawDum(float min, float max, double drawXLabel);

//--------------------------------------------------------------
// drawPatch() is a crazy way of removing 0 in the second and third 
// pad which is partially shown due to no margin between the pads
// if anybody has a better way of doing it let me know! - Andre
//--------------------------------------------------------------
void drawPatch(float x1, float y1, float x2, float y2); 
//---------------------------------------------------------------------

void plotLeadingPhi_Balance_vs_Imbalnce_AllCent() {

   TCanvas *c1 = new TCanvas("c1","",1250,1100);
   
   makeMultiPanelCanvas(c1,3,3,0.0,0.0,0.2,0.15,0.02);
   
   c1->cd(1);
   plotBal_vs_Imbal(2,"data.root",true,false,false,0);
   gPad->SetLogy();
   drawText("30-100%",0.67,0.24);
   //  drawPatch(0.976,0.0972,1.1,0.141);
   
  c1->cd(2);
  plotBal_vs_Imbal(1,"data.root",true,true,false,0);
  gPad->SetLogy();
  drawText("10-30%",0.65,0.24);
  //  drawPatch(-0.00007,0.0972,0.0518,0.141);
  //  drawPatch(0.976,0.0972,1.1,0.141);

  c1->cd(3);
  plotBal_vs_Imbal(0,"data.root",true,false,true,0);
  gPad->SetLogy();
  drawText("0-10%",0.65,0.24);
  //  drawPatch(-0.00007,0.0972,0.0518,0.141);

  TLatex *cms = new TLatex(-2.5,1.29,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();
  TLatex *lumi = new TLatex(0.2,1.29,"#intL dt = 6.7 #mub^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw();

 
  c1->cd(4);
  plotBal_vs_Imbal(2,"mix.root",true,false,false,1);
  gPad->SetLogy();
  drawText("30-100%",0.67,0.24);
  //  drawPatch(0.966,0.0972,1.1,0.141);
  
  c1->cd(5);
  plotBal_vs_Imbal(1,"mix.root",true,true,false,1);
  gPad->SetLogy();
  drawText("10-30%",0.65,0.24);
  //  drawPatch(-0.00007,0.0972,0.0518,0.141);
  //  drawPatch(0.966,0.0972,1.1,0.141);
  
  c1->cd(6);
  plotBal_vs_Imbal(0,"mix.root",true,false,true,1);
  gPad->SetLogy();
  drawText("0-10%",0.65,0.24);
  //  drawPatch(-0.00007,0.0972,0.0318,0.141);
  TLatex *mix = new TLatex(-2.5,1.29,"embedded PYTHIA");
  mix->SetTextFont(63);
  mix->SetTextSize(18);
  mix->Draw();

   

  c1->cd(7);
  plotBal_vs_Imbal(2,"pythia.root",true,false,false,2);
  gPad->SetLogy();
  drawText("30-100%",0.67,0.24);
  drawPatch(0.966,0.0972,1.1,0.141);

  c1->cd(8);
  plotBal_vs_Imbal(1,"pythia.root",true,true,false,2);
  gPad->SetLogy();
  drawText("10-30%",0.65,0.24);
  drawPatch(-0.00007,0.0972,0.0518,0.141);
  drawPatch(0.956,0.0992,1.1,0.141);
  c1->cd(9);
  plotBal_vs_Imbal(0,"pythia.root",true,false,true,2);
  gPad->SetLogy();
  drawText("0-10%",0.65,0.24);
  drawPatch(-0.00007,0.0972,0.0418,0.141);
  TLatex *pythia = new TLatex(-2.5,1.29,"PYTHIA");
  pythia->SetTextFont(63);
  pythia->SetTextSize(18);
  pythia->Draw();




  c1->Print("./fig/dijet_leadingPhi_balance_vs_Imbalance_all_cent_20101126_v0.gif");
  c1->Print("./fig/dijet_leadingPhi_balance_vs_Imbalance_all_cent_20101126_v0.eps");
  c1->Print("./fig/dijet_leadingPhi_balance_vs_Imbalance_all_cent_20101126_v0.pdf");

}



void plotBal_vs_Imbal(int cbin,
		      TString infname,
		      bool useWeight,
		      bool drawXLabel,
		      bool drawLeg,
		      int kind)
{

  TString cut="et1>120 && et2>50";
  TString cutpp="et1>120 && et2>50";
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
  
  TString cutBal   = cut+"&& ((et1-et2)/(et1+et2) < 0.3)";
  TString cutImBal = cut+"&& ((et1-et2)/(et1+et2) > 0.3)";
  TString cutBalpp   = cutpp+"&& ((et1-et2)/(et1+et2) < 0.3)";
  TString cutImBalpp = cutpp+"&& ((et1-et2)/(et1+et2) > 0.3)";

  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");
  // projection histogram
  TH1D *hBal = new TH1D("hBal","",20,-3.1416,3.1416);
  TH1D *hImBal = new TH1D("hImBal","",20,-3.1416,3.1416);
  
  char* var="phi1";
  
  if ( kind == 0) {
     nt->Draw(Form("%s>>hBal",var),Form("(%s)",cutBal.Data())); 
     nt->Draw(Form("%s>>hImBal",var),Form("(%s)",cutImBal.Data()));
  }
  if ( kind == 1) {
     
     if (useWeight) {
	// use the weight value caluculated by Matt's analysis macro
	nt->Draw(Form("%s>>hBal",var),Form("(%s)*weight",cutBal.Data()));
	nt->Draw(Form("%s>>hImBal",var),Form("(%s)*weight",cutImBal.Data()));
     } else {
	// ignore centrality reweighting
	nt->Draw(Form("%s>>hImBal",var),Form("(%s)",cutBal.Data()));
        nt->Draw(Form("%s>>hImBal",var),Form("(%s)",cutImBal.Data()));
     }
  }
  if ( kind == 2) {
      nt->Draw(Form("%s>>hBal",var),Form("(%s)",cutBalpp.Data())); 
      nt->Draw(Form("%s>>hImBal",var),Form("(%s)",cutImBalpp.Data()));
  }

  // calculate the statistical error and normalize
  if ( kind == 0) {
     hBal->Sumw2();
     hBal->Scale(1./hBal->GetEntries());
     hBal->SetMarkerStyle(24);
     hImBal->Sumw2();
     hImBal->Scale(1./hImBal->GetEntries());
     hImBal->SetMarkerStyle(20);
  }
  if ( kind == 1) {
     hBal->Scale(1./hBal->Integral(0,20));
     hBal->SetLineColor(kRed);
     hBal->SetFillColor(kRed-9);
     hBal->SetFillStyle(3001);
     hImBal->Scale(1./hImBal->Integral(0,20));
     hImBal->SetLineColor(kRed);
     hImBal->SetFillColor(kRed-9);
     hImBal->SetFillStyle(3004);
  }
  if ( kind ==2){
     hBal->Scale(1./hBal->Integral(0,20));
     hBal->SetLineColor(kBlue);
     hBal->SetFillColor(kAzure-8);
     hBal->SetFillStyle(3001);
     hImBal->Scale(1./hImBal->Integral(0,20));
     hImBal->SetLineColor(kBlue);
     hImBal->SetFillColor(kAzure-8);
     hImBal->SetFillStyle(3004);
  }
  
  
  hBal->SetStats(0);
  hImBal->SetStats(0);
  hBal->SetAxisRange(8E-4,2.9,"Y");
  if(drawXLabel) hBal->SetXTitle("Leading jet #phi");
  hBal->GetXaxis()->SetLabelSize(20);
  hBal->GetXaxis()->SetLabelFont(43);
  hBal->GetXaxis()->SetTitleSize(22);
  hBal->GetXaxis()->SetTitleFont(43);
  hBal->GetXaxis()->SetTitleOffset(2.4);
  hBal->GetXaxis()->CenterTitle();
  hBal->SetYTitle("Event Fraction");
  hBal->GetYaxis()->SetLabelSize(20);
  hBal->GetYaxis()->SetLabelFont(43);
  hBal->GetYaxis()->SetTitleSize(20);
  hBal->GetYaxis()->SetTitleFont(43);
  hBal->GetYaxis()->SetTitleOffset(3.1);
  hBal->GetYaxis()->CenterTitle();
  
  hBal->Draw();
  hImBal->Draw("same");
  
  TLegend *t3[10]; 

  if(drawLeg){
    t3[kind]=new TLegend(0.05,0.69,0.59,0.85);
    //   t3->AddEntry(hBal,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","pl");
    char* legOps;
    if ( kind ==0)  legOps = "pl";
    else  legOps = "lf";
    t3[kind]->AddEntry(hBal,"Balanced jet ( A_{J} < 0.3)",legOps); //unquenched PYTHIA","lf");
    t3[kind]->AddEntry(hImBal,"Imbalanced jet (A_{J} > 0.3)",legOps);//DataMix,"unquenched PYTHIA + Data","lf");
    t3[kind]->SetFillColor(0);
    t3[kind]->SetBorderSize(0);
    t3[kind]->SetFillStyle(0);
    t3[kind]->SetTextFont(63);
    t3[kind]->SetTextSize(15);
    t3[kind]->Draw();
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

  if(drawXLabel) hdum->SetXTitle("(p_{T}^{j1}-p_{T}^{j2})/(p_{T}^{j1}+p_{T}^{j2})");
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
