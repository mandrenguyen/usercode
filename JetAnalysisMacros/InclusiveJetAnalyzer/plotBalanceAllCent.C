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
  if(1)plotPPBalanceAll();
  drawText("(a)",0.25,0.885);

  c1->cd(2);
  plotBalance(4,"data.root","pythia.root","mix.root",true,false,true);
  drawText("50-100%",0.7,0.1);
  drawText("(b)",0.05,0.885);


  TLatex *jetf_PbPb;
  jetf_PbPb = new TLatex(0.477,0.14,"Anti-k_{T} (PFlow), R=0.3");
  jetf_PbPb->SetTextFont(63);
  jetf_PbPb->SetTextSize(15);
  jetf_PbPb->Draw();


  TLatex *lumi_PbPb = new TLatex(0.50,0.23,"#intL dt = 6.7 #mub^{-1}");
  lumi_PbPb->SetTextFont(63);
  lumi_PbPb->SetTextSize(15);
//  lumi_PbPb->Draw();

  c1->cd(3);
  plotBalance(3,"data.root","pythia.root","mix.root",true,false,false);
  drawText("30-50%",0.7,0.1);
  drawText("(c)",0.05,0.885);

  TLatex tsel;
  tsel.SetNDC();
  tsel.SetTextFont(63);
  tsel.SetTextSize(15);
  tsel.DrawLatex(0.55,0.75,"p_{T,1} > 120 GeV/c");
  tsel.DrawLatex(0.55,0.65,"p_{T,2} > 50 GeV/c");
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


  c1->Print("./fig/dijet_imbalance_all_cent_20101126_v0.gif");
  c1->Print("./fig/dijet_imbalance_all_cent_20101126_v0.eps");
  c1->Print("./fig/dijet_imbalance_all_cent_20101126_v0.pdf");

}

void plotBalance(int cbin,
		 TString infname,
		 TString pythia,
		 TString mix,
		 bool useWeight,
		 bool drawXLabel,
		 bool drawLeg)
{

  useWeight = 0;// forced!

  TString cut="pt1>120 && pt2>50 && abs(dphi)>2.0944 && abs(eta1) < 2 && abs(eta2) < 2";
  TString cutpp="pt1>120 && pt2>50 && abs(dphi)>2.0944  && abs(eta1) < 2 && abs(eta2) < 2";

  TString cstring = "";
  if(cbin==-1) {
     cstring = "0-100%";
     cut+=" && bin>=0 && bin<40";
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

  // open the pythia (MC) file
  TFile *infPythia = new TFile(pythia.Data());
  TTree *ntPythia = (TTree*) infPythia->FindObjectAny("nt");

  // open the datamix file
  TFile *infMix = new TFile(mix.Data());
  TTree *ntMix =(TTree*)infMix->FindObjectAny("nt");

  // projection histogram
  TH1D *h = new TH1D("h","",25,0,1.5);
  TH1D *hPythia = new TH1D("hPythia","",25,0,1.5);
  TH1D *hDataMix;

  if(0){
  ntMix->SetAlias("pt1","et1");
  ntMix->SetAlias("pt2","et2");
  }

  ntPythia->SetAlias("pt1","et1");
  ntPythia->SetAlias("pt2","et2");
  nt->SetAlias("adphi","acos(cos(phi1-phi2))");
  ntMix->SetAlias("adphi","abs(dphi)");
  ntPythia->SetAlias("adphi","abs(dphi)");


  if(cbin==2) hDataMix= new TH1D("hDataMix","",25,0,1.5);
  else if(cbin==0) hDataMix= new TH1D("hDataMix","",25,0.0001,1.5);
  else hDataMix= new TH1D("hDataMix","",25,0.0001,1.5);

  nt->Draw("(pt1-pt2)/(pt1+pt2)>>h",Form("(%s)",cut.Data())); 
   
  if (useWeight) {
    // use the weight value caluculated by Matt's analysis macro
    ntMix->Draw("(pt1-pt2)/(pt1+pt2)>>hDataMix",Form("(%s)*weight",cut.Data())); 
  } else {
    // ignore centrality reweighting
    ntMix->Draw("(pt1-pt2)/(pt1+pt2)>>hDataMix",Form("(%s)",cut.Data()));  
  }
  ntPythia->Draw("(pt1-pt2)/(pt1+pt2)>>hPythia",Form("(%s)",cutpp.Data()));

  // calculate the statistical error and normalize
  h->SetLineColor(kRed);
	  h->SetMarkerColor(kRed);
	h->Sumw2();
  h->Scale(1./h->GetEntries());
  h->SetMarkerStyle(20);

  hPythia->Scale(1./hPythia->Integral(0,20));
  hPythia->SetLineColor(kBlue);
  hPythia->SetFillColor(kAzure-8);
  hPythia->SetFillStyle(3005);

  hDataMix->Scale(1./hDataMix->Integral(0,20));
  hDataMix->SetLineColor(kBlue);
  hDataMix->SetFillColor(kBlue-9);
  hDataMix->SetFillStyle(3004);
  
  hDataMix->SetMarkerSize(0);
  hDataMix->SetStats(0);

  hDataMix->GetXaxis()->SetLabelSize(22);
  hDataMix->GetXaxis()->SetLabelFont(43);
  hDataMix->GetXaxis()->SetTitleSize(24);
  hDataMix->GetXaxis()->SetTitleFont(43);
  hDataMix->GetXaxis()->SetTitleOffset(2.4);
  hDataMix->GetXaxis()->CenterTitle();
  
  
  hDataMix->GetYaxis()->SetLabelSize(22);
  hDataMix->GetYaxis()->SetLabelFont(43);
  hDataMix->GetYaxis()->SetTitleSize(22);
  hDataMix->GetYaxis()->SetTitleFont(43);
  hDataMix->GetYaxis()->SetTitleOffset(3);
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

  cout<<" mean value of data "<<h->GetMean()<<endl;

  if(drawLeg){
    TLegend *t3=new TLegend(0.44,0.6,0.89,0.95); 
    t3->AddEntry(h,"PbPb  #sqrt{s}_{_{NN}}=2.76 TeV","p");
    t3->AddEntry(h,"2011 #intL dt = 40 #mub^{-1}","");
    //t3->AddEntry(hPythia,"PYTHIA","lf");  
    t3->AddEntry(hDataMix,"PYTHIA+DATA","lf");
    //    t3->AddEntry(hDataMix,"2010 #intL dt = 6.7 #mub^{-1}","lf");
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
