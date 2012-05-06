void formatHisto(TH1F *h, char *title){
  h->Sumw2();
  h->SetMarkerStyle(8);
  h->SetXTitle(title);
  h->SetYTitle("Entries");
}

void setFillColor(TH1F *h[3])
{
  h[0]->SetFillColor(kred);
  h[1]->SetFillColor(kgreen);
  h[2]->SetFillColor(kblue);
}

void formatCanvas(TCanvas *c){

  c->Divide(1,2,0.01,0.01);
  c->cd(1);
  c->GetPad(1)->SetLogy();
  c->GetPad(1)->SetPad(0.,0.225,1.,1.);

  c->GetPad(2)->SetPad(0.,0.0,1.,0.3);
  c->GetPad(2)->SetBottomMargin(0.3);
  c->GetPad(2)->SetGridy(1);
}

void formatRatioHist(TH1F *h){

  h->GetYaxis()->SetRangeUser(0.4,1.6);
  h->GetYaxis()->SetNdivisions(505);
  h->SetYTitle("Data/MC");
  h->GetYaxis()->CenterTitle(1);
  h->GetYaxis()->SetLabelSize(14);
  h->GetYaxis()->SetLabelFont(43);

}

void stackHistos(TH1F *hArray[3]){
  hArray[1]->Add(hArray[0]);
  hArray[2]->Add(hArray[1]);

}


