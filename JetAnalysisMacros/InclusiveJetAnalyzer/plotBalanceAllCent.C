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

#include "weightMix.C"

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

void plotPPBalanceAll();

void drawText(const char *text, float xp, float yp);

//--------------------------------------------------------------
// drawPatch() is a crazy way of removing 0 in the second and third 
// pad which is partially shown due to no margin between the pads
// if anybody has a better way of doing it let me know! - Andre
//--------------------------------------------------------------
void drawPatch(float x1, float y1, float x2, float y2); 
//---------------------------------------------------------------------

void plotBalanceAllCent(){

	TH1::SetDefaultSumw2();
	
   TCanvas *c1 = new TCanvas("c1","",1050,700);
   makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.2,0.2,0.02);

 
  c1->cd(1);
  //plotBalance(-1,"data.root","pythia.root","mix.root",true,false,false);
  //drawText(" 0-100%",0.70,0.1);
  //drawPatch(0.05,0.0,0.2,0.05);
  //  if(1)plotPPBalanceAll();


  plotBalance(5,"data.root","pythia.root","mix.root",true,false,false);
  drawText("70-100%",0.7,0.1);
  drawText("(a)",0.25,0.885);

  c1->cd(2);
  plotBalance(4,"data.root","pythia.root","mix.root",true,false,true);
  drawText("50-70%",0.7,0.1);
  drawText("(b)",0.05,0.885);


  TLatex *jetf_PbPb;
  jetf_PbPb = new TLatex(0.477,0.14,"Anti-k_{T} (PFlow), R=0.3");
  jetf_PbPb->SetTextFont(63);
  jetf_PbPb->SetTextSize(15);
  jetf_PbPb->Draw();

  c1->cd(3);
  plotBalance(3,"data.root","pythia.root","mix.root",true,false,false);
  drawText("30-50%",0.7,0.1);
  drawText("(c)",0.05,0.885);

  TLatex tsel;
  tsel.SetNDC();
  tsel.SetTextFont(63);
  tsel.SetTextSize(15);
  tsel.DrawLatex(0.55,0.75,Form("p_{T,1} > %d GeV/c",leadCut));
  tsel.DrawLatex(0.55,0.65,Form("p_{T,2} > %d GeV/c",subleadCut));
  tsel.DrawLatex(0.55,0.55,"#Delta#phi_{12} > #frac{2}{3}#pi");



  c1->cd(4);
  plotBalance(2,"data.root","pythia.root","mix.root",true,false,false);
  drawText("20-30%",0.7,0.3);
  drawText("(d)",0.25,0.92);
  //drawPatch(0.05,0.972,0.2,1.1);
  //drawPatch(0.9,0.0972,1.12,0.191);

  //  gPad->SetBottomMargin(0.22);                                                                                                                     

  c1->cd(5);
  plotBalance(1,"data.root","pythia.root","mix.root",true,true,false);
  drawText("10-20%",0.7,0.3);
  drawText("(e)",0.05,0.92);
  //drawPatch(-0.00007,0.0972,0.128,0.195);
  //drawPatch(0.9,0.0972,1.12,0.191);
  //drawPatch(0.976,0.0972,1.1,0.141);
  //  gPad->SetBottomMargin(0.22);                                                                                                                     
  if(sideCorrect == 0){
    jetf_PbPb = new TLatex(0.05,0.21,"Mismatched background not subtracted");
    jetf_PbPb->Draw();
  }

  c1->cd(6);
  plotBalance(0,"data.root","pythia.root","mix.root",true,false,false);
  drawText("0-10%",0.7,0.3);
  drawText("(f)",0.05,0.92);
  //drawPatch(-0.00007,0.0972,0.0518,0.141);
  //drawPatch(-0.00007,0.0972,0.128,0.195);

  c1->cd(3);
        
  TLatex *cms = new TLatex(0.34,0.23,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(17);
  cms->Draw();

  if(!plotSubtraction){
    if(sideCorrect > 0){
      c1->Print(Form("./fig/dijet_imbalance_lead%d_sub%d_all_cent_20120103_subt.gif",leadCut,subleadCut));
      c1->Print(Form("./fig/dijet_imbalance_lead%d_sub%d_all_cent_20120103_subt.eps",leadCut,subleadCut));
      c1->Print(Form("./fig/dijet_imbalance_lead%d_sub%d_all_cent_20120103_subt.pdf",leadCut,subleadCut));
    }else{
      c1->Print(Form("./fig/dijet_imbalance_lead%d_sub%d_cent_20101126_v0.gif",leadCut,subleadCut));
      c1->Print(Form("./fig/dijet_imbalance_lead%d_sub%d_all_cent_20101126_v0.eps",leadCut,subleadCut));
      c1->Print(Form("./fig/dijet_imbalance_lead%d_sub%d_all_cent_20101126_v0.pdf",leadCut,subleadCut));
    }
  }
}

