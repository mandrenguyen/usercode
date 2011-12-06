/*
  Based on the jet response analyzer
  Modified by Matt Nguyen, November 2010

*/

#include "MNguyen/ElectronAnalyzer/interface/electronAnalyzer.h"


#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include <Math/DistFunc.h>
#include "TMath.h"



#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/HeavyIonEvent/interface/CentralityBins.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"

#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"

#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "SimDataFormats/HiGenData/interface/GenHIEvent.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"

#include "DataFormats/Common/interface/TriggerResults.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetup.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"
#include "L1Trigger/GlobalTrigger/interface/L1GlobalTrigger.h"

#include "CLHEP/Units/GlobalPhysicalConstants.h"



using namespace std;
using namespace edm;
using namespace reco;


ElectronAnalyzer::ElectronAnalyzer(const edm::ParameterSet& iConfig) {
  


  eleTag_ = iConfig.getParameter<InputTag>("eleTag");
  isMC_ = iConfig.getUntrackedParameter<bool>("isMC",false);
  
  if(isMC_){
    mcTag_ = iConfig.getParameter<InputTag>("mcTag");
  }
  verbose_ = iConfig.getUntrackedParameter<bool>("verbose",false);

  useVtx_ = iConfig.getUntrackedParameter<bool>("useVtx",true);
  useCentrality_ = iConfig.getUntrackedParameter<bool>("useCentrality",false);

  vtxTag_ = iConfig.getUntrackedParameter<edm::InputTag>("vtxTag",edm::InputTag("hiSelectedVertex"));

  if(!isMC_){
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
  }

  cout<<" electron collection : "<<eleTag_<<endl;
   
}



ElectronAnalyzer::~ElectronAnalyzer() { }



void 
ElectronAnalyzer::beginRun(const edm::Run& run, 
			      const edm::EventSetup & es) {}

