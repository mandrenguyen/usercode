void printBfraction(char *tagger="discr_ssvHighEff", Double_t workingPoint=2, char *taggerName="SSVHE", int doCent=1, int do3bin=0) {

  gROOT->ForceStyle(1);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);


  gStyle->SetTextFont(43);
  gStyle->SetLabelFont(43,"XYZ");
  gStyle->SetTitleFont(43,"XYZ");

  // hack
  if(doCent)gStyle->SetErrorX(0);
  gStyle->SetLabelSize(28,"xy");
  gStyle->SetTitleSize(28,"xy");
  gStyle->SetTitleOffset(1.1,"x");
  gStyle->SetTitleOffset(1.5,"y");
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetNdivisions(408,"y");


  TFile *fin1, *fin2;
  if(doCent){
    if(doCent==1){
      fin1 = new TFile("output/bFractionMerged_SSVHEat2.0FixCL1_centDep_80_100.root");
      fin2 = new TFile("output/bFractionMerged_SSVHEat2.0FixCL0_centDep_80_100.root");
    }
    if(doCent==2){
      fin1 = new TFile("output/bFractionMerged_SSVHEat2.0FixCL1_centDep_100_120.root");
      fin2 = new TFile("output/bFractionMerged_SSVHEat2.0FixCL0_centDep_100_120.root");
    }
    // broken
    //fin1 = new TFile("output/bFractionMerged_ssvHighEffat2.0FixCL1_centDep_80_100.root");
    //fin2 = new TFile("output/bFractionMerged_ssvHighEffat2.0FixCL0_centDep_80_100.root");
  }
  else{
    // try to smear by 2 sigma
    //fin1 = new TFile("output/bFractionMerged_Smear2Sigma_SSVHEat2.0FixCL1_bin_0_40_eta_0_2.root");
    //fin2 = new TFile("output/bFractionMerged_Smear2Sigma_SSVHEat2.0FixCL0_bin_0_40_eta_0_2.root");
    // with reg tracks in reco jets
    //fin1 = new TFile("output/bFractionMerged_regPFJets_SSVHEat2.0FixCL1_bin_0_40_eta_0_2.root");
    //fin2 = new TFile("output/bFractionMerged_regPFJets_SSVHEat2.0FixCL0_bin_0_40_eta_0_2.root");
    if(do3bin){
      fin1 = new TFile("output/bFractionMerged_3bins_SSVHEat2.0FixCL1_bin_0_40_eta_0_2.root");
      fin2 = new TFile("output/bFractionMerged_3bins_SSVHEat2.0FixCL0_bin_0_40_eta_0_2.root");
    }
    else{
      fin1 = new TFile("outputPostApproval/bFractionMerged_SSVHEat2.0FixCL1_bin_0_40_eta_0_2.root");
      fin2 = new TFile("outputPostApproval/bFractionMerged_SSVHEat2.0FixCL0_bin_0_40_eta_0_2.root");
      // old naming convention
    //fin1 = new TFile("output/bFractionMerged_ssvHighEffat2.0FixCL1_bin_0_40_eta_0_2.root");
    //fin2 = new TFile("output/bFractionMerged_ssvHighEffat2.0FixCL0_bin_0_40_eta_0_2.root");

    }
  }

  TH1F *hBFractionMC = (TH1F*) fin1->Get("hBFractionMC");

  TH1F *hBFractionData = (TH1F*) fin1->Get("hBFractionData");
  TH1F *hBFractionDataLTJP = (TH1F*) fin1->Get("hBFractionDataLTJP");
  TH1F *hBFractionJPdirect = (TH1F*) fin1->Get("hBFractionJPdirect");

  TH1F *hBFractionDataFixC = (TH1F*) fin2->Get("hBFractionData");
  TH1F *hBFractionDataLTJPFixC = (TH1F*) fin2->Get("hBFractionDataLTJP");
  TH1F *hBFractionJPdirectFixC = (TH1F*) fin2->Get("hBFractionJPdirect");

  TH1F *hBPurityMC = (TH1F*) fin1->Get("hBPurityMC");
  TH1F *hBPurityData = (TH1F*) fin1->Get("hBPurityData");
  TH1F *hBEfficiencyMC = (TH1F*) fin1->Get("hBEfficiencyMC");
  TH1F *hBEfficiencyData = (TH1F*) fin1->Get("hBEfficiencyDataLTJP");


  /*  --- correction due to Jet Energy Scale (calcul) ---
  correct(hBFractionMC);
  correct(hBFractionData);
  correct(hBFractionDataMoreC);
  correct(hBFractionDataLessC);
  correct(hBFractionDataLTJP);
  correct(hBFractionDataLTJPMoreC);
  correct(hBFractionDataLTJPLessC);
  correct(hBFractionJPdirect);
  correct(hBFractionJPdirectMoreC);
  correct(hBFractionJPdirectLessC);
  //*/

  //*  --- correction due to Jet Energy Scale (by hand) ---
  if(doCent){
    correctByCent(hBFractionMC,doCent);
    correctByCent(hBFractionData,doCent);
    correctByCent(hBFractionDataFixC,doCent);
    correctByCent(hBFractionDataLTJP,doCent);
    correctByCent(hBFractionDataLTJPFixC,doCent);
    correctByCent(hBFractionJPdirect,doCent);
    correctByCent(hBFractionJPdirectFixC,doCent);
  }
  else{
    ///*
    correct2(hBFractionMC,do3bin,0);
    correct2(hBFractionData,do3bin,1);
    correct2(hBFractionDataFixC,do3bin,1);
    correct2(hBFractionDataLTJP,do3bin,1);
    correct2(hBFractionDataLTJPFixC,do3bin,1);
    correct2(hBFractionJPdirect,do3bin,1);
    correct2(hBFractionJPdirectFixC,do3bin,1);
    //*/
  }
  //*/

  //  --- plots with variation of charm ---


  TCanvas *cBFraction1 = new TCanvas("cBFraction1","b-jet fraction",600,600);
  hBFractionMC->SetLineColor(2);
  hBFractionMC->SetLineWidth(2);
  hBFractionMC->SetMarkerColor(2);
  //hBFractionMC->SetMarkerStyle(4);
  if(doCent)hBFractionMC->SetAxisRange(0,400,"X");
  else hBFractionMC->SetAxisRange(80,200,"X");
  hBFractionMC->SetAxisRange(0,0.06,"Y");
  hBFractionMC->SetTitleOffset(1,"X");  
  hBFractionMC->GetYaxis()->SetTitle("b-jet fraction");;
  //hBFractionMC->Draw("e1"); 
  
  //hBFractionMC->GetYaxis()->SetNdivisions(505);
  
  TH1F *hBFractionMC_dummy= (TH1F*)hBFractionMC->Clone("hBFractionMC_dummy");
  hBFractionMC_dummy->SetLineWidth(0);
  hBFractionMC_dummy->SetLineColor(0);
  hBFractionMC_dummy->SetMarkerSize(0);
  hBFractionMC_dummy->Draw(); 
  hBFractionMC->Draw("hist,same"); 

  hBFractionData->SetMarkerStyle(8);
  hBFractionData->Draw("e1,same");  
  hBFractionDataLTJP->SetLineColor(kGreen-2); 
  hBFractionDataLTJP->SetMarkerColor(kGreen-2); 
  hBFractionDataLTJP->SetMarkerStyle(8); 
  hBFractionDataLTJP->Draw("e1,same");
  hBFractionJPdirect->SetMarkerStyle(8); 
  hBFractionJPdirect->SetMarkerColor(kBlue);
  hBFractionJPdirect->SetLineColor(kBlue);
  hBFractionJPdirect->Draw("e1,same");

  
  hBFractionDataFixC->SetLineStyle(2);
  //hBFractionDataFixC->Draw("e1same");   
  hBFractionDataLTJPFixC->SetLineColor(kGreen-2);
  hBFractionDataLTJPFixC->SetMarkerColor(kGreen-2);
  hBFractionDataLTJPFixC->SetMarkerStyle(4);
  hBFractionDataLTJPFixC->SetLineStyle(2);
  hBFractionDataLTJPFixC->Draw("e1same");
  hBFractionJPdirectFixC->SetLineStyle(2);
  //hBFractionJPdirectFixC->Draw("e1same");

  for(int i=0;i<hBFractionDataLTJP->GetNbinsX();i++){
    cout<<" LTJP "<<hBFractionDataLTJP->GetBinContent(i+1)<<endl;
    cout<<" Sim "<<hBFractionData->GetBinContent(i+1)<<endl;
    cout<<" JP direct "<<hBFractionJPdirect->GetBinContent(i+1)<<endl;
  }

  
  TLegend *legFrac1 = new TLegend(0.15,0.65,0.87,0.95);

  legFrac1->SetBorderSize(0);
  legFrac1->SetFillStyle(0);
  legFrac1->SetTextSize(23);
  legFrac1->SetHeader("PbPb, #sqrt{s_{NN}} = 2.76 TeV");
  legFrac1->AddEntry(hBFractionDataLTJP,Form("SSVHE, LT method",taggerName,workingPoint),"pl");
  legFrac1->AddEntry(hBFractionDataLTJPFixC,Form("SSVHE, LT method, Floating Charm Norm.",taggerName,workingPoint),"pl");
  legFrac1->AddEntry(hBFractionData,Form("SSVHE, MC eff.",taggerName,workingPoint),"pl");
  legFrac1->AddEntry(hBFractionJPdirect,"Jet Probability","pl");
  //legFrac1->AddEntry(hBFractionMC,"MC Input (reconstructed)","l");

  legFrac1->Draw();




  //  --- plots of LT method with syst. uncertainty ---

  TCanvas *cBFraction2 = new TCanvas("cBFraction2","b-jet fraction",600,600);

  TH1F *hBFractionMC2;
  if(doCent){
    hBFractionMC2= new TH1F("hBFractionMC2","hBFractionMC2",1,0,400);
    hBFractionMC2->SetXTitle("N_{part}");
    hBFractionMC2->SetYTitle("b-jet fraction");
  }
  else{
    hBFractionMC2= (TH1F*)hBFractionMC->Clone("hBFractionMC2");
  }
  if(!doCent)hBFractionMC2->GetXaxis()->SetRangeUser(80,200);
  hBFractionMC2->SetMarkerSize(0);
  hBFractionMC2->SetMaximum(0.06);
  hBFractionMC2->SetMinimum(0.0);
  hBFractionMC2->Draw("hist");


  TGraphAsymmErrors *gBFractionMC2 = new TGraphAsymmErrors(hBFractionMC);
  if(doCent){
    setNpart(gBFractionMC2,hBFractionMC,0);
    gBFractionMC2->GetXaxis()->SetRangeUser(0,400);
  }
  else{
    setMeanPt(gBFractionMC2,hBFractionMC,0,do3bin);
    gBFractionMC2->GetXaxis()->SetRangeUser(80,200);
  }



  TLatex *prel;
  if(doCent)prel= new TLatex(5,0.0615,"CMS preliminary");
  else prel= new TLatex(85,0.0615,"CMS preliminary");
  prel->SetTextSize(23);
  prel->Draw();

  TLatex *roots;
  if(doCent)roots = new TLatex(250,0.0615,"#sqrt{s_{NN}} = 2.76 TeV");
  else roots = new TLatex(147,0.0615,"#sqrt{s_{NN}} = 2.76 TeV");
  roots->SetTextSize(23);
  roots->Draw();

  if(!doCent){
    TLatex *csel = new TLatex(90,0.05,"Centrality 0-100%");
    csel->SetTextSize(28);
    csel->Draw();
  }
  TLatex *ptlabel;
  //if(doCent==1) ptlabel= new TLatex(200,0.052,"80 < Jet p_{T} < 100 GeV/c");
  //if(doCent==2) ptlabel= new TLatex(200,0.052,"100 < Jet p_{T} < 120 GeV/c");
  if(doCent==1) ptlabel= new TLatex(150,0.015,"80 < Jet p_{T} < 100 GeV/c");
  if(doCent==2) ptlabel= new TLatex(150,0.015,"100 < Jet p_{T} < 120 GeV/c");
  if(doCent){
    ptlabel->SetTextSize(23);
    ptlabel->Draw();
  }
  // to be precise we should evaluate mcStatErr for 3 bins seperately
  float mcStatErr[4] = {0.03,0.06,0.07,0.15};

  
  TGraphErrors *gSyst, *gSystCL, *gSystMeth, *gSystJES;

  if(do3bin){
    gSyst= new TGraphErrors(3);
    gSystCL= new TGraphErrors(3);
    gSystMeth= new TGraphErrors(3);
    gSystJES= new TGraphErrors(3);
  }
  else{
    gSyst= new TGraphErrors(4);
    gSystCL= new TGraphErrors(4);
    gSystMeth= new TGraphErrors(4);
    gSystJES= new TGraphErrors(4);
  }

  gSyst->SetName("gSyst");
  gSystCL->SetName("gSystCL");
  gSystMeth->SetName("gSystMeth");
  gSystJES->SetName("gSystJES");

  Double_t errCLratio, errMethod, totalSystErr;
  
  for(Int_t i=1;i<=hBFractionDataLTJP->GetNbinsX();i++) {

    float xVal = hBFractionDataLTJP->GetBinCenter(i);

    if(doCent){
      if(i==1) xVal=308.47;
      else if(i==2) xVal=134.55;
      else if(i==3) xVal=22.06;
  }
    
    float xErr = hBFractionDataLTJP->GetBinWidth(i)/2;

    if(doCent){
      xErr=12;
    }

    gSyst->SetPoint(i-1,xVal,hBFractionDataLTJP->GetBinContent(i));
    gSystCL->SetPoint(i-1,xVal,hBFractionDataLTJP->GetBinContent(i));
    gSystMeth->SetPoint(i-1,xVal,hBFractionDataLTJP->GetBinContent(i));
    gSystJES->SetPoint(i-1,xVal,hBFractionDataLTJP->GetBinContent(i));

    cout<<" central value "<<hBFractionDataLTJP->GetBinContent(i)<<endl;
    errCLratio = abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionDataLTJPFixC->GetBinContent(i));
    errMethod = max(abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionData->GetBinContent(i)),abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionJPdirect->GetBinContent(i)));
    double errJES = 0.14*hBFractionDataLTJP->GetBinContent(i);
    
    totalSystErr = norm(errCLratio,errMethod,errJES);

    gSyst->SetPointError(i-1,xErr,totalSystErr);
    gSystCL->SetPointError(i-1,xErr,errCLratio);
    gSystMeth->SetPointError(i-1,xErr,errMethod);
    gSystJES->SetPointError(i-1,xErr,errJES);
    cout<<" rel METH "<<errMethod/hBFractionDataLTJP->GetBinContent(i)<<endl;
    cout<<" rel CL "<<errCLratio/hBFractionDataLTJP->GetBinContent(i)<<endl;
    cout<<" rel JES "<<errJES/hBFractionDataLTJP->GetBinContent(i)<<endl;
    cout<<" rel sys error "<<totalSystErr/hBFractionDataLTJP->GetBinContent(i)<<endl;
    // add in MC template uncertainties
    float origStatErr = hBFractionDataLTJP->GetBinError(i);
    int statBin=i-1;
    //if(doCent==1) statBin=0;
    //if(doCent==2) statBin=1;
    if(doCent) statBin+=1;
    float extraStatErr = mcStatErr[statBin]*hBFractionDataLTJP->GetBinContent(i);
    float totalStatErr = sqrt(origStatErr*origStatErr + extraStatErr*extraStatErr);
    hBFractionDataLTJP->SetBinError(i,totalStatErr);
    cout<<" total error "<<sqrt(totalSystErr*totalSystErr+totalStatErr*totalStatErr)<<endl;

  }

  gSyst->SetFillColor(5);
  gSyst->Draw("2");


  gBFractionMC2->Draw("Z,p,same");
  hBFractionMC2->Draw("hist,same");


  TGraphAsymmErrors *gBFractionDataLTJP2 = new TGraphAsymmErrors(hBFractionDataLTJP);
  if(doCent)setNpart(gBFractionDataLTJP2,hBFractionDataLTJP,1);
  else setMeanPt(gBFractionDataLTJP2,hBFractionDataLTJP,1,do3bin);
  gBFractionDataLTJP2->SetLineColor(1);
  gBFractionDataLTJP2->SetMarkerColor(1);
  gBFractionDataLTJP2->SetMarkerSize(1.5);
  gBFractionDataLTJP2->Draw("p,e1,same");


  TLegend *legFrac2 = new TLegend(0.2,0.15,0.8,0.34);  
  if(doCent){
    /*
    legFrac2->SetX1(0.365);
    legFrac2->SetY1(0.657);
    legFrac2->SetX2(0.965);
    legFrac2->SetY2(0.848);
    */
    legFrac2->SetX1(0.225);
    legFrac2->SetY1(0.65);
    legFrac2->SetX2(0.85);
    legFrac2->SetY2(0.85);
  }

  legFrac2->SetHeader("#int L dt = 150 #mub^{-1}");
  legFrac2->SetTextFont(43);
  legFrac2->SetTextSize(23);
  legFrac2->SetBorderSize(0);
  legFrac2->SetFillStyle(0);
  legFrac2->AddEntry(gBFractionDataLTJP2,"PbPb data","p");
  legFrac2->AddEntry(gBFractionMC2,"PYTHIA+HYDJET 1.8","lp");
  legFrac2->AddEntry(gSyst,"Exp. uncertainty","f");
  legFrac2->Draw();

  if(doCent){
    TLatex *c50100 = new TLatex(10,0.006,"50-100%");
    TLatex *c2050 = new TLatex(110,0.006,"20-50%");
    TLatex *c020 = new TLatex(290,0.006,"0-20%");

    c50100->SetTextSize(23);
    c2050->SetTextSize(23);
    c020->SetTextSize(23);

    c50100->Draw();
    c2050->Draw();
    c020->Draw();
  }
 
  cBFraction2->RedrawAxis();


  if(do3bin&&!doCent){

    TFile *fout = new TFile("pbpb_bFraction.root","recreate");
    gBFractionDataLTJP2->Write();
    gSyst->Write();
    gSystCL->Write();
    gSystMeth->Write();
    gSystJES->Write();
    gBFractionMC2->Write();
    fout->Close();
  }



  

  TCanvas *cBPurity = new TCanvas("cBPurity","b purity",600,600);

  hBPurityMC->GetYaxis()->SetRange(0,1);
  

  TGraphAsymmErrors *gBPurityMC = new TGraphAsymmErrors(hBPurityMC);
  if(doCent)setNpart(gBPurityMC,hBPurityMC,0);
  else setMeanPt(gBPurityMC,hBPurityMC,0,do3bin);
  TGraphAsymmErrors *gBPurityData = new TGraphAsymmErrors(hBPurityData);
  if(doCent)setNpart(gBPurityData,hBPurityData,1);
  else setMeanPt(gBPurityData,hBPurityData,1,do3bin);

  hBPurityMC->SetMarkerSize(0);
  hBPurityMC->SetLineColor(0);

  
  gBPurityMC->GetYaxis()->SetRange(0,1);
  //gBPurityMC->GetYaxis()->SetTitleOffset(1.3);
  gBPurityMC->SetLineColor(2);
  gBPurityMC->SetMarkerColor(2);
  gBPurityMC->SetMarkerSize(1.2);
  gBPurityMC->SetMarkerStyle(20);
  gBPurityData->SetMarkerSize(1.5);
  gBPurityData->SetLineColor(1);
  gBPurityData->SetMarkerColor(1);
  gBPurityData->SetMarkerStyle(25);

  hBPurityMC->Draw("");
  gBPurityData->Draw("p,same");   
  gBPurityMC->Draw("pl,same");

  TLegend *legPur = new TLegend(0.25,0.6,0.8,0.82);
  legPur->SetHeader("PbPb, #int L dt = 150 #mub^{-1}");
  legPur->SetBorderSize(0);
  //legPur->SetFillColor(kGray);
  legPur->SetFillStyle(0);
  legPur->AddEntry(gBPurityMC,"PYTHIA+HYDJET 1.8","p");
  legPur->AddEntry(gBPurityData,"Template Fit to Data","p");
  //legPur->AddEntry(gBPurityMC,"SSVHE > 2","");
  legPur->Draw();

  TLatex *prel;
  if(doCent)prel= new TLatex(10,1.025,"CMS preliminary");
  else prel= new TLatex(85,1.025,"CMS preliminary");
  prel->SetTextSize(28);
  prel->Draw();

  TLatex *roots;
  if(doCent) roots= new TLatex(200,1.025,"#sqrt{s_{NN}} = 2.76 TeV");
  else roots= new TLatex(147,1.025,"#sqrt{s_{NN}} = 2.76 TeV");
  roots->SetTextSize(28);
  roots->Draw();

  if(!doCent){
    TLatex *csel = new TLatex(90,0.05,"Centrality 0-100%");
    csel->SetTextSize(28);
    csel->Draw();
  }
  TLatex *ptlabel;
  if(doCent==1) ptlabel= new TLatex(20,0.005,"80 < Jet p_{T} < 100 GeV/c");
  if(doCent==2) ptlabel= new TLatex(20,0.005,"100 < Jet p_{T} < 120 GeV/c");
  if(doCent)ptlabel->SetTextSize(23);
  if(doCent)ptlabel->Draw();




 TCanvas *cBEfficiency = new TCanvas("cBEfficiency","b purity",600,600);

  hBEfficiencyMC->GetYaxis()->SetRange(0,1);
  

  TGraphAsymmErrors *gBEfficiencyMC = new TGraphAsymmErrors(hBEfficiencyMC);
  if(!doCent)setMeanPt(gBEfficiencyMC,hBEfficiencyMC,0,do3bin);
  TGraphAsymmErrors *gBEfficiencyData = new TGraphAsymmErrors(hBEfficiencyData);
  if(!doCent)setMeanPt(gBEfficiencyData,hBEfficiencyData,1,do3bin);

  hBEfficiencyMC->SetMarkerSize(0);
  hBEfficiencyMC->SetLineColor(0);

  
  gBEfficiencyMC->GetYaxis()->SetRange(0,1);
  //gBEfficiencyMC->GetYaxis()->SetTitleOffset(1.3);
  gBEfficiencyMC->SetLineColor(2);
  gBEfficiencyMC->SetMarkerColor(2);
  gBEfficiencyMC->SetMarkerSize(1.2);
  gBEfficiencyMC->SetMarkerStyle(20);
  gBEfficiencyData->SetMarkerSize(1.5);
  gBEfficiencyData->SetLineColor(kGreen+1);
  gBEfficiencyData->SetMarkerColor(kGreen+1);
  gBEfficiencyData->SetMarkerStyle(25);

  hBEfficiencyMC->Draw("");
  gBEfficiencyData->Draw("p,same");   
  gBEfficiencyMC->Draw("p,same");



  TLegend *legEff = new TLegend(0.25,0.6,0.8,0.82);
  legEff->SetHeader("PbPb, #int L dt = 150 #mub^{-1}");
  legEff->SetBorderSize(0);
  //legEff->SetFillColor(kGray);
  legEff->SetFillStyle(0);
  legEff->AddEntry(gBEfficiencyMC,"Simulation","p");
  legEff->AddEntry(gBEfficiencyData,"Reference Tagger","p");
  //legEff->AddEntry(gBPurityMC,"SSVHE > 2","");
  legEff->Draw();


  prel->Draw();

  roots->Draw();

  if(!doCent){
    csel->Draw();
  }
  

}


