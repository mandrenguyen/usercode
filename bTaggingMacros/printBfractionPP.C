//void printBfraction(char *tagger="discr_ssvHighPur", Double_t workingPoint=2, char *taggerName="SSVHP") {
void printBfractionPP(char *tagger="discr_ssvHighEff", Double_t workingPoint=2, char *taggerName="SSVHE") {


  gROOT->ForceStyle(1);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  gStyle->SetErrorX(0);
  gStyle->SetLabelSize(0.05,"xy");
  gStyle->SetTitleSize(0.05,"xy");
  gStyle->SetTitleOffset(1.5,"xy");
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.12);

  TFile *fin = new TFile(Form("/home/llr/cms/regnard/CMSSW_4_4_2_patch5/src/bFractionpp290512/histos/bFraction_%sat%.1f.root",taggerName,workingPoint));
  //TFile *fin = new TFile("output/bFractionMerged_ppPbPb0_ssvHighEffat2.0FixCL1_bin_0_40_eta_0_2.root");

  TH1F *hBFractionMC = (TH1F*) fin->Get("hBFractionMC");
  TH1F *hBFractionMCRefLevel = (TH1F*) fin->Get("hBFractionMCRefLevel");

  TH1F *hBFractionData = (TH1F*) fin->Get(Form("hBFractionData_%sat%.1f_CLshift%.0f",taggerName,workingPoint,0.));
  TH1F *hBFractionDataMoreC = (TH1F*) fin->Get(Form("hBFractionData_%sat%.1f_CLshift%.0f",taggerName,workingPoint,20.));
  TH1F *hBFractionDataLessC = (TH1F*) fin->Get(Form("hBFractionData_%sat%.1f_CLshift%.0f",taggerName,workingPoint,-20.));

  TH1F *hBFractionDataLTJP = (TH1F*) fin->Get(Form("hBFractionDataLTJP_%sat%.1f_CLshift%.0f",taggerName,workingPoint,0.));
  TH1F *hBFractionDataLTJPMoreC = (TH1F*) fin->Get(Form("hBFractionDataLTJP_%sat%.1f_CLshift%.0f",taggerName,workingPoint,20.));
  TH1F *hBFractionDataLTJPLessC = (TH1F*) fin->Get(Form("hBFractionDataLTJP_%sat%.1f_CLshift%.0f",taggerName,workingPoint,-20.));

  TH1F *hBFractionJPdirect = (TH1F*) fin->Get(Form("hBFractionJPdirect_%sat%.1f_CLshift%.0f",taggerName,workingPoint,0.));
  TH1F *hBFractionJPdirectMoreC = (TH1F*) fin->Get(Form("hBFractionJPdirect_%sat%.1f_CLshift%.0f",taggerName,workingPoint,20.));
  TH1F *hBFractionJPdirectLessC = (TH1F*) fin->Get(Form("hBFractionJPdirect_%sat%.1f_CLshift%.0f",taggerName,workingPoint,-20.));


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
  correct2(hBFractionDataMoreC);
  correct2(hBFractionDataLessC);
  correct2(hBFractionDataLTJP);
  correct2(hBFractionDataLTJPMoreC);
  correct2(hBFractionDataLTJPLessC);
  correct2(hBFractionJPdirect);
  correct2(hBFractionJPdirectMoreC);
  correct2(hBFractionJPdirectLessC);
  //*/

  //  --- plots with variation of charm ---

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  TCanvas *cBFraction1 = new TCanvas("cBFraction1","b-jet fraction",600,600);
  hBFractionMC->SetLineColor(2);
  hBFractionMC->SetMarkerColor(2);
  hBFractionMC->SetMarkerStyle(4);
  hBFractionMC->SetAxisRange(0,0.06,"Y");
  hBFractionMC->SetTitleOffset(1,"X");
  hBFractionMC->Draw("e1"); 

  hBFractionData->SetMarkerStyle(8);
  hBFractionDataLTJP->SetMarkerStyle(8);
  hBFractionData->Draw("e1same");   
  hBFractionDataLTJP->Draw("e1same");
  hBFractionJPdirect->Draw("e1same");

  hBFractionDataMoreC->SetLineStyle(2);
  hBFractionDataMoreC->Draw("e1same");   
  hBFractionDataLTJPMoreC->SetLineStyle(2);
  hBFractionDataLTJPMoreC->Draw("e1same");
  hBFractionJPdirectMoreC->SetLineStyle(2);
  hBFractionJPdirectMoreC->Draw("e1same");

  hBFractionDataLessC->SetLineStyle(3);
  hBFractionDataLessC->Draw("e1same");   
  hBFractionDataLTJPLessC->SetLineStyle(3);
  hBFractionDataLTJPLessC->Draw("e1same");
  hBFractionJPdirectLessC->SetLineStyle(3);
  hBFractionJPdirectLessC->Draw("e1same");
  
  TLegend *legFrac1 = new TLegend(0.35,0.13,0.87,0.3);
  legFrac1->SetBorderSize(1);
  legFrac1->SetFillColor(18);
  legFrac1->AddEntry(hBFractionMC,"Pythia","pl");
  legFrac1->AddEntry(hBFractionJPdirect,"direct fit of JP","pl");
  legFrac1->AddEntry(hBFractionData,Form("%s at %.1f + pur. from svtxm + eff. from MC",taggerName,workingPoint),"pl");
  legFrac1->AddEntry(hBFractionDataLTJP,Form("%s at %.1f + pur. from svtxm + eff. from LT (JP)",taggerName,workingPoint),"pl");
  legFrac1->Draw();




  //  --- plots of LT method with syst. uncertainty ---

  TCanvas *cBFraction2 = new TCanvas("cBFraction2","b-jet fraction",600,600);

  //*
  TH1F *hBFractionMC2 = hBFractionMC->Clone();
  hBFractionMC2->GetXaxis()->SetRangeUser(80,200);
  hBFractionMC2->Draw();
  //*/
  /*
  TH1F *hBFractionMCRefLevel2 = hBFractionMCRefLevel->Clone();
  hBFractionMCRefLevel2->SetAxisRange(0,0.05,"Y");
  hBFractionMCRefLevel2->SetTitleOffset(1,"Y");
  hBFractionMCRefLevel2->SetLineColor(2);
  hBFractionMCRefLevel2->SetMarkerColor(2);
  hBFractionMCRefLevel2->SetMarkerStyle(21);
  hBFractionMCRefLevel2->Draw();
  //*/

  TGraphErrors *gSyst = new TGraphErrors(5);
  Double_t errCLratio, errMethod, totalSystErr;

  for(Int_t i=1;i<=hBFractionDataLTJP->GetNbinsX();i++) {

    gSyst->SetPoint(i,hBFractionDataLTJP->GetBinCenter(i),hBFractionDataLTJP->GetBinContent(i));

    errCLratio = max(abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionDataLTJPMoreC->GetBinContent(i)),abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionDataLTJPLessC->GetBinContent(i)));
    errMethod = max(abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionData->GetBinContent(i)),abs(hBFractionDataLTJP->GetBinContent(i)-hBFractionJPdirect->GetBinContent(i)));
    totalSystErr = norm(errCLratio,errMethod);

    gSyst->SetPointError(i,hBFractionDataLTJP->GetBinWidth(i)/2,totalSystErr);

  }

  gSyst->SetFillColor(5);
  gSyst->Draw("2");

  TH1F *hBFractionDataLTJP2 = hBFractionDataLTJP->Clone();
  hBFractionDataLTJP2->SetAxisRange(0,0.06,"Y");
  hBFractionDataLTJP2->SetLineColor(1);
  hBFractionDataLTJP2->SetMarkerColor(1);
  hBFractionDataLTJP2->Draw("e1same");
  hBFractionMC2->Draw("e1same");
  //hBFractionMCRefLevel2->Draw("e1same");
  



  TLegend *legFrac2 = new TLegend(0.2,0.75,0.5,0.85);
  legFrac2->SetBorderSize(0);
  legFrac2->SetFillStyle(0);
  legFrac2->AddEntry(hBFractionMC2,"Pythia","p");
  //legFrac2->AddEntry(hBFractionMCRefLevel2,"Pythia","ple");
  legFrac2->AddEntry(hBFractionDataLTJP2,"pp, #sqrt{s} = 2.76 TeV","p");
  //legFrac2->AddEntry(gSyst,"syst. error","f");
  legFrac2->Draw();

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
  /* // Simon's original values
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

  //updated with n = 5.95

  h->SetBinContent(1,1.90158*h->GetBinContent(1));
  h->SetBinContent(2,1.83493*h->GetBinContent(2));
  h->SetBinContent(3,1.78004*h->GetBinContent(3));
  h->SetBinContent(4,1.73233*h->GetBinContent(4));
  h->SetBinContent(5,1.67081*h->GetBinContent(5));
  h->SetBinError(1,1.90158*h->GetBinError(1));
  h->SetBinError(2,1.83493*h->GetBinError(2));
  h->SetBinError(3,1.78004*h->GetBinError(3));
  h->SetBinError(4,1.73233*h->GetBinError(4));
  h->SetBinError(5,1.67081*h->GetBinError(5));



}
