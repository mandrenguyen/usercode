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

int type = 3;

bool central = 1;

//---------------------------------------------------------------------
void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns, 
			  const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0., 
			  const Float_t leftMargin=0.2, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);

TString plotBalance(int cbin = 0,
		 TString infname = "data.root",
		 TString pythia = "pythia.root",
		 TString mix = "mix.root",
		 bool useWeight = true,
		 bool drawXLabel = false,
		    bool drawLeg = false,
		    TFile* outf = 0
		    );

void plotPPBalanceAll();

void drawText(const char *text, float xp, float yp);

//--------------------------------------------------------------
// drawPatch() is a crazy way of removing 0 in the second and third 
// pad which is partially shown due to no margin between the pads
// if anybody has a better way of doing it let me know! - Andre
//--------------------------------------------------------------
void drawPatch(float x1, float y1, float x2, float y2); 
//---------------------------------------------------------------------

void plotBalanceAllPt(){

	TH1::SetDefaultSumw2();

	TFile* outf = new TFile("hists_AJ.root","recreate");
	
   TCanvas *c1 = new TCanvas("c1","",1050,700);
   makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.2,0.2,0.02);
 
   TString label;
  c1->cd(1);
  label = plotBalance(0,"data.root","pythia.root","mix.root",true,false,true,outf);
  drawText(label.Data(),0.4,0.42);


  TLatex *jetf_PbPb;
  jetf_PbPb = new TLatex(0.477,0.18,"Anti-k_{T} (PFlow), R=0.3");

  jetf_PbPb->SetTextFont(63);
  jetf_PbPb->SetTextSize(15);
  jetf_PbPb->Draw();

  TLatex *lumi_PbPb = new TLatex(0.50,0.23,"84 #mub^{-1}");
  lumi_PbPb->SetTextFont(63);
  lumi_PbPb->SetTextSize(15);
//  lumi_PbPb->Draw();

  c1->cd(2);
  label = plotBalance(1,"data.root","pythia.root","mix.root",true,false,false,outf);
  drawText(label.Data(),0.3,0.42);

  if(sideCorrect == 0){
    jetf_PbPb = new TLatex(0.12,0.29,"Mismatched background not subtracted");
    jetf_PbPb->Draw();
  }

  TLatex tsel;
  tsel.SetNDC();
  tsel.SetTextFont(63);
  tsel.SetTextSize(15);
  tsel.DrawLatex(0.55,0.75,Form("p_{T,1} > %d GeV/c",leadCut));
  tsel.DrawLatex(0.55,0.65,Form("p_{T,2} > %d GeV/c",subleadCut));
  tsel.DrawLatex(0.55,0.55,"#Delta#phi_{12} > #frac{2}{3}#pi");


  c1->cd(3);
  label = plotBalance(2,"data.root","pythia.root","mix.root",true,false,false,outf);
  drawText(label.Data(),0.3,0.42);
  drawText(Form("0-%d%s",(int)(2.5*centralBin),"%"),0.64,0.6);

  c1->cd(4);
  label = plotBalance(3,"data.root","pythia.root","mix.root",true,false,false,outf);
  drawText(label.Data(),0.4,0.9);
  //  drawText("(e)",0.05,0.92);
  //drawPatch(-0.00007,0.0972,0.128,0.195);
  //drawPatch(0.9,0.0972,1.12,0.191);
  //drawPatch(0.976,0.0972,1.1,0.141);
  //  gPad->SetBottomMargin(0.22);                                                                                                                     
  c1->cd(5);
  label = plotBalance(4,"data.root","pythia.root","mix.root",true,true,false,outf);
  drawText(label.Data(),0.3,0.9);
  //  drawText("(f)",0.05,0.92);
  //drawPatch(-0.00007,0.0972,0.0518,0.141);
  //drawPatch(-0.00007,0.0972,0.128,0.195);
        
  c1->cd(6);
  label = plotBalance(5,"data.root","pythia.root","mix.root",true,false,false,outf);
  drawText(label.Data(),0.3,0.9);
  //  drawText("(f)",0.05,0.92);

  c1->cd(3);

  TLatex *cms = new TLatex(0.54,0.28,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(17);
  cms->Draw();

  outf->Write();

  if(!plotSubtraction){
    c1->Print(Form("./fig/dijet_imbalance%d_0to%d_pt_20120103%s%s.pdf",type,(int)(2.5*centralBin),sideCorrect > 0?"_subt":"",central?"":"_peripheral"));
    c1->Print(Form("./fig/dijet_imbalance%d_0to%d_pt_20120103%s%s.eps",type,(int)(2.5*centralBin),sideCorrect > 0?"_subt":"",central?"":"_peripheral"));
    c1->Print(Form("./fig/dijet_imbalance%d_0to%d_pt_20120103%s%s.gif",type,(int)(2.5*centralBin),sideCorrect > 0?"_subt":"",central?"":"_peripheral"));
    
  }

}

