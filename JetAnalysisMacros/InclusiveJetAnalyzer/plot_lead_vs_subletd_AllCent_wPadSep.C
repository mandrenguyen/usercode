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
#include "TStyle.h"
#include "TPaletteAxis.h"

#endif


//---------------------------------------------------------------------



void plotBal_vs_Imbal(int cbin = 0,
		  TString infname = "data.root",
	          bool useWeight = true,
		  bool drawXLabel = false,
		      bool drawLeg = false,
		      int kind = 0    // kind = 0 for data, 1 for mix 2 for pythia
		      );

void drawText(const char *text, float xp, float yp);


void plot_lead_vs_subletd_AllCent_wPadSep() {
   
  gStyle->SetNdivisions(505,"xyz");
  gStyle->SetPadRightMargin(0.12);
  gStyle->SetPadLeftMargin(0.13);
  gStyle->SetPadTopMargin(0.);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetLabelOffset(0.0,"xy");

   TCanvas *c1 = new TCanvas("c1","",1250,1100);
   

   c1->Divide(3,3);

   c1->cd(1);
   plotBal_vs_Imbal(2,"data.root",true,false,false,0);
   float px(0.25),py(0.75);
   drawText("30-100%",px,py);
   

  c1->cd(2);
  plotBal_vs_Imbal(1,"data.root",true,true,false,0);

  drawText("10-30%",px,py);

  drawText("#Delta#phi_{12} > #frac{2}{3}#pi rad",px,py+0.14);



  c1->cd(3);
  plotBal_vs_Imbal(0,"data.root",true,false,true,0);

  drawText("0-10%",px,py);

  c1->cd(1);
  TLatex *datalabel = new TLatex(130,225,"PbPb  #sqrt{s}_{_{NN}}=2.76 TeV");
  datalabel->SetTextFont(63);
  datalabel->SetTextSize(18);
  datalabel->Draw();

  c1->cd(3);
  TLatex *cms = new TLatex(130,225,"CMS");
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();
  TLatex *lumi = new TLatex(155,225,"#intL dt = 6.7 #mub^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(18);
  lumi->Draw();
  
  c1->cd(4);
  plotBal_vs_Imbal(2,"mix.root",true,false,false,1);
  drawText("30-100%",px,py);
  c1->cd(5);
  plotBal_vs_Imbal(1,"mix.root",true,true,false,1);

  drawText("10-30%",px,py);
  
  c1->cd(6);
  plotBal_vs_Imbal(0,"mix.root",true,false,true,1);
  drawText("0-10%",px,py);
 
 

  c1->cd(4);
  TLatex *mix = new TLatex(125,225,"PYTHIA+DATA");
  mix->SetTextFont(63);
  mix->SetTextSize(18);
  mix->Draw();
  


  c1->cd(7);
  plotBal_vs_Imbal(1,"pythia.root",true,true,false,2);

  TLatex *pythia = new TLatex(125,225,"PYTHIA");
  pythia->SetTextFont(63);
  pythia->SetTextSize(18);

  
  pythia->Draw();
  


  c1->Print("./fig/dijet_lead_vs_sublead_et_all_cent_20101126_v0.gif");
  c1->Print("./fig/dijet_lead_vs_sublead_et_all_cent_20101126_v0.eps");
  c1->Print("./fig/dijet_lead_vs_sublead_et_all_cent_20101126_v0.pdf");

}



void plotBal_vs_Imbal(int cbin,
		      TString infname,
		      bool useWeight,
		      bool drawXLabel,
		      bool drawLeg,
		      int kind
		      )
{
  
   TString cut="dphi>2./3.*acos(-1.) &&";//  "et1>120 && et2>50";

   TString cstring = "";
  if(cbin==0) {
    cstring = "0-10%";
    cut+="  bin>=0 && bin<4";
  } else if (cbin==1) {
    cstring = "10-30%";
    cut+="  bin>=4 && bin<12";

  } else {
    cstring = "30-100%";
    cut+="  bin>=12 && bin<40";
  }
  

  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");
  // projection histogram
  TH2D *h = new TH2D("h","",28,120.001,259.999,40,50.001,249.999);
  h->GetZaxis()->SetRangeUser(0.001,4.2);
  char* var="et2:et1";
  
  cout<<" infname.Data() "<<infname.Data()<<endl;
  if(infname.BeginsWith("pythia")){
    nt->Draw(Form("%s>>h",var)); 
  }
  else nt->Draw(Form("%s>>h",var),Form("(%s)",cut.Data())); 
    
  h->Scale(1./h->GetEntries());
  h->Scale(1000);

   
  h->SetStats(0);
  if(drawXLabel) h->SetXTitle("Leading jet p_{T} (GeV/c)");
  h->GetXaxis()->SetLabelSize(20);
  h->GetXaxis()->SetLabelFont(43);
  h->GetXaxis()->SetTitleSize(22);
  h->GetXaxis()->SetTitleFont(43);
  h->GetXaxis()->SetTitleOffset(2.6);
  h->GetXaxis()->CenterTitle();
  h->SetYTitle("Subleading jet p_{T} (GeV/c)");
  h->GetYaxis()->SetLabelSize(20);
  h->GetYaxis()->SetLabelFont(43);
  h->GetYaxis()->SetTitleSize(20);
  h->GetYaxis()->SetTitleFont(43);
  h->GetYaxis()->SetTitleOffset(3.1);
  h->GetYaxis()->CenterTitle();
  gPad->SetLogz();


  //if(cbin!=0)h->GetZaxis()->SetTickLength(0);
  h->Draw("zcol");
  // Need to update to grab the palette
  gPad->Update();
  TPaletteAxis *palette = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");
  palette->SetLabelOffset(-0.01);
  palette->SetLabelSize(0.045);

    
  TLegend *t3[10]; 

  if(drawLeg){
    t3[kind]=new TLegend(0.05,0.69,0.59,0.85);
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





