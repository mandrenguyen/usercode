#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include "TCanvas.h"
#include "TError.h"
#include "TPad.h"
#include "TString.h"
#include "TRandom.h"
#include "TH1F.h"
#include "TPaveText.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TString.h"
#include "TStyle.h"
#include "TROOT.h"

#endif


//---------------------------------------------------------------------
void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns, 
			  const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0., 
			  const Float_t leftMargin=0.2, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);

void plotLeadingJet(int cbin = 0,
		    TString infname = "data_akPu3PF.root",
		    TString mix = "mix_akPu3PF.root",
		    bool useWeight = true,
		    bool drawXLabel = false,
		    bool drawLeg = false
);

void drawText(const char *text, float xp, float yp);
void drawDum(float min, float max, double drawXLabel);


void plotLeadingJetAll();

void plotLeadingJetAllCent(){


  TCanvas *c1 = new TCanvas("c1","",1050,700);

  //makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.2,0.15,0.02);
  makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.1,0.15,0.02);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gROOT->ForceStyle(1);
  c1->cd(1);
  plotLeadingJet(-1,"pp_akPu3PF.root","pythia_akPu3PF.root",true,false,false);


  //plotLeadingJetAll();
  gPad->SetLogy();
  //c1->GetPad(1)->SetTicks(0);
  //gPad->SetTicky(0);
  drawText("(a)",0.12,0.9);
  //drawText(" 0-100%",0.28,0.8);
  //gPad->SetBottomMargin(0.18);
  // gPad->SetLeftMargin(0.24);


  TLatex *jetf;
  jetf = new TLatex(215,0.05,"anti-k_{T}, PF, R = 0.3");
  jetf->SetTextFont(63);
  jetf->SetTextSize(15);
  jetf->Draw();


  TLatex *lumi_pp = new TLatex(220,1.,"#intL dt = 260 nb^{-1}");
  lumi_pp->SetTextFont(63);
  lumi_pp->SetTextSize(15);
  lumi_pp->Draw();

  c1->cd(2);
  plotLeadingJet(4,"data_akPu3PF.root","mix_akPu3PF.root",true,false,true);
  gPad->SetLogy();
  drawText("50-100%",0.15,0.89);
  drawText("(b)",0.02,0.9);

  TLatex *lumi = new TLatex(220,1.,"#intL dt = 7.2 #mub^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw();


  c1->cd(3);
  plotLeadingJet(3,"data_akPu3PF.root","mix_akPu3PF.root",true,false,false);
  gPad->SetLogy();
  drawText("30-50%",0.15,0.89);
  drawText("(c)",0.02,0.9);


  TLatex tsel;
  tsel.SetNDC();
  tsel.SetTextFont(63);
  tsel.SetTextSize(15);
  tsel.DrawLatex(0.55,0.78,"p_{T,1} > 100 GeV/c");
  tsel.DrawLatex(0.55,0.68,"p_{T,2} > 40 GeV/c");
  tsel.DrawLatex(0.55,0.58,"#Delta#phi_{12} > #frac{2}{3}#pi");
  /*
  drawText("p_{T,1} > 100 GeV/c",0.55,0.78);
  drawText("p_{T,2} > 50 GeV/c",0.55,0.68);
  drawText("#Delta#phi_{12} > #frac{2}{3}#pi rad",0.55,0.58);
  */


  TLatex *cms = new TLatex(210,1.,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(17);
  cms->Draw();


  c1->cd(4);
   plotLeadingJet(2,"data_akPu3PF.root","mix_akPu3PF.root",true,false,false);
  gPad->SetLogy();
  drawText("20-30%",0.25,0.925);
  drawText("(d)",0.12,0.93);
  //gPad->SetLeftMargin(0.24);
  //  gPad->SetBottomMargin(0.22);
  
  c1->cd(5);
  plotLeadingJet(1,"data_akPu3PF.root","mix_akPu3PF.root",true,true,false);
  gPad->SetLogy();
  drawText("10-20%",0.15,0.925);
  drawText("(e)",0.02,0.93);


  c1->cd(6);
  plotLeadingJet(0,"data_akPu3PF.root","mix_akPu3PF.root",true,false,false);
  gPad->SetLogy();
  drawText("0-10%",0.15,0.925);
  drawText("(f)",0.02,0.93);

  c1->cd(0);
  TLatex *ytitle = new TLatex(0.025,0.2,"1/N_{jet} dN/dp_{T} (GeV/c)^{-1}, Arbitrary Normalization");
  ytitle->SetTextAngle(90);
  ytitle->SetTextFont(63);
  ytitle->SetTextSize(22);
  //ytitle->SetTextSize(0.05);
  ytitle->SetTextColor(kBlack);
  ytitle->SetLineWidth(1);
  //ytitle->SetNDC();
  ytitle->Draw();


  c1->Print("./fig/dijet_leadingjet_ak3PF_v0.gif");
  c1->Print("./fig/dijet_leadingjet_ak3PF_v0.eps");
  c1->Print("./fig/dijet_leadingjet_ak3PF_v0.pdf");

}



void plotLeadingJet(int cbin,
		    TString infname,
		    TString mix,
		    bool useWeight,
		    bool drawXLabel,
		    bool drawLeg
		    )
{

  TString cut="et1>100 && et2>40 && dphi>2./3.*acos(-1.)";
  TString cutpp="et1>100 && et2>40 && dphi>2./3.*acos(-1.)";


  TString cstring = "";
  if(cbin==-1) {  
    //cstring = "0-100%";
    //cut+=" && bin>=0 && bin<40";
    cstring = "pp";
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

  float binlim[10]={100.0001,120,140,160,180,200,220,240,280,320};
  float binlim_full[10]={100,120,140,160,180,200,220,240,280,320};
  TH1D *h, *hDataMix;
  if(cbin==2){
  h= new TH1D("h","h",9,binlim_full);
  hDataMix = new TH1D("hDataMix","hDataMix",9,binlim_full);
  }
  else{
  h= new TH1D("h","h",9,binlim);
  hDataMix = new TH1D("hDataMix","hDataMix",9,binlim);
  }
  //TH1D *h = new TH1D("h","",10,120,319.99999);
  //TH1D *hDataMix;
  //if(cbin==2) hDataMix = new TH1D("hDataMix","",10,120,319.99999);
  //else hDataMix = new TH1D("hDataMix","",10,120.00001,319.99999);

  nt->Draw("et1>>h",Form("(%s)",cut.Data())); 
   
  if (useWeight) {
    // use the weight value caluculated by Matt's analysis macro
    ntMix->Draw("et1>>hDataMix",Form("(%s)*weight",cut.Data())); 
  } else {
    // ignore centrality reweighting
    ntMix->Draw("et1>>hDataMix",Form("(%s)",cut.Data()));  
  }

  // calculate the statistical error and normalize
  h->Sumw2();


  for(int ibin=0;ibin<h->GetNbinsX();ibin++){
    h->SetBinContent(ibin+1, h->GetBinContent(ibin+1)/h->GetBinWidth(ibin+1));
    h->SetBinError(ibin+1, h->GetBinError(ibin+1)/h->GetBinWidth(ibin+1));    
  }

  h->Scale(1./h->Integral(1,h->GetNbinsX()));

  h->SetMarkerStyle(20);




  for(int ibin=0;ibin<hDataMix->GetNbinsX();ibin++){
    hDataMix->SetBinContent(ibin+1, hDataMix->GetBinContent(ibin+1)/hDataMix->GetBinWidth(ibin+1));
    hDataMix->SetBinError(ibin+1, hDataMix->GetBinError(ibin+1)/hDataMix->GetBinWidth(ibin+1));    
  }
  hDataMix->Scale(1./hDataMix->Integral(1,hDataMix->GetNbinsX()));

  if(cbin==-1){
    hDataMix->SetLineColor(kBlue);
    hDataMix->SetFillColor(kAzure-8);
    hDataMix->SetFillStyle(3004);
  }
  else{
    hDataMix->SetLineColor(kRed);
    hDataMix->SetFillColor(kRed-9);
    hDataMix->SetFillStyle(3004);
  }
   
  hDataMix->SetStats(0);
  if(drawXLabel) hDataMix->SetXTitle("Leading Jet p_{T} (GeV/c)");

  hDataMix->GetXaxis()->SetLabelSize(22);
  hDataMix->GetXaxis()->SetLabelFont(43);
  hDataMix->GetXaxis()->SetTitleSize(24);
  hDataMix->GetXaxis()->SetTitleFont(43);
  hDataMix->GetXaxis()->SetTitleOffset(1.4);
  hDataMix->GetXaxis()->CenterTitle();

  //hDataMix->GetXaxis()->SetNdivisions(904,true);
  hDataMix->GetXaxis()->SetNdivisions(505,true);

//hDataMix->SetYTitle("dN_{jet}/dp_{T} (GeV/c)^{-1}, Arb. Norm.");


//hDataMix->SetYTitle("dN_{jet}/dp_{T} (GeV/c)^{-1}");

  hDataMix->GetYaxis()->SetLabelSize(22);
  hDataMix->GetYaxis()->SetLabelFont(43);
  hDataMix->GetYaxis()->SetTitleSize(22);
  hDataMix->GetYaxis()->SetTitleFont(43);
  hDataMix->GetYaxis()->SetTitleOffset(2.4);
  hDataMix->GetYaxis()->CenterTitle();

  hDataMix->SetAxisRange(2E-4,3,"Y");
  h->SetAxisRange(2E-4,3,"Y");



  hDataMix->SetTitleOffset(1.9,"X");
  hDataMix->Draw("hist");
  h->Draw("same");


  if(drawLeg){
    TLegend *t3=new TLegend(0.4,0.63,0.95,0.83);
    t3->AddEntry(h,"PbPb  #sqrt{s}_{_{NN}} = 2.76 TeV","p");
    //t3->AddEntry(hPythia,"PYTHIA","lf");
    t3->AddEntry(hDataMix,"PYTHIA+HYDJET","lf");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
  }

  if(cbin==-1){
    TLegend *t3a=new TLegend(0.55,0.63,0.98,0.83);
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

  //hdum->SetYTitle("dN_{jet}/dp_{T} (GeV^{1}), Arbitrary Normalization");

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

//   for(int i=0;i<6;i++){
//   Xlow[i]+=0.05;
//   Xup[i]+=0.05;
//   }

   Xlow[0]+=0.05;
   Xup[0]+=2./3.*0.05;
   Xlow[1]+=2./3.*0.05;
   Xup[1]+=1./3.*0.05;
   Xlow[2]+=1./3.*0.05;


   //Xup[0]+=0.05/3.;

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

void plotLeadingJetAll(){

  bool isPF = false;

  TString data_tag;
  TString mc_tag;
  TString jetfinder, jetfinder_tag;

  if(!isPF){
    data_tag = "hdata_ak5calo_Jet1RawPt";
    mc_tag = "hmc_ak5calo_Jet1RawPt_histonly";
    jetfinder_tag = "calo";
  }else{
    data_tag = "hdata_ak5pf_Jet1RawPt";
    mc_tag = "hmc_ak5pf_Jet1RawPt_histonly";
    jetfinder_tag ="pf";
  }

  cout<<" data_tag.Data() "<<data_tag.Data()<<endl;
  cout<<" mc_tag.Data() "<<mc_tag.Data()<<endl;
  TFile *fDATA = new TFile(Form("./pp/%s.root",data_tag.Data()));
  TFile *fMC = new TFile(Form("./pp/%s.root",mc_tag.Data()));


  TH1F *hLeadingJetPt_data = (TH1F*) fDATA->Get("hDataJet1Pt");
  TH1F *hLeadingJetPt_mc = (TH1F*) fMC->Get("hQCDJet1Pt");




  // data, mc styling


  hLeadingJetPt_data->Rebin(2);
  hLeadingJetPt_mc->Rebin(2);

  float binlim[9]={120,140,160,180,200,220,240,280,320};
  TH1D *hLeadingJetPt_data_Rebin = new TH1D("hLeadingJetPt_data_Rebin","hLeadingJetPt_data_Rebin",8,binlim);
  TH1D *hLeadingJetPt_mc_Rebin = new TH1D("hLeadingJetPt_mc_Rebin","hLeadingJetPt_mc_Rebin",8,binlim);

  for(int ibin=0;ibin<7;ibin++){
    hLeadingJetPt_data_Rebin->SetBinContent(ibin+1,hLeadingJetPt_data->GetBinContent(ibin+1));
    hLeadingJetPt_data_Rebin->SetBinError(ibin+1,hLeadingJetPt_data->GetBinError(ibin+1));
    hLeadingJetPt_mc_Rebin->SetBinContent(ibin+1,hLeadingJetPt_mc->GetBinContent(ibin+1));
    hLeadingJetPt_mc_Rebin->SetBinError(ibin+1,hLeadingJetPt_mc->GetBinError(ibin+1));
  }

  float bin7Content_data = hLeadingJetPt_data->GetBinContent(7)+hLeadingJetPt_data->GetBinContent(8);
  float bin7Error_data = sqrt(hLeadingJetPt_data->GetBinError(7)*hLeadingJetPt_data->GetBinError(7)+hLeadingJetPt_data->GetBinError(8)*hLeadingJetPt_data->GetBinError(8));
  float bin8Content_data = hLeadingJetPt_data->GetBinContent(9)+hLeadingJetPt_data->GetBinContent(10);
  float bin8Error_data = sqrt(hLeadingJetPt_data->GetBinError(9)*hLeadingJetPt_data->GetBinError(9)+hLeadingJetPt_data->GetBinError(10)*hLeadingJetPt_data->GetBinError(10));

  float bin7Content_mc = hLeadingJetPt_mc->GetBinContent(7)+hLeadingJetPt_mc->GetBinContent(8);
  float bin7Error_mc = sqrt(hLeadingJetPt_mc->GetBinError(7)*hLeadingJetPt_mc->GetBinError(7)+hLeadingJetPt_mc->GetBinError(8)*hLeadingJetPt_mc->GetBinError(8));
  float bin8Content_mc = hLeadingJetPt_mc->GetBinContent(9)+hLeadingJetPt_mc->GetBinContent(10);
  float bin8Error_mc = sqrt(hLeadingJetPt_mc->GetBinError(9)*hLeadingJetPt_mc->GetBinError(9)+hLeadingJetPt_mc->GetBinError(10)*hLeadingJetPt_mc->GetBinError(10));


  hLeadingJetPt_data_Rebin->SetBinContent(7,bin7Content_data);
  hLeadingJetPt_data_Rebin->SetBinError(7,bin7Error_data);
  hLeadingJetPt_data_Rebin->SetBinContent(8,bin8Content_data);
  hLeadingJetPt_data_Rebin->SetBinError(8,bin8Error_data);

  hLeadingJetPt_mc_Rebin->SetBinContent(7,bin7Content_mc);
  hLeadingJetPt_mc_Rebin->SetBinError(7,bin7Error_mc);
  hLeadingJetPt_mc_Rebin->SetBinContent(8,bin8Content_mc);
  hLeadingJetPt_mc_Rebin->SetBinError(8,bin8Error_mc);



  for(int ibin=0;ibin<hLeadingJetPt_data_Rebin->GetNbinsX();ibin++){
    hLeadingJetPt_data_Rebin->SetBinContent(ibin+1, hLeadingJetPt_data_Rebin->GetBinContent(ibin+1)/hLeadingJetPt_data_Rebin->GetBinWidth(ibin+1));
    hLeadingJetPt_data_Rebin->SetBinError(ibin+1, hLeadingJetPt_data_Rebin->GetBinError(ibin+1)/hLeadingJetPt_data_Rebin->GetBinWidth(ibin+1));    
  }

  for(int ibin=0;ibin<hLeadingJetPt_mc_Rebin->GetNbinsX();ibin++){
    hLeadingJetPt_mc_Rebin->SetBinContent(ibin+1, hLeadingJetPt_mc_Rebin->GetBinContent(ibin+1)/hLeadingJetPt_mc_Rebin->GetBinWidth(ibin+1));
    hLeadingJetPt_mc_Rebin->SetBinError(ibin+1, hLeadingJetPt_mc_Rebin->GetBinError(ibin+1)/hLeadingJetPt_mc_Rebin->GetBinWidth(ibin+1));    
  }

  // normalization should be matched with what's in ANA
  hLeadingJetPt_data_Rebin->Scale(1./hLeadingJetPt_data_Rebin->Integral());
  hLeadingJetPt_mc_Rebin->Scale(1./hLeadingJetPt_mc_Rebin->Integral());



  cout<<"Bin Width = "<<hLeadingJetPt_data->GetBinWidth(1)<<endl;
  cout<<"Number of bins = "<<hLeadingJetPt_data->GetNbinsX()<<endl;
  cout<<"Max Bin Edge "<<hLeadingJetPt_data->GetBinLowEdge(20)+hLeadingJetPt_data->GetBinWidth(1)<<endl;

 // data, mc styling
  hLeadingJetPt_mc_Rebin->SetLineColor(kBlue);
  hLeadingJetPt_mc_Rebin->SetFillColor(kAzure-8);
  hLeadingJetPt_mc_Rebin->SetFillStyle(3005);

  hLeadingJetPt_mc_Rebin->GetXaxis()->SetLabelSize(22);
  hLeadingJetPt_mc_Rebin->GetXaxis()->SetLabelFont(43);
  hLeadingJetPt_mc_Rebin->GetXaxis()->SetTitleSize(24);
  hLeadingJetPt_mc_Rebin->GetXaxis()->SetTitleFont(43);
  hLeadingJetPt_mc_Rebin->GetXaxis()->SetTitleOffset(1.7);
  hLeadingJetPt_mc_Rebin->GetXaxis()->CenterTitle();

  //hLeadingJetPt_mc_Rebin->GetXaxis()->SetNdivisions(905,true);

  //hLeadingJetPt_mc_Rebin->SetYTitle("dN_{jet}/dp_{T} (GeV/c)^{-1}, Arb. Norm.");
  //hLeadingJetPt_mc_Rebin->GetYaxis()->CenterTitle(0);
  //hLeadingJetPt_mc_Rebin->SetYTitle("Arbitrary Normalization");

  hLeadingJetPt_mc_Rebin->GetYaxis()->SetLabelSize(22);
  hLeadingJetPt_mc_Rebin->GetYaxis()->SetLabelFont(43);
  hLeadingJetPt_mc_Rebin->GetYaxis()->SetTitleSize(22);
  hLeadingJetPt_mc_Rebin->GetYaxis()->SetTitleFont(43);
  hLeadingJetPt_mc_Rebin->GetYaxis()->SetTitleOffset(2.4);
  hLeadingJetPt_mc_Rebin->GetYaxis()->CenterTitle();


  //hDum->Draw("hist");

  hLeadingJetPt_mc_Rebin->SetAxisRange(2e-4,3,"Y");



  hLeadingJetPt_mc_Rebin->GetXaxis()->SetNdivisions(505);
  hLeadingJetPt_mc_Rebin->Draw("hist");
  hLeadingJetPt_data_Rebin->Draw("pzsame");


  // Legend
    TLegend *t3a=new TLegend(0.59,0.63,0.98,0.83);
  //t3a->SetHeader("ant-k_{T} (R=0.5) CaloJets");
  t3a->AddEntry(hLeadingJetPt_data_Rebin,"pp  #sqrt{s}=7.0 TeV","p");
  t3a->AddEntry(hLeadingJetPt_mc_Rebin,"PYTHIA","lf");
  t3a->SetFillColor(0);
  t3a->SetBorderSize(0);
  t3a->SetFillStyle(0);
  t3a->SetTextFont(63);
  t3a->SetTextSize(15);
  t3a->Draw();


  // other labeling
  
  
  TLatex *lumi_pp = new TLatex(240,1.,"#intL dt = 35.1 pb^{-1}");
  lumi_pp->SetTextFont(63);
  lumi_pp->SetTextSize(15);
  lumi_pp->Draw();

  
  TLatex *jetf_pp;
  jetf_pp = new TLatex(235,0.05,"Anti-k_{T}, R=0.5");
  jetf_pp->SetTextFont(63);
  jetf_pp->SetTextSize(15);
  jetf_pp->Draw();


}
