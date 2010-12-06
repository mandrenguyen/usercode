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

void plotLeadingJet(int cbin = 0,
		    TString infname = "data.root",
		    TString hydjet = "pythia.root",
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

void plotLeadingJetAllCent(){

  TCanvas *c1 = new TCanvas("c1","",1650,430);

  makeMultiPanelCanvas(c1,5,1,0.0,0.0,0.2,0.15,0.02);


  c1->cd(1);
  plotLeadingJet(2,"data.root","pythia.root","mix.root",true,false,false);
  gPad->SetLogy();
  drawText("50-100%",0.30,0.8);
  drawPatch(0.976,0.0972,1.1,0.141);
  gPad->SetBottomMargin(0.18);
  gPad->SetLeftMargin(0.24);

  c1->cd(2);
  plotLeadingJet(1,"data.root","pythia.root","mix.root",true,false,false);
  gPad->SetLogy();
  drawText("30-50%",0.08,0.8);
  drawPatch(-0.00007,0.0972,0.0518,0.141);
  drawPatch(0.976,0.0972,1.1,0.141);
  gPad->SetBottomMargin(0.18);

  c1->cd(3);
  plotLeadingJet(2,"data.root","pythia.root","mix.root",true,true,false);
  gPad->SetLogy();
  drawText("20-30%",0.08,0.8);
  drawPatch(0.976,0.0972,1.1,0.141);
  gPad->SetBottomMargin(0.18);

  c1->cd(4);
  plotLeadingJet(1,"data.root","pythia.root","mix.root",true,false,false);
  gPad->SetLogy();
  drawText("10-20%",0.08,0.8);
  drawPatch(-0.00007,0.0972,0.0518,0.141);
  drawPatch(0.976,0.0972,1.1,0.141);
  gPad->SetBottomMargin(0.18);

  c1->cd(5);
  plotLeadingJet(0,"data.root","pythia.root","mix.root",true,false,true);
  gPad->SetLogy();
  drawText("0-10%",0.08,0.8);
  drawPatch(-0.00007,0.0972,0.0518,0.141);
  gPad->SetBottomMargin(0.18);

  TLatex *cms = new TLatex(140,1.625,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();

  TLatex *lumi = new TLatex(186,1.625,"#intL dt = 7 #mub^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw();

  c1->Print("./fig/dijet_leadingjet_all_cent_20101126_v0.gif");
  c1->Print("./fig/dijet_leadingjet_all_cent_20101126_v0.eps");
  c1->Print("./fig/dijet_leadingjet_all_cent_20101126_v0.pdf");

}



void plotLeadingJet(int cbin,
		    TString infname,
		    TString hydjet,
		    TString mix,
		    bool useWeight,
		    bool drawXLabel,
		    bool drawLeg)
{

  TString cut="et1>120 && et2>50 && dphi>2.5";
  TString cutpp="et1>120 && et2>50 && dphi>2.5";
  TString cstring = "";


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

  // open the hydjet (MC) file
  TFile *infHydjet = new TFile(hydjet.Data());
  TTree *ntHydjet = (TTree*) infHydjet->FindObjectAny("nt");

  // open the datamix file
  TFile *infMix = new TFile(mix.Data());
  TTree *ntMix =(TTree*)infMix->FindObjectAny("nt");


  // projection histogram
  TH1D *h = new TH1D("h","",10,120,220);
  TH1D *hPythia = new TH1D("hPythia","",10,120,220);
  TH1D *hDataMix = new TH1D("hDataMix","",10,120,220);
  nt->Draw("et1>>h",Form("(%s)",cut.Data())); 
   
  if (useWeight) {
    // use the weight value caluculated by Matt's analysis macro
    ntHydjet->Draw("et1>>hPythia",Form("(%s)",cutpp.Data())); 
    ntMix->Draw("et1>>hDataMix",Form("(%s)*weight",cut.Data())); 
  } else {
    // ignore centrality reweighting
    ntHydjet->Draw("et1>>hPythia",Form("(%s)",cutpp.Data()));
    ntMix->Draw("et1>>hDataMix",Form("(%s)",cut.Data()));  
  }

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
  if(drawXLabel) hDataMix->SetXTitle("Leading Jet E_{T} (GeV)");

  hDataMix->GetXaxis()->SetLabelSize(22);
  hDataMix->GetXaxis()->SetLabelFont(43);
  hDataMix->GetXaxis()->SetTitleSize(24);
  hDataMix->GetXaxis()->SetTitleFont(43);
  hDataMix->GetXaxis()->SetTitleOffset(1.4);
  hDataMix->GetXaxis()->CenterTitle();

  hDataMix->GetXaxis()->SetNdivisions(904,true);

  hDataMix->SetYTitle("Event Fraction");

  hDataMix->GetYaxis()->SetLabelSize(22);
  hDataMix->GetYaxis()->SetLabelFont(43);
  hDataMix->GetYaxis()->SetTitleSize(22);
  hDataMix->GetYaxis()->SetTitleFont(43);
  hDataMix->GetYaxis()->SetTitleOffset(2.4);
  hDataMix->GetYaxis()->CenterTitle();

  hPythia->SetAxisRange(2E-3,3,"Y");
  hDataMix->SetAxisRange(2E-3,3,"Y");
  h->SetAxisRange(2E-3,3,"Y");




  hDataMix->Draw("hist");
  //hPythia->Draw("same");
  h->Draw("same");


  if(drawLeg){
    TLegend *t3=new TLegend(0.25,0.64,0.79,0.90);
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