void 
ElectronAnalyzer::beginJob() {

  centrality_ = 0;

  string eleTagTitle = eleTag_.label()+" Electron Analysis Tree"; 
  t = fs1->make<TTree>("t",eleTagTitle.c_str());


  //  TTree* t= new TTree("t","Jet Response Analyzer");
  t->Branch("run",&ele_.run,"run/I");
  t->Branch("evt",&ele_.evt,"evt/I");
  t->Branch("lumi",&ele_.lumi,"lumi/I");
  t->Branch("b",&ele_.b,"b/D");
  t->Branch("vx",&ele_.vx,"vx/D");
  t->Branch("vy",&ele_.vy,"vy/D");
  t->Branch("vz",&ele_.vz,"vz/D");
  t->Branch("hf",&ele_.hf,"hf/D");
  t->Branch("nref",&ele_.nref,"nref/I");
  t->Branch("bin",&ele_.bin,"bin/I");

  // reconstructed electrons 
  t->Branch("ee",ele_.ee,"ee[nref]/D");
  t->Branch("ep",ele_.ep,"ep[nref]/D");
  t->Branch("ept",ele_.ept,"ept[nref]/D");
  t->Branch("ephi",ele_.ephi,"ephi[nref]/D");
  t->Branch("eeta",ele_.eeta,"eeta[nref]/D");
  t->Branch("echarge",ele_.echarge,"echarge[nref]/I");
  t->Branch("hOe",ele_.hOe,"hOe[nref]/D");
  t->Branch("sigmaietaieta",ele_.sigmaietaieta,"sigmaietaieta[nref]/D");
  t->Branch("eseedpout",ele_.eseedOpout,"eseedpout[nref]/D");
  t->Branch("eOp",ele_.eOp,"eOp[nref]/D");
  t->Branch("eseedOp",ele_.eseedOp,"eseedOp[nref]/D");  
  t->Branch("eeleOpout",ele_.eeleOpout,"eeleOpout[nref]/D");
  t->Branch("deltaetain",ele_.deltaetain,"deltaetain[nref]/D");
  t->Branch("deltaphiin",ele_.deltaphiin,"deltaphiin[nref]/D");
  t->Branch("sigmaetaeta",ele_.sigmaetaeta,"sigmaetaeta[nref]/D");
  t->Branch("e15",ele_.e15,"e15[nref]/D");
  t->Branch("e25max",ele_.e25max,"e25max[nref]/D");
  t->Branch("e55",ele_.e55,"e55[nref]/D");
  t->Branch("fbrem",ele_.fbrem,"fbrem[nref]/D");
  t->Branch("mva",ele_.mva,"mva[nref]/D");
  t->Branch("isbarrel",ele_.isbarrel,"isbarrel[nref]/I");
  t->Branch("isendcap",ele_.isendcap,"isendcap[nref]/I");

  if(isMC_){    
    t->Branch("pthat",&ele_.pthat,"pthat/D");    
    
    // MC Truth Matching
    t->Branch("isMCEle",ele_.isMCEle,"isMCEle[nref]/I");
    t->Branch("isMCPhoton",ele_.isMCPhoton,"isMCPhoton[nref]/I");
    t->Branch("isMCHadron",ele_.isMCHadron,"isMCHadron[nref]/I");
    t->Branch("idPDGMatch",ele_.idPDGMatch,"idPDGMatch[nref]/I");
    t->Branch("MC_chosenEle_PoP_pt",ele_.MC_chosenEle_PoP_pt,"MC_chosenEle_PoP_pt[nref]/D");
    t->Branch("MC_chosenEle_PoP_phi",ele_.MC_chosenEle_PoP_phi,"MC_chosenEle_PoP_phi[nref]/D");
    t->Branch("MC_chosenEle_PoP_eta",ele_.MC_chosenEle_PoP_eta,"MC_chosenEle_PoP_eta[nref]/D");
    t->Branch("MC_chosenEle_PoP_e",ele_.MC_chosenEle_PoP_e,"MC_chosenEle_PoP_e[nref]/D");
    t->Branch("MC_chosenPho_PoP_pt",ele_.MC_chosenPho_PoP_pt,"MC_chosenPho_PoP_pt[nref]/D");
    t->Branch("MC_chosenPho_PoP_phi",ele_.MC_chosenPho_PoP_phi,"MC_chosenPho_PoP_phi[nref]/D");
    t->Branch("MC_chosenPho_PoP_eta",ele_.MC_chosenPho_PoP_eta,"MC_chosenPho_PoP_eta[nref]/D");
    t->Branch("MC_chosenPho_PoP_e",ele_.MC_chosenPho_PoP_e,"MC_chosenPho_PoP_e[nref]/D");
    t->Branch("MC_chosenHad_PoP_pt",ele_.MC_chosenHad_PoP_pt,"MC_chosenHad_PoP_pt[nref]/D");
    t->Branch("MC_chosenHad_PoP_phi",ele_.MC_chosenHad_PoP_phi,"MC_chosenHad_PoP_phi[nref]/D");
    t->Branch("MC_chosenHad_PoP_eta",ele_.MC_chosenHad_PoP_eta,"MC_chosenHad_PoP_eta[nref]/D");
    t->Branch("MC_chosenHad_PoP_e",ele_.MC_chosenHad_PoP_e,"MC_chosenHad_PoP_e[nref]/D");
    t->Branch("MC_closest_DR_pt",ele_.MC_closest_DR_pt,"MC_closest_DR_pt[nref]/D");
    t->Branch("MC_closest_DR_phi",ele_.MC_closest_DR_phi,"MC_closest_DR_phi[nref]/D");
    t->Branch("MC_closest_DR_eta",ele_.MC_closest_DR_eta,"MC_closest_DR_eta[nref]/D");
    t->Branch("MC_closest_DR_e",ele_.MC_closest_DR_e,"MC_closest_DR_e[nref]/D");

    
  }
  
  if(!isMC_){
    t->Branch("nL1TBit",&ele_.nL1TBit,"nL1TBit/I");
    t->Branch("l1TBit",ele_.l1TBit,"l1TBit[nL1TBit]/O");
    
    t->Branch("nL1ABit",&ele_.nL1ABit,"nL1ABit/I");
    t->Branch("l1ABit",ele_.l1ABit,"l1ABit[nL1ABit]/O");
    
    t->Branch("nHLTBit",&ele_.nHLTBit,"nHLTBit/I");
    t->Branch("hltBit",ele_.hltBit,"hltBit[nHLTBit]/O");

  }

  TH1D::SetDefaultSumw2();
  
  
}


