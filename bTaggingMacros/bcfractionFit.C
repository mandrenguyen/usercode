#include <TFile.h>
#include <TH1.h>
#include <TTree.h>
#include <TCanvas.h>

#ifndef __CINT__
#include <RooRealVar.h>
#include <RooDataHist.h>
#include <RooDataSet.h>
#include <RooHistPdf.h>
#include <RooPlot.h>
#include <RooAddPdf.h>
#include <RooGlobalFunc.h>
#endif

using namespace RooFit;

void fixEmpty(TH1 *h)
{
   for (int i=1;i<=h->GetNbinsX();i++)
   {
      if (h->GetBinContent(i)==0) h->SetBinContent(i,1e-20);
   }
}

RooRealVar bcfractionFit(char *var = "discr_csvSimple",double minX = 0,double maxX = 1,double ptMin = 60, double ptMax = 500)
{
   // Prepare a canvas
   TCanvas *c = new TCanvas("c","",600,600);
   c->SetLogy();
   

   // MC shape file
   TFile *inf = new TFile("histos/ppMC.root");
   TTree *t = (TTree*) inf->Get("nt");

   // b-jet signal shape
   TH1D *hB = new TH1D("hB","",50,minX,maxX);
   hB->Sumw2();
   t->Draw(Form("%s>>hB",var),Form("abs(refparton_flavorForB)==5&&jtpt>%f&&jtpt<%f",ptMin,ptMax));
   fixEmpty(hB);

   TH1D *hC = new TH1D("hC","",50,minX,maxX);
   hC->Sumw2();
   t->Draw(Form("%s>>hC",var),Form("abs(refparton_flavorForB)==4&&jtpt>%f&&jtpt<%f",ptMin,ptMax));
   fixEmpty(hC);
   
   // b-jet background shape
   TH1D *hOtherFlavor = new TH1D("hOtherFlavor","",50,minX,maxX);
   hOtherFlavor->Sumw2();
   t->Draw(Form("%s>>hOtherFlavor",var),Form("abs(refparton_flavorForB)!=5&&abs(refparton_flavorForB)!=4&&jtpt>%f&&jtpt<%f",ptMin,ptMax));
   fixEmpty(hOtherFlavor);
   
   // data sample   
   TFile *infData = new TFile("histos/ppdata.root");
   TTree *tData = (TTree*) infData->Get("nt");
   
   // --- Observable ---
   RooRealVar s(var,var,0,minX,maxX);
   RooRealVar jtpt("jtpt","jtpt",0,ptMin,ptMax);
 
   // --- Parameters ---
 
   // --- Build Histogram PDF ---
   RooDataHist xB("xB","xB",s,hB);
   RooDataHist xC("xB","xB",s,hC);
   RooHistPdf signal("signal","signalB PDF",s,xB);
   RooHistPdf signalC("signalC","signalC PDF",s,xC);

   RooDataHist xOtherFlavor("xOtherFlavor","xOtherFlavor",s,hOtherFlavor);
   RooHistPdf background("background","Background PDF",s,xOtherFlavor);

   // --- Construct signal+background PDF ---
   // signal = bjets
   // background = all the other flavors
   //   RooRealVar nsig("nsig","#signal events",6000,0.,1e7) ;
   //   RooRealVar nbkg("nbkg","#background events",1e5,0.,1e7) ;
   //   RooAddPdf model("model","g+a",RooArgList(signal,background),RooArgList(nsig,nbkg)) ;
   RooRealVar frac("frac","#background events",0.1,0.,1) ;
   RooRealVar fracC("fracC","#background events",0.1,0.,1) ;
   RooAddPdf modelB("modelB","g+a",signalC,background,fracC) ;
   RooAddPdf model("model","g+a",signal,modelB,frac) ;

   // data sample
   //RooDataSet *data = new RooDataSet("data","data",RooArgSet(s),Import(*tData));
   RooDataSet *data = new RooDataSet("data","data",tData,RooArgSet(s,jtpt),Form("jtpt>%f&&jtpt<%f",ptMin,ptMax));
   
   // --- Perform extended ML fit of composite PDF to data ---
   model.fitTo(*data) ;
 
   // --- Plot data and composite PDF overlaid ---
   RooPlot* sframe = s.frame() ;
   TH2D *htemp = new TH2D("htemp","",100,minX,maxX,100,0.5,1e5) ;
   htemp->SetXTitle(Form("%s %.0f < p_{T} < %.0f GeV/c",var,ptMin,ptMax));
   htemp->SetYTitle("Entries");
   htemp->Draw();
   cout <<"Min "<<sframe->GetMinimum()<<endl;
   data->plotOn(sframe,Binning(50)) ;
   sframe->SetTitle("");
   model.plotOn(sframe,Components(background),LineStyle(kDashed),LineColor(kBlack)) ;   
   model.plotOn(sframe,Components(modelB),LineStyle(kDashed),LineColor(kBlue)) ;   
   model.plotOn(sframe,Components(signal),LineStyle(kDashed),LineColor(kRed),FillColor(kRed),FillStyle(1)) ;   
   model.plotOn(sframe) ;

   cout <<"b jet fraction = "<<frac.getVal()<<endl;
   sframe->Draw("same");
   c->SaveAs(Form("fit/%s-%.0f-%.0f.gif",var,ptMin,ptMax));
   return frac;
}

void ptDependence()
{
   const int nBins = 8;
   double ptBin[nBins+1] = {60,70,80,90,100,120,140,160,200};
   TH1D *hProb = new TH1D("hProb","",nBins,ptBin);
   TH1D *hCSV = new TH1D("hCSV","",nBins,ptBin);

   for (int n=0; n<nBins;n++)
   {
      RooRealVar f1 = bcfractionFit("discr_prob",0,3.5,ptBin[n],ptBin[n+1]);
      RooRealVar f2 = bcfractionFit("discr_csvSimple",0,1,ptBin[n],ptBin[n+1]);
      hProb->SetBinContent(n+1,f1.getVal());    
      hProb->SetBinError(n+1,f1.getError());    
      hCSV->SetBinContent(n+1,f2.getVal());    
      hCSV->SetBinError(n+1,f2.getError());    
   }
   TCanvas *c2 = new TCanvas("c2","",600,600);
   hProb->SetAxisRange(0,0.05,"Y");
   hProb->SetXTitle("Jet p_{T} (GeV/c)");
   hProb->SetYTitle("b-jet fraction");
   hProb->SetTitleOffset(1.5,"Y");
   hProb->Draw();
   hCSV->SetLineColor(2);
   hCSV->SetMarkerColor(2);
   hCSV->SetMarkerStyle(24);
   hCSV->Draw("same");
   
   TLegend *leg = new TLegend(0.2,0.7,0.5,0.9);
   leg->SetBorderSize(0);
   leg->SetFillStyle(0);
   leg->SetFillColor(0);
   leg->AddEntry(hProb,"Jet Probability","pl");
   leg->AddEntry(hCSV,"CSV","pl");
   leg->Draw();
}