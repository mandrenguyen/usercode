void plotDoubleRatio(int doCorrelated=0, int drawMC=0){


 gROOT->ForceStyle(1);

  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);

  //gStyle->SetErrorX(0);
  gStyle->SetLabelSize(0.05,"xy");
  gStyle->SetTitleSize(0.05,"xy");
  gStyle->SetTitleOffset(1.25,"y");
  gStyle->SetTitleOffset(1.1,"x");
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetNdivisions(408,"y");
  gStyle->SetTextFont(43); 
  //gStyle->SetEndErrorSize(0);
  gROOT->ForceStyle(1);

  TFile *fpbpb = new TFile("pbpb_bFraction.root");
  TFile *fpp = new TFile("pp_bFraction.root");

  TGraphAsymmErrors *gPbPbData, *gPbPbMC;
  TGraphErrors *gPbPbSystCL, *gPbPbSystMeth;
  gPbPbData = (TGraphAsymmErrors*)fpbpb->Get("Graph_from_hBFractionDataLTJP");
  gPbPbMC = (TGraphAsymmErrors*)fpbpb->Get("Graph_from_hBFractionMC");
  gPbPbSystCL = (TGraphErrors*)fpbpb->Get("gSystCL");
  gPbPbSystMeth = (TGraphErrors*)fpbpb->Get("gSystMeth");

  TGraphAsymmErrors *gPPData, *gPPMC;
  TGraphErrors *gPPSystCL, *gPPSystMeth;
  gPPData = (TGraphAsymmErrors*)fpp->Get("Graph_from_hBFractionDataLTJP_SSVHEat2.0_CLshift0");
  gPPMC = (TGraphAsymmErrors*)fpp->Get("Graph_from_hBFractionMC");
  gPPSystCL = (TGraphErrors*)fpp->Get("gSystCL");
  gPPSystMeth = (TGraphErrors*)fpp->Get("gSystMeth");
  
  TH1F *hframe = new TH1F("hframe","hframe",1,80,200);
  hframe->SetMinimum(0.);
  hframe->SetMaximum(2.05);

  hframe->SetXTitle("Jet p_{T} (GeV/c)");
  hframe->SetYTitle("PbPb b-Jet fraction / pp b-Jet fraction");
  
  TGraphAsymmErrors *gDR = new TGraphAsymmErrors(3);
  TGraphAsymmErrors *gDRerr = new TGraphAsymmErrors(3);
  TGraphAsymmErrors *gDRMC = new TGraphAsymmErrors(3);
  
  double meanPtPbPb[3], meanPtPP[3];
  double bFracPbPb[3], bFracPP[3];
  double statErrPbPb[3], statErrPP[3];
  double bFracPbPbUp[3], bFracPPUp[3];
  double bFracPbPbDown[3], bFracPPDown[3];


  double meanPtPbPbMC[3], meanPtPPMC[3];
  double bFracPbPbMC[3], bFracPPMC[3];
  double statErrPbPbMC[3], statErrPPMC[3];
 
  float binBounds[4] = {80,100,120,200};


  for(int i=0;i<3;i++){
    gPbPbData->GetPoint(i,meanPtPbPb[i],bFracPbPb[i]);
    gPPData->GetPoint(i,meanPtPP[i],bFracPP[i]);

    gPbPbMC->GetPoint(i,meanPtPbPbMC[i],bFracPbPbMC[i]);
    gPPMC->GetPoint(i,meanPtPPMC[i],bFracPPMC[i]);
    
    statErrPbPb[i]=gPbPbData->GetErrorYhigh(i);
    statErrPP[i]=gPPData->GetErrorYhigh(i);

    statErrPbPbMC[i]=gPbPbMC->GetErrorYhigh(i);
    statErrPPMC[i]=gPPMC->GetErrorYhigh(i);
   
    double dR = bFracPbPb[i]/bFracPP[i];
    double dRstatErr = dR * sqrt(pow(statErrPbPb[i]/bFracPbPb[i],2) + pow(statErrPP[i]/bFracPP[i],2) );

    double dRMC = bFracPbPbMC[i]/bFracPPMC[i];
    double dRstatErrMC = dR * sqrt(pow(statErrPbPbMC[i]/bFracPbPbMC[i],2) + pow(statErrPPMC[i]/bFracPPMC[i],2) );
 
    double errxlo=meanPtPbPb[i]-binBounds[i];
    double errxhi=binBounds[i+1]-meanPtPbPb[i];

    //cout<<" errxlo "<<errxlo<<" errxhi "<<errxhi<<endl;

    gDR->SetPoint(i,meanPtPbPb[i],dR);
    gDR->SetPointError(i,0.,0.,dRstatErr,dRstatErr);
    gDRerr->SetPoint(i,meanPtPbPb[i],dR);
    gDRMC->SetPoint(i,meanPtPbPbMC[i],dRMC);
    gDRMC->SetPointError(i,0,0,dRstatErrMC,dRstatErrMC);

    double cLSystPbPb = gPbPbSystCL->GetErrorY(i);
    double cLSystPP = gPPSystCL->GetErrorY(i);

    double cLSysErr = dR*sqrt(pow(cLSystPbPb[i]/bFracPbPb[i],2) + pow(cLSystPP[i]/bFracPP[i],2) );

    double dummy;
    
    bFracPbPbUp[i] = bFracPbPb[i] + gPbPbSystMeth->GetErrorY(i);
    bFracPPUp[i] = bFracPP[i] + gPPSystMeth->GetErrorY(i);
    bFracPbPbDown[i] = bFracPbPb[i] - gPbPbSystMeth->GetErrorY(i);
    bFracPPDown[i] = bFracPP[i] - gPPSystMeth->GetErrorY(i);
    
    double totalSysErrUp, totalSysErrDown;

    if(doCorrelated){
      double dRUp = bFracPbPbUp[i]/bFracPPUp[i]; 
      double dRDown = bFracPbPbDown[i]/bFracPPDown[i]; 
      
      // swap them if necessary
      if(dRUp > dR && dRDown < dR){
	// do nothing
      }
      else if(dRUp < dR && dRDown > dR){
	double temp = dRUp;
	dRUp = dRDown;
	dRDown = temp;
      }
      else cout<<" saddle point ! "<<endl;
      
      cout<<" dR "<<dR<<" dRUp "<<dRUp<<" dRDown "<<dRDown<<endl;
      
       totalSysErrUp = sqrt(pow(cLSysErr,2) + pow(dR*0.1,2) + pow(dRUp-dR,2) );
       totalSysErrDown = sqrt(pow(cLSysErr,2) +  pow(dR*0.1,2) + pow(dR-dRDown,2) );
    }
    else{
      double methSystPbPb = gPbPbSystMeth->GetErrorY(i);
      double methSystPP = gPPSystMeth->GetErrorY(i);
      
      double methSysErr = dR*sqrt(pow(methSystPbPb[i]/bFracPbPb[i],2) + pow(methSystPP[i]/bFracPP[i],2) );

      totalSysErrUp = sqrt(pow(cLSysErr,2) + pow(dR*0.1,2) + pow(methSysErr,2) );
      totalSysErrDown = sqrt(pow(cLSysErr,2) +  pow(dR*0.1,2) + pow(methSysErr,2) );
      

    }

    gDRerr->SetPointError(i,errxlo,errxhi,totalSysErrDown,totalSysErrUp);

    cout<<" value  "<<dR<<endl;
    cout<<" value - stat -sys "<<dR-sqrt(totalSysErrDown*totalSysErrDown+dRstatErr*dRstatErr)<<endl;
    cout<<" value + stat +sys "<<dR+sqrt(totalSysErrUp*totalSysErrUp+dRstatErr*dRstatErr)<<endl;

  }



  TCanvas *cDR = new TCanvas("cDR","double ratio",700,600);

  gDR->SetMarkerStyle(8);
  gDR->SetMarkerSize(1.5);
  hframe->Draw();
  hframe->GetXaxis()->CenterTitle();
  hframe->GetYaxis()->CenterTitle();
  //gDRerr->SetLineColor(5);
  gDRerr->SetFillColor(5);
  gDRerr->Draw("2,same");
  gDR->Draw("Z,p");

  double ptbins[4]={80,100,120,200};

  TH1D *hDRMC=new TH1D("hDRMC","hDRMC",3,ptbins);

  for(int i=0;i<3;i++){
    double dummy, val;
    gDRMC->GetPoint(i,dummy,val);
    hDRMC->SetBinContent(i+1,val);
  }

  gDRMC->SetLineColor(2);
  gDRMC->SetMarkerColor(2);
  gDRMC->SetLineWidth(2);
  hDRMC->SetLineColor(2);
  hDRMC->SetMarkerColor(2);
  hDRMC->SetLineWidth(2);
  if(drawMC){
    hDRMC->Draw("same");
    gDRMC->Draw("p");
  }
  cDR->RedrawAxis();

  TLatex *prel = new TLatex(81,2.08,"CMS Preliminary");
  prel->SetTextFont(43);
  prel->SetTextSize(23);
  prel->SetTextColor(kBlack);
  prel->Draw();

  TLatex *roots = new TLatex(160,2.08,"#sqrt{s_{NN}} = 2.76 TeV");
  roots->SetTextSize(23);
  roots->SetTextFont(43);
  roots->SetTextColor(kBlack);
  roots->Draw();

  TLegend *leg1 = new TLegend(0.2,0.15,0.99,0.34);
  leg1->SetTextFont(42);
  leg1->SetHeader("#int L dt = 150 #mub^{-1} (PbPb), 231 nb^{-1} (pp) ");
  leg1->SetBorderSize(0);
  leg1->SetFillStyle(0);
  leg1->AddEntry(gDR,"Data","p");
  leg1->AddEntry(gDRerr,"Sys. uncertainty","f");
  leg1->Draw();

  cDR->Print("PDFS/doubleRatio.pdf","pdf");
  cDR->Print("MACROS/doubleRatio.C","cxx");


}
