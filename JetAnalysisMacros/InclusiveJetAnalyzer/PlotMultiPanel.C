
#include "ptresolution.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TF1.h"
#include "TGraphErrors.h"

#include "MultiCanvas.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;


void PlotMultiPanel(){


   TCanvas* c1 = new TCanvas("c1","",900,600);
   makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.22,0.22,0.02);

   TLatex latex;
   latex.SetTextFont(63);
   latex.SetTextSize(18);

   TFile* inf[3];
   inf[0] = new TFile("JES_50to100.root");
   inf[1] = new TFile("JES_20to30.root");
   inf[2] = new TFile("JES_0to10.root");

   TF1 * fres = new TF1("fres",res,30,1000,0);
   TLine *l = new TLine(50,1,220,1);

   l->SetLineStyle(2);
   fres->SetLineStyle(2);

   fres->SetLineWidth(1);
   
   TH1D* hRES[10];
   TH1D* hReso[10];
   
   TH1D* pRES = new TH1D("pRES",";p_{T}^{GenJet} (GeV/c);<p_{T}^{CaloJet}/p_{T}^{GenJet}>",100,50.5,210);
   TH1D* pReso = new TH1D("pReso",";p_{T}^{GenJet} (GeV/c);#sigma(p_{T}^{CaloJet}/p_{T}^{GenJet})",100,50.5,210);
   pRES->SetMaximum(1.7);
   pRES->SetMinimum(.7);
   pReso->SetMaximum(0.59);
   pReso->SetMinimum(0);
   pReso->GetXaxis()->CenterTitle();
   pReso->GetYaxis()->CenterTitle();
   pRES->GetXaxis()->CenterTitle();
   pRES->GetYaxis()->CenterTitle();

   pReso->GetXaxis()->SetNdivisions(505);
   pRES->GetXaxis()->SetNdivisions(505);
   pReso->GetYaxis()->SetNdivisions(505);
   pRES->GetYaxis()->SetNdivisions(505);

   int ts = 21;

   pReso->GetXaxis()->SetLabelFont(63);
   pReso->GetXaxis()->SetLabelSize(ts);
   pReso->GetYaxis()->SetLabelFont(63);
   pReso->GetYaxis()->SetLabelSize(ts);
   pReso->GetXaxis()->SetTitleFont(63);
   pReso->GetXaxis()->SetTitleSize(ts);
   pReso->GetYaxis()->SetTitleFont(63);
   pReso->GetYaxis()->SetTitleSize(ts);
   pReso->GetXaxis()->SetTitleOffset(1.9);
   pReso->GetYaxis()->SetTitleOffset(2.3);


   pRES->GetXaxis()->SetLabelFont(63);
   pRES->GetXaxis()->SetLabelSize(ts);
   pRES->GetYaxis()->SetLabelFont(63);
   pRES->GetYaxis()->SetLabelSize(ts);
   pRES->GetXaxis()->SetTitleFont(63);
   pRES->GetXaxis()->SetTitleSize(ts);
   pRES->GetYaxis()->SetTitleFont(63);
   pRES->GetYaxis()->SetTitleSize(ts);
   pRES->GetXaxis()->SetTitleOffset(1.9);
   pRES->GetYaxis()->SetTitleOffset(2.3);

   int nCbins = 3;

   string jRES[2] = {"hJESAw2D","hJESNr2D"};
   int color[2] = {2,1};

   string centLabel[3] = {
      "50-100%",
      "20-30%",
      "0-10%"
   };

   for(int j = 0; j < 2 ; ++j){
      for(int cbin = 0; cbin < nCbins; ++cbin){
	 c1->cd(cbin+1);
	 hRES[cbin] = (TH1D*)inf[cbin]->Get(Form("%s_%d",jRES[j].data(),1));
	 hRES[cbin]->GetXaxis()->SetRange(hRES[cbin]->FindBin(50),hRES[cbin]->FindBin(220));
	 hRES[cbin]->SetLineColor(color[j]);
	 hRES[cbin]->SetMarkerColor(color[j]);
	 if(j == 0){
	    hRES[cbin]->SetMarkerStyle(25);
	    pRES->Draw();
	 }
	 hRES[cbin]->Draw("same");
	 l->Draw();

	 latex.DrawLatex(60,1.57,centLabel[cbin].data());

	 if(cbin == 0){
	    TLatex *cms = new TLatex(180,1.57,"CMS");
	    cms->SetTextFont(63);
	    cms->SetTextSize(18);
	    cms->Draw();
	 }

	    if(cbin == 2 && j == 1){
	    TLegend *leg=new TLegend(0.03,0.54,0.41,0.83);
	    leg->SetFillColor(0);
	    leg->SetBorderSize(0);
	    leg->SetFillStyle(0);
	    leg->SetTextFont(63);
            leg->SetTextSize(15);
            leg->AddEntry(hRES[1],"Embedded PYTHIA","");
	    leg->AddEntry(hRES[1],"Leading Jet Response","pl");
            TLegendEntry* entry = leg->AddEntry("hr","Sub-Leading Jet Response","pl");
	    entry->SetLineColor(color[0]);
            entry->SetMarkerColor(color[0]);
	    entry->SetMarkerStyle(25);

	    leg->Draw();
	 }

	 c1->cd(cbin+4);
	 hReso[cbin] = (TH1D*)inf[cbin]->Get(Form("%s_%d",jRES[j].data(),2));
	 hReso[cbin]->GetXaxis()->SetRange(hReso[cbin]->FindBin(50),hReso[cbin]->FindBin(220));
	 hReso[cbin]->SetLineColor(color[j]);
	 hReso[cbin]->SetMarkerColor(color[j]);
	 if(j == 0){
            hReso[cbin]->SetMarkerStyle(25);
	    pReso->Draw();
	 }
	 hReso[cbin]->Draw("same");
	 fres->Draw("same");

         latex.DrawLatex(60,0.52,centLabel[cbin].data());

         if(cbin == 2 && j == 1){
            TLegend *leg=new TLegend(0.03,0.63,0.41,0.88);
            leg->SetFillColor(0);
            leg->SetBorderSize(0);
            leg->SetFillStyle(0);
            leg->SetTextFont(63);
	    leg->SetTextSize(15);
            leg->AddEntry(hReso[1],"Leading Jet Resolution","pl");
            TLegendEntry* entry = leg->AddEntry("hr","Sub-Leading Jet Resolution","pl");
	    entry->SetLineColor(color[0]);
            entry->SetMarkerColor(color[0]);
            entry->SetMarkerStyle(25);
            leg->AddEntry(fres,"Resolution in p+p","l");
            leg->Draw();
         }



	 
      }

   }

   c1->Print("MultiPanel_JetResponseResolution.gif");
   c1->Print("MultiPanel_JetResponseResolution.eps");
   c1->Print("MultiPanel_JetResponseResolution.C");

}
