#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "TTree.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityBins.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"

//
// class declaration
//

class PFTowerAnalyzer : public edm::EDAnalyzer {
  public:
    explicit PFTowerAnalyzer(const edm::ParameterSet&);
    ~PFTowerAnalyzer();

    // class methods


  private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;
    
    void FillEventInfo(const edm::Event&, const edm::EventSetup&);

    // ----------member data ---------------------------
    edm::Service<TFileService> fs;

    CentralityProvider * centrality_;

    // === Ana setup ===

    // Event Info
    edm::InputTag pfTowerLabel_;

    TTree	  *pfTowerTree;

    // cuts
    Double_t        pfPtMin_;

   // debug
    Int_t	  verbosity_;


    static const int MAXTOWERS = 5000;

    struct PFTOWER{
      int run;
      int evt;
      int lumi;
      int bin;
      float vx, vy, vz;
      int nPFTowers;
      float eta[MAXTOWERS], phi[MAXTOWERS], pt[MAXTOWERS];
      int ieta[MAXTOWERS], iphi[MAXTOWERS];
    };

    PFTOWER pfTower;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
