

void easyLeg( TLegend *a=0 , char * head="")
{
  a->SetBorderSize(0);
  a->SetHeader(head);
  a->SetTextFont(62);
  a->SetLineColor(1);
  a->SetLineStyle(1);
  a->SetLineWidth(1);
  a->SetFillColor(0);
  a->SetFillStyle(0);

}



void plotTriggeredCentrality(){

  TH1::SetDefaultSumw2(); 

  TChain* t = new TChain("hltanalysis/HltTree");
  t->Add("/d101/yetkin/AllPhysics/runsWithTowerCut/r151153/OpenHLT_*.root");
  t->Add("/d101/yetkin/AllPhysics/runsWithTowerCut/r151350/OpenHLT_*.root");
  /*
  TChain* t = new TChain("anaPixelHitMB/PixelTree");
  TChain* t2 = new TChain("anaPixelHitJet50U/PixelTree");
  t->Add("/d101/kimy/macro/pixelTrees/rootFiles/r151878.root");
  t2->Add("/d101/kimy/macro/pixelTrees/rootFiles/r151878.root");
  */

  TH1D* hMB = new TH1D("hMB",";Centrality Bin;Fraction of minimum bias events",40,0,40);
  TH1D* hJ = new TH1D("hJ","",40,0,40);

  TCanvas* c1 = new TCanvas("c1","",400,400);
  c1->SetLogy();
  t->Draw("39-hiBin>>hMB","HLT_HIMinBiasHfOrBSC_Core && !(L1Tech_BSC_halo_beam2_inner.v0 || L1Tech_BSC_halo_beam2_outer.v0 || L1Tech_BSC_halo_beam1_inner.v0 || L1Tech_BSC_halo_beam1_outer.v0)");
  t->Draw("39-hiBin>>hJ","HLT_HIJet50U && !(L1Tech_BSC_halo_beam2_inner.v0 || L1Tech_BSC_halo_beam2_outer.v0 || L1Tech_BSC_halo_beam1_inner.v0 || L1Tech_BSC_halo_beam1_outer.v0)","same");

  double sc = hMB->Integral();

  float triggerRatio = 0.29 / 49.;

  hMB->Scale(1./sc);
  hJ->Scale(1./hJ->Integral());
  hJ->Scale(triggerRatio);

  hJ->SetLineColor(kRed);
  hJ->SetFillColor(kRed-9);
  hJ->SetFillStyle(3544);

  hMB->SetMaximum(1.);
  hMB->SetMinimum(0.00001);

  hMB->GetXaxis()->SetBinLabel(1,"100%");
  hMB->GetXaxis()->SetBinLabel(11,"75%");
  hMB->GetXaxis()->SetBinLabel(21,"50%");
  hMB->GetXaxis()->SetBinLabel(31,"25%");
  hMB->GetXaxis()->SetBinLabel(40,"0%");

  hMB->GetXaxis()->SetNdivisions(40);
  hMB->GetXaxis()->CenterTitle();
  hMB->GetYaxis()->CenterTitle();

  hMB->Draw("hist");
  hJ->Draw("hist same");

  TLegend* leg0 = new TLegend(0.27,0.74,0.88,0.86,NULL,"brNDC");
  easyLeg(leg0,"Centrality");
  leg0->AddEntry(hMB,"HLT_MinBiasHForBSC_Core","l");
  leg0->AddEntry(hJ,"HLT_HiJet50U","l");
  leg0->Draw();

  TLatex *cms = new TLatex(0.6013,0.85,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(16);
  cms->SetNDC();
  cms->Draw();

  c1->Print("CentralityBinning_d20101129.gif");
  c1->Print("CentralityBinning_d20101129.C");
  c1->Print("CentralityBinning_d20101129.eps");
  c1->Print("CentralityBinning_d20101129.root");



}


