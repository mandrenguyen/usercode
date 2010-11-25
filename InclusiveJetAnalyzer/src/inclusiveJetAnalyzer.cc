/*
  Based on the jet response analyzer
  Modified by Matt Nguyen, November 2010

*/

#include "MNguyen/InclusiveJetAnalyzer/interface/inclusiveJetAnalyzer.h"


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

using namespace std;
using namespace edm;
using namespace reco;


InclusiveJetAnalyzer::InclusiveJetAnalyzer(const edm::ParameterSet& iConfig) {
  

  jetTag_ = iConfig.getParameter<InputTag>("jetTag");

  verbose_ = iConfig.getUntrackedParameter<bool>("verbose",false);

  useCentrality_ = iConfig.getUntrackedParameter<bool>("useCentrality",false);
  isMC_ = iConfig.getUntrackedParameter<bool>("isMC",false);


  cout<<" jet collection : "<<jetTag_<<endl;



   
}



InclusiveJetAnalyzer::~InclusiveJetAnalyzer() { }



void 
InclusiveJetAnalyzer::beginRun(const edm::Run& run, 
			      const edm::EventSetup & es) {}

void 
InclusiveJetAnalyzer::beginJob() {

  centrality_ = 0;

  string jetTagName = jetTag_.label()+"_tree"; 
  string jetTagTitle = jetTag_.label()+" Jet Analysis Tree"; 
  t = f->make<TTree>(jetTagName.c_str(),jetTagTitle.c_str());


  //  TTree* t= new TTree("t","Jet Response Analyzer");
  t->Branch("run",&jets_.run,"run/I");
  t->Branch("evt",&jets_.evt,"evt/I");
  t->Branch("b",&jets_.b,"b/F");
  t->Branch("hf",&jets_.hf,"hf/F");
  t->Branch("nref",&jets_.nref,"nref/b");
  t->Branch("bin",&jets_.bin,"bin/I");
  t->Branch("rawpt",jets_.rawpt,"rawpt[nref]/F");
  t->Branch("jtpt",jets_.jtpt,"jtpt[nref]/F");
  t->Branch("jteta",jets_.jteta,"jteta[nref]/F");
  t->Branch("jty",jets_.jty,"jty[nref]/F");
  t->Branch("jtphi",jets_.jtphi,"jtphi[nref]/F");
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
InclusiveJetAnalyzer::analyze(const Event& iEvent, 
			     const EventSetup& iSetup) {
  

 int event = iEvent.id().event();
  int run = iEvent.id().run();

  jets_.run = run;
  jets_.evt = event;

  LogDebug("InclusiveJetAnalyzer")<<"START event: "<<event<<" in run "<<run<<endl;

  

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

   
   
   
   // FILL JRA TREE

   jets_.b = b;
   jets_.nref = 0;
   
   //cout<<" jets size "<<jets->size()<<endl;

   
   for(unsigned int j = 0 ; j < jets->size(); ++j){
     const pat::Jet& jet = (*jets)[j];
     
     //cout<<" jet pt "<<jet.pt()<<endl;
     if(jet.pt() < jetPtMin) continue;
     jets_.rawpt[jets_.nref]=jet.correctedJet("raw").pt();
     jets_.jtpt[jets_.nref] = jet.pt();                            
     jets_.jteta[jets_.nref] = jet.eta();
     jets_.jtphi[jets_.nref] = jet.phi();
     jets_.jty[jets_.nref] = jet.eta();
     
     
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
   
   t->Fill();
}

  






				     					
DEFINE_FWK_MODULE(InclusiveJetAnalyzer);
