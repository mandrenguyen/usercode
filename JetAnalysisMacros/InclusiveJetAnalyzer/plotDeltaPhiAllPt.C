#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>
#include "TCanvas.h"
#include "TError.h"
#include "TPad.h"
#include "TString.h"
#include "TRandom.h"
#include "TH1F.h"
#include "TF1.h"

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

static const bool central = 1;

//---------------------------------------------------------------------
void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns, 
			  const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0., 
			  const Float_t leftMargin=0.2, 
			  const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);

TString plotDeltaPhi(int cbin = 0,
		 TString infname = "data.root",
		 TString pythia = "pythia.root",
		 TString mix = "mix.root",
		 bool useWeight = true,
		 bool drawXLabel = false,
		 bool drawLeg = false);

void plotPPDPhiAll();

void drawText(const char *text, float xp, float yp);

//--------------------------------------------------------------
// drawPatch() is a crazy way of removing 0 in the second and third 
// pad which is partially shown due to no margin between the pads
// if anybody has a better way of doing it let me know! - Andre
//--------------------------------------------------------------
void drawPatch(float x1, float y1, float x2, float y2); 
//---------------------------------------------------------------------

void plotDeltaPhiAllPt(){

  weightMix();
   TCanvas *c1 = new TCanvas("c1","",1050,700);
   makeMultiPanelCanvas(c1,3,2,0.0,0.0,0.2,0.15,0.02);

   TString label("");

   c1->cd(1);
   label += plotDeltaPhi(0,"data.root","pythia.root","mix.root",true,false,true);
   gPad->SetLogy();
   drawText(label.Data(),0.29,0.46);
   //   drawText("(a)",0.02,0.9);

   TLatex *cms = new TLatex(0.5,1.3,"CMS Preliminary");
   cms->SetTextFont(63);
   cms->SetTextSize(17);
   cms->Draw();


   TLatex *lumi = new TLatex(2.,1,Form("#intL dt = %s #mub^{-1}",LUM));
   lumi->SetTextFont(63);
   lumi->SetTextSize(15);
   lumi->Draw(); 

  c1->cd(2);
  label = plotDeltaPhi(1,"data.root","pythia.root","mix.root",true,false,false);
   gPad->SetLogy();
   drawText(label.Data(),0.19,0.46);
   //   drawText("(b)",0.02,0.9);

   TLatex *jetf;
   jetf = new TLatex(0.5,0.05,"Anti-k_{T} (PFlow), R=0.3");
   jetf->SetTextFont(63);
   jetf->SetTextSize(15);
   jetf->Draw();



   TLatex tsel;
   tsel.SetNDC();
   tsel.SetTextFont(63);
   tsel.SetTextSize(15);
   tsel.DrawLatex(0.15,0.78,Form("p_{T,1} > %d GeV/c",leadCut));
   tsel.DrawLatex(0.15,0.68,Form("p_{T,2} > %d GeV/c",subleadCut));
   
   c1->cd(3);
   label = plotDeltaPhi(2,"data.root","pythia.root","mix.root",true,false,false);
   gPad->SetLogy();
   drawText(label.Data(),0.19,0.46);
   //   drawText("(c)",0.22,0.93);
   if(central){
     drawText("0-20%",0.44,0.6);
   }else{
     drawText("30-100%",0.44,0.6);
   }

   //drawPatch(0.976,0.0972,1.1,0.141);
   c1->cd(4);
   label = plotDeltaPhi(3,"data.root","pythia.root","mix.root",true,false,false);
   gPad->SetLogy();
   drawText(label.Data(),0.29,0.56);
   //   drawText("(d)",0.02,0.93);
   //   drawPatch(-0.00007,0.0972,0.0518,0.141);
   //drawPatch(0.976,0.0972,1.1,0.141);
   //  gPad->SetBottomMargin(0.22);

   c1->cd(5);
   label = plotDeltaPhi(4,"data.root","pythia.root","mix.root",true,true,false);
   gPad->SetLogy();
   drawText(label.Data(),0.19,0.56);
   //   drawText("(e)",0.02,0.93);
   //   drawPatch(-0.00007,0.0972,0.0518,0.141);
   //  gPad->SetBottomMargin(0.22);                                 

   c1->cd(6);
   label = plotDeltaPhi(5,"data.root","pythia.root","mix.root",true,false,false);
   gPad->SetLogy();
   drawText(label.Data(),0.19,0.56);
   //   drawText("(f)",0.02,0.93);

   if(central){
     c1->Print("./fig/dijet_dphi_all_pt_20101126_0to20_v0.gif");
     c1->Print("./fig/dijet_dphi_all_pt_20101126_0to20_v0.eps");
     c1->Print("./fig/dijet_dphi_all_pt_20101126_0to20_v0.pdf");
   }else{
     c1->Print("./fig/dijet_dphi_all_pt_20101126_30to100_v0.gif");
     c1->Print("./fig/dijet_dphi_all_pt_20101126_30to100_v0.eps");
     c1->Print("./fig/dijet_dphi_all_pt_20101126_30to100_v0.pdf");
   }


}



