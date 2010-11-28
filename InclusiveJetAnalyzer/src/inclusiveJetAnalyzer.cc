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

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"
#include "L1Trigger/GlobalTrigger/interface/L1GlobalTrigger.h"




using namespace std;
using namespace edm;
using namespace reco;


InclusiveJetAnalyzer::InclusiveJetAnalyzer(const edm::ParameterSet& iConfig) {
  

  jetTag_ = iConfig.getParameter<InputTag>("jetTag");

  verbose_ = iConfig.getUntrackedParameter<bool>("verbose",false);

  useCentrality_ = iConfig.getUntrackedParameter<bool>("useCentrality",false);
  isMC_ = iConfig.getUntrackedParameter<bool>("isMC",false);

  L1gtReadout_ = iConfig.getParameter<edm::InputTag>("L1gtReadout");
  hltResName_ = iConfig.getUntrackedParameter<string>("hltTrgResults","TriggerResults::HLT");
   
  if (iConfig.exists("hltTrgNames"))
    hltTrgNames_ = iConfig.getUntrackedParameter<vector<string> >("hltTrgNames");
  
  if (iConfig.exists("hltProcNames"))
    hltProcNames_ = iConfig.getUntrackedParameter<vector<string> >("hltProcNames");
  else {
    hltProcNames_.push_back("FU");
    hltProcNames_.push_back("HLT");
  }


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
  
  int filltrigger = 1;
  if(filltrigger){

    t->Branch("nL1TBit",&jets_.nL1TBit,"nL1TBit/I");
    t->Branch("l1TBit",jets_.l1TBit,"l1TBit[nL1TBit]/O");

    t->Branch("nL1ABit",&jets_.nL1ABit,"nL1ABit/I");
    t->Branch("l1ABit",jets_.l1ABit,"l1ABit[nL1ABit]/O");

    t->Branch("nHLTBit",&jets_.nHLTBit,"nHLTBit/I");
    t->Branch("hltBit",jets_.hltBit,"hltBit[nHLTBit]/O");

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

   fillL1Bits(iEvent);
   fillHLTBits(iEvent);
    
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

  

//--------------------------------------------------------------------------------------------------
void InclusiveJetAnalyzer::fillL1Bits(const edm::Event &iEvent)
{
  edm::Handle< L1GlobalTriggerReadoutRecord >  L1GlobalTrigger;

  iEvent.getByLabel(L1gtReadout_, L1GlobalTrigger); 
  const TechnicalTriggerWord&  technicalTriggerWordBeforeMask = L1GlobalTrigger->technicalTriggerWord();

  for (int i=0; i<64;i++)
    {
      jets_.l1TBit[i] = technicalTriggerWordBeforeMask.at(i);
    }
  jets_.nL1TBit = 64;

  int ntrigs = L1GlobalTrigger->decisionWord().size();
  jets_.nL1ABit = ntrigs;

  for (int i=0; i != ntrigs; i++) {
    bool accept = L1GlobalTrigger->decisionWord()[i];
    //jets_.l1ABit[i] = (accept == true)? 1:0;
    if(accept== true){
      jets_.l1ABit[i] = 1;
    }
    else{
      jets_.l1ABit[i] = 0;
    }
    
  }
}

//--------------------------------------------------------------------------------------------------
void InclusiveJetAnalyzer::fillHLTBits(const edm::Event &iEvent)
{
  // Fill HLT trigger bits.
  Handle<TriggerResults> triggerResultsHLT;
  getProduct(hltResName_, triggerResultsHLT, iEvent);

  const TriggerResults *hltResults = triggerResultsHLT.product();
  const TriggerNames & triggerNames = iEvent.triggerNames(*hltResults);

  jets_.nHLTBit = triggerNames.size();

  for(size_t i=0;i<triggerNames.size();++i) {
    cout <<triggerNames.triggerName(i)<<endl;
    jets_.hltBit[i] = triggerResultsHLT->accept(i);
  }
}


//--------------------------------------------------------------------------------------------------
template <typename TYPE>
inline void InclusiveJetAnalyzer::getProduct(const std::string name, edm::Handle<TYPE> &prod,
					 const edm::Event &event) const
{
  // Try to access data collection from EDM file. We check if we really get just one
  // product with the given name. If not we throw an exception.

  event.getByLabel(edm::InputTag(name),prod);
  if (!prod.isValid()) 
    throw edm::Exception(edm::errors::Configuration, "InclusiveJetAnalyzer::GetProduct()\n")
      << "Collection with label '" << name << "' is not valid" <<  std::endl;
}

//--------------------------------------------------------------------------------------------------
template <typename TYPE>
inline bool InclusiveJetAnalyzer::getProductSafe(const std::string name, edm::Handle<TYPE> &prod,
					     const edm::Event &event) const
{
  // Try to safely access data collection from EDM file. We check if we really get just one
  // product with the given name. If not, we return false.

  if (name.size()==0)
    return false;

  try {
    event.getByLabel(edm::InputTag(name),prod);
    if (!prod.isValid()) 
      return false;
  } catch (...) {
    return false;
  }
  return true;
}



				     					
DEFINE_FWK_MODULE(InclusiveJetAnalyzer);
