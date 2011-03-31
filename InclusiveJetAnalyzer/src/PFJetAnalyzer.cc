/*
  Based on the jet response analyzer
  Modified by Matt Nguyen, November 2010

*/

#include "MNguyen/InclusiveJetAnalyzer/interface/PFJetAnalyzer.h"


#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
//#include <Math/DistFunc.h>
#include "TMath.h"



#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"


#include "DataFormats/HeavyIonEvent/interface/CentralityBins.h"

#include "DataFormats/CaloTowers/interface/CaloTowerCollection.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticleFwd.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "DataFormats/RecoCandidate/interface/TrackAssociation.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorByHits.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowReco/interface/PFBlock.h"
#include "DataFormats/ParticleFlowReco/interface/PFCluster.h"
#include "DataFormats/ParticleFlowReco/interface/PFClusterFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

using namespace std;
using namespace edm;
using namespace reco;


PFJetAnalyzer::PFJetAnalyzer(const edm::ParameterSet& iConfig) {
  

  jetTag_ = iConfig.getParameter<InputTag>("jetTag");
  jetTag2_ = iConfig.getParameter<InputTag>("jetTag2");
  jetTag3_ = iConfig.getParameter<InputTag>("jetTag3");
  jetTag4_ = iConfig.getParameter<InputTag>("jetTag4");


  recoJetTag_ = iConfig.getParameter<InputTag>("recoJetTag");
  recoJetTag2_ = iConfig.getParameter<InputTag>("recoJetTag2");
  recoJetTag3_ = iConfig.getParameter<InputTag>("recoJetTag3");
  recoJetTag4_ = iConfig.getParameter<InputTag>("recoJetTag4");

  pfCandidatesTag_ = iConfig.getParameter<InputTag>("pfCandidatesTag");
  trackTag_ = iConfig.getParameter<edm::InputTag>("trackTag");

  verbose_ = iConfig.getUntrackedParameter<bool>("verbose",false);

  useCentrality_ = iConfig.getUntrackedParameter<bool>("useCentrality",false);
  isMC_ = iConfig.getUntrackedParameter<bool>("isMC",false);

  genParticleTag_ = iConfig.getParameter<InputTag>("genParticleTag");
  eventInfoTag_ = iConfig.getParameter<InputTag>("eventInfoTag");

  hasSimInfo_ = iConfig.getUntrackedParameter<bool>("hasSimInfo");
  simTracksTag_ = iConfig.getParameter<InputTag>("SimTracks");

  cout<<" tracks : "<<trackTag_<<endl;
  cout<<" jet collection : "<<jetTag_<<endl;
  cout<<" jet collection 2: "<<jetTag2_<<endl;
  cout<<" jet collection 3: "<<jetTag3_<<endl;
  cout<<" jet collection 4: "<<jetTag4_<<endl;

   jets_.nj1 = 0;
   jets_.nj2 = 0;
   jets_.nj3 = 0;
   jets_.nj4 = 0;
   jets_.nPFcand = 0;
   jets_.ntrack = 0;
   jets_.ngenp = 0;


   
}



PFJetAnalyzer::~PFJetAnalyzer() { }



void 
PFJetAnalyzer::beginRun(const edm::Run& run, 
			      const edm::EventSetup & es) {}

