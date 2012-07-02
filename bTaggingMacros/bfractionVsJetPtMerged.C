#include <iostream>
#include "TMath.h"
#include "TFile.h"
#include "TH1.h"
#include "TF1.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TPad.h"

#ifndef __CINT__
#include <RooFit.h>
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooDataSet.h>
#include <RooHistPdf.h>
#include <RooPlot.h>
#include <RooAddPdf.h>
#include <RooGlobalFunc.h>
#endif

using namespace RooFit;

struct Enumerations {

  Double_t nTaggedJetsMC;
  Double_t nUntaggedJetsMC;
  Double_t nJetsMC;
  Double_t nTaggedBjetsMC;
  Double_t nUntaggedBjetsMC;
  Double_t nBjetsMC;
  Double_t nNonBjetsMC;
  Double_t nTaggedNonBjetsMC;

  Double_t nTaggedJetsData;
  Double_t nUntaggedJetsData;

  Double_t cbForJP;
  Double_t cbForCSV;

  Double_t nTaggedJetsMCError;
  Double_t nUntaggedJetsMCError;
  Double_t nJetsMCError;
  Double_t nTaggedBjetsMCError;
  Double_t nUntaggedBjetsMCError;
  Double_t nBjetsMCError;
  Double_t nNonBjetsMCError;
  Double_t nTaggedNonBjetsMCError;

  Double_t nTaggedJetsDataError;
  Double_t nUntaggedJetsDataError;

};



