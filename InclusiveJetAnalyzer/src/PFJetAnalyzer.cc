/*
  Based on the jet response analyzer
  Modified by Matt Nguyen, November 2010

*/

#include "MNguyen/InclusiveJetAnalyzer/interface/PFJetAnalyzer.h"


#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include <Math/DistFunc.h>
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
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/PFJetCollection.h"

using namespace std;
using namespace edm;
using namespace reco;


PFJetAnalyzer::PFJetAnalyzer(const edm::ParameterSet& iConfig) {
  

  jetTag_ = iConfig.getParameter<InputTag>("jetTag");
  pfCandidatesTag_ = iConfig.getParameter<InputTag>("pfCandidatesTag");
  recoJetTag_ = iConfig.getParameter<InputTag>("recoJetTag");

  verbose_ = iConfig.getUntrackedParameter<bool>("verbose",false);

  useCentrality_ = iConfig.getUntrackedParameter<bool>("useCentrality",false);
  isMC_ = iConfig.getUntrackedParameter<bool>("isMC",false);


  cout<<" jet collection : "<<jetTag_<<endl;



   
}



PFJetAnalyzer::~PFJetAnalyzer() { }



void 
PFJetAnalyzer::beginRun(const edm::Run& run, 
			      const edm::EventSetup & es) {}

void 
PFJetAnalyzer::beginJob() {

  centrality_ = 0;

  string jetTagName = jetTag_.label()+"_tree"; 
  string jetTagTitle = jetTag_.label()+" Jet Analysis Tree"; 
  t = f->make<TTree>(jetTagName.c_str(),jetTagTitle.c_str());


  //  TTree* t= new TTree("t","Jet Response Analyzer");
  t->Branch("run",&jets_.run,"run/I");
  t->Branch("evt",&jets_.evt,"evt/I");
  t->Branch("b",&jets_.b,"b/F");
  t->Branch("hf",&jets_.hf,"hf/F");
  t->Branch("nref",&jets_.nref,"nref/I");
  t->Branch("bin",&jets_.bin,"bin/I");
  t->Branch("rawpt",jets_.rawpt,"rawpt[nref]/F");
  t->Branch("jtpt",jets_.jtpt,"jtpt[nref]/F");
  t->Branch("jteta",jets_.jteta,"jteta[nref]/F");
  t->Branch("jty",jets_.jty,"jty[nref]/F");
  t->Branch("jtphi",jets_.jtphi,"jtphi[nref]/F");

  t->Branch("preL1et",jets_.preL1et,"preL1et[nref]/F");
  t->Branch("L2",jets_.L2,"L2[nref]/F");
  t->Branch("L3",jets_.L3,"L3[nref]/F");

  t->Branch("nPFcand",&jets_.nPFcand,"nPFcand/I");
  t->Branch("candID",jets_.candID,"candID[nPFcand]/I");
  t->Branch("candpt",jets_.candpt,"candpt[nPFcand]/F");
  t->Branch("candeta",jets_.candeta,"candeta[nPFcand]/F");
  t->Branch("candy",jets_.candy,"candy[nPFcand]/F");
  t->Branch("candphi",jets_.candphi,"candphi[nPFcand]/F");


  if(isMC_){
    t->Branch("refpt",jets_.refpt,"refpt[nref]/F");
    t->Branch("refeta",jets_.refeta,"refeta[nref]/F");
    t->Branch("refy",jets_.refy,"refy[nref]/F");
    t->Branch("refphi",jets_.refphi,"refphi[nref]/F");
    t->Branch("refdrjt",jets_.refdrjt,"refdrjt[nref]/F");
  }
  
  TH1D::SetDefaultSumw2();
  
  
}


