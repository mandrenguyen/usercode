#ifndef MNguyen_ElectronAnalyzer_electronAnalyzer_
#define MNguyen_ElectronAnalyzer_electronAnalyzer_

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
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "TFile.h"
#include "TTree.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"

//

/**\class ElectronAnalyzer 

\author Matt Nguyen
\date   December 2011
*/




class ElectronAnalyzer : public edm::EDAnalyzer {
 public:

  explicit ElectronAnalyzer(const edm::ParameterSet&);

  ~ElectronAnalyzer();
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);

  virtual void beginRun(const edm::Run & r, const edm::EventSetup & c);

  virtual void beginJob();

  void fillL1Bits(const edm::Event &iEvent);

  void fillHLTBits(const edm::Event &iEvent);

  template <typename TYPE>
    void                          getProduct(const std::string name, edm::Handle<TYPE> &prod,
					     const edm::Event &event) const;    
  template <typename TYPE>
    bool                          getProductSafe(const std::string name, edm::Handle<TYPE> &prod,
						 const edm::Event &event) const;
  

 private:
  


  edm::InputTag   eleTag_, eventInfoTag_, L1gtReadout_, vtxTag_, mcTag_; 


  /// verbose ?
  bool   verbose_;

  bool useVtx_;
  bool useCentrality_;
  bool isMC_;


  TTree *t;
  edm::Service<TFileService> fs1;

  CentralityProvider * centrality_;



  std::string                   hltResName_;         //HLT trigger results name
  std::vector<std::string>      hltProcNames_;       //HLT process name(s)
  std::vector<std::string>      hltTrgNames_;        //HLT trigger name(s)

  std::vector<int>              hltTrgBits_;         //HLT trigger bit(s)
  std::vector<bool>             hltTrgDeci_;         //HLT trigger descision(s)
  std::vector<std::string>      hltTrgUsedNames_;    //HLT used trigger name(s)
  std::string                   hltUsedResName_;     //used HLT trigger results name



  static const int MAXELES = 500;
  static const int MAXHLTBITS = 5000;


  struct ELE{
    
    int nref;
    int run;
    int evt;
    int lumi;
    int bin;
    float vx, vy, vz;
    float b;
    float hf;

    double ee[MAXELES];
    double ep[MAXELES];
    double ept[MAXELES];
    double eeta[MAXELES];
    double ephi[MAXELES];
    int echarge[MAXELES];
    double hOe[MAXELES];
    double sigmaietaieta[MAXELES];
    double eseedOpout[MAXELES];
    double eOp[MAXELES];
    double eseedOp[MAXELES];
    double eeleOpout[MAXELES];
    double deltaetain[MAXELES];
    double deltaphiin[MAXELES];
    double sigmaetaeta[MAXELES];
    double e15[MAXELES];
    double e25max[MAXELES];
    double e55[MAXELES];
    double fbrem[MAXELES];
    double mva[MAXELES];
    int isbarrel[MAXELES];
    int isendcap[MAXELES];

    double pthat;
    
    // MC Truth Matching
    int isMCEle[MAXELES];
    int isMCPhoton[MAXELES];
    int isMCHadron[MAXELES];
    int idPDGMatch[MAXELES];
    double MC_chosenEle_PoP_pt[MAXELES];
    double MC_chosenEle_PoP_phi[MAXELES];
    double MC_chosenEle_PoP_eta[MAXELES];
    double MC_chosenEle_PoP_e[MAXELES];
    double MC_chosenPho_PoP_pt[MAXELES];
    double MC_chosenPho_PoP_phi[MAXELES];
    double MC_chosenPho_PoP_eta[MAXELES];
    double MC_chosenPho_PoP_e[MAXELES];
    double MC_chosenHad_PoP_pt[MAXELES];
    double MC_chosenHad_PoP_phi[MAXELES];
    double MC_chosenHad_PoP_eta[MAXELES];
    double MC_chosenHad_PoP_e[MAXELES];
    double MC_closest_DR_pt[MAXELES];
    double MC_closest_DR_phi[MAXELES];
    double MC_closest_DR_eta[MAXELES];
    double MC_closest_DR_e[MAXELES];


    // hlt
    int nHLTBit;
    bool hltBit[MAXHLTBITS];

    // l1
    int nL1TBit;
    bool l1TBit[MAXHLTBITS];
    int nL1ABit;
    bool l1ABit[MAXHLTBITS];

  };

  ELE ele_;

};

#endif