TString plotBalance(int cbin,
		 TString infname,
		 TString pythia,
		 TString mix,
		 bool useWeight,
		 bool drawXLabel,
		    bool drawLeg,
		    TFile* outf)
{

  //  useWeight = 0;// forced!

  TString cut=Form("pt1>%d && pt2>%d && abs(dphi)>2.0944 && abs(eta1) < 1.5 && abs(eta2) < 1.5",leadCut,subleadCut);
  TString side=Form("pt1>%d && pt2>%d && abs(dphi)>%f && abs(dphi)<%f && abs(eta1) < 1.5 && abs(eta2) < 1.5",leadCut,subleadCut, sideMin, sideMax);

  TString cutNorm=Form("pt1>%d && abs(eta1) < 1.5",leadCut);

  double sideScale = sideCorrect*(3.1415926536-2.0944)/(sideMax-sideMin);

  TString cutpp=cut;
  TString cstring = "";
  if(central){
    cut+=Form(" && bin>=0 && bin<%d",centralBin);
    cutNorm+=Form(" && bin>=0 && bin<%d",centralBin);
  }else{
    cut+=Form(" && bin>=%d",centralBin);
    cutNorm+=Form(" && bin>=%d",centralBin);
  }

  double m[] =   {120,150,180,230,260,300,1500};

  int ptMin = 0;
  int ptMax = 0;
  if(cbin==-1) {
    ptMin = m[0];
    ptMax = 999;
  } else if (cbin==0) {
    ptMin = m[0];
    ptMax = m[1];
  } else if (cbin==1) {
    ptMin = m[1];
    ptMax = m[2];
  } else if (cbin==2) {
    ptMin = m[2];
    ptMax = m[3];
  } else if (cbin==3) {
    ptMin = m[3];
    ptMax = m[4]; 
  } else if (cbin==4) {
    ptMin = m[4];
    ptMax = m[5];
  } else if (cbin==5) {
    ptMin = m[5];
    ptMax = 999;
  }

  cstring = Form("%d < p_{T} < %d GeV/c",ptMin,ptMax);
  cut+=Form(" && pt1>=%d && pt1<%d",ptMin,ptMax);
  side+=Form(" && pt1>=%d && pt1<%d",ptMin,ptMax);
  cutNorm+=Form(" && pt1>=%d && pt1<%d",ptMin,ptMax);

  if (cbin==5) {
    cstring = Form("p_{T} > %d GeV/c",ptMin);
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

  double max = 1.5;
  if(type == 0) max = 300;

  // projection histogram
  TH1D *h = new TH1D(Form("h",cbin),"",Nbin,0,max);
  TH1D *hPythia = new TH1D(Form("hPythia",cbin),"",Nbin,0,max);
  TH1D *hDataMix = new TH1D(Form("hDataMix",cbin),"",Nbin,0,max);

  TH1D *hB = new TH1D(Form("hB",cbin),"",Nbin,0,max);
  TH1D *hPythiaB = new TH1D(Form("hPythiaB",cbin),"",Nbin,0,max);
  TH1D *hDataMixB = new TH1D(Form("hDataMixB",cbin),"",Nbin,0,max);

  TH1D *hFull = new TH1D("hFull","",25,0,max);
  TH1D *hPythiaFull = new TH1D("hPythiaFull","",25,0,max);
  TH1D *hDataMixFull = new TH1D("hDataMixFull","",25,0,max);

  TH1D* hNorm = new TH1D("hNorm","",1000,0,1000);
  TH1D* hNormPythia = new TH1D("hNormPythia","",1000,0,1000);
  TH1D* hNormDataMix = new TH1D("hNormDataMix","",1000,0,1000);

  hB->SetLineStyle(2);
  hPythiaB->SetLineStyle(2);
  hDataMixB->SetLineStyle(2);

  ntMix->SetAlias("pt1","et1");
  ntPythia->SetAlias("pt1","et1");
  ntMix->SetAlias("pt2","et2");
  ntPythia->SetAlias("pt2","et2");
  nt->SetAlias("adphi","acos(cos(phi1-phi2))");
  ntMix->SetAlias("adphi","dphi");
  ntPythia->SetAlias("adphi","dphi");

  if(type == 0){
    nt->SetAlias("var","pt1-pt2");
    ntPythia->SetAlias("var","pt1-pt2");
    ntMix->SetAlias("var","pt1-pt2");
  }
  if(type == 1){
    nt->SetAlias("var","(pt1-pt2)/pt1");
    ntPythia->SetAlias("var","(pt1-pt2)/pt1");
    ntMix->SetAlias("var","(pt1-pt2)/pt1");
  }
  if(type == 3){
    nt->SetAlias("var","(pt1-pt2)/(pt1+pt2)");
    ntPythia->SetAlias("var","(pt1-pt2)/(pt1+pt2)");
    ntMix->SetAlias("var","(pt1-pt2)/(pt1+pt2)");
  }
  if(type == 5){
    nt->SetAlias("var","pt2/pt1");
    ntPythia->SetAlias("var","pt2/pt1");
    ntMix->SetAlias("var","pt2/pt1");
  }


  nt->Draw("var>>hFull",Form("%s && noise < 0",cut.Data()));
  nt->Draw("var>>hB",Form("%s && noise < 0",side.Data()));


  nt->Draw("pt1>>hNorm",Form("%s && noise < 0",cutNorm.Data()));

  if (useWeight) {
    // use the weight value caluculated by Matt's analysis macro
    ntMix->Draw("var>>hDataMixFull",Form("(%s)*weight",cut.Data())); 
    ntMix->Draw("var>>hDataMixB",Form("(%s)*weight",side.Data()));
    ntMix->Draw("pt1>>hNormDataMix",Form("(%s)*weight",cutNorm.Data()));

  } else {
    // ignore centrality reweighting
    ntMix->Draw("var>>hDataMixFull",Form("(%s)",cut.Data()));  
    ntMix->Draw("var>>hDataMixB",Form("(%s)",side.Data()));
    ntMix->Draw("pt1>>hNormDataMix",Form("(%s)",cutNorm.Data()));

  }
  ntPythia->Draw("var>>hPythiaFull",Form("(%s)",cutpp.Data()));
  ntPythia->Draw("var>>hPythiaB",Form("(%s)",side.Data()));
  ntPythia->Draw("pt1>>hNormPythia",Form("(%s)",cutNorm.Data()));

  hDataMixB->Scale(sideScale);
  hB->Scale(sideScale);
  hPythiaB->Scale(sideScale);

  hDataMix->Add(hDataMixFull);
  h->Add(hFull);
  hPythia->Add(hPythiaFull);

  hDataMix->Add(hDataMixB,-1);
  h->Add(hB,-1);
  hPythia->Add(hPythiaB,-1);

  if(plotSubtraction){

  TCanvas* c1 = new TCanvas("cc1","",800,400);
  c1->Divide(2,1);
  c1->cd(1);
  hFull->Draw("hist");
  hB->Draw("hist same");
  TH1D* hPlot = ((TH1D*)h->Clone(Form("%s_plot",h->GetName())));
  hPlot->SetMarkerStyle(24);
  hPlot->SetLineColor(2);
  hPlot->SetMarkerColor(2);
  hPlot->Draw("same");

  c1->cd(2);

  hDataMixFull->Draw("hist");
  hDataMixB->Draw("hist same");
  hPlot = ((TH1D*)hDataMix->Clone(Form("%s_plot",hDataMix->GetName())));
  hPlot->SetMarkerStyle(24);
  hPlot->SetLineColor(2);
  hPlot->SetMarkerColor(2);
  hPlot->Draw("same");

  c1->Print(Form("AJ_SideBand_Subtraction_lead%d_sub%d_ptbin%d.gif",leadCut,subleadCut,cbin));

  new TCanvas();

  }

  cout<<"ntMix entries : "<<ntMix->GetEntries()<<endl;

  cout<<"h integral : "<<h->Integral()<<endl;
  cout<<"hNorm integral : "<<hNorm->Integral()<<endl;

  cout<<"hDataMix integral : "<<hDataMix->Integral()<<endl;
  cout<<"hNormDataMix integral : "<<hNormDataMix->Integral()<<endl;

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
  hB->SetLineStyle(2);
  hB->SetFillStyle(3002);
  hB->SetFillColor(15);

  hPythia->Scale(1./hNormPythia->Integral());
  hPythia->SetLineColor(kBlue);
  hPythia->SetFillColor(kAzure-8);
  hPythia->SetFillStyle(3005);

  if(normLead){
  hDataMix->Scale(1./hNormDataMix->Integral());
  hDataMixB->Scale(1./hNormDataMix->Integral());
  }else{
    hDataMixB->Scale(1./hDataMix->Integral());
    hDataMix->Scale(1./hDataMix->Integral());
  }
  hDataMix->SetLineColor(mixColor);
  hDataMix->SetFillColor(mixColor);
  hDataMix->SetFillStyle(3004);
  hDataMixB->SetLineStyle(2);
  hDataMixB->SetLineColor(mixColor);

  hDataMix->SetMarkerSize(0);
  hDataMix->SetStats(0);

  TH2D* hPad;  
  double maxX, minX, maxY, minY;
  maxX = 0.95;
  minX = -0.01;
  maxY = 0.32;
  minY = 0.;
  if(cbin >= 3) maxY = 0.48;
  if(type == 0){
    maxX = 306;    
    maxY = 0.26;
  }
  hPad = new TH2D("hPad","",500,minX,maxX,500,minY,maxY);

  hPad->GetXaxis()->SetLabelSize(22);
  hPad->GetXaxis()->SetLabelFont(43);
  hPad->GetXaxis()->SetTitleSize(28);
  hPad->GetXaxis()->SetTitleFont(43);
  hPad->GetXaxis()->SetTitleOffset(2.2);
  hPad->GetXaxis()->CenterTitle();

  hPad->GetYaxis()->SetLabelSize(22);
  hPad->GetYaxis()->SetLabelFont(43);
  hPad->GetYaxis()->SetTitleSize(28);
  hPad->GetYaxis()->SetTitleFont(43);
  hPad->GetYaxis()->SetTitleOffset(2.2);
  hPad->GetYaxis()->CenterTitle();

  if(drawXLabel){
    if(type == 0)hPad->SetXTitle("p_{T,1}-p_{T,2} (GeV/c)");
    if(type == 1)hPad->SetXTitle("(p_{T,1}-p_{T,2})/p_{T,1}");
    if(type == 3)hPad->SetXTitle("A_{J} = (p_{T,1}-p_{T,2})/(p_{T,1}+p_{T,2})");  
    if(type == 5)hPad->SetXTitle("p_{T,2}/p_{T,1}");
  }

  hPad->SetYTitle("Event Fraction");
  hPad->GetYaxis()->SetNdivisions(505,true);
  hPad->GetXaxis()->SetNdivisions(905,true);

  hPad->Draw();
  hDataMix->Draw("hist same");
  hDataMix->Draw("same");

	h->SetLineWidth(1);
	h->Draw("same");
	h->SetLineWidth(2);
	h->Draw("same");

	//	hDataMixB->Draw("same hist");
	//        hB->Draw("same hist");

  cout<<" mean value of data "<<h->GetMean()<<endl;

  if(drawLeg){
    TLegend *t3=new TLegend(0.44,0.6,0.89,0.95); 

    t3->AddEntry(h,Form("%s #mub^{-1}",LUM),"");
    t3->AddEntry(h,"PbPb  #sqrt{s}_{_{NN}}=2.76 TeV","p");
    //    t3->AddEntry(hB,"Estimated Background","l");
    t3->AddEntry(hDataMix,"PYTHIA+HYDJET 1.8","lf");


    //    t3->AddEntry(hDataMix,"2010 #intL dt = 6.7 #mub^{-1}","lf");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
  }

  h->SetName(Form("h%d",cbin));
  hDataMix->SetName(Form("hDataMix%d",cbin));
  hPythia->SetName(Form("hPythia%d",cbin));

  outf->cd();
  h->Write();
  hDataMix->Write();
  hPythia->Write();

  return cstring;

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
  TLatex *cms = new TLatex(0.34,0.23,"CMS Preliminary");
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
