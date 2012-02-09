// -*- C++ -*-
//
// Package:    PFTowerAnalyzer
// Class:      PFTowerAnalyzer
// 
/**\class PFTowerAnalyzer PFTowerAnalyzer.cc ana/PFCandAnalyzer/src/PFTowerAnalyzer.cc

Description: [one line class summary]

Implementation:
[Notes on implementation]
 */
//
// Original Author:  Matt Nguyen 
//         Created:   Feb 10 2012
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
#include "MNguyen/PFCandAnalyzer/interface/PFTowerAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/HeavyIonEvent/interface/CentralityProvider.h"

//#include "DataFormats/HcalDetId/interface/HcalDetId.h"
//#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "Geometry/Records/interface/CaloGeometryRecord.h"
//#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"


using namespace std;
using namespace edm;
using namespace reco;

//
// constructors and destructor
//
PFTowerAnalyzer::PFTowerAnalyzer(const ParameterSet& iConfig)
{
  // Event source
  // Event Info
  pfTowerLabel_ = iConfig.getParameter<InputTag>("pfTowerLabel");

  pfPtMin_ = iConfig.getParameter<double>("pfPtMin");

  // debug
  verbosity_ = iConfig.getUntrackedParameter<int>("verbosity", 0);



}


PFTowerAnalyzer::~PFTowerAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
  void
PFTowerAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{

  //pfEvt_.Clear();

  // Fill PF Tower info

  Handle<CaloTowerCollection> caloTowers;
  iEvent.getByLabel( pfTowerLabel_, caloTowers );

  if (caloTowers.isValid()) {

    FillEventInfo(iEvent, iSetup);

    for( CaloTowerCollection::const_iterator cal = caloTowers->begin(); cal != caloTowers->end(); ++ cal ){
      pfTower.eta[pfTower.nPFTowers] = cal->eta();
      pfTower.phi[pfTower.nPFTowers] = cal->phi();
      pfTower.ieta[pfTower.nPFTowers] = cal->ieta();
      pfTower.iphi[pfTower.nPFTowers] = cal->iphi();
      pfTower.pt[pfTower.nPFTowers] = cal->pt();

      pfTower.nPFTowers++;
    }
    
    pfTowerTree->Fill();
  }

  memset(&pfTower,0,sizeof pfTower);
}


void PFTowerAnalyzer::FillEventInfo(const Event& iEvent, const EventSetup& iSetup)
{
  // General Info

  pfTower.run	  = iEvent.id().run();
  pfTower.evt	  = iEvent.id().event();
  pfTower.lumi	  = iEvent.luminosityBlock();

  Handle<vector<Vertex> >vertex;
  iEvent.getByLabel("hiSelectedVertex", vertex);  
 
  if(vertex->size()>0 || vertex->begin()->isFake()) {
    pfTower.vx=vertex->begin()->x();
    pfTower.vy=vertex->begin()->y();
    pfTower.vz=vertex->begin()->z();
  }
  else{
    pfTower.vx=-999;
    pfTower.vy=-999;
    pfTower.vz=-999;
  }

  if(!centrality_) centrality_ = new CentralityProvider(iSetup);
  centrality_->newEvent(iEvent,iSetup); // make sure you do this first in every event                         
  pfTower.bin = centrality_->getBin();

}

void PFTowerAnalyzer::beginJob()
{

  pfTowerTree = fs->make<TTree>("pfTowerTree","PF Tower Tree");

  pfTowerTree->Branch("run",&pfTower.run,"run/I");
  pfTowerTree->Branch("evt",&pfTower.evt,"evt/I");
  pfTowerTree->Branch("lumi",&pfTower.lumi,"lumi/I");

  pfTowerTree->Branch("vx",&pfTower.vx,"vx/F");
  pfTowerTree->Branch("vy",&pfTower.vy,"vy/F");
  pfTowerTree->Branch("vz",&pfTower.vz,"vz/F");

  pfTowerTree->Branch("bin",&pfTower.bin,"bin/I");

  pfTowerTree->Branch("nPFTowers",&pfTower.nPFTowers,"nPFTowers/I");

  pfTowerTree->Branch("ieta",pfTower.ieta,"ieta[nPFTowers]/I");
  pfTowerTree->Branch("iphi",pfTower.iphi,"iphi[nPFTowers]/I");
  pfTowerTree->Branch("eta",pfTower.eta,"eta[nPFTowers]/F");
  pfTowerTree->Branch("phi",pfTower.phi,"phi[nPFTowers]/F");
  pfTowerTree->Branch("pt",pfTower.pt,"pt[nPFTowers]/F");

}

// ------------ method called once each job just after ending the event loop  ------------                                                                                                                                              
void
PFTowerAnalyzer::endJob() {
}


//define this as a plug-in
DEFINE_FWK_MODULE(PFTowerAnalyzer);

