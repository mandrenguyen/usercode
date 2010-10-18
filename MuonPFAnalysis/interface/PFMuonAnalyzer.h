#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "TH1D.h"
#include "TH2D.h"
#include "TH3D.h"
#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"

const Int_t NRECOSEL = 4;
const Int_t NPFSEL = 8;

//
// class declaration
//

class PFMuonAnalyzer : public edm::EDAnalyzer {
  public:
    explicit PFMuonAnalyzer(const edm::ParameterSet&);
    ~PFMuonAnalyzer();

    // class methods


  private:
    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    bool tightMuonPOG(const reco::MuonRef& muonRef,  const edm::Handle<reco::BeamSpot>& beamspot);
    bool isGlobalTightMuon( const reco::MuonRef& muonRef );
    bool isTrackerTightMuon( const reco::MuonRef& muonRef );
    bool isGlobalLooseMuon( const reco::MuonRef& muonRef );
    bool isTrackerLooseMuon( const reco::MuonRef& muonRef );
    
    // ----------member data ---------------------------
    edm::Service<TFileService> fs;

    // === Ana setup ===

    // Event Info
    edm::InputTag pfCandidateLabel_;
    edm::InputTag recoMuonLabel_;
    edm::InputTag beamSpotLabel_;
    edm::InputTag genLabel_;


    //TString recoSel[NRECOSEL];
    //TString pfSel[NPFSEL];

    // 1d histograms
    TH1D *hNGenMuons_;
    TH1D *hNRecoMuons_;

   // 2d histograms
    TH2D *hRecoMuon_Pt_vs_Eta_[NRECOSEL];
    TH2D *hPFMuon_Pt_vs_Eta_[NPFSEL];
    TH2D *hGenMuon_Pt_vs_Eta_;

    TH2D *hRecoMuonGenDeltaR_vs_GenPt_[NRECOSEL];
    TH2D *hPFMuonGenDeltaR_vs_GenPt_[NPFSEL];

    TH2D *hRecoMuon_DeltaPtOverPt_vs_GenPt_[NRECOSEL];
    TH2D *hPFMuon_DeltaPtOverPt_vs_GenPt_[NPFSEL];

    TH2D *hRecoMuonGenMatched_RecPt_RecEta_[NRECOSEL];
    TH2D *hPFMuonGenMatched_RecPt_RecEta_[NPFSEL];

    TH2D *hRecoMuonNotGenMatched_RecPt_RecEta_[NRECOSEL];
    TH2D *hPFMuonNotGenMatched_RecPt_RecEta_[NPFSEL];

   // 3d histograms   
    TH3D *hRecoMuon_RecPt_vs_GenPt_vs_GenEta_[NRECOSEL];
    TH3D *hPFMuon_RecPt_vs_GenPt_vs_GenEta_[NPFSEL];


   // debug
    Int_t	  verbosity_;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//
