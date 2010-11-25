#ifndef MNguyen_PFJetAnalyzer_PFJetAnalyzer_
#define MNguyen_PFJetAnalyzer_PFJetAnalyzer_

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
  


  edm::InputTag   jetTag_, pfCandidatesTag_, recoJetTag_;



  /// verbose ?
  bool   verbose_;

  bool useCentrality_;
  bool isMC_;


  TTree *t;
  edm::Service<TFileService> f;

  CentralityProvider * centrality_;

  static const int MAXJETS = 5000;
  static const int MAXPFCANDS = 5000;
  

  struct JRA{
    
    unsigned char nref;
    int run;
    int evt;
    int bin;
    float b;
    float hf;
    float rawpt[MAXJETS];
    float jtpt[MAXJETS];
    float refpt[MAXJETS];
    float jteta[MAXJETS];
    float refeta[MAXJETS];
    float jtphi[MAXJETS];
    float refphi[MAXJETS];
    float jty[MAXJETS];
    float refy[MAXJETS];
    float refdrjt[MAXJETS];
    float preL1et[MAXJETS];
    float L2[MAXJETS];
    float L3[MAXJETS];
    
    int nPFcand;
    int candID[MAXPFCANDS];
    float candpt[MAXPFCANDS];
    float candeta[MAXPFCANDS];
    float candphi[MAXPFCANDS];
    float candy[MAXPFCANDS];
    
  };

  JRA jets_;

};

#endif
