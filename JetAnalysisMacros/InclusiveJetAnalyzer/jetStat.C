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
#include "/home/kimy/script/commonUtility.h"

#endif


void jetStat() {
   
   TCanvas *c1 = new TCanvas("c1","",700,400);
   makeMultiPanelCanvas(c1,2,1,0.0,0.0,0.2,0.15,0.02);
   c1->cd(2);
   TString cut="et1>120 && et2>50 && dphi>2.5";
   TString cstring = "";
   
   // open the data file                                                                                                                                                                
   TFile *inf = new TFile("data_3ub.root");
   TTree *nt =(TTree*)inf->FindObjectAny("nt");
   
   TH1D *h = new TH1D("h","centrality bin",10,0,100);
   nt->Draw("bin*2.5 + 1>>h",cut);
   h->Scale(9./3.4);
   h->SetAxisRange(0.5,1e4,"Y");
   h->Draw();
   gPad->SetLogy();
   // calculate the statistical error and normalize
   h->Sumw2();
   //   h->Scale(1./h->GetEntries());
   handsomeTH1(h,1);
   
   TH1F* h_int = (TH1F*)h->Clone("h_int");
   float inte[20];
   for  ( int i=1 ; i<=10 ; i++) {
      inte[i] = h->Integral(i,-1);
      cout << int(h->GetBinLowEdge(i)) << "% ~  : " << inte[i] << endl;
      h_int->SetBinContent( i,  inte[i] ) ;
   }

   TLegend* leg0 = new TLegend(0.5813131,0.7115054,0.9,0.8439785,NULL,"brNDC");
   easyLeg(leg0,"#intL dt = 9 #mub^{-1}");
   leg0->Draw();

   
   c1->cd(1);
   h_int->SetYTitle(" Integrated Entries"); 
   h_int->Draw();
   gPad->SetLogy();
   gPad->SetGridy();
   gPad->SetGridx();
   cout << " 1562 * 9 / 3.4 = " << 1562. * 9. / 3.4 << endl;
   
   TCanvas *c1_cum = new TCanvas("c1_cum","",400,400);
   TH1F* h_cum = (TH1F*)h->Clone("h_cum");
   for  ( int i=1 ; i<=10 ; i++) {
      inte[i] = h->Integral(1,i);
      cout << int(h_cum->GetBinLowEdge(i)) << "% ~  : " << inte[i] << endl;
      h_cum->SetBinContent( i,  inte[i] ) ;
   }
   h_cum->Sumw2();
   h_cum->Scale(1./ h->Integral(1,10) );
   h_cum->SetYTitle("accum. fraction");
   h_cum->SetXTitle("Centrality");
   
   h_cum->SetAxisRange(0,1.1,"Y");
   h_cum->Draw();
   gPad->SetGridy();
   gPad->SetGridx();
   leg0->Draw();

   TCanvas *c2 = new TCanvas("c2","",700,400);
   makeMultiPanelCanvas(c2,2,1,0.0,0.0,0.2,0.15,0.02);

   TH1D *h2 = new TH1D("h2",";Leading jet pT (GeV);Entries",9,120,300);
   cut += " && bin<12";
   nt->Draw("et1>>h2",cut);
   h2->Sumw2();
   handsomeTH1(h2,1);
   h2->Scale(9./3.4);
   h2->SetAxisRange(30,5e3,"Y");
   h2->SetNdivisions(5);
   h2->Draw();
   gPad->SetLogy();
   
   
   TH1F* h2_int = (TH1F*)h2->Clone("h_int");
   for  ( int i=1 ; i<=9 ; i++) {
      inte[i] = h2->Integral(i,-1);
      cout << int(h2->GetBinLowEdge(i)) << "GeV ~  : " << inte[i] << endl;
      h2_int->SetBinContent( i,  inte[i] ) ;
   }
   leg0->Draw();
   
   TLegend* leg1 = new TLegend(0.3813131,0.6115054,0.95,0.7439785,NULL,"brNDC");
   easyLeg(leg1,"0~10%");
   leg1->Draw();
   
   c2->cd(1);
   h2_int->SetYTitle("Entries");
   h2_int->Draw();
   gPad->SetLogy();
   gPad->SetGridy();
   gPad->SetGridx();
      

   TCanvas *c2_cum = new TCanvas("c2_cum","",400,400);
   TH1F* h2_cum = (TH1F*)h2->Clone("h2_cum");
   for  ( int i=1 ; i<=9 ; i++) {
      inte[i] = h2->Integral(1,i);
      cout << int(h2_cum->GetBinLowEdge(i)) << "% ~  : " << inte[i] << endl;
      h2_cum->SetBinContent( i,  inte[i] ) ;
   }
   h2_cum->Sumw2();
   h2_cum->Scale(1./ h2->Integral(1,9) );
   h2_cum->SetYTitle("accum. fraction");
   h2_cum->SetXTitle("Leading Jet pT (GeV)");

   h2_cum->SetAxisRange(0,1.1,"Y");
   h2_cum->Draw();
   gPad->SetGridy();
   gPad->SetGridx();
   leg0->Draw();

   
   
}
