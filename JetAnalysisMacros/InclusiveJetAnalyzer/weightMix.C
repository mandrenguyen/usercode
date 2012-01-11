#include <iostream>
#include "TCanvas.h"
#include "TError.h"
#include "TPad.h"
#include "TString.h"
#include "TRandom.h"
#include "TH1F.h"
#include "TMath.h"
#include "TF1.h"

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TString.h"
#include "TChain.h"

#include "TCut.h"
#include "TNtuple.h"

#include "THStack.h"

using namespace std;

static TString weightString;

static bool normLead = 1;

static int mixColor = 2;
static int dataColor = 1;

static int centralBin = 8;
static int leadCut = 120;
static int subleadCut = 30;

static double sideMin = 0.;
static double sideMax = TMath::Pi()/3 + 0.;

static double sideCorrect = 1;

static const char* LUM = "150";

static bool plotSubtraction = 0;

static bool reweightCentrality = 1;

TCut weightMix(){

  TH1::SetDefaultSumw2();
  bool validate = 1;
  validate = 0;

  double xs30 = 1.079E-02 * 1000000;
  double xs50 = 1.021E-03 * 1000000;
  double xs80 = 9.913E-05 * 1000000;
  double xs120 = 1.128E-05 * 1000000;
  double xs170 = 1.470E-06 * 1000000;
  double xs200 = 5.310E-07 * 1000000;

  xs30  -= xs50;
  xs50  -= xs80;
  xs80  -= xs120;
  xs120 -= xs170;
  xs170 -= xs200;

  TFile* outf = new TFile("weights.root","recreate");
  TNtuple* ntw = new TNtuple("ntw","","weight:ptweight:cweight");

  float varpthat, varbin;
  TNtuple* nt = (TNtuple*)(new TFile("mix.root"))->Get("nt");

  nt->SetBranchAddress("pthat",&varpthat);
  nt->SetBranchAddress("bin",&varbin);

  int nev= 0,nev30 = 0, nev50 = 0, nev80 = 0, nev120 = 0, nev170 = 0, nev200 = 0;
  int n = 0, n30 = 0, n50 = 0, n80 = 0, n120 = 0, n170 = 0, n200 = 0;

  nev = nt->GetEntries();

  TCut pthat30("pthat >= 30 && pthat < 50");
  TCut pthat50("pthat >= 50 && pthat < 80");
  TCut pthat80("pthat >= 80 && pthat < 120");
  TCut pthat120("pthat >= 120 && pthat < 170");
  TCut pthat170("pthat >= 170 && pthat < 200");
  TCut pthat200("pthat >= 200");

  n = nev;
  n30 = nt->GetEntries(pthat30);
  //  n50 = nt->GetEntries(pthat50);
  n80 = nt->GetEntries(pthat80);
  n120 = nt->GetEntries(pthat120);
  n170 = nt->GetEntries(pthat170);
  n200 = nt->GetEntries(pthat200);

  TCut w30(Form("%f/%d",xs30,n30));
  TCut w80(Form("%f/%d",xs80,n80));
  TCut w120(Form("%f/%d",xs120,n120));
  TCut w170(Form("%f/%d",xs170,n170));
  TCut w200(Form("%f/%d",xs200,n200));

  w30 = w30*(pthat30);
  w80 = w80*(pthat80);
  w120 = w120*(pthat120);
  w170 = w170*(pthat170);
  w200 = w200*(pthat200);

  for(int ie = 0; ie < nt->GetEntries(); ++ie){
    nt->GetEntry(ie);
    double pthatweight = 0;
    if(varpthat >= 30) pthatweight = xs30/n30;
    if(varpthat >= 80) pthatweight = xs80/n80;
    if(varpthat >= 120) pthatweight = xs120/n120;
    if(varpthat >= 170) pthatweight = xs170/n170;
    if(varpthat >= 200) pthatweight = xs200/n200;

    double cweight = exp(-  pow(varbin+1.11957e+01,2)  /    pow(1.34120e+01,2) / 2);
    ntw->Fill(pthatweight*cweight,pthatweight,cweight);
  }

  TCut weight(Form("%s + %s + %s + %s",(const char*)w80,(const char*)w120,(const char*)w170,(const char*)w200));

  if(reweightCentrality) weight = Form("(%s)*%s",(const char*)weight,
				       "exp(-  pow(bin+1.11957e+01,2)  /    pow(1.34120e+01,2) / 2)");
  
  if(validate){

    TCanvas* c1 = new TCanvas("c1","",600,600);
    TH1D* hMix = new TH1D("hMix",";p_{T}^{Leading RecoJet};",120,0,600);

  cout<<"Entries : "<<nt->GetEntries(weight)<<endl;
  nt->Draw("pt1>>hMix",weight);

  THStack* h = new THStack("h",";p_{T}^{Leading RecoJet}; Weighted Entries");
  TH1D* h80 = new TH1D("h80",";p_{T}^{Leading RecoJet};",380,120,600);
  TH1D* h120 = new TH1D("h120",";p_{T}^{Leading RecoJet};",380,120,600);
  TH1D* h170 = new TH1D("h170",";p_{T}^{Leading RecoJet};",380,120,600);
  TH1D* h200 = new TH1D("h200",";p_{T}^{Leading RecoJet};",380,120,600);

  c1->SetLogy();
  //  hMix->SetAxisRange(120,500);
  //  hMix->SetMinimum(0.00001);
  hMix->Draw();

  TCanvas* c2 = new TCanvas("c2","",600,600);
  c2->SetLogy();
  nt->Draw("pt1>>h80",w80);
  nt->Draw("pt1>>h120",w120);
  nt->Draw("pt1>>h170",w170);
  nt->Draw("pt1>>h200",w200);

  h80->Rebin(5);
  h120->Rebin(5);
  h170->Rebin(5);
  h200->Rebin(5);

  h80->SetFillColor(12);
  h120->SetFillColor(4);
  h170->SetFillColor(5);
  h200->SetFillColor(2);

  h80->SetFillStyle(1);
  h120->SetFillStyle(1);
  h170->SetFillStyle(1);
  h200->SetFillStyle(1);

  h->Add(h80);
  h->Add(h120);
  h->Add(h170);
  h->Add(h200);

  h->Draw("hist");

  weightString = TString((const char*)weight);
  new TCanvas();

  nt->Draw("bin",weightString.Data());

  new TCanvas();


  TF1* f = new TF1("f","exp(-  pow(x+1.11957e+01,2)  /    pow(1.34120e+01,2) / 2)",0,40);
  f->Draw();

  }

  outf->cd();
  ntw->Write();
  outf->Write();

  cout<<(const char*)weight<<endl;
  weightString = TString((const char*)weight);


  return weight;
}

