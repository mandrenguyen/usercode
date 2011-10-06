#include "TROOT.h"
#include <iostream>
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"
//#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"
//#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
//#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h" 


void weight_PtHatBins_Escale(char *infile="~/scratch/InclusiveJetAnalyzer/413patch2/Pyquen_UnquenchedDiJet_Pt15_START39V7HI_GEN_SIM_RAW_RECO_393_v1/Escale_PFTowers_hiGoodTightTracks/merged_JetAnalysisTTrees_hiGoodTightTracks_v1.root", char *outfile="test.root", float maxpthat=30., float xSection=1., int isMC=1)
{

  //Reset ROOT and connect tree file
  gROOT->Reset();
  TFile *fin = new TFile(infile);
  
  cout<<outfile<<endl;
  
  string inputDirName[7]= {
    "inclusiveJetAnalyzer",
    "ak3PFJetAnalyzer",
    "ak4PFJetAnalyzer",
    "ak5PFJetAnalyzer",
    "ak3JetAnalyzer",
    "ak4JetAnalyzer",
    "ak5JetAnalyzer"
  };
  

   string outputDirName[7]= {
    "ic5calo",
    "ak3pf",
    "ak4pf",
    "ak5pf",
    "ak3calo",
    "ak4calo",
    "ak5calo"
   };


   TTree *tr_in[7], *tr_out[7];
   TFile *fout=new TFile(outfile,"RECREATE");


   for(int idir=0;idir<7;idir++)
     {
       
       
       fin->cd(inputDirName[idir].c_str());
       
       tr_in[idir] = (TTree*)gDirectory->Get("t");
       
       
       //Declaration of leaves types
       Int_t           run;
       Int_t           evt;
       Int_t           lumi;
       Float_t         b;
       Float_t         vx;
       Float_t         vy;
       Float_t         vz;
       Float_t         hf;
       Int_t           nref;
       Int_t           bin;
       Float_t         rawpt[999];
       Float_t         jtpt[999];
       Float_t         corrpt[999];
       Float_t         jteta[999];
       Float_t         jty[999];
       Float_t         jtphi[999];
       Float_t         pthat;
       Float_t         refpt[999];
       Float_t         refeta[999];
       Float_t         refy[999];
       Float_t         refphi[999];
       Float_t         refdphijt[999];
       Float_t         refdrjt[999];
       Float_t         refparton_pt[999];
       Float_t         refparton_flavor[999];
       /*
       Int_t           ngen;
       Int_t           genmatchindex[999];
       Float_t         genpt[999];
       Float_t         geneta[999];
       Float_t         geny[999];
       Float_t         genphi[999];
       Float_t         gendphijt[999];
       Float_t         gendrjt[999];
       */
       float fentries = (float)tr_in[idir]->GetEntries();
       float weight = xSection/(fentries/1000.);
       cout<<" weight "<<weight<<endl;
       
       // Set branch addresses.
       if(!isMC){
	 tr_in[idir]->SetBranchAddress("run",&run);
	 tr_in[idir]->SetBranchAddress("evt",&evt);
	 tr_in[idir]->SetBranchAddress("lumi",&lumi);
       }
       tr_in[idir]->SetBranchAddress("b",&b);
       tr_in[idir]->SetBranchAddress("vx",&vx);
       tr_in[idir]->SetBranchAddress("vy",&vy);
       tr_in[idir]->SetBranchAddress("vz",&vz);
       tr_in[idir]->SetBranchAddress("hf",&hf);
       tr_in[idir]->SetBranchAddress("nref",&nref);
       tr_in[idir]->SetBranchAddress("bin",&bin);
       tr_in[idir]->SetBranchAddress("rawpt",rawpt);
       tr_in[idir]->SetBranchAddress("jtpt",jtpt);
       tr_in[idir]->SetBranchAddress("jteta",jteta);
       tr_in[idir]->SetBranchAddress("jty",jty);
       tr_in[idir]->SetBranchAddress("jtphi",jtphi);
       tr_in[idir]->SetBranchAddress("pthat",&pthat);
       tr_in[idir]->SetBranchAddress("refpt",refpt);
       tr_in[idir]->SetBranchAddress("refeta",refeta);
       tr_in[idir]->SetBranchAddress("refy",refy);
       tr_in[idir]->SetBranchAddress("refphi",refphi);
       tr_in[idir]->SetBranchAddress("refdphijt",refdphijt);
       tr_in[idir]->SetBranchAddress("refdrjt",refdrjt);
       tr_in[idir]->SetBranchAddress("refparton_pt",refparton_pt);
       tr_in[idir]->SetBranchAddress("refparton_flavor",refparton_flavor);
       /*
       tr_in[idir]->SetBranchAddress("ngen",&ngen);
       tr_in[idir]->SetBranchAddress("genmatchindex",genmatchindex);
       tr_in[idir]->SetBranchAddress("genpt",genpt);
       tr_in[idir]->SetBranchAddress("geneta",geneta);
       tr_in[idir]->SetBranchAddress("geny",geny);
       tr_in[idir]->SetBranchAddress("genphi",genphi);
       tr_in[idir]->SetBranchAddress("gendphijt",gendphijt);
       tr_in[idir]->SetBranchAddress("gendrjt",gendrjt);
       */
       fout->mkdir(outputDirName[idir].c_str());
       fout->cd(outputDirName[idir].c_str());

       tr_out[idir] = new TTree("t","Jet Response Analyzer");
       tr_out[idir]->SetMaxTreeSize(4200000000);
       
       // Set output branch addresses.
       
       tr_out[idir]->Branch("nref",&nref,"nref/I");
       tr_out[idir]->Branch("jtpt",jtpt,"jtpt[nref]/F");
       tr_out[idir]->Branch("rawpt",rawpt,"rawpt[nref]/F");
       tr_out[idir]->Branch("corrpt",corrpt,"corr[nref]/F");
       tr_out[idir]->Branch("jteta",jteta,"jteta[nref]/F");
       tr_out[idir]->Branch("jty",jty,"jty[nref]/F");
       tr_out[idir]->Branch("jtphi",jtphi,"jtphi[nref]/F");
       tr_out[idir]->Branch("refpt",refpt,"refpt[nref]/F");
       tr_out[idir]->Branch("refeta",refeta,"refeta[nref]/F");
       tr_out[idir]->Branch("refphi",refphi,"refphi[nref]/F");
       tr_out[idir]->Branch("refdphijt",refdphijt,"refdphijt[nref]/F");
       tr_out[idir]->Branch("refdrjt",refdrjt,"refdrjt[nref]/F");

       tr_out[idir]->Branch("pthat",&pthat,"pthat/F");
       tr_out[idir]->Branch("weight",&weight,"weight/F");
       
       // grab the JEC's
       string L2Name, L3Name;

       string corrFileName[7]= {
	 "IC5Calo",
	 "AK3PF",
	 "AK4PF",
	 "AK5PF",
	 "AK3Calo",
	 "AK4Calo",
	 "AK5Calo"
       };
       
       /*
       JetCorrectorParameters* parHI310x_l2, * parHI310x_l3;
       vector<JetCorrectorParameters> vpar_HI310x;   
       FactorizedJetCorrector *_JEC_HI310X;
       

       if(updateCorrections){   
	 
	 if(idir<1){
	   L2Name = "../JEC/data/HI_Calo_hiGoodTightTracks_D6T_399_L2Relative_"+corrFileName[idir]+".txt";
	   L3Name = "../JEC/data/HI_Calo_hiGoodTightTracks_D6T_399_L3Absolute_"+corrFileName[idir]+".txt";
	 }
	 else if(idir<4){
	   L2Name = "../JEC/data/HI_PFTowers_hiGoodTightTracks_D6T_399_L2Relative_"+corrFileName[idir]+".txt";
	   L3Name = "../JEC/data/HI_PFTowers_hiGoodTightTracks_D6T_399_L3Absolute_"+corrFileName[idir]+".txt";
	 }
	 else{
	   L2Name = "../JEC/data/HI_hiGoodTightTracks_D6T_399_L2Relative_"+corrFileName[idir]+".txt";
	   L3Name = "../JEC/data/HI_hiGoodTightTracks_D6T_399_L3Absolute_"+corrFileName[idir]+".txt";
	   }
	 cout<<" L2Name "<<L2Name<<endl;
	 parHI310x_l2 = new JetCorrectorParameters(L2Name.c_str());
	 parHI310x_l3 = new JetCorrectorParameters(L3Name.c_str());
	 
	 
	 vpar_HI310x.push_back(*parHI310x_l2);
	 vpar_HI310x.push_back(*parHI310x_l3);
	 _JEC_HI310X = new FactorizedJetCorrector(vpar_HI310x);
       }     
       */
       
   
       
       Long64_t nentries = tr_in[idir]->GetEntries();
       
       Long64_t nbytes = 0;
       for (Long64_t i=0; i<nentries;i++) {
	 nbytes += tr_in[idir]->GetEntry(i);
	 
	 if(pthat>maxpthat)continue;	 

	 for(int iref=0;iref<nref;iref++)
	   {
	     corrpt[iref] = jtpt[iref];
	     jtpt[iref] = rawpt[iref];
	     

	     /*    if(updateCorrections){
	       
	       _JEC_HI310X->setJetEta(jteta[iref]);
	       _JEC_HI310X->setJetPt(rawpt[iref]);
	       corrpt[iref] = rawpt[iref]*_JEC_HI310X->getCorrection(); 
	       
	     }
	     */
	   }
	 tr_out[idir]->Fill();
       }
       
       tr_out[idir]->Write();
       /*
       delete parHI310x_l2;
       delete parHI310x_l3;
       delete _JEC_HI310X;
       */
 
     }
   
   fout->Close();
   
}
