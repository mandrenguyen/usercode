// -*- C++ -*-
//
// Package:    PFMuonAnalyzer
// Class:      PFMuonAnalyzer
// 
/**\class PFMuonAnalyzer PFMuonAnalyzer.cc ana/PFMuonAnalyzer/src/PFMuonAnalyzer.cc

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
#include "MNguyen/MuonPFAnalysis/interface/PFMuonAnalyzer.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "TMath.h"
#include "TStopwatch.h"


using namespace std;
using namespace edm;
using namespace reco;

//
// constructors and destructor
//
PFMuonAnalyzer::PFMuonAnalyzer(const edm::ParameterSet& iConfig)
{
  // Event source
  // Event Info
  pfCandidateLabel_ = iConfig.getParameter<edm::InputTag>("pfCandidateLabel");
  recoMuonLabel_ = iConfig.getParameter<edm::InputTag>("recoMuonLabel");
  beamSpotLabel_ = iConfig.getParameter<edm::InputTag>("beamSpotLabel");
  genLabel_ = iConfig.getParameter<edm::InputTag>("genLabel");


  // debug
  //verbosity_ = iConfig.getUntrackedParameter<int>("verbosity", 0);



}


PFMuonAnalyzer::~PFMuonAnalyzer()
{

  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)

}




// ------------ method called to for each event  ------------
  void
PFMuonAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{


  edm::Handle<reco::GenParticleCollection> genParticles;
  // use this instead
  //edm::Handle<std::vector <reco::PFSimParticle> >simParticles;
  edm::Handle<reco::PFCandidateCollection> pfCandidates;
  edm::Handle<reco::MuonCollection> recoMuons;
  edm::Handle<reco::BeamSpot> beamSpotHandle;

  iEvent.getByLabel(genLabel_,genParticles);     
  // sim particles not available, use Gen
  //if(useSim_) iEvent.getByLabel(simLabel_,simParticles);     
  iEvent.getByLabel(pfCandidateLabel_,pfCandidates);     
  iEvent.getByLabel(recoMuonLabel_,recoMuons);     
  iEvent.getByLabel(beamSpotLabel_,beamSpotHandle);     

  reco::BeamSpot beamSpot = *beamSpotHandle;
  
  const reco::MuonCollection *recoMuonColl = &(*recoMuons);
  const reco::PFCandidateCollection *pfCandidateColl = &(*pfCandidates);



  


// Reco Muons

  vector<bool> isRecoMuon[NRECOSEL];
        
  for(unsigned imu=0;imu<recoMuonColl->size(); imu++) {
    
    const reco::Muon recoMuon = recoMuonColl->at(imu);
    hRecoMuon_Pt_vs_Eta_[0]->Fill(recoMuon.pt(),recoMuon.eta());    
    
    reco::MuonRef muonRef( recoMuonColl, imu );

    bool pass_Global = muonRef->isGlobalMuon();
    bool pass_Tight = tightMuonPOG(muonRef, beamSpotHandle);    
    bool pass_GlobalPrompt = muon::isGoodMuon(*muonRef,muon::GlobalMuonPromptTight);

    if( pass_Global ) {
      hRecoMuon_Pt_vs_Eta_[1]->Fill(recoMuon.pt(),recoMuon.eta());    
    }
    
    if( pass_Tight ) {
      hRecoMuon_Pt_vs_Eta_[2]->Fill(recoMuon.pt(),recoMuon.eta());    
    }
    
    if( pass_GlobalPrompt ) {
      hRecoMuon_Pt_vs_Eta_[3]->Fill(recoMuon.pt(),recoMuon.eta());    
    }

    isRecoMuon[0].push_back(true);
    isRecoMuon[1].push_back(pass_Global);
    isRecoMuon[2].push_back(pass_Tight);
    isRecoMuon[3].push_back(pass_GlobalPrompt);
    
  }  // for(unsigned imu=0;imu<recoMuonColl->size(); imu++)    

    // PF Muons

  vector<bool> isPFMuon[NPFSEL];

  
  for(unsigned imu=0;imu<pfCandidateColl->size(); imu++) {


    const reco::PFCandidate pfCandidate = pfCandidateColl->at(imu);
    
    bool isGlobalTight = false;
    bool isTrackerTight = false;
    bool isGlobalLoose = false;
    bool isTrackerLoose = false;

    bool isGlobalTrackerMuon = false;
    bool isTrackerOnlyMuon = false;
    bool isNotTrackerMuon = false;

    int pdgid = pfCandidate.pdgId();      
    
    if(abs(pdgid)==13){    

      const reco::MuonRef recoMuon_ref = pfCandidate.muonRef();
      if(!recoMuon_ref.isNonnull()) cout<<" c'est null "<<endl;
      
      const reco::Muon recoMuon = *recoMuon_ref;
            
      hPFMuon_Pt_vs_Eta_[0]->Fill(recoMuon.pt(),recoMuon.eta());    
      
      if(isGlobalTightMuon(recoMuon_ref)){
	isGlobalTight = true;
	hPFMuon_Pt_vs_Eta_[1]->Fill(recoMuon.pt(),recoMuon.eta());    
      }
      else if(isTrackerTightMuon(recoMuon_ref)){
	isTrackerTight = true;
	hPFMuon_Pt_vs_Eta_[2]->Fill(recoMuon.pt(),recoMuon.eta());    
      }
      else if(isGlobalLooseMuon(recoMuon_ref)){
	isGlobalLoose = true;
	hPFMuon_Pt_vs_Eta_[3]->Fill(recoMuon.pt(),recoMuon.eta());    
      }
      else if(isTrackerLooseMuon(recoMuon_ref)){
	isTrackerLoose = true;
	hPFMuon_Pt_vs_Eta_[4]->Fill(recoMuon.pt(),recoMuon.eta());    
      }
      bool isGlobalMuon = recoMuon_ref->isGlobalMuon();
      bool isTrackerMuon = recoMuon_ref->isTrackerMuon();
      
      if(isGlobalMuon && isTrackerMuon){ 
	isGlobalTrackerMuon = true; 
	hPFMuon_Pt_vs_Eta_[5]->Fill(recoMuon.pt(),recoMuon.eta());    
      }
      if(!isGlobalMuon && isTrackerMuon){
	isTrackerOnlyMuon = true; 
	hPFMuon_Pt_vs_Eta_[6]->Fill(recoMuon.pt(),recoMuon.eta());    
      }
      if(isGlobalMuon && !isTrackerMuon){
	isNotTrackerMuon = true; 
	hPFMuon_Pt_vs_Eta_[7]->Fill(recoMuon.pt(),recoMuon.eta());    
      }

    }
    isPFMuon[0].push_back(true);
    isPFMuon[1].push_back(isGlobalTight );
    isPFMuon[2].push_back(isTrackerTight);
    isPFMuon[3].push_back(isGlobalLoose );
    isPFMuon[4].push_back(isTrackerLoose);
    isPFMuon[5].push_back(isGlobalTrackerMuon);
    isPFMuon[6].push_back(isTrackerOnlyMuon);
    isPFMuon[7].push_back(isNotTrackerMuon);
    
  } // for(unsigned imu=0;imu<pfCandidateColl->size(); imu++)
    



  // Loop over gen particles and find whether there is a corresponding reconstructed muon passing cuts
  int ngenmuons = 0;
  
  const reco::GenParticleCollection* genColl= &(*genParticles);

  for(unsigned igen=0;igen<genColl->size(); igen++) {
    const reco::GenParticle gen = genColl->at(igen);
    
    if(abs(gen.pdgId())==13 && gen.status()==1){
      hGenMuon_Pt_vs_Eta_->Fill(gen.pt(),gen.eta());    
      if(gen.pt()>10. && fabs(gen.eta())<2.1) ngenmuons++;
      
    }
  }
  
  
    // map each reconstructed muon to a specific gen muon (if one exists)

    // Reco Muons
    
  
  map<int, int> mapReco2Gen;    
  
  for(unsigned imu=0;imu<recoMuonColl->size(); imu++) {
    const reco::Muon muon = recoMuonColl->at(imu);	  
    
    
    //double best_dpt = 9999.;
    int best_gen_index = -1;
    
    for(unsigned igen=0;igen<genColl->size(); igen++) {
      
      const reco::GenParticle gen = genColl->at(igen);
      
      if(abs(gen.pdgId())==13 && gen.status()==1){      
	
	double dR = deltaR(muon.eta(), muon.phi(), gen.eta(), gen.phi());    
	//double dpt = fabs(muon.pt()-gen.pt());



	if(dR<0.02) {
	  //best_dpt = dpt;
	  best_gen_index = igen;
	}
      }
    }
    

    if(best_gen_index>-1){
      //const reco::GenParticle gen = genColl->at(best_gen_index);	  
      //cout<< "inserting pair, gen index: "<< best_gen_index <<", mu index: "<<imu<<", gen pt: "<<gen.pt()<<", muon pt: "<<muon.pt()<<endl;
      mapReco2Gen.insert(pair<int, int>(best_gen_index, imu));
      for(int isel = 0; isel < NRECOSEL; isel++){
	if(isRecoMuon[isel][imu]==true)hRecoMuonGenMatched_RecPt_RecEta_[isel]->Fill(muon.pt(),muon.eta());    
      }
    }
    else{

      for(int isel = 0; isel < NRECOSEL; isel++){
	if(isRecoMuon[isel][imu]==true)hRecoMuonNotGenMatched_RecPt_RecEta_[isel]->Fill(muon.pt(),muon.eta());    
      }
    }
  }          // for(unsigned imu=0;imu<recoMuonColl->size(); imu++) 
  

  
  for(unsigned igen=0;igen<genColl->size(); igen++) {
    
    const reco::GenParticle gen = genColl->at(igen);
    
    if(abs(gen.pdgId())==13 && gen.status()==1){      
      
      bool foundRec[NRECOSEL];
      reco::Muon bestRecoMuon[NRECOSEL];
      double best_dpt[NRECOSEL];

      for(int isel=0;isel<NRECOSEL;isel++){
	foundRec[isel]=false;
	best_dpt[isel]=9999.;
      }

      double gen_pt = gen.pt();
      int parent_id =  gen.mother()->pdgId();
      
      pair<map<int, int>::iterator, map<int, int>::iterator> pairGenReco;      
      pairGenReco = mapReco2Gen.equal_range(igen);
      
      // find the best Reco Muon corresponding to the Gen Muon
      for (map<int, int>::iterator map_iter = pairGenReco.first; map_iter != pairGenReco.second; ++map_iter)
	{
	  
	  //cout << "  [" << (*map_iter).first << ", " << (*map_iter).second << "]" << endl;
	  
	  const reco::Muon recoMuon = recoMuonColl->at((*map_iter).second);
	  
	  double dpt = fabs(gen_pt - recoMuon.pt());
	  
	  for(int isel=0;isel<NRECOSEL;isel++){
	    if(isRecoMuon[isel][(*map_iter).second] == true){
	      foundRec[isel] = true;
	      if(dpt < best_dpt[isel]){	    
		bestRecoMuon[isel] =  recoMuon;
		best_dpt[isel] = dpt;
	      }
	    }
	  }

	}
      
      for(int isel=0;isel<NRECOSEL;isel++){
	if(foundRec[isel]){
	  float deltaPt = (gen.pt()-bestRecoMuon[isel].pt())/gen.pt();
	  
	  hRecoMuon_RecPt_vs_GenPt_vs_GenEta_[isel]->Fill(bestRecoMuon[isel].pt(),gen.pt(),gen.eta());    
	  hRecoMuon_DeltaPtOverPt_vs_GenPt_[isel]->Fill(deltaPt,gen.pt());    
	  
	}
	
      
      }   // if(foundRec)  
    }        // if(abs(gen.pdgId())==13 && gen.status()==1)      
  }     // for(unsigned igen=0;igen<genColl->size(); igen++) 

  


  
  
  // PF Muons
  

  
  map<int, int> mapPF2Gen;    
  
  for(unsigned imu=0;imu<pfCandidateColl->size(); imu++) {
    
    const reco::PFCandidate pfCandidate = pfCandidateColl->at(imu);	  
    
    int pdgid = pfCandidate.pdgId();      
    
    if(abs(pdgid)==13){          
      
      const reco::PFCandidate muon = pfCandidate;
      
      //double best_dpt = 9999.;
      int best_gen_index = -1;
      
      for(unsigned igen=0;igen<genColl->size(); igen++) {
	
	const reco::GenParticle gen = genColl->at(igen);
	
	if(abs(gen.pdgId())==13 && gen.status()==1){      
	  
	  double dR = deltaR(muon.eta(), muon.phi(), gen.eta(), gen.phi());    
	  //double dpt = fabs(muon.pt()-gen.pt());
	  
	  if(dR<0.02) {
	    //best_dpt = dpt;
	    best_gen_index = igen;
	  }
	}
      }
      
      if(best_gen_index>-1){
	const reco::GenParticle gen = genColl->at(best_gen_index);	  
	// cout<< "inserting pair, gen index: "<< best_gen_index <<", mu index: "<<imu<<", gen pt: "<<gen.pt()<<", muon pt: "<<muon.pt()<<endl;
	mapPF2Gen.insert(pair<int, int>(best_gen_index, imu));
	
	for(int isel = 0;isel<NPFSEL;isel++){
	  if(isPFMuon[isel][imu]==true) hPFMuonGenMatched_RecPt_RecEta_[isel]->Fill(muon.pt(),muon.eta());    	    
	}	
      }
      else{
	for(int isel = 0;isel<NPFSEL;isel++){
	  if(isPFMuon[isel][imu]==true) hPFMuonNotGenMatched_RecPt_RecEta_[isel]->Fill(muon.pt(),muon.eta());    	       	
	}
      }
    }    
  }
  
      
  for(unsigned igen=0;igen<genColl->size(); igen++) {
    
    const reco::GenParticle gen = genColl->at(igen);
    
    if(abs(gen.pdgId())==13 && gen.status()==1){      
      
      bool foundPFMu[NPFSEL];
      reco::PFCandidate bestPFMuon[NPFSEL];
      double best_dpt[NPFSEL];
      
      for(int isel=0;isel<NPFSEL;isel++){
	foundPFMu[isel]= false;
	best_dpt[isel] = 9999.;
      }
      
      double gen_pt = gen.pt();
      int parent_id =  gen.mother()->pdgId();
      
      pair<map<int, int>::iterator, map<int, int>::iterator> pairGenPF;      
      pairGenPF = mapPF2Gen.equal_range(igen);
      
      // find the best PF Muon corresponding to the Gen Muon
      for (map<int, int>::iterator map_iter = pairGenPF.first; map_iter != pairGenPF.second; ++map_iter)
	{
	  
	  //cout << "  [" << (*map_iter).first << ", " << (*map_iter).second << "]" << endl;
	  
	  const reco::PFCandidate pfMuon = pfCandidateColl->at((*map_iter).second);
	  
	  double dpt = fabs(gen_pt - pfMuon.pt());

	  for(int isel=0;isel<NPFSEL;isel++){
	    if(isPFMuon[isel][(*map_iter).second] == true){
	      foundPFMu[isel] = true;
	      if(dpt < best_dpt[isel]){
		bestPFMuon[isel] =  pfMuon;
		best_dpt[isel] = dpt;
	      }	    	    
	    }
	  }
	}
      
      for(int isel=0;isel<NPFSEL;isel++){
	if(foundPFMu[isel]){
	  float deltaPt = (gen.pt()-bestPFMuon[isel].pt())/gen.pt();
	  
	  hPFMuon_RecPt_vs_GenPt_vs_GenEta_[isel]->Fill(bestPFMuon[isel].pt(),gen.pt(),gen.eta());    
	  hPFMuon_DeltaPtOverPt_vs_GenPt_[isel]->Fill(deltaPt,gen.pt());           
	  
	}   // if(foundPFMu)  
      }
    }        // if(abs(gen.pdgId())==13 && gen.status()==1)      
  }     // for(unsigned igen=0;igen<genColl->size(); igen++) 
  

}

void PFMuonAnalyzer::beginJob()
{

  // -- book histograms --


   
   TString recoSel[NRECOSEL] = {"","Global","Tight","GlobalPrompt"};
   for(int iRecoSel = 0; iRecoSel<NRECOSEL; iRecoSel++){
     
     TString recoHistName = "hRecoMuon"+recoSel[iRecoSel];
     hRecoMuon_Pt_vs_Eta_[iRecoSel] = fs->make<TH2D>((recoHistName+"_Pt_vs_Eta").Data(),"Reco muon p_{T};p_{T} [GeV/c]#eta;Events",100,0,200,60,-3.,3.);
     hRecoMuonGenDeltaR_vs_GenPt_[iRecoSel] = fs->make<TH2D>((recoHistName+"_GenDeltaR_vs_GenPt").Data(),"RecoMuon;#Delta R;p_{T}^{GEN} [GeV/c]",500,0,0.1,100,0,200);
     hRecoMuon_DeltaPtOverPt_vs_GenPt_[iRecoSel] = fs->make<TH2D>((recoHistName+"_DeltaPtOverPt_vs_GenPt").Data(),"RecoMuon;#Delta p_{T};p_{T}^{GEN} [GeV/c]",500,-0.5,0.5,100,0,200);

     hRecoMuonGenMatched_RecPt_RecEta_[iRecoSel] = fs->make<TH2D>((recoHistName+"_GenMatched_RecPt_vs_RecEta").Data(),"Reco Muon, No Gen Match;p_{T}^{rec} [GeV/c];#eta^{rec};Events",100,0.,100.,60,-3.,3.);
     hRecoMuonNotGenMatched_RecPt_RecEta_[iRecoSel] = fs->make<TH2D>((recoHistName+"_NotGenMatched_RecPt_vs_RecEta").Data(),"Reco Muon, No Gen Match;p_{T}^{rec} [GeV/c];#eta^{rec};Events",100,0.,100.,60,-3.,3.);

      hRecoMuon_RecPt_vs_GenPt_vs_GenEta_[iRecoSel] = fs->make<TH3D>((recoHistName+"RecPt_vs_GenPt_vs_GenEta").Data(),"RecoMuon;p_{T}^{REC} [GeV/c];p_{T}^{Gen} [GeV/c], #eta^{Gen}",100,0,200,100,0,200,60,-3.,3.);

   }

   TString pfSel[NPFSEL] = {"","GlobalTight","TrackerTight","GlobalLoose","TrackerLoose","GlobalTracker","TrackerOnly","NotTracker"};
   for(int iPFSel = 0; iPFSel<NPFSEL; iPFSel++){

     TString pfHistName = "hPFMuon"+pfSel[iPFSel];
     hPFMuon_Pt_vs_Eta_[iPFSel] = fs->make<TH2D>((pfHistName+"_Pt_vs_Eta").Data(),"PF muon p_{T};p_{T} [GeV/c]#eta;Events",100,0,200,60,-3.,3.);
     hPFMuonGenDeltaR_vs_GenPt_[iPFSel] = fs->make<TH2D>((pfHistName+"_GenDeltaR_vs_GenPt").Data(),"PFMuon;#Delta R;p_{T}^{GEN} [GeV/c]",500,0,0.1,100,0,200);
     hPFMuon_DeltaPtOverPt_vs_GenPt_[iPFSel] = fs->make<TH2D>((pfHistName+"_DeltaPtOverPt_vs_GenPt").Data(),"PFMuon;#Delta p_{T};p_{T}^{GEN} [GeV/c]",500,-0.5,0.5,100,0,200);

     hPFMuonGenMatched_RecPt_RecEta_[iPFSel] = fs->make<TH2D>((pfHistName+"_GenMatched_RecPt_vs_RecEta").Data(),"PF Muon, No Gen Match;p_{T}^{rec} [GeV/c];#eta^{rec};Events",100,0.,100.,60,-3.,3.);
     hPFMuonNotGenMatched_RecPt_RecEta_[iPFSel] = fs->make<TH2D>((pfHistName+"_NotGenMatched_RecPt_vs_RecEta").Data(),"PF Muon, No Gen Match;p_{T}^{rec} [GeV/c];#eta^{rec};Events",100,0.,100.,60,-3.,3.);

      hPFMuon_RecPt_vs_GenPt_vs_GenEta_[iPFSel] = fs->make<TH3D>((pfHistName+"RecPt_vs_GenPt_vs_GenEta").Data(),"PFMuon;p_{T}^{REC} [GeV/c];p_{T}^{Gen} [GeV/c], #eta^{Gen}",100,0,200,100,0,200,60,-3.,3.);

   }

   hGenMuon_Pt_vs_Eta_ = fs->make<TH2D>("GenMuon_Pt_vs_Eta","Gen muon p_{T};p_{T} [GeV/c]#eta;Events",100,0,200,60,-3.,3.);





}

// ------------ method called once each job just after ending the event loop  ------------                                                                                                                                              
void
PFMuonAnalyzer::endJob() {
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


bool 
PFMuonAnalyzer::tightMuonPOG(const reco::MuonRef& muonRef, const edm::Handle<reco::BeamSpot>& beamspot){
			     //const reco::BeamSpot& beamSpot){

  if(!muon::isGoodMuon(*muonRef,muon::GlobalMuonPromptTight)) return false;

  if(!muonRef->isTrackerMuon()) return false;
  
  if(muonRef->numberOfMatches()<2) return false;
  
  //const reco::TrackRef& combinedMuon = muonRef->combinedMuon();    
  const reco::TrackRef& combinedMuon = muonRef->globalTrack();    
  
  if(combinedMuon->hitPattern().numberOfValidTrackerHits()<11) return false;
  
  if(combinedMuon->hitPattern().numberOfValidPixelHits()==0) return false;
  
  if(combinedMuon->hitPattern().numberOfValidMuonHits()==0) return false;  

  //if(fabs(combinedMuon->dxy(beamspot->position()))>0.2) return false;
  
  return true;

}

// From PFMuonAlgo 1.21

bool
PFMuonAnalyzer::isGlobalTightMuon( const reco::MuonRef& muonRef ) {

 if ( !muonRef.isNonnull() ) return false;

 if ( !muonRef->isGlobalMuon() ) return false;
 if ( !muonRef->isStandAloneMuon() ) return false;
 
 
 if ( muonRef->isTrackerMuon() ) { 
   bool result = muon::isGoodMuon(*muonRef,muon::GlobalMuonPromptTight);
   
   bool isTM2DCompatibilityTight =  muon::isGoodMuon(*muonRef,muon::TM2DCompatibilityTight);   
   int nMatches = muonRef->numberOfMatches();
   bool quality = nMatches > 2 || isTM2DCompatibilityTight;
   
   return result && quality;
   
 } else {

   reco::TrackRef standAloneMu = muonRef->standAloneMuon();
    // No tracker muon -> Request a perfect stand-alone muon, or an even better global muon
    bool result = false;
    // Check the quality of the stand-alone muon : 
    // good chi**2 and large number of hits and good pt error
    if ( ( standAloneMu->hitPattern().numberOfValidMuonDTHits() < 22 &&
	   standAloneMu->hitPattern().numberOfValidMuonCSCHits() < 15 ) ||
	 standAloneMu->normalizedChi2() > 10. || 
	 standAloneMu->ptError()/standAloneMu->pt() > 0.20 ) {
      result = false;
    } else { 
      reco::TrackRef combinedMu = muonRef->combinedMuon();
      reco::TrackRef trackerMu = muonRef->track();
            
      // If the stand-alone muon is good, check the global muon
      if ( combinedMu->normalizedChi2() > standAloneMu->normalizedChi2() ) {
	// If the combined muon is worse than the stand-alone, it 
	// means that either the corresponding tracker track was not 
	// reconstructed, or that the sta muon comes from a late 
	// pion decay (hence with a momentum smaller than the track)
	// Take the stand-alone muon only if its momentum is larger
	// than that of the track
	result = standAloneMu->pt() > trackerMu->pt() ;
     } else { 
	// If the combined muon is better (and good enough), take the 
	// global muon
	result = 
	  combinedMu->ptError()/combinedMu->pt() < 
	  std::min(0.20,standAloneMu->ptError()/standAloneMu->pt());
      }
    }      

    return result;    
  }

  return false;

}

bool
PFMuonAnalyzer::isTrackerTightMuon( const reco::MuonRef& muonRef ) {

  if ( !muonRef.isNonnull() ) return false;
    
  if(!muonRef->isTrackerMuon()) return false;
  
  reco::TrackRef trackerMu = muonRef->track();
  const reco::Track& track = *trackerMu;
  
  unsigned nTrackerHits =  track.hitPattern().numberOfValidTrackerHits();
  
  if(nTrackerHits<=12) return false;
  
  bool isAllArbitrated = muon::isGoodMuon(*muonRef,muon::AllArbitrated);
  
  bool isTM2DCompatibilityTight = muon::isGoodMuon(*muonRef,muon::TM2DCompatibilityTight);
  
  if(!isAllArbitrated || !isTM2DCompatibilityTight)  return false;

  if((trackerMu->ptError()/trackerMu->pt() > 0.10)){
    //std::cout<<" PT ERROR > 10 % "<< trackerMu->pt() <<std::endl;
    return false;
  }
  return true;
  
}

bool
PFMuonAnalyzer::isGlobalLooseMuon( const reco::MuonRef& muonRef ) {

  if ( !muonRef.isNonnull() ) return false;
  if ( !muonRef->isGlobalMuon() ) return false;
  if ( !muonRef->isStandAloneMuon() ) return false;
  
  reco::TrackRef standAloneMu = muonRef->standAloneMuon();
  reco::TrackRef combinedMu = muonRef->combinedMuon();
  reco::TrackRef trackerMu = muonRef->track();
 
  unsigned nMuonHits =
    standAloneMu->hitPattern().numberOfValidMuonDTHits() +
    2*standAloneMu->hitPattern().numberOfValidMuonCSCHits();
    
  bool quality = false;
  
  if ( muonRef->isTrackerMuon() ){

    bool result = combinedMu->normalizedChi2() < 100.;
    
    bool laststation =
      muon::isGoodMuon(*muonRef,muon::TMLastStationAngTight);
        
    int nMatches = muonRef->numberOfMatches();    
    
    quality = laststation && nMuonHits > 12 && nMatches > 1;

    return result && quality;
    
  }
  else{

    // Check the quality of the stand-alone muon : 
    // good chi**2 and large number of hits and good pt error
    if (  nMuonHits <=15  ||
	  standAloneMu->normalizedChi2() > 10. || 
	  standAloneMu->ptError()/standAloneMu->pt() > 0.20 ) {
      quality = false;
    }
   else { 
      // If the stand-alone muon is good, check the global muon
      if ( combinedMu->normalizedChi2() > standAloneMu->normalizedChi2() ) {
	// If the combined muon is worse than the stand-alone, it 
	// means that either the corresponding tracker track was not 
	// reconstructed, or that the sta muon comes from a late 
	// pion decay (hence with a momentum smaller than the track)
	// Take the stand-alone muon only if its momentum is larger
	// than that of the track

	// Note that here we even take the standAlone if it has a smaller pT, in contrast to GlobalTight
	if(standAloneMu->pt() > trackerMu->pt() || combinedMu->normalizedChi2()<5.) quality =  true;
      }
      else { 
	// If the combined muon is better (and good enough), take the 
	// global muon
	if(combinedMu->ptError()/combinedMu->pt() < std::min(0.20,standAloneMu->ptError()/standAloneMu->pt())) 
	  quality = true;
	
      }
   }         
  }
  

  return quality;

}


bool
PFMuonAnalyzer::isTrackerLooseMuon( const reco::MuonRef& muonRef ) {

  if ( !muonRef.isNonnull() ) return false;
  if(!muonRef->isTrackerMuon()) return false;

  reco::TrackRef trackerMu = muonRef->track();

  if(trackerMu->ptError()/trackerMu->pt() > 0.20) return false;

  if(trackerMu->pt()>20.) return false;
    
  bool isAllArbitrated = muon::isGoodMuon(*muonRef,muon::AllArbitrated);
  bool isTMLastStationAngTight = muon::isGoodMuon(*muonRef,muon::TMLastStationAngTight);

  bool quality = isAllArbitrated && isTMLastStationAngTight;

  return quality;
  
}



//define this as a plug-in
DEFINE_FWK_MODULE(PFMuonAnalyzer);

