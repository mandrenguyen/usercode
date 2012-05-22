#include <iostream>
#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

void weightPtHatBins(){
    
  gROOT->Reset();
  
  // number of bins
  int N=4;

  TFile *fin[7], *fout[7];
  TTree *tr_in[7], *tr_out[7];
  TTree *tr_in_skim[7], *tr_out_skim[7];

  Int_t bounds[7] = {15,30,50,80,120,170,200};

  Float_t eff[4] ={
    37492./5430000.,
    5365./297000.,
    6037./200000.,
    9425./210000.
  };

  /* // uncomment for jet-triggered data
  Float_t xSections[7] = {
    1.94e-01,
    1.079e-02,
    1.021e-03,
    9.913e-05,
    1.128e-05,
    1.470e-06, 
    5.310e-07
  };
  //*/

  //* // uncomment for muon-triggered data
  Float_t xSections[7] = {
    1.94e-01 * (eff[0]*1.94e-01 - eff[1]*1.079e-02) / (1.94e-01 - 1.079e-02),
    1.079e-02 * (eff[1]*1.079e-02 - eff[2]*1.021e-03) / (1.079e-02 - 1.021e-03),
    1.021e-03 * (eff[2]*1.021e-03 - eff[3]*9.913e-05) / (1.021e-03 - 9.913e-05),
    9.913e-05 * eff[3],
    1.128e-05,
    1.470e-06, 
    5.310e-07
  };
  //*/

  char name[100];
  
  for (Int_t it=0; it<N; it++) {

    // specify input path
    sprintf(name,"/data_CMS/cms/mnguyen/bTaggingOutput/pythia/pthat%d/merged_bTagAnalyzers_ppRecoFromRaw_HLTMu3v2_fixTR.root",bounds[it]);
    fin[it] = new TFile(name);

    fin[it]->cd("/akPu3PFJetAnalyzer");
    tr_in[it] = (TTree*)gDirectory->Get("t");
    fin[it]->cd("/skimanalysis");
    tr_in_skim[it] = (TTree*)gDirectory->Get("HltTree");

    // specify output path
    sprintf(name,"/data_CMS/cms/sregnard/merged_bTagAnalyzers_ppRecoFromRaw_HLTMu3v2_fixTR_weighted3_%d.root",bounds[it]);
    fout[it] = new TFile(name,"RECREATE");

    fout[it]->mkdir("akPu3PFJetAnalyzer");
    fout[it]->mkdir("skimanalysis");

    tr_out[it] = new TTree("t","Jet Analyzer");
    tr_out_skim[it] = new TTree("HltTree","HltTree");

    Float_t fentries = (Float_t)tr_in[it]->GetEntries();
    Float_t weight = xSections[it]/(fentries/1000.);
    cout<<"  weight ("<< bounds[it] <<") : "<< weight <<endl;

    //Declaration of leaves types
    Int_t           evt;
    Float_t         b;
    Float_t         hf;
    Int_t           bin;
    Int_t           nref;
    Float_t         vx,vy,vz;
    Float_t         rawpt[30];
    Float_t         jtpt[30];
    Float_t         jteta[30];
    Float_t         jty[30];
    Float_t         jtphi[30];
    Float_t         jtpu[30];
    Float_t         discr_ssvHighEff[30];
    Float_t         discr_ssvHighPur[30];
    Float_t         discr_csvMva[30];
    Float_t         discr_csvSimple[30];
    Float_t         discr_muByIp3[30];
    Float_t         discr_muByPt[30];
    Float_t         discr_prob[30];
    Float_t         discr_probb[30];
    Float_t         discr_tcHighEff[30];
    Float_t         discr_tcHighPur[30];
    Int_t           nsvtx[30];
    Int_t           svtxntrk[30];
    Float_t         svtxdl[30];
    Float_t         svtxdls[30];
    Float_t         svtxm[30];
    Float_t         svtxpt[30];
    Int_t           nIPtrk[30];
    Int_t           nselIPtrk[30];
    Int_t   nIP;
    Int_t   ipJetIndex[400];
    Float_t ipPt[400];
    Float_t ipProb0[400];
    Float_t ipProb1[400];
    Float_t ip2d[400];
    Float_t ip2dSig[400];
    Float_t ip3d[400];
    Float_t ip3dSig[400];
    Float_t ipDist2Jet[400];
    Float_t ipDist2JetSig[400];
    Float_t ipClosest2Jet[400];
    Float_t         mue[30];
    Float_t         mupt[30];
    Float_t         mueta[30];
    Float_t         muphi[30];
    Float_t         mudr[30];
    Float_t         muptrel[30];
    Int_t           muchg[30];
    Float_t         pthat;
    Int_t         beamId1;
    Int_t         beamId2;
    Float_t         refpt[30];
    Float_t         refeta[30];
    Float_t         refy[30];
    Float_t         refphi[30];
    Float_t         refdphijt[30];
    Float_t         refdrjt[30];
    Float_t         refparton_pt[30];
    Int_t           refparton_flavor[30];
    Int_t           refparton_flavorForB[30];
    /*
    Int_t           ngen;
    Int_t           genmatchindex[100];
    Float_t         genpt[100];
    Float_t         geneta[100];
    Float_t         geny[100];
    Float_t         genphi[100];
    Float_t         gendphijt[100];
    Float_t         gendrjt[100];
    */

    // Set branch addresses.
    tr_in[it]->SetBranchAddress("evt",&evt);
    tr_in[it]->SetBranchAddress("b",&b);
    //tr_in[it]->SetBranchAddress("hf",&hf);
    //tr_in[it]->SetBranchAddress("bin",&bin);
    tr_in[it]->SetBranchAddress("vx",&vx);
    tr_in[it]->SetBranchAddress("vy",&vy);
    tr_in[it]->SetBranchAddress("vz",&vz);
    tr_in[it]->SetBranchAddress("nref",&nref);
    tr_in[it]->SetBranchAddress("rawpt",rawpt);
    tr_in[it]->SetBranchAddress("jtpt",jtpt);
    tr_in[it]->SetBranchAddress("jteta",jteta);
    tr_in[it]->SetBranchAddress("jty",jty);
    tr_in[it]->SetBranchAddress("jtphi",jtphi);
    tr_in[it]->SetBranchAddress("jtpu",jtpu);
    tr_in[it]->SetBranchAddress("discr_ssvHighEff",discr_ssvHighEff);
    tr_in[it]->SetBranchAddress("discr_ssvHighPur",discr_ssvHighPur);
    tr_in[it]->SetBranchAddress("discr_csvMva",discr_csvMva);
    tr_in[it]->SetBranchAddress("discr_csvSimple",discr_csvSimple);
    tr_in[it]->SetBranchAddress("discr_muByIp3",discr_muByIp3);
    tr_in[it]->SetBranchAddress("discr_muByPt",discr_muByPt);
    tr_in[it]->SetBranchAddress("discr_prob",discr_prob);
    tr_in[it]->SetBranchAddress("discr_probb",discr_probb);
    tr_in[it]->SetBranchAddress("discr_tcHighEff",discr_tcHighEff);
    tr_in[it]->SetBranchAddress("discr_tcHighPur",discr_tcHighPur);
    tr_in[it]->SetBranchAddress("nsvtx",nsvtx);
    tr_in[it]->SetBranchAddress("svtxntrk",svtxntrk);
    tr_in[it]->SetBranchAddress("svtxdl",svtxdl);
    tr_in[it]->SetBranchAddress("svtxdls",svtxdls);
    tr_in[it]->SetBranchAddress("svtxm",svtxm);
    tr_in[it]->SetBranchAddress("svtxpt",svtxpt);
    tr_in[it]->SetBranchAddress("nIPtrk",nIPtrk);
    tr_in[it]->SetBranchAddress("nIP",&nIP);
    tr_in[it]->SetBranchAddress("nselIPtrk",nselIPtrk);
    tr_in[it]->SetBranchAddress("ipJetIndex",ipJetIndex);
    tr_in[it]->SetBranchAddress("ipPt",ipPt);
    tr_in[it]->SetBranchAddress("ipProb0",ipProb0);
    tr_in[it]->SetBranchAddress("ipProb1",ipProb1);
    tr_in[it]->SetBranchAddress("ip2d",ip2d);
    tr_in[it]->SetBranchAddress("ip2dSig",ip2dSig);
    tr_in[it]->SetBranchAddress("ip3d",ip3d);
    tr_in[it]->SetBranchAddress("ip3dSig",ip3dSig);
    tr_in[it]->SetBranchAddress("ipDist2Jet",ipDist2Jet);
    tr_in[it]->SetBranchAddress("ipDist2JetSig",ipDist2JetSig);
    tr_in[it]->SetBranchAddress("ipClosest2Jet",ipClosest2Jet);
    tr_in[it]->SetBranchAddress("mue",mue);
    tr_in[it]->SetBranchAddress("mupt",mupt);
    tr_in[it]->SetBranchAddress("mueta",mueta);
    tr_in[it]->SetBranchAddress("muphi",muphi);
    tr_in[it]->SetBranchAddress("mudr",mudr);
    tr_in[it]->SetBranchAddress("muptrel",muptrel);
    tr_in[it]->SetBranchAddress("muchg",muchg);
    tr_in[it]->SetBranchAddress("pthat",&pthat);
    tr_in[it]->SetBranchAddress("beamId1",&beamId1);
    tr_in[it]->SetBranchAddress("beamId2",&beamId2);
    tr_in[it]->SetBranchAddress("refpt",refpt);
    tr_in[it]->SetBranchAddress("refeta",refeta);
    tr_in[it]->SetBranchAddress("refy",refy);
    tr_in[it]->SetBranchAddress("refphi",refphi);
    tr_in[it]->SetBranchAddress("refdphijt",refdphijt);
    tr_in[it]->SetBranchAddress("refdrjt",refdrjt);
    tr_in[it]->SetBranchAddress("refparton_pt",refparton_pt);
    tr_in[it]->SetBranchAddress("refparton_flavor",refparton_flavor);
    tr_in[it]->SetBranchAddress("refparton_flavorForB",refparton_flavorForB);
    /*
    tr_in[it]->SetBranchAddress("ngen",&ngen);
    tr_in[it]->SetBranchAddress("genmatchindex",genmatchindex);
    tr_in[it]->SetBranchAddress("genpt",genpt);
    tr_in[it]->SetBranchAddress("geneta",geneta);
    tr_in[it]->SetBranchAddress("geny",geny);
    tr_in[it]->SetBranchAddress("genphi",genphi);
    tr_in[it]->SetBranchAddress("gendphijt",gendphijt);
    tr_in[it]->SetBranchAddress("gendrjt",gendrjt);
    */
 
    // Set output branch addresses.
    tr_out[it]->Branch("evt",&evt,"evt/I");
    tr_out[it]->Branch("b",&b,"b/F");
    tr_out[it]->Branch("hf",&hf,"hf/F");
    tr_out[it]->Branch("bin",&bin,"bin/I");
    tr_out[it]->Branch("vx",&vx,"vx/F");
    tr_out[it]->Branch("vy",&vy,"vy/F");
    tr_out[it]->Branch("vz",&vz,"vz/F");
    tr_out[it]->Branch("nref",&nref,"nref/I");
    tr_out[it]->Branch("rawpt",rawpt,"rawpt[nref]/F");
    tr_out[it]->Branch("jtpt",jtpt,"jtpt[nref]/F");
    tr_out[it]->Branch("jteta",jteta,"jteta[nref]/F");
    tr_out[it]->Branch("jty",jty,"jty[nref]/F");
    tr_out[it]->Branch("jtphi",jtphi,"jtphi[nref]/F");
    tr_out[it]->Branch("jtpu",jtpu,"jtpu[nref]/F");
    tr_out[it]->Branch("discr_ssvHighEff",discr_ssvHighEff,"discr_ssvHighEff[nref]/F");
    tr_out[it]->Branch("discr_ssvHighPur",discr_ssvHighPur,"discr_ssvHighPur[nref]/F");
    tr_out[it]->Branch("discr_csvMva",discr_csvMva,"discr_csvMva[nref]/F");
    tr_out[it]->Branch("discr_csvSimple",discr_csvSimple,"discr_csvSimple[nref]/F");
    tr_out[it]->Branch("discr_muByIp3",discr_muByIp3,"discr_muByIp3[nref]/F");
    tr_out[it]->Branch("discr_muByPt",discr_muByPt,"discr_muByPt[nref]/F");
    tr_out[it]->Branch("discr_prob",discr_prob,"discr_prob[nref]/F");
    tr_out[it]->Branch("discr_probb",discr_probb,"discr_probb[nref]/F");
    tr_out[it]->Branch("discr_tcHighEff",discr_tcHighEff,"discr_tcHighEff[nref]/F");
    tr_out[it]->Branch("discr_tcHighPur",discr_tcHighPur,"discr_tcHighPur[nref]/F");
    tr_out[it]->Branch("nsvtx",nsvtx,"nsvtx[nref]/I");
    tr_out[it]->Branch("svtxntrk",svtxntrk,"svtxntrk[nref]/I");
    tr_out[it]->Branch("svtxdl",svtxdl,"svtxdl[nref]/F");
    tr_out[it]->Branch("svtxdls",svtxdls,"svtxdls[nref]/F");
    tr_out[it]->Branch("svtxm",svtxm,"svtxm[nref]/F");
    tr_out[it]->Branch("svtxpt",svtxpt,"svtxpt[nref]/F");
    tr_out[it]->Branch("nIPtrk",nIPtrk,"nIPtrk[nref]/I");
    tr_out[it]->Branch("nselIPtrk",nselIPtrk,"nselIPtrk[nref]/I");
    tr_out[it]->Branch("nIP",&nIP,"nIP/I");
    tr_out[it]->Branch("ipJetIndex",ipJetIndex,"ipJetIndex[nIP]/I");
    tr_out[it]->Branch("ipPt",ipPt,"ipPt[nIP]/F");
    tr_out[it]->Branch("ipProb0",ipProb0,"ipProb0[nIP]/F");
    tr_out[it]->Branch("ipProb1",ipProb1,"ipProb1[nIP]/F");
    tr_out[it]->Branch("ip2d",ip2d,"ip2d[nIP]/F");
    tr_out[it]->Branch("ip2dSig",ip2dSig,"ip2dSig[nIP]/F");
    tr_out[it]->Branch("ip3d",ip3d,"ip3d[nIP]/F");
    tr_out[it]->Branch("ip3dSig",ip3dSig,"ip3dSig[nIP]/F");
    tr_out[it]->Branch("ipDist2Jet",ipDist2Jet,"ipDist2Jet[nIP]/F");
    tr_out[it]->Branch("ipDist2JetSig",ipDist2JetSig,"ipDist2JetSig[nIP]/F");
    tr_out[it]->Branch("ipClosest2Jet",ipClosest2Jet,"ipClosest2Jet[nIP]/F");  
    tr_out[it]->Branch("mue",mue,"mue[nref]/F");
    tr_out[it]->Branch("mupt",mupt,"mupt[nref]/F");
    tr_out[it]->Branch("mueta",mueta,"mueta[nref]/F");
    tr_out[it]->Branch("muphi",muphi,"muphi[nref]/F");
    tr_out[it]->Branch("mudr",mudr,"mudr[nref]/F");
    tr_out[it]->Branch("muptrel",muptrel,"muptre[nref]/F");
    tr_out[it]->Branch("muchg",muchg,"muchg[nref]/I");
    tr_out[it]->Branch("pthat",&pthat,"pthat/F");
    tr_out[it]->Branch("beamId1",&beamId1,"beamId1/I");
    tr_out[it]->Branch("beamId2",&beamId1,"beamId2/I");
    tr_out[it]->Branch("refpt",refpt,"refpt[nref]/F");
    tr_out[it]->Branch("refeta",refeta,"refeta[nref]/F");
    tr_out[it]->Branch("refy",refy,"refy[nref]/F");
    tr_out[it]->Branch("refphi",refphi,"refphi[nref]/F");
    tr_out[it]->Branch("refdphijt",refdphijt,"refdphijt[nref]/F");
    tr_out[it]->Branch("refdrjt",refdrjt,"refdrjt[nref]/F");
    tr_out[it]->Branch("refparton_pt",refparton_pt,"refparton_pt[nref]/F");
    tr_out[it]->Branch("refparton_flavor",refparton_flavor,"refparton_flavor[nref]/I");
    tr_out[it]->Branch("refparton_flavorForB",refparton_flavorForB,"refparton_flavorForB[nref]/I");
    /*
    tr_out[it]->Branch("ngen",&ngen,"ngen/I");
    tr_out[it]->Branch("genmatchindex",genmatchindex,"genmatchindex[nref]/I");
    tr_out[it]->Branch("genpt",genpt,"genpt[nref]/F");
    tr_out[it]->Branch("geneta",geneta,"geneta[nref]/F");
    tr_out[it]->Branch("geny",geny,"geny[nref]/F");
    tr_out[it]->Branch("genphi",genphi,"genphi[nref]/F");
    tr_out[it]->Branch("gendphijt",gendphijt,"gendphijt[nref]/F");
    tr_out[it]->Branch("gendrjt",gendrjt,"gendrjt[nref]/F");
    */
    tr_out[it]->Branch("weight",&weight,"weight/F");

    //Declaration of leaves types
    Int_t           raw2digi_step;
    Int_t           L1Reco_step;
    Int_t           reconstruction_step;
    Int_t           reco_extra_jet;
    Int_t           higen_step;
    Int_t           pat_step;
    Int_t           ana_step;
    Int_t           pvSel;
    Int_t           hbheNoiseSel;
    Int_t           spikeSel;
    Int_t           collSell;
    Int_t           hltAna;
    Int_t           superFilterPath;

    // Set branch addresses.
    tr_in_skim[it]->SetBranchAddress("raw2digi_step",&raw2digi_step);
    tr_in_skim[it]->SetBranchAddress("L1Reco_step",&L1Reco_step);
    tr_in_skim[it]->SetBranchAddress("reconstruction_step",&reconstruction_step);
    tr_in_skim[it]->SetBranchAddress("reco_extra_jet",&reco_extra_jet);
    tr_in_skim[it]->SetBranchAddress("higen_step",&higen_step);
    tr_in_skim[it]->SetBranchAddress("pat_step",&pat_step);
    tr_in_skim[it]->SetBranchAddress("ana_step",&ana_step);
    tr_in_skim[it]->SetBranchAddress("pvSel",&pvSel);
    tr_in_skim[it]->SetBranchAddress("hbheNoiseSel",&hbheNoiseSel);
    tr_in_skim[it]->SetBranchAddress("spikeSel",&spikeSel);
    tr_in_skim[it]->SetBranchAddress("collSell",&collSell);
    tr_in_skim[it]->SetBranchAddress("hltAna",&hltAna);
    tr_in_skim[it]->SetBranchAddress("superFilterPath",&superFilterPath);

    // Set output branch addresses.
    tr_out_skim[it]->Branch("raw2digi_step",&raw2digi_step,"raw2digi_step/I");
    tr_out_skim[it]->Branch("L1Reco_step",&L1Reco_step,"L1Reco_step/I");
    tr_out_skim[it]->Branch("reconstruction_step",&reconstruction_step,"reconstruction_step/I");
    tr_out_skim[it]->Branch("reco_extra_jet",&reco_extra_jet,"reco_extra_jet/I");
    tr_out_skim[it]->Branch("higen_step",&higen_step,"higen_step/I");
    tr_out_skim[it]->Branch("pat_step",&pat_step,"pat_step/I");
    tr_out_skim[it]->Branch("ana_step",&ana_step,"ana_step/I");
    tr_out_skim[it]->Branch("pvSel",&pvSel,"pvSel/I");
    tr_out_skim[it]->Branch("hbheNoiseSel",&hbheNoiseSel,"hbheNoiseSel/I");
    tr_out_skim[it]->Branch("spikeSel",&spikeSel,"spikeSel/I");
    tr_out_skim[it]->Branch("collSell",&collSell,"collSell/I");
    tr_out_skim[it]->Branch("hltAna",&hltAna,"hltAna/I");
    tr_out_skim[it]->Branch("superFilterPath",&superFilterPath,"superFilterPath/I");


    Long64_t nentries = tr_in[it]->GetEntries();
    Long64_t nbytes = 0;
    Long64_t nbytes_skim = 0;

    for (Long64_t i=0; i<nentries;i++) {

      if(i%100000==0) cout<<" i = "<<i<<" out of "<<nentries<<" ("<<(int)(100*(float)i/(float)nentries)<<"%)"<<endl;

      nbytes += tr_in[it]->GetEntry(i);
      nbytes_skim += tr_in_skim[it]->GetEntry(i);

      if (it<N-1){
	if (pthat>bounds[it+1]) continue;
      }

      tr_out[it]->Fill();
      tr_out_skim[it]->Fill();

    }
    
    fout[it]->cd("akPu3PFJetAnalyzer");
    tr_out[it]->Write();
    fout[it]->cd("skimanalysis");
    tr_out_skim[it]->Write();

    fout[it]->Close();

  }

}

