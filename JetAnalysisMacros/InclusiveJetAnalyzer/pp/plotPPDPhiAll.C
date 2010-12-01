


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

  TFile *fDATA = new TFile(Form("./%s.root",data_tag.Data()));
  TFile *fMC = new TFile(Form("./%s.root",mc_tag.Data()));

  TH1F *hDijetBal_data = (TH1F*) fDATA->Get("hDataDijetDPhi");
  TH1F *hDijetBal_mc = (TH1F*) fMC->Get("hQCDDijetDPhi");


  // normalization should be matched with what's in ANA
  hDijetBal_data->Scale(1./hDijetBal_data->Integral());
  //hDijetBal_data->Rebin(2);

  hDijetBal_mc->Scale(1./hDijetBal_mc->Integral());
  //hDijetBal_mc->Rebin(2);

  cout<<"Bin Width = "<<hDijetBal_data->GetBinWidth(1)<<endl;


  // canvas setting ---
  TCanvas *c1 = new TCanvas("c1","",490,530);
  c1->SetLogy();

  // dum styling ----
  TH1F *hDum = new TH1F("hDum","",10,0,3.142);
  hDum->SetLineColor(kBlue);
  hDum->SetFillColor(kAzure-8);
  hDum->SetFillStyle(3005);

  hDum->SetStats(0);
  hDum->SetXTitle("#Delta #Phi");
  hDum->SetYTitle("Event Fraction");

  hDum->GetXaxis()->SetLabelSize(20);
  hDum->GetXaxis()->SetLabelFont(43);
  hDum->GetXaxis()->SetTitleSize(24);
  hDum->GetXaxis()->SetTitleFont(43);
  hDum->GetXaxis()->SetTitleOffset(1.2);
  hDum->GetXaxis()->CenterTitle();

  hDum->GetXaxis()->SetNdivisions(905,true);
  
  hDum->GetYaxis()->SetLabelSize(20);
  hDum->GetYaxis()->SetLabelFont(43);
  hDum->GetYaxis()->SetTitleSize(22);
  hDum->GetYaxis()->SetTitleFont(43);
  hDum->GetYaxis()->SetTitleOffset(1.8);
  hDum->GetYaxis()->CenterTitle();

  hDum->SetAxisRange(3E-3,0.5,"Y");


  // data, mc styling
  hDijetBal_mc->SetLineColor(kBlue);
  hDijetBal_mc->SetFillColor(kAzure-8);
  hDijetBal_mc->SetFillStyle(3005);

  hDum->Draw("hist");

  hDijetBal_mc->Draw("histsame");
  hDijetBal_data->Draw("pzsame");


  // Legend
  TLegend *t3=new TLegend(0.21,0.57,0.8,0.82);
  //t3->SetHeader("ant-k_{T} (R=0.5) CaloJets");
  t3->AddEntry(hDijetBal_data,"p + p  #sqrt{s}=7.0 TeV","pl");
  t3->AddEntry(hDijetBal_mc,"PYTHIA","lf");
  t3->SetFillColor(0);
  t3->SetBorderSize(0);
  t3->SetFillStyle(0);
  t3->SetTextFont(63);
  t3->SetTextSize(15);
  t3->Draw();


  // other labeling
  TLatex *cms = new TLatex(0.24,0.33,"CMS Preliminary");
  cms->SetTextFont(63);
  cms->SetTextSize(17);
  cms->Draw();

  TLatex *lumi = new TLatex(1.3,0.33,"#intL dt = 35.1 pb^{-1}");
  lumi->SetTextFont(63);
  lumi->SetTextSize(15);
  lumi->Draw();

  
  TLatex *jetf;
  if(!isPF) jetf = new TLatex(0.24,0.21,"anti-k_{T} (R=0.5) CaloJets");
  else jetf = new TLatex(0.24,0.21,"anti-k_{T} (R=0.5) PFJets");
  jetf->SetTextFont(63);
  jetf->SetTextSize(15);
  jetf->Draw();

  c1->Print(Form("./fig/dphi_%s_%s_v1.gif",data_tag.Data(),jetfinder_tag.Data()));
  c1->Print(Form("./fig/dphi_%s_%s_v1.pdf",data_tag.Data(),jetfinder_tag.Data()));
  c1->Print(Form("./fig/dphi_%s_%s_v1.eps",data_tag.Data(),jetfinder_tag.Data()));
}
