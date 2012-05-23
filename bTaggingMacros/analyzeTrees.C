#include <iostream>
#include "TH1.h"
#include "TTree.h"
#include "TFile.h"
#include "TNtuple.h"

void analyzeTrees(int isMC=1, int useWeight=1, int doJets=1, int doTracks=1, float minJetPt=60, float maxJetEta=2, float minMuPt=5)
{

  TFile *fin;

  if(isMC)fin=new TFile("/data_CMS/cms/sregnard/merged_bJetAnalyzers_ppRecoFromRaw_fixTR_weighted.root");
  else fin=new TFile("/data_CMS/cms/mnguyen/bTaggingOutput/ppData/ppDataJet40_ppRecoFromRaw_fixTR/merged_bTagAnalyzers.root");

  //if(isMC)fin=new TFile("/data_CMS/cms/sregnard/merged_bTagAnalyzers_ppRecoFromRaw_HLTMu3v2_fixTR_weighted3.root");
  //else fin=new TFile("/data_CMS/cms/mnguyen/bTaggingOutput/ppData/ppDataMu3_ppRecoFromRaw_fixTR/merged_bTagAnalyzers.root");

  //if(isMC)fin=new TFile("../sample/merged_bJetAnalyzers_ppRecoFromRaw_fixEvtSel_pythia30.root");
  //else fin=new TFile("../sample/merged_jetJetAnalyzers_ppData2760_ppRecoFromRaw_fixEvtSel.root");

  //TTree *t = (TTree*) fin->Get("akPu3PFJetAnalyzer/t");
  TTree *t = (TTree*) fin->Get("ak5PFJetAnalyzer/t");
  TTree *tSkim = (TTree*) fin->Get("skimanalysis/HltTree");

  //Declaration of leaves types                  
  Int_t           evt;
  //Float_t         b;
  //Float_t         hf;
  //Int_t           bin;
  Int_t           nref;
  Float_t         rawpt[1000];
  Float_t         jtpt[1000];
  Float_t         jteta[1000];
  Float_t         jty[1000];
  Float_t         jtphi[1000];
  Float_t         jtpu[1000];
  Float_t         discr_ssvHighEff[1000];
  Float_t         discr_ssvHighPur[1000];
  Float_t         discr_csvMva[1000];
  Float_t         discr_csvSimple[1000];
  Float_t         discr_muByIp3[1000];
  Float_t         discr_muByPt[1000];
  Float_t         discr_prob[1000];
  Float_t         discr_probb[1000];
  Float_t         discr_tcHighEff[1000];
  Float_t         discr_tcHighPur[1000];
  Int_t           nsvtx[1000];
  Int_t           svtxntrk[1000];
  Float_t         svtxdl[1000];
  Float_t         svtxdls[1000];
  Float_t         svtxm[1000];
  Float_t         svtxpt[1000];
  Int_t           nIPtrk[1000];
  Int_t           nselIPtrk[1000];
  Int_t   nIP;
  Int_t   ipJetIndex[1000];
  Float_t ipPt[1000];
  Float_t ipProb0[1000];
  Float_t ipProb1[1000];
  Float_t ip2d[1000];
  Float_t ip2dSig[1000];
  Float_t ip3d[1000];
  Float_t ip3dSig[1000];
  Float_t ipDist2Jet[1000];
  Float_t ipDist2JetSig[1000];
  Float_t ipClosest2Jet[1000];
  Float_t         mue[1000];
  Float_t         mupt[1000];
  Float_t         mueta[1000];
  Float_t         muphi[1000];
  Float_t         mudr[1000];
  Float_t         muptrel[1000];
  Int_t           muchg[1000];
  Float_t         pthat;
  Int_t           beamId1;
  Int_t           beamId2;
  Float_t         refpt[1000];
  Float_t         refeta[1000];
  Float_t         refy[1000];
  Float_t         refphi[1000];
  Float_t         refdphijt[1000];
  Float_t         refdrjt[1000];
  Float_t         refparton_pt[1000];
  Int_t           refparton_flavor[1000];
  Int_t           refparton_flavorForB[1000];
  /*
  Int_t           ngen;
  Int_t           genmatchindex[1000];
  Float_t         genpt[1000];
  Float_t         geneta[1000];
  Float_t         geny[1000];
  Float_t         genphi[1000];
  Float_t         gendphijt[1000];
  Float_t         gendrjt[1000];
  */
  Float_t         weight;
  
  Int_t pvSel;
  Int_t hbheNoiseSel;
  Int_t spikeSel;
  Int_t collSell;
    
  t->SetBranchAddress("evt",&evt);
  //t->SetBranchAddress("b",&b);C
  //t->SetBranchAddress("hf",&hf);   
  //t->SetBranchAddress("bin",&bin);           
  t->SetBranchAddress("nref",&nref);
  t->SetBranchAddress("rawpt",rawpt);
  t->SetBranchAddress("jtpt",jtpt);
  t->SetBranchAddress("jteta",jteta);
  t->SetBranchAddress("jty",jty);
  t->SetBranchAddress("jtphi",jtphi);
  t->SetBranchAddress("jtpu",jtpu);
  t->SetBranchAddress("discr_ssvHighEff",discr_ssvHighEff);
  t->SetBranchAddress("discr_ssvHighPur",discr_ssvHighPur);
  t->SetBranchAddress("discr_csvMva",discr_csvMva);
  t->SetBranchAddress("discr_csvSimple",discr_csvSimple);
  t->SetBranchAddress("discr_muByIp3",discr_muByIp3);
  t->SetBranchAddress("discr_muByPt",discr_muByPt);
  t->SetBranchAddress("discr_prob",discr_prob);
  t->SetBranchAddress("discr_probb",discr_probb);
  t->SetBranchAddress("discr_tcHighEff",discr_tcHighEff);
  t->SetBranchAddress("discr_tcHighPur",discr_tcHighPur);
  t->SetBranchAddress("nsvtx",nsvtx);
  t->SetBranchAddress("svtxntrk",svtxntrk);
  t->SetBranchAddress("svtxdl",svtxdl);
  t->SetBranchAddress("svtxdls",svtxdls);
  t->SetBranchAddress("svtxm",svtxm);
  t->SetBranchAddress("svtxpt",svtxpt);
  t->SetBranchAddress("nIPtrk",nIPtrk);
  t->SetBranchAddress("nselIPtrk",nselIPtrk); 
  t->SetBranchAddress("nIP",&nIP);
  if(doTracks){
    t->SetBranchAddress("ipJetIndex",ipJetIndex);
    t->SetBranchAddress("ipPt",ipPt);
    t->SetBranchAddress("ipProb0",ipProb0);
    t->SetBranchAddress("ipProb1",ipProb1);
    t->SetBranchAddress("ip2d",ip2d);
    t->SetBranchAddress("ip2dSig",ip2dSig);
    t->SetBranchAddress("ip3d",ip3d);
    t->SetBranchAddress("ip3dSig",ip3dSig);
    t->SetBranchAddress("ipDist2Jet",ipDist2Jet);
    t->SetBranchAddress("ipDist2JetSig",ipDist2JetSig);
    t->SetBranchAddress("ipClosest2Jet",ipClosest2Jet);
  }
  t->SetBranchAddress("mue",mue);
  t->SetBranchAddress("mupt",mupt);
  t->SetBranchAddress("mueta",mueta);
  t->SetBranchAddress("muphi",muphi);
  t->SetBranchAddress("mudr",mudr);
  t->SetBranchAddress("muptrel",muptrel);
  t->SetBranchAddress("muchg",muchg);
  if(isMC){
    t->SetBranchAddress("pthat",&pthat);
    t->SetBranchAddress("beamId1",&beamId1);
    t->SetBranchAddress("beamId2",&beamId2);
    t->SetBranchAddress("refpt",refpt);
    t->SetBranchAddress("refeta",refeta);
    t->SetBranchAddress("refy",refy);
    t->SetBranchAddress("refphi",refphi);
    t->SetBranchAddress("refdphijt",refdphijt);
    t->SetBranchAddress("refdrjt",refdrjt);
    t->SetBranchAddress("refparton_pt",refparton_pt);
    t->SetBranchAddress("refparton_flavor",refparton_flavor);
    t->SetBranchAddress("refparton_flavorForB",refparton_flavorForB);
    /*
    t->SetBranchAddress("ngen",&ngen);
    t->SetBranchAddress("genmatchindex",genmatchindex);
    t->SetBranchAddress("genpt",genpt);
    t->SetBranchAddress("geneta",geneta);
    t->SetBranchAddress("geny",geny);
    t->SetBranchAddress("genphi",genphi);
    t->SetBranchAddress("gendphijt",gendphijt);
    t->SetBranchAddress("gendrjt",gendrjt);
    */
  }
  if(isMC&&useWeight)t->SetBranchAddress("weight",&weight);

  tSkim->SetBranchAddress("pvSel",&pvSel);
  tSkim->SetBranchAddress("hbheNoiseSel",&hbheNoiseSel);
  tSkim->SetBranchAddress("spikeSel",&spikeSel);
  tSkim->SetBranchAddress("collSell",&collSell);

  TH1F *hjtpt = new TH1F("hjtpt","hjtpt",50,0,250);
  TH1F *hjtptB = new TH1F("hjtptB","hjtptB",50,0,250);
  TH1F *hjtptC = new TH1F("hjtptC","hjtptC",50,0,250);
  TH1F *hjtptL = new TH1F("hjtptL","hjtptL",50,0,250);
  TH1F *hjtptU = new TH1F("hjtptU","hjtptU",50,0,250);

  TH1F *hdiscr_csvSimple = new TH1F("hdiscr_csvSimple","hdiscr_csvSimple",40,0,1);
  TH1F *hdiscr_csvSimpleB = new TH1F("hdiscr_csvSimpleB","hdiscr_csvSimpleB",40,0,1);
  TH1F *hdiscr_csvSimpleC = new TH1F("hdiscr_csvSimpleC","hdiscr_csvSimpleC",40,0,1);
  TH1F *hdiscr_csvSimpleL = new TH1F("hdiscr_csvSimpleL","hdiscr_csvSimpleL",40,0,1);
  
  TH1F *hdiscr_prob = new TH1F("hdiscr_prob","hdiscr_prob",40,0,3);
  TH1F *hdiscr_probB = new TH1F("hdiscr_probB","hdiscr_probB",40,0,3);
  TH1F *hdiscr_probC = new TH1F("hdiscr_probC","hdiscr_probC",40,0,3);
  TH1F *hdiscr_probL = new TH1F("hdiscr_probL","hdiscr_probL",40,0,3);
  
  TH1F *hdiscr_ssvHighEff = new TH1F("hdiscr_ssvHighEff","hdiscr_ssvHighEff",50,1,6);
  TH1F *hdiscr_ssvHighEffB = new TH1F("hdiscr_ssvHighEffB","hdiscr_ssvHighEffB",50,1,6);
  TH1F *hdiscr_ssvHighEffC = new TH1F("hdiscr_ssvHighEffC","hdiscr_ssvHighEffC",50,1,6);
  TH1F *hdiscr_ssvHighEffL = new TH1F("hdiscr_ssvHighEffL","hdiscr_ssvHighEffL",50,1,6);
  
  TH1F *hdiscr_ssvHighPur = new TH1F("hdiscr_ssvHighPur","hdiscr_ssvHighPur",50,1,6);
  TH1F *hdiscr_ssvHighPurB = new TH1F("hdiscr_ssvHighPurB","hdiscr_ssvHighPurB",50,1,6);
  TH1F *hdiscr_ssvHighPurC = new TH1F("hdiscr_ssvHighPurC","hdiscr_ssvHighPurC",50,1,6);
  TH1F *hdiscr_ssvHighPurL = new TH1F("hdiscr_ssvHighPurL","hdiscr_ssvHighPurL",50,1,6);
  
  TH1F *hnsvtx = new TH1F("hnsvtx","hnsvtx",6,-0.5,5.5);
  TH1F *hnsvtxB = new TH1F("hnsvtxB","hnsvtxB",6,-0.5,5.5);
  TH1F *hnsvtxC = new TH1F("hnsvtxC","hnsvtxC",6,-0.5,5.5);
  TH1F *hnsvtxL = new TH1F("hnsvtxL","hnsvtxL",6,-0.5,5.5);
  
  TH1F *hsvtxntrk = new TH1F("hsvtxntrk","hsvtxntrk",12,-0.5,11.5);
  TH1F *hsvtxntrkB = new TH1F("hsvtxntrkB","hsvtxntrkB",12,-0.5,11.5);
  TH1F *hsvtxntrkC = new TH1F("hsvtxntrkC","hsvtxntrkC",12,-0.5,11.5);
  TH1F *hsvtxntrkL = new TH1F("hsvtxntrkL","hsvtxntrkL",12,-0.5,11.5);

  TH1F *hsvtxdl = new TH1F("hsvtxdl","hsvtxdl",20,0,10);
  TH1F *hsvtxdlB = new TH1F("hsvtxdlB","hsvtxdlB",20,0,10);
  TH1F *hsvtxdlC = new TH1F("hsvtxdlC","hsvtxdlC",20,0,10);
  TH1F *hsvtxdlL = new TH1F("hsvtxdlL","hsvtxdlL",20,0,10);

  TH1F *hsvtxdls = new TH1F("hsvtxdls","hsvtxdls",40,0,80);
  TH1F *hsvtxdlsB = new TH1F("hsvtxdlsB","hsvtxdlsB",40,0,80);
  TH1F *hsvtxdlsC = new TH1F("hsvtxdlsC","hsvtxdlsC",40,0,80);
  TH1F *hsvtxdlsL = new TH1F("hsvtxdlsL","hsvtxdlsL",40,0,80);
  
  TH1F *hsvtxm = new TH1F("hsvtxm","hsvtxm",32,0,8);
  TH1F *hsvtxmB = new TH1F("hsvtxmB","hsvtxmB",32,0,8);
  TH1F *hsvtxmC = new TH1F("hsvtxmC","hsvtxmC",32,0,8);
  TH1F *hsvtxmL = new TH1F("hsvtxmL","hsvtxmL",32,0,8);
  
  TH1F *hsvtxmSV3 = new TH1F("hsvtxmSV3","hsvtxmSV3",32,0,8);
  TH1F *hsvtxmSV3B = new TH1F("hsvtxmSV3B","hsvtxmSV3B",32,0,8);
  TH1F *hsvtxmSV3C = new TH1F("hsvtxmSV3C","hsvtxmSV3C",32,0,8);
  TH1F *hsvtxmSV3L = new TH1F("hsvtxmSV3L","hsvtxmSV3L",32,0,8);
  
  TH1F *hsvtxpt = new TH1F("hsvtxpt","hsvtxpt",20,0,100);
  TH1F *hsvtxptB = new TH1F("hsvtxptB","hsvtxptB",20,0,100);
  TH1F *hsvtxptC = new TH1F("hsvtxptC","hsvtxptC",20,0,100);
  TH1F *hsvtxptL = new TH1F("hsvtxptL","hsvtxptL",20,0,100);
  
  TH1F *hsvtxptSV3 = new TH1F("hsvtxptSV3","hsvtxptSV3",20,0,100);
  TH1F *hsvtxptSV3B = new TH1F("hsvtxptSV3B","hsvtxptSV3B",20,0,100);
  TH1F *hsvtxptSV3C = new TH1F("hsvtxptSV3C","hsvtxptSV3C",20,0,100);
  TH1F *hsvtxptSV3L = new TH1F("hsvtxptSV3L","hsvtxptSV3L",20,0,100);
  
  TH1F *hnIPtrk = new TH1F("hnIPtrk","hnIPtrk",40,0,40);
  TH1F *hnIPtrkB = new TH1F("hnIPtrkB","hnIPtrkB",40,0,40);
  TH1F *hnIPtrkC = new TH1F("hnIPtrkC","hnIPtrkC",40,0,40);
  TH1F *hnIPtrkL = new TH1F("hnIPtrkL","hnIPtrkL",40,0,40);
  
  TH1F *hnselIPtrk = new TH1F("hnselIPtrk","hnselIPtrk",40,0,40);
  TH1F *hnselIPtrkB = new TH1F("hnselIPtrkB","hnselIPtrkB",40,0,40);
  TH1F *hnselIPtrkC = new TH1F("hnselIPtrkC","hnselIPtrkC",40,0,40);
  TH1F *hnselIPtrkL = new TH1F("hnselIPtrkL","hnselIPtrkL",40,0,40);
  
  TH1F *hmuptrel = new TH1F("hmuptrel","hmuptrel",40,0,4);
  TH1F *hmuptrelB = new TH1F("hmuptrelB","hmuptrelB",40,0,4);
  TH1F *hmuptrelC = new TH1F("hmuptrelC","hmuptrelC",40,0,4);
  TH1F *hmuptrelL = new TH1F("hmuptrelL","hmuptrelL",40,0,4);
  
  TH1F *hmuptrelSV2 = new TH1F("hmuptrelSV2","hmuptrelSV2",40,0,4);
  TH1F *hmuptrelSV2B = new TH1F("hmuptrelSV2B","hmuptrelSV2B",40,0,4);
  TH1F *hmuptrelSV2C = new TH1F("hmuptrelSV2C","hmuptrelSV2C",40,0,4);
  TH1F *hmuptrelSV2L = new TH1F("hmuptrelSV2L","hmuptrelSV2L",40,0,4);
  
  TH1F *hmuptrelSV3 = new TH1F("hmuptrelSV3","hmuptrelSV3",40,0,4);
  TH1F *hmuptrelSV3B = new TH1F("hmuptrelSV3B","hmuptrelSV3B",40,0,4);
  TH1F *hmuptrelSV3C = new TH1F("hmuptrelSV3C","hmuptrelSV3C",40,0,4);
  TH1F *hmuptrelSV3L = new TH1F("hmuptrelSV3L","hmuptrelSV3L",40,0,4);
  
  TH1F *hipPt = new TH1F("hipPt","hipPt",40,0,40);
  TH1F *hipPtB = new TH1F("hipPtB","hipPtB",40,0,40);
  TH1F *hipPtC = new TH1F("hipPtC","hipPtC",40,0,40);
  TH1F *hipPtL = new TH1F("hipPtL","hipPtL",40,0,40);
  
  TH1F *hipProb0 = new TH1F("hipProb0","hipProb0",40,-1,1);
  TH1F *hipProb0B = new TH1F("hipProb0B","hipProb0B",40,-1,1);
  TH1F *hipProb0C = new TH1F("hipProb0C","hipProb0C",40,-1,1);
  TH1F *hipProb0L = new TH1F("hipProb0L","hipProb0L",40,-1,1);
  
  TH1F *hipProb1 = new TH1F("hipProb1","hipProb1",40,-1,1);
  TH1F *hipProb1B = new TH1F("hipProb1B","hipProb1B",40,-1,1);
  TH1F *hipProb1C = new TH1F("hipProb1C","hipProb1C",40,-1,1);
  TH1F *hipProb1L = new TH1F("hipProb1L","hipProb1L",40,-1,1);
  
  TH1F *hip2d = new TH1F("hip2d","hip2d",40,-0.2,0.2);
  TH1F *hip2dB = new TH1F("hip2dB","hip2dB",40,-0.2,0.2);
  TH1F *hip2dC = new TH1F("hip2dC","hip2dC",40,-0.2,0.2);
  TH1F *hip2dL = new TH1F("hip2dL","hip2dL",40,-0.2,0.2);
  
  TH1F *hip2dSig = new TH1F("hip2dSig","hip2dSig",40,-100,100);
  TH1F *hip2dSigB = new TH1F("hip2dSigB","hip2dSigB",40,-100,100);
  TH1F *hip2dSigC = new TH1F("hip2dSigC","hip2dSigC",40,-100,100);
  TH1F *hip2dSigL = new TH1F("hip2dSigL","hip2dSigL",40,-100,100);
  
  TH1F *hip3d = new TH1F("hip3d","hip3d",40,-0.2,0.2);
  TH1F *hip3dB = new TH1F("hip3dB","hip3dB",40,-0.2,0.2);
  TH1F *hip3dC = new TH1F("hip3dC","hip3dC",40,-0.2,0.2);
  TH1F *hip3dL = new TH1F("hip3dL","hip3dL",40,-0.2,0.2);
  
  TH1F *hip3dSig = new TH1F("hip3dSig","hip3dSig",40,-100,100);
  TH1F *hip3dSigB = new TH1F("hip3dSigB","hip3dSigB",40,-100,100);
  TH1F *hip3dSigC = new TH1F("hip3dSigC","hip3dSigC",40,-100,100);
  TH1F *hip3dSigL = new TH1F("hip3dSigL","hip3dSigL",40,-100,100);
  
  TH1F *hipDist2Jet = new TH1F("hipDist2Jet","hipDist2Jet",40,-0.1,0);
  TH1F *hipDist2JetB = new TH1F("hipDist2JetB","hipDist2JetB",40,-0.1,0);
  TH1F *hipDist2JetC = new TH1F("hipDist2JetC","hipDist2JetC",40,-0.1,0);
  TH1F *hipDist2JetL = new TH1F("hipDist2JetL","hipDist2JetL",40,-0.1,0);
  
  TH1F *hipDist2JetSig = new TH1F("hipDist2JetSig","hipDist2JetSig",40,-0.1,0.1);
  TH1F *hipDist2JetSigB = new TH1F("hipDist2JetSigB","hipDist2JetSigB",40,-0.1,0.1);
  TH1F *hipDist2JetSigC = new TH1F("hipDist2JetSigC","hipDist2JetSigC",40,-0.1,0.1);
  TH1F *hipDist2JetSigL = new TH1F("hipDist2JetSigL","hipDist2JetSigL",40,-0.1,0.1);
  
  TH1F *hipClosest2Jet = new TH1F("hipClosest2Jet","hipClosest2Jet",40,0,1);
  TH1F *hipClosest2JetB = new TH1F("hipClosest2JetB","hipClosest2JetB",40,0,1);
  TH1F *hipClosest2JetC = new TH1F("hipClosest2JetC","hipClosest2JetC",40,0,1);
  TH1F *hipClosest2JetL = new TH1F("hipClosest2JetL","hipClosest2JetL",40,0,1);
  
  Long64_t nentries = t->GetEntries();

  TFile *fout;
  if(isMC)fout=new TFile("histos/ppMC.root","recreate");
  else fout=new TFile("histos/ppdata.root","recreate");

  TNtuple *nt = new TNtuple("nt","","jtpt:discr_csvSimple:discr_prob:svtxm:refparton_flavorForB");

  for (Long64_t i=0; i<nentries;i++) {

    if (i%100000==0) cout<<" i = "<<i<<" out of "<<nentries<<" ("<<(int)(100*(float)i/(float)nentries)<<"%)"<<endl; 

    tSkim->GetEntry(i);
    if(!pvSel||!hbheNoiseSel||!spikeSel) continue;

    t->GetEntry(i);
    if(isMC){
      if(beamId1==2112 || beamId2==2112)  continue;
    }

    float w=1.;
    if(isMC&&useWeight) w=weight;

    if(doJets){

      for(int ij=0;ij<nref;ij++){
	
	//if(jtpt[ij]>minJetPt && fabs(jteta[ij])<maxJetEta && sqrt(acos(cos(jtphi[ij]-muphi[ij]))*acos(cos(jtphi[ij]-muphi[ij]))+(jteta[ij]-mueta[ij])*(jteta[ij]-mueta[ij]))<0.5 && mupt[ij]>minMuPt){ // with muon requirement
	if(jtpt[ij]>minJetPt && fabs(jteta[ij])<maxJetEta){ // without muon requirement
	  
	  // fill ntuple
	  nt->Fill(jtpt[ij],discr_csvSimple[ij],discr_probb[ij],svtxm[ij],refparton_flavorForB[ij]);
	 
	  hjtpt->Fill(jtpt[ij],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ij])==5)hjtptB->Fill(jtpt[ij],w);    
	    else if(abs(refparton_flavorForB[ij])==4)hjtptC->Fill(jtpt[ij],w);    
	    else if(abs(refparton_flavorForB[ij])<99)hjtptL->Fill(jtpt[ij],w);    
	    else hjtptU->Fill(jtpt[ij],w);    
	  }
	  //*
	  hdiscr_csvSimple->Fill(discr_csvSimple[ij],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ij])==5)hdiscr_csvSimpleB->Fill(discr_csvSimple[ij],w);    
	    else if(abs(refparton_flavorForB[ij])==4)hdiscr_csvSimpleC->Fill(discr_csvSimple[ij],w);    
	    else if(abs(refparton_flavorForB[ij])<99)hdiscr_csvSimpleL->Fill(discr_csvSimple[ij],w);    
	  }
	  
	  hdiscr_prob->Fill(discr_probb[ij],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ij])==5)hdiscr_probB->Fill(discr_probb[ij],w); 
	    else if(abs(refparton_flavorForB[ij])==4)hdiscr_probC->Fill(discr_probb[ij],w);    
	    else if(abs(refparton_flavorForB[ij])<99)hdiscr_probL->Fill(discr_probb[ij],w);    
	  }
	  
	  hdiscr_ssvHighEff->Fill(discr_ssvHighEff[ij],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ij])==5)hdiscr_ssvHighEffB->Fill(discr_ssvHighEff[ij],w); 
	    else if(abs(refparton_flavorForB[ij])==4)hdiscr_ssvHighEffC->Fill(discr_ssvHighEff[ij],w);    
	    else if(abs(refparton_flavorForB[ij])<99)hdiscr_ssvHighEffL->Fill(discr_ssvHighEff[ij],w);    
	  }
	  
	  hdiscr_ssvHighPur->Fill(discr_ssvHighPur[ij],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ij])==5)hdiscr_ssvHighPurB->Fill(discr_ssvHighPur[ij],w); 
	    else if(abs(refparton_flavorForB[ij])==4)hdiscr_ssvHighPurC->Fill(discr_ssvHighPur[ij],w);    
	    else if(abs(refparton_flavorForB[ij])<99)hdiscr_ssvHighPurL->Fill(discr_ssvHighPur[ij],w);    
	  }
	  //*
	  hnsvtx->Fill(nsvtx[ij],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ij])==5)hnsvtxB->Fill(nsvtx[ij],w);    
	    else if(abs(refparton_flavorForB[ij])==4)hnsvtxC->Fill(nsvtx[ij],w);    
	    else if(abs(refparton_flavorForB[ij])<99)hnsvtxL->Fill(nsvtx[ij],w); 
	  }
	  
	  if(nsvtx[ij]>0){
	    
	    hsvtxntrk->Fill(svtxntrk[ij],w);    
	    if(isMC){
	      if(abs(refparton_flavorForB[ij])==5)hsvtxntrkB->Fill(svtxntrk[ij],w);    
	      else if(abs(refparton_flavorForB[ij])==4)hsvtxntrkC->Fill(svtxntrk[ij],w);
	      else if(abs(refparton_flavorForB[ij])<99)hsvtxntrkL->Fill(svtxntrk[ij],w);
	    }
	    
	    // require at least 1 tracks as in btagging @ 7 TeV note
	    if(svtxntrk[ij]>1){	  
	      
	      hsvtxdl->Fill(svtxdl[ij],w);    
	      if(isMC){
		if(abs(refparton_flavorForB[ij])==5)hsvtxdlB->Fill(svtxdl[ij],w);    
		else if(abs(refparton_flavorForB[ij])==4)hsvtxdlC->Fill(svtxdl[ij],w);
		else if(abs(refparton_flavorForB[ij])<99)hsvtxdlL->Fill(svtxdl[ij],w);
	      }
	      
	      hsvtxdls->Fill(svtxdls[ij],w);    
	      if(isMC){
		if(abs(refparton_flavorForB[ij])==5)hsvtxdlsB->Fill(svtxdls[ij],w);    
		else if(abs(refparton_flavorForB[ij])==4)hsvtxdlsC->Fill(svtxdls[ij],w); 
		else if(abs(refparton_flavorForB[ij])<99)hsvtxdlsL->Fill(svtxdls[ij],w); 
	      }
	      
	      hsvtxm->Fill(svtxm[ij],w);    
	      if(isMC){
		if(abs(refparton_flavorForB[ij])==5)hsvtxmB->Fill(svtxm[ij],w);    
		else if(abs(refparton_flavorForB[ij])==4)hsvtxmC->Fill(svtxm[ij],w);
		else if(abs(refparton_flavorForB[ij])<99)hsvtxmL->Fill(svtxm[ij],w); 
	      }

	      hsvtxpt->Fill(svtxpt[ij],w);    
	      if(isMC){
		if(abs(refparton_flavorForB[ij])==5)hsvtxptB->Fill(svtxpt[ij],w);    
		else if(abs(refparton_flavorForB[ij])==4)hsvtxptC->Fill(svtxpt[ij],w);
		else if(abs(refparton_flavorForB[ij])<99)hsvtxptL->Fill(svtxpt[ij],w);
	      }
	      
	      if(svtxntrk[ij]>=3) {

		hsvtxmSV3->Fill(svtxm[ij],w);    
		if(isMC){
		  if(abs(refparton_flavorForB[ij])==5)hsvtxmSV3B->Fill(svtxm[ij],w);    
		  else if(abs(refparton_flavorForB[ij])==4)hsvtxmSV3C->Fill(svtxm[ij],w);
		  else if(abs(refparton_flavorForB[ij])<99)hsvtxmSV3L->Fill(svtxm[ij],w); 
		}

		hsvtxptSV3->Fill(svtxpt[ij],w);    
		if(isMC){
		  if(abs(refparton_flavorForB[ij])==5)hsvtxptSV3B->Fill(svtxpt[ij],w);    
		  else if(abs(refparton_flavorForB[ij])==4)hsvtxptSV3C->Fill(svtxpt[ij],w);
		  else if(abs(refparton_flavorForB[ij])<99)hsvtxptSV3L->Fill(svtxpt[ij],w);
		}
	      }
	    }
	  }
	  
	  hnIPtrk->Fill(nIPtrk[ij],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ij])==5)hnIPtrkB->Fill(nIPtrk[ij],w);    
	    else if(abs(refparton_flavorForB[ij])==4)hnIPtrkC->Fill(nIPtrk[ij],w);    
	    else if(abs(refparton_flavorForB[ij])<99)hnIPtrkL->Fill(nIPtrk[ij],w);    
	  }
	  
	  hnselIPtrk->Fill(nselIPtrk[ij],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ij])==5)hnselIPtrkB->Fill(nselIPtrk[ij],w);    
	    else if(abs(refparton_flavorForB[ij])==4)hnselIPtrkC->Fill(nselIPtrk[ij],w);
	    else if(abs(refparton_flavorForB[ij])<99)hnselIPtrkL->Fill(nselIPtrk[ij],w);
	  }

	  if (sqrt(acos(cos(jtphi[ij]-muphi[ij]))*acos(cos(jtphi[ij]-muphi[ij]))+(jteta[ij]-mueta[ij])*(jteta[ij]-mueta[ij]))<0.5 && mupt[ij]>minMuPt) {  

	    hmuptrel->Fill(muptrel[ij],w);    
	    if(isMC){
	      if(abs(refparton_flavorForB[ij])==5)hmuptrelB->Fill(muptrel[ij],w);
	      else if(abs(refparton_flavorForB[ij])==4)hmuptrelC->Fill(muptrel[ij],w);
	      else if(abs(refparton_flavorForB[ij])<99)hmuptrelL->Fill(muptrel[ij],w);
	    }

	    if(svtxntrk[ij]>=2) {  
	      hmuptrelSV2->Fill(muptrel[ij],w);    
	      if(isMC){
		if(abs(refparton_flavorForB[ij])==5)hmuptrelSV2B->Fill(muptrel[ij],w);
		else if(abs(refparton_flavorForB[ij])==4)hmuptrelSV2C->Fill(muptrel[ij],w);
		else if(abs(refparton_flavorForB[ij])<99)hmuptrelSV2L->Fill(muptrel[ij],w);
	      }
	    }

	    if(svtxntrk[ij]>=3) {  
	      hmuptrelSV3->Fill(muptrel[ij],w);    
	      if(isMC){
		if(abs(refparton_flavorForB[ij])==5)hmuptrelSV3B->Fill(muptrel[ij],w);
		else if(abs(refparton_flavorForB[ij])==4)hmuptrelSV3C->Fill(muptrel[ij],w);
		else if(abs(refparton_flavorForB[ij])<99)hmuptrelSV3L->Fill(muptrel[ij],w);
	      }
	    }
	    
	  }

	  //*/
	}

      }

    }
    //*
    if(doTracks){
      
      for(int it=0;it<nIP;it++){
	
	int ijet = ipJetIndex[it];
	
	if(jtpt[ijet]>minJetPt && fabs(jteta[ijet])<maxJetEta){
	  
	  hipPt->Fill(ipPt[it],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ijet])==5)hipPtB->Fill(ipPt[it],w);
	    else if(abs(refparton_flavorForB[ijet])==4)hipPtC->Fill(ipPt[it],w); 
	    else if(abs(refparton_flavorForB[ijet])<99)hipPtL->Fill(ipPt[it],w); 
	  }
	  
	  hipProb0->Fill(ipProb0[it],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ijet])==5)hipProb0B->Fill(ipProb0[it],w);
	    else if(abs(refparton_flavorForB[ijet])==4)hipProb0C->Fill(ipProb0[it],w);
	    else if(abs(refparton_flavorForB[ijet])<99)hipProb0L->Fill(ipProb0[it],w);
	  }
	  
	  hipProb1->Fill(ipProb1[it],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ijet])==5)hipProb1B->Fill(ipProb1[it],w);
	    else if(abs(refparton_flavorForB[ijet])==4)hipProb1C->Fill(ipProb1[it],w);
	    else if(abs(refparton_flavorForB[ijet])<99)hipProb1L->Fill(ipProb1[it],w);
	  }

	  hip2d->Fill(ip2d[it],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ijet])==5)hip2dB->Fill(ip2d[it],w);
	    else if(abs(refparton_flavorForB[ijet])==4)hip2dC->Fill(ip2d[it],w); 
	    else if(abs(refparton_flavorForB[ijet])<99)hip2dL->Fill(ip2d[it],w); 
	  }

	  hip2dSig->Fill(ip2dSig[it],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ijet])==5)hip2dSigB->Fill(ip2dSig[it],w);
	    else if(abs(refparton_flavorForB[ijet])==4)hip2dSigC->Fill(ip2dSig[it],w);
	    else if(abs(refparton_flavorForB[ijet])<99)hip2dSigL->Fill(ip2dSig[it],w);
	  }

	  hip3d->Fill(ip3d[it],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ijet])==5)hip3dB->Fill(ip3d[it],w);
	    else if(abs(refparton_flavorForB[ijet])==4)hip3dC->Fill(ip3d[it],w); 
	    else if(abs(refparton_flavorForB[ijet])<99)hip3dL->Fill(ip3d[it],w); 
	  }

	  hip3dSig->Fill(ip3dSig[it],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ijet])==5)hip3dSigB->Fill(ip3dSig[it],w);
	    else if(abs(refparton_flavorForB[ijet])==4)hip3dSigC->Fill(ip3dSig[it],w);
	    else if(abs(refparton_flavorForB[ijet])<99)hip3dSigL->Fill(ip3dSig[it],w);
	  }

	  hipDist2Jet->Fill(ipDist2Jet[it],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ijet])==5)hipDist2JetB->Fill(ipDist2Jet[it],w);
	    else if(abs(refparton_flavorForB[ijet])==4)hipDist2JetC->Fill(ipDist2Jet[it],w);
	    else if(abs(refparton_flavorForB[ijet])<99)hipDist2JetL->Fill(ipDist2Jet[it],w);
	  }

	  hipDist2JetSig->Fill(ipDist2JetSig[it],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ijet])==5)hipDist2JetSigB->Fill(ipDist2JetSig[it],w);
	    else if(abs(refparton_flavorForB[ijet])==4)hipDist2JetSigC->Fill(ipDist2JetSig[it],w);
	    else if(abs(refparton_flavorForB[ijet])<99)hipDist2JetSigL->Fill(ipDist2JetSig[it],w);
	  }

	  hipClosest2Jet->Fill(ipClosest2Jet[it],w);    
	  if(isMC){
	    if(abs(refparton_flavorForB[ijet])==5)hipClosest2JetB->Fill(ipClosest2Jet[it],w);
	    else if(abs(refparton_flavorForB[ijet])==4)hipClosest2JetC->Fill(ipClosest2Jet[it],w);
	    else if(abs(refparton_flavorForB[ijet])<99)hipClosest2JetL->Fill(ipClosest2Jet[it],w);
	  }

	}
	
      }
    
    }
    //*/
  }

  hjtpt->Write();
  if(isMC) hjtptB->Write(); hjtptC->Write(); hjtptL->Write(); hjtptU->Write();

  hdiscr_csvSimple->Write();
  if(isMC) hdiscr_csvSimpleB->Write(); hdiscr_csvSimpleC->Write(); hdiscr_csvSimpleL->Write(); 

  hdiscr_prob->Write();
  if(isMC) hdiscr_probB->Write(); hdiscr_probC->Write(); hdiscr_probL->Write(); 

  hdiscr_ssvHighEff->Write();
  if(isMC) hdiscr_ssvHighEffB->Write(); hdiscr_ssvHighEffC->Write(); hdiscr_ssvHighEffL->Write(); 

  hdiscr_ssvHighPur->Write();
  if(isMC) hdiscr_ssvHighPurB->Write(); hdiscr_ssvHighPurC->Write(); hdiscr_ssvHighPurL->Write(); 

  hnsvtx->Write();
  if(isMC) hnsvtxB->Write(); hnsvtxC->Write(); hnsvtxL->Write();

  hsvtxntrk->Write();
  if(isMC) hsvtxntrkB->Write();hsvtxntrkC->Write(); hsvtxntrkL->Write();
  
  hsvtxdl->Write();
  if(isMC) hsvtxdlB->Write(); hsvtxdlC->Write(); hsvtxdlL->Write();

  hsvtxdls->Write();
  if(isMC) hsvtxdlsB->Write(); hsvtxdlsC->Write(); hsvtxdlsL->Write();

  hsvtxm->Write();
  if(isMC) hsvtxmB->Write(); hsvtxmC->Write(); hsvtxmL->Write();

  hsvtxmSV3->Write();
  if(isMC) hsvtxmSV3B->Write(); hsvtxmSV3C->Write(); hsvtxmSV3L->Write();

  hsvtxpt->Write();
  if(isMC) hsvtxptB->Write(); hsvtxptC->Write(); hsvtxptL->Write();

  hsvtxptSV3->Write();
  if(isMC) hsvtxptSV3B->Write(); hsvtxptSV3C->Write(); hsvtxptSV3L->Write();

  hnIPtrk->Write();
  if(isMC) hnIPtrkB->Write(); hnIPtrkC->Write(); hnIPtrkL->Write();

  hnselIPtrk->Write();
  if(isMC) hnselIPtrkB->Write(); hnselIPtrkC->Write(); hnselIPtrkL->Write();

  hmuptrel->Write();
  if(isMC) hmuptrelB->Write(); hmuptrelC->Write(); hmuptrelL->Write();

  hmuptrelSV2->Write();
  if(isMC) hmuptrelSV2B->Write(); hmuptrelSV2C->Write(); hmuptrelSV2L->Write();

  hmuptrelSV3->Write();
  if(isMC) hmuptrelSV3B->Write(); hmuptrelSV3C->Write(); hmuptrelSV3L->Write();

  hipPt->Write();
  if(isMC) hipPtB->Write(); hipPtC->Write(); hipPtL->Write();

  hipProb0->Write();
  if(isMC) hipProb0B->Write(); hipProb0C->Write(); hipProb0L->Write();

  hipProb1->Write();
  if(isMC) hipProb1B->Write(); hipProb1C->Write(); hipProb1L->Write();

  hip2d->Write();
  if(isMC) hip2dB->Write(); hip2dC->Write(); hip2dL->Write();

  hip2dSig->Write();
  if(isMC) hip2dSigB->Write(); hip2dSigC->Write(); hip2dSigL->Write();

  hip3d->Write();
  if(isMC) hip3dB->Write(); hip3dC->Write(); hip3dL->Write();

  hip3dSig->Write();
  if(isMC) hip3dSigB->Write(); hip3dSigC->Write(); hip3dSigL->Write();

  hipDist2Jet->Write();
  if(isMC) hipDist2JetB->Write(); hipDist2JetC->Write(); hipDist2JetL->Write();

  hipDist2JetSig->Write();
  if(isMC) hipDist2JetSigB->Write(); hipDist2JetSigC->Write(); hipDist2JetSigL->Write();

  hipClosest2Jet->Write();
  if(isMC) hipClosest2JetB->Write(); hipClosest2JetC->Write(); hipClosest2JetL->Write();

  nt->Write();
  
  fout->Close();

}
