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
#include "TStyle.h"

#endif


//---------------------------------------------------------------------


void makeMultiPanelCanvas(TCanvas*& canv, const Int_t columns,
                          const Int_t rows, const Float_t leftOffset=0.,
                          const Float_t bottomOffset=0.,
                          const Float_t leftMargin=0.2,
                          const Float_t bottomMargin=0.2,
                          const Float_t edge=0.05);

void plotBal_vs_Imbal(int cbin = 0,
		  TString infname = "data.root",
	          bool useWeight = true,
		  bool drawXLabel = false,
		      bool drawLeg = false,
		      int kind = 0    // kind = 0 for data, 1 for mix 2 for pythia
		      );

void drawText(const char *text, float xp, float yp);
void drawDum(float min, float max, double drawXLabel);

//--------------------------------------------------------------
// drawPatch() is a crazy way of removing 0 in the second and third 
// pad which is partially shown due to no margin between the pads
// if anybody has a better way of doing it let me know! - Andre
//--------------------------------------------------------------
void drawPatch(float x1, float y1, float x2, float y2); 
//---------------------------------------------------------------------

void plot_lead_vs_subletd_AllCent() {
   
  gStyle->SetNdivisions(505,"xyz");

   TCanvas *c1 = new TCanvas("c1","",1250,1100);
   
   //makeMultiPanelCanvas(c1,3,3,0.0,0.0,0.2,0.15,0.02);
   c1->Divide(3,3,0.,0.);

   c1->cd(1);
   plotBal_vs_Imbal(2,"data.root",true,false,false,0);
   float px(0.25),py(0.75);
   drawText("30-100%",px,py);
   //  drawPatch(0.976,0.0972,1.1,0.141);
   

  c1->cd(2);
  plotBal_vs_Imbal(1,"data.root",true,true,false,0);

  drawText("10-30%",0.06,py);
  //  drawPatch(-0.00007,0.0972,0.0518,0.141);
  //  drawPatch(0.976,0.0972,1.1,0.141);

  drawText("#Delta#phi_{12} > #frac{2}{3}#pi rad",0.06,py+0.12);



  c1->cd(3);
  plotBal_vs_Imbal(0,"data.root",true,false,true,0);

  drawText("0-10%",0.05,py);
  //drawPatch(-0.00007,0.0972,0.0518,0.141);

  c1->cd(1);
  TLatex *datalabel = new TLatex(130,225,"PbPb  #sqrt{s}_{_{NN}}=2.76 TeV");
  datalabel->SetTextFont(63);
  datalabel->SetTextSize(18);
  datalabel->Draw();

  c1->cd(3);
  TLatex *cms = new TLatex(130,225,"CMS");
  cms->SetTextFont(63);
  cms->SetTextSize(18);
  cms->Draw();
  TLatex *lumi = new TLatex(155,225,"#intL dt = 6.7 #mub^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(18);
  lumi->Draw();
  
  c1->cd(3);
  gPad->SetRightMargin(0.15);

 
  c1->cd(4);
  plotBal_vs_Imbal(2,"mix.root",true,false,false,1);
  drawText("30-100%",px,py);
  //drawPatch(0.06,0.912,0.19,0.999);
  c1->cd(5);
  plotBal_vs_Imbal(1,"mix.root",true,true,false,1);
  //  gPad->SetLogy();
  drawText("10-30%",0.05,py);
  //  drawPatch(-0.00007,0.0972,0.0518,0.141);
  //  drawPatch(0.966,0.0972,1.1,0.141);
  
  c1->cd(6);
  plotBal_vs_Imbal(0,"mix.root",true,false,true,1);
  drawText("0-10%",0.05,py);
  //drawPatch(-0.00007,0.0972,0.0318,0.141);
  gPad->SetRightMargin(0.15);
 
 
  //gPad->SetRightMargin(0.1);

  c1->cd(4);
  TLatex *mix = new TLatex(125,225,"PYTHIA+DATA");
  mix->SetTextFont(63);
  mix->SetTextSize(18);
  mix->Draw();
  

  /*
  c1->cd(7);
  plotBal_vs_Imbal(2,"pythia.root",true,false,false,2);
  drawText("30-100%",65,170);
  //drawPatch(0.966,0.0972,1.1,0.141);
  //drawPatch(0.06,0.922,0.19,0.999);
  */
  c1->cd(7);
  plotBal_vs_Imbal(1,"pythia.root",true,true,false,2);
  //drawText("10-30%",65,170);
  //drawPatch(-0.00007,0.0972,0.1318,0.141);
  //drawPatch(0.956,0.0992,1.1,0.141);
  /*
  c1->cd(9);
  plotBal_vs_Imbal(0,"pythia.root",true,false,true,2);
  drawText("0-10%",65,170);
  drawPatch(-0.00007,0.0972,0.1318,0.141);
  */
  TLatex *pythia = new TLatex(125,225,"PYTHIA");
  pythia->SetTextFont(63);
  pythia->SetTextSize(18);

  
  pythia->Draw();
  

  //gPad->SetRightMargin(0.1);



  c1->Print("./fig/dijet_lead_vs_sublead_et_all_cent_20101126_v0.gif");
  c1->Print("./fig/dijet_lead_vs_sublead_et_all_cent_20101126_v0.eps");
  c1->Print("./fig/dijet_lead_vs_sublead_et_all_cent_20101126_v0.pdf");

}



void plotBal_vs_Imbal(int cbin,
		      TString infname,
		      bool useWeight,
		      bool drawXLabel,
		      bool drawLeg,
		      int kind
		      )
{
  
   TString cut="dphi>2./3.*acos(-1.) &&";//  "et1>120 && et2>50";

   TString cstring = "";
  if(cbin==0) {
    cstring = "0-10%";
    cut+="  bin>=0 && bin<4";
  } else if (cbin==1) {
    cstring = "10-30%";
    cut+="  bin>=4 && bin<12";

  } else {
    cstring = "30-100%";
    cut+="  bin>=12 && bin<40";
  }
  
  //  TString cutBal   = cut+"&& ((et1-et2)/(et1+et2) < 0.3)";
  //  TString cutImBal = cut+"&& ((et1-et2)/(et1+et2) > 0.3)";

  // open the data file
  TFile *inf = new TFile(infname.Data());
  TTree *nt =(TTree*)inf->FindObjectAny("nt");
  // projection histogram
  TH2D *h = new TH2D("h","",28,120.001,259.999,40,50.001,249.999);
  h->GetZaxis()->SetRangeUser(0.001,4.2);
  char* var="et2:et1";
  
  cout<<" infname.Data() "<<infname.Data()<<endl;
  if(infname.BeginsWith("pythia")){
    nt->Draw(Form("%s>>h",var)); 
  }
  else nt->Draw(Form("%s>>h",var),Form("(%s)",cut.Data())); 
    
  // calculate the statistical error and normalize
  //  hBal->Sumw2();
  h->Scale(1./h->GetEntries());
  h->Scale(1000);
  //    hBal->SetMarkerStyle(24);
  //    hImBal->Sumw2();
  //    hImBal->Scale(1./hImBal->GetEntries());
  //    hImBal->SetMarkerStyle(20);

   
  h->SetStats(0);
  if(drawXLabel) h->SetXTitle("Leading jet p_{T} (GeV/c)");
  h->GetXaxis()->SetLabelSize(20);
  h->GetXaxis()->SetLabelFont(43);
  h->GetXaxis()->SetTitleSize(22);
  h->GetXaxis()->SetTitleFont(43);
  h->GetXaxis()->SetTitleOffset(2.6);
  h->GetXaxis()->CenterTitle();
  h->SetYTitle("Subleading jet p_{T} (GeV/c)");
  h->GetYaxis()->SetLabelSize(20);
  h->GetYaxis()->SetLabelFont(43);
  h->GetYaxis()->SetTitleSize(20);
  h->GetYaxis()->SetTitleFont(43);
  h->GetYaxis()->SetTitleOffset(3.1);
  h->GetYaxis()->CenterTitle();
  gPad->SetLogz();

  if(cbin!=0)h->GetZaxis()->SetTickLength(0);
  h->Draw("zcol");
    
  TLegend *t3[10]; 

  if(drawLeg){
    t3[kind]=new TLegend(0.05,0.69,0.59,0.85);

    char* legOps;
    if ( kind ==0)  legOps = "pl";
    else  legOps = "lf";
    //    t3[kind]->AddEntry(h,"Balanced jet ( AJ<0.3)",legOps); //unquenched PYTHIA + HYDJET","lf");
    //  t3[kind]->AddEntry(hImBal,"Imbalanced jet (AJ>0.3)",legOps);//DataMix,"unquenched PYTHIA + Data","lf");
    //  t3[kind]->SetFillColor(0);
    //    t3[kind]->SetBorderSize(0);
    //  t3[kind]->SetFillStyle(0);
    //  t3[kind]->SetTextFont(63);
    //  t3[kind]->SetTextSize(15);
 //   t3[kind]->Draw();
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

void drawDum(float min, float max, double drawXLabel){

  TH1D *hdum = new TH1D("hdum","",20,0,1);
  hdum->SetMaximum(max);

  hdum->SetStats(0);

  if(drawXLabel) hdum->SetXTitle("(p_{T}^{j1}-p_{T}^{j2})/(p_{T}^{j1}+p_{T}^{j2})");
  hdum->GetXaxis()->SetLabelSize(20);
  hdum->GetXaxis()->SetLabelFont(43);
  hdum->GetXaxis()->SetTitleSize(22);
  hdum->GetXaxis()->SetTitleFont(43);
  hdum->GetXaxis()->SetTitleOffset(1.5);
  hdum->GetXaxis()->CenterTitle();

  hdum->GetXaxis()->SetNdivisions(905,true);

  hdum->SetYTitle("Event Fraction");

  hdum->GetYaxis()->SetLabelSize(20);
  hdum->GetYaxis()->SetLabelFont(43);
  hdum->GetYaxis()->SetTitleSize(20);
  hdum->GetYaxis()->SetTitleFont(43);
  hdum->GetYaxis()->SetTitleOffset(2.5);
  hdum->GetYaxis()->CenterTitle();

  hdum->SetAxisRange(0,0.2,"Y");

  hdum->Draw("");

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