void compareDataMC(){

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  
  gStyle->SetLabelFont(43,"xyz");
  gStyle->SetLabelSize(14,"xyz");
  gStyle->SetTitleFont(43,"xyz");
  gStyle->SetTitleSize(16,"xyz");
  
  gStyle->SetTitleOffset(3.5,"x");
  
  gROOT->ForceStyle(1);
  TFile *fMC = new TFile("histos/ppMC.root");
  TFile *fdata = new TFile("histos/ppdata.root");

  // declare histos
  TH1F *hjtpt, *hnsvtx, *hsvtxntrk, *hsvtxdl, *hsvtxdls, *hsvtxm, *hsvtxpt, *hnIPtrk, *hnselIPtrk, *hdiscr_csvSimple, *hdiscr_prob;
  TH1F *hjtptMC[4], *hnsvtxMC[3], *hsvtxntrkMC[3], *hsvtxdlMC[3], *hsvtxdlsMC[3], *hsvtxmMC[3], *hsvtxptMC[3], *hnIPtrkMC[3], *hnselIPtrkMC[3], *hdiscr_csvSimpleMC[3], *hdiscr_probMC[3];

  // grab histos
  string suffix[4]={"B","C","L","U"};
  
  string name="hjtpt";
  hjtpt = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<4;i++) hjtptMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  string name="hnsvtx";
  hnsvtx = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<3;i++) hnsvtxMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  name="hsvtxntrk";
  hsvtxntrk = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<3;i++) hsvtxntrkMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  name="hsvtxdl";
  hsvtxdl = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<3;i++) hsvtxdlMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  name="hsvtxdls";
  hsvtxdls = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<3;i++) hsvtxdlsMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  name="hsvtxm";
  hsvtxm = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<3;i++) hsvtxmMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  name="hsvtxpt";
  hsvtxpt = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<3;i++) hsvtxptMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  name="hnIPtrk";
  hnIPtrk = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<3;i++) hnIPtrkMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  name="hnselIPtrk";
  hnselIPtrk = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<3;i++) hnselIPtrkMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  name="hdiscr_csvSimple";
  hdiscr_csvSimple = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<3;i++) hdiscr_csvSimpleMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  name="hdiscr_prob";
  hdiscr_prob = (TH1F*) fdata->Get(name.c_str());
  for(int i=0;i<3;i++) hdiscr_probMC[i] = (TH1F*) fMC->Get((name+suffix[i]).c_str());

  // turn off heavy flavor for testing
  /*
//  for(int i=0;i<2;i++) hjtptMC[i]->Scale(0.);
//  for(int i=0;i<2;i++) hnsvtxMC[i]->Scale(0.);
//  for(int i=0;i<2;i++) hsvtxntrkMC[i]->Scale(0.);
//  for(int i=0;i<2;i++) hsvtxdlMC[i]->Scale(0.);
//  for(int i=0;i<2;i++) hsvtxdlsMC[i]->Scale(0.);
//  for(int i=0;i<2;i++) hsvtxmMC[i]->Scale(0.);
//  for(int i=0;i<2;i++) hsvtxptMC[i]->Scale(0.);
//  for(int i=0;i<2;i++) hnIPtrkMC[i]->Scale(0.);
//  for(int i=0;i<2;i++) hnselIPtrkMC[i]->Scale(0.);
//  for(int i=0;i<2;i++) hdiscr_csvSimpleMC[i]->Scale(0.);
  */
  // stack em

  stackHistos(hjtptMC);
  stackHistos(hnsvtxMC);
  stackHistos(hsvtxntrkMC);
  stackHistos(hsvtxdlMC);
  stackHistos(hsvtxdlsMC);
  stackHistos(hsvtxmMC);
  stackHistos(hsvtxptMC);
  stackHistos(hnIPtrkMC);
  stackHistos(hnselIPtrkMC);
  stackHistos(hdiscr_csvSimpleMC);
  stackHistos(hdiscr_probMC);

  // scale em
  double scale = hjtpt->Integral()/hjtptMC[2]->Integral();
  
  for(int i=0;i<4;i++) hjtptMC[i]->Scale(scale);
  for(int i=0;i<3;i++) hnsvtxMC[i]->Scale(scale);
  for(int i=0;i<3;i++) hsvtxntrkMC[i]->Scale(scale);
  for(int i=0;i<3;i++) hsvtxdlMC[i]->Scale(scale);
  for(int i=0;i<3;i++) hsvtxdlsMC[i]->Scale(scale);
  for(int i=0;i<3;i++) hsvtxmMC[i]->Scale(scale);
  for(int i=0;i<3;i++) hsvtxptMC[i]->Scale(scale);
  for(int i=0;i<3;i++) hnIPtrkMC[i]->Scale(scale);
  for(int i=0;i<3;i++) hnselIPtrkMC[i]->Scale(scale);
  for(int i=0;i<3;i++) hdiscr_csvSimpleMC[i]->Scale(scale);
  for(int i=0;i<3;i++) hdiscr_probMC[i]->Scale(scale);

  // style em
  setFillColor(hjtptMC);
  setFillColor(hnsvtxMC);
  setFillColor(hsvtxntrkMC);
  setFillColor(hsvtxdlMC);
  setFillColor(hsvtxdlsMC);
  setFillColor(hsvtxmMC);
  setFillColor(hsvtxptMC);
  setFillColor(hnIPtrkMC);
  setFillColor(hnselIPtrkMC);
  setFillColor(hdiscr_csvSimpleMC);
  setFillColor(hdiscr_probMC);

  TLegend *leg = new TLegend(0.6,0.55,0.9,0.85);
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->AddEntry(hnsvtx,"pp @ 2.76 TeV","p");
  leg->AddEntry(hnsvtxMC[0],"b","f");
  leg->AddEntry(hnsvtxMC[1],"c","f");
  leg->AddEntry(hnsvtxMC[2],"udsg","f");


  // draw em

  char *title = "Jet p_{T} (GeV/c)";
  TCanvas *c0=new TCanvas("c0",title,200,10,600,480);
  formatHisto(hjtpt,title);
  formatCanvas(c0);
  hjtpt->Draw();   
  for(int i=3;i>-1;i--) hjtptMC[i]->Draw("same");
  hjtpt->Draw("same");
  leg->Draw();
  c0->GetPad(1)->RedrawAxis();

  c0->cd(2);
  TH1F *hRjtpt = hjtpt->Clone("hRjtpt");
  formatRatioHist(hRjtpt);
  hRjtpt->Divide(hjtptMC[2]);
  hRjtpt->Draw();

  char *title = "# of secondary vertices";
  TCanvas *c1=new TCanvas("c1",title,200,10,600,480);
  formatHisto(hnsvtx,title);
  formatCanvas(c1);
  hnsvtx->Draw();   
  for(int i=2;i>-1;i--) hnsvtxMC[i]->Draw("same");
  hnsvtx->Draw("same");
  leg->Draw();
  c1->GetPad(1)->RedrawAxis();

  c1->cd(2);
  TH1F *hRnsvtx = (TH1F*) hnsvtx->Clone("hRnsvtx");
  formatRatioHist(hRnsvtx);
  hRnsvtx->Divide(hnsvtxMC[2]);
  hRnsvtx->Draw();

  title = "# of tracks / SV";
  TCanvas *c2=new TCanvas("c2",title,200,10,600,480);
  formatHisto(hsvtxntrk,title);
  formatCanvas(c2);
  hsvtxntrk->Draw();
  for(int i=2;i>-1;i--) hsvtxntrkMC[i]->Draw("same");
  hsvtxntrk->Draw("same");
  c2->GetPad(1)->RedrawAxis();
  leg->Draw();

  c2->cd(2);
  TH1F *hRsvtxntrk = hsvtxntrk->Clone("hRsvtxntrk");
  formatRatioHist(hRsvtxntrk);
  hRsvtxntrk->Divide(hsvtxntrkMC[2]);
  hRsvtxntrk->Draw();

  title = "flight distance";
  TCanvas *c3=new TCanvas("c3",title,200,10,600,480);
  formatHisto(hsvtxdl,title);
  formatCanvas(c3);
  hsvtxdl->Draw();
  for(int i=2;i>-1;i--) hsvtxdlMC[i]->Draw("same");
  hsvtxdl->Draw("same");
  leg->Draw();
  c3->GetPad(1)->RedrawAxis();

  c3->cd(2);
  TH1F *hRsvtxdl = hsvtxdl->Clone("hRsvtxdl");
  formatRatioHist(hRsvtxdl);
  hRsvtxdl->Divide(hsvtxdlMC[2]);
  hRsvtxdl->Draw();

  title = "flight distance significance";
  TCanvas *c4=new TCanvas("c4",title,200,10,600,480);
  formatHisto(hsvtxdls,title);
  formatCanvas(c4);
  hsvtxdls->Draw();
  for(int i=2;i>-1;i--) hsvtxdlsMC[i]->Draw("same");
  hsvtxdls->Draw("same");
  leg->Draw();
  c4->GetPad(1)->RedrawAxis();

  c4->cd(2);
  TH1F *hRsvtxdls = hsvtxdls->Clone("hRsvtxdls");
  formatRatioHist(hRsvtxdls);
  hRsvtxdls->Divide(hsvtxdlsMC[2]);
  hRsvtxdls->Draw();
 
  title = "secondary vertex mass (GeV/c^{2})";
  TCanvas *c5=new TCanvas("c5",title,200,10,600,480);
  formatHisto(hsvtxm,title);
  formatCanvas(c5);
  hsvtxm->Draw();
  for(int i=2;i>-1;i--) hsvtxmMC[i]->Draw("same");
  hsvtxm->Draw("same");
  leg->Draw();
  c5->GetPad(1)->RedrawAxis();

  c5->cd(2);
  TH1F *hRsvtxm = hsvtxm->Clone("hRsvtxm");
  formatRatioHist(hRsvtxm);
  hRsvtxm->Divide(hsvtxmMC[2]);
  hRsvtxm->Draw();


  title = "secondary vertex p_{T} (GeV/c)";
  TCanvas *c6=new TCanvas("c6",title,200,10,600,480);
  formatHisto(hsvtxpt,title);
  formatCanvas(c6);
  hsvtxpt->Draw();
  for(int i=2;i>-1;i--) hsvtxptMC[i]->Draw("same");
  hsvtxpt->Draw("same");
  leg->Draw();
  c6->GetPad(1)->RedrawAxis();

  c6->cd(2);
  TH1F *hRsvtxpt = hsvtxpt->Clone("hRsvtxpt");
  formatRatioHist(hRsvtxpt);
  hRsvtxpt->Divide(hsvtxptMC[2]);
  hRsvtxpt->Draw();


  title = "# of IP tracks";
  TCanvas *c7=new TCanvas("c7",title,200,10,600,480);
  formatHisto(hnIPtrk,title);
  formatCanvas(c7);
  hnIPtrk->Draw();
  for(int i=2;i>-1;i--) hnIPtrkMC[i]->Draw("same");
  hnIPtrk->Draw("same");
  leg->Draw();
  c7->GetPad(1)->RedrawAxis();

  c7->cd(2);
  TH1F *hRnIPtrk = hnIPtrk->Clone("hRnIPtrk");
  formatRatioHist(hRnIPtrk);
  hRnIPtrk->Divide(hnIPtrkMC[2]);
  hRnIPtrk->Draw();

  title = "# of selectecd IP tracks";
  TCanvas *c8=new TCanvas("c8",title,200,10,600,480);
  formatHisto(hnselIPtrk,title);
  formatCanvas(c8);
  hnselIPtrk->Draw();
  for(int i=2;i>-1;i--) hnselIPtrkMC[i]->Draw("same");
  hnselIPtrk->Draw("same");
  leg->Draw();
  c8->GetPad(1)->RedrawAxis();

  c8->cd(2);
  TH1F *hRnselIPtrk = hnselIPtrk->Clone("hRnselIPtrk");
  formatRatioHist(hRnselIPtrk);
  hRnselIPtrk->Divide(hnselIPtrkMC[2]);
  hRnselIPtrk->Draw();

  title = "CSV Discriminant";
  TCanvas *c9=new TCanvas("c9",title,200,10,600,480);
  formatHisto(hdiscr_csvSimple,title);
  formatCanvas(c9);
  hdiscr_csvSimple->Draw();
  for(int i=2;i>-1;i--) hdiscr_csvSimpleMC[i]->Draw("same");
  hdiscr_csvSimple->Draw("same");
  leg->Draw();
  c9->GetPad(1)->RedrawAxis();

  c9->cd(2);
  TH1F *hRdiscr_csvSimple = hdiscr_csvSimple->Clone("hRdiscr_csvSimple");
  formatRatioHist(hRdiscr_csvSimple);
  hRdiscr_csvSimple->Divide(hdiscr_csvSimpleMC[2]);
  hRdiscr_csvSimple->Draw();

  title = "Jet Prob Discriminant";
  TCanvas *c10=new TCanvas("c10",title,200,10,600,480);
  formatHisto(hdiscr_prob,title);
  formatCanvas(c10);
  hdiscr_prob->Draw();
  for(int i=2;i>-1;i--) hdiscr_probMC[i]->Draw("same");
  hdiscr_prob->Draw("same");
  leg->Draw();
  c10->GetPad(1)->RedrawAxis();

  c10->cd(2);
  TH1F *hRdiscr_prob = hdiscr_prob->Clone("hRdiscr_prob");
  formatRatioHist(hRdiscr_prob);
  hRdiscr_prob->Divide(hdiscr_probMC[2]);
  hRdiscr_prob->Draw();

}
