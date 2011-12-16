
#include "ptresolution.h"
#include "TFile.h"
#include "TTree.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLegendEntry.h"
#include "TF1.h"
#include "TGraphErrors.h"
#include "TLine.h"
#include "TLatex.h"



#include "MultiCanvas.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

void PlotMultiPanels(string algo = "ak3",int mode = 0);

void PlotMultiPanel(){

   PlotMultiPanels("ak3",2);
   if(0){
   PlotMultiPanels("ak3",1);
   PlotMultiPanels("ak3",2);
   PlotMultiPanels("ak3",3);
   }
}

void PlotMultiPanels(string algo,int mode){

   bool absolute = 1;

   bool limitRange = 0;
   bool plotInclusive = 0;
   bool recombinePtHat = 1;

   if(mode == 0) recombinePtHat = 1;
   else recombinePtHat = 0;

   string pthatName[4] = {"combined","pthat30","pthat50","pthat80"};

   double rangeMin = 40;
   double rangeMax = 220;

   if(!limitRange){
      rangeMin = 0;
      rangeMax = 300;
   }

   TCanvas* c1 = new TCanvas("c1","",900,600);
   makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.22,0.22,0.02);

   TLatex latex;
   latex.SetTextFont(63);
   latex.SetTextSize(18);

   double awayShift = 0;

   double binMin[3] = {0,0,50};
   double binMax[3] = {0,50,500};

   TFile* inf = 0;
   //   TFile* inf = new TFile("relative_resolutions_ic5gen_eta0_matchic5_ref-1_jet0.root", "READ");
   TFile* inf30 = new TFile(Form("dijet_%s_resolutions_%s_frank_PtHat30.root",absolute ? "absolute" : "relative",algo.data()));
   TFile* inf50 = new TFile(Form("dijet_%s_resolutions_%s_frank_PtHat50.root",absolute ? "absolute" : "relative",algo.data()));
   TFile* inf80 = new TFile(Form("dijet_%s_resolutions_%s_frank_PtHat80.root",absolute ? "absolute" : "relative",algo.data()));

   TFile* infs[3] = {inf30,inf50,inf80};

   if(mode > 0){
      inf = infs[mode-1];
   }else{
      inf = inf80;
   }

   TFile* inf2 = new TFile(Form("dijet_%s_resolutions_%s_matt_PtHat0.root",absolute ? "absolute" : "relative",algo.data()));

   TF1 * fres = new TF1("fres",res,30,1000,0);
   TLine *l = new TLine(50,1,220,1);

   l->SetLineStyle(2);
   fres->SetLineStyle(2);

   fres->SetLineWidth(1);
   
   TH1D* hRES[10];
   TH1D* hReso[10];
   
   TH1D* pRES = new TH1D("pRES",";p_{T}^{GenJet} (GeV/c);<p_{T}^{RecoJet}/p_{T}^{GenJet}>",100,rangeMin,rangeMax);
   TH1D* pReso = new TH1D("pReso",";p_{T}^{GenJet} (GeV/c);#sigma(p_{T}^{RecoJet}/p_{T}^{GenJet})",100,rangeMin,rangeMax);
   pRES->SetMaximum(1.7);
   pRES->SetMinimum(.7);
   pReso->SetMaximum(0.59);
   pReso->SetMinimum(0);

   if(absolute){
      pRES->SetMaximum(50);
      pRES->SetMinimum(-50);
      pReso->SetMaximum(100);
      pReso->SetMinimum(-100);
   }

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
 
   int cBins[5] = {5,2,0};
   string jRES[3] = {"hEtAw","hEtNr","hEtInc"};
   int color[4] = {2,1,4};

   string centLabel[3] = {
      "50-100%",
      "20-30%",
      "0-10%"
   };

   for(int j = 0; j < 2+plotInclusive ; ++j){
      cout<<"a"<<endl;
      for(int cbin = 0; cbin < nCbins; ++cbin){
	 cout<<"b"<<endl;

	 c1->cd(cbin+1);
         cout<<"b1"<<endl;
	 
	 if(j < 2){
	    cout<<"b2"<<endl;
	    hRES[cbin] = (TH1D*)inf->Get(Form("%s_%d_%d",jRES[j].data(),cBins[cbin],1));
	    cout<<"b3"<<endl;
	 }else{
	    cout<<"b4"<<endl;
	    hRES[cbin] = (TH1D*)inf2->Get(Form("%s_%d_%d",jRES[j].data(),cBins[cbin],1));
	    cout<<"b5"<<endl;
	 }
	 cout<<"c"<<endl;

	 if(recombinePtHat && j < 2){
	    hRES[cbin] = (TH1D*)hRES[cbin]->Clone(Form("%s_recombined",hRES[cbin]->GetName()));
	    hRES[cbin]->Reset();
	    for(int i = 0; i < 3 ; ++i){
	       TH1D* hPTbin = (TH1D*)infs[i]->Get(Form("%s_%d_%d",jRES[j].data(),cBins[cbin],1));
	       for(int ih = 0; ih < hPTbin->GetNbinsX(); ++ih){
		  if(hPTbin->GetBinCenter(ih) < binMin[i] - awayShift*(j == 1) || hPTbin->GetBinCenter(ih) >= binMax[i]- awayShift*(j == 1)) continue;
		  hRES[cbin]->SetBinContent(ih, hPTbin->GetBinContent(ih));
                  hRES[cbin]->SetBinError(ih, hPTbin->GetBinError(ih));
	       }
	    }
	 }
	 cout<<"d"<<endl;

	 if(limitRange) hRES[cbin]->GetXaxis()->SetRange(hRES[cbin]->FindBin(rangeMin),hRES[cbin]->FindBin(rangeMax));
	 else hRES[cbin]->GetXaxis()->SetRange(0,hRES[cbin]->GetNbinsX());

	 cout<<"e"<<endl;
	 hRES[cbin]->SetLineColor(color[j]);
	 hRES[cbin]->SetMarkerColor(color[j]);
	 if(j == 0){
	    hRES[cbin]->SetMarkerStyle(25);
	    pRES->Draw();
	 }
	 hRES[cbin]->Draw("same");
	 l->Draw();
	 cout<<"f"<<endl;

	 latex.DrawLatex(60,1.57,centLabel[cbin].data());

	 if(cbin == 0){
	    TLatex *cms = new TLatex(180,1.57,"CMS");
	    cms->SetTextFont(63);
	    cms->SetTextSize(18);
	    cms->Draw();
	 }

	    if(cbin == 2 && j == 1){
	    TLegend *leg=new TLegend(0.03,0.52,0.41,0.81);
	    leg->SetFillColor(0);
	    leg->SetBorderSize(0);
	    leg->SetFillStyle(0);
	    leg->SetTextFont(63);
            leg->SetTextSize(15);
            leg->AddEntry(hRES[1],"Embedded PYTHIA #sqrt{s}_{NN} = 2.76 TeV","");
	    leg->AddEntry(hRES[1],"Leading Jet Response","pl");
            TLegendEntry* entry = leg->AddEntry("hr","Sub-Leading Jet Response","pl");
	    entry->SetLineColor(color[0]);
            entry->SetMarkerColor(color[0]);
	    entry->SetMarkerStyle(25);

	    leg->Draw();
	 }

	 c1->cd(cbin+4);
         if(j < 2){
	    hReso[cbin] = (TH1D*)inf->Get(Form("%s_%d_%d",jRES[j].data(),cBins[cbin],2));
	 }else{
	    hReso[cbin] = (TH1D*)inf2->Get(Form("%s_%d_%d",jRES[j].data(),cBins[cbin],2));
	 }

	 if(recombinePtHat && j < 2){
            hReso[cbin] = (TH1D*)hReso[cbin]->Clone(Form("%s_recombined",hReso[cbin]->GetName()));
            hReso[cbin]->Reset();
            for(int i = 0; i < 3 ; ++i){
               TH1D* hPTbin = (TH1D*)infs[i]->Get(Form("%s_%d_%d",jRES[j].data(),cBins[cbin],2));
               for(int ih = 0; ih < hPTbin->GetNbinsX(); ++ih){
                  if( hPTbin->GetBinCenter(ih) < binMin[i] - awayShift*(j == 1) || hPTbin->GetBinCenter(ih) >= binMax[i]  - awayShift*(j == 1)) continue;
                  hReso[cbin]->SetBinContent(ih,hPTbin->GetBinContent(ih));
                  hReso[cbin]->SetBinError(ih,hPTbin->GetBinError(ih));
               }
            }
         }

	 if(limitRange) hReso[cbin]->GetXaxis()->SetRange(hReso[cbin]->FindBin(rangeMin),hReso[cbin]->FindBin(rangeMax));
	 else hReso[cbin]->GetXaxis()->SetRange(0,hReso[cbin]->GetNbinsX());

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
            TLegend *leg=new TLegend(0.03,0.64,0.41,0.88);
            leg->SetFillColor(0);
            leg->SetBorderSize(0);
            leg->SetFillStyle(0);
            leg->SetTextFont(63);
	    leg->SetTextSize(15);
	    //            leg->AddEntry(hRES[1],"Embedded PYTHIA #sqrt{s}_{NN} = 2.76 TeV","");
            leg->AddEntry(hReso[1],"Leading Jet Resolution","pl");
            TLegendEntry* entry = leg->AddEntry("hr","Sub-Leading Jet Resolution","pl");
	    entry->SetLineColor(color[0]);
            entry->SetMarkerColor(color[0]);
            entry->SetMarkerStyle(25);
	    //            leg->AddEntry(fres,"Resolution in #font[12]{pp} #sqrt{s} = 7 TeV","l");
            leg->AddEntry(fres,"Resolution in pp #sqrt{s} = 7 TeV","l");
            leg->Draw();
         }



	 
      }

   }

   c1->Print(Form("MultiPanel_JetResponseResolution_%s_%s_%s%s.gif",absolute ? "absolute" : "relative",algo.data(),pthatName[mode].data(),limitRange ? "" : "_fullRange"));
   c1->Print(Form("MultiPanel_JetResponseResolution_%s_%s_%s%s.eps",absolute ? "absolute" : "relative",algo.data(),pthatName[mode].data(),limitRange ? "" : "_fullRange"));

}
