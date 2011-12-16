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

static bool normLead = 0;

static int mixColor = 2;
static int dataColor = 1;

static int centralBin = 8;
static int leadCut = 120;
static int subleadCut = 30;

static double sideMin = 0.;
static double sideMax = TMath::Pi()/3;

static double sideCorrect = 1;

static const char* LUM = "112";

static bool plotSubtraction = 0;

static bool reweightCentrality = 1;

TCut weightMix(){

  TH1::SetDefaultSumw2();
  bool validate = 1;
  validate = 0;

  double xs80 = 9.913E-05 * 1000000;
  double xs120 = 1.128E-05 * 1000000;
  double xs170 = 1.470E-06 * 1000000;

  TNtuple* nt80 = (TNtuple*)(new TFile("mix80.root"))->Get("nt");
  TNtuple* nt120 = (TNtuple*)(new TFile("mix120.root"))->Get("nt");
  TNtuple* nt170 = (TNtuple*)(new TFile("mix170.root"))->Get("nt");

  int nev30 = 0, nev50 = 0, nev80 = 0, nev120 = 0, nev170 = 0;
  int n30 = 0, n50 = 0, n80 = 0, n120 = 0, n170 = 0;

  nev80 = nt80->GetEntries();
  nev120 = nt120->GetEntries();
  nev170 = nt170->GetEntries();

  TCut pthat80("pthat >= 80 && pthat < 120");
  TCut pthat120("pthat >= 120 && pthat < 170");
  TCut pthat170("pthat >= 170");

  n80 = nt80->GetEntries(pthat80);
  n120 = nt120->GetEntries(pthat120);
  n170 = nt170->GetEntries(pthat170);

  TCut cut80(Form("Entry$>=%d && Entry$ < %d",0,nev80));
  TCut cut120(Form("Entry$>=%d && Entry$ < %d",nev80,nev80+nev120));
  TCut cut170(Form("Entry$>=%d && Entry$ < %d",nev80+nev120,nev80+nev120+nev170));

  TCut w80(Form("%f/%d",xs80,n80));
  TCut w120(Form("%f/%d",xs120,n120));
  TCut w170(Form("%f/%d",xs170,n170));

  w80 = w80*(cut80 * pthat80);
  w120 = w120*(cut120 * pthat120);
  w170 = w170*(cut170 * pthat170);

  TCut weight(Form("%s + %s + %s",(const char*)w80,(const char*)w120,(const char*)w170));

  if(reweightCentrality) weight = Form("(%s + %s + %s)*%s",(const char*)w80,(const char*)w120,(const char*)w170,
				       "exp(-  pow(bin+1.11957e+01,2)  /    pow(1.34120e+01,2) / 2)");




  if(validate){


    TCanvas* c1 = new TCanvas("c1","",600,600);
    TNtuple* nt = (TNtuple*)(new TFile("mix.root"))->Get("nt");
    TH1D* hMix = new TH1D("hMix",";p_{T}^{Leading RecoJet};",100,0,500);

  cout<<"Entries : "<<nt->GetEntries(weight)<<endl;
  nt->Draw("pt1>>hMix",weight);

  THStack* h = new THStack("h",";p_{T}^{Leading RecoJet}; Weighted Entries");
  TH1D* h80 = new TH1D("h80",";p_{T}^{Leading RecoJet};",380,120,500);
  TH1D* h120 = new TH1D("h120",";p_{T}^{Leading RecoJet};",380,120,500);
  TH1D* h170 = new TH1D("h170",";p_{T}^{Leading RecoJet};",380,120,500);

  c1->SetLogy();
  //  hMix->SetAxisRange(120,500);
  //  hMix->SetMinimum(0.00001);
  hMix->Draw();

  TCanvas* c2 = new TCanvas("c2","",600,600);
  c2->SetLogy();
  nt->Draw("pt1>>h80",w80);
  nt->Draw("pt1>>h120",w120);
  nt->Draw("pt1>>h170",w170);

  h80->Rebin(5);
  h120->Rebin(5);
  h170->Rebin(5);


  h80->SetFillColor(12);
  h120->SetFillColor(4);
  h170->SetFillColor(2);

  h->Add(h80);
  h->Add(h120);
  h->Add(h170);

  h->Draw("hist");

  weightString = TString((const char*)weight);
  new TCanvas();

  nt->Draw("bin",weightString.Data());

  new TCanvas();


  TF1* f = new TF1("f","exp(-  pow(x+1.11957e+01,2)  /    pow(1.34120e+01,2) / 2)",0,40);
  f->Draw();

  }

  cout<<(const char*)weight<<endl;
  weightString = TString((const char*)weight);


  return weight;
}

