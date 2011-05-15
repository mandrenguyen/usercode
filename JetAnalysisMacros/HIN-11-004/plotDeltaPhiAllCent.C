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

void plotDeltaPhi(int cbin = 0,
		 TString infname = "data_akPu3PF.root",
		 TString mix = "mix_akPu3PF.root",
		 bool useWeight = true,
		 bool drawXLabel = false,
		 bool drawLeg = false);

void plotPPDPhiAll();

void drawText(const char *text, float xp, float yp);


void plotDeltaPhiAllCent(){

   TCanvas *c1 = new TCanvas("c1","",1050,700);
   makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.2,0.15,0.02);



   c1->cd(1);
      plotDeltaPhi(-1,"pp_akPu3PF.root","pythia_akPu3PF.root",true,false,false);
   //plotPPDPhiAll();  
 gPad->SetLogy();
   drawText("(a)",0.22,0.9);
   //drawText(" 0-100%",0.28,0.55);

  TLatex *jetf;
  jetf = new TLatex(0.5,0.025,"anti-k_{T}, PF, R = 0.3");
  jetf->SetTextFont(63);
  jetf->SetTextSize(15);
  jetf->Draw();


   TLatex *lumi_pp = new TLatex(0.55,0.27,"#intL dt = 260 nb^{-1}");
   lumi_pp->SetTextFont(63);
   lumi_pp->SetTextSize(15);
   lumi_pp->Draw(); 



   c1->cd(2);
   plotDeltaPhi(4,"data_akPu3PF.root","mix_akPu3PF.root",true,false,true);
   gPad->SetLogy();
   drawText("50-100%",0.6,0.85);
   drawText("(b)",0.02,0.9);

   TLatex *lumi = new TLatex(0.55,0.27,"#intL dt = 7.2 #mub^{-1}");
   lumi->SetTextFont(63);
   lumi->SetTextSize(15);
   lumi->Draw(); 


   c1->cd(3);
   plotDeltaPhi(3,"data_akPu3PF.root","mix_akPu3PF.root",true,false,false);
   gPad->SetLogy();
   drawText("30-50%",0.6,0.85);
   drawText("(c)",0.02,0.9);

   TLatex tsel;
   tsel.SetNDC();
   tsel.SetTextFont(63);
   tsel.SetTextSize(15);
   tsel.DrawLatex(0.15,0.78,"p_{T,1} > 100 GeV/c");
   tsel.DrawLatex(0.15,0.68,"p_{T,2} > 40 GeV/c");
   
   // other labeling
  TLatex *cms = new TLatex(0.5,0.3,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(17);
  cms->Draw();

   c1->cd(4);
   plotDeltaPhi(2,"data_akPu3PF.root","mix_akPu3PF.root",true,false,false);
   gPad->SetLogy();
   drawText("20-30%",0.7,0.85);
   drawText("(d)",0.22,0.93);

   c1->cd(5);
   plotDeltaPhi(1,"data_akPu3PF.root","mix_akPu3PF.root",true,true,false);
   gPad->SetLogy();
   drawText("10-20%",0.6,0.85);
   drawText("(e)",0.02,0.93);


   c1->cd(6);
   plotDeltaPhi(0,"data_akPu3PF.root","mix_akPu3PF.root",true,false,false);
   gPad->SetLogy();
   drawText("0-10%",0.6,0.85);
   drawText("(f)",0.02,0.93);

  c1->Print("./fig/dijet_dphi_ak3PF_v0.gif");
  c1->Print("./fig/dijet_dphi_ak3PF_v0.eps");
  c1->Print("./fig/dijet_dphi_ak3PF_v0.pdf");

}