void 
ElectronAnalyzer::analyze(const Event& iEvent, 
			     const EventSetup& iSetup) {
  
  int event = iEvent.id().event();
  int run = iEvent.id().run();
  int lumi = iEvent.id().luminosityBlock();
  
  ele_.run = run;
  ele_.evt = event;
  ele_.lumi = lumi;

  LogDebug("ElectronAnalyzer")<<"START event: "<<event<<" in run "<<run<<endl;


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

  }
   



   // loop the events
   
   ele_.bin = bin;
   ele_.hf = hf;
   
   
   if (useVtx_) {
      edm::Handle<vector<reco::Vertex> >vertex;
      iEvent.getByLabel(vtxTag_, vertex);
      
      if(vertex->size()>0) {
        ele_.vx=vertex->begin()->x();
        ele_.vy=vertex->begin()->y();
        ele_.vz=vertex->begin()->z();
      }
   }
   
   
   const HepMC::GenEvent* MCEvt = 0; 
   HepMC::GenParticle* genPc = 0;
   HepMC::FourVector pAssSim;
   
   if(isMC_){
     edm::Handle<edm::HepMCProduct> HepMCEvt;
     iEvent.getByLabel(mcTag_, HepMCEvt);
     
     MCEvt = HepMCEvt->GetEvent();
     ele_.pthat = HepMCEvt -> GetEvent() -> event_scale();
   }

   
   // FILL ELE TREE

   ele_.b = b;
   ele_.nref = 0;
   
   if(!isMC_){
     fillL1Bits(iEvent);
     fillHLTBits(iEvent);
   }

   edm::Handle<reco::GsfElectronCollection> EleHandle ;
   iEvent.getByLabel (eleTag_.label(),EleHandle) ;
   

   // loop over electrons
   for(unsigned i=0; i< EleHandle->size(); i++){
     
     edm::Ref<reco::GsfElectronCollection> electronEdmRef(EleHandle,i);
     
     ele_.ep[ele_.nref] = (*EleHandle)[i].p();
     ele_.ee[ele_.nref] = (*EleHandle)[i].energy();
     ele_.ept[ele_.nref] = (*EleHandle)[i].pt();
     ele_.ephi[ele_.nref] = (*EleHandle)[i].eta();
     ele_.eeta[ele_.nref] = (*EleHandle)[i].phi();
     ele_.echarge[ele_.nref] = (*EleHandle)[i].charge(); 
     ele_.hOe[ele_.nref]      = (*EleHandle)[i].hadronicOverEm() ;	
     ele_.eseedOpout[ele_.nref] = (*EleHandle)[i].eSeedClusterOverPout();
     ele_.eOp[ele_.nref]        = (*EleHandle)[i].eSuperClusterOverP() ;        
     ele_.eseedOp[ele_.nref]    = (*EleHandle)[i].eSeedClusterOverP() ;         
     ele_.eeleOpout[ele_.nref]  = (*EleHandle)[i].eEleClusterOverPout() ;      
     ele_.deltaetain[ele_.nref]   = (*EleHandle)[i].deltaEtaSuperClusterTrackAtVtx();
     ele_.deltaphiin[ele_.nref]   = (*EleHandle)[i].deltaPhiSuperClusterTrackAtVtx();   
     ele_.sigmaietaieta[ele_.nref] = (*EleHandle)[i].sigmaIetaIeta() ; 	
     ele_.e15[ele_.nref]           = (*EleHandle)[i].e1x5() ;
     ele_.e25max[ele_.nref]        = (*EleHandle)[i].e2x5Max() ;
     ele_.e55[ele_.nref]           = (*EleHandle)[i].e5x5() ;
     ele_.sigmaetaeta[ele_.nref]   = (*EleHandle)[i].sigmaEtaEta() ;
     ele_.fbrem[ele_.nref] = (*EleHandle)[i].fbrem() ;
     ele_.mva[ele_.nref]   = (*EleHandle)[i].mva() ;
     
   

  
     // match electrons
     if(isMC_){
       
       
       
       double eleOkRatio = 999999.;
       double eleOkRatioE = 999999.;
       double eleOkRatioG = 999999.;
       double eleOkRatioH = 999999.;
       
       int idPDG = 999999 ;
       int idPDG_ele = 999999 ;
       int idPDG_mother_conv = 999999;
       int idPDG_pho = 999999 ;
       int idPDG_had = 999999 ;
       
       HepMC::FourVector MC_chosenEle_PoP_loc;
       HepMC::FourVector MC_chosenPho_PoP_loc;
       HepMC::FourVector MC_chosenHad_PoP_loc;
       HepMC::FourVector MC_closest_DR_loc;
       
       HepMC::GenParticle* mother = 0; // for particle gun	
       for (HepMC::GenEvent::particle_const_iterator partIter = MCEvt->particles_begin();
	    partIter != MCEvt->particles_end(); ++partIter) {
	 
	 int idTmpPDG = (*partIter)->pdg_id();  
	 //MC particle
	 genPc = (*partIter);
	 pAssSim = genPc->momentum();
	 //reco electron
	 float eta = (*EleHandle)[i].eta();
	 float phi = (*EleHandle)[i].phi();
	 float p = (*EleHandle)[i].p();
	 //matching conditions
	 double dphi = phi-pAssSim.phi();
	 if (fabs(dphi)>CLHEP::pi)
	   dphi = dphi < 0? (CLHEP::twopi) + dphi : dphi - CLHEP::twopi;
	 double deta = eta - pAssSim.eta();
	 float deltaR = sqrt(pow(deta,2) + pow(dphi,2));
	 //standard
	 if ( deltaR < 0.15 ){                  // in the cone
	   if (  pAssSim.perp() > 1.5 ){
	     double tmpeleRatio = p/pAssSim.t();
	     
	     if (idTmpPDG == 11 || idTmpPDG == -11 ){              // looking at Ele
	       if ( fabs(tmpeleRatio-1) < fabs(eleOkRatioE-1) ) { //best p/p
		 eleOkRatioE = tmpeleRatio;
		 idPDG_ele = idTmpPDG;
		 //for particle gun 
		 //idPDG_mother_conv = (*((*partIter)->production_vertex()->particles_begin(HepMC::parents)))->pdg_id();
		 mother = *((*partIter)->production_vertex()->particles_begin(HepMC::parents)); 	 
		 if (mother!=0) idPDG_mother_conv = mother->pdg_id();
		 MC_chosenEle_PoP_loc = pAssSim;
	       } //best p/p conditions
	     }  // looking at Ele
	     if(idTmpPDG == 22) {                                 // looking at Gamma
	       if ( fabs(tmpeleRatio-1) < fabs(eleOkRatioG-1) ) {
		 eleOkRatioG = tmpeleRatio; 
		 idPDG_pho = idTmpPDG;
		 MC_chosenPho_PoP_loc = pAssSim;
	       } //best p/p conditions
	     }  // looking at E/Gamma
	     if(abs(idTmpPDG) == 211 || abs(idTmpPDG) == 321){   //looking at had
	       if ( fabs(tmpeleRatio-1) < fabs(eleOkRatioH-1) ) {
		 eleOkRatioH = tmpeleRatio; 
		 idPDG_had = idTmpPDG;
		 MC_chosenHad_PoP_loc = pAssSim;
	       }  //best p/p
	     }  // looking at had
	     
	     if ( fabs(tmpeleRatio-1) < fabs(eleOkRatio-1) ) {   // overall best p/p ratio
	       eleOkRatio = tmpeleRatio; 
	       idPDG = idTmpPDG;
	       MC_closest_DR_loc = pAssSim;
	     }
	   
	   }  // p > 1.5 
	 }  // deltaR
       
       }//end loop over MC particles

       //real electrons
       if (idPDG_ele == 11 || idPDG_ele == -11) {
	 
	 double pxMC = MC_chosenEle_PoP_loc.px();
	 double pyMC = MC_chosenEle_PoP_loc.py();
	 double pzMC = MC_chosenEle_PoP_loc.pz();
	 double eMC = MC_chosenEle_PoP_loc.e();
	 
	 double ptMC = sqrt(pxMC*pxMC+pyMC*pyMC);
	 double pMC = sqrt(pxMC*pxMC+pyMC*pyMC+pzMC+pzMC);
	 double thetaMC = atan2(ptMC,pMC);
	 double etaMC = -1.*log(tan(thetaMC/2.));
	 double phiMC = atan2(pyMC,pxMC);

	 ele_.isMCEle[ele_.nref] = 1;   
	 ele_.MC_chosenEle_PoP_pt[ele_.nref] =   ptMC;
	 ele_.MC_chosenEle_PoP_phi[ele_.nref] =  etaMC;
	 ele_.MC_chosenEle_PoP_eta[ele_.nref] =  phiMC;
	 ele_.MC_chosenEle_PoP_e[ele_.nref] =    eMC; 
	 //idPDGmother_MCEle[ele_.nref] = idPDG_mother_conv;
       }
       //photons (or pi0)
       if(idPDG_pho == 22) {

	 double pxMC = MC_chosenPho_PoP_loc.px();
	 double pyMC = MC_chosenPho_PoP_loc.py();
	 double pzMC = MC_chosenPho_PoP_loc.pz();
	 double eMC = MC_chosenPho_PoP_loc.e();
	 
	 double ptMC = sqrt(pxMC*pxMC+pyMC*pyMC);
	 double pMC = sqrt(pxMC*pxMC+pyMC*pyMC+pzMC+pzMC);
	 double thetaMC = atan2(ptMC,pMC);
	 double etaMC = -1.*log(tan(thetaMC/2.));
	 double phiMC = atan2(pyMC,pxMC);

	 ele_.isMCPhoton[ele_.nref] = 1;
	 ele_.MC_chosenPho_PoP_pt[ele_.nref] =   ptMC;
	 ele_.MC_chosenPho_PoP_phi[ele_.nref] =	 phiMC;
	 ele_.MC_chosenPho_PoP_eta[ele_.nref] =	 etaMC;
	 ele_.MC_chosenPho_PoP_e[ele_.nref] =  	 eMC;
       }
       //hadrons
       if(abs(idPDG_had) == 211 || abs(idPDG_had) == 321){

	 double pxMC = MC_chosenHad_PoP_loc.px();
	 double pyMC = MC_chosenHad_PoP_loc.py();
	 double pzMC = MC_chosenHad_PoP_loc.pz();
	 double eMC = MC_chosenHad_PoP_loc.e();
	 
	 double ptMC = sqrt(pxMC*pxMC+pyMC*pyMC);
	 double pMC = sqrt(pxMC*pxMC+pyMC*pyMC+pzMC+pzMC);
	 double thetaMC = atan2(ptMC,pMC);
	 double etaMC = -1.*log(tan(thetaMC/2.));
	 double phiMC = atan2(pyMC,pxMC);

	 ele_.isMCHadron[ele_.nref] = 1; 
	 ele_.MC_chosenHad_PoP_pt[ele_.nref] =   ptMC;
	 ele_.MC_chosenHad_PoP_phi[ele_.nref] =  phiMC;
	 ele_.MC_chosenHad_PoP_eta[ele_.nref] =  etaMC;
	 ele_.MC_chosenHad_PoP_e[ele_.nref] =  	 eMC;
       }       
       if(idPDG != 999999){

	 double pxMC = MC_closest_DR_loc.px();
	 double pyMC = MC_closest_DR_loc.py();
	 double pzMC = MC_closest_DR_loc.pz();
	 double eMC = MC_closest_DR_loc.e();
	 
	 double ptMC = sqrt(pxMC*pxMC+pyMC*pyMC);
	 double pMC = sqrt(pxMC*pxMC+pyMC*pyMC+pzMC+pzMC);
	 double thetaMC = atan2(ptMC,pMC);
	 double etaMC = -1.*log(tan(thetaMC/2.));
	 double phiMC = atan2(pyMC,pxMC);

	 ele_.idPDGMatch[ele_.nref] = idPDG;		  
	 ele_.MC_closest_DR_pt[ele_.nref] = ptMC;
	 ele_.MC_closest_DR_phi[ele_.nref] = phiMC;
	 ele_.MC_closest_DR_eta[ele_.nref] = etaMC;
	 ele_.MC_closest_DR_e[ele_.nref] = eMC;
       }
       
       
     } // isMC
     
     ele_.nref++;
     
   } // loop over electrons
         
   t->Fill();
}


  