Double_t abs (Double_t a) {return (a>0)?a:-a;}
Double_t max (Double_t a, Double_t b) {return (a>b)?a:b;}
Double_t norm (Double_t a, Double_t b) {return sqrt(a*a+b*b);}
Double_t norm (Double_t a, Double_t b, Double_t c) {return sqrt(a*a+b*b+c*c);}




Double_t NtrueOverNmeas (Double_t ptMin, Double_t ptMax) {

  //Double_t n = 6;
  //Double_t n = 5.78121e+00; // from ref level
  Double_t n = 5.95950e+00; // from reco level
  Double_t c1 = 5.23359e-01;
  Double_t c2 = 1.15226e-01;
  Double_t c3 = -7.96949e-03;

  TF1 *fTrue = new TF1("fMeas","([1]+[2]*log(x)+[3]*log(x)*log(x)+[2]+2*[3]*log(x))/exp([0]*log(x*([1]+[2]*log(x)+[3]*log(x)*log(x))))",0,500); 
  fTrue->SetParameters(n,c1,c2,c3);
  TF1 *fMeas = new TF1("fTrue","1/exp([0]*log(x))",0,500);
  fMeas->SetParameter(0,n);

  return fTrue->Integral(ptMin,ptMax)/fMeas->Integral(ptMin,ptMax);

}