TString plotDeltaPhi(int cbin,
		  TString infname,
		  TString pythia,
		  TString mix,
		  bool useWeight,
		  bool drawXLabel,
		  bool drawLeg)
{

  //  useWeight = 0;

  TString cut=Form("pt1>%d && pt2>%d && abs(eta1) < 2 && abs(eta2) < 2 && bin < %d",leadCut,subleadCut,centralBin);
  TString cutNorm=Form("pt1>%d && abs(eta1) < 2  && bin < %d",leadCut,centralBin);

  TString cutpp=cut;
  if(!central){
    cut=Form("pt1>%f && pt2>%d && abs(eta1) < 2 && abs(eta2) < 2 && bin >= %d",leadCut,subleadCut,centralBin);
    cutNorm=Form("pt1>%d && abs(eta1) < 2  && bin >= %d",leadCut,centralBin);

  }

 
  TString cstring = "";

  int ptMin = 0;
  int ptMax = 0;
  if(cbin==-1) {
    ptMin = 120;
    ptMax = 999;
  } else if (cbin==0) {
    ptMin = 120;
    ptMax = 140;
  } else if (cbin==1) {
    ptMin = 140;
    ptMax = 160;
  } else if (cbin==2) {
    ptMin = 160;
    ptMax = 180;
  } else if (cbin==3) {
    ptMin = 180;
    ptMax = 200;
  } else if (cbin==4) {
    ptMin = 200;
    ptMax = 230;
  } else if (cbin==5) {
    ptMin = 230;
    ptMax = 999;
  }

  cstring = Form("%d < p_{T} < %d",ptMin,ptMax);
  cut+=Form(" && pt1>=%d && pt1<%d",ptMin,ptMax);
  cutNorm+=Form(" && pt1>=%d && pt1<%d",ptMin,ptMax);

  if (cbin==5) {
    cstring = "230 < p_{T}";
  }  

  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");

  // open the pythia (MC) file
  TFile *infPythia = new TFile(pythia.Data());
  TTree *ntPythia = (TTree*) infPythia->FindObjectAny("nt");

  // open the datamix file
  TFile *infMix = new TFile(mix.Data());
  TTree *ntMix =(TTree*)infMix->FindObjectAny("nt");


  //  ntMix->SetAlias("pt1","et1");
  //  ntMix->SetAlias("pt2","et2");

  ntPythia->SetAlias("pt1","et1");
  ntPythia->SetAlias("pt2","et2");

  nt->SetAlias("adphi","acos(cos(phi1-phi2))");
  ntMix->SetAlias("adphi","acos(cos(phi1-phi2))");
  ntPythia->SetAlias("adphi","acos(cos(phi1-phi2))");

  //  ntMix->SetAlias("weight",weightString.Data());

  // projection histogram
  TH1D *h = new TH1D("h","",30,0,3.14159);
  TH1D *hPythia = new TH1D("hPythia","",30,0,3.14159);
  TH1D *hDataMix= new TH1D("hDataMix","",30,0,3.14159);

  TH1D* hNorm = new TH1D("hNorm","",1000,0,1000);
  TH1D* hNormPythia = new TH1D("hNormPythia","",1000,0,1000);
  TH1D* hNormDataMix = new TH1D("hNormDataMix","",1000,0,1000);

  nt->Draw("abs(dphi)>>h",Form("%s && noise < 0",cut.Data())); 
  nt->Draw("pt1>>hNorm",Form("%s && noise < 0",cutNorm.Data()));
   
  if (useWeight) {
    // use the weight value caluculated by Matt's analysis macro
    ntPythia->Draw("abs(dphi)>>hPythia",Form("(%s)",cutpp.Data())); 
    ntMix->Draw("abs(dphi)>>hDataMix",Form("(%s)*(%s)",weightString.Data(),cut.Data())); 
    cout<<"WEEIIGHGTEDD"<<endl;
    ntMix->Draw("pt1>>hNormDataMix",Form("(%s)*(%s)",weightString.Data(),cutNorm.Data()));
    ntPythia->Draw("pt1>>hNormPythia",Form("(%s)",cutNorm.Data()));
  } else {
    // ignore centrality reweighting
    ntPythia->Draw("abs(dphi)>>hPythia",Form("(%s)",cutpp.Data()));
    ntMix->Draw("abs(dphi)>>hDataMix",Form("(%s)",cut.Data()));  
    ntMix->Draw("pt1>>hNormDataMix",Form("(%s)",cutNorm.Data()));
    ntPythia->Draw("pt1>>hNormPythia",Form("(%s)",cutNorm.Data()));
  }

  cout<<"ntMix entries : "<<ntMix->GetEntries()<<endl;

  cout<<"h integral : "<<h->Integral()<<endl;
  cout<<"hNorm integral : "<<hNorm->Integral()<<endl;

  cout<<"hDataMix integral : "<<hDataMix->Integral()<<endl;
  cout<<"hNormDataMix integral : "<<hNormDataMix->Integral()<<endl;

  // calculate the statistical error and normalize
  h->Sumw2();
  if(normLead){
    h->Scale(1./hNorm->Integral());
  }else{
    h->Scale(1./h->Integral());
  }
  h->SetMarkerStyle(20);

  h->SetMarkerColor(dataColor);
  h->SetLineColor(dataColor);

  hPythia->Scale(1./hNormPythia->Integral());
  hPythia->SetLineColor(kBlue);
  hPythia->SetFillColor(kAzure-8);
  hPythia->SetFillStyle(3005);
   
  if(normLead){
    hDataMix->Scale(1./hNormDataMix->Integral());
  }else{
    hDataMix->Scale(1./hDataMix->Integral());
  }
  hDataMix->SetLineColor(mixColor);
  hDataMix->SetFillColor(mixColor);
  hDataMix->SetFillStyle(3004);

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
  hDataMix->SetAxisRange(2e-4,50.6,"Y");
  }
  else{
  hDataMix->SetAxisRange(2e-4,50.6,"Y");
  }
  if(cbin==2){
    hDataMix->GetXaxis()->SetRangeUser(0.,acos(-1.));
  }
  else{
    hDataMix->GetXaxis()->SetRangeUser(0.1,acos(-1.));
  }


  hDataMix->SetMaximum(4);
  //  hDataMix->SetMaximum(40);
  hDataMix->SetMinimum(0.0001);

  hDataMix->Draw("hist");
  //hPythia->Draw("same");
  h->Draw("same");

  if(drawLeg){
    TLegend *t3=new TLegend(0.25,0.63,0.79,0.83);
    t3->AddEntry(h,"PbPb  #sqrt{s}_{_{NN}}=2.76 TeV","p");
    //    t3->AddEntry(h,"2011","p");
    //t3->AddEntry(hPythia,"PYTHIA","lf");
    t3->AddEntry(hDataMix,"PYTHIA+HYDJET 1.6","lf");
    //    t3->AddEntry(hDataMix,"2010","lf");
    t3->SetFillColor(0);
    t3->SetBorderSize(0);
    t3->SetFillStyle(0);
    t3->SetTextFont(63);
    t3->SetTextSize(15);
    t3->Draw();
  }

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


void plotPPDPhiAll(){

  bool isPF = false;

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

  hDijetBal_mc->SetAxisRange(5e-4,6,"Y");

  hDijetBal_mc->Draw("hist");
  hDijetBal_data->Draw("pzsame");


  // Legend
    TLegend *t3a=new TLegend(0.37,0.53,0.69,0.73);
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
  TLatex *cms = new TLatex(0.35,0.33,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(17);
  cms->Draw();

  TLatex *lumi_pp = new TLatex(0.8,0.15,"#intL dt = 35.1 pb^{-1}");
  lumi_pp->SetTextFont(63);
  lumi_pp->SetTextSize(15);
  lumi_pp->Draw();
  
  TLatex *jetf;
  jetf = new TLatex(0.75,0.015,"Anti-k_{T}, R=0.5");
  jetf->SetTextFont(63);
  jetf->SetTextSize(15);
  jetf->Draw();


}
