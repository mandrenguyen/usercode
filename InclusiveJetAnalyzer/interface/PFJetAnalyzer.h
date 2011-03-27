#ifndef MNguyen_InclusiveJetAnalyzer_PFJetAnalyzer_
#define MNguyen_InclusiveJetAnalyzer_PFJetAnalyzer_

// system include files
#include <memory>
#include <string>
#include <iostream>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"



#include "TFile.h"
#include "TTree.h"
#include "TH1.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"


#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

//

/**\class PFJetAnalyzer 

\author Matt Nguyen
\date   November 2010
*/




class PFJetAnalyzer : public edm::EDAnalyzer {
 public:

  explicit PFJetAnalyzer(const edm::ParameterSet&);

  ~PFJetAnalyzer();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  virtual void beginRun(const edm::Run & r, const edm::EventSetup & c);

  virtual void beginJob();


  void getPartons( const edm::Event & iEvent, const edm::EventSetup & iEs );

 private:
  


  edm::InputTag  pfCandidatesTag_, trackTag_, simTracksTag_, genParticleTag_;
  edm::InputTag jetTag_, jetTag2_, jetTag3_, jetTag4_;
  edm::InputTag recoJetTag_, recoJetTag2_, recoJetTag3_, recoJetTag4_;

  /// verbose ?
  bool   verbose_;

  bool useCentrality_;
  bool isMC_;

  bool hasSimInfo_;

  TTree *t;
  edm::Service<TFileService> fs2;

  CentralityProvider * centrality_;

  static const int MAXJETS = 25000;
  static const int MAXPFCANDS = 25000;
  static const int MAXTRACKS = 25000;
  

  struct JRA{
    
    int nj1;
    int nj2;
    int nj3;
    int nj4;
    int run;
    int evt;
    int lumi;
    int bin;
    float b;
    float vx, vy, vz;
    float hf;

    float rawpt_j1[MAXJETS];
    float jtpt_j1[MAXJETS];
    float refpt_j1[MAXJETS];
    float jteta_j1[MAXJETS];
    float refeta_j1[MAXJETS];
    float jtphi_j1[MAXJETS];
    float refphi_j1[MAXJETS];
    float jty_j1[MAXJETS];
    float refy_j1[MAXJETS];
    float refdrjt_j1[MAXJETS];
    float preL1et_j1[MAXJETS];
    float L2_j1[MAXJETS];
    float L3_j1[MAXJETS];
    float area_j1[MAXJETS];


    float rawpt_j2[MAXJETS];
    float jtpt_j2[MAXJETS];
    float refpt_j2[MAXJETS];
    float jteta_j2[MAXJETS];
    float refeta_j2[MAXJETS];
    float jtphi_j2[MAXJETS];
    float refphi_j2[MAXJETS];
    float jty_j2[MAXJETS];
    float refy_j2[MAXJETS];
    float refdrjt_j2[MAXJETS];
    float preL1et_j2[MAXJETS];
    float L2_j2[MAXJETS];
    float L3_j2[MAXJETS];
    float area_j2[MAXJETS];



    float rawpt_j4[MAXJETS];
    float jtpt_j4[MAXJETS];
    float refpt_j4[MAXJETS];
    float jteta_j4[MAXJETS];
    float refeta_j4[MAXJETS];
    float jtphi_j4[MAXJETS];
    float refphi_j4[MAXJETS];
    float jty_j4[MAXJETS];
    float refy_j4[MAXJETS];
    float refdrjt_j4[MAXJETS];
    float preL1et_j4[MAXJETS];
    float L2_j4[MAXJETS];
    float L3_j4[MAXJETS];
    float area_j4[MAXJETS];

    float rawpt_j3[MAXJETS];
    float jtpt_j3[MAXJETS];
    float refpt_j3[MAXJETS];
    float jteta_j3[MAXJETS];
    float refeta_j3[MAXJETS];
    float jtphi_j3[MAXJETS];
    float refphi_j3[MAXJETS];
    float jty_j3[MAXJETS];
    float refy_j3[MAXJETS];
    float refdrjt_j3[MAXJETS];
    float preL1et_j3[MAXJETS];
    float L2_j3[MAXJETS];
    float L3_j3[MAXJETS];
    float area_j3[MAXJETS];

    int nPFcand;
    int candID[MAXPFCANDS];
    float candpt[MAXPFCANDS];
    float candeta[MAXPFCANDS];
    float candphi[MAXPFCANDS];
    float candy[MAXPFCANDS];

    int ntrack;
    int tracknhits[MAXTRACKS];
    float trackpt[MAXTRACKS];
    float tracketa[MAXTRACKS];
    float trackphi[MAXTRACKS];
    float tracksumecal[MAXTRACKS];
    float tracksumhcal[MAXTRACKS];
    int trackfake[MAXTRACKS];
    int trackqual[MAXTRACKS];

    int parton1_flavor, parton2_flavor;
    float parton1_pt, parton2_pt;
    float parton1_phi, parton2_phi;
    float parton1_eta, parton2_eta;
    float parton1_y, parton2_y;
  };

  JRA jets_;
};

#endif