void 
PFJetAnalyzer::beginJob() {

  centrality_ = 0;

  //string jetTagName = jetTag_.label()+"_tree"; 

  //string jetTagTitle = jetTag_.label()+" Jet Analysis Tree"; 
  //t = f->make<TTree>(jetTagName.c_str(),jetTagTitle.c_str());
  t = fs2->make<TTree>("t","Jet Analysis Tree");


  //  TTree* t= new TTree("t","Jet Response Analyzer");
  t->Branch("run",&jets_.run,"run/I");
  t->Branch("evt",&jets_.evt,"evt/I");
  t->Branch("lumi",&jets_.lumi,"lumi/I");
  t->Branch("b",&jets_.b,"b/F");
  t->Branch("vx",&jets_.vx,"vx/F");
  t->Branch("vy",&jets_.vy,"vy/F");
  t->Branch("vz",&jets_.vz,"vz/F");
  t->Branch("hf",&jets_.hf,"hf/F");
  t->Branch("bin",&jets_.bin,"bin/I");

  // ICPU5
  t->Branch("nj1",&jets_.nj1,"nj1/I");
  t->Branch("nj2",&jets_.nj2,"nj2/I");
  t->Branch("nj3",&jets_.nj3,"nj3/I");
  t->Branch("nj4",&jets_.nj4,"nj4/I");

  t->Branch("rawpt_j1",jets_.rawpt_j1,"rawpt_j1[nj1]/F");
  t->Branch("jtpt_j1",jets_.jtpt_j1,"jtpt_j1[nj1]/F");
  t->Branch("jteta_j1",jets_.jteta_j1,"jteta_j1[nj1]/F");
  t->Branch("jty_j1",jets_.jty_j1,"jty_j1[nj1]/F");
  t->Branch("jtphi_j1",jets_.jtphi_j1,"jtphi_j1[nj1]/F");
  t->Branch("preL1et_j1",jets_.preL1et_j1,"preL1et_j1[nj1]/F");
  t->Branch("L2_j1",jets_.L2_j1,"L2_j1[nj1]/F");
  t->Branch("L3_j1",jets_.L3_j1,"L3_j1[nj1]/F");
  t->Branch("area_j1",jets_.area_j1,"area_j1[nj1]/F");

  if(isMC_){
    t->Branch("refpt_j1",jets_.refpt_j1,"refpt_j1[nj1]/F");
    t->Branch("refeta_j1",jets_.refeta_j1,"refeta_j1[nj1]/F");
    t->Branch("refy_j1",jets_.refy_j1,"refy_j1[nj1]/F");
    t->Branch("refphi_j1",jets_.refphi_j1,"refphi_j1[nj1]/F");
    t->Branch("refdrjt_j1",jets_.refdrjt_j1,"refdrjt_j1[nj1]/F");
  }


  // J2

  t->Branch("rawpt_j2",jets_.rawpt_j2,"rawpt_j2[nj2]/F");
  t->Branch("jtpt_j2",jets_.jtpt_j2,"jtpt_j2[nj2]/F");
  t->Branch("jteta_j2",jets_.jteta_j2,"jteta_j2[nj2]/F");
  t->Branch("jty_j2",jets_.jty_j2,"jty_j2[nj2]/F");
  t->Branch("jtphi_j2",jets_.jtphi_j2,"jtphi_j2[nj2]/F");
  t->Branch("preL1et_j2",jets_.preL1et_j2,"preL1et_j2[nj2]/F");
  t->Branch("L2_j2",jets_.L2_j2,"L2_j2[nj2]/F");
  t->Branch("L3_j2",jets_.L3_j2,"L3_j2[nj2]/F");
  t->Branch("area_j2",jets_.area_j2,"area_j2[nj2]/F");

  if(isMC_){
    t->Branch("refpt_j2",jets_.refpt_j2,"refpt_j2[nj2]/F");
    t->Branch("refeta_j2",jets_.refeta_j2,"refeta_j2[nj2]/F");
    t->Branch("refy_j2",jets_.refy_j2,"refy_j2[nj2]/F");
    t->Branch("refphi_j2",jets_.refphi_j2,"refphi_j2[nj2]/F");
    t->Branch("refdrjt_j2",jets_.refdrjt_j2,"refdrjt_j2[nj2]/F");
  }




  // J3

  t->Branch("rawpt_j3",jets_.rawpt_j3,"rawpt_j3[nj3]/F");
  t->Branch("jtpt_j3",jets_.jtpt_j3,"jtpt_j3[nj3]/F");
  t->Branch("jteta_j3",jets_.jteta_j3,"jteta_j3[nj3]/F");
  t->Branch("jty_j3",jets_.jty_j3,"jty_j3[nj3]/F");
  t->Branch("jtphi_j3",jets_.jtphi_j3,"jtphi_j3[nj3]/F");
  t->Branch("preL1et_j3",jets_.preL1et_j3,"preL1et_j3[nj3]/F");
  t->Branch("L2_j3",jets_.L2_j3,"L2_j3[nj3]/F");
  t->Branch("L3_j3",jets_.L3_j3,"L3_j3[nj3]/F");
  t->Branch("area_j3",jets_.area_j3,"area_j3[nj3]/F");

  if(isMC_){
    t->Branch("refpt_j3",jets_.refpt_j3,"refpt_j3[nj3]/F");
    t->Branch("refeta_j3",jets_.refeta_j3,"refeta_j3[nj3]/F");
    t->Branch("refy_j3",jets_.refy_j3,"refy_j3[nj3]/F");
    t->Branch("refphi_j3",jets_.refphi_j3,"refphi_j3[nj3]/F");
    t->Branch("refdrjt_j3",jets_.refdrjt_j3,"refdrjt_j3[nj3]/F");
  }

 // J4

  t->Branch("rawpt_j4",jets_.rawpt_j4,"rawpt_j4[nj4]/F");
  t->Branch("jtpt_j4",jets_.jtpt_j4,"jtpt_j4[nj4]/F");
  t->Branch("jteta_j4",jets_.jteta_j4,"jteta_j4[nj4]/F");
  t->Branch("jty_j4",jets_.jty_j4,"jty_j4[nj4]/F");
  t->Branch("jtphi_j4",jets_.jtphi_j4,"jtphi_j4[nj4]/F");
  t->Branch("preL1et_j4",jets_.preL1et_j4,"preL1et_j4[nj4]/F");
  t->Branch("L2_j4",jets_.L2_j4,"L2_j4[nj4]/F");
  t->Branch("L3_j4",jets_.L3_j4,"L3_j4[nj4]/F");
  t->Branch("area_j4",jets_.area_j4,"area_j4[nj4]/F");

  if(isMC_){
    t->Branch("refpt_j4",jets_.refpt_j4,"refpt_j4[nj4]/F");
    t->Branch("refeta_j4",jets_.refeta_j4,"refeta_j4[nj4]/F");
    t->Branch("refy_j4",jets_.refy_j4,"refy_j4[nj4]/F");
    t->Branch("refphi_j4",jets_.refphi_j4,"refphi_j4[nj4]/F");
    t->Branch("refdrjt_j4",jets_.refdrjt_j4,"refdrjt_j4[nj4]/F");
  }

  t->Branch("nPFcand",&jets_.nPFcand,"nPFcand/I");
  t->Branch("candId",jets_.candId,"candId[nPFcand]/I");
  t->Branch("candpt",jets_.candpt,"candpt[nPFcand]/F");
  t->Branch("candeta",jets_.candeta,"candeta[nPFcand]/F");
  //t->Branch("candy",jets_.candy,"candy[nPFcand]/F");
  t->Branch("candphi",jets_.candphi,"candphi[nPFcand]/F");

  t->Branch("ntrack",&jets_.ntrack,"ntrack/I");
  t->Branch("tracknhits",jets_.tracknhits,"tracknhits[ntrack]/I");
  t->Branch("trackpt",jets_.trackpt,"trackpt[ntrack]/F");
  t->Branch("tracketa",jets_.tracketa,"tracketa[ntrack]/F");
  t->Branch("trackphi",jets_.trackphi,"trackphi[ntrack]/F");
  t->Branch("tracksumecal",jets_.tracksumecal,"tracksumecal[ntrack]/F");
  t->Branch("tracksumhcal",jets_.tracksumhcal,"tracksumhcal[ntrack]/F");
  t->Branch("trackqual",jets_.trackqual,"trackqual[ntrack]/I");

  if(isMC_){
    t->Branch("pthat",&jets_.pthat,"pthat/F");    
    t->Branch("trackfake",jets_.trackfake,"trackfake[ntrack]/I");
    t->Branch("parton1_flavor",&jets_.parton1_flavor,"parton1_flavor/I");
    t->Branch("parton2_flavor",&jets_.parton2_flavor,"parton2_flavor/I");
    t->Branch("parton1_pt",&jets_.parton1_pt,"parton1_pt/F");
    t->Branch("parton2_pt",&jets_.parton2_pt,"parton2_pt/F");
    t->Branch("parton2_eta",&jets_.parton2_eta,"parton2_eta/F");
    t->Branch("parton1_eta",&jets_.parton1_eta,"parton1_eta/F");
    t->Branch("parton2_phi",&jets_.parton2_phi,"parton2_phi/F");
    t->Branch("parton1_phi",&jets_.parton1_phi,"parton1_phi/F");
    t->Branch("parton1_y",&jets_.parton1_y,"parton1_y/F");
    t->Branch("parton2_y",&jets_.parton2_y,"parton2_y/F");

    t->Branch("ngenp",&jets_.ngenp,"ngenp/I");
    t->Branch("genppdgId",jets_.genppdgId,"genppdgId[ngenp]/I");
    t->Branch("genppt",jets_.genppt,"genppt[ngenp]/F");
    t->Branch("genpeta",jets_.genpeta,"genpeta[ngenp]/F");
    t->Branch("genpphi",jets_.genpphi,"genpphi[ngenp]/F");

  }
  

  TH1D::SetDefaultSumw2();
  
  
}


