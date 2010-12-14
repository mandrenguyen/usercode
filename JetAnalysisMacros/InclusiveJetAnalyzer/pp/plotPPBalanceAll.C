


void plotPPBalanceAll(){

  bool isPF = true;

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

  TFile *fDATA = new TFile(Form("./%s.root",data_tag.Data()));
  TFile *fMC = new TFile(Form("./%s.root",mc_tag.Data()));

  TH1F *hDijetBal_data = (TH1F*) fDATA->Get("hDataDijetBalance");
  TH1F *hDijetBal_mc = (TH1F*) fMC->Get("hQCDDijetBalance");


  // normalization should be matched with what's in ANA
  hDijetBal_data->Scale(1./hDijetBal_data->Integral());
  hDijetBal_data->Rebin(2);

  hDijetBal_mc->Scale(1./hDijetBal_mc->Integral());
  hDijetBal_mc->Rebin(2);

  cout<<"Bin Width = "<<hDijetBal_data->GetBinWidth(1)<<endl;


  // canvas setting ---
  TCanvas *c1 = new TCanvas("c1","",560,620);

  // dum styling ----
  TH1F *hDum = new TH1F("hDum","",10,0,1.0);
  hDum->SetLineColor(kBlue);
  hDum->SetFillColor(kAzure-8);
  hDum->SetFillStyle(3005);

  hDum->SetStats(0);
  hDum->SetXTitle("A_{J} = (p_{T}^{j1}-p_{T}^{j2})/(p_{T}^{j1}+p_{T}^{j2})");
  hDum->SetYTitle("Event Fraction");

  hDum->GetXaxis()->SetLabelSize(30);
  hDum->GetXaxis()->SetLabelFont(43);
  hDum->GetXaxis()->SetTitleSize(30);
  hDum->GetXaxis()->SetTitleFont(43);
  hDum->GetXaxis()->SetTitleOffset(1.4);
  hDum->GetXaxis()->CenterTitle();

  hDum->GetXaxis()->SetNdivisions(905,true);
  
  hDum->GetYaxis()->SetLabelSize(30);
  hDum->GetYaxis()->SetLabelFont(43);
  hDum->GetYaxis()->SetTitleSize(30);
  hDum->GetYaxis()->SetTitleFont(43);
  hDum->GetYaxis()->SetTitleOffset(1.8);
  hDum->GetYaxis()->CenterTitle();

  hDum->SetAxisRange(0,0.25,"Y");


  // data, mc styling
  hDijetBal_mc->SetLineColor(kBlue);
  hDijetBal_mc->SetFillColor(kAzure-8);
  hDijetBal_mc->SetFillStyle(3352);
// 3352
  hDum->Draw("hist");
  
  hDijetBal_mc->SetMarkerSize(2.0);
  hDijetBal_mc->Draw("histsame");
  hDijetBal_data->SetMarkerSize(2.0);
  hDijetBal_data->Draw("pzsame");


  // Legend
  TLegend *t3=new TLegend(0.53,0.57,0.91,0.82);
  //t3->SetHeader("ant-k_{T} (R=0.5) CaloJets");
  t3->AddEntry(hDijetBal_data,"p + p  #sqrt{s}=7.0 TeV","pl");
  t3->AddEntry(hDijetBal_mc,"PYTHIA","lf");
  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(63);
  t3->SetTextSize(20);
  t3->Draw();


  // other labeling
  TLatex *cms = new TLatex(0.20,0.23,"CMS");
  cms->SetTextFont(63);
  cms->SetTextSize(20);
  cms->Draw();

  TLatex *lumi = new TLatex(0.35,0.23,"#intL dt = 35.1 pb^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(18);
  lumi->Draw();

  
  TLatex *jetf;
  if(!isPF) jetf = new TLatex(0.46,0.21,"anti-k_{T} (R=0.5) CaloJets");
  else jetf = new TLatex(0.46,0.21,"anti-k_{T} (R=0.5) PFJets");
  jetf->SetTextFont(63);
  jetf->SetTextSize(20);
  jetf->Draw();

  c1->Print(Form("./fig/%s_%s_v1.gif",data_tag.Data(),jetfinder_tag.Data()));
  c1->Print(Form("./fig/%s_%s_v1.pdf",data_tag.Data(),jetfinder_tag.Data()));
  c1->Print(Form("./fig/%s_%s_v1.eps",data_tag.Data(),jetfinder_tag.Data()));
}