void bfractionVsJetPtMerged(char *tagger="discr_ssvHighEff", double workingPoint=2., int fixCL=0, char *taggerName="ssvHighEff") {

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetLabelFont(43,"xyz");
  gStyle->SetLabelSize(14,"xyz");
  gStyle->SetTitleFont(43,"xyz");
  gStyle->SetTitleSize(16,"xyz");
  gStyle->SetTitleOffset(3.5,"x"); 
  gROOT->ForceStyle(1);
  
  int doLTJP=1;
  int doLTCSV=0;

  const int nBins = 4;
  double ptBin[nBins+1] = {80,100,120,150,200};
  //const int nBins = 2;
  //double ptBin[nBins+1] = {80,100,150};
  
  Double_t bPurMC, bPurData, bEffMC, bEffDataLTJP, bEffDataLTCSV, taggedFracData, bFracMC, bFracData, bFracDataLTJP, bFracDataLTCSV, bFracJPdirect;
  Double_t bPurMCError, bPurDataError, bEffMCError, bEffDataLTJPError, bEffDataLTCSVError, taggedFracDataError, bFracMCError, bFracDataError, bFracDataLTJPError, bFracDataLTCSVError, bFracJPdirectError;
  Enumerations numbers;
  
  TH1D *hBPurityData = new TH1D("hBPurityData","hBPurityData;Jet p_{T} (GeV/c);B-Tagging purity",nBins,ptBin);
  TH1D *hBPurityMC = new TH1D("hBPurityMC","hBPurityMC;Jet p_{T} (GeV/c);B-Tagging purity",nBins,ptBin);
  
  TH1D *hBEfficiencyMC = new TH1D("hBEfficiencyMC","hBEfficiencyMC;Jet p_{T} (GeV/c);B-Tagging efficiency",nBins,ptBin);
  TH1D *hBEfficiencyDataLTJP = new TH1D("hBEfficiencyDataLTJP","hBEfficiencyDataLTJP;Jet p_{T} (GeV/c);B-Tagging efficiency",nBins,ptBin);
  TH1D *hBEfficiencyDataLTCSV = new TH1D("hBEfficiencyDataLTCSV","hBEfficiencyDataLTCSV;Jet p_{T} (GeV/c);B-Tagging efficiency",nBins,ptBin);
  
  TH1D *hBFractionMC = new TH1D("hBFractionMC","hBFractionMC;Jet p_{T} (GeV/c);B-jet fraction",nBins,ptBin);
  TH1D *hBFractionData = new TH1D("hBFractionData","hBFractionData;Jet p_{T} (GeV/c);B-jet fraction",nBins,ptBin);
  TH1D *hBFractionDataLTJP = new TH1D("hBFractionDataLTJP","hBFractionDataLTJP;Jet p_{T} (GeV/c);B-jet fraction",nBins,ptBin);
  TH1D *hBFractionDataLTCSV = new TH1D("hBFractionDataLTCSV","hBFractionDataLTCSV;Jet p_{T} (GeV/c);B-jet fraction",nBins,ptBin);
  TH1D *hBFractionJPdirect = new TH1D("hBFractionJPdirect","hBFractionJPdirect;Jet p_{T} (GeV/c);B-jet fraction",nBins,ptBin);
  
  int ncol=1;
  int nrow=1;

  if(nBins==3||nBins==2){
    ncol=nBins;
  }
  if(nBins==4){
    ncol=nBins/2;
    nrow=nBins/2;
  }

  TCanvas *c1=new TCanvas("c1","c1",1200,600);
  c1->Divide(ncol,nrow,0,0);
  TCanvas *c2=new TCanvas("c2","c2",1200,600);
  c2->Divide(ncol,nrow,0,0);
  TCanvas *c3=new TCanvas("c3","c3",1200,600);
  c3->Divide(ncol,nrow,0,0);
  TCanvas *c4=new TCanvas("c4","c4",1200,600);
  c4->Divide(ncol,nrow,0,0);

  TCanvas *cCount = new TCanvas("cCount","cCount",600,600);

  for (int n=0;n<nBins;n++) {

    cout<<"Processing jet pT bin ["<<ptBin[n]<<","<<ptBin[n+1]<<"] ..."<<endl;
    cCount->cd();
    numbers = count(ptBin[n],ptBin[n+1],tagger,workingPoint);
    c1->cd(n+1);
    c1->GetPad(n+1)->SetLogy();
    RooRealVar fitSvtxmTag = bfractionFit(fixCL,"svtxm",0,7,ptBin[n],ptBin[n+1],tagger,workingPoint,7,Form("b-tagged sample (%s at %.1f)",taggerName,workingPoint));
    //RooRealVar fitSvtxmTag = bfractionFit(fixCL,"svtxm",0,7,ptBin[n],ptBin[n+1],tagger,workingPoint,10,Form("b-tagged sample (%s at %.1f)",taggerName,workingPoint));
    //RooRealVar fitJpDirect = bfractionFit(fixCL,"discr_prob",0,3,ptBin[n],ptBin[n+1],tagger,-2,10,"inclusive sample",5e4);
    c2->cd(n+1);
    c2->GetPad(n+1)->SetLogy();
    RooRealVar fitJpDirect = bfractionFit(fixCL,"discr_prob",0.0,3.5,ptBin[n],ptBin[n+1],"discr_prob",0.,3.5,"inclusive sample",5e4);

    if (doLTJP) {
      c3->cd(n+1);
      c3->GetPad(n+1)->SetLogy();
      RooRealVar fitJpBeforetag = bfractionFit(fixCL,"discr_prob",0.0001,3.5,ptBin[n],ptBin[n+1],"discr_prob",0,3.5,"jets with JP info",5e4);
      c4->cd(n+1);
      c4->GetPad(n+1)->SetLogy();
      RooRealVar fitJpTag = bfractionFit(fixCL,"discr_prob",0.0001,3.5,ptBin[n],ptBin[n+1],tagger,workingPoint,7,Form("b-tagged sample (%s at %.1f)",taggerName,workingPoint),5e4);
    } 
    if (doLTCSV) {
      RooRealVar fitCsvBeforetag = bfractionFit(fixCL,"discr_csvSimple",0,1,ptBin[n],ptBin[n+1],tagger,-2,10,"jets with CSV info",5e4);
      RooRealVar fitCsvTag = bfractionFit(fixCL,"discr_csvSimple",0,1,ptBin[n],ptBin[n+1],tagger,workingPoint,10,Form("b-tagged sample (%s at %.1f)",taggerName,workingPoint),5e4);
    } 

    taggedFracData = numbers.nTaggedJetsData / (numbers.nTaggedJetsData+numbers.nUntaggedJetsData);
    taggedFracDataError = fracError(numbers.nTaggedJetsData,numbers.nUntaggedJetsData,numbers.nTaggedJetsDataError,numbers.nUntaggedJetsDataError);
    
    //*  --- b-tagging purity --- 

    bPurMC = numbers.nTaggedBjetsMC / numbers.nTaggedJetsMC;
    bPurMCError = fracError(numbers.nTaggedBjetsMC,numbers.nTaggedNonBjetsMC,numbers.nTaggedBjetsMCError,numbers.nTaggedNonBjetsMCError);
    bPurData = fitSvtxmTag.getVal();
    bPurDataError = fitSvtxmTag.getError();

    hBPurityMC->SetBinContent(n+1,bPurMC); 
    hBPurityMC->SetBinError(n+1,bPurMCError); 
    hBPurityData->SetBinContent(n+1,bPurData);    
    hBPurityData->SetBinError(n+1,bPurDataError); 
    //*/
    
    //*  --- b-tagging efficiency --- 

    bEffMC = numbers.nTaggedBjetsMC / numbers.nBjetsMC;
    bEffMCError = fracError(numbers.nTaggedBjetsMC,numbers.nUntaggedBjetsMC,numbers.nTaggedBjetsMCError,numbers.nUntaggedBjetsMCError);
    hBEfficiencyMC->SetBinContent(n+1,bEffMC); 
    hBEfficiencyMC->SetBinError(n+1,bEffMCError);

    if (doLTJP) {
      bEffDataLTJP = taggedFracData * numbers.cbForJP * fitJpTag.getVal() / fitJpBeforetag.getVal();
      bEffDataLTJPError = prodError(taggedFracData,fitJpTag.getVal(),taggedFracDataError,fitJpTag.getError()) * numbers.cbForJP / fitJpBeforetag.getVal(); 
      hBEfficiencyDataLTJP->SetBinContent(n+1,bEffDataLTJP);    
      hBEfficiencyDataLTJP->SetBinError(n+1,bEffDataLTJPError);
    } 

    if (doLTCSV) {
      bEffDataLTCSV = taggedFracData * numbers.cbForCSV * fitCsvTag.getVal() / fitCsvBeforetag.getVal();
      bEffDataLTCSVError = prodError(taggedFracData,fitCsvTag.getVal(),taggedFracDataError,fitCsvTag.getError()) * numbers.cbForCSV / fitCsvBeforetag.getVal(); 
      hBEfficiencyDataLTCSV->SetBinContent(n+1,bEffDataLTCSV);    
      hBEfficiencyDataLTCSV->SetBinError(n+1,bEffDataLTCSVError); 
    } 
    
    //*  --- b fraction --- 

    bFracMC = numbers.nBjetsMC / numbers.nJetsMC;
    //bFracMC = numbers.nTaggedJetsMC * bPurMC / (bEffMC * numbers.nJetsMC); // for check : same as previous
    bFracMCError = fracError(numbers.nBjetsMC,numbers.nNonBjetsMC,numbers.nBjetsMCError,numbers.nNonBjetsMCError); 
    hBFractionMC->SetBinContent(n+1,bFracMC); 
    hBFractionMC->SetBinError(n+1,bFracMCError); 


    bFracData = taggedFracData * bPurData / bEffMC; // efficiency from MC
    bFracDataError = prodError(taggedFracData,bPurData,taggedFracDataError,bPurDataError) / bEffMC; // stat.error from purity and tagged-fraction (assumed independent)
    //bFracDataError = bFracData * bPurDataError / bPurData; // stat.error only from purity
    hBFractionData->SetBinContent(n+1,bFracData);    
    hBFractionData->SetBinError(n+1,bFracDataError);

    if (doLTJP) {
      bFracDataLTJP = taggedFracData * bPurData / bEffDataLTJP ; // efficiency from LTJP method
      bFracDataLTJPError = prodError(taggedFracData,bPurData,taggedFracDataError,bPurDataError) / bEffDataLTJP; // stat.error from purity and tagged-fraction (assumed independent)
      //bFracDataLTJPError = bFracDataLTJP * bPurDataError / bPurData; // stat.error only from purity
      hBFractionDataLTJP->SetBinContent(n+1,bFracDataLTJP);    
      hBFractionDataLTJP->SetBinError(n+1,bFracDataLTJPError);
    } 

    if (doLTCSV) {
      bFracDataLTCSV = taggedFracData * bPurData / bEffDataLTCSV; // efficiency from LTCSV method
      bFracDataLTCSVError = prodError(taggedFracData,bPurData,taggedFracDataError,bPurDataError) / bEffDataLTCSV; // stat.error from purity and tagged-fraction (assumed independent)
      //bFracDataLTCSVError = bFracDataLTCSV * bPurDataError / bPurData; // stat.error only from purity
      hBFractionDataLTCSV->SetBinContent(n+1,bFracDataLTCSV);    
      hBFractionDataLTCSV->SetBinError(n+1,bFracDataLTCSVError);
    } 

    bFracJPdirect = fitJpDirect.getVal();
    bFracJPdirectError = fitJpDirect.getError();
    hBFractionJPdirect->SetBinContent(n+1,bFracJPdirect);   
    hBFractionJPdirect->SetBinError(n+1,bFracJPdirectError);
    //*/

    //*
    cout<<"nTaggedJetsMC "<<numbers.nTaggedJetsMC<<endl;
    cout<<"nUntaggedJetsMC "<<numbers.nUntaggedJetsMC<<endl;
    cout<<"nJetsMC "<<numbers.nJetsMC<<endl;
    cout<<"nTaggedBjetsMC "<<numbers.nTaggedBjetsMC<<endl;
    cout<<"nUntaggedBjetsMC "<<numbers.nUntaggedBjetsMC<<endl;
    cout<<"nBjetsMC "<<numbers.nBjetsMC<<endl;
    cout<<"nNonBjetsMC "<<numbers.nNonBjetsMC<<endl;
    cout<<"nTaggedNonBjetsMC "<<numbers.nTaggedNonBjetsMC<<endl;
    cout<<"nTaggedJetsData "<<numbers.nTaggedJetsData<<endl;
    cout<<"nUntaggedJetsData "<<numbers.nUntaggedJetsData<<endl;
    cout<<"bPurMC "<<bPurMC<<endl;
    cout<<"bPurData "<<bPurData<<endl;
    cout<<"bEffMC "<<bEffMC<<endl;
    cout<<"CbForJP "<<numbers.cbForJP<<endl;
    cout<<"bEffDataLTJP "<<bEffDataLTJP<<endl;
    cout<<"CbForCSV "<<numbers.cbForCSV<<endl;
    cout<<"bEffDataLTCSV "<<bEffDataLTCSV<<endl;
    cout<<"bFracMC "<<bFracMC<<endl;
    cout<<"bFracData "<<bFracData<<endl;
    cout<<"bFracDataLTJP "<<bFracDataLTJP<<endl;
    cout<<"bFracDataLTCSV "<<bFracDataLTCSV<<endl;
    cout<<"bFracJPdirect "<<bFracJPdirect<<endl;
    cout<<endl;
    //*/
  }
  
  TLegend *legPur = new TLegend(0.4,0.15,0.85,0.3,Form("Purity of b-tagged sample (%s at %.1f)",taggerName,workingPoint));
  legPur->SetBorderSize(0);
  legPur->SetFillColor(kGray);
  legPur->AddEntry(hBPurityMC,"MC","pl");
  legPur->AddEntry(hBPurityData,"Data (fit of svtxm)","pl");
  TCanvas *cBPurity = new TCanvas("cBPurity","b purity",600,600);
  hBPurityMC->SetAxisRange(0,1,"Y");
  hBPurityMC->SetTitleOffset(1.3,"Y");
  hBPurityMC->SetLineColor(2);
  hBPurityMC->SetMarkerColor(2);
  hBPurityMC->SetMarkerStyle(21);
  hBPurityMC->Draw();
  hBPurityData->SetLineColor(1);
  hBPurityData->SetMarkerColor(1);
  hBPurityData->SetMarkerStyle(20);
  hBPurityData->Draw("same");   
  legPur->Draw();
  cBPurity->SaveAs("purity.gif");

  TLegend *legEff = new TLegend(0.4,0.65,0.85,0.8,Form("Efficiency for tagging b-jets (%s at %.1f)",taggerName,workingPoint));
  legEff->SetBorderSize(0);
  legEff->SetFillColor(kGray);
  legEff->AddEntry(hBEfficiencyMC,"MC","pl");
  if (doLTJP) legEff->AddEntry(hBEfficiencyDataLTJP,"LT method (JP)","pl");
  if (doLTCSV) legEff->AddEntry(hBEfficiencyDataLTCSV,"LT method (CSV)","pl");
  TCanvas *cBEfficiency = new TCanvas("cBEfficiency","B-Tagging efficiency",600,600);
  hBEfficiencyMC->SetAxisRange(0,1,"Y");
  hBEfficiencyMC->SetTitleOffset(1.3,"Y");
  hBEfficiencyMC->SetLineColor(2);
  hBEfficiencyMC->SetMarkerColor(2);
  hBEfficiencyMC->SetMarkerStyle(21);
  hBEfficiencyMC->Draw();
  if (doLTJP) {
    hBEfficiencyDataLTJP->SetLineColor(8);
    hBEfficiencyDataLTJP->SetMarkerColor(8);
    hBEfficiencyDataLTJP->SetMarkerStyle(20);
    hBEfficiencyDataLTJP->Draw("same");
  }
  if (doLTCSV) {
    hBEfficiencyDataLTCSV->SetLineColor(7);
    hBEfficiencyDataLTCSV->SetMarkerColor(7);
    hBEfficiencyDataLTCSV->SetMarkerStyle(20);
    hBEfficiencyDataLTCSV->Draw("same");
  }
  legEff->Draw();
  cBEfficiency->SaveAs("efficiency.gif");


  TLegend *legFrac = new TLegend(0.25,0.15,0.85,0.3);
  legFrac->SetBorderSize(0);
  legFrac->SetFillColor(kGray);
  legFrac->AddEntry(hBFractionMC,"MC","pl");
  legFrac->AddEntry(hBFractionJPdirect,"direct fit of JP","pl");
  legFrac->AddEntry(hBFractionData,Form("%s at %.1f + pur. from svtxm + eff. from MC",taggerName,workingPoint),"pl");
  if (doLTJP) legFrac->AddEntry(hBFractionDataLTJP,Form("%s at %.1f + pur. from svtxm + eff. from LT (JP)",taggerName,workingPoint),"pl");
  if (doLTCSV) legFrac->AddEntry(hBFractionDataLTCSV,Form("%s at %.1f + pur. from svtxm + eff. from LT (CSV)",taggerName,workingPoint),"pl");
  TCanvas *cBFraction = new TCanvas("cBFraction","B-jet fraction",600,600);
  hBFractionMC->SetAxisRange(0,0.03,"Y");
  hBFractionMC->SetTitleOffset(1.8,"Y");
  hBFractionMC->SetLineColor(2);
  hBFractionMC->SetMarkerColor(2);
  hBFractionMC->SetMarkerStyle(21);
  hBFractionMC->Draw(); 
  hBFractionData->SetLineColor(1);
  hBFractionData->SetMarkerColor(1);
  hBFractionData->SetMarkerStyle(20);
  hBFractionData->Draw("same");   
  if (doLTJP) {
    hBFractionDataLTJP->SetLineColor(8);
    hBFractionDataLTJP->SetMarkerColor(8);
    hBFractionDataLTJP->SetMarkerStyle(20);
    hBFractionDataLTJP->Draw("same");
  }
  if (doLTCSV) {
    hBFractionDataLTCSV->SetLineColor(7);
    hBFractionDataLTCSV->SetMarkerColor(7);
    hBFractionDataLTCSV->SetMarkerStyle(20);
    hBFractionDataLTCSV->Draw("same");
  }
  hBFractionJPdirect->SetLineColor(4);
  hBFractionJPdirect->SetMarkerColor(4);
  hBFractionJPdirect->SetMarkerStyle(20);
  hBFractionJPdirect->Draw("same");
  legFrac->Draw();
  cBFraction->SaveAs("bfraction.gif");


  TFile *fout = new TFile(Form("output/bFractionMerged_%sat%.1fFixCL%d.root",taggerName,workingPoint,fixCL),"recreate");
  hBFractionMC->Write();
  hBFractionData->Write();
  if (doLTJP) hBFractionDataLTJP->Write();
  if (doLTCSV) hBFractionDataLTCSV->Write();
  hBFractionJPdirect->Write();
  fout->Close();

  c1->SaveAs(Form("gifs/svtxMassFit_%s.gif",fixCL?"CLfixed":"CLfree"));
  c2->SaveAs(Form("gifs/jpDirectFit_%s.gif",fixCL?"CLfixed":"CLfree"));
  c3->SaveAs(Form("gifs/jpBeforeTag_%s.gif",fixCL?"CLfixed":"CLfree"));
  c4->SaveAs(Form("gifs/jpAfterTag_%s.gif",fixCL?"CLfixed":"CLfree"));


}