void 
PFJetAnalyzer::analyze(const Event& iEvent, 
			     const EventSetup& iSetup) {
  


  int event = iEvent.id().event();
  int run = iEvent.id().run();
  int lumi = iEvent.id().luminosityBlock();

  jets_.run = run;
  jets_.evt = event;
  jets_.lumi = lumi;

  LogDebug("PFJetAnalyzer")<<"START event: "<<event<<" in run "<<run<<endl;
  

  edm::Handle<vector<reco::Vertex> >vertex;
  iEvent.getByLabel(edm::InputTag("hiSelectedVertex"), vertex);  
 
  if(vertex->size()>0) {
    jets_.vx=vertex->begin()->x();
    jets_.vy=vertex->begin()->x();
    jets_.vz=vertex->begin()->z();
  }

 int bin = -1;
  double hf = 0.;
  double b = 999.;

  if(useCentrality_){
    //if(!isMC_){

      if(!centrality_) centrality_ = new CentralityProvider(iSetup);      
      centrality_->newEvent(iEvent,iSetup); // make sure you do this first in every event
      //double c = centrality_->centralityValue();
      const reco::Centrality *cent = centrality_->raw();
      
      hf = cent->EtHFhitSum();
      
      bin = centrality_->getBin();
      b = centrality_->bMean();
      //}
      /*
    else{
      TFile * centFile = new TFile("/net/hidsk0001/d00/scratch/mnguyen/CMSSW_3_9_1_patch1/src/macros/Hydjet_CentTable.root");

      edm::Handle<reco::Centrality> cent;
      iEvent.getByLabel(edm::InputTag("hiCentrality"),cent);
      //cout<<" grabbed centrality "<<endl;
      CentralityBins::RunMap cmap = getCentralityFromFile(centFile, "makeCentralityTableTFile", "HFhitsHydjet_2760GeV", 1, 1);

      // Still getting cent from hits.  When tower tables become available, we need to switch
      hf = cent->EtHFhitSum();
      //cout<<" hf "<<hf<<endl;
      bin = cmap[run]->getBin(hf);
      b = cmap[run]->bMeanOfBin(bin);
    }
      */
  }
   


  // not used, taking all jet
  //double jetPtMin = 35.;


   // loop the events
   
   jets_.bin = bin;
   //jets_.hf = hf;
   

   edm::Handle<pat::JetCollection> jets;
   iEvent.getByLabel(jetTag_, jets);

   // add a few more jet algos to the same ntuple
   // default is j1

   edm::Handle<pat::JetCollection> jets2;
   iEvent.getByLabel(jetTag2_, jets2);

   edm::Handle<pat::JetCollection> jets3;
   iEvent.getByLabel(jetTag3_, jets3);

   edm::Handle<pat::JetCollection> jets4;
   iEvent.getByLabel(jetTag4_, jets4);


   edm::Handle< edm::View<reco::CaloJet> > recoJetColl;
   iEvent.getByLabel( recoJetTag_, recoJetColl );
   
   edm::Handle< edm::View<reco::PFJet> > recoJetColl2;
   iEvent.getByLabel(recoJetTag2_, recoJetColl2 );

   edm::Handle< edm::View<reco::PFJet> > recoJetColl3;
   iEvent.getByLabel(recoJetTag3_, recoJetColl3 );

   edm::Handle< edm::View<reco::CaloJet> > recoJetColl4;
   iEvent.getByLabel(recoJetTag4_, recoJetColl4 );


   Handle<PFCandidateCollection> pfCandidates;
   iEvent.getByLabel(pfCandidatesTag_, pfCandidates);


   Handle<vector<Track> > tracks;
   iEvent.getByLabel(trackTag_, tracks);
   
  // do reco-to-sim association

  Handle<TrackingParticleCollection>  TPCollectionHfake;
  Handle<edm::View<reco::Track> >  trackCollection;
  ESHandle<TrackAssociatorBase> theAssociator;
  const TrackAssociatorByHits *theAssociatorByHits;
  reco::RecoToSimCollection recSimColl;
  
  if(hasSimInfo_) {
    iEvent.getByLabel(simTracksTag_,TPCollectionHfake);
    iEvent.getByLabel(trackTag_,trackCollection);
    iSetup.get<TrackAssociatorRecord>().get("TrackAssociatorByHits",theAssociator);
    theAssociatorByHits = (const TrackAssociatorByHits*) theAssociator.product();  
    recSimColl= theAssociatorByHits->associateRecoToSim(trackCollection,TPCollectionHfake,&iEvent);
  }


   
   // FILL JRA TREE

   jets_.b = b;



   jets_.nj1 = 0;
   

   
   for(unsigned int j = 0 ; j < jets->size(); ++j){
     const pat::Jet& jet = (*jets)[j];
     
     //cout<<" jet pt "<<jet.pt()<<endl;
     //if(jet.pt() < jetPtMin) continue;
     jets_.rawpt_j1[jets_.nj1]=jet.correctedJet("Uncorrected").pt();
     jets_.jtpt_j1[jets_.nj1] = jet.pt();                            
     jets_.jteta_j1[jets_.nj1] = jet.eta();
     jets_.jtphi_j1[jets_.nj1] = jet.phi();
     jets_.jty_j1[jets_.nj1] = jet.eta();


     //  cout<<" abs corr "<<jet.corrFactor("L3Absolute")<<endl;
     //cout<<" abs corr "<<jet.corrFactor("L3Absolute")<<endl;


     float L2Corr = jet.correctedJet("L2Relative").pt()/jet.correctedJet("Uncorrected").pt();
     float L3Corr = jet.correctedJet("L3Absolute").pt()/jet.correctedJet("L2Relative").pt();
     
     
     jets_.L2_j1[jets_.nj1] = L2Corr;
     jets_.L3_j1[jets_.nj1] = L3Corr;
     
     
     jets_.area_j1[jets_.nj1] = jet.jetArea();
     
     // Match to reco jet to find unsubtracted jet energy
     
     if(1==0){
	 int recoJetSize = recoJetColl->size();
	 
	 jets_.preL1et_j1[jets_.nj1] = -1;
	 
	 //cout<<" patJet_eta "<<jet.eta()<<" patJet_phi "<<jet.phi()<<" patJet_et "<<jet.et()<<endl;
	 
	 for(int iRecoJet = 0; iRecoJet < recoJetSize; ++iRecoJet){
	   
	   reco::CaloJet recoJet = ((*recoJetColl)[iRecoJet]);
	   
	   
	   double recoJet_eta = recoJet.eta();
	   double recoJet_phi = recoJet.phi();
	   //cout<<" recoJet_eta "<<recoJet_eta<<" recoJet_phi "<<recoJet_phi<<" recoJet_et "<<recoJet.et()<<endl;
	   
	   
	   if(fabs(recoJet.eta()-jet.eta()) < 0.001
	      && fabs(acos(cos((recoJet.phi()-jet.phi())))) < 0.001)
	     {
	       jets_.preL1et_j1[jets_.nj1] = recoJet.et();
	       
	       //cout<<"Match found,  recoJet.et "<<recoJet.et()<< " recoJet.eta "<<jet.eta()<<" recoJet.phi "<<recoJet.phi()<<endl;
	       break;
	     }
	 }
	 if(jets_.preL1et_j1[jets_.nj1] == -1){
	   
	   
	   //There's a known issue here.  If the background subtraction oversubtracts I've set the patJet.et() to zero.  That would be fine if I had also set the eta and phi.  We could then recover the pre-subtracted energy.  However, I never bothered to set the eta and phi for theses jets (doh!).  Next time I repass the data I won't be so stupid.
	   
	   
	   
	   if(jet.et()>0)cout<<"Match *NOT* found,  patJet.et "<<jet.et()<< " patJet.eta "<<jet.eta()<<" patJet.phi() "<<jet.phi()<<endl;
	 }
       }
     if(isMC_){
       
       
       if(jet.genJet()!=0 && jet.genJet()->pt()>1.0 && jet.genJet()->pt()<999999){
	 jets_.refpt_j1[jets_.nj1] = jet.genJet()->pt();
	 jets_.refeta_j1[jets_.nj1] = jet.genJet()->eta();
	 jets_.refphi_j1[jets_.nj1] = jet.genJet()->phi();
	 jets_.refy_j1[jets_.nj1] = jet.genJet()->eta();
	 
	 jets_.refdrjt_j1[jets_.nj1] = reco::deltaR(jet,*(jet.genJet()));
       }        
       else{
	 jets_.refpt_j1[jets_.nj1] = 0;
	 jets_.refeta_j1[jets_.nj1] = -999;
	 jets_.refphi_j1[jets_.nj1] = -999;
	 jets_.refy_j1[jets_.nj1] = -999;
       }
       
     }



       jets_.nj1++;
       
       
   }






   jets_.nj2 = 0;
   

   
   for(unsigned int j = 0 ; j < jets2->size(); ++j){
     const pat::Jet& jet2 = (*jets2)[j];

     //cout<<" jet pt "<<jet2.pt()<<endl;
     //if(jet2.pt() < jetPtMin) continue;
     jets_.rawpt_j2[jets_.nj2]=jet2.correctedJet("Uncorrected").pt();
     jets_.jtpt_j2[jets_.nj2] = jet2.pt();                            
     jets_.jteta_j2[jets_.nj2] = jet2.eta();
     jets_.jtphi_j2[jets_.nj2] = jet2.phi();
     jets_.jty_j2[jets_.nj2] = jet2.eta();
     //  cout<<" abs corr "<<jet2.corrFactor("L3Absolute")<<endl;
     //cout<<" abs corr "<<jet2.corrFactor("L3Absolute")<<endl;


       float L2Corr = jet2.correctedJet("L2Relative").pt()/jet2.correctedJet("Uncorrected").pt();
       float L3Corr = jet2.correctedJet("L3Absolute").pt()/jet2.correctedJet("L2Relative").pt();
       

       jets_.L2_j2[jets_.nj2] = L2Corr;
       jets_.L3_j2[jets_.nj2] = L3Corr;

       jets_.area_j2[jets_.nj2] = jet2.jetArea();

       // Match to reco jet to find unsubtracted jet energy
       if(1==0){
	 int recoJetSize2 = recoJetColl2->size();
	 
	 jets_.preL1et_j2[jets_.nj2] = -1;
	 
	 //cout<<" patJet_eta "<<jet2.eta()<<" patJet_phi "<<jet2.phi()<<" patJet_et "<<jet2.et()<<endl;
	 
	 for(int iRecoJet = 0; iRecoJet < recoJetSize2; ++iRecoJet){
	   
	   reco::PFJet recoJet2 = ((*recoJetColl2)[iRecoJet]);
	   
	   
	   double recoJet_eta = recoJet2.eta();
	   double recoJet_phi = recoJet2.phi();
	   //cout<<" recoJet_eta "<<recoJet_eta<<" recoJet_phi "<<recoJet_phi<<" recoJet_et "<<recoJet2.et()<<endl;
	   
	   
	   if(fabs(recoJet2.eta()-jet2.eta()) < 0.001
	      && fabs(acos(cos((recoJet2.phi()-jet2.phi())))) < 0.001)
	     {
	       jets_.preL1et_j2[jets_.nj2] = recoJet2.et();
	       
	       //cout<<"Match found,  recoJet2.et "<<recoJet2.et()<< " recoJet2.eta "<<jet2.eta()<<" recoJet2.phi "<<recoJet2.phi()<<endl;
	       break;
	     }
	 }
	 if(jets_.preL1et_j2[jets_.nj2] == -1){
	   
	   
	   //There's a known issue here.  If the background subtraction oversubtracts I've set the patJet.et() to zero.  That would be fine if I had also set the eta and phi.  We could then recover the pre-subtracted energy.  However, I never bothered to set the eta and phi for theses jets (doh!).  Next time I repass the data I won't be so stupid.
	   
	   
	   
	   if(jet2.et()>0)cout<<"Match *NOT* found,  patJet2.et "<<jet2.et()<< " patJet2.eta "<<jet2.eta()<<" patJet2.phi() "<<jet2.phi()<<endl;
	 }
       }
     if(isMC_){
       
       
       if(jet2.genJet()!=0 && jet2.genJet()->pt()>1.0 && jet2.genJet()->pt()<999999){
	 jets_.refpt_j2[jets_.nj2] = jet2.genJet()->pt();
	 jets_.refeta_j2[jets_.nj2] = jet2.genJet()->eta();
	 jets_.refphi_j2[jets_.nj2] = jet2.genJet()->phi();
	 jets_.refy_j2[jets_.nj2] = jet2.genJet()->eta();
	 
	 jets_.refdrjt_j2[jets_.nj2] = reco::deltaR(jet2,*(jet2.genJet()));
       }        
       else{
	 jets_.refpt_j2[jets_.nj2] = 0;
	 jets_.refeta_j2[jets_.nj2] = -999;
	 jets_.refphi_j2[jets_.nj2] = -999;
	 jets_.refy_j2[jets_.nj2] = -999;
       }
       
     }

     
     jets_.nj2++;
     
   }
   

   jets_.nj3 = 0;
   
   //cout<<" jets size "<<jets->size()<<endl;

   
   for(unsigned int j = 0 ; j < jets3->size(); ++j){
     const pat::Jet& jet3 = (*jets3)[j];
     
     //cout<<" jet pt "<<jet3.pt()<<endl;
     //if(jet3.pt() < jetPtMin) continue;
     jets_.rawpt_j3[jets_.nj3]=jet3.correctedJet("Uncorrected").pt();
     jets_.jtpt_j3[jets_.nj3] = jet3.pt();                            
     jets_.jteta_j3[jets_.nj3] = jet3.eta();
     jets_.jtphi_j3[jets_.nj3] = jet3.phi();
     jets_.jty_j3[jets_.nj3] = jet3.eta();
     //  cout<<" abs corr "<<jet3.corrFactor("L3Absolute")<<endl;
     //cout<<" abs corr "<<jet3.corrFactor("L3Absolute")<<endl;



       float L2Corr = jet3.correctedJet("L2Relative").pt()/jet3.correctedJet("Uncorrected").pt();
       float L3Corr = jet3.correctedJet("L3Absolute").pt()/jet3.correctedJet("L2Relative").pt();
       

       jets_.L2_j3[jets_.nj3] = L2Corr;
       jets_.L3_j3[jets_.nj3] = L3Corr;

       jets_.area_j3[jets_.nj3] = jet3.jetArea();

       // Match to reco jet to find unsubtracted jet energy
       if(1==0){
       int recoJetSize3 = recoJetColl3->size();
       
       jets_.preL1et_j3[jets_.nj3] = -1;
       
       //cout<<" patJet_eta "<<jet3.eta()<<" patJet_phi "<<jet3.phi()<<" patJet_et "<<jet3.et()<<endl;

       for(int iRecoJet = 0; iRecoJet < recoJetSize3; ++iRecoJet){
	 
	 reco::PFJet recoJet3 = ((*recoJetColl3)[iRecoJet]);
	 
	   
	 double recoJet_eta = recoJet3.eta();
	 double recoJet_phi = recoJet3.phi();
	 //cout<<" recoJet_eta "<<recoJet_eta<<" recoJet_phi "<<recoJet_phi<<" recoJet_et "<<recoJet3.et()<<endl;
	 
	 
	 if(fabs(recoJet3.eta()-jet3.eta()) < 0.001
	    && fabs(acos(cos((recoJet3.phi()-jet3.phi())))) < 0.001)
	   {
	     jets_.preL1et_j3[jets_.nj3] = recoJet3.et();
	     
	     //cout<<"Match found,  recoJet3.et "<<recoJet3.et()<< " recoJet3.eta "<<jet3.eta()<<" recoJet3.phi "<<recoJet3.phi()<<endl;
	     break;
	   }
       }
       if(jets_.preL1et_j3[jets_.nj3] == -1){

	 
	 //  There's a known issue here.  If the background subtraction oversubtracts I've set the patJet.et() to zero.  That would be fine if I had also set the eta and phi.  We could then recover the pre-subtracted energy.  However, I never bothered to set the eta and phi for theses jets (doh!).  Next time I repass the data I won't be so stupid.
	 

	 
	 if(jet3.et()>0)cout<<"Match *NOT* found,  patJet3.et "<<jet3.et()<< " patJet3.eta "<<jet3.eta()<<" patJet3.phi() "<<jet3.phi()<<endl;
       }
       }
     if(isMC_){
       
       
       if(jet3.genJet()!=0 && jet3.genJet()->pt()>1.0 && jet3.genJet()->pt()<999999){
	 jets_.refpt_j3[jets_.nj3] = jet3.genJet()->pt();
	 jets_.refeta_j3[jets_.nj3] = jet3.genJet()->eta();
	 jets_.refphi_j3[jets_.nj3] = jet3.genJet()->phi();
	 jets_.refy_j3[jets_.nj3] = jet3.genJet()->eta();
	 
	 jets_.refdrjt_j3[jets_.nj3] = reco::deltaR(jet3,*(jet3.genJet()));
       }        
       else{
	 jets_.refpt_j3[jets_.nj3] = 0;
	 jets_.refeta_j3[jets_.nj3] = -999;
	 jets_.refphi_j3[jets_.nj3] = -999;
	 jets_.refy_j3[jets_.nj3] = -999;
       }
       
     }



       jets_.nj3++;
       
       
   }





   jets_.nj4 = 0;
   

   
   for(unsigned int j = 0 ; j < jets4->size(); ++j){
     const pat::Jet& jet4 = (*jets4)[j];

     //cout<<" jet pt "<<jet4.pt()<<endl;
     //if(jet4.pt() < jetPtMin) continue;
     jets_.rawpt_j4[jets_.nj4]=jet4.correctedJet("Uncorrected").pt();
     jets_.jtpt_j4[jets_.nj4] = jet4.pt();                            
     jets_.jteta_j4[jets_.nj4] = jet4.eta();
     jets_.jtphi_j4[jets_.nj4] = jet4.phi();
     jets_.jty_j4[jets_.nj4] = jet4.eta();
     //  cout<<" abs corr "<<jet4.corrFactor("L3Absolute")<<endl;
     //cout<<" abs corr "<<jet4.corrFactor("L3Absolute")<<endl;


       float L2Corr = jet4.correctedJet("L2Relative").pt()/jet4.correctedJet("Uncorrected").pt();
       float L3Corr = jet4.correctedJet("L3Absolute").pt()/jet4.correctedJet("L2Relative").pt();
       

       jets_.L2_j4[jets_.nj4] = L2Corr;
       jets_.L3_j4[jets_.nj4] = L3Corr;

       jets_.area_j4[jets_.nj4] = jet4.jetArea();

       // Match to reco jet to find unsubtracted jet energy
       if(1==0){
	 int recoJetSize4 = recoJetColl4->size();
	 
	 jets_.preL1et_j4[jets_.nj4] = -1;
	 
	 //cout<<" patJet_eta "<<jet4.eta()<<" patJet_phi "<<jet4.phi()<<" patJet_et "<<jet4.et()<<endl;
	 
	 for(int iRecoJet = 0; iRecoJet < recoJetSize4; ++iRecoJet){
	   
	   reco::CaloJet recoJet4 = ((*recoJetColl4)[iRecoJet]);
	   
	   
	   double recoJet_eta = recoJet4.eta();
	   double recoJet_phi = recoJet4.phi();
	   //cout<<" recoJet_eta "<<recoJet_eta<<" recoJet_phi "<<recoJet_phi<<" recoJet_et "<<recoJet4.et()<<endl;
	   
	   
	   if(fabs(recoJet4.eta()-jet4.eta()) < 0.001
	      && fabs(acos(cos((recoJet4.phi()-jet4.phi())))) < 0.001)
	     {
	       jets_.preL1et_j4[jets_.nj4] = recoJet4.et();
	       
	       //cout<<"Match found,  recoJet4.et "<<recoJet4.et()<< " recoJet4.eta "<<jet4.eta()<<" recoJet4.phi "<<recoJet4.phi()<<endl;
	       break;
	     }
	 }
	 if(jets_.preL1et_j4[jets_.nj4] == -1){
	   
	   
	   //There's a known issue here.  If the background subtraction oversubtracts I've set the patJet.et() to zero.  That would be fine if I had also set the eta and phi.  We could then recover the pre-subtracted energy.  However, I never bothered to set the eta and phi for theses jets (doh!).  Next time I repass the data I won't be so stupid.
	   
	   
	   
	   if(jet4.et()>0)cout<<"Match *NOT* found,  patJet4.et "<<jet4.et()<< " patJet4.eta "<<jet4.eta()<<" patJet4.phi() "<<jet4.phi()<<endl;
	 }
       }
     if(isMC_){
       
       
       if(jet4.genJet()!=0 && jet4.genJet()->pt()>1.0 && jet4.genJet()->pt()<999999){
	 jets_.refpt_j4[jets_.nj4] = jet4.genJet()->pt();
	 jets_.refeta_j4[jets_.nj4] = jet4.genJet()->eta();
	 jets_.refphi_j4[jets_.nj4] = jet4.genJet()->phi();
	 jets_.refy_j4[jets_.nj4] = jet4.genJet()->eta();
	 
	 jets_.refdrjt_j4[jets_.nj4] = reco::deltaR(jet4,*(jet4.genJet()));
       }        
       else{
	 jets_.refpt_j4[jets_.nj4] = 0;
	 jets_.refeta_j4[jets_.nj4] = -999;
	 jets_.refphi_j4[jets_.nj4] = -999;
	 jets_.refy_j4[jets_.nj4] = -999;
       }
       
     }

     
     jets_.nj4++;
     
   }



   for( unsigned icand=0; icand<pfCandidates->size(); icand++ ) {
     
     const reco::PFCandidate& cand = (*pfCandidates)[icand];
     
       float particleEta = cand.eta();
       
       //if(fabs(particleEta)>2.5) continue;
       
      

      // PF PId Convention:
      // 1 = Charged Hadrons
      // 2 = Electrons (not included)
      // 3 = Muons
      // 4 = Photons
      // 5 = Neutral Hadrons




       int particleId = (int)cand.particleId();
       float particlePt = cand.pt();
       
       if(particlePt<0.3) continue;
       

       // can use varid thresholds if we want
       //if(particleId==1 && particlePt < 0.9) continue;
       //if(particleId==3 && particlePt < 0.9) continue;
       //if(particleId==4 && particlePt < 0.3) continue;
       //if(particleId==5 && particlePt < 0.9) continue;

       
       

       jets_.candId[jets_.nPFcand] = particleId;
       jets_.candpt[jets_.nPFcand] = particlePt;
       jets_.candeta[jets_.nPFcand] = particleEta;
       jets_.candphi[jets_.nPFcand] = cand.phi();
       //jets_.candy[jets_.nPFcand] = cand.y();
       jets_.nPFcand++;

       //cout<<" jets_.nPFcand "<<jets_.nPFcand<<endl;
   }
   
   //cout<<" ntracks: "<<tracks->size()<<endl;

   for(unsigned int it=0; it<tracks->size(); ++it){
     const reco::Track & track = (*tracks)[it];

     // Could makes some track selection here
     jets_.tracknhits[jets_.ntrack] = track.numberOfValidHits();
     jets_.trackpt[jets_.ntrack] = track.pt();
     jets_.tracketa[jets_.ntrack] = track.eta();
     jets_.trackphi[jets_.ntrack] = track.phi();

     jets_.tracksumecal[jets_.ntrack] = 0.;
     jets_.tracksumhcal[jets_.ntrack] = 0.;

     reco::TrackBase::TrackQuality trackQualityTight = TrackBase::qualityByName("highPurity");
     jets_.trackqual[jets_.ntrack]=(int)track.quality(trackQualityTight);

     jets_.trackfake[jets_.ntrack]=0;

     reco::TrackRef trackRef=reco::TrackRef(tracks,it);


     if(hasSimInfo_)
       if(recSimColl.find(edm::RefToBase<reco::Track>(trackRef)) == recSimColl.end())
	 jets_.trackfake[jets_.ntrack]=1;
     


     int pfCandMatchFound = 0;

     // loop over pf candidates to get calo-track matching info	
       for( unsigned icand=0; icand<pfCandidates->size(); icand++ ) {
	 
	 const reco::PFCandidate& cand = (*pfCandidates)[icand];
    
	 float cand_type = cand.particleId();

	 // only charged hadrons and leptons can be asscociated with a track
	 if(!(cand_type == PFCandidate::h ||     //type1
	      cand_type == PFCandidate::e ||     //type2
	      cand_type == PFCandidate::mu      //type3
	      )
	    ) continue;
	 

	 // if working with 2 different track collections this doesn't work
	 if(cand.trackRef() != trackRef) continue;
	  //if(fabs(cand.pt()-track.pt())>0.001||fabs(cand.eta()-track.eta())>0.001||fabs(acos(cos(cand.phi()-track.phi())))>0.001) continue;

	 pfCandMatchFound = 1;

	 for(unsigned iblock=0; iblock<cand.elementsInBlocks().size(); iblock++) {

	   PFBlockRef blockRef = cand.elementsInBlocks()[iblock].first;
	   unsigned indexInBlock = cand.elementsInBlocks()[iblock].second;

	   
	   const edm::OwnVector<  reco::PFBlockElement>&  elements = (*blockRef).elements();

	   //This tells you what type of element it is:
	   //cout<<" block type"<<elements[indexInBlock].type()<<endl;
	   
	   switch (elements[indexInBlock].type()) {
	     
	   case PFBlockElement::ECAL: {
	     reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
	     double eet = clusterRef->energy()/cosh(clusterRef->eta());
	     if(verbose_)cout<<" ecal energy "<<clusterRef->energy()<<endl;
	     jets_.tracksumecal[jets_.ntrack] += eet;
	     break;
	   }
	     
	   case PFBlockElement::HCAL: {
	     reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
	     double eet = clusterRef->energy()/cosh(clusterRef->eta());
	     if(verbose_)cout<<" hcal energy "<<clusterRef->energy()<<endl;
	     jets_.tracksumhcal[jets_.ntrack] += eet;
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
	 
       }

       if(!pfCandMatchFound){
	 jets_.tracksumecal[jets_.ntrack] =-1;
	 jets_.tracksumhcal[jets_.ntrack] =-1;

       }

     jets_.ntrack++;
       
   }
   
   // make configurable, so that gen particles aren't run with MB
    
   if(isMC_){

     edm::Handle<GenEventInfoProduct> hEventInfo;
     iEvent.getByLabel(eventInfoTag_,hEventInfo);

     jets_.pthat = hEventInfo->qScale();

     getPartons(iEvent, iSetup );
     
     edm::Handle <reco::GenParticleCollection> genParticles;
     iEvent.getByLabel (genParticleTag_, genParticles );


     for( unsigned igen=0; igen<genParticles->size(); igen++ ) {

       const reco::GenParticle & genp = (*genParticles)[igen];
       
       if(genp.status()!=1) continue;
       
       jets_.genppt[jets_.ngenp] = genp.pt();
       jets_.genpeta[jets_.ngenp] = genp.eta();
       jets_.genpphi[jets_.ngenp] = genp.phi();
       jets_.genppdgId[jets_.ngenp] = genp.pdgId();
       
       jets_.ngenp++;
     }
   }


   t->Fill();



   jets_.nj1 = 0;
   jets_.nj2 = 0;
   jets_.nj3 = 0;
   jets_.nj4 = 0;
   jets_.nPFcand = 0;
   jets_.ntrack = 0;
   jets_.ngenp = 0;

}

  
// copied from PhysicsTools/JetMCAlgos/plugins/PartonSelector.cc
void PFJetAnalyzer::getPartons( const Event& iEvent, const EventSetup& iEs )
{

  //edm::Handle <reco::CandidateView> genParticles;
  edm::Handle <reco::GenParticleCollection> genParticles;
  iEvent.getByLabel (genParticleTag_, genParticles );

  auto_ptr<GenParticleRefVector> thePartons ( new GenParticleRefVector);


    const GenParticle & parton1 = (*genParticles)[ 6 ];
    jets_.parton1_flavor = abs(parton1.pdgId());
    jets_.parton1_pt = parton1.pt();
    jets_.parton1_phi =  parton1.phi();
    jets_.parton1_eta = parton1.eta();
    jets_.parton1_y = parton1.y();

    const GenParticle & parton2 = (*genParticles)[ 7 ];
    jets_.parton2_flavor = abs(parton2.pdgId());
    jets_.parton2_pt = parton2.pt();
    jets_.parton2_phi =  parton2.phi();
    jets_.parton2_eta = parton2.eta();
    jets_.parton2_y = parton2.y();




}






				     					
DEFINE_FWK_MODULE(PFJetAnalyzer);
