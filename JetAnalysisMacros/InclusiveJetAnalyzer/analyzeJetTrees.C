#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TTree.h"
#include "TROOT.h"
#include "TNtuple.h"
#include "TRandom.h"
#include "TF1.h"
#include "TTimeStamp.h"

// Hacks to get the jet energy scale uncertainty
#include "CondFormats/JetMETObjects/src/Utilities.cc"
#include "CondFormats/JetMETObjects/src/JetCorrectorParameters.cc"
#include "CondFormats/JetMETObjects/src/SimpleJetCorrector.cc"
#include "CondFormats/JetMETObjects/src/FactorizedJetCorrector.cc"

#include "CondFormats/JetMETObjects/src/SimpleJetCorrectionUncertainty.cc"
#include "CondFormats/JetMETObjects/src/JetCorrectionUncertainty.cc"


// Note that centrality flag is somewhat deprecated, just use central = 0 to make the tree and make cuts in the plotting macros
int analyzeJetTrees(int data_pythia_mix=2, int central = 0, int useRawPt = 0, int useAK5PF=0, int correctAwaySideJet = 0, int smearEnergy = 0, int simulateIneff = 0){
  
  string infile;
  string outfile;
  int useWeight = 0;
  int isMC = 0;

  if(data_pythia_mix==0){
    infile = "/castor/cern.ch/user/m/mnguyen//HIDATA/JetTrees/merged_jetTree_Data_v13.root";
    if(useAK5PF) outfile = "data_AK5PF.root";
    else outfile  = "data.root";
  }
  if(data_pythia_mix==1){
    infile = "/castor/cern.ch/user/m/mnguyen//HIDATA/JetTrees/merged_jetTree_Pyquen_pt80_v4.root";
    if(useAK5PF) outfile = "pythia_AK5PF.root";
    else outfile  = "pythia.root";
    isMC=1;
  }
  if(data_pythia_mix==2){
    //infile = "/afs/cern.ch/user/e/edwenger/public/dijets/dijetTreeDM_pt80.root";
     infile = "/castor/cern.ch/user/m/mnguyen//HIDATA/JetTrees//merged_jetTree_Pyquen_DataEmbedded_pt80_v2.root";
    if(useAK5PF) outfile = "mix_AK5PF.root";
    else outfile  = "mix.root";
    useWeight = 1;
    isMC=1;
  }
    
  cout<<" set defaults "<<endl;
  // central =0, all centralities or p+p
  //central =1, 0-10
  //central =2, 10-30
  // central =3, 30-90

  // Set Random Seed
  TTimeStamp myTime;
  gRandom->SetSeed(myTime.GetNanoSec());
  
  // Jet energy correction
  JetCorrectionUncertainty *jecUnc = new JetCorrectionUncertainty("CondFormats/JetMETObjects/data/Spring10_Uncertainty_AK5Calo.txt");
  
  // Scale factor for fRes
  TF1 *fRes = new TF1("fRes","TMath::Gaus(x,[0],[1])");
  fRes->SetParameters(1,0.1);
  fRes->SetRange(0,2);

  TF1 *fSmear = new TF1("fRes","TMath::Gaus(x,[0],[1])");
  fSmear->SetParameters(0,7);
  fSmear->SetRange(-35,35);

  cout<< "opening file: "<<infile<<endl;
  
  // Take the tree from the file
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(infile.c_str());

  char *dirname=NULL;
  //  if(data_pythia_mix==0 || data_pythia_mix==2){
  if(data_pythia_mix==0){
    if(useAK5PF)dirname = "ak5PFJetAnalyzer";
    else dirname = "inclusiveJetAnalyzer";
  }
  else{
    if(useAK5PF)dirname = "ak5PFJetAnalyzer_ppMC";
    else dirname = "inclusiveJetAnalyzer_ppMC";
  }
  cout<<" looking in directory: "<<dirname<<endl;

  if (!f) {
    f =  TFile::Open(infile.c_str());
    f->cd(dirname);
  }

  char *treename=NULL;
  if(useAK5PF)treename = "ak5PFpatJets_tree";
  else treename = "icPu5patJets_tree";

  TTree *t = (TTree*)gDirectory->Get(treename);
  
    /*  // legacy
   if (t==0) {
   cout <<"Tree 't' is not found! Use icPu5patJets_tree instead..."<<endl;
   //t = (TTree*)gDirectory->Get("icPu5patJets_tree");
   t = (TTree*)gDirectory->Get("ak5PFpatJets_tree");
   if (t==0) return 0;
   }
    */
   

//Declaration of leaves types
   Int_t           run;
   Int_t           evt;
   Int_t           lumi;
   Float_t         b;
   Float_t         hf;
   Int_t           nref;
   Int_t           ngen;
   Int_t           bin;
   Float_t         jtpt[99];
   Float_t         jteta[99];
   Float_t         jty[99];
   Float_t         jtphi[99];
   Float_t         genpt[99];
   Float_t         geneta[99];
   Float_t         geny[99];
   Float_t         genphi[99];
   Float_t         gendrjt[99];

   // Set branch addresses.
   t->SetBranchAddress("run",&run);
   t->SetBranchAddress("lumi",&lumi);
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
   
   /*
   if(isMC){
     t->SetBranchAddress("ngen",&ngen);
     t->SetBranchAddress("genpt",genpt);
     t->SetBranchAddress("geneta",geneta);
     t->SetBranchAddress("geny",geny);
     t->SetBranchAddress("genphi",genphi);
     t->SetBranchAddress("gendrjt",gendrjt);
   }
   */
//     This is the loop skeleton
//       To read only selected branches, Insert statements like:
// t->SetBranchStatus("*",0);  // disable all branches
// TTreePlayer->SetBranchStatus("branchname",1);  // activate branchname

   // create output file 
   TFile *fout=new TFile(outfile.c_str(),"RECREATE");
   // add a small ntuple for light analysis
   TNtuple *nt = new TNtuple("nt","","et1:eta1:phi1:unc1:et2:eta2:phi2:unc2:et3:eta3:phi3:bin:dphi:dphi2:weight:fRes:run");


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

   TFile *fcent_Data = new TFile("CentDist_Data_v13.root");
   TH1F *hCent_Data = (TH1F*)fcent_Data->Get("h");
   float cent_integral_Data = 1.;
   if(central==1)cent_integral_Data=hCent_Data->Integral(1,4);
   if(central==2)cent_integral_Data=hCent_Data->Integral(5,12);
   if(central==3)cent_integral_Data=hCent_Data->Integral(13,36);


   Long64_t nentries = t->GetEntries();

   Long64_t nbytes = 0;
   int count =0;
   for (Long64_t i=0; i<nentries;i++) {
      if (i% 10000 == 0) cout <<run<<" "<<i<<" "<<count<<endl;
      nbytes += t->GetEntry(i);
      if (nref==0) continue;

      //Marguerite's HCAL filterering identified these events, but too late for a repass so remove by hand
      if(data_pythia_mix==0){
        if(run== 151211 && evt == 555881 && lumi ==  103 ) continue;
	if(run== 151238 && evt == 581194 && lumi ==  105 ) continue;
	if(run== 151238 && evt == 1299690 && lumi ==  242) continue;
	if(run== 151352 && evt == 55195 && lumi ==  10	 ) continue;
	if(run== 151878 && evt == 594808 && lumi ==  135 ) continue;
	if(run== 152047 && evt == 666877 && lumi ==  114 ) continue;
	if(run== 152112 && evt == 2207843 && lumi ==  426) continue;
	if(run== 152112 && evt == 3151220 && lumi ==  608) continue;
	if(run== 152349 && evt == 939393 && lumi ==  220 ) continue;
	if(run== 152350 && evt == 595632 && lumi ==  111 ) continue;
	if(run== 152350 && evt == 2482917 && lumi ==  472) continue;
	if(run== 152350 && evt == 2686548 && lumi ==  512) continue;
	if(run== 152474 && evt == 2085185 && lumi ==  403) continue;
	if(run== 152477 && evt == 1890056 && lumi ==  392) continue;
	if(run== 152485 && evt == 55917 && lumi ==  12	 ) continue;
	if(run== 152561 && evt == 3406888 && lumi ==  606) continue;
	if(run== 152561 && evt == 3758331 && lumi ==  670) continue;
	if(run== 152561 && evt == 4478132 && lumi ==  803) continue;
	if(run== 152561 && evt == 4797830 && lumi ==  863) continue;
	if(run== 152561 && evt == 5176016 && lumi ==  936) continue;
	if(run== 152592 && evt == 22234 && lumi ==  4	 ) continue;
	if(run== 152592 && evt == 402212 && lumi ==  66	 ) continue;
	if(run== 152594 && evt == 587793 && lumi ==  110 ) continue;
	if(run== 152601 && evt == 1417393 && lumi ==  267) continue;
	if(run== 152602 && evt == 686565 && lumi ==  111 ) continue;
	if(run== 152602 && evt == 3940942 && lumi ==  684) continue;
	if(run== 152624 && evt == 995626 && lumi ==  170 ) continue;
	if(run== 152624 && evt == 1063452 && lumi ==  182) continue;
	if(run== 152624 && evt == 1250655 && lumi ==  215) continue;
	if(run== 152624 && evt == 1846646 && lumi ==  322) continue;
	if(run== 152625 && evt == 1634959 && lumi ==  282) continue;
	if(run== 152625 && evt == 3162245 && lumi ==  563) continue;
	if(run== 152641 && evt == 173478 && lumi ==  31	 ) continue;
	if(run== 152642 && evt == 359181 && lumi ==  57	 ) continue;
	if(run== 152642 && evt == 1764595 && lumi ==  286) continue;
	if(run== 152642 && evt == 2686223 && lumi ==  446) continue;
	if(run== 152721 && evt == 1304320 && lumi ==  249) continue;
	if(run== 152721 && evt == 1983505 && lumi ==  358) continue;
	if(run== 152722 && evt == 2963949 && lumi ==  485) continue;
	if(run== 152741 && evt == 558533 && lumi ==  91	 ) continue;
	if(run== 152751 && evt == 3432123 && lumi ==  582) continue;
	if(run== 152791 && evt == 246343 && lumi ==  39  ) continue; 
      }

      int max_jet_index = -1;
      int sub_jet_index = -1;
      int third_jet_index = -1;
      float max_jet_pt = 0.;
      float sub_jet_pt = 0.;
      float third_jet_pt = 0.;
      float max_jet_phi = 0.;
      float sub_jet_phi = 0.;
      float third_jet_phi = 0.;
      float max_jet_eta = 0.;
      float sub_jet_eta = 0.;
      float third_jet_eta = 0.;

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
  
      // if the leading jet has eta > 2 --> throw it away
      if(fabs(jteta[0])>2) continue;
      
      max_jet_index = 0;
      max_jet_pt = jtpt[0];
      if (smearEnergy) max_jet_pt+=fSmear->GetRandom();
      max_jet_phi = jtphi[0];
      max_jet_eta = jteta[0];
      
      for(int ijet=1;ijet<nref;ijet++){
        double candPt = jtpt[ijet];
	if (correctAwaySideJet) {
	   //double cor=(1-0.221219/candPt-427.546/candPt/candPt);
	   //double cor=0.652738+0.00408465*candPt-(9.95783e-6)*candPt*candPt;
	   //double cor= (-13.02)/(candPt+0.2071)-87.64/candPt/candPt+1.151;
	   //double cor= (-3.013)/(candPt+89.01)-75.44/candPt/candPt+1.071;
	   double cor= 1.3895 -0.0161403*candPt + 0.000186386 *candPt * candPt -(6.4896e-7)*candPt*candPt*candPt;
	   if (candPt>140) cor = 1;
	   cout <<cor<<endl;
	   candPt/=cor;
	} 
        if (smearEnergy) candPt+=fSmear->GetRandom();
        if (simulateIneff) {
           if (candPt<60&&gRandom->Rndm()<0.1) {
              candPt=0;
           }
        }	
        if(fabs(jteta[ijet]) < 2 // this is the value that has been changed from 2 -> 3
           && candPt > sub_jet_pt)
          {
	    third_jet_index = sub_jet_index;
	    third_jet_pt = sub_jet_pt;
	    third_jet_phi = sub_jet_phi;
	    third_jet_eta = sub_jet_eta;

            sub_jet_index = ijet;
            sub_jet_pt = candPt;
            sub_jet_phi = jtphi[ijet];
            sub_jet_eta = jteta[ijet];
          } else if (fabs(jteta[ijet]) < 2 // this is the value that has been changed from 2 -> 3
           && candPt > third_jet_pt)
          {
            third_jet_index = ijet;
            third_jet_pt = candPt;
            third_jet_phi = jtphi[ijet];
            third_jet_eta = jteta[ijet];
          }
      }

      float dphi = max_jet_phi - sub_jet_phi;
      if(dphi<0) dphi+=2*pi;
      if(dphi>2*pi) dphi-=2*pi;
      if(dphi>pi) dphi = 2*pi - dphi;

      float dphi2 = max_jet_phi - third_jet_phi;
      if(dphi2<0) dphi2+=2*pi;
      if(dphi2>2*pi) dphi2-=2*pi;
      if(dphi2>pi) dphi2 = 2*pi - dphi2;

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
      Float_t var[100];
      var[0]=max_jet_pt;
      var[1]=max_jet_eta;
      var[2]=max_jet_phi;
      var[3]=max_unc;
      var[4]=sub_jet_pt;
      var[5]=sub_jet_eta;
      var[6]=sub_jet_phi;
      var[7]=sub_unc;
      var[8]=third_jet_pt;
      var[9]=third_jet_eta;
      var[10]=third_jet_phi;
      var[11]=bin;
      
      var[12]=dphi;
      var[13]=dphi2;
      var[14]=weight;
      var[15]=fRes->GetRandom();
      var[16]=run;
      nt->Fill(var);
      count++;
      /*  // This would need to be updated to use gen info
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
      */
   }

   // Write to output file.
   fout->cd();
   nt->SetAlias("aj","(et1-et2)/(et1+et2)");
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

   cout <<"Di-jet: "<<count<<" "<<nentries<<endl;
   cout <<t->GetEntries("abs(jteta[0])<2");
   fout->Close();
   return 1;
}