void correct(TH1* h) {

  Double_t coef;

  for (Int_t i=1;i<=h->GetNbinsX();i++) {

    coef = NtrueOverNmeas(h->GetBinLowEdge(i),h->GetBinLowEdge(i)+h->GetBinWidth(i));

    cout<<"  correction for bin "<<i<<" is "<<coef<<endl;

    h->SetBinContent(i,coef*h->GetBinContent(i));
    h->SetBinError(i,coef*h->GetBinError(i));

  }

}

void correct2(TH1* h, int do3bin, int isData) {
  /*
// pp errors 
  h->SetBinContent(1,1.88055*h->GetBinContent(1));
  h->SetBinContent(2,1.84526*h->GetBinContent(2));
  h->SetBinContent(3,1.80878*h->GetBinContent(3));
  h->SetBinContent(4,1.75974*h->GetBinContent(4));
  h->SetBinContent(5,1.68418*h->GetBinContent(5));
  h->SetBinError(1,1.88055*h->GetBinError(1));
  h->SetBinError(2,1.84526*h->GetBinError(2));
  h->SetBinError(3,1.80878*h->GetBinError(3));
  h->SetBinError(4,1.75974*h->GetBinError(4));
  h->SetBinError(5,1.68418*h->GetBinError(5));
  */

  /*
  h->SetBinContent(1,1.83493*h->GetBinContent(1));
  h->SetBinContent(2,1.78004*h->GetBinContent(2));
  h->SetBinContent(3,1.73233*h->GetBinContent(3));
  h->SetBinContent(4,1.67081*h->GetBinContent(4));

  h->SetBinError(1,1.83493*h->GetBinError(1));
  h->SetBinError(2,1.78004*h->GetBinError(2));
  h->SetBinError(3,1.73233*h->GetBinError(3));
  h->SetBinError(4,1.67081*h->GetBinError(4));
  */
  
  // 80-100,100-120,120-200
  if(do3bin){
    float corrs[3]={2.01493,1.94923,1.86321};
    
    
    // multiply first bin by trigger efficiency correction
    corrs[0]*=9.18707422938016771e-01/9.57885863954237693e-01;
    for(int i=0;i<3;i++){
      h->SetBinContent(i+1,corrs[i]*h->GetBinContent(i+1));    
      h->SetBinError(i+1,corrs[i]*h->GetBinError(i+1));
    }
  }
  else{
    float corrs[4]={2.01493,1.94923,1.89018,1.81609};
    corrs[0]*=9.18707422938016771e-01/9.57885863954237693e-01;
    for(int i=0;i<4;i++){
      h->SetBinContent(i+1,corrs[i]*h->GetBinContent(i+1));    
      h->SetBinError(i+1,corrs[i]*h->GetBinError(i+1));
    }
  }


}


