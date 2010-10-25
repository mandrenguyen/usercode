#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TTree.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "MNguyen/PFCandAnalyzer/interface/TreePFCandEventData.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityBins.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

//
// class declaration
//

class PFCandAnalyzer : public edm::EDAnalyzer {
  public:
    explicit PFCandAnalyzer(const edm::ParameterSet&);
    ~PFCandAnalyzer();

    // class methods


  private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    // ----------member data ---------------------------
    edm::Service<TFileService> fs;

    // === Ana setup ===

    // Event Info
    edm::InputTag pfCandidateLabel_;
    edm::InputTag genLabel_;
    edm::InputTag jetLabel_;

    TTree	  *pfTree_;
    TreePFCandEventData pfEvt_;

    // cuts
    Double_t        pfPtMin_;
    Double_t        jetPtMin_;
    Double_t        genPtMin_;

   // debug
    Int_t	  verbosity_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