Double_t addError(Double_t aErr, Double_t bErr) {
  // if a,b are independent
  return sqrt(aErr*aErr + bErr*bErr);
}
Double_t substractError(Double_t aErr, Double_t bErr) {
  // if b is a subset of a
  return sqrt(aErr*aErr - bErr*bErr);
}
Double_t prodError(Double_t a, Double_t b, Double_t aErr, Double_t bErr) {
  // if a,b are independent
  return sqrt(a*a*bErr*bErr + b*b*aErr*aErr);
}
Double_t fracError(Double_t a, Double_t b, Double_t aErr, Double_t bErr) {
  // error on a/(a+b) 
  // if a,b are independent
  // and aErr,bErr can be given by sqrt(sum of squares of weights)
  Double_t c=a+b;
  return sqrt( aErr*aErr * b*b/(c*c*c*c)
	      +bErr*bErr * a*a/(c*c*c*c) ) ;
}



void fixEmpty(TH1 *h){
   for (int i=1;i<=h->GetNbinsX();i++){
      if (h->GetBinContent(i)==0) h->SetBinContent(i,1e-20);
   }
}

RooRealVar bfractionFit(bool fixCL=1, char *var="discr_prob", double minXvar=0, double maxXvar=3, double ptMin=100, double ptMax=500, 
// by default, no b-tagging :
char *discr="discr_prob", double minXdiscr=-999, double maxXdiscr=999, char *comment="inclusive sample", 
double maxYaxis=1e3)
{
  // discr_prob : from (0) 0 to 3, operating point : 0.6 (1%), 0.7 
  // discr_ssvHighEff : from (-1) 1 to 6, operating point : 2 ?
  // discr_ssvHighPur : from (-1) 1 to 6, operating point : 2 ?  
  // discr_csvSimple : from (-10,-1) 0 to 1, operating point  : 0.9  
  // svtxm : from (0) 0 to 7 
  // muptrel : from (0) 0 to 5

  //*
  TFile *fQCDMC = new TFile("../histos/PbPbQCDMC.root"); 
  TFile *fBMC = new TFile("../histos/PbPbBMC.root"); 
  TFile *fCMC = new TFile("../histos/PbPbCMC.root"); 
  TFile *fdata = new TFile("../histos/PbPbdata.root");
  //*/

  /*
  TFile *fQCDMC = new TFile("../histos/ppMC_hiReco_jetTrig.root"); 
  TFile *fBMC = new TFile("../histos/ppMC_hiReco_jetTrig.root"); 
  TFile *fCMC = new TFile("../histos/ppMC_hiReco_jetTrig.root"); 
  TFile *fdata = new TFile("../histos/ppdata_hiReco_jetTrig.root");
  */

  TTree *tQCDMC = (TTree*) fQCDMC->Get("nt");
  TTree *tBMC = (TTree*) fBMC->Get("nt");
  TTree *tCMC = (TTree*) fCMC->Get("nt");
  TTree *tdata = (TTree*) fdata->Get("nt");

  
  TH1D *hB = new TH1D("hB","",50,minXvar,maxXvar);
  hB->Sumw2();
  tBMC->Draw(Form("%s>>hB",var),Form("weight*(abs(refparton_flavorForB)==5&&jtpt>=%f&&jtpt<%f&&%s>=%f&&%s<%f)",ptMin,ptMax,discr,minXdiscr,discr,maxXdiscr));
  fixEmpty(hB);
  
  TH1D *hC = new TH1D("hC","",50,minXvar,maxXvar);
  hC->Sumw2();
  tCMC->Draw(Form("%s>>hC",var),Form("weight*(abs(refparton_flavorForB)==4&&jtpt>=%f&&jtpt<%f&&%s>=%f&&%s<%f)",ptMin,ptMax,discr,minXdiscr,discr,maxXdiscr));
  fixEmpty(hC);

  TH1D *hL = new TH1D("hL","",50,minXvar,maxXvar);
  hL->Sumw2();
  tQCDMC->Draw(Form("%s>>hL",var),Form("weight*(abs(refparton_flavorForB)!=5&&abs(refparton_flavorForB)!=4&&abs(refparton_flavorForB)<99&&jtpt>=%f&&jtpt<%f&&%s>=%f&&%s<%f)",ptMin,ptMax,discr,minXdiscr,discr,maxXdiscr));
  fixEmpty(hL);
  
  TH1D *hCaux = new TH1D("hCaux","",50,minXvar,maxXvar);
  hCaux->Sumw2();
  tQCDMC->Draw(Form("%s>>hCaux",var),Form("weight*(abs(refparton_flavorForB)==4&&jtpt>=%f&&jtpt<%f&&%s>=%f&&%s<%f)",ptMin,ptMax,discr,minXdiscr,discr,maxXdiscr));
  fixEmpty(hCaux);

  /*
  TH1D *hCL = new TH1D("hCL","",50,minXvar,maxXvar);
  hCL->Sumw2();
  tQCDMC->Draw(Form("%s>>hCL",var),Form("weight*(abs(refparton_flavorForB)!=5&&abs(refparton_flavorForB)<99&&jtpt>=%f&&jtpt<%f&&%s>=%f&&%s<%f)",ptMin,ptMax,discr,minXdiscr,discr,maxXdiscr));
  fixEmpty(hCL);
  //*/
  //*
  TH1D *hCL = hL->Clone();
  Double_t cCoef = hCaux->Integral()/hC->Integral();
  hCL->Add(hC,cCoef);
  //*/


  // --- Observable ---
  RooRealVar s(var,var,0,minXvar,maxXvar);
  RooRealVar jtpt("jtpt","jtpt",0,ptMin,ptMax);
  RooRealVar discriminator(discr,discr,0,minXdiscr,maxXdiscr);
 
  // --- Build Histogram PDF ---
  RooDataHist xB("xB","xB",s,hB);
  RooHistPdf bottom("bottom","bottom PDF",s,xB);
  RooDataHist xC("xC","xC",s,hC);
  RooHistPdf charm("charm","charm PDF",s,xC);
  RooDataHist xL("xL","xL",s,hL);
  RooHistPdf light("light","light PDF",s,xL);
  RooDataHist xCL("xCL","xCL",s,hCL);
  RooHistPdf charmlight("charmlight","charmlight PDF",s,xCL);

  /*
  cout<<"hB "<<hB->Integral()<<endl;
  cout<<"hC "<<hC->Integral()<<endl;
  cout<<"hL "<<hL->Integral()<<endl;
  cout<<"hCL "<<hCL->Integral()<<endl;
  //*/

  // --- Construct signal+background PDF ---
  //Double_t bInitFrac = hB->Integral()/(hB->Integral()+hCL->Integral());
  //Double_t cInitFrac = hC->Integral()/(hB->Integral()+hCL->Integral());
  RooRealVar Bfraction("Bfraction","#light events",0.3,0.,1);
  RooRealVar Cfraction("Cfraction","#background events",0.3,0.,1); 
  if(fixCL) RooAddPdf model("model","",bottom,charmlight,Bfraction);
  else RooAddPdf model("model","",RooArgList(bottom,charm,light),RooArgList(Bfraction,Cfraction));  

  // --- Data sample ---
  RooDataSet *data = new RooDataSet("data","data",tdata,RooArgSet(s,jtpt,discriminator),Form("jtpt>=%f&&jtpt<%f&&%s>=%f&&%s<%f",ptMin,ptMax,discr,minXdiscr,discr,maxXdiscr));
    


  TPaveText *header = new TPaveText(0.05,0.9,0.95,0.99);
  header->AddText(Form("%s  -  ROOFIT ML unbinned fit of %s",var,fixCL?"2 components : bottom and (charm + light)":"3 components : bottom, charm and light"));
  header->AddText(Form("Pb-Pb data - %s",comment));
  header->AddText(Form("%s%.0f <= jet pT < %.0f",(var=="muptrel")?"deltaR < 0.5 ; muon pT > 5 ; ":"",ptMin,ptMax));
  header->SetTextSize(0.027);
  header->SetTextAlign(12);
  header->SetBorderSize(0);
  header->SetFillStyle(0);
  //header->Draw();


  //RooPlot* sframe = s.frame();
  TH2D *htemp = new TH2D(Form("%s%.0f%.0f",var,ptMin,ptMax),Form("%s%.0f%.0f",var,ptMin,ptMax),100,minXvar,maxXvar,100,0.5,maxYaxis) ;
  htemp->SetXTitle(Form("%s %.0f < p_{T} < %.0f GeV/c",var,ptMin,ptMax));
  htemp->SetYTitle("Entries");
  /*
  htemp->Draw();
  data->plotOn(sframe,Binning(50));
  if(fixCL) {
    model.plotOn(sframe,Components(charmlight),LineStyle(kDashed),LineColor(30),LineWidth(2));
  } else {
    model.plotOn(sframe,Components(light),LineStyle(kDashed),LineColor(kBlue),LineWidth(2));
    model.plotOn(sframe,Components(charm),LineStyle(kDashed),LineColor(kGreen),LineWidth(2));
    model.plotOn(sframe,Components(RooArgSet(charm,light)),LineStyle(kDashed),LineColor(30),LineWidth(2));
  }
  model.plotOn(sframe,Components(bottom),LineStyle(kDashed),LineColor(kRed),LineWidth(2),FillColor(kRed),FillStyle(1));   
  model.plotOn(sframe,LineWidth(2),LineColor(13));
  data->plotOn(sframe,Binning(50));
  model.paramOn(sframe,Layout(0.4,0.9,0.9),Format("NEU",FixedPrecision(3)));
  sframe->Draw("same");
  */

  // --- Perform extended ML fit of composite PDF to data ---
  RooFitResult *fitresult = model.fitTo(*data,Save(),PrintLevel(-1));
  

  RooPlot* sframe = s.frame();
  //  sframe = s.frame();

  htemp->Draw();
  data->plotOn(sframe,Binning(50));
  if(fixCL) {
    model.plotOn(sframe,Components(charmlight),LineStyle(kDashed),LineColor(30),LineWidth(2));
  } else {
    model.plotOn(sframe,Components(light),LineStyle(kDashed),LineColor(kBlue),LineWidth(2));
    model.plotOn(sframe,Components(charm),LineStyle(kDashed),LineColor(kGreen),LineWidth(2));
    model.plotOn(sframe,Components(RooArgSet(charm,light)),LineStyle(kDashed),LineColor(30),LineWidth(2));
  }
  model.plotOn(sframe,Components(bottom),LineStyle(kDashed),LineColor(kRed),LineWidth(2),FillColor(kRed),FillStyle(1));   
  model.plotOn(sframe,LineWidth(2),VisualizeError(*fitresult),FillColor(17));
  model.plotOn(sframe,LineWidth(2),LineColor(13));
  data->plotOn(sframe,Binning(50));
  model.paramOn(sframe,Layout(0.4,0.9,0.9),Format("NEU",FixedPrecision(3)));
  sframe->Draw("same");
  TLegend *leg = new TLegend(0.61,fixCL?0.60:0.50,0.98,fixCL?0.78:0.75);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->AddEntry("h_data","PbPb data","p");
  leg->AddEntry(Form("model_Norm[%s]_Comp[bottom]",var),"b","l");
  if(fixCL) {
    leg->AddEntry(Form("model_Norm[%s]_Comp[charmlight]",var),"c + udsg","l");
  } else {
    leg->AddEntry(Form("model_Norm[%s]_Comp[charm]",var),"c","l");
    leg->AddEntry(Form("model_Norm[%s]_Comp[light]",var),"udsg","l");
    leg->AddEntry(Form("model_Norm[%s]_Comp[charm,light]",var),"c + udsg","l");    
  }
  leg->AddEntry(Form("model_Norm[%s]",var),"b + c + udsg","l");
  leg->Draw("same");


  // --- Print results ---
  //cout <<"b jet fraction in MC = "<<bInitFrac<<endl;
  cout <<"b jet fraction in data = "<<Bfraction.getVal()<<endl;
  if(!fixCL) cout <<"c jet fraction = "<<Cfraction.getVal()<<endl;

  // --- Save canvas ---
  TString path = Form("gifs/%s_jtpt%.0fto%.0f_%s%.2fto%.2f_%s.gif",var,ptMin,ptMax,discr,minXdiscr,maxXdiscr,fixCL?"CLfixed":"CLfree");
  //cROOFIT->SaveAs(path);

  return Bfraction;
}



