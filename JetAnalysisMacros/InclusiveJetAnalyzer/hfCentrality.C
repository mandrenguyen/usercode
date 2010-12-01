#include <TStyle.h>
#include <TH1F.h>
#include <TH2D.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TNtuple.h>
#include <iostream>
#include <TLine.h>
#include <TMath.h>
#include <math.h>
#include <TVector3.h>
#include <TLorentzVector.h>
#include <TROOT.h>
#include <TRandom.h>
#include <TCut.h>
#include <TClonesArray.h>
#include "/home/kimy/script/commonUtility.h"
#include <TLatex.h>
#define PI 3.141592653589


void hfCentrality(char* fname = "r151878.root",  char* trg="anaPixelHitJet50U")
{
   
   TFile *f1=new TFile(Form("/d101/kimy/macro/pixelTrees/rootFiles/%s",fname),"r");
   TTree *trPixMB = (TTree*)f1->Get("anaPixelHitMB/PixelTree");
   TTree *trPixJet = (TTree*)f1->Get("anaPixelHitJet50U/PixelTree");
   
   //   const int nscEtBin = 10;
   //   double scetBin[nscEtBin+1] = { 15.1,18,21,24,28,32,36,40,48,55,70};
   
   TH1D* hhfMB  = new TH1D(Form("hhfMB",trg),";Sum HF Energy (TeV);Fraction of minimum bias events",70,0,160);
   TH1D* hhfMB1  = (TH1D*)hhfMB->Clone("hhfMB1");
   TH1D* hhfMB2  = (TH1D*)hhfMB->Clone("hhfMB2");
   TH1D* hhfMB3  = (TH1D*)hhfMB->Clone("hhfMB3");
   TH1D* hhfJET  = (TH1D*)hhfMB->Clone("hhfJET");

   TCanvas* c1 = new TCanvas(Form("c1_%s",trg),"",400,400);

   trPixMB->Draw("hf/1000.>>hhfMB");
   trPixMB->Draw("hf/1000.>>hhfMB1","cBin<=4");
   trPixMB->Draw("hf/1000.>>hhfMB2","cBin>=4 && cBin<=12");
   trPixMB->Draw("hf/1000.>>hhfMB3","cBin>=12 && cBin<=35");
   
   trPixJet->Draw("hf/1000.>>hhfJET");

   
   handsomeTH1(hhfMB1,1);
   handsomeTH1(hhfMB2,2);
   handsomeTH1(hhfMB3,4);
   hhfMB1->SetFillColor(1);
   hhfMB2->SetFillColor(2);
   hhfMB3->SetFillColor(4);

   
   hhfMB->Scale(1./hhfMB->GetEntries());
   hhfMB->SetAxisRange(1e-5,1,"Y");
   hhfJET->Scale(1./hhfJET->GetEntries());
   hhfJET->SetAxisRange(1e-5,1,"Y");

   hhfMB->SetLineWidth(2);
   handsomeTH1(hhfMB);
   hhfMB->DrawCopy();
   
   //   TLine* t1 = new TLine(.511,1e-5,.511,hhfMB->GetBinContent(hhfMB->FindBin(0.511)));
   TLine* t1 = new TLine(0,1e-5,.511,hhfMB->GetBinContent(hhfMB->FindBin(0.511)));
   TLine* t2 = new TLine(35.397,1e-5,35.397,hhfMB->GetBinContent(hhfMB->FindBin(35.397)));
   TLine* t3 = new TLine(79.370,1e-5,79.370,hhfMB->GetBinContent(hhfMB->FindBin(79.370)));
   t1->SetLineWidth(1);
   t2->SetLineWidth(1);
   t3->SetLineWidth(1);
   t1->SetLineStyle(7);
   t2->SetLineStyle(7);
   t3->SetLineStyle(7);
   t1->Draw();
   t2->Draw();
   t3->Draw();
   
   //  hhfMB3->DrawCopy();
   //  hhfMB2->DrawCopy("same");
   //  hhfMB1->DrawCopy("same");
   gPad->SetLogy();

   TLegend* leg0 = new TLegend(0.2813131,0.7115054,0.9,0.8439785,NULL,"brNDC");
   TLegend* leg0b =new TLegend(0.3813131,0.7115054,1,0.8439785,NULL,"brNDC");
   TLegend* leg1 = new TLegend(0.1767677,0.3826344,0.4467677,0.5875591,NULL,"brNDC");
   TLegend* leg2 = new TLegend(0.3611111,0.3876344,0.6111111,0.5865591,NULL,"brNDC");
   TLegend* leg3 = new TLegend(0.5606061,0.3876344,0.8106061,0.5865591,NULL,"brNDC");

   easyLeg(leg0b,"Centrality, HLT_HIJet50U");

   easyLeg(leg1,"30%-100%");
   easyLeg(leg2,"10%-30% ");
   easyLeg(leg3," 0%-10% ");

   //   TCanvas* c2 = new TCanvas(Form("c2_%s",trg),"",400,400);
   
   float triggerRatio = 0.29 / 49.;
   
   hhfJET->SetLineWidth(2);
   hhfJET->SetLineColor(2);
   hhfJET->SetFillColor(2);
   hhfJET->SetFillStyle(3544);
   hhfJET->Scale(triggerRatio);
   hhfJET->DrawCopy("same");
   
   easyLeg(leg0,"Centrality");
   leg0->AddEntry(hhfMB,"HLT_MinBiasHForBSC_Core","l");
   leg0->AddEntry(hhfJET,"HLT_HiJet50U","l");
   leg0->Draw();
   //   leg1->Draw();
   //   leg2->Draw();
   //   leg3->Draw();
     
   TLatex *bint = new TLatex(0.1867677,0.4876344,"30%-100%");
   bint->SetTextFont(63);
   bint->SetTextSize(13);
   bint->SetNDC();
   bint->Draw();

   bint = new TLatex(0.4011111,0.4876344,"10%-30% ");
   bint->SetTextFont(63);
   bint->SetTextSize(13);
   bint->SetNDC();
   bint->Draw();

   bint = new TLatex(0.6006061,0.4876344," 0%-10% ");
   bint->SetTextFont(63);
   bint->SetTextSize(13);
   bint->SetNDC();
   bint->Draw();

 
   TLatex *cms = new TLatex(0.6013,0.89,"CMS Preliminary");
   cms->SetTextFont(63);
   cms->SetTextSize(16);
   cms->SetNDC();
   cms->Draw();
   c1->SaveAs("hf_centrality_distribution_mb_jet50_20101126_v0.eps");
   
   // TCanvas* c2 = new TCanvas(Form("c2_%s",trg),"",400,400);
   //  hhfJET->Divide(hhfMB);
   //  hhfJET->Draw();
   //   c2->SaveAs("ratio.gif");
   
   //  TLine* t1j = new TLine(.511,1e-5,.511,hhfJET->GetBinContent(hhfJET->FindBin(0.511)));
   //  TLine* t2j = new TLine(35.397,1e-5,35.397,hhfJET->GetBinContent(hhfJET->FindBin(35.397)));
   //  TLine* t3j = new TLine(79.370,1e-5,79.370,hhfJET->GetBinContent(hhfJET->FindBin(79.370)));
   //  t1j->SetLineWidth(2);
   // t2j->SetLineWidth(2);
   //  t3j->SetLineWidth(2);
   //  t1j->Draw();
   //  t2j->Draw();
   // t3j->Draw();
   

   
   //   TLegend* leg0 = new TLegend(0.46,0.80, 0.99, 0.95,NULL,"brNDC");
   //  easyLeg(leg0,"offline cleaned Superlclusters");
   //   leg0->AddEntry(hetSc,"HLT_HIPhoton15");
   //  leg0->AddEntry(hetScClean,"HLT_HICleanPhoton15");
   // leg0->Draw();
   
}