void plotDeltaPhi(int cbin,
		  TString infname,
		  TString mix,
		  bool useWeight,
		  bool drawXLabel,
		  bool drawLeg)
{

  TString cut="et1>100 && et2>40";
  TString cutpp="et1>100 && et2>40";
 
  TString cstring = "";
  if(cbin==-1) {
    //cstring = "0-100%";
     //cut+=" && bin>=0 && bin<40";
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


  // open the datamix file
  TFile *infMix = new TFile(mix.Data());
  TTree *ntMix =(TTree*)infMix->FindObjectAny("nt");

  // projection histogram
  TH1D *h = new TH1D("h","",30,0,3.14159);
  TH1D *hDataMix;
  if(cbin==2) hDataMix= new TH1D("hDataMix","",30,0,3.14159);
  else hDataMix= new TH1D("hDataMix","",30,0.0001,3.14159);

  
  nt->Draw("dphi>>h",Form("(%s)",cut.Data())); 
   
  if (useWeight) {
    // use the weight value caluculated by Matt's analysis macro
    ntMix->Draw("dphi>>hDataMix",Form("(%s)*weight",cut.Data())); 
  } else {
    // ignore centrality reweighting
    ntMix->Draw("dphi>>hDataMix",Form("(%s)",cut.Data()));  
  }

  // calculate the statistical error and normalize
  h->Sumw2();
  h->Scale(1./h->GetEntries());
  h->SetMarkerStyle(20);

   
  hDataMix->Scale(1./hDataMix->Integral());
  if(cbin==-1){
    hDataMix->SetLineColor(kBlue);
    hDataMix->SetFillColor(kAzure-8);
    hDataMix->SetFillStyle(3005);
  }
  else{
    hDataMix->SetLineColor(kRed);
    hDataMix->SetFillColor(kRed-9);
    hDataMix->SetFillStyle(3004);
  }
  hDataMix->SetStats(0);

  if(drawXLabel) hDataMix->SetXTitle("#Delta#phi_{1,2}");

  hDataMix->GetXaxis()->SetLabelSize(22);
  hDataMix->GetXaxis()->SetLabelFont(43);
  hDataMix->GetXaxis()->SetTitleSize(24);
  hDataMix->GetXaxis()->SetTitleFont(43);
  hDataMix->GetXaxis()->SetTitleOffset(1.7);
  hDataMix->GetXaxis()->CenterTitle();

  //hDataMix->GetXaxis()->SetNdivisions(705,true);

  hDataMix->SetYTitle("Event Fraction");

  hDataMix->GetYaxis()->SetLabelSize(22);
  hDataMix->GetYaxis()->SetLabelFont(43);
  hDataMix->GetYaxis()->SetTitleSize(22);
  hDataMix->GetYaxis()->SetTitleFont(43);
  hDataMix->GetYaxis()->SetTitleOffset(2.4);
  hDataMix->GetYaxis()->CenterTitle();

  if(cbin==2){
  hDataMix->SetAxisRange(5e-4,0.6,"Y");
  }
  else{
  hDataMix->SetAxisRange(5e-4,0.6,"Y");
  }
  if(cbin==2){
    hDataMix->GetXaxis()->SetRangeUser(0.,acos(-1.));
  }
  else{
    hDataMix->GetXaxis()->SetRangeUser(0.1,acos(-1.));
  }



  hDataMix->Draw("hist");
  h->Draw("same");

  if(drawLeg){
    TLegend *t3=new TLegend(0.15,0.63,0.69,0.83);
    t3->AddEntry(h,"PbPb  #sqrt{s}_{_{NN}} = 2.76 TeV","p");
    t3->AddEntry(hDataMix,"PYTHIA+HYDJET","lf");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
  }

  if(cbin==-1){

  // Legend
    TLegend *t3a=new TLegend(0.3,0.63,0.69,0.83);
  //t3a->SetHeader("ant-k_{T} (R=0.5) CaloJets");
  t3a->AddEntry(h,"pp  #sqrt{s} = 2.76 TeV","p");
  t3a->AddEntry(hDataMix,"PYTHIA","lf");
  t3a->SetFillColor(0);
  t3a->SetBorderSize(0);
  t3a->SetFillStyle(0);
  t3a->SetTextFont(63);
  t3a->SetTextSize(15);
  t3a->Draw();
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


void plotPPDPhiAll(){

  bool isPF = true;

  TString data_tag;
  TString mc_tag;
  TString jetfinder, jetfinder_tag;

  if(!isPF){
    data_tag = "hdata_ak5calo_DijetDPhi";
    mc_tag = "hmc_ak5calo_DijetDPhi_histonly";
    jetfinder_tag = "calo";
  }else{
    data_tag = "hdata_ak5pf_DijetDPhi";
    mc_tag = "hmc_ak5pf_DijetDPhi_histonly";
    jetfinder_tag ="pf";
  }

  TFile *fDATA = new TFile(Form("./pp/%s.root",data_tag.Data()));
  TFile *fMC = new TFile(Form("./pp/%s.root",mc_tag.Data()));

  TH1F *hDijetBal_data = (TH1F*) fDATA->Get("hDataDijetDPhi");
  TH1F *hDijetBal_mc = (TH1F*) fMC->Get("hQCDDijetDPhi");


  // normalization should be matched with what's in ANA
  hDijetBal_data->Scale(1./hDijetBal_data->Integral());
  //hDijetBal_data->Rebin(2);

  hDijetBal_mc->Scale(1./hDijetBal_mc->Integral());
  //hDijetBal_mc->Rebin(2);

  cout<<"Bin Width = "<<hDijetBal_data->GetBinWidth(1)<<endl;
  cout<<"Number of bins = "<<hDijetBal_data->GetNbinsX()<<endl;
  cout<<"Max Bin Edge "<<hDijetBal_data->GetBinLowEdge(20)+hDijetBal_data->GetBinWidth(1)<<endl;



  hDijetBal_mc->GetXaxis()->SetLabelSize(22);
  hDijetBal_mc->GetXaxis()->SetLabelFont(43);
  hDijetBal_mc->GetXaxis()->SetTitleSize(24);
  hDijetBal_mc->GetXaxis()->SetTitleFont(43);
  hDijetBal_mc->GetXaxis()->SetTitleOffset(1.7);
  hDijetBal_mc->GetXaxis()->CenterTitle();

  hDijetBal_mc->GetXaxis()->SetNdivisions(705,true);

  hDijetBal_mc->SetYTitle("Event Fraction");

  hDijetBal_mc->GetYaxis()->SetLabelSize(22);
  hDijetBal_mc->GetYaxis()->SetLabelFont(43);
  hDijetBal_mc->GetYaxis()->SetTitleSize(22);
  hDijetBal_mc->GetYaxis()->SetTitleFont(43);
  hDijetBal_mc->GetYaxis()->SetTitleOffset(2.4);
  hDijetBal_mc->GetYaxis()->CenterTitle();
  // data, mc styling
  hDijetBal_mc->SetLineColor(kBlue);
  hDijetBal_mc->SetFillColor(kAzure-8);
  hDijetBal_mc->SetFillStyle(3005);

  //hDum->Draw("hist");

  hDijetBal_mc->SetAxisRange(5e-4,0.6,"Y");



  hDijetBal_mc->Draw("hist");
  hDijetBal_data->Draw("pzsame");





  TLatex *lumi_pp = new TLatex(0.8,0.35,"#intL dt = 35.1 pb^{-1}");
  lumi_pp->SetTextFont(63);
  lumi_pp->SetTextSize(15);
  lumi_pp->Draw();

  
  TLatex *jetf;
  jetf = new TLatex(0.75,0.035,"Anti-k_{T}, PF, R=0.5");
  jetf->SetTextFont(63);
  jetf->SetTextSize(15);
  jetf->Draw();


}
