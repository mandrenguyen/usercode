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
#include "TChain.h"



// Note that centrality flag is somewhat deprecated, just use central = 0 to make the tree and make cuts in the plotting macros
int analyzeJetTrees(int data_pythia_mix_pp=2, int central = 0, int useRawPt = 0, int jetType=3, int correctAwaySideJet = 0, int smearEnergy = 0, int simulateIneff = 0){
  
  string infile;
  int isMC = 0;


  char outfile[500];

  if(data_pythia_mix_pp==0){
    infile="~/scratch/InclusiveJetAnalyzer/310X/HIData_Jet35U/hiGoodTightTracks_extraTrackInfo/merged_JetAnalyisTTrees_hiGoodTightTracks_v1_EscaleRepass.root";
    if(jetType==0) sprintf(outfile,"data_icPu5.root");
    if(jetType==1) sprintf(outfile,"data_akPu5PF.root");
    if(jetType==2) sprintf(outfile,"data_akPu4PF.root");
    if(jetType==3) sprintf(outfile,"data_akPu3PF.root");
  }
  if(data_pythia_mix_pp==1){
    infile = "~/scratch/InclusiveJetAnalyzer/413patch2/Pyquen_UnquenchedDiJet_Pt80_START39V7HI_GEN_SIM_RAW_RECO_393_v1/DefaultCorr_PFTowers_hiGoodTightTracks/merged_JetAnalysisTTrees_hiGoodTightTracks_v1_EscaleRepass_D6T_413_noCutoff_JetsOnly.root";
    if(jetType==0) sprintf(outfile,"pythia_icPu5.root");
    if(jetType==1) sprintf(outfile,"pythia_akPu5PF.root");
    if(jetType==2) sprintf(outfile,"pythia_akPu4PF.root");
    if(jetType==3) sprintf(outfile,"pythia_akPu3PF.root");
    isMC=1;
  }
  if(data_pythia_mix_pp==2){
    infile = "~mnguyen/scratch/InclusiveJetAnalyzer/310X/Pyquen_UnquenchedDiJet_Pt80_GEN-SIM-RECO_393_setX/HICorrJetTuples_PFTowers_hiGoodTightTracks/merged_JetAnalyisTTrees_hiGoodTightTracks_v1_EscaleRepass.root";
    if(jetType==0) sprintf(outfile,"mix_icPu5.root");
    if(jetType==1) sprintf(outfile,"mix_akPu5PF.root");
    if(jetType==2) sprintf(outfile,"mix_akPu4PF.root");
    if(jetType==3) sprintf(outfile,"mix_akPu3PF.root");
    isMC=1;
  }
  if(data_pythia_mix_pp==3){
    infile = "~/scratch/InclusiveJetAnalyzer/413patch2/AllPhysics2760/DefaultCorrJetTuples_pp2760_HIReco_Jet_SD_hiGoodTightTracks/merged_JetAnalysisTTrees_hiGoodTightTracks_v1_EscaleRepass_D6T_413.root";
    if(jetType==0) sprintf(outfile,"pp_icPu5.root");
    if(jetType==1) sprintf(outfile,"pp_akPu5PF.root");
    if(jetType==2) sprintf(outfile,"pp_akPu4PF.root");
    if(jetType==3) sprintf(outfile,"pp_akPu3PF.root");
  }

  // Set Random Seed
  TTimeStamp myTime;
  gRandom->SetSeed(myTime.GetNanoSec());
  
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

  
  if (!f) {
    f =  TFile::Open(infile.c_str());
  }

  char *treename = "t";
  //if(data_pythia_mix_pp==3) treename= "PFJetAnalyzer/t";
  //else treename= "t";

  TTree *t = (TTree*)gDirectory->Get(treename);

  
  
  
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
   //Float_t         jty[99];
   Float_t         jtphi[99];
   Float_t         genpt[99];
   Float_t         geneta[99];
   Float_t         geny[99];
   Float_t         genphi[99];
   Float_t         gendrjt[99];
   Float_t         xSecWeight;

   // Set branch addresses.
   if(data_pythia_mix_pp==0||data_pythia_mix_pp==3){
   t->SetBranchAddress("run",&run);
   t->SetBranchAddress("lumi",&lumi);
   t->SetBranchAddress("evt",&evt);
   }
   //t->SetBranchAddress("b",&b);
   //t->SetBranchAddress("hf",&hf);
   t->SetBranchAddress("bin",&bin);
   
   if(data_pythia_mix_pp==1||data_pythia_mix_pp==2)t->SetBranchAddress("weight",&xSecWeight);

   if(jetType==0){
     t->SetBranchAddress("nj1",&nref);
     if (useRawPt) t->SetBranchAddress("rawpt_j1",jtpt);
     else t->SetBranchAddress("corrpt_j1",jtpt);
     t->SetBranchAddress("jteta_j1",jteta);
     //t->SetBranchAddress("jty_j1",jty);
     t->SetBranchAddress("jtphi_j1",jtphi);
   }
   if(jetType==1){
     t->SetBranchAddress("nj2",&nref);
     if (useRawPt) t->SetBranchAddress("rawpt_j2",jtpt);
     else t->SetBranchAddress("corrpt_j2",jtpt);
     t->SetBranchAddress("jteta_j2",jteta);
     //t->SetBranchAddress("jty_j2",jty);
     t->SetBranchAddress("jtphi_j2",jtphi);
   }
   if(jetType==2){
     t->SetBranchAddress("nj3",&nref);
     if (useRawPt) t->SetBranchAddress("rawpt_j3",jtpt);
     else t->SetBranchAddress("corrpt_j3",jtpt);
     t->SetBranchAddress("jteta_j3",jteta);
     //t->SetBranchAddress("jty_j3",jty);
     t->SetBranchAddress("jtphi_j3",jtphi);
   }
   if(jetType==3){
     t->SetBranchAddress("nj4",&nref);
     if (useRawPt) t->SetBranchAddress("rawpt_j4",jtpt);
     else{
       if(data_pythia_mix_pp==3)t->SetBranchAddress("jtpt_j4",jtpt);
       else t->SetBranchAddress("corrpt_j4",jtpt);
     }
     t->SetBranchAddress("jteta_j4",jteta);
     //t->SetBranchAddress("jty_j4",jty);
     t->SetBranchAddress("jtphi_j4",jtphi);
   }
   

   // create output file 
   TFile *fout=new TFile(outfile,"RECREATE");
   // add a small ntuple for light analysis
   TNtuple *nt = new TNtuple("nt","","et1:eta1:phi1:unc1:et2:eta2:phi2:unc2:et3:eta3:phi3:bin:dphi:dphi2:weight:fRes:run");


   float pi= acos(-1.);

   TFile *fcent_Data = new TFile("CentDist_Data_v14.root");
   TH1F *hCent_Data = (TH1F*)fcent_Data->Get("h");
   float cent_integral_Data = 1.;
   if(central==1)cent_integral_Data=hCent_Data->Integral(1,4);
   if(central==2)cent_integral_Data=hCent_Data->Integral(5,12);
   if(central==3)cent_integral_Data=hCent_Data->Integral(13,36);


   Long64_t nentries = t->GetEntries();

   Long64_t nbytes = 0;
   int count =0;
   for (Long64_t i=0; i<nentries;i++) {

      nbytes += t->GetEntry(i);

     if (i% 10000 == 0) cout <<"run "<<run<<" "<<i<<" "<<count<<endl;
     //cout<<" jtpt[0] "<<jtpt[0]<<endl;
      if (nref==0) continue;

      //Marguerite's HCAL filterering identified these events, but too late for a repass so remove by hand
      if(data_pythia_mix_pp==0){
        if(run== 151211 && evt == 555881 && lumi ==  103 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 151238 && evt == 581194 && lumi ==  105 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 151238 && evt == 1299690 && lumi ==  242) {cout<<" removing noise event "<<endl; continue;}
	if(run== 151352 && evt == 55195 && lumi ==  10	 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 151878 && evt == 594808 && lumi ==  135 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152047 && evt == 666877 && lumi ==  114 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152112 && evt == 2207843 && lumi ==  426) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152112 && evt == 3151220 && lumi ==  608) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152349 && evt == 939393 && lumi ==  220 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152350 && evt == 595632 && lumi ==  111 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152350 && evt == 2482917 && lumi ==  472) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152350 && evt == 2686548 && lumi ==  512) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152474 && evt == 2085185 && lumi ==  403) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152477 && evt == 1890056 && lumi ==  392) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152485 && evt == 55917 && lumi ==  12	 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152561 && evt == 3406888 && lumi ==  606) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152561 && evt == 3758331 && lumi ==  670) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152561 && evt == 4478132 && lumi ==  803) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152561 && evt == 4797830 && lumi ==  863) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152561 && evt == 5176016 && lumi ==  936) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152592 && evt == 22234 && lumi ==  4	 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152592 && evt == 402212 && lumi ==  66	 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152594 && evt == 587793 && lumi ==  110 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152601 && evt == 1417393 && lumi ==  267) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152602 && evt == 686565 && lumi ==  111 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152602 && evt == 3940942 && lumi ==  684) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152624 && evt == 995626 && lumi ==  170 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152624 && evt == 1063452 && lumi ==  182) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152624 && evt == 1250655 && lumi ==  215) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152624 && evt == 1846646 && lumi ==  322) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152625 && evt == 1634959 && lumi ==  282) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152625 && evt == 3162245 && lumi ==  563) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152641 && evt == 173478 && lumi ==  31	 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152642 && evt == 359181 && lumi ==  57	 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152642 && evt == 1764595 && lumi ==  286) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152642 && evt == 2686223 && lumi ==  446) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152721 && evt == 1304320 && lumi ==  249) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152721 && evt == 1983505 && lumi ==  358) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152722 && evt == 2963949 && lumi ==  485) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152741 && evt == 558533 && lumi ==  91	 ) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152751 && evt == 3432123 && lumi ==  582) {cout<<" removing noise event "<<endl; continue;}
	if(run== 152791 && evt == 246343 && lumi ==  39  ) {cout<<" removing noise event "<<endl; continue;} 
	if(run==152722 && evt == 2963949 && lumi == 485  ) {cout<<" removing noise event "<<endl; continue;} 
	if(run==152721 && evt == 1304320 && lumi == 249) {cout<<" removing noise event "<<endl; continue;}
	if(run==152721 && evt == 1983505 && lumi == 358) {cout<<" removing noise event "<<endl; continue;}
	if(run==152652 && evt == 1807781 && lumi == 320) {cout<<" removing noise event "<<endl; continue;}
	if(run==152957 && evt == 5429107 && lumi == 912) {cout<<" removing noise event "<<endl; continue;}
	if(run==152957 && evt == 5308902 && lumi == 890) {cout<<" removing noise event "<<endl; continue;}
	if(run==152957 && evt == 4942453 && lumi == 823) {cout<<" removing noise event "<<endl; continue;}
	if(run==152957 && evt == 3416438 && lumi == 555) {cout<<" removing noise event "<<endl; continue;}
	if(run==152957 && evt == 2344086 && lumi == 376) {cout<<" removing noise event "<<endl; continue;}
	if(run==152957 && evt == 1512943 && lumi == 240) {cout<<" removing noise event "<<endl; continue;}
	if(run==152957 && evt == 934279 && lumi == 150 ) {cout<<" removing noise event "<<endl; continue;}
	if(run==152791 && evt == 562383 && lumi == 90	) {cout<<" removing noise event "<<endl; continue;}
	if(run==152791 && evt == 246343 && lumi == 39	) {cout<<" removing noise event "<<endl; continue;}
	if(run==152785 && evt == 1320640 && lumi == 237) {cout<<" removing noise event "<<endl; continue;}
	if(run==152766 && evt == 949850 && lumi == 156	) {cout<<" removing noise event "<<endl; continue;}
	if(run==152745 && evt == 1028981 && lumi == 169) {cout<<" removing noise event "<<endl; continue;} 
	  
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
      
      if(data_pythia_mix_pp==2||data_pythia_mix_pp==1)
	{
	  if(data_pythia_mix_pp==2&&cent_integral_Data!=0){
	    weight = hCent_Data->GetBinContent(bin+1)/cent_integral_Data;
	  }
	  weight *= xSecWeight;
	}
  
      // if the leading jet has eta > 2 --> throw it away
      //if(fabs(jteta[0])>2) continue;
      
      // find max 3 jets in event
      for(int ijet=0;ijet<nref;ijet++){
	if(jtpt[ijet]>max_jet_pt && abs(jteta[ijet])<2){
	  max_jet_index=ijet;
	  max_jet_pt= jtpt[ijet];
	  max_jet_phi= jtphi[ijet];
	  max_jet_eta= jteta[ijet];
	}
      }
      for(int ijet=0;ijet<nref;ijet++){
	if(jtpt[ijet]>sub_jet_pt && abs(jteta[ijet])<2 && ijet!=max_jet_index){
	  sub_jet_index=ijet;
	  sub_jet_pt= jtpt[ijet];
	  sub_jet_phi= jtphi[ijet];
	  sub_jet_eta= jteta[ijet];
	}
      }
      for(int ijet=0;ijet<nref;ijet++){
	if(jtpt[ijet]>third_jet_pt && abs(jteta[ijet])<2 && ijet!=max_jet_index && ijet!=sub_jet_index){
	  third_jet_index=ijet;
	  third_jet_pt= jtpt[ijet];
	  third_jet_phi= jtphi[ijet];
	  third_jet_eta= jteta[ijet];
	}
      }
      /*
      if (smearEnergy) max_jet_pt+=fSmear->GetRandom();
      
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
      */
      float dphi = max_jet_phi - sub_jet_phi;
      if(dphi<0) dphi+=2*pi;
      if(dphi>2*pi) dphi-=2*pi;
      if(dphi>pi) dphi = 2*pi - dphi;

      float dphi2 = max_jet_phi - third_jet_phi;
      if(dphi2<0) dphi2+=2*pi;
      if(dphi2>2*pi) dphi2-=2*pi;
      if(dphi2>pi) dphi2 = 2*pi - dphi2;


      // Calculate jet energy scale uncertainty 
      //jecUnc->setJetEta(max_jet_eta);
      //jecUnc->setJetPt(max_jet_pt);
      //double max_unc = jecUnc->getUncertainty(true);
      double max_unc = 0.;

      //jecUnc->setJetEta(sub_jet_eta);
      //jecUnc->setJetPt(sub_jet_pt);
      //double sub_unc = jecUnc->getUncertainty(true);
      double sub_unc = 0.;

      // Fill mini-ntuple
      Float_t var[100];

      //cout<<" max jet pt "<<max_jet_pt<<endl;
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

   }

   // Write to output file.
   fout->cd();
   nt->SetAlias("aj","(et1-et2)/(et1+et2)");
   nt->Write();
   /*
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
   */
   cout <<"Di-jet: "<<count<<" "<<nentries<<endl;
   cout <<t->GetEntries("abs(jteta_j4[0])<2");
   fout->Close();
   return 1;
}






