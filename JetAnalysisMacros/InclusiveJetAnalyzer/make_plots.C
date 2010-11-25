void make_plots(int normalize=1, int lowSubLeadingBin=8, int central=1)
{

  //gStyle->SetOptStat(1);
  gStyle->SetOptTitle(0);
  TFile *fdata, *fembedded;
  if(central)
    {
      if(central==1){
	fdata = new TFile("histos_Data_v5_010.root");
	fembedded = new TFile("histos_Hydjet_Unquenched_w010.root");
      }
      if(central==2){
	fdata = new TFile("histos_Data_v4_1030.root");
	fembedded = new TFile("histos_Hydjet_Unquenched_w1030.root");
      }
      if(central==3){
	fdata = new TFile("histos_Data_v4_3090.root");
	fembedded = new TFile("histos_Hydjet_Unquenched_w3090.root");
      }
    }
  else
    {
      if(central==1){
	fdata = new TFile("histos_Data_v3_010.root");
	fembedded = new TFile("histos_Hydjet_Unquenched_w010.root");
      }
      if(central==2){
	fdata = new TFile("histos_Data_v3_1030.root");
	fembedded = new TFile("histos_Hydjet_Unquenched_w1030.root");
      }
      if(central==3){
	fdata = new TFile("histos_Data_v3_3090.root");
	fembedded = new TFile("histos_Hydjet_Unquenched_w3090.root");
      }


      }

  TFile *fPyquen = new TFile("histos_Pyquen.root");


  TH3F* hLeading_Pt_Eta_Phi_Data = (TH3F*) fdata->Get("hLeading_Pt_Eta_Phi");
  TH3F* hLeading_Pt_Eta_Phi_Pyquen = (TH3F*) fPyquen->Get("hLeading_Pt_Eta_Phi");
  TH3F* hLeading_Pt_Eta_Phi_Embedded = (TH3F*) fembedded->Get("hLeading_Pt_Eta_Phi");
  
  TH1F *hLeadingPt_Data=hLeading_Pt_Eta_Phi_Data->Project3D("x");
  hLeadingPt_Data->SetName("hLeadinPt_Data");
  TH1F *hLeadingPt_Pyquen=hLeading_Pt_Eta_Phi_Pyquen->Project3D("x");
  hLeadingPt_Pyquen->SetName("hLeadinPt_Pyquen");
  TH1F *hLeadingPt_Embedded=hLeading_Pt_Eta_Phi_Embedded->Project3D("x");
  hLeadingPt_Embedded->SetName("hLeadinPt_Embedded");

  TCanvas *c0=new TCanvas("c0","c0",1);
  c0->SetLogy();

  hLeadingPt_Data->Sumw2();
  hLeadingPt_Data->SetMarkerStyle(8);
  

  hLeadingPt_Pyquen->SetLineColor(kred);
  hLeadingPt_Embedded->SetLineColor(kblue);
  hLeadingPt_Pyquen->SetFillColor(kredLight);
  hLeadingPt_Embedded->SetFillColor(kblueLight);
  hLeadingPt_Pyquen->SetFillStyle(3004);
  hLeadingPt_Embedded->SetFillStyle(3005);

 if(normalize){
   hLeadingPt_Data->Scale(1./hLeadingPt_Data->Integral(25,100));
   hLeadingPt_Pyquen->Scale(1./hLeadingPt_Pyquen->Integral(25,100));
   hLeadingPt_Embedded->Scale(1./hLeadingPt_Embedded->Integral(25,100));
  }



  hLeadingPt_Data->SetXTitle("Leading Jet E_{T} [GeV]");
  //if(normalize)hLeadingPt_Data->SetYTitle("Normalized above 120 GeV");
  if(normalize)hLeadingPt_Data->SetYTitle("Arbitrary Normalization");

  hLeadingPt_Data->SetTitle("Leading Jet E_{T} Distribution, |#eta| < 2");
  if(normalize)hLeadingPt_Data->SetMaximum(1.);
  else hLeadingPt_Data->SetMaximum(5e4);

  hLeadingPt_Data->GetXaxis()->SetRange(1,60);

  //hLeadingPt_Data->GetXaxis()->SetRangeUser(120,250);
  hLeadingPt_Data->Draw();
  hLeadingPt_Pyquen->Draw("sames");
  hLeadingPt_Embedded->Draw("sames");
  hLeadingPt_Data->Draw("sames");  

  TLegend *t= new TLegend(0.45,0.65,0.92,0.85);
  t->SetFillStyle(0);
  if(central==1)t->SetHeader("Leading Jet E_{T} > 120 GeV, Centrality < 10%");
  if(central==2)t->SetHeader("Leading Jet E_{T} > 120 GeV, 10% < Centrality < 30%");
  if(central==3)t->SetHeader("Leading Jet E_{T} > 120 GeV, Centrality > 30%");
  t->AddEntry(hLeadingPt_Data,"Data","p");
  //else t->AddEntry(hLeadingPt_Data,"Data, cent > 25%","p");
  t->AddEntry(hLeadingPt_Pyquen,"Pyquen","lf");
  t->AddEntry(hLeadingPt_Embedded,"Unquenched, embedded simulation","lf");
  //else t->AddEntry(hLeadingPt_Embedded,"Unquenched, embedded, cent > 25%","lf");
  t->Draw();


  TCanvas *c1a=new TCanvas("c1a","c1a",1);
  
  TH3F *hPt1_Pt2_Dphi_Data = (TH3F*)fdata->Get("hPt1_Pt2_Dphi");  
  hPt1_Pt2_Dphi_Data->SetName("hPt1_Pt2_Dphi_Data");
  TH2D *hPt1_Pt2_Data = hPt1_Pt2_Dphi_Data->Project3D("yx");
  hPt1_Pt2_Dphi_Data->SetName("hPt1_Pt2_Data");

  hPt1_Pt2_Data->SetXTitle("Leading Jet E_{T} [GeV]");
  hPt1_Pt2_Data->SetYTitle("Sub-Leading Jet E_{T} [GeV]");

  hPt1_Pt2_Data->SetTitle("Leading E_{T} vs Sub-Leading E_{T}, Data");

  hPt1_Pt2_Data->Draw("zcol");

  TCanvas *c1b=new TCanvas("c1b","c1b",1);

  TH3F *hPt1_Pt2_Dphi_Pyquen = (TH3F*)fPyquen->Get("hPt1_Pt2_Dphi");  
  hPt1_Pt2_Dphi_Pyquen->SetName("hPt1_Pt2_Dphi_Pyquen");
  TH2D *hPt1_Pt2_Pyquen = hPt1_Pt2_Dphi_Pyquen->Project3D("yx");
  hPt1_Pt2_Pyquen->SetName("hPt1_Pt2_Pyquen");

  hPt1_Pt2_Pyquen->SetXTitle("Leading Jet E_{T} [GeV]");
  hPt1_Pt2_Pyquen->SetYTitle("Sub-Leading Jet E_{T} [GeV]");

  hPt1_Pt2_Pyquen->SetTitle("Leading E_{T} vs Sub-Leading E_{T}, Pyquen");

  hPt1_Pt2_Pyquen->Draw("zcol");


  TCanvas *c1c=new TCanvas("c1c","c1c",1);

  TH3F *hPt1_Pt2_Dphi_Embedded = (TH3F*)fembedded->Get("hPt1_Pt2_Dphi");  
  hPt1_Pt2_Dphi_Embedded->SetName("hPt1_Pt2_Dphi_Embedded");
  TH2D *hPt1_Pt2_Embedded = hPt1_Pt2_Dphi_Embedded->Project3D("yx");
  hPt1_Pt2_Embedded->SetName("hPt1_Pt2_Embedded");

  hPt1_Pt2_Embedded->SetXTitle("Leading Jet E_{T} [GeV]");
  hPt1_Pt2_Embedded->SetYTitle("Sub-Leading Jet E_{T} [GeV]");
  
  hPt1_Pt2_Embedded->SetTitle("Leading E_{T} vs Sub-Leading E_{T}, Unquenched, Embedded");
  hPt1_Pt2_Embedded->Draw("zcol");


  
  TCanvas *c2a=new TCanvas("c2a","c2a",1);
  c2a->SetLogy();

  TH1D *hPt1_NoSubLeading35_Data =hPt1_Pt2_Data->ProjectionX("hPt1_NoSubLeading35_Data",1,7);
  TH1D *hPt1_SubLeadingGT35_Data =hPt1_Pt2_Data->ProjectionX("hPt1_SubLeadingGT35_Data",8,500);

  hPt1_SubLeadingGT35_Data->SetTitle("Leading Jet E_{T}, Data");

  hPt1_SubLeadingGT35_Data->Draw();
  hPt1_NoSubLeading35_Data->SetLineColor(2);
  hPt1_NoSubLeading35_Data->Draw("same");


  TLegend *t2= new TLegend(0.6,0.6,0.9,0.9);
  t2->SetFillStyle(0);
  t2->AddEntry(hPt1_SubLeadingGT35_Data,"Sub-Leading E_{T}> 35 GeV","l");
  t2->AddEntry(hPt1_NoSubLeading35_Data,"Sub-Leading E_{T} < 35 GeV","l");

  t2->Draw();

  TCanvas *c2b=new TCanvas("c2b","c2b",1);
  c2b->SetLogy();



  TH1D *hPt1_NoSubLeading35_Pyquen =hPt1_Pt2_Pyquen->ProjectionX("hPt1_NoSubLeading35_Pyquen",1,7);
  TH1D *hPt1_SubLeadingGT35_Pyquen =hPt1_Pt2_Pyquen->ProjectionX("hPt1_SubLeadingGT35_Pyquen",8,500);

  hPt1_SubLeadingGT35_Pyquen->SetTitle("Leading Jet E_{T}, Pyquen"); 

  hPt1_SubLeadingGT35_Pyquen->Draw();
  hPt1_NoSubLeading35_Pyquen->SetLineColor(2);
  hPt1_NoSubLeading35_Pyquen->Draw("same");


  t2->Draw();

  TCanvas *c2c=new TCanvas("c2c","c2c",1);
  c2c->SetLogy();

  TH1D *hPt1_NoSubLeading35_Embedded =hPt1_Pt2_Embedded->ProjectionX("hPt1_NoSubLeading35_Embedded",1,7);
  TH1D *hPt1_SubLeadingGT35_Embedded =hPt1_Pt2_Embedded->ProjectionX("hPt1_SubLeadingGT35_Embedded",8,500);

  hPt1_SubLeadingGT35_Embedded->SetTitle("Leading Jet E_{T}, Embedded");

  hPt1_SubLeadingGT35_Embedded->Draw();
  hPt1_NoSubLeading35_Embedded->SetLineColor(2);
  hPt1_NoSubLeading35_Embedded->Draw("same");

  t2->Draw();

  TCanvas *c3a=new TCanvas("c3a","c3a",1);
  c3a->SetLogy();

  TH1D *hDphi_Data = hPt1_Pt2_Dphi_Data->ProjectionZ("Dphi_Data",25,500,lowSubLeadingBin,500);
  TH1D *hDphi_Pyquen = hPt1_Pt2_Dphi_Pyquen->ProjectionZ("Dphi_Pyquen",25,500,lowSubLeadingBin,500);
  TH1D *hDphi_Embedded = hPt1_Pt2_Dphi_Embedded->ProjectionZ("Dphi_Embedded",25,500,lowSubLeadingBin,500);

  hDphi_Data->SetTitle("#Delta#phi,Leading E_{T} > 120 GeV, Sub-Leading > 35 GeV");
  //hDphi_Data->SetMaximum(5e4);

  hDphi_Data->Sumw2();
  hDphi_Data->SetMarkerStyle(8);

  if(normalize){
  hDphi_Data->Scale(1./hDphi_Data->Integral());
  hDphi_Pyquen->Scale(1./hDphi_Pyquen->Integral());
  hDphi_Embedded->Scale(1./hDphi_Embedded->Integral());
  }
  hDphi_Data->SetXTitle("#Delta#phi [rad]");
  //if(normalize)hDphi_Data->SetYTitle("Normalized to Unity");
  hDphi_Data->SetYTitle("Arbitrary Normalization");

  hDphi_Data->SetLineWidth(2);
  hDphi_Pyquen->SetLineWidth(2);
  hDphi_Embedded->SetLineWidth(2);

  hDphi_Pyquen->SetFillColor(kredLight);
  hDphi_Embedded->SetFillColor(kblueLight);
  hDphi_Pyquen->SetFillStyle(3004);
  hDphi_Embedded->SetFillStyle(3005);
 
  hDphi_Data->SetMaximum(1.);
  hDphi_Data->SetMinimum(1e-3);


  hDphi_Data->Draw();
  hDphi_Pyquen->SetLineColor(kred);
  hDphi_Pyquen->Draw("sames");
  hDphi_Embedded->SetLineColor(kblue);
  hDphi_Embedded->Draw("sames");
  hDphi_Data->Draw("sames");


 TLegend *ta= new TLegend(0.15,0.65,0.62,0.85);
  ta->SetFillStyle(0);
  if(central==1)ta->SetHeader("Leading Jet E_{T} > 120 GeV, Sub-Leading Jet E_{T} > 35 GeV, Centrality < 10%");
  if(central==2)ta->SetHeader("Leading Jet E_{T} > 120 GeV, Sub-Leading Jet E_{T} > 35 GeV, 10% < Centrality < 30%");
  if(central==3)ta->SetHeader("Leading Jet E_{T} > 120 GeV, Sub-Leading Jet E_{T} > 35 GeV, Centrality > 30%");
  ta->AddEntry(hDphi_Data,"Data","p");
  //else t->AddEntry(hDphi_Data,"Data, cent > 25%","p");
  ta->AddEntry(hDphi_Pyquen,"Pyquen","lf");
  ta->AddEntry(hDphi_Embedded,"Unquenched, embedded simulation","lf");
  //else t->AddEntry(hDphi_Embedded,"Unquenched, embedded, cent > 25%","lf");
  ta->Draw();




  TCanvas *c3b=new TCanvas("c3b","c3b",1);
  c3b->SetLogy();

  TH1D *hDphi50_Data = hPt1_Pt2_Dphi_Data->ProjectionZ("Dphi_Data50",25,500,11,500);
  TH1D *hDphi50_Pyquen = hPt1_Pt2_Dphi_Pyquen->ProjectionZ("Dphi50_Pyquen",25,500,11,500);
  TH1D *hDphi50_Embedded = hPt1_Pt2_Dphi_Embedded->ProjectionZ("Dphi50_Embedded",25,500,11,500);

  hDphi50_Data->SetTitle("#Delta#phi,Leading E_{T} > 120 GeV, Sub-Leading > 50 GeV");
  //hDphi50_Data->SetMaximum(5e4);

  hDphi50_Data->Sumw2();
  hDphi50_Data->SetMarkerStyle(8);

  if(normalize){
  hDphi50_Data->Scale(1./hDphi50_Data->Integral());
  hDphi50_Pyquen->Scale(1./hDphi50_Pyquen->Integral());
  hDphi50_Embedded->Scale(1./hDphi50_Embedded->Integral());
  }
  hDphi50_Data->SetXTitle("#Delta#phi [rad]");
  ///if(normalize)hDphi50_Data->SetYTitle("Normalized to Unity");
  if(normalize)hDphi50_Data->SetYTitle("Aribtrary Normalization");

  hDphi50_Data->SetLineWidth(2);
  hDphi50_Pyquen->SetLineWidth(2);
  hDphi50_Embedded->SetLineWidth(2);

  hDphi50_Pyquen->SetFillColor(kredLight);
  hDphi50_Embedded->SetFillColor(kblueLight);
  hDphi50_Pyquen->SetFillStyle(3004);
  hDphi50_Embedded->SetFillStyle(3005);

  hDphi50_Data->SetMaximum(0.7);
  hDphi50_Data->Draw();
  hDphi50_Pyquen->SetLineColor(kred);
  //hDphi50_Pyquen->Draw("sames");
  hDphi50_Embedded->SetLineColor(kblue);

  hDphi50_Data->SetMaximum(1.);
  hDphi50_Data->SetMinimum(1e-3);


  hDphi50_Embedded->Draw("sames");
  hDphi50_Data->Draw("sames");

TLegend *tb= new TLegend(0.15,0.65,0.62,0.85);
  tb->SetFillStyle(0);
  if(central==1)tb->SetHeader("Leading Jet E_{T} > 120 GeV, Sub-Leading Jet E_{T} > 50 GeV, Centrality < 10%");
  if(central==2)tb->SetHeader("Leading Jet E_{T} > 120 GeV, Sub-Leading Jet E_{T} > 50 GeV, 10% < Centrality < 30%");
  if(central==3)tb->SetHeader("Leading Jet E_{T} > 120 GeV, Sub-Leading Jet E_{T} > 50 GeV, Centrality > 30%");
  
  tb->AddEntry(hDphi50_Data,"Data","p");
  //else t->AddEntry(hDphi_Data,"Data, cent > 25%","p");
  tb->AddEntry(hDphi_Pyquen,"Pyquen","lf");
  tb->AddEntry(hDphi50_Embedded,"Unquenched, embedded simulation","lf");
  //else t->AddEntry(hDphi_Embedded,"Unquenched, embedded, cent > 25%","lf");
  tb->Draw();





  TCanvas *c4=new TCanvas("c4","c4",1);
  c4->SetLogy();

  float njet_Data = hPt1_Pt2_Dphi_Data->Integral(25,500,1,500,17,20);
  float njet_Pyquen = hPt1_Pt2_Dphi_Pyquen->Integral(25,500,1,500,17,20);
  float njet_Embedded = hPt1_Pt2_Dphi_Embedded->Integral(25,500,1,500,17,20);

  hPt1_Pt2_Dphi_Data->GetXaxis()->SetRange(25,500);
  hPt1_Pt2_Dphi_Pyquen->GetXaxis()->SetRange(25,500);
  hPt1_Pt2_Dphi_Embedded->GetXaxis()->SetRange(25,500);

  hPt1_Pt2_Dphi_Data->GetZaxis()->SetRange(17,20);
  hPt1_Pt2_Dphi_Pyquen->GetZaxis()->SetRange(17,20);
  hPt1_Pt2_Dphi_Embedded->GetZaxis()->SetRange(17,20);

  TH1D *hSubLeadingPt_Data = hPt1_Pt2_Dphi_Data->Project3D("y");
  TH1D *hSubLeadingPt_Pyquen = hPt1_Pt2_Dphi_Pyquen->Project3D("y");
  TH1D *hSubLeadingPt_Embedded = hPt1_Pt2_Dphi_Embedded->Project3D("y");
  
  hSubLeadingPt_Data->Sumw2();
  hSubLeadingPt_Data->SetMarkerStyle(8);


  if(normalize){
    hSubLeadingPt_Data->Scale(1./njet_Data);
    hSubLeadingPt_Pyquen->Scale(1./njet_Pyquen);
    hSubLeadingPt_Embedded->Scale(1./njet_Embedded);
    hSubLeadingPt_Data->Scale(1./hSubLeadingPt_Data->GetBinWidth(1));
    hSubLeadingPt_Pyquen->Scale(1./hSubLeadingPt_Pyquen->GetBinWidth(1));
    hSubLeadingPt_Embedded->Scale(1./hSubLeadingPt_Embedded->GetBinWidth(1));

    hSubLeadingPt_Data->SetMaximum(0.16);

  }
    else hSubLeadingPt_Data->SetMaximum(1e4);

  hSubLeadingPt_Data->SetTitle("Sub-Leading E_{T} Dist., Leading E_{T} > 120 GeV");

  hSubLeadingPt_Data->SetXTitle("Sub-Leading E_{T} [GeV]");
  if(normalize)hSubLeadingPt_Data->SetYTitle("(1 / # of Leading Jets) dN/dE_{T} [GeV^{-1}]");

  hSubLeadingPt_Data->SetLineWidth(2);
  hSubLeadingPt_Pyquen->SetLineWidth(2);
  hSubLeadingPt_Embedded->SetLineWidth(2);
  /*
  hSubLeadingPt_Data->Rebin(4);
  hSubLeadingPt_Pyquen->Rebin(4);
  hSubLeadingPt_Embedded->Rebin(4);
  */

  hSubLeadingPt_Pyquen->SetLineColor(kred);
  hSubLeadingPt_Embedded->SetLineColor(kblue);
  hSubLeadingPt_Pyquen->SetFillColor(kredLight);
  hSubLeadingPt_Embedded->SetFillColor(kblueLight);
  hSubLeadingPt_Pyquen->SetFillStyle(3004);
  hSubLeadingPt_Embedded->SetFillStyle(3005);

  hSubLeadingPt_Data->GetXaxis()->SetRange(1,60);

  hSubLeadingPt_Data->Draw();
  hSubLeadingPt_Pyquen->Draw("sames");
  hSubLeadingPt_Embedded->Draw("sames");
  hSubLeadingPt_Data->Draw("sames");


  t->Draw();





  int draw_sim = 0;

  if(draw_sim){
  
  TCanvas *c5a=new TCanvas("c5a","c5a",1);
  c5a->SetLogy();

  TH2F *hLeadingResponseVsPt_Pyquen = fPyquen->Get("hLeadingResponseVsPt");
  TH2F *hLeadingResponseVsPt_Embedded = fembedded->Get("hLeadingResponseVsPt");
  
  TH1D *hLeadingResponseVsPt_50_100_Pyquen =  hLeadingResponseVsPt_Pyquen->ProjectionX("hLeadinResponseVsPt_50_100_Pyquen",20,28);
  TH1D *hLeadingResponseVsPt_50_100_Embedded =  hLeadingResponseVsPt_Embedded->ProjectionX("hLeadinResponseVsPt_50_100_Embedded",20,28);
  
  hLeadingResponseVsPt_50_100_Pyquen->SetLineColor(kred);
  hLeadingResponseVsPt_50_100_Embedded->SetLineColor(kblue);
  hLeadingResponseVsPt_50_100_Pyquen->SetFillColor(kredLight);
  hLeadingResponseVsPt_50_100_Embedded->SetFillColor(kblueLight);
  hLeadingResponseVsPt_50_100_Pyquen->SetFillStyle(3004);
  hLeadingResponseVsPt_50_100_Embedded->SetFillStyle(3005);
  hLeadingResponseVsPt_50_100_Pyquen->SetLineWidth(2);
  hLeadingResponseVsPt_50_100_Embedded->SetLineWidth(2);

  hLeadingResponseVsPt_50_100_Pyquen->GetXaxis()->SetRangeUser(0,2);

  hLeadingResponseVsPt_50_100_Pyquen->SetTitle("Leading Jet Response,  120 GeV < Leading E_{T} < 140 GeV");
  hLeadingResponseVsPt_50_100_Pyquen->SetXTitle("Reco E_{T}/Gen E_{T}");
  hLeadingResponseVsPt_50_100_Pyquen->SetYTitle("Normalized to Unity");


  if(normalize){
    hLeadingResponseVsPt_50_100_Pyquen->Scale(1./hLeadingResponseVsPt_50_100_Pyquen->Integral());
    hLeadingResponseVsPt_50_100_Embedded->Scale(1./hLeadingResponseVsPt_50_100_Embedded->Integral());
  }

  hLeadingResponseVsPt_50_100_Pyquen->Draw();
  hLeadingResponseVsPt_50_100_Embedded->Draw("sames");

  TLegend *t3=new TLegend(0.6,0.6,0.9,0.9); 
  t3->AddEntry(hLeadingResponseVsPt_50_100_Pyquen,"Pyquen","pl");
  t3->AddEntry(hLeadingResponseVsPt_50_100_Embedded,"Unquenched,  Embedded, 0-10%","pl");
  t3->SetFillColor(0);
  t3->SetBorderSize(1);
  t3->Draw();

  TCanvas *c5b=new TCanvas("c5b","c5b",1);
  c5b->SetLogy();

  TH2F *hSubLeadingResponseVsPt_Pyquen = fPyquen->Get("hSubLeadingResponseVsPt");
  TH2F *hSubLeadingResponseVsPt_Embedded = fembedded->Get("hSubLeadingResponseVsPt");
  
  TH1D *hSubLeadingResponseVsPt_50_100_Pyquen =  hSubLeadingResponseVsPt_Pyquen->ProjectionX("hSubLeadinResponseVsPt_50_100_Pyquen",11,20);
  TH1D *hSubLeadingResponseVsPt_50_100_Embedded =  hSubLeadingResponseVsPt_Embedded->ProjectionX("hSubLeadinResponseVsPt_50_100_Embedded",11,20);
  
  hSubLeadingResponseVsPt_50_100_Pyquen->SetLineColor(kred);
  hSubLeadingResponseVsPt_50_100_Embedded->SetLineColor(kblue);
  hSubLeadingResponseVsPt_50_100_Pyquen->SetFillColor(kredLight);
  hSubLeadingResponseVsPt_50_100_Embedded->SetFillColor(kblueLight);
  hSubLeadingResponseVsPt_50_100_Pyquen->SetFillStyle(3004);
  hSubLeadingResponseVsPt_50_100_Embedded->SetFillStyle(3005);
  hSubLeadingResponseVsPt_50_100_Pyquen->SetLineWidth(2);
  hSubLeadingResponseVsPt_50_100_Embedded->SetLineWidth(2);

  hSubLeadingResponseVsPt_50_100_Pyquen->GetXaxis()->SetRangeUser(0,2);

  hSubLeadingResponseVsPt_50_100_Pyquen->SetTitle("Sub-Leading Jet Response, 50 GeV < Sub-Leading E_{T} < 100 GeV");
  hSubLeadingResponseVsPt_50_100_Pyquen->SetXTitle("Reco E_{T}/Gen E_{T}");
  hSubLeadingResponseVsPt_50_100_Pyquen->SetYTitle("Normalized to Unity");


 if(normalize){
    hSubLeadingResponseVsPt_50_100_Pyquen->Scale(1./hSubLeadingResponseVsPt_50_100_Pyquen->Integral());
    hSubLeadingResponseVsPt_50_100_Embedded->Scale(1./hSubLeadingResponseVsPt_50_100_Embedded->Integral());
  }
  hSubLeadingResponseVsPt_50_100_Pyquen->Draw();
  hSubLeadingResponseVsPt_50_100_Embedded->Draw("sames");

  t3->Draw();


 TCanvas *c6a=new TCanvas("c6a","c6a",1);
  c6a->SetLogy();

  TH2F *hLeadingResolutionVsPt_Pyquen = fPyquen->Get("hLeadingResolutionVsPt");
  TH2F *hLeadingResolutionVsPt_Embedded = fembedded->Get("hLeadingResolutionVsPt");
  
  TH1D *hLeadingResolutionVsPt_50_100_Pyquen =  hLeadingResolutionVsPt_Pyquen->ProjectionX("hLeadinResolutionVsPt_50_100_Pyquen",20,28);
  TH1D *hLeadingResolutionVsPt_50_100_Embedded =  hLeadingResolutionVsPt_Embedded->ProjectionX("hLeadinResolutionVsPt_50_100_Embedded",20,28);
  
  hLeadingResolutionVsPt_50_100_Pyquen->SetLineColor(kred);
  hLeadingResolutionVsPt_50_100_Embedded->SetLineColor(kblue);

  hLeadingResolutionVsPt_50_100_Pyquen->GetXaxis()->SetRangeUser(-1,1);

  hLeadingResolutionVsPt_50_100_Pyquen->SetTitle("Leading Jet Resolution,  120 GeV < Leading E_{T} < 140 GeV");
  hLeadingResolutionVsPt_50_100_Pyquen->SetXTitle("(Reco E_{T}-Gen E_{T})/Gen E_{T}");

 if(normalize){
    hLeadingResolutionVsPt_50_100_Pyquen->Scale(1./hLeadingResolutionVsPt_50_100_Pyquen->Integral());
    hLeadingResolutionVsPt_50_100_Embedded->Scale(1./hLeadingResolutionVsPt_50_100_Embedded->Integral());
  }

  hLeadingResolutionVsPt_50_100_Pyquen->Draw();
  hLeadingResolutionVsPt_50_100_Embedded->Draw("same");

  TCanvas *c6b=new TCanvas("c6b","c6b",1);
  c6b->SetLogy();

  TH2F *hSubLeadingResolutionVsPt_Pyquen = fPyquen->Get("hSubLeadingResolutionVsPt");
  TH2F *hSubLeadingResolutionVsPt_Embedded = fembedded->Get("hSubLeadingResolutionVsPt");
  
  TH1D *hSubLeadingResolutionVsPt_50_100_Pyquen =  hSubLeadingResolutionVsPt_Pyquen->ProjectionX("hSubLeadinResolutionVsPt_50_100_Pyquen",11,20);
  TH1D *hSubLeadingResolutionVsPt_50_100_Embedded =  hSubLeadingResolutionVsPt_Embedded->ProjectionX("hSubLeadinResolutionVsPt_50_100_Embedded",11,20);
  
  hSubLeadingResolutionVsPt_50_100_Pyquen->SetLineColor(kred);
  hSubLeadingResolutionVsPt_50_100_Embedded->SetLineColor(kblue);

  hSubLeadingResolutionVsPt_50_100_Pyquen->GetXaxis()->SetRangeUser(-1,1);

  hSubLeadingResolutionVsPt_50_100_Pyquen->SetTitle("Sub-Leading Jet Resolution,  50 GeV < Sub-Leading E_{T} < 100 GeV");
  hSubLeadingResolutionVsPt_50_100_Pyquen->SetXTitle("(Reco E_{T}-Gen E_{T})/Gen E_{T}");

 if(normalize){
    hSubLeadingResolutionVsPt_50_100_Pyquen->Scale(1./hSubLeadingResolutionVsPt_50_100_Pyquen->Integral());
    hSubLeadingResolutionVsPt_50_100_Embedded->Scale(1./hSubLeadingResolutionVsPt_50_100_Embedded->Integral());
  }

  hSubLeadingResolutionVsPt_50_100_Pyquen->Draw();
  hSubLeadingResolutionVsPt_50_100_Embedded->Draw("same");

  

  TCanvas *c7=new TCanvas("c7","c7",1);
  c7->SetLogy();
  
  TH1F* hGenLeadingJetPt_Pyquen = fPyquen->Get("hGenLeadingJetPt");
  TH1F* hGenLeadingJetPt_Embedded = fembedded->Get("hGenLeadingJetPt");

  hGenLeadingJetPt_Pyquen->SetLineColor(kred);
  hGenLeadingJetPt_Embedded->SetLineColor(kblue);
  hGenLeadingJetPt_Pyquen->SetFillColor(kredLight);
  hGenLeadingJetPt_Embedded->SetFillColor(kblueLight);
  hGenLeadingJetPt_Pyquen->SetFillStyle(3004);
  hGenLeadingJetPt_Embedded->SetFillStyle(3005);



  hGenLeadingJetPt_Pyquen->SetTitle("Generated E_{T} Dist. (#hat{p}_{T} > 80 GeV/c)");
  hGenLeadingJetPt_Pyquen->SetXTitle("Generated E_{T} [GeV]");
  hGenLeadingJetPt_Pyquen->SetYTitle("Normalized to Unity");



 if(normalize){
   hGenLeadingJetPt_Pyquen->Scale(1./hGenLeadingJetPt_Pyquen->Integral(2,500));
   hGenLeadingJetPt_Embedded->Scale(1./hGenLeadingJetPt_Embedded->Integral(2,500));
  }

 hGenLeadingJetPt_Pyquen->Rebin(5);
 hGenLeadingJetPt_Embedded->Rebin(5);

 hGenLeadingJetPt_Pyquen->SetLineWidth(2);
 hGenLeadingJetPt_Embedded->SetLineWidth(2);

  hGenLeadingJetPt_Pyquen->Draw();
  hGenLeadingJetPt_Embedded->Draw("same");
  
  t3->Draw();

  }

}
