{
//=========Macro generated from canvas: cBFraction2/b-jet Fraction
//=========  (Thu Jul 26 18:20:56 2012) by ROOT version5.26/00
   TCanvas *cBFraction2 = new TCanvas("cBFraction2", "b-jet Fraction",20,42,700,600);
   gStyle->SetOptFit(1);
   gStyle->SetOptStat(0);
   gStyle->SetOptTitle(0);
   cBFraction2->Range(54.28572,-0.008674699,225.7143,0.06361446);
   cBFraction2->SetFillColor(0);
   cBFraction2->SetBorderMode(0);
   cBFraction2->SetBorderSize(2);
   cBFraction2->SetTickx(1);
   cBFraction2->SetTicky(1);
   cBFraction2->SetLeftMargin(0.15);
   cBFraction2->SetRightMargin(0.15);
   cBFraction2->SetTopMargin(0.05);
   cBFraction2->SetBottomMargin(0.12);
   cBFraction2->SetFrameFillStyle(0);
   cBFraction2->SetFrameBorderMode(0);
   cBFraction2->SetFrameFillStyle(0);
   cBFraction2->SetFrameBorderMode(0);
   Double_t xAxis1[4] = {80, 100, 120, 200}; 
   
   TH1D *hBFractionMC2 = new TH1D("hBFractionMC2","hBFractionMC",3, xAxis1);
   hBFractionMC2->SetBinContent(1,0.02846329);
   hBFractionMC2->SetBinContent(2,0.02658122);
   hBFractionMC2->SetBinContent(3,0.02463515);
   hBFractionMC2->SetBinError(1,0.0002591295);
   hBFractionMC2->SetBinError(2,0.0002592612);
   hBFractionMC2->SetBinError(3,0.0003185524);
   hBFractionMC2->SetMinimum(0);
   hBFractionMC2->SetMaximum(0.06);
   hBFractionMC2->SetEntries(6);
   hBFractionMC2->SetLineColor(2);
   hBFractionMC2->SetLineStyle(0);
   hBFractionMC2->SetLineWidth(2);
   hBFractionMC2->SetMarkerColor(2);
   hBFractionMC2->SetMarkerStyle(1);
   hBFractionMC2->SetMarkerSize(0);
   hBFractionMC2->GetXaxis()->SetTitle("Jet p_{T} (GeV/c)");
   hBFractionMC2->GetXaxis()->CenterTitle(true);
   hBFractionMC2->GetXaxis()->SetLabelFont(43);
   hBFractionMC2->GetXaxis()->SetLabelOffset(0.007);
   hBFractionMC2->GetXaxis()->SetLabelSize(28);
   hBFractionMC2->GetXaxis()->SetTitleSize(28);
   hBFractionMC2->GetXaxis()->SetTitleFont(43);
   hBFractionMC2->GetYaxis()->SetTitle("b-jet fraction");
   hBFractionMC2->GetYaxis()->CenterTitle(true);
   hBFractionMC2->GetYaxis()->SetNdivisions(408);
   hBFractionMC2->GetYaxis()->SetLabelFont(43);
   hBFractionMC2->GetYaxis()->SetLabelOffset(0.007);
   hBFractionMC2->GetYaxis()->SetLabelSize(28);
   hBFractionMC2->GetYaxis()->SetTitleSize(28);
   hBFractionMC2->GetYaxis()->SetTitleOffset(1.5);
   hBFractionMC2->GetYaxis()->SetTitleFont(43);
   hBFractionMC2->GetZaxis()->SetLabelFont(43);
   hBFractionMC2->GetZaxis()->SetLabelOffset(0.007);
   hBFractionMC2->GetZaxis()->SetLabelSize(28);
   hBFractionMC2->GetZaxis()->SetTitleSize(28);
   hBFractionMC2->GetZaxis()->SetTitleFont(43);
   hBFractionMC2->Draw("hist");
   TLatex *   tex = new TLatex(81,0.061,"CMS Preliminary");
   tex->SetTextFont(43);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
      tex = new TLatex(168,0.061,"#sqrt{s} = 2.76 TeV");
   tex->SetTextFont(43);
   tex->SetTextSize(23);
   tex->SetLineWidth(2);
   tex->Draw();
   
   TGraphErrors *gre = new TGraphErrors(3);
   gre->SetName("gSyst");
   gre->SetTitle("Graph");
   gre->SetFillColor(5);
   gre->SetMarkerStyle(20);
   gre->SetPoint(0,90,0.02105573);
   gre->SetPointError(0,10,0.003402182);
   gre->SetPoint(1,110,0.03030188);
   gre->SetPointError(1,10,0.007488027);
   gre->SetPoint(2,160,0.02993758);
   gre->SetPointError(2,40,0.00885989);
   
   TH1F *gSyst1__1 = new TH1F("gSyst1__1","Graph",100,68,212);
   gSyst1__1->SetMinimum(0.01553915);
   gSyst1__1->SetMaximum(0.04091186);
   gSyst1__1->SetDirectory(0);
   gSyst1__1->SetStats(0);
   gSyst1__1->SetLineStyle(0);
   gSyst1__1->SetLineWidth(2);
   gSyst1__1->SetMarkerStyle(20);
   gSyst1__1->GetXaxis()->SetLabelFont(43);
   gSyst1__1->GetXaxis()->SetLabelOffset(0.007);
   gSyst1__1->GetXaxis()->SetLabelSize(28);
   gSyst1__1->GetXaxis()->SetTitleSize(43);
   gSyst1__1->GetXaxis()->SetTitleOffset(1.5);
   gSyst1__1->GetXaxis()->SetTitleFont(43);
   gSyst1__1->GetYaxis()->SetNdivisions(408);
   gSyst1__1->GetYaxis()->SetLabelFont(43);
   gSyst1__1->GetYaxis()->SetLabelOffset(0.007);
   gSyst1__1->GetYaxis()->SetLabelSize(28);
   gSyst1__1->GetYaxis()->SetTitleSize(28);
   gSyst1__1->GetYaxis()->SetTitleOffset(1.5);
   gSyst1__1->GetYaxis()->SetTitleFont(43);
   gSyst1__1->GetZaxis()->SetLabelFont(43);
   gSyst1__1->GetZaxis()->SetLabelOffset(0.007);
   gSyst1__1->GetZaxis()->SetLabelSize(28);
   gSyst1__1->GetZaxis()->SetTitleSize(28);
   gSyst1__1->GetZaxis()->SetTitleFont(43);
   gre->SetHistogram(gSyst1__1);
   
   gre->Draw("2");
   
   TGraphAsymmErrors *grae = new TGraphAsymmErrors(3);
   grae->SetName("Graph_from_hBFractionMC");
   grae->SetTitle("hBFractionMC");
   grae->SetLineColor(2);
   grae->SetLineStyle(0);
   grae->SetLineWidth(2);
   grae->SetMarkerColor(2);
   grae->SetMarkerStyle(1);
   grae->SetPoint(0,87.83,0.02846329);
   grae->SetPointError(0,7.830002,12.17,0.0002591295,0.0002591295);
   grae->SetPoint(1,108.2,0.02658122);
   grae->SetPointError(1,8.199997,11.8,0.0002592612,0.0002592612);
   grae->SetPoint(2,140.1,0.02463515);
   grae->SetPointError(2,20.10001,59.89999,0.0003185524,0.0003185524);
   
   TH1F *Graph_from_hBFractionMC1 = new TH1F("Graph_from_hBFractionMC1","hBFractionMC",100,68,212);
   Graph_from_hBFractionMC1->SetMinimum(0.02387602);
   Graph_from_hBFractionMC1->SetMaximum(0.02916301);
   Graph_from_hBFractionMC1->SetDirectory(0);
   Graph_from_hBFractionMC1->SetStats(0);
   Graph_from_hBFractionMC1->SetLineStyle(0);
   Graph_from_hBFractionMC1->SetLineWidth(2);
   Graph_from_hBFractionMC1->SetMarkerStyle(1);
   Graph_from_hBFractionMC1->GetXaxis()->SetRange(9,92);
   Graph_from_hBFractionMC1->GetXaxis()->SetLabelFont(42);
   Graph_from_hBFractionMC1->GetXaxis()->SetLabelOffset(0.007);
   Graph_from_hBFractionMC1->GetXaxis()->SetLabelSize(0.05);
   Graph_from_hBFractionMC1->GetXaxis()->SetTitleSize(0.05);
   Graph_from_hBFractionMC1->GetXaxis()->SetTitleOffset(1.5);
   Graph_from_hBFractionMC1->GetXaxis()->SetTitleFont(42);
   Graph_from_hBFractionMC1->GetYaxis()->SetNdivisions(408);
   Graph_from_hBFractionMC1->GetYaxis()->SetLabelFont(42);
   Graph_from_hBFractionMC1->GetYaxis()->SetLabelOffset(0.007);
   Graph_from_hBFractionMC1->GetYaxis()->SetLabelSize(0.05);
   Graph_from_hBFractionMC1->GetYaxis()->SetTitleSize(0.05);
   Graph_from_hBFractionMC1->GetYaxis()->SetTitleOffset(1.5);
   Graph_from_hBFractionMC1->GetYaxis()->SetTitleFont(42);
   Graph_from_hBFractionMC1->GetZaxis()->SetLabelFont(42);
   Graph_from_hBFractionMC1->GetZaxis()->SetLabelOffset(0.007);
   Graph_from_hBFractionMC1->GetZaxis()->SetLabelSize(0.05);
   Graph_from_hBFractionMC1->GetZaxis()->SetTitleSize(0.06);
   Graph_from_hBFractionMC1->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_from_hBFractionMC1);
   
   grae->Draw("z,p,");
   Double_t xAxis2[4] = {80, 100, 120, 200}; 
   
   TH1D *hBFractionMC2 = new TH1D("hBFractionMC2","hBFractionMC",3, xAxis2);
   hBFractionMC2->SetBinContent(1,0.02846329);
   hBFractionMC2->SetBinContent(2,0.02658122);
   hBFractionMC2->SetBinContent(3,0.02463515);
   hBFractionMC2->SetBinError(1,0.0002591295);
   hBFractionMC2->SetBinError(2,0.0002592612);
   hBFractionMC2->SetBinError(3,0.0003185524);
   hBFractionMC2->SetMinimum(0);
   hBFractionMC2->SetMaximum(0.06);
   hBFractionMC2->SetEntries(6);
   hBFractionMC2->SetLineColor(2);
   hBFractionMC2->SetLineStyle(0);
   hBFractionMC2->SetLineWidth(2);
   hBFractionMC2->SetMarkerColor(2);
   hBFractionMC2->SetMarkerStyle(1);
   hBFractionMC2->SetMarkerSize(0);
   hBFractionMC2->GetXaxis()->SetTitle("Jet p_{T} (GeV/c)");
   hBFractionMC2->GetXaxis()->CenterTitle(true);
   hBFractionMC2->GetXaxis()->SetLabelFont(42);
   hBFractionMC2->GetXaxis()->SetLabelOffset(0.007);
   hBFractionMC2->GetXaxis()->SetLabelSize(0.05);
   hBFractionMC2->GetXaxis()->SetTitleSize(0.05);
   hBFractionMC2->GetXaxis()->SetTitleFont(42);
   hBFractionMC2->GetYaxis()->SetTitle("b-jet fraction");
   hBFractionMC2->GetYaxis()->CenterTitle(true);
   hBFractionMC2->GetYaxis()->SetNdivisions(408);
   hBFractionMC2->GetYaxis()->SetLabelFont(42);
   hBFractionMC2->GetYaxis()->SetLabelOffset(0.007);
   hBFractionMC2->GetYaxis()->SetLabelSize(0.05);
   hBFractionMC2->GetYaxis()->SetTitleSize(0.05);
   hBFractionMC2->GetYaxis()->SetTitleOffset(1.5);
   hBFractionMC2->GetYaxis()->SetTitleFont(42);
   hBFractionMC2->GetZaxis()->SetLabelFont(42);
   hBFractionMC2->GetZaxis()->SetLabelOffset(0.007);
   hBFractionMC2->GetZaxis()->SetLabelSize(0.05);
   hBFractionMC2->GetZaxis()->SetTitleSize(0.06);
   hBFractionMC2->GetZaxis()->SetTitleFont(42);
   hBFractionMC2->Draw("hist,same");
   
   grae = new TGraphAsymmErrors(3);
   grae->SetName("Graph_from_hBFractionDataLTJP_SSVHEat2.0_CLshift0");
   grae->SetTitle("hBFractionDataLTJP");
   grae->SetLineStyle(0);
   grae->SetLineWidth(2);
   grae->SetMarkerStyle(8);
   grae->SetMarkerSize(1.5);
   grae->SetPoint(0,87.88,0.02105573);
   grae->SetPointError(0,0,0,0.002561513,0.002561513);
   grae->SetPoint(1,108.2,0.03030188);
   grae->SetPointError(1,0,0,0.005336787,0.005336787);
   grae->SetPoint(2,140.6,0.02993758);
   grae->SetPointError(2,0,0,0.006678497,0.006678497);
   
   TH1F *Graph_from_hBFractionDataLTJP_Fix = new TH1F("Graph_from_hBFractionDataLTJP_Fix","hBFractionDataLTJP",100,82.608,145.872);
   Graph_from_hBFractionDataLTJP_Fix->SetMinimum(0.01668203);
   Graph_from_hBFractionDataLTJP_Fix->SetMaximum(0.03842826);
   Graph_from_hBFractionDataLTJP_Fix->SetDirectory(0);
   Graph_from_hBFractionDataLTJP_Fix->SetStats(0);
   Graph_from_hBFractionDataLTJP_Fix->SetLineStyle(0);
   Graph_from_hBFractionDataLTJP_Fix->SetLineWidth(2);
   Graph_from_hBFractionDataLTJP_Fix->SetMarkerStyle(20);
   Graph_from_hBFractionDataLTJP_Fix->GetXaxis()->SetLabelFont(42);
   Graph_from_hBFractionDataLTJP_Fix->GetXaxis()->SetLabelOffset(0.007);
   Graph_from_hBFractionDataLTJP_Fix->GetXaxis()->SetLabelSize(0.05);
   Graph_from_hBFractionDataLTJP_Fix->GetXaxis()->SetTitleSize(0.05);
   Graph_from_hBFractionDataLTJP_Fix->GetXaxis()->SetTitleOffset(1.5);
   Graph_from_hBFractionDataLTJP_Fix->GetXaxis()->SetTitleFont(42);
   Graph_from_hBFractionDataLTJP_Fix->GetYaxis()->SetNdivisions(408);
   Graph_from_hBFractionDataLTJP_Fix->GetYaxis()->SetLabelFont(42);
   Graph_from_hBFractionDataLTJP_Fix->GetYaxis()->SetLabelOffset(0.007);
   Graph_from_hBFractionDataLTJP_Fix->GetYaxis()->SetLabelSize(0.05);
   Graph_from_hBFractionDataLTJP_Fix->GetYaxis()->SetTitleSize(0.05);
   Graph_from_hBFractionDataLTJP_Fix->GetYaxis()->SetTitleOffset(1.5);
   Graph_from_hBFractionDataLTJP_Fix->GetYaxis()->SetTitleFont(42);
   Graph_from_hBFractionDataLTJP_Fix->GetZaxis()->SetLabelFont(42);
   Graph_from_hBFractionDataLTJP_Fix->GetZaxis()->SetLabelOffset(0.007);
   Graph_from_hBFractionDataLTJP_Fix->GetZaxis()->SetLabelSize(0.05);
   Graph_from_hBFractionDataLTJP_Fix->GetZaxis()->SetTitleSize(0.06);
   Graph_from_hBFractionDataLTJP_Fix->GetZaxis()->SetTitleFont(42);
   grae->SetHistogram(Graph_from_hBFractionDataLTJP_Fix);
   
   grae->Draw("p,e1,");
   
   TLegend *leg = new TLegend(0.3,0.15,0.8,0.34,NULL,"brNDC");
   leg->SetTextFont(43);
   leg->SetTextSize(23);
   leg->SetBorderSize(0);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(19);
   leg->SetFillStyle(0);
   TLegendEntry *entry=leg->AddEntry("NULL","#int L dt = 231 nb^{-1}","h");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   //entry->SetTextFont(42);
   entry=leg->AddEntry("Graph_from_hBFractionDataLTJP_SSVHEat2.0_CLshift0","pp data","p");
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(8);
   entry->SetMarkerSize(1.5);
   entry=leg->AddEntry("hBFractionMC2","PYTHIA","l");
   entry->SetLineColor(2);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   entry=leg->AddEntry("gSyst","Sys. uncertainty","f");
   entry->SetFillColor(5);
   entry->SetFillStyle(1001);
   entry->SetLineColor(1);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(21);
   entry->SetMarkerSize(1);
   leg->Draw();
   Double_t xAxis3[4] = {80, 100, 120, 200}; 
   
   TH1D *hBFractionMC2__2 = new TH1D("hBFractionMC2__2","hBFractionMC",3, xAxis3);
   hBFractionMC2__2->SetBinContent(1,0.02846329);
   hBFractionMC2__2->SetBinContent(2,0.02658122);
   hBFractionMC2__2->SetBinContent(3,0.02463515);
   hBFractionMC2__2->SetBinError(1,0.0002591295);
   hBFractionMC2__2->SetBinError(2,0.0002592612);
   hBFractionMC2__2->SetBinError(3,0.0003185524);
   hBFractionMC2__2->SetMinimum(0);
   hBFractionMC2__2->SetMaximum(0.06);
   hBFractionMC2__2->SetEntries(6);
   hBFractionMC2__2->SetDirectory(0);
   hBFractionMC2__2->SetLineColor(2);
   hBFractionMC2__2->SetLineStyle(0);
   hBFractionMC2__2->SetLineWidth(2);
   hBFractionMC2__2->SetMarkerColor(2);
   hBFractionMC2__2->SetMarkerStyle(1);
   hBFractionMC2__2->SetMarkerSize(0);
   hBFractionMC2__2->GetXaxis()->SetTitle("Jet p_{T} (GeV/c)");
   hBFractionMC2__2->GetXaxis()->CenterTitle(true);
   hBFractionMC2__2->GetXaxis()->SetLabelFont(42);
   hBFractionMC2__2->GetXaxis()->SetLabelOffset(0.007);
   hBFractionMC2__2->GetXaxis()->SetLabelSize(0.05);
   hBFractionMC2__2->GetXaxis()->SetTitleSize(0.05);
   hBFractionMC2__2->GetXaxis()->SetTitleFont(42);
   hBFractionMC2__2->GetYaxis()->SetTitle("b-jet fraction");
   hBFractionMC2__2->GetYaxis()->CenterTitle(true);
   hBFractionMC2__2->GetYaxis()->SetNdivisions(408);
   hBFractionMC2__2->GetYaxis()->SetLabelFont(42);
   hBFractionMC2__2->GetYaxis()->SetLabelOffset(0.007);
   hBFractionMC2__2->GetYaxis()->SetLabelSize(0.05);
   hBFractionMC2__2->GetYaxis()->SetTitleSize(0.05);
   hBFractionMC2__2->GetYaxis()->SetTitleOffset(1.5);
   hBFractionMC2__2->GetYaxis()->SetTitleFont(42);
   hBFractionMC2__2->GetZaxis()->SetLabelFont(42);
   hBFractionMC2__2->GetZaxis()->SetLabelOffset(0.007);
   hBFractionMC2__2->GetZaxis()->SetLabelSize(0.05);
   hBFractionMC2__2->GetZaxis()->SetTitleSize(0.06);
   hBFractionMC2__2->GetZaxis()->SetTitleFont(42);
   hBFractionMC2__2->Draw("sameaxis");
   cBFraction2->Modified();
   cBFraction2->cd();
   cBFraction2->SetSelected(cBFraction2);
}
