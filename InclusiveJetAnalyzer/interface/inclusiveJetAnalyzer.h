#ifndef MNguyen_InclusiveJetAnalyzer_inclusiveJetAnalyzer_
#define MNguyen_InclusiveJetAnalyzer_inclusiveJetAnalyzer_

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

//

/**\class InclusiveJetAnalyzer 

\author Matt Nguyen
\date   November 2010
*/




class InclusiveJetAnalyzer : public edm::EDAnalyzer {
 public:

  explicit InclusiveJetAnalyzer(const edm::ParameterSet&);

  ~InclusiveJetAnalyzer();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  virtual void beginRun(const edm::Run & r, const edm::EventSetup & c);

  virtual void beginJob();

 private:
  


  edm::InputTag   jetTag_;



  /// verbose ?
  bool   verbose_;

  bool useCentrality_;
  bool isMC_;


  TTree *t;
  edm::Service<TFileService> f;

  CentralityProvider * centrality_;

  static const int MAXJETS = 5000;
  

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
    
  };

  JRA jets_;

};

#endif