void plotBalance(int cbin,
		 TString infname,
		 TString pythia,
		 TString mix,
		 bool useWeight,
		 bool drawXLabel,
		 bool drawLeg)
{

  //  useWeight = 0;

  TString cut=Form("pt1>%d && pt2>%d && abs(dphi)>2.0944 && abs(eta1) < 2 && abs(eta2) < 2",leadCut,subleadCut);
  TString cutpp = cut;

  TString side=Form("pt1>%d && pt2>%d && abs(dphi)>%f && abs(dphi)<%f && abs(eta1) < 2 && abs(eta2) < 2",leadCut,subleadCut, sideMin, sideMax);

  TString cutNorm=Form("pt1>%d && abs(eta1) < 2",leadCut);

  cout<<"Side band : "<<side.Data()<<endl;

  double sideScale = sideCorrect*(3.1415926536-2.0944)/(sideMax-sideMin);

  TString cstring = "";
  if(cbin==-1) {
     cstring = "0-100%";
     cut+=" && bin>=0 && bin<40";
     side+=" && bin>=0 && bin<40";
     cutNorm+=" && bin>=0 && bin<40";

  }
  else if(cbin==0) {
     cstring = "0-10%";
     cut+=" && bin>=0 && bin<4";
     side+=" && bin>=0 && bin<4";
     cutNorm+=" && bin>=0 && bin<4";

  } else if (cbin==1) {
     cstring = "10-20%";
     cut+=" && bin>=4 && bin<8";
     side+=" && bin>=4 && bin<8";
     cutNorm+=" && bin>=4 && bin<8";


  } else if (cbin==2) {
     cstring = "20-30%";
     cut+=" && bin>=8 && bin<12";
     side+=" && bin>=8 && bin<12";
     cutNorm+=" && bin>=8 && bin<12";

  } else if (cbin==3) {
     cstring = "30-50%";
     cut+=" && bin>=12 && bin<20";
     side+=" && bin>=12 && bin<20";
     cutNorm+=" && bin>=12 && bin<20";

  }else if (cbin==4) {
    cstring = "50-70%";
    cut+=" && bin>=20 && bin<28";
    side+=" && bin>=20 && bin<28";
    cutNorm+=" && bin>=20 && bin<28";

  }else if (cbin==5) {
     cstring = "70-100%";
     cut+=" && bin>=28";
     side+=" && bin>=28";
     cutNorm+=" && bin>=28";

  }
  
  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");

  //  TChain* nt = new TChain("nt");
  //  nt->Add("/Users/yetkinyilmaz/analysis/data2011/dijet20111215/Hi*.root");

  // open the pythia (MC) file
  TFile *infPythia = new TFile(pythia.Data());
  TTree *ntPythia = (TTree*) infPythia->FindObjectAny("nt");

  // open the datamix file
  TFile *infMix = new TFile(mix.Data());
  TTree *ntMix =(TTree*)infMix->FindObjectAny("nt");
  TFile *infW = new TFile("weights.root");
  TTree *ntw =(TTree*)infW->FindObjectAny("ntw");
  ntMix->AddFriend(ntw);

  int Nbin = 25;

  if(plotSubtraction) Nbin = 50;

  // projection histogram

  TH1D *hFull = new TH1D("hFull","",Nbin,0,1.5);
  TH1D *hPythiaFull = new TH1D("hPythiaFull","",Nbin,0,1.5);
  TH1D *hDataMixFull = new TH1D("hDataMixFull","",Nbin,0,1.5);

  TH1D *h = new TH1D("h","",Nbin,0,1.5);
  TH1D *hPythia = new TH1D("hPythia","",Nbin,0,1.5);
  TH1D *hDataMix = new TH1D("hDataMix","",Nbin,0,1.5);

  TH1D *hB = new TH1D(Form("hB",cbin),"",Nbin,0,1.5);
  TH1D *hPythiaB = new TH1D(Form("hPythiaB",cbin),"",Nbin,0,1.5);
  TH1D *hDataMixB = new TH1D(Form("hDataMixB",cbin),"",Nbin,0,1.5);

  hB->SetLineStyle(2);
  hPythiaB->SetLineStyle(2);
  hDataMixB->SetLineStyle(2);

  TH1D* hNorm = new TH1D("hNorm","",1000,0,1000);
  TH1D* hNormPythia = new TH1D("hNormPythia","",1000,0,1000);
  TH1D* hNormDataMix = new TH1D("hNormDataMix","",1000,0,1000);

  if(0){
  ntMix->SetAlias("pt1","et1");
  ntMix->SetAlias("pt2","et2");
  }

  ntPythia->SetAlias("pt1","et1");
  ntPythia->SetAlias("pt2","et2");
  nt->SetAlias("adphi","acos(cos(phi1-phi2))");
  ntMix->SetAlias("adphi","abs(dphi)");
  ntPythia->SetAlias("adphi","abs(dphi)");

  nt->Draw("(pt1-pt2)/(pt1+pt2)>>hFull",Form("%s && noise < 0",cut.Data())); 
  nt->Draw("(pt1-pt2)/(pt1+pt2)>>hB",Form("%s && noise < 0",side.Data()));
  nt->Draw("pt1>>hNorm",Form("%s && noise < 0",cutNorm.Data()));
   
  if (useWeight) {
    // use the weight value caluculated by Matt's analysis macro
    ntMix->Draw("(pt1-pt2)/(pt1+pt2)>>hDataMixFull",Form("(%s)*weight",cut.Data())); 
    ntMix->Draw("(pt1-pt2)/(pt1+pt2)>>hDataMixB",Form("(%s)*weight",side.Data()));
    ntMix->Draw("pt1>>hNormDataMix",Form("(%s)*(%s)","weight",cutNorm.Data()));
  } else {
    // ignore centrality reweighting
    ntMix->Draw("(pt1-pt2)/(pt1+pt2)>>hDataMixFull",Form("(%s)",cut.Data()));  
    ntMix->Draw("(pt1-pt2)/(pt1+pt2)>>hDataMixB",Form("(%s)",side.Data()));
    ntMix->Draw("pt1>>hNormDataMix",Form("%s",cutNorm.Data()));
  }
  ntPythia->Draw("(pt1-pt2)/(pt1+pt2)>>hPythiaFull",Form("(%s)",cutpp.Data()));
  ntPythia->Draw("(pt1-pt2)/(pt1+pt2)>>hPythiaB",Form("(%s)",side.Data()));

  hDataMixB->Scale(sideScale);
  hB->Scale(sideScale);
  hPythiaB->Scale(sideScale);

  hDataMix->Add(hDataMixFull);
  h->Add(hFull);
  hPythia->Add(hPythiaFull);

  hDataMix->Add(hDataMixB,-1);
  h->Add(hB,-1);
  hPythia->Add(hPythiaB,-1);
  hB->SetFillStyle(3005);
  hB->SetFillColor(15);

  if(plotSubtraction){
    TCanvas* c1 = new TCanvas("cc1","",500,500);
    if(0){
      
    }

    hFull->SetTitle(";A_{J};Entries");
    hFull->SetMaximum(750);
    hFull->SetAxisRange(0,1);
    hFull->GetXaxis()->CenterTitle();
    hFull->GetYaxis()->CenterTitle();
    
    hFull->Draw("hist");

    hB->Draw("hist same");
    TH1D* hPlot = ((TH1D*)h->Clone(Form("%s_plot",h->GetName())));
    hPlot->SetMarkerStyle(24);
    hPlot->SetLineColor(2);
    hPlot->SetMarkerColor(2);
    hPlot->SetMarkerSize(0);
    hPlot->Draw("hist same");
    hPlot->Draw("same");
    
    TLegend *tt3=new TLegend(0.6,0.55,0.94,0.95);
    if(cbin == 0) tt3->AddEntry(h,"0-10%","");
    if(cbin == 1) tt3->AddEntry(h,"10-20%","");
    if(cbin == 2) tt3->AddEntry(h,"20-30%","");
    if(cbin == 3) tt3->AddEntry(h,"30-50%","");
    if(cbin == 4) tt3->AddEntry(h,"50-70%","");
    if(cbin == 5) tt3->AddEntry(h,"70-100%","");

    tt3->AddEntry(hFull,Form("p_{T,1} > %d GeV/c",leadCut),"");
    tt3->AddEntry(hFull,Form("p_{T,2} > %d GeV/c",subleadCut),"");
    tt3->AddEntry(hFull,"#Delta#phi_{12} > #frac{2}{3}#pi","l");
    tt3->AddEntry(hB,"#Delta#phi_{12} < #frac{1}{3}#pi","lf");
    tt3->AddEntry(hPlot,"Subtracted A_{J}","l");
    
    tt3->SetFillColor(0);
    tt3->SetBorderSize(0);
    tt3->SetFillStyle(0);
    tt3->SetTextFont(63);
    tt3->SetTextSize(15);
    tt3->Draw();

    TLatex *cms = new TLatex(0.04,700,"CMS Preliminary");
    cms->SetTextFont(63);
    cms->SetTextSize(17);
    cms->Draw();
    
  if(0){
    c1->cd(2);
  hDataMixFull->Draw("hist");
  hDataMixB->Draw("hist same");
  hPlot = ((TH1D*)hDataMix->Clone(Form("%s_plot",hDataMix->GetName())));
  hPlot->SetMarkerStyle(24);
  hPlot->SetLineColor(2);
  hPlot->SetMarkerColor(2);
  hPlot->Draw("same");
  }

    c1->Print(Form("AJ_SideBand_Subtraction_lead%d_sub%d_centrality%d.C",leadCut,subleadCut,cbin));
    c1->Print(Form("AJ_SideBand_Subtraction_lead%d_sub%d_centrality%d.pdf",leadCut,subleadCut,cbin));
    c1->Print(Form("AJ_SideBand_Subtraction_lead%d_sub%d_centrality%d.eps",leadCut,subleadCut,cbin));
    c1->Print(Form("AJ_SideBand_Subtraction_lead%d_sub%d_centrality%d.gif",leadCut,subleadCut,cbin));

  new TCanvas();
  }


  // calculate the statistical error and normalize
  h->SetLineColor(dataColor);
  h->SetMarkerColor(dataColor);
  h->Sumw2();
  if(normLead){
    h->Scale(1./hNorm->Integral());
    hB->Scale(1./hNorm->Integral());
  }else{
    hB->Scale(1./h->Integral());
    h->Scale(1./h->Integral());
  }
  h->SetMarkerStyle(20);

  hPythia->Scale(1./hNormPythia->Integral());
  hPythia->SetLineColor(kBlue);
  hPythia->SetFillColor(kAzure-8);
  hPythia->SetFillStyle(3005);

  if(normLead){
    hDataMixB->Scale(1./hNormDataMix->Integral());
    hDataMix->Scale(1./hNormDataMix->Integral());
  }else{
    hDataMixB->Scale(1./hDataMix->Integral());
    hDataMix->Scale(1./hDataMix->Integral());
  }
  hDataMix->SetLineColor(mixColor);
  hDataMix->SetFillColor(mixColor);
  hDataMix->SetFillStyle(3004);
  
  hDataMix->SetMarkerSize(0);
  hDataMix->SetStats(0);

  hDataMix->GetXaxis()->SetLabelSize(22);
  hDataMix->GetXaxis()->SetLabelFont(43);
  hDataMix->GetXaxis()->SetTitleSize(28);
  hDataMix->GetXaxis()->SetTitleFont(43);
  hDataMix->GetXaxis()->SetTitleOffset(2.2);
  hDataMix->GetXaxis()->CenterTitle();
  
  
  hDataMix->GetYaxis()->SetLabelSize(22);
  hDataMix->GetYaxis()->SetLabelFont(43);
  hDataMix->GetYaxis()->SetTitleSize(28);
  hDataMix->GetYaxis()->SetTitleFont(43);
  hDataMix->GetYaxis()->SetTitleOffset(2.2);
  hDataMix->GetYaxis()->CenterTitle();
  
  if(drawXLabel) hDataMix->SetXTitle("A_{J} = (p_{T,1}-p_{T,2})/(p_{T,1}+p_{T,2})");  
  hDataMix->SetYTitle("Event Fraction");
  if(cbin==2)hDataMix->SetAxisRange(0,0.999,"X");
  else hDataMix->SetAxisRange(0.001,0.999,"X");

  if(cbin==2)hDataMix->SetAxisRange(0,0.26,"Y");
  else hDataMix->SetAxisRange(0.0001,0.26,"Y");

  //hDataMix->GetXaxis()->SetNdivisions(905,true);
  hDataMix->GetYaxis()->SetNdivisions(505,true);
  
 
	hDataMix->Draw();//"hist");
	hDataMix->Draw("hist same");
	//hPythia->Draw("hist");  

	h->SetLineWidth(1);
	h->Draw("same");
	h->SetLineWidth(2);
	h->Draw("same");
	//	hDataMixB->Draw("same hist");
	//        hB->Draw("same hist");

  cout<<" mean value of data "<<h->GetMean()<<endl;

  if(drawLeg){
    TLegend *t3=new TLegend(0.34,0.6,0.83,0.95); 

    t3->AddEntry(h,Form("%s #mub^{-1}",LUM),"");
    t3->AddEntry(h,"PbPb  #sqrt{s}_{_{NN}}=2.76 TeV","p");
    //    t3->AddEntry(hB,"Estimated Background","l");
    t3->AddEntry(hDataMix,"PYTHIA+HYDJET 1.8","lf");

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


void plotPPBalanceAll(){

  bool isPF = false;

  TString data_tag;
  TString mc_tag;
  TString jetfinder, jetfinder_tag;

  if(!isPF){
    data_tag = "hdata_ak5calo_DijetBalance";
    mc_tag = "hmc_ak5calo_DijetBalance_histonly";
    jetfinder_tag = "calo";
  }else{
    data_tag = "hdata_ak5pf_DijetBalance";
    mc_tag = "hmc_ak5pf_DijetBalance_histonly";
    jetfinder_tag ="pf";
  }

  TFile *fDATA = new TFile(Form("./pp/%s.root",data_tag.Data()));
  TFile *fMC = new TFile(Form("./pp/%s.root",mc_tag.Data()));

  TH1F *hDijetBal_data = (TH1F*) fDATA->Get("hDataDijetBalance");
  TH1F *hDijetBal_mc = (TH1F*) fMC->Get("hQCDDijetBalance");


  // normalization should be matched with what's in ANA
  hDijetBal_data->Scale(1./hDijetBal_data->Integral());
  hDijetBal_data->Rebin(2);

  hDijetBal_mc->Scale(1./hDijetBal_mc->Integral());
  hDijetBal_mc->Rebin(2);

  cout<<"Bin Width, pp = "<<hDijetBal_data->GetBinWidth(1)<<endl;
  cout<<"# of bins, pp = "<<hDijetBal_data->GetNbinsX()<<endl;
  cout<<"Max Bin Center, pp = "<<hDijetBal_data->GetBinCenter(25)<<endl;


  // canvas setting ---
  //  TCanvas *c1 = new TCanvas("c1","",490,530);

  // dum styling ----
  /*
  TH1F *hDum = new TH1F("hDum","",10,0,1.0);
  hDum->SetLineColor(kBlue);
  hDum->SetFillColor(kAzure-8);
  hDum->SetFillStyle(3005);
  */
  hDijetBal_mc->SetStats(0);
  hDijetBal_mc->SetXTitle("A_{J} = (E_{T}^{j1}-E_{T}^{j2})/(E_{T}^{j1}+E_{T}^{j2})");
  hDijetBal_mc->SetYTitle("Event Fraction");

  hDijetBal_mc->GetXaxis()->SetLabelSize(22);
  hDijetBal_mc->GetXaxis()->SetLabelFont(43);
  hDijetBal_mc->GetXaxis()->SetTitleSize(24);
  hDijetBal_mc->GetXaxis()->SetTitleFont(43);
  hDijetBal_mc->GetXaxis()->SetTitleOffset(2.4);
  hDijetBal_mc->GetXaxis()->CenterTitle();

  hDijetBal_mc->GetXaxis()->SetNdivisions(905,true);
  hDijetBal_mc->GetYaxis()->SetNdivisions(505,true);

  hDijetBal_mc->GetYaxis()->SetLabelSize(22);
  hDijetBal_mc->GetYaxis()->SetLabelFont(43);
  hDijetBal_mc->GetYaxis()->SetTitleSize(22);
  hDijetBal_mc->GetYaxis()->SetTitleFont(43);
  hDijetBal_mc->GetYaxis()->SetTitleOffset(3.);
  hDijetBal_mc->GetYaxis()->CenterTitle();

  hDijetBal_mc->SetAxisRange(0.0001,0.26,"Y");
  hDijetBal_mc->SetAxisRange(0.0001,1.0,"X");

 

  // data, mc styling
  hDijetBal_mc->SetLineColor(kBlue);
  hDijetBal_mc->SetFillColor(kAzure-8);
  hDijetBal_mc->SetFillStyle(3005);

	hDijetBal_mc->Draw();

	hDijetBal_mc->Draw("hist");
  hDijetBal_data->Draw("pzsame");


  // Legend
    TLegend *t3a=new TLegend(0.57,0.6,0.89,0.8); 
  //t3a->SetHeader("ant-k_{T} (R=0.5) CaloJets");
  t3a->AddEntry(hDijetBal_data,"pp  #sqrt{s}=7.0 TeV","p");
  t3a->AddEntry(hDijetBal_mc,"PYTHIA","lf");
  t3a->SetFillColor(0);
  t3a->SetBorderSize(0);
  t3a->SetFillStyle(0);
  t3a->SetTextFont(63);
  t3a->SetTextSize(15);
  t3a->Draw();


  // other labeling
  TLatex *cms = new TLatex(0.34,0.23,"CMS");
  cms->SetTextFont(63);
  cms->SetTextSize(17);
  cms->Draw();

  TLatex *lumi_pp = new TLatex(0.50,0.23,"#intL dt = 35.1 pb^{-1}");
  lumi_pp->SetTextFont(63);
  lumi_pp->SetTextSize(15);
  lumi_pp->Draw();

  
  TLatex *jetf_pp;
  if(!isPF) jetf_pp = new TLatex(0.477,0.14,"Anti-k_{T}, R=0.5");
  else jetf_pp = new TLatex(0.477,0.14,"anti-k_{T} (R=0.5) PFJets");
  jetf_pp->SetTextFont(63);
  jetf_pp->SetTextSize(15);
  jetf_pp->Draw();


}
