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

void plotBalance(int cbin = 0,
		 TString dataname1 = "data.root",
		 TString dataname2 = "data_IC5.root",
		 TString dataname3 = "data_IC5PF.root",
		 TString dataname4 = "data_AK5PF.root",
		 TString pythianame1 = "pythia.root",
		 TString pythianame2 = "pythia_IC5.root",
		 TString pythianame3 = "pythia_IC5PF.root",
		 TString pythianame4 = "pythia_AK5PF.root",
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

void plotBalanceAllCent_withPF(){

  TCanvas *c1 = new TCanvas("c1","",1250,530);

  makeMultiPanelCanvas(c1,3,1,0.0,0.0,0.2,0.15,0.02);

  c1->cd(1);
  plotBalance(2,"data.root","data_IC5.root","data_IC5PF.root","data_AK5PF.root","pythia.root","pythia_IC5.root","pythia_IC5PF.root","pythia_AK5PF.root",false,false);
  drawText("30-100%",0.76,0.24);
  drawPatch(0.976,0.0972,1.1,0.141);

  c1->cd(2);
  plotBalance(1,"data.root","data_IC5.root","data_IC5PF.root","data_AK5PF.root","pythia.root","pythia_IC5.root","pythia_IC5PF.root","pythia_AK5PF.root",true,false);
  drawText("10-30%",0.75,0.24);
  drawPatch(-0.00007,0.0972,0.0518,0.141);
  drawPatch(0.976,0.0972,1.1,0.141);

  c1->cd(3);
  plotBalance(0,"data.root","data_IC5.root","data_IC5PF.root","data_AK5PF.root","pythia.root","pythia_IC5.root","pythia_IC5PF.root","pythia_AK5PF.root",false,true);
  drawText("0-10%",0.75,0.24);
  drawPatch(-0.00007,0.0972,0.0518,0.141);

  TLatex *cms = new TLatex(0.35,0.23,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();                                                                                                                                        

  TLatex *lumi = new TLatex(0.73,0.23,"#intL dt = 7 #mub^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw(); 

  c1->Print("./fig/dijet_imbalance_all_cent_withPF_v0.gif");
  c1->Print("./fig/dijet_imbalance_all_cent_withPF_v0.eps");
  c1->Print("./fig/dijet_imbalance_all_cent_withPF_v0.pdf");

}

void plotBalance(int cbin,
		 TString dataname1,
		 TString dataname2,
		 TString dataname3,
		 TString dataname4,
		 TString pythianame1,
		 TString pythianame2,
		 TString pythianame3,
		 TString pythianame4,
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
  TFile *infdata1 = new TFile(dataname1.Data());
  TTree *ntdata1 =(TTree*)infdata1->FindObjectAny("nt");

  TFile *infdata2 = new TFile(dataname2.Data());
  TTree *ntdata2 =(TTree*)infdata2->FindObjectAny("nt");

  TFile *infdata3 = new TFile(dataname3.Data());
  TTree *ntdata3 =(TTree*)infdata3->FindObjectAny("nt");

  TFile *infdata4 = new TFile(dataname4.Data());
  TTree *ntdata4 =(TTree*)infdata4->FindObjectAny("nt");

  TFile *infpythia1 = new TFile(pythianame1.Data());
  TTree *ntpythia1 =(TTree*)infpythia1->FindObjectAny("nt");

  TFile *infpythia2 = new TFile(pythianame2.Data());
  TTree *ntpythia2 =(TTree*)infpythia2->FindObjectAny("nt");

  TFile *infpythia3 = new TFile(pythianame3.Data());
  TTree *ntpythia3 =(TTree*)infpythia3->FindObjectAny("nt");

  TFile *infpythia4 = new TFile(pythianame4.Data());
  TTree *ntpythia4 =(TTree*)infpythia4->FindObjectAny("nt");



  // projection histogram
  TH1D *hdata1 = new TH1D("hdata1","hdata1",20,0,1);
  TH1D *hdata2 = new TH1D("hdata2","hdata2",20,0,1);
  TH1D *hdata3 = new TH1D("hdata3","hdata3",20,0,1);
  TH1D *hdata4 = new TH1D("hdata4","hdata4",20,0,1);

  TH1D *hpythia1 = new TH1D("hpythia1","hpythia1",20,0,1);
  TH1D *hpythia2 = new TH1D("hpythia2","hpythia2",20,0,1);
  TH1D *hpythia3 = new TH1D("hpythia3","hpythia3",20,0,1);
  TH1D *hpythia4 = new TH1D("hpythia4","hpythia4",20,0,1);

  ntdata1->Draw("(et1-et2)/(et1+et2)>>hdata1",Form("(%s)",cut.Data())); 
  ntdata2->Draw("(et1-et2)/(et1+et2)>>hdata2",Form("(%s)",cut.Data())); 
  ntdata3->Draw("(et1-et2)/(et1+et2)>>hdata3",Form("(%s)",cut.Data())); 
  ntdata4->Draw("(et1-et2)/(et1+et2)>>hdata4",Form("(%s)",cut.Data())); 

  ntpythia1->Draw("(et1-et2)/(et1+et2)>>hpythia1",Form("(%s)",cutpp.Data())); 
  ntpythia2->Draw("(et1-et2)/(et1+et2)>>hpythia2",Form("(%s)",cutpp.Data())); 
  ntpythia3->Draw("(et1-et2)/(et1+et2)>>hpythia3",Form("(%s)",cutpp.Data())); 
  ntpythia4->Draw("(et1-et2)/(et1+et2)>>hpythia4",Form("(%s)",cutpp.Data())); 


  // calculate the statistical error and normalize
  hdata1->Sumw2();
  hdata1->Scale(1./hdata1->GetEntries());
  hdata1->SetMarkerStyle(20);

  hdata2->Sumw2();
  hdata2->Scale(1./hdata2->GetEntries());
  hdata2->SetMarkerStyle(20);
  hdata2->SetMarkerColor(2);

  hdata3->Sumw2();
  hdata3->Scale(1./hdata3->GetEntries());
  hdata3->SetMarkerStyle(20);
  hdata3->SetMarkerColor(3);

  hdata4->Sumw2();
  hdata4->Scale(1./hdata4->GetEntries());
  hdata4->SetMarkerStyle(20);
  hdata4->SetMarkerColor(4);


  if(drawXLabel) hpythia1->SetXTitle("A_{J} = (E_{T}^{j1}-E_{T}^{j2})/(E_{T}^{j1}+E_{T}^{j2})");

  hpythia1->Scale(1./hpythia1->GetEntries());
  hpythia2->Scale(1./hpythia2->GetEntries());
  hpythia3->Scale(1./hpythia3->GetEntries());
  hpythia4->Scale(1./hpythia4->GetEntries());

  hpythia2->SetLineColor(2);
  hpythia3->SetLineColor(3);
  hpythia4->SetLineColor(4);
  
  hpythia1->GetXaxis()->SetLabelSize(22);
  hpythia1->GetXaxis()->SetLabelFont(43);
  hpythia1->GetXaxis()->SetTitleSize(24);
  hpythia1->GetXaxis()->SetTitleFont(43);
  hpythia1->GetXaxis()->SetTitleOffset(1.4);
  hpythia1->GetXaxis()->CenterTitle();
  
  hpythia1->GetXaxis()->SetNdivisions(905,true);
  
  hpythia1->SetYTitle("Event Fraction");
  
  hpythia1->GetYaxis()->SetLabelSize(22);
  hpythia1->GetYaxis()->SetLabelFont(43);
  hpythia1->GetYaxis()->SetTitleSize(22);
  hpythia1->GetYaxis()->SetTitleFont(43);
  hpythia1->GetYaxis()->SetTitleOffset(2.4);
  hpythia1->GetYaxis()->CenterTitle();
  

  hpythia1->SetAxisRange(0,0.2,"Y");


  hpythia1->SetMaximum(1.3*hpythia1->GetMaximum());

  hpythia1->Draw("hist");
  hpythia2->Draw("hist,same");
  hpythia3->Draw("hist,same");
  hpythia4->Draw("hist,same");

  hdata1->Draw("same");
  hdata2->Draw("same");
  hdata3->Draw("same");
  hdata4->Draw("same");
  
  if(drawLeg){
    TLegend *t3=new TLegend(0.175,0.675,0.725,0.88); 
    //t3->AddEntry(h,"Pb+Pb  #sqrt{s}_{_{NN}}=2.76 TeV","pl");
    t3->SetHeader("Pb+Pb #sqrt{s}_{_NN}=2.76 TeV");
    t3->AddEntry(hdata1,"IC5 Calo Jets, Standard Subtraction","pl");
    t3->AddEntry(hdata2,"IC5 Calo Jets, FastJet Subtraction","pl");
    t3->AddEntry(hdata3,"IC5 PF Jets, FastJet Subtraction","pl");
    t3->AddEntry(hdata4,"AK5 PF Jets, FastJet Subtraction","pl");
    t3->AddEntry(hpythia1,"Unembedded Pythia, Same Color Code","l");
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
