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

 private:
  


  edm::InputTag  pfCandidatesTag_, trackTag_;
  edm::InputTag jetTag_, jetTag2_, jetTag3_, jetTag4_;
  edm::InputTag recoJetTag_, recoJetTag2_, recoJetTag3_, recoJetTag4_;

  /// verbose ?
  bool   verbose_;

  bool useCentrality_;
  bool isMC_;


  TTree *t;
  edm::Service<TFileService> fs2;

  CentralityProvider * centrality_;

  static const int MAXJETS = 25000;
  static const int MAXPFCANDS = 25000;
  static const int MAXTRACKS = 25000;
  

  struct JRA{
    
    int nicPu5;
    int nic5;
    int nic5FJ;
    int nak5;
    int run;
    int evt;
    int lumi;
    int bin;
    float b;
    float vx, vy, vz;
    float hf;

    float rawpt_icPu5[MAXJETS];
    float jtpt_icPu5[MAXJETS];
    float refpt_icPu5[MAXJETS];
    float jteta_icPu5[MAXJETS];
    float refeta_icPu5[MAXJETS];
    float jtphi_icPu5[MAXJETS];
    float refphi_icPu5[MAXJETS];
    float jty_icPu5[MAXJETS];
    float refy_icPu5[MAXJETS];
    float refdrjt_icPu5[MAXJETS];
    float preL1et_icPu5[MAXJETS];
    float L2_icPu5[MAXJETS];
    float L3_icPu5[MAXJETS];
    float area_icPu5[MAXJETS];


    float rawpt_ic5[MAXJETS];
    float jtpt_ic5[MAXJETS];
    float refpt_ic5[MAXJETS];
    float jteta_ic5[MAXJETS];
    float refeta_ic5[MAXJETS];
    float jtphi_ic5[MAXJETS];
    float refphi_ic5[MAXJETS];
    float jty_ic5[MAXJETS];
    float refy_ic5[MAXJETS];
    float refdrjt_ic5[MAXJETS];
    float preL1et_ic5[MAXJETS];
    float L2_ic5[MAXJETS];
    float L3_ic5[MAXJETS];
    float area_ic5[MAXJETS];



    float rawpt_ak5[MAXJETS];
    float jtpt_ak5[MAXJETS];
    float refpt_ak5[MAXJETS];
    float jteta_ak5[MAXJETS];
    float refeta_ak5[MAXJETS];
    float jtphi_ak5[MAXJETS];
    float refphi_ak5[MAXJETS];
    float jty_ak5[MAXJETS];
    float refy_ak5[MAXJETS];
    float refdrjt_ak5[MAXJETS];
    float preL1et_ak5[MAXJETS];
    float L2_ak5[MAXJETS];
    float L3_ak5[MAXJETS];
    float area_ak5[MAXJETS];

    float rawpt_ic5FJ[MAXJETS];
    float jtpt_ic5FJ[MAXJETS];
    float refpt_ic5FJ[MAXJETS];
    float jteta_ic5FJ[MAXJETS];
    float refeta_ic5FJ[MAXJETS];
    float jtphi_ic5FJ[MAXJETS];
    float refphi_ic5FJ[MAXJETS];
    float jty_ic5FJ[MAXJETS];
    float refy_ic5FJ[MAXJETS];
    float refdrjt_ic5FJ[MAXJETS];
    float preL1et_ic5FJ[MAXJETS];
    float L2_ic5FJ[MAXJETS];
    float L3_ic5FJ[MAXJETS];
    float area_ic5FJ[MAXJETS];

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


  };

  JRA jets_;
};

#endif
