/*
  Based on edwenger/TrkEffAnalyzer
  Modified by Matt Nguyen, November 2010

*/

#include "MNguyen/PFCandidateTrackAnalyzer/interface/pfCandidateTrackAnalyzer.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"


#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "edwenger/VertexAnalyzer/interface/VertexComparator.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include <Math/DistFunc.h>
#include "TMath.h"



#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"


using namespace std;
using namespace edm;
using namespace reco;


PFCandidateTrackAnalyzer::PFCandidateTrackAnalyzer(const edm::ParameterSet& iConfig) {
  

  inputTagPFCandidates_ = iConfig.getParameter<InputTag>("PFCandidates");
  inputTagVertices_ = iConfig.getParameter<InputTag>("Vertices");
  inputTagSimTracks_ = iConfig.getParameter<InputTag>("SimTracks");
  inputTagTracks_ = iConfig.getParameter<InputTag>("Tracks");

  verbose_ = iConfig.getUntrackedParameter<bool>("verbose",false);
  printBlocks_ = iConfig.getUntrackedParameter<bool>("printBlocks",false);
  thePtMin_ = iConfig.getUntrackedParameter<double>("ptMin",3.0);
  hasSimInfo_ = iConfig.getUntrackedParameter<bool>("hasSimInfo");

  minHits_ = iConfig.getUntrackedParameter<double>("minHits");
  maxPtErr_ = iConfig.getUntrackedParameter<double>("maxPtErr");
  maxD0_ = iConfig.getUntrackedParameter<double>("maxD0");
  maxDZ_ = iConfig.getUntrackedParameter<double>("maxDZ");
  maxD0Norm_ = iConfig.getUntrackedParameter<double>("maxD0Norm");
  maxDZNorm_ = iConfig.getUntrackedParameter<double>("maxDZNorm");
  pixelSeedOnly_ = iConfig.getUntrackedParameter<bool>("pixelSeedOnly");

  LogDebug("PFCandidateTrackAnalyzer")
    <<" input collection : "<<inputTagPFCandidates_ ;



   
}



PFCandidateTrackAnalyzer::~PFCandidateTrackAnalyzer() { }



void 
PFCandidateTrackAnalyzer::beginRun(const edm::Run& run, 
			      const edm::EventSetup & es) {}

void 
PFCandidateTrackAnalyzer::beginJob() {

  nt = f->make<TNtuple>("nt","PF Testing","type:pt:tkptmax:tkptsum:eetmax:eetsum:hetmax:hetsum:nhits:relpterr:algo:nd0:ndz:fake");

}