//--------------------------------------------------------------------------------------------------
void ElectronAnalyzer::fillL1Bits(const edm::Event &iEvent)
{
  edm::Handle< L1GlobalTriggerReadoutRecord >  L1GlobalTrigger;

  iEvent.getByLabel(L1gtReadout_, L1GlobalTrigger); 
  const TechnicalTriggerWord&  technicalTriggerWordBeforeMask = L1GlobalTrigger->technicalTriggerWord();

  for (int i=0; i<64;i++)
    {
      ele_.l1TBit[i] = technicalTriggerWordBeforeMask.at(i);
    }
  ele_.nL1TBit = 64;

  int ntrigs = L1GlobalTrigger->decisionWord().size();
  ele_.nL1ABit = ntrigs;

  for (int i=0; i != ntrigs; i++) {
    bool accept = L1GlobalTrigger->decisionWord()[i];
    //ele_.l1ABit[i] = (accept == true)? 1:0;
    if(accept== true){
      ele_.l1ABit[i] = 1;
    }
    else{
      ele_.l1ABit[i] = 0;
    }
    
  }
}

//--------------------------------------------------------------------------------------------------
void ElectronAnalyzer::fillHLTBits(const edm::Event &iEvent)
{
  // Fill HLT trigger bits.
  Handle<TriggerResults> triggerResultsHLT;
  getProduct(hltResName_, triggerResultsHLT, iEvent);

  const TriggerResults *hltResults = triggerResultsHLT.product();
  const TriggerNames & triggerNames = iEvent.triggerNames(*hltResults);

  ele_.nHLTBit = hltTrgNames_.size();

  for(size_t i=0;i<hltTrgNames_.size();i++){
   
    for(size_t j=0;j<triggerNames.size();++j) {
      
      if(triggerNames.triggerName(j) == hltTrgNames_[i]){
	
	//cout <<"hltTrgNames_(i) "<<hltTrgNames_[i]<<endl;
	//cout <<"triggerName(j) "<<triggerNames.triggerName(j)<<endl;
	//cout<<" result "<<triggerResultsHLT->accept(j)<<endl;
	ele_.hltBit[i] = triggerResultsHLT->accept(j);
      }
      
    }
  }
}

//--------------------------------------------------------------------------------------------------
template <typename TYPE>
inline void ElectronAnalyzer::getProduct(const std::string name, edm::Handle<TYPE> &prod,
					 const edm::Event &event) const
{
  // Try to access data collection from EDM file. We check if we really get just one
  // product with the given name. If not we throw an exception.

  event.getByLabel(edm::InputTag(name),prod);
  if (!prod.isValid()) 
    throw edm::Exception(edm::errors::Configuration, "ElectronAnalyzer::GetProduct()\n")
      << "Collection with label '" << name << "' is not valid" <<  std::endl;
}

//--------------------------------------------------------------------------------------------------
template <typename TYPE>
inline bool ElectronAnalyzer::getProductSafe(const std::string name, edm::Handle<TYPE> &prod,
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



				     					
DEFINE_FWK_MODULE(ElectronAnalyzer);
