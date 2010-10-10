#include "MNguyen/JetFilter/plugins/JetFilter.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"

JetFilter::JetFilter(const edm::ParameterSet& ps)
{

  patJetLabel_ = ps.getParameter<edm::InputTag>("patJetLabel");
  genJetLabel_ = ps.getParameter<edm::InputTag>("genJetLabel");

  minPt_ = ps.getParameter<double>("minPt");
  verbose_ = ps.getParameter<bool>("verbose");
}

JetFilter::~JetFilter() {}

void JetFilter::beginJob()
{
  std::cout << "FL: beginJob" << std::endl;
}

bool JetFilter::filter(edm::Event& iEvent, 
			   const edm::EventSetup& iSetup)
{

  // Reject the event so far
  bool keep=false;

  edm::Handle<pat::JetCollection > patJets_;
  edm::Handle<reco::GenJetCollection > genJets_;



  iEvent.getByLabel(patJetLabel_,patJets_);     
  iEvent.getByLabel(genJetLabel_,genJets_);     

  const pat::JetCollection *patJetColl = &(*patJets_);
  const reco::GenJetCollection *genJetColl = &(*genJets_);
 

  for(unsigned i=0;i<patJetColl->size(); i++) {

    //std::cout<<"patJet # "<<i<<std::endl;
    const pat::Jet patJet = patJetColl->at(i);
    double patJetPt = patJet.pt();

    if(patJetPt>30.0)
      {
	if(!patJet.genJet()){
	  std::cout<<" No gen jet found for pat jet of pT =  "<<patJetPt<<std::endl;
	}
	else{
	  const reco::GenJet *genJet = patJet.genJet();	  
	  double genJetPt = genJet->pt();
	  
	  if(genJetPt>100.0){
	      if(patJetPt/genJetPt < 0.3)
		keep = true;
	  }

	}
      }
  }
  /*
  for(unsigned i=0;i<genJetColl->size(); i++) {

    std::cout<<"genJet # "<<i<<std::endl;
    const reco::GenJet genJet = genJetColl->at(i);
    
  }
  */
  return keep;

}

void JetFilter::endJob()
{
  std::cout << "FL: endJob" << std::endl;
}

#include "FWCore/PluginManager/interface/ModuleDef.h"
#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE (JetFilter) ;