void correctByCent(TH1* h, int doCent) {
  
  
  float corrs[3];
  if(doCent==1) corrs[0]=2.03721; corrs[1]=1.9942; corrs[2]=1.94481;
  if(doCent==2) corrs[0]=1.97228; corrs[1]=1.92631; corrs[2]=1.88142;


  // fold in the trigger efficiency    
  corrs[0]*=0.908224/0.959339;
  corrs[1]*=0.935908/0.955979;
  corrs[2]*=0.93663/0.953573;
  

  for(int i=0;i<3;i++){
    h->SetBinContent(i+1,corrs[i]*h->GetBinContent(i+1));
    h->SetBinError(i+1,corrs[i]*h->GetBinError(i+1));
  }

}

void setMeanPt(TGraphAsymmErrors *g, TH1F *h, int isData, int do3bin){

  float meanPtData[4]={88.39,108.3,131.7,167.5};
  float meanPtMC[4]={88.24,108.1,131.5,167.};

  if(do3bin){
    meanPtData[2]=141.;
    meanPtMC[2]=140.1;
  }
  
  for(int i=0;i<h->GetNbinsX();i++){
    if(isData){
      g->SetPoint(i,meanPtData[i],h->GetBinContent(i+1));
      g->SetPointError(i,0,0,h->GetBinError(i+1), h->GetBinError(i+1));

    }
    else{
      g->SetPoint(i,meanPtMC[i],h->GetBinContent(i+1));
      g->SetPointError(i,0,0,h->GetBinError(i+1), h->GetBinError(i+1));
      //g->SetPointError(i,meanPtMC[i]-h->GetBinLowEdge(i+1),h->GetBinLowEdge(i+1)+h->GetBinWidth(i+1)-meanPtMC[i],h->GetBinError(i+1), h->GetBinError(i+1));
    }

  }
  
}



void setNpart(TGraphAsymmErrors *g, TH1F *h, int isData){

  
  float nPart[3]={308.47,134.55,22.06};

  for(int i=0;i<h->GetNbinsX();i++){
    if(isData){
      g->SetPoint(i,nPart[i],h->GetBinContent(i+1));
      g->SetPointError(i,0,0,h->GetBinError(i+1), h->GetBinError(i+1));
      
    }
    else{
      g->SetPoint(i,nPart[i],h->GetBinContent(i+1));
      g->SetPointError(i,0,0,h->GetBinError(i+1), h->GetBinError(i+1));
      //g->SetPointError(i,nPart[i]-h->GetBinLowEdge(i+1),h->GetBinLowEdge(i+1)+h->GetBinWidth(i+1)-nPart[i],h->GetBinError(i+1), h->GetBinError(i+1));
    }

  }
  
}