void 
PFJetAnalyzer::analyze(const Event& iEvent, 
			     const EventSetup& iSetup) {
  

 int event = iEvent.id().event();
  int run = iEvent.id().run();

  jets_.run = run;
  jets_.evt = event;

  LogDebug("PFJetAnalyzer")<<"START event: "<<event<<" in run "<<run<<endl;

  

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
   



   double jetPtMin = 35.;


   // loop the events
   
   jets_.bin = bin;
   //jets_.hf = hf;
   
   edm::Handle<pat::JetCollection> jets;
   //ev.getByLabel(edm::InputTag(jetTag),jets);
   iEvent.getByLabel(jetTag_, jets);


   edm::Handle< edm::View<reco::PFJet> > recoJetColl;
   iEvent.getByLabel( recoJetTag_, recoJetColl );
   
   Handle<PFCandidateCollection> pfCandidates;
   iEvent.getByLabel(pfCandidatesTag_, pfCandidates);
   
   
   
   
   // FILL JRA TREE

   jets_.b = b;
   jets_.nref = 0;
   
   //cout<<" jets size "<<jets->size()<<endl;

   
   for(unsigned int j = 0 ; j < jets->size(); ++j){
     const pat::Jet& jet = (*jets)[j];
     
     //cout<<" jet pt "<<jet.pt()<<endl;
     //if(jet.pt() < jetPtMin) continue;
     jets_.rawpt[jets_.nref]=jet.correctedJet("raw").pt();
     jets_.jtpt[jets_.nref] = jet.pt();                            
     jets_.jteta[jets_.nref] = jet.eta();
     jets_.jtphi[jets_.nref] = jet.phi();
     jets_.jty[jets_.nref] = jet.eta();
     //  cout<<" abs corr "<<jet.corrFactor("abs")<<endl;
     //cout<<" abs corr "<<jet.corrFactor("L3Absolute")<<endl;

     /*
       // L2L3 makes sense I think, check for yourself
       
       cout<<" raw corr "<<jet.corrFactor("raw")<<endl;
       cout<<" off corr "<<jet.corrFactor("off")<<endl;
       cout<<" rel corr "<<jet.corrFactor("rel")<<endl;
       cout<<" abs corr "<<jet.corrFactor("abs")<<endl;
       cout<<" emf corr "<<jet.corrFactor("emf")<<endl;
       //cout<<" had corr "<<jet.corrFactor("had")<<endl;
       
       
       
       cout<<" L2Corr "<<L2Corr<<" L3Corr "<<L3Corr<<endl;
     */


       float L2Corr = jet.correctedJet("rel").pt()/jet.correctedJet("raw").pt();
       float L3Corr = jet.correctedJet("abs").pt()/jet.correctedJet("rel").pt();
       

       jets_.L2[jets_.nref] = L2Corr;
       jets_.L3[jets_.nref] = L3Corr;

       // Match to reco jet to find unsubtracted jet energy

       int recoJetSize = recoJetColl->size();
       
       jets_.preL1et[jets_.nref] = -1;
       
       //cout<<" patJet_eta "<<jet.eta()<<" patJet_phi "<<jet.phi()<<" patJet_et "<<jet.et()<<endl;


       for(int iRecoJet = 0; iRecoJet < recoJetSize; ++iRecoJet){
	 
	 reco::PFJet recoJet = ((*recoJetColl)[iRecoJet]);
	 
	   
	 double recoJet_eta = recoJet.eta();
	 double recoJet_phi = recoJet.phi();
	 //cout<<" recoJet_eta "<<recoJet_eta<<" recoJet_phi "<<recoJet_phi<<" recoJet_et "<<recoJet.et()<<endl;
	 
	 
	 if(fabs(recoJet.eta()-jet.eta()) < 0.001
	    && fabs(acos(cos((recoJet.phi()-jet.phi())))) < 0.001)
	   {
	     jets_.preL1et[jets_.nref] = recoJet.et();
	     
	     //cout<<"Match found,  recoJet.et "<<recoJet.et()<< " recoJet.eta "<<jet.eta()<<" recoJet.phi "<<recoJet.phi()<<endl;
	     break;
	   }
       }
       if(jets_.preL1et[jets_.nref] == -1){

	 /*
	   There's a known issue here.  If the background subtraction oversubtracts I've set the patJet.et() to zero.  That would be fine if I had also set the eta and phi.  We could then recover the pre-subtracted energy.  However, I never bothered to set the eta and phi for theses jets (doh!).  Next time I repass the data I won't be so stupid.
	 */

	 
	 if(jet.et()>0)cout<<"Match *NOT* found,  patJet.et "<<jet.et()<< " patJet.eta "<<jet.eta()<<" patJet.phi() "<<jet.phi()<<endl;
       }

     if(isMC_){
       
       
       if(jet.genJet()!=0 && jet.genJet()->pt()>1.0 && jet.genJet()->pt()<999999){
	 jets_.refpt[jets_.nref] = jet.genJet()->pt();
	 jets_.refeta[jets_.nref] = jet.genJet()->eta();
	 jets_.refphi[jets_.nref] = jet.genJet()->phi();
	 jets_.refy[jets_.nref] = jet.genJet()->eta();
	 
	 jets_.refdrjt[jets_.nref] = reco::deltaR(jet,*(jet.genJet()));
       }        
       else{
	 jets_.refpt[jets_.nref] = 0;
	 jets_.refeta[jets_.nref] = -999;
	 jets_.refphi[jets_.nref] = -999;
	 jets_.refy[jets_.nref] = -999;
       }
       
     }



       jets_.nref++;
       
       
   }


   for( unsigned icand=0; icand<pfCandidates->size(); icand++ ) {
     
     const reco::PFCandidate& cand = (*pfCandidates)[icand];
     
       float particleEta = cand.eta();
       
       if(fabs(particleEta)>2.5) continue;
       
       /*

       PF PID Convention:
       1 = Charged Hadrons
       2 = Electrons (not included)
       3 = Muons
       4 = Photons
       5 = Neutral Hadrons

       */


       int particleId = (int)cand.particleId();
       float particlePt = cand.pt();
       
       if(particlePt<0.9) continue;
       
       /*
       // can use varid thresholds if we want
       if(particleID==1 && particlePt < 0.9) continue;
       if(particleID==3 && particlePt < 0.9) continue;
       if(particleID==4 && particlePt < 0.3) continue;
       if(particleID==5 && particlePt < 0.9) continue;
       */
       
       

       jets_.candID[jets_.nPFcand] = particleId;
       jets_.candpt[jets_.nPFcand] = particlePt;
       jets_.candeta[jets_.nPFcand] = particleEta;
       jets_.candphi[jets_.nPFcand] = cand.phi();
       jets_.candy[jets_.nPFcand] = cand.y();
       jets_.nPFcand++;

       //cout<<" jets_.nPFcand "<<jets_.nPFcand<<endl;
   }
   

   
   t->Fill();





   jets_.nref = 0;
   jets_.nPFcand = 0;


}

  






				     					
DEFINE_FWK_MODULE(PFJetAnalyzer);
