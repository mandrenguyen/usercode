#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TNtuple.h"
#include "TF1.h"

// Hacks to get the jet energy scale uncertainty
#include "CondFormats/JetMETObjects/src/Utilities.cc"
#include "CondFormats/JetMETObjects/src/JetCorrectorParameters.cc"
#include "CondFormats/JetMETObjects/src/SimpleJetCorrector.cc"
#include "CondFormats/JetMETObjects/src/FactorizedJetCorrector.cc"

#include "CondFormats/JetMETObjects/src/SimpleJetCorrectionUncertainty.cc"
#include "CondFormats/JetMETObjects/src/JetCorrectionUncertainty.cc"

//void analyzeJetTrees(char *infile="merged_jetTree_v5.root",char *outfile="histos_Data_v5_010_v2.root",int isMC=0, int useWeight=0, int central = 1)
int analyzeJetTrees(char *infile="merged_jetTree_Pyquen.root",char *outfile="histos_Pyquen.root",int isMC=1, int useWeight=0, int central = 0, int useRawPt = 0)
{
  // central =0, all centralities or p+p
  //central =1, 0-10
  //central =2, 10-30
  // central =3, 30-90

  // Jet energy correction
  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty("CondFormats/JetMETObjects/data/Spring10_Uncertainty_AK5Calo.txt");
  
  // Scale factor for fRes
  TF1 *fRes = new TF1("fRes","TMath::Gaus(x,[0],[1])");
  fRes->SetParameters(1,0.1);
  fRes->SetRange(0,2);

  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(infile);
   if (!f) {
      f = new TFile(infile);
      f->cd("inclusiveJetAnalyzer");
   }
   TTree *t;
   if(isMC) t= (TTree*)gDirectory->Get("t");  // Old hydjet file
   else t = (TTree*)gDirectory->Get("icPu5patJets_tree");

   if (t==0) {
      cout <<"Tree 't' is not found! Use icPu5patJets_tree instead..."<<endl;
      t = (TTree*)gDirectory->Get("icPu5patJets_tree");
      if (t==0) return 0;
   }
   

//Declaration of leaves types
   Int_t           run;
   Int_t           evt;
   Float_t         b;
   Float_t         hf;
   UChar_t         nref;
   Int_t           bin;
   Float_t         jtpt[99];
   Float_t         jteta[99];
   Float_t         jty[99];
   Float_t         jtphi[99];
   Float_t         refpt[99];
   Float_t         refeta[99];
   Float_t         refy[99];
   Float_t         refphi[99];
   Float_t         refdrjt[99];

   // Set branch addresses.
   t->SetBranchAddress("run",&run);
   t->SetBranchAddress("evt",&evt);
   t->SetBranchAddress("b",&b);
   t->SetBranchAddress("hf",&hf);
   t->SetBranchAddress("nref",&nref);
   t->SetBranchAddress("bin",&bin);

   if (useRawPt) {
      t->SetBranchAddress("rawpt",jtpt);
   } else {
      t->SetBranchAddress("jtpt",jtpt);
   }

   t->SetBranchAddress("jteta",jteta);
   t->SetBranchAddress("jty",jty);
   t->SetBranchAddress("jtphi",jtphi);
   
   if(isMC){
     t->SetBranchAddress("refpt",refpt);
     t->SetBranchAddress("refeta",refeta);
     t->SetBranchAddress("refy",refy);
     t->SetBranchAddress("refphi",refphi);
     t->SetBranchAddress("refdrjt",refdrjt);
   }

//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// t->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   // create output file 
   TFile *fout=new TFile(outfile,"RECREATE");
   // add a small ntuple for light analysis
   TNtuple *nt = new TNtuple("nt","","et1:unc1:et2:unc2:bin:dphi:weight:fRes");


   float pi= acos(-1.);

   TH1F* hLeadingJetPt = new TH1F("hLeadingJetPt","Leading Jet p_{T}",500,0,500);
   TH3F* hPt1_Pt2_Dphi = new TH3F("hPt1_Pt2_Dphi","Leading Jet p_{T} vs Sub-Leading Jet p_{T} vs #Delta#phi",100,0,500,100,0,500,20,0.,pi);
   TH3F* hPt1_Pt2_Phi2 = new TH3F("hPt1_Pt2_Phi2","Leading Jet p_{T} vs Sub-Leading Jet p_{T} vs Sub-Leading #phi",100,0,500,100,0,500,40,-acos(-1.),acos(-1.));
   TH3F* hPt1_Pt2_Eta2 = new TH3F("hPt1_Pt2_Eta2","Leading Jet p_{T} vs Sub-Leading Jet p_{T} vs Sub-Leading #eta",100,0,500,100,0,500,40,-2.,2);
   TH3F* hLeading_Pt_Eta_Phi = new TH3F("hLeading_Pt_Eta_Phi","Leading Jet p_{T} vs #eta vs #phi",100,0,500,20,-2.,2.,20,-pi,pi);
   TH3F* hSubLeading_Pt_Eta_Phi = new TH3F("hSubLeading_Pt_Eta_Phi","SubLeading Jet p_{T} vs #eta vs #phi",100,0,500,20,-2.,2.,20,-pi,pi);


   TH1F* hGenLeadingJetPt = new TH1F("hGenLeadingJetPt","Leading Jet p_{T}",500,0,500);
   TH3F* hGen_Pt1_Pt2_Dphi = new TH3F("hGen_Pt1_Pt2_Dphi","Leading Jet p_{T} vs Sub-Leading Jet p_{T} vs #Delta#phi",100,0,500,100,0,500,20,0.,pi);
   TH3F* hGenLeading_Pt_Eta_Phi = new TH3F("hGenLeading_Pt_Eta_Phi","Leading Jet p_{T} vs #eta vs #phi",100,0,500,20,-2.,2.,20,-pi,pi);
   TH3F* hGenSubLeading_Pt_Eta_Phi = new TH3F("hGenSubLeading_Pt_Eta_Phi","SubLeading Jet p_{T} vs #eta vs #phi",100,0,500,20,-2.,2.,20,-pi,pi);

   TH2F *hLeadingResponseVsPt = new TH2F("hLeadingResponseVsPt","hLeadingResponseVsPt",100,0.,5.0,100,0,500);
   TH2F *hSubLeadingResponseVsPt = new TH2F("hSubLeadingResponseVsPt","hSubLeadingResponseVsPt",100,0.,5.0,100,0,500);
   TH2F *hLeadingResolutionVsPt = new TH2F("hLeadingResolutionVsPt","hLeadingResolutionVsPt",100,-5.,5.,100,0,500);
   TH2F *hSubLeadingResolutionVsPt = new TH2F("hSubLeadingResolutionVsPt","hSubLeadingResolutionVsPt",100,-5.,5.,100,0,500);

   TFile *fcent_Data = new TFile("CentDist_Data_v5.root");
   TH1F *hCent_Data = (TH1F*)fcent_Data->Get("h");
   float cent_integral_Data = 1.;
   if(central==1)cent_integral_Data=hCent_Data->Integral(1,4);
   if(central==2)cent_integral_Data=hCent_Data->Integral(5,12);
   if(central==3)cent_integral_Data=hCent_Data->Integral(13,36);


   Long64_t nentries = t->GetEntries();

   Long64_t nbytes = 0;
   for (Long64_t i=0; i<nentries;i++) {
      nbytes += t->GetEntry(i);


      int max_jet_index = -1;
      int sub_jet_index = -1;
      float max_jet_pt = 0.;
      float sub_jet_pt = 0.;
      float max_jet_phi = 0.;
      float sub_jet_phi = 0.;
      float max_jet_eta = 0.;
      float sub_jet_eta = 0.;

      //if(bin<10) continue;
      if(central==1){
	if(bin>3) continue;
      }
      if(central==2){
	if(bin<4||bin>11) continue;
      }
      if(central==3){
	if(bin<12||bin>35) continue;
      }


      float weight = 1.;
      
      if(useWeight)
	{
	  if(cent_integral_Data!=0)weight = hCent_Data->GetBinContent(bin+1)/cent_integral_Data;
	  
	}
  

      if(fabs(jteta[0])>2) continue;
      
      max_jet_index = 0;
      max_jet_pt = jtpt[0];
      max_jet_phi = jtphi[0];
      max_jet_eta = jteta[0];
      
      for(int ijet=1;ijet<nref;ijet++){
	
        if(fabs(jteta[ijet]) < 2 // this is the value that has been changed from 2 -> 3
           && jtpt[ijet]> sub_jet_pt)
          {
            sub_jet_index = ijet;
            sub_jet_pt = jtpt[ijet];
            sub_jet_phi = jtphi[ijet];
            sub_jet_eta = jteta[ijet];
          }
	
      }

 
      

      float dphi = max_jet_phi - sub_jet_phi;
      if(dphi<0) dphi+=2*pi;
      if(dphi>2*pi) dphi-=2*pi;
      if(dphi>pi) dphi = 2*pi - dphi;

      hLeadingJetPt->Fill(max_jet_pt,weight);
      hPt1_Pt2_Dphi->Fill(max_jet_pt,sub_jet_pt,dphi,weight);
      //cout<<" sub_jet_phi "<<sub_jet_phi<<endl;
      hPt1_Pt2_Phi2->Fill(max_jet_pt,sub_jet_pt,sub_jet_phi,weight);
      hPt1_Pt2_Eta2->Fill(max_jet_pt,sub_jet_pt,sub_jet_eta,weight);
      hLeading_Pt_Eta_Phi->Fill(max_jet_pt,max_jet_eta,max_jet_phi,weight);
      hSubLeading_Pt_Eta_Phi->Fill(sub_jet_pt,sub_jet_eta,sub_jet_phi,weight);

      // Calculate jet energy scale uncertainty 
      jecUnc->setJetEta(max_jet_eta);
      jecUnc->setJetPt(max_jet_pt);
      double max_unc = jecUnc->getUncertainty(true);

      jecUnc->setJetEta(sub_jet_eta);
      jecUnc->setJetPt(sub_jet_pt);
      double sub_unc = jecUnc->getUncertainty(true);

      // Fill mini-ntuple
      nt->Fill(max_jet_pt,max_unc,sub_jet_pt,sub_unc,bin,dphi,weight,fRes->GetRandom());

      if(isMC){

	float max_ref_pt = refpt[max_jet_index];
	float max_ref_eta = refeta[max_jet_index];
	float max_ref_phi = refphi[max_jet_index];

	float sub_ref_pt = refpt[sub_jet_index];
	float sub_ref_eta = refeta[sub_jet_index];
	float sub_ref_phi = refphi[sub_jet_index];


	float gen_dphi = max_ref_phi - sub_ref_phi;
	if(gen_dphi<0) gen_dphi+=2*pi;
	if(gen_dphi>2*pi) gen_dphi-=2*pi;
	if(gen_dphi>pi) gen_dphi = 2*pi - gen_dphi;

	hGenLeadingJetPt->Fill(max_ref_pt,weight);
	hGen_Pt1_Pt2_Dphi->Fill(max_ref_pt,sub_ref_pt,gen_dphi,weight);
	hGenLeading_Pt_Eta_Phi->Fill(max_ref_pt,max_ref_eta,max_ref_phi,weight);
	hGenSubLeading_Pt_Eta_Phi->Fill(sub_ref_pt,sub_ref_eta,sub_ref_phi,weight);
	

	hLeadingResponseVsPt->Fill(max_jet_pt/max_ref_pt,max_ref_pt,weight);
	if(sub_jet_index>-1)hSubLeadingResponseVsPt->Fill(sub_jet_pt/sub_ref_pt,sub_ref_pt,weight);
	if(max_ref_pt>0)hLeadingResolutionVsPt->Fill((max_jet_pt-max_ref_pt)/max_ref_pt,max_ref_pt,weight);
	if(sub_jet_index>-1 && sub_ref_pt>0)hSubLeadingResolutionVsPt->Fill((sub_jet_pt-sub_ref_pt)/sub_ref_pt,sub_ref_pt,weight);

      }

   }

   // Write to output file.
   fout->cd();
   nt->Write();

   hLeadingJetPt->Write();
   hPt1_Pt2_Dphi->Write();
   hPt1_Pt2_Phi2->Write();
   hPt1_Pt2_Eta2->Write();
   hLeading_Pt_Eta_Phi->Write();
   hSubLeading_Pt_Eta_Phi->Write();

   hGenLeadingJetPt->Write();
   hGen_Pt1_Pt2_Dphi->Write();
   hGenLeading_Pt_Eta_Phi->Write();
   hGenSubLeading_Pt_Eta_Phi->Write();

   hLeadingResponseVsPt->Write();
   hSubLeadingResponseVsPt->Write();
   hLeadingResolutionVsPt->Write();
   hSubLeadingResolutionVsPt->Write();


   fout->Close();
   return 1;
}