Enumerations count(double ptMin, double ptMax, char *discr, double workingPoint) {

  //*
  TFile *fQCDMC = new TFile("../histos/PbPbQCDMC.root"); 
  TFile *fBMC = new TFile("../histos/PbPbBMC.root"); 
  //TFile *fCMC = new TFile("../histos/PbPbCMC.root"); 
  TFile *fdata = new TFile("../histos/PbPbdata.root");
  //*/

  /*
  //TFile *fMC = new TFile("../histos/ppMC_hiReco_jetTrig.root"); 
  //TFile *fdata = new TFile("../histos/ppdata_hiReco_jetTrig.root");
  */

  TTree *tQCDMC = (TTree*) fQCDMC->Get("nt");
  TTree *tBMC = (TTree*) fBMC->Get("nt");
  TTree *tdata = (TTree*) fdata->Get("nt");

  //TCanvas *c = new TCanvas("c","",600,600);
 
  TH1D *hTaggedJetsMC = new TH1D("hTaggedJetsMC","",1,ptMin,ptMax);
  hTaggedJetsMC->Sumw2();
  tQCDMC->Draw("jtpt>>hTaggedJetsMC",Form("weight*(%s>=%f)",discr,workingPoint));

  TH1D *hUntaggedJetsMC = new TH1D("hUntaggedJetsMC","",1,ptMin,ptMax);
  hUntaggedJetsMC->Sumw2();
  tQCDMC->Draw("jtpt>>hUntaggedJetsMC",Form("weight*(%s<%f)",discr,workingPoint));
  
  TH1D *hTaggedBjetsMC = new TH1D("hTaggedBjetsMC","",1,ptMin,ptMax);
  hTaggedBjetsMC->Sumw2();
  tBMC->Draw("jtpt>>hTaggedBjetsMC",Form("weight*(abs(refparton_flavorForB)==5&&%s>=%f)",discr,workingPoint));  

  TH1D *hUntaggedBjetsMC = new TH1D("hUntaggedBjetsMC","",1,ptMin,ptMax);
  hUntaggedBjetsMC->Sumw2();
  tBMC->Draw("jtpt>>hUntaggedBjetsMC",Form("weight*(abs(refparton_flavorForB)==5&&%s<%f)",discr,workingPoint));

  TH1D *hBjetsWithJPinfoMC = new TH1D("hBjetsWithJPinfoMC","",1,ptMin,ptMax);
  hBjetsWithJPinfoMC->Sumw2();
  tBMC->Draw("jtpt>>hBjetsWithJPinfoMC","weight*(abs(refparton_flavorForB)==5&&discr_prob>0)");

  TH1D *hBjetsWithCSVinfoMC = new TH1D("hBjetsWithCSVinfoMC","",1,ptMin,ptMax);
  hBjetsWithCSVinfoMC->Sumw2();
  tBMC->Draw("jtpt>>hBjetsWithCSVinfoMC","weight*(abs(refparton_flavorForB)==5&&discr_prob>0)");
 
  TH1D *hTaggedJetsData = new TH1D("hTaggedJetsData","",1,ptMin,ptMax);
  hTaggedJetsData->Sumw2();
  tdata->Draw("jtpt>>hTaggedJetsData",Form("weight*(%s>=%f)",discr,workingPoint));

  TH1D *hUntaggedJetsData = new TH1D("hUntaggedJetsData","",1,ptMin,ptMax);
  hUntaggedJetsData->Sumw2();
  tdata->Draw("jtpt>>hUntaggedJetsData",Form("weight*(%s<%f)",discr,workingPoint));

  Enumerations res;

  res.nTaggedJetsMC = hTaggedJetsMC->GetBinContent(1);
  res.nUntaggedJetsMC = hUntaggedJetsMC->GetBinContent(1);
  res.nJetsMC = res.nTaggedJetsMC + res.nUntaggedJetsMC;
  res.nTaggedBjetsMC = hTaggedBjetsMC->GetBinContent(1);
  res.nUntaggedBjetsMC = hUntaggedBjetsMC->GetBinContent(1);
  res.nBjetsMC = res.nTaggedBjetsMC + res.nUntaggedBjetsMC;
  res.nNonBjetsMC = res.nJetsMC - res.nBjetsMC;
  res.nTaggedNonBjetsMC = res.nTaggedJetsMC - res.nTaggedBjetsMC;

  res.nTaggedJetsData = hTaggedJetsData->GetBinContent(1);
  res.nUntaggedJetsData = hUntaggedJetsData->GetBinContent(1);

  res.cbForJP = hBjetsWithJPinfoMC->GetBinContent(1) / res.nBjetsMC;
  res.cbForCSV = hBjetsWithCSVinfoMC->GetBinContent(1) / res.nBjetsMC;

  res.nTaggedJetsMCError = hTaggedJetsMC->GetBinError(1);
  res.nUntaggedJetsMCError = hUntaggedJetsMC->GetBinError(1);
  res.nJetsMCError = addError(res.nTaggedJetsMCError,res.nUntaggedJetsMCError);
  res.nTaggedBjetsMCError = hTaggedBjetsMC->GetBinError(1);
  res.nUntaggedBjetsMCError = hUntaggedBjetsMC->GetBinError(1);
  res.nBjetsMCError = addError(res.nTaggedBjetsMCError,res.nUntaggedBjetsMCError);
  res.nNonBjetsMCError = substractError(res.nJetsMCError,res.nBjetsMCError);
  res.nTaggedNonBjetsMCError = substractError(res.nTaggedJetsMCError,res.nTaggedBjetsMCError);

  res.nTaggedJetsDataError = hTaggedJetsData->GetBinError(1);
  res.nUntaggedJetsDataError = hUntaggedJetsData->GetBinError(1);

  return res;
 
}

