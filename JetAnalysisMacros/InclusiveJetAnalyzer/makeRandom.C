
#include <iostream>
#include "TCanvas.h"
#include "TRandom.h"
#include "TFile.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TNtuple.h"



void makeRandom(int N = 1000000){

  TFile* outf = new TFile("random.root","recreate");
  TNtuple* nt = new TNtuple("ntRandom","","r1:r2");

  TRandom* gen = new TRandom();

  for(int i = 0; i < N; ++i){
    double r1 = gen->Gaus();
    double r2 = gen->Gaus();

    nt->Fill(r1,r2);
  }

  nt->Draw("r1");
  nt->Draw("r2","","same");

}

