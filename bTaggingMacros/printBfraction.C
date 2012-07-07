void printBfraction(char *tagger="discr_ssvHighEff", Double_t workingPoint=2, char *taggerName="ssvHighEff") {

  gROOT->ForceStyle(1);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);


  gStyle->SetTextFont(42);
  gStyle->SetLabelFont(42,"XYZ");
  gStyle->SetTitleFont(42,"XYZ");

  gStyle->SetErrorX(0);
  gStyle->SetLabelSize(0.05,"xy");
  gStyle->SetTitleSize(0.05,"xy");
  gStyle->SetTitleOffset(1.5,"xy");
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.12);


  TFile *fin1 = new TFile("output/bFractionMerged_ssvHighEffat2.0FixCL1_bin_0_40_eta_0_2.root");
  TFile *fin2 = new TFile("output/bFractionMerged_ssvHighEffat2.0FixCL0_bin_0_40_eta_0_2.root");
  
  TH1F *hBFractionMC = (TH1F*) fin1->Get("hBFractionMC");

  TH1F *hBFractionData = (TH1F*) fin1->Get("hBFractionData");
  TH1F *hBFractionDataLTJP = (TH1F*) fin1->Get("hBFractionDataLTJP");
  TH1F *hBFractionJPdirect = (TH1F*) fin1->Get("hBFractionJPdirect");

  TH1F *hBFractionDataFixC = (TH1F*) fin2->Get("hBFractionData");
  TH1F *hBFractionDataLTJPFixC = (TH1F*) fin2->Get("hBFractionDataLTJP");
  TH1F *hBFractionJPdirectFixC = (TH1F*) fin2->Get("hBFractionJPdirect");


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
  correct2(hBFractionMC);
  correct2(hBFractionData);
  correct2(hBFractionDataFixC);
  correct2(hBFractionDataLTJP);
  correct2(hBFractionDataLTJPFixC);
  correct2(hBFractionJPdirect);
  correct2(hBFractionJPdirectFixC);
  //*/

  //  --- plots with variation of charm ---


  TCanvas *cBFraction1 = new TCanvas("cBFraction1","b-jet fraction",600,600);
  hBFractionMC->SetLineColor(2);
  hBFractionMC->SetLineWidth(2);
  hBFractionMC->SetMarkerColor(2);
  //hBFractionMC->SetMarkerStyle(4);
  hBFractionMC->SetAxisRange(80,200,"X");
  hBFractionMC->SetAxisRange(0,0.05,"Y");
  hBFractionMC->SetTitleOffset(1,"X");  
  hBFractionMC->GetYaxis()->SetTitle("b-jet fraction");;
  //hBFractionMC->Draw("e1"); 
  
  hBFractionMC->GetYaxis()->SetNdivisions(505);
  hBFractionMC->Draw("hist"); 

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

  
  TLegend *legFrac1 = new TLegend(0.15,0.13,0.87,0.3);
  legFrac1->SetBorderSize(0);
  legFrac1->SetFillStyle(0);
  legFrac1->AddEntry(hBFractionDataLTJP,Form("SSVHE, LT method",taggerName,workingPoint),"p");
  legFrac1->AddEntry(hBFractionDataLTJPFixC,Form("SSVHE, LT method, Floating Charm Norm.",taggerName,workingPoint),"p");
  legFrac1->AddEntry(hBFractionData,Form("SSVHE, MC eff.",taggerName,workingPoint),"p");
  legFrac1->AddEntry(hBFractionJPdirect,"Jet Probability","p");
  legFrac1->AddEntry(hBFractionMC,"MC Input","l");

  legFrac1->Draw();




  //  --- plots of LT method with syst. uncertainty ---

  TCanvas *cBFraction2 = new TCanvas("cBFraction2","b-jet fraction",600,600);

  TH1F *hBFractionMC2 = hBFractionMC->Clone();
  hBFractionMC2->SetMaximum(0.06);
  hBFractionMC2->GetXaxis()->SetRangeUser(80,200);
  hBFractionMC2->Draw();

  TLatex *prel = new TLatex(83,0.0615,"CMS preliminary");
  prel->Draw();

  TLatex *roots = new TLatex(147,0.0615,"#sqrt{s_{NN}} = 2.76 TeV");
  roots->Draw();

  //TLatex *csel = new TLatex(90,0.0025,"Centrality 0-100%");
  TLatex *csel = new TLatex(90,0.05,"Centrality 0-100%");
  csel->Draw();

  //TLatex *ptlabel = new TLatex(90,0.0075,"Jet p_{T} > 80 GeV/c");
  TLatex *ptlabel = new TLatex(90,0.055,"Jet p_{T} > 80 GeV/c");
  ptlabel->Draw();


  TGraphErrors *gSyst = new TGraphErrors(5);
  Double_t errCLratio, errMethod, totalSystErr;

  for(Int_t i=1;i<=hBFractionDataLTJP->GetNbinsX();i++) {

    gSyst->SetPoint(i,hBFractionDataLTJP->GetBinCenter(i),hBFractionDataLTJP->GetBinContent(i));

    errCLratio = abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionDataLTJPFixC->GetBinContent(i));
    //errMethod = abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionData->GetBinContent(i));
    errMethod = max(abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionData->GetBinContent(i)),abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionJPdirect->GetBinContent(i)));
    totalSystErr = norm(errCLratio,errMethod);

    gSyst->SetPointError(i,hBFractionDataLTJP->GetBinWidth(i)/2,totalSystErr);

  }

  gSyst->SetFillColor(5);
  gSyst->Draw("2");

  TH1F *hBFractionDataLTJP2 = hBFractionDataLTJP->Clone();
  hBFractionDataLTJP2->SetAxisRange(0,0.05,"Y");
  hBFractionDataLTJP2->SetLineColor(1);
  hBFractionDataLTJP2->SetMarkerColor(1);
  hBFractionDataLTJP2->SetMarkerSize(1.5);
  hBFractionDataLTJP2->Draw("e1same");

  //hBFractionMC2->Draw("e1same");
  hBFractionMC2->Draw("hist,same");
  

  //TLegend *legFrac2 = new TLegend(0.2,0.67,0.8,0.85);
  TLegend *legFrac2 = new TLegend(0.2,0.15,0.8,0.34);
  legFrac2->SetHeader("#int L dt = 150 #mub^{-1}");
  legFrac2->SetBorderSize(0);
  legFrac2->SetFillStyle(0);
  legFrac2->AddEntry(hBFractionDataLTJP2,"PbPb data","p");
  legFrac2->AddEntry(hBFractionMC2,"PYTHIA+HYDJET 1.8","l");
  legFrac2->AddEntry(gSyst,"Exp. uncertainty","f");
  legFrac2->Draw();

  /*
  TLegend *legFrac2 = new TLegend(0.2,0.75,0.5,0.85);
  legFrac2->SetBorderSize(0);
  legFrac2->SetFillStyle(0);
  legFrac2->AddEntry(hBFractionMC2,"Pythia+Hydjet","l");
  legFrac2->AddEntry(hBFractionDataLTJP2,"PbPb, 0-100%","pl");
  //legFrac2->AddEntry(hBFractionDataLTJP2,"PbPb, 50-100%","pl");
  //legFrac2->AddEntry(gSyst,"syst. error","f");
  legFrac2->Draw();
  */
  cBFraction2->RedrawAxis();

}


Double_t abs (Double_t a) {return (a>0)?a:-a;}
Double_t max (Double_t a, Double_t b) {return (a>b)?a:b;}
Double_t norm (Double_t a, Double_t b) {return sqrt(a*a+b*b);}




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

void correct2(TH1* h) {
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

  h->SetBinContent(1,1.83493*h->GetBinContent(1));
  h->SetBinContent(2,1.78004*h->GetBinContent(2));
  h->SetBinContent(3,1.73233*h->GetBinContent(3));
  h->SetBinContent(4,1.67081*h->GetBinContent(4));

  h->SetBinError(1,1.83493*h->GetBinError(1));
  h->SetBinError(2,1.78004*h->GetBinError(2));
  h->SetBinError(3,1.73233*h->GetBinError(3));
  h->SetBinError(4,1.67081*h->GetBinError(4));




}
