// -*- C++ -*-
//
// Package:    PFCandAnalyzer
// Class:      PFCandAnalyzer
// 
/**\class PFCandAnalyzer PFCandAnalyzer.cc ana/PFCandAnalyzer/src/PFCandAnalyzer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
 */
//
// Original Author:  Matt, Nguyen 
//         Created:   Oct  10 2010
// 
//
//


// system include files
#include <memory>

// stl
#include <algorithm>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

// ana
#include "MNguyen/PFCandAnalyzer/interface/PFCandAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "RecoJets/JetAlgorithms/interface/JetAlgoHelper.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"
#include "TStopwatch.h"

#include "MNguyen/PFCandAnalyzer/interface/TreePFCandEventData.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"


using namespace std;
using namespace edm;
using namespace reco;

//
// constructors and destructor
//
PFCandAnalyzer::PFCandAnalyzer(const edm::ParameterSet& iConfig)
{
  // Event source
  // Event Info
  pfCandidateLabel_ = iConfig.getParameter<edm::InputTag>("pfCandidateLabel");
  genLabel_ = iConfig.getParameter<edm::InputTag>("genLabel");
  jetLabel_ = iConfig.getParameter<edm::InputTag>("jetLabel");

  pfPtMin_ = iConfig.getParameter<double>("pfPtMin");
  genPtMin_ = iConfig.getParameter<double>("genPtMin");
  jetPtMin_ = iConfig.getParameter<double>("jetPtMin");

  // debug
  verbosity_ = iConfig.getUntrackedParameter<int>("verbosity", 0);



}


PFCandAnalyzer::~PFCandAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
  void
PFCandAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  pfEvt_.Clear();

  // Fill PF info

  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  iEvent.getByLabel(pfCandidateLabel_,pfCandidates);  
  const reco::PFCandidateCollection *pfCandidateColl = &(*pfCandidates);


  for(unsigned icand=0;icand<pfCandidateColl->size(); icand++) {
      const reco::PFCandidate pfCandidate = pfCandidateColl->at(icand);
      
      double pt =  pfCandidate.pt();
      
      if(pt>pfPtMin_){
	pfEvt_.pfId_[pfEvt_.nPFpart_] = pfCandidate.particleId();      
	pfEvt_.pfPt_[pfEvt_.nPFpart_] = pt;      
	pfEvt_.pfEta_[pfEvt_.nPFpart_] = pfCandidate.eta();      
	pfEvt_.pfPhi_[pfEvt_.nPFpart_] = pfCandidate.phi();      
	pfEvt_.nPFpart_++;
      }
  }
	

  // Fill GEN info
  edm::Handle<reco::GenParticleCollection> genParticles;
  iEvent.getByLabel(genLabel_,genParticles);     
  const reco::GenParticleCollection* genColl= &(*genParticles);

  for(unsigned igen=0;igen<genColl->size(); igen++) {    

    const reco::GenParticle gen = genColl->at(igen);    
    double eta = gen.eta();      
    double pt = gen.pt();      

    if(gen.status()==1 && fabs(eta)<3.0 && pt> genPtMin_){      
      pfEvt_.genPDGId_[pfEvt_.nGENpart_] = gen.pdgId();      
      pfEvt_.genPt_[pfEvt_.nGENpart_] = pt;      
      pfEvt_.genEta_[pfEvt_.nGENpart_] = eta;      
      pfEvt_.genPhi_[pfEvt_.nGENpart_] = gen.phi();      
      pfEvt_.nGENpart_++;
    }
  }

    // Fill Jet info
  
  edm::Handle<pat::JetCollection> jets;
  iEvent.getByLabel(jetLabel_,jets);  
  const pat::JetCollection *jetColl = &(*jets);


  for(unsigned ijet=0;ijet<jetColl->size(); ijet++) {
      const pat::Jet jet = jetColl->at(ijet);
      
      double pt =  jet.pt();
      
      if(pt>jetPtMin_){
	pfEvt_.pfPt_[pfEvt_.njets_] = pt;      
	pfEvt_.pfEta_[pfEvt_.njets_] = jet.eta();      
	pfEvt_.pfPhi_[pfEvt_.njets_] = jet.phi();      
	pfEvt_.njets_++;
      }
  }
	

  // All done
  pfTree_->Fill();
}

/*
void PFCandAnalyzer::FillEventInfo(const edm::Event& iEvent, const edm::EventSetup& iSetup, TreePFCandEventData & tr)
{
  // General Info
  tr.run_	  = iEvent.id().run();
  tr.evt_	  = iEvent.id().event();
  tr.lumi_	  = iEvent.luminosityBlock();

  if(!genOnly_&&sampleType_<10){
    // HI Event info
    edm::Handle<reco::Centrality> cent;
    iEvent.getByLabel(edm::InputTag("hiCentrality"),cent);
    Double_t hf	  = cent->EtHFhitSum();
    // Get Centrality bin
    cbins_ = getCentralityBinsFromDB(iSetup);
    tr.cent_ = cbins_->getBin(hf)*(100./cbins_->getNbins());
  }

  if (isMC_&&sampleType_<10) {
    edm::Handle<edm::GenHIEvent> mchievt;
    iEvent.getByLabel(edm::InputTag("heavyIon"),mchievt);
    tr.b_	  = mchievt->b();
    tr.npart_	  = mchievt->Npart();
    tr.ncoll_	  = mchievt->Ncoll();
  }
}
*/
void PFCandAnalyzer::beginJob()
{

  // -- trees --                                                                                                                                                                                                                        
    pfTree_ = fs->make<TTree>("pfTree","dijet tree");
    pfEvt_.SetTree(pfTree_);
    pfEvt_.SetBranches();
}

// ------------ method called once each job just after ending the event loop  ------------                                                                                                                                              
void
PFCandAnalyzer::endJob() {
  // ===== Done =====                                                                                                                                                                                                                   
  /*  if (verbosity_>=1) {
    cout << endl << "================ Ana Process Summaries =============" << endl;
    cout << " AnaJet: " << jetsrc_ << endl;
    if (refJetType_>=0) cout << " RefJet: " << refjetsrc_ << endl;
    cout << " AnaTrk: " << trksrc_ << endl;
    cout << "# HI Events : "<< numHiEvtSel_<< endl;
    cout << "# Base Events: "<< numEvtSel_ << endl;
    cout << "# Jet Events: "<< numJetEvtSel_<< endl;
  }
  */
}


//define this as a plug-in
DEFINE_FWK_MODULE(PFCandAnalyzer);