void 
PFCandidateTrackAnalyzer::analyze(const Event& iEvent, 
			     const EventSetup& iSetup) {
  
  LogDebug("PFCandidateTrackAnalyzer")<<"START event: "<<iEvent.id().event()
			 <<" in run "<<iEvent.id().run()<<endl;

  // get vertices

  edm::Handle<reco::VertexCollection> vtxsH;
  iEvent.getByLabel(inputTagVertices_,vtxsH);
  reco::VertexCollection vtxs = *vtxsH;
  
  // do reco-to-sim association

  edm::Handle<TrackingParticleCollection>  TPCollectionHfake;
  edm::Handle<edm::View<reco::Track> >  trackCollection;
  edm::ESHandle<TrackAssociatorBase> theAssociator;
  const TrackAssociatorByHits *theAssociatorByHits;
  reco::RecoToSimCollection recSimColl;
  
  if(hasSimInfo_) {
    iEvent.getByLabel(inputTagSimTracks_,TPCollectionHfake);
    iEvent.getByLabel(inputTagTracks_,trackCollection);
    iSetup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",theAssociator);
    theAssociatorByHits = (const TrackAssociatorByHits*) theAssociator.product();  
    recSimColl= theAssociatorByHits->associateRecoToSim(trackCollection,TPCollectionHfake,&iEvent);
  }

  // get PFCandidates

  Handle<PFCandidateCollection> pfCandidates;
  iEvent.getByLabel(inputTagPFCandidates_, pfCandidates);

  if(verbose_)cout<<" # of pf cands: "<<pfCandidates->size()<<endl;

  for( unsigned i=0; i<pfCandidates->size(); i++ ) {
    
    max_nhits=0.0;
    max_relpterr=0.0;
    max_algo=0.0;
    max_nd0=0.0;
    max_ndz=0.0;
    max_fake=0.0;

    const reco::PFCandidate& cand = (*pfCandidates)[i];
    
    cand_type = cand.particleId();
    cand_pt = cand.pt();

    //--------- EDIT: Apr 21, 2010 (EAW)
    if(cand_pt < thePtMin_) continue;
    
    if(!(cand_type == PFCandidate::h ||     //type1
	 cand_type == PFCandidate::e ||     //type2
	 cand_type == PFCandidate::mu ||     //type3
	 cand_type == PFCandidate::gamma || //type4
	 cand_type == PFCandidate::h0      //type5
	 )
       ) continue;
    
    // Matt:  Look at only charged hardons for the moment, could be extended to study leptons
    if(!(cand_type == PFCandidate::h)) continue;

    reco::TrackRef trackRef = cand.trackRef();

    //----- track quality selections, MATT:  Already using high Purity hiSelectedTracks by default anyway

    double nhits = trackRef->numberOfValidHits();

    if(nhits<minHits_) continue;
    
    double relpterr = trackRef->ptError()/trackRef->pt();
    if(relpterr > maxPtErr_) continue;
    
    double algo = trackRef->algo();
    if(algo > 7 && pixelSeedOnly_) continue;
    
    double d0 = trackRef->dxy(vtxs[0].position());
    double dz = trackRef->dz(vtxs[0].position());
    
    if(fabs(d0) > maxD0_) continue;
    if(fabs(dz) > maxDZ_) continue;
    
    double d0err = sqrt(trackRef->d0Error()*trackRef->d0Error()+vtxs[0].xError()*vtxs[0].yError());
    double dzerr = sqrt(trackRef->dzError()*trackRef->dzError()+vtxs[0].zError()*vtxs[0].zError());
    
    if(fabs(d0/d0err) > maxD0Norm_) continue;
    if(fabs(dz/dzerr) > maxDZNorm_) continue;

    bool fake=false;
    
    if(hasSimInfo_)
      if(recSimColl.find(edm::RefToBase<reco::Track>(trackRef)) == recSimColl.end())
	fake=true;

    //-----
    if(verbose_)if(fake==true) std::cout<<" fake! "<<std::endl;
    max_trk=0.0, sum_trk=0.0, max_ecal=0.0, sum_ecal=0.0, max_hcal=0.0, sum_hcal=0.0;
    
    
    for(unsigned i=0; i<cand.elementsInBlocks().size(); i++) {

      PFBlockRef blockRef = cand.elementsInBlocks()[i].first;
      
      
      
      unsigned indexInBlock = cand.elementsInBlocks()[i].second;
      const edm::OwnVector<  reco::PFBlockElement>&  elements = (*blockRef).elements();

      //This tells you what type of element it is:
      //cout<<" block type"<<elements[indexInBlock].type()<<endl;
      
      switch (elements[indexInBlock].type()) {
	
      case PFBlockElement::ECAL: {
	reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
	double eet = clusterRef->energy()/cosh(clusterRef->eta());
	if(verbose_)cout<<" ecal energy "<<clusterRef->energy()<<endl;
	sum_ecal+=eet;
	if(eet>max_ecal) max_ecal=eet;
	break;
      }
	
      case PFBlockElement::HCAL: {
	reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
	double eet = clusterRef->energy()/cosh(clusterRef->eta());
	sum_hcal+=eet;
	if(eet>max_hcal) max_hcal=eet;
	if(verbose_)cout<<" hcal energy "<<clusterRef->energy()<<endl;
	break; 
      }       
      case PFBlockElement::TRACK: {
	//This is just the reference to the track itself, since tracks can never be linked to other tracks
	break; 
      }       
      default:
	break;
      }
      // Could do more stuff here, e.g., pre-shower, HF

    }
	
    double trkpt = trackRef->pt();
    cout << "pt=" << trkpt << endl;

    sum_trk+=trkpt;
    if(trkpt>max_trk) {
      max_trk=trkpt;
      max_nhits=nhits;
      max_relpterr=relpterr;
      max_algo=algo;
      max_nd0=d0/d0err;
      max_ndz=dz/dzerr;
      max_fake=fake;
    }
    
    
    
    if( verbose_ ) {
      cout<<cand<<endl;
      // Matt:  I think you need to protect against empty blocks here
      if (printBlocks_) printElementsInBlocks(cand);
    }   
    
    if(verbose_)cout<<" number of elements in blocks "<<cand.elementsInBlocks().size()<<endl;
    




    nt->Fill(cand_type,cand_pt,max_trk,sum_trk,max_ecal,sum_ecal,max_hcal,sum_hcal,max_nhits,max_relpterr,max_algo,max_nd0,max_ndz,max_fake);
    
    //---------
  }
  
 
  LogDebug("PFCandidateTrackAnalyzer")<<"STOP event: "<<iEvent.id().event()
			 <<" in run "<<iEvent.id().run()<<endl;
}




void PFCandidateTrackAnalyzer::printElementsInBlocks(const PFCandidate& cand,
						ostream& out) const {
  if(!out) return;

  PFBlockRef firstRef;

  assert(!cand.elementsInBlocks().empty() );
  for(unsigned i=0; i<cand.elementsInBlocks().size(); i++) {
    PFBlockRef blockRef = cand.elementsInBlocks()[i].first;

    if(blockRef.isNull()) {
      cerr<<"ERROR! no block ref!";
      continue;
    }

    if(!i) {
      out<<(*blockRef);
      firstRef = blockRef;
    }
    else if( blockRef!=firstRef) {
      cerr<<"WARNING! This hiPFCandidate is not made from a single block"<<endl;
    }
 
    out<<"\t"<<cand.elementsInBlocks()[i].second<<endl;
  }
}


DEFINE_FWK_MODULE(PFCandidateTrackAnalyzer);
