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
  jetTag2_ = iConfig.getParameter<InputTag>("jetTag2");
  jetTag3_ = iConfig.getParameter<InputTag>("jetTag3");


  recoJetTag_ = iConfig.getParameter<InputTag>("recoJetTag");
  recoJetTag2_ = iConfig.getParameter<InputTag>("recoJetTag2");
  recoJetTag3_ = iConfig.getParameter<InputTag>("recoJetTag3");

  pfCandidatesTag_ = iConfig.getParameter<InputTag>("pfCandidatesTag");


  verbose_ = iConfig.getUntrackedParameter<bool>("verbose",false);

  useCentrality_ = iConfig.getUntrackedParameter<bool>("useCentrality",false);
  isMC_ = iConfig.getUntrackedParameter<bool>("isMC",false);


  cout<<" jet collection : "<<jetTag_<<endl;
  cout<<" jet collection 2: "<<jetTag2_<<endl;
  cout<<" jet collection 3: "<<jetTag3_<<endl;

   jets_.nicPu5 = 0;
   jets_.nic5 = 0;
   jets_.nak5 = 0;
   jets_.nPFcand = 0;


   
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
  t = f->make<TTree>("t","Jet Analysis Tree");


  //  TTree* t= new TTree("t","Jet Response Analyzer");
  t->Branch("run",&jets_.run,"run/I");
  t->Branch("evt",&jets_.evt,"evt/I");
  t->Branch("b",&jets_.b,"b/F");
  t->Branch("hf",&jets_.hf,"hf/F");
  t->Branch("bin",&jets_.bin,"bin/I");

  // ICPU5
  t->Branch("nicPu5",&jets_.nicPu5,"nicPu5/I");
  t->Branch("nic5",&jets_.nic5,"nic5/I");
  t->Branch("nak5",&jets_.nak5,"nak5/I");

  t->Branch("rawpt_icPu5",jets_.rawpt_icPu5,"rawpt_icPu5[nicPu5]/F");
  t->Branch("jtpt_icPu5",jets_.jtpt_icPu5,"jtpt_icPu5[nicPu5]/F");
  t->Branch("jteta_icPu5",jets_.jteta_icPu5,"jteta_icPu5[nicPu5]/F");
  t->Branch("jty_icPu5",jets_.jty_icPu5,"jty_icPu5[nicPu5]/F");
  t->Branch("jtphi_icPu5",jets_.jtphi_icPu5,"jtphi_icPu5[nicPu5]/F");
  t->Branch("preL1et_icPu5",jets_.preL1et_icPu5,"preL1et_icPu5[nicPu5]/F");
  t->Branch("L2_icPu5",jets_.L2_icPu5,"L2_icPu5[nicPu5]/F");
  t->Branch("L3_icPu5",jets_.L3_icPu5,"L3_icPu5[nicPu5]/F");
  t->Branch("area_icPu5",jets_.area_icPu5,"area_icPu5[nicPu5]/F");

  if(isMC_){
    t->Branch("refpt_icPu5",jets_.refpt_icPu5,"refpt_icPu5[nicPu5]/F");
    t->Branch("refeta_icPu5",jets_.refeta_icPu5,"refeta_icPu5[nicPu5]/F");
    t->Branch("refy_icPu5",jets_.refy_icPu5,"refy_icPu5[nicPu5]/F");
    t->Branch("refphi_icPu5",jets_.refphi_icPu5,"refphi_icPu5[nicPu5]/F");
    t->Branch("refdrjt_icPu5",jets_.refdrjt_icPu5,"refdrjt_icPu5[nicPu5]/F");
  }


  // IC5

  t->Branch("rawpt_ic5",jets_.rawpt_ic5,"rawpt_ic5[nic5]/F");
  t->Branch("jtpt_ic5",jets_.jtpt_ic5,"jtpt_ic5[nic5]/F");
  t->Branch("jteta_ic5",jets_.jteta_ic5,"jteta_ic5[nic5]/F");
  t->Branch("jty_ic5",jets_.jty_ic5,"jty_ic5[nic5]/F");
  t->Branch("jtphi_ic5",jets_.jtphi_ic5,"jtphi_ic5[nic5]/F");
  t->Branch("preL1et_ic5",jets_.preL1et_ic5,"preL1et_ic5[nic5]/F");
  t->Branch("L2_ic5",jets_.L2_ic5,"L2_ic5[nic5]/F");
  t->Branch("L3_ic5",jets_.L3_ic5,"L3_ic5[nic5]/F");
  t->Branch("area_ic5",jets_.area_ic5,"area_ic5[nic5]/F");

  if(isMC_){
    t->Branch("refpt_ic5",jets_.refpt_ic5,"refpt_ic5[nic5]/F");
    t->Branch("refeta_ic5",jets_.refeta_ic5,"refeta_ic5[nic5]/F");
    t->Branch("refy_ic5",jets_.refy_ic5,"refy_ic5[nic5]/F");
    t->Branch("refphi_ic5",jets_.refphi_ic5,"refphi_ic5[nic5]/F");
    t->Branch("refdrjt_ic5",jets_.refdrjt_ic5,"refdrjt_ic5[nic5]/F");
  }




  // AK5

  t->Branch("rawpt_ak5",jets_.rawpt_ak5,"rawpt_ak5[nak5]/F");
  t->Branch("jtpt_ak5",jets_.jtpt_ak5,"jtpt_ak5[nak5]/F");
  t->Branch("jteta_ak5",jets_.jteta_ak5,"jteta_ak5[nak5]/F");
  t->Branch("jty_ak5",jets_.jty_ak5,"jty_ak5[nak5]/F");
  t->Branch("jtphi_ak5",jets_.jtphi_ak5,"jtphi_ak5[nak5]/F");
  t->Branch("preL1et_ak5",jets_.preL1et_ak5,"preL1et_ak5[nak5]/F");
  t->Branch("L2_ak5",jets_.L2_ak5,"L2_ak5[nak5]/F");
  t->Branch("L3_ak5",jets_.L3_ak5,"L3_ak5[nak5]/F");
  t->Branch("area_ak5",jets_.area_ak5,"area_ak5[nak5]/F");

  if(isMC_){
    t->Branch("refpt_ak5",jets_.refpt_ak5,"refpt_ak5[nak5]/F");
    t->Branch("refeta_ak5",jets_.refeta_ak5,"refeta_ak5[nak5]/F");
    t->Branch("refy_ak5",jets_.refy_ak5,"refy_ak5[nak5]/F");
    t->Branch("refphi_ak5",jets_.refphi_ak5,"refphi_ak5[nak5]/F");
    t->Branch("refdrjt_ak5",jets_.refdrjt_ak5,"refdrjt_ak5[nak5]/F");
  }

  t->Branch("nPFcand",&jets_.nPFcand,"nPFcand/I");
  t->Branch("candID",jets_.candID,"candID[nPFcand]/I");
  t->Branch("candpt",jets_.candpt,"candpt[nPFcand]/F");
  t->Branch("candeta",jets_.candeta,"candeta[nPFcand]/F");
  t->Branch("candy",jets_.candy,"candy[nPFcand]/F");
  t->Branch("candphi",jets_.candphi,"candphi[nPFcand]/F");


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
   


  // not used, taking all jet
  //double jetPtMin = 35.;


   // loop the events
   
   jets_.bin = bin;
   //jets_.hf = hf;
   

   edm::Handle<pat::JetCollection> jets;
   iEvent.getByLabel(jetTag_, jets);

   // add a few more jet algos to the same ntuple
   // default is icPu5

   edm::Handle<pat::JetCollection> jets2;
   iEvent.getByLabel(jetTag2_, jets2);

   edm::Handle<pat::JetCollection> jets3;
   iEvent.getByLabel(jetTag3_, jets3);


   edm::Handle< edm::View<reco::CaloJet> > recoJetColl;
   iEvent.getByLabel( recoJetTag_, recoJetColl );
   
   edm::Handle< edm::View<reco::PFJet> > recoJetColl2;
   iEvent.getByLabel(recoJetTag2_, recoJetColl2 );

   edm::Handle< edm::View<reco::PFJet> > recoJetColl3;
   iEvent.getByLabel(recoJetTag3_, recoJetColl3 );


   Handle<PFCandidateCollection> pfCandidates;
   iEvent.getByLabel(pfCandidatesTag_, pfCandidates);




   
   
   // FILL JRA TREE

   jets_.b = b;



   jets_.nicPu5 = 0;
   

   
   for(unsigned int j = 0 ; j < jets->size(); ++j){
     const pat::Jet& jet = (*jets)[j];
     
     //cout<<" jet pt "<<jet.pt()<<endl;
     //if(jet.pt() < jetPtMin) continue;
     jets_.rawpt_icPu5[jets_.nicPu5]=jet.correctedJet("raw").pt();
     jets_.jtpt_icPu5[jets_.nicPu5] = jet.pt();                            
     jets_.jteta_icPu5[jets_.nicPu5] = jet.eta();
     jets_.jtphi_icPu5[jets_.nicPu5] = jet.phi();
     jets_.jty_icPu5[jets_.nicPu5] = jet.eta();


     //  cout<<" abs corr "<<jet.corrFactor("abs")<<endl;
     //cout<<" abs corr "<<jet.corrFactor("L3Absolute")<<endl;


       float L2Corr = jet.correctedJet("rel").pt()/jet.correctedJet("raw").pt();
       float L3Corr = jet.correctedJet("abs").pt()/jet.correctedJet("rel").pt();
       

       jets_.L2_icPu5[jets_.nicPu5] = L2Corr;
       jets_.L3_icPu5[jets_.nicPu5] = L3Corr;


       jets_.area_icPu5[jets_.nicPu5] = jet.jetArea();

       // Match to reco jet to find unsubtracted jet energy
       
       if(1==0){
	 int recoJetSize = recoJetColl->size();
	 
	 jets_.preL1et_icPu5[jets_.nicPu5] = -1;
	 
	 //cout<<" patJet_eta "<<jet.eta()<<" patJet_phi "<<jet.phi()<<" patJet_et "<<jet.et()<<endl;
	 
	 for(int iRecoJet = 0; iRecoJet < recoJetSize; ++iRecoJet){
	   
	   reco::CaloJet recoJet = ((*recoJetColl)[iRecoJet]);
	   
	   
	   double recoJet_eta = recoJet.eta();
	   double recoJet_phi = recoJet.phi();
	   //cout<<" recoJet_eta "<<recoJet_eta<<" recoJet_phi "<<recoJet_phi<<" recoJet_et "<<recoJet.et()<<endl;
	   
	   
	   if(fabs(recoJet.eta()-jet.eta()) < 0.001
	      && fabs(acos(cos((recoJet.phi()-jet.phi())))) < 0.001)
	     {
	       jets_.preL1et_icPu5[jets_.nicPu5] = recoJet.et();
	       
	       //cout<<"Match found,  recoJet.et "<<recoJet.et()<< " recoJet.eta "<<jet.eta()<<" recoJet.phi "<<recoJet.phi()<<endl;
	       break;
	     }
	 }
	 if(jets_.preL1et_icPu5[jets_.nicPu5] == -1){
	   
	   
	   //There's a known issue here.  If the background subtraction oversubtracts I've set the patJet.et() to zero.  That would be fine if I had also set the eta and phi.  We could then recover the pre-subtracted energy.  However, I never bothered to set the eta and phi for theses jets (doh!).  Next time I repass the data I won't be so stupid.
	   
	   
	   
	   if(jet.et()>0)cout<<"Match *NOT* found,  patJet.et "<<jet.et()<< " patJet.eta "<<jet.eta()<<" patJet.phi() "<<jet.phi()<<endl;
	 }
       }
     if(isMC_){
       
       
       if(jet.genJet()!=0 && jet.genJet()->pt()>1.0 && jet.genJet()->pt()<999999){
	 jets_.refpt_icPu5[jets_.nicPu5] = jet.genJet()->pt();
	 jets_.refeta_icPu5[jets_.nicPu5] = jet.genJet()->eta();
	 jets_.refphi_icPu5[jets_.nicPu5] = jet.genJet()->phi();
	 jets_.refy_icPu5[jets_.nicPu5] = jet.genJet()->eta();
	 
	 jets_.refdrjt_icPu5[jets_.nicPu5] = reco::deltaR(jet,*(jet.genJet()));
       }        
       else{
	 jets_.refpt_icPu5[jets_.nicPu5] = 0;
	 jets_.refeta_icPu5[jets_.nicPu5] = -999;
	 jets_.refphi_icPu5[jets_.nicPu5] = -999;
	 jets_.refy_icPu5[jets_.nicPu5] = -999;
       }
       
     }



       jets_.nicPu5++;
       
       
   }






   jets_.nic5 = 0;
   

   
   for(unsigned int j = 0 ; j < jets2->size(); ++j){
     const pat::Jet& jet2 = (*jets2)[j];

     //cout<<" jet pt "<<jet2.pt()<<endl;
     //if(jet2.pt() < jetPtMin) continue;
     jets_.rawpt_ic5[jets_.nic5]=jet2.correctedJet("raw").pt();
     jets_.jtpt_ic5[jets_.nic5] = jet2.pt();                            
     jets_.jteta_ic5[jets_.nic5] = jet2.eta();
     jets_.jtphi_ic5[jets_.nic5] = jet2.phi();
     jets_.jty_ic5[jets_.nic5] = jet2.eta();
     //  cout<<" abs corr "<<jet2.corrFactor("abs")<<endl;
     //cout<<" abs corr "<<jet2.corrFactor("L3Absolute")<<endl;


       float L2Corr = jet2.correctedJet("rel").pt()/jet2.correctedJet("raw").pt();
       float L3Corr = jet2.correctedJet("abs").pt()/jet2.correctedJet("rel").pt();
       

       jets_.L2_ic5[jets_.nic5] = L2Corr;
       jets_.L3_ic5[jets_.nic5] = L3Corr;

       jets_.area_ic5[jets_.nic5] = jet2.jetArea();

       // Match to reco jet to find unsubtracted jet energy
       if(1==0){
	 int recoJetSize2 = recoJetColl2->size();
	 
	 jets_.preL1et_ic5[jets_.nic5] = -1;
	 
	 //cout<<" patJet_eta "<<jet2.eta()<<" patJet_phi "<<jet2.phi()<<" patJet_et "<<jet2.et()<<endl;
	 
	 for(int iRecoJet = 0; iRecoJet < recoJetSize2; ++iRecoJet){
	   
	   reco::PFJet recoJet2 = ((*recoJetColl2)[iRecoJet]);
	   
	   
	   double recoJet_eta = recoJet2.eta();
	   double recoJet_phi = recoJet2.phi();
	   //cout<<" recoJet_eta "<<recoJet_eta<<" recoJet_phi "<<recoJet_phi<<" recoJet_et "<<recoJet2.et()<<endl;
	   
	   
	   if(fabs(recoJet2.eta()-jet2.eta()) < 0.001
	      && fabs(acos(cos((recoJet2.phi()-jet2.phi())))) < 0.001)
	     {
	       jets_.preL1et_ic5[jets_.nic5] = recoJet2.et();
	       
	       //cout<<"Match found,  recoJet2.et "<<recoJet2.et()<< " recoJet2.eta "<<jet2.eta()<<" recoJet2.phi "<<recoJet2.phi()<<endl;
	       break;
	     }
	 }
	 if(jets_.preL1et_ic5[jets_.nic5] == -1){
	   
	   
	   //There's a known issue here.  If the background subtraction oversubtracts I've set the patJet.et() to zero.  That would be fine if I had also set the eta and phi.  We could then recover the pre-subtracted energy.  However, I never bothered to set the eta and phi for theses jets (doh!).  Next time I repass the data I won't be so stupid.
	   
	   
	   
	   if(jet2.et()>0)cout<<"Match *NOT* found,  patJet2.et "<<jet2.et()<< " patJet2.eta "<<jet2.eta()<<" patJet2.phi() "<<jet2.phi()<<endl;
	 }
       }
     if(isMC_){
       
       
       if(jet2.genJet()!=0 && jet2.genJet()->pt()>1.0 && jet2.genJet()->pt()<999999){
	 jets_.refpt_ic5[jets_.nic5] = jet2.genJet()->pt();
	 jets_.refeta_ic5[jets_.nic5] = jet2.genJet()->eta();
	 jets_.refphi_ic5[jets_.nic5] = jet2.genJet()->phi();
	 jets_.refy_ic5[jets_.nic5] = jet2.genJet()->eta();
	 
	 jets_.refdrjt_ic5[jets_.nic5] = reco::deltaR(jet2,*(jet2.genJet()));
       }        
       else{
	 jets_.refpt_ic5[jets_.nic5] = 0;
	 jets_.refeta_ic5[jets_.nic5] = -999;
	 jets_.refphi_ic5[jets_.nic5] = -999;
	 jets_.refy_ic5[jets_.nic5] = -999;
       }
       
     }

     
     jets_.nic5++;
     
   }
   

   jets_.nak5 = 0;
   
   //cout<<" jets size "<<jets->size()<<endl;

   
   for(unsigned int j = 0 ; j < jets3->size(); ++j){
     const pat::Jet& jet3 = (*jets3)[j];
     
     //cout<<" jet pt "<<jet3.pt()<<endl;
     //if(jet3.pt() < jetPtMin) continue;
     jets_.rawpt_ak5[jets_.nak5]=jet3.correctedJet("raw").pt();
     jets_.jtpt_ak5[jets_.nak5] = jet3.pt();                            
     jets_.jteta_ak5[jets_.nak5] = jet3.eta();
     jets_.jtphi_ak5[jets_.nak5] = jet3.phi();
     jets_.jty_ak5[jets_.nak5] = jet3.eta();
     //  cout<<" abs corr "<<jet3.corrFactor("abs")<<endl;
     //cout<<" abs corr "<<jet3.corrFactor("L3Absolute")<<endl;



       float L2Corr = jet3.correctedJet("rel").pt()/jet3.correctedJet("raw").pt();
       float L3Corr = jet3.correctedJet("abs").pt()/jet3.correctedJet("rel").pt();
       

       jets_.L2_ak5[jets_.nak5] = L2Corr;
       jets_.L3_ak5[jets_.nak5] = L3Corr;

       jets_.area_ak5[jets_.nak5] = jet3.jetArea();

       // Match to reco jet to find unsubtracted jet energy
       if(1==0){
       int recoJetSize3 = recoJetColl3->size();
       
       jets_.preL1et_ak5[jets_.nak5] = -1;
       
       //cout<<" patJet_eta "<<jet3.eta()<<" patJet_phi "<<jet3.phi()<<" patJet_et "<<jet3.et()<<endl;

       for(int iRecoJet = 0; iRecoJet < recoJetSize3; ++iRecoJet){
	 
	 reco::PFJet recoJet3 = ((*recoJetColl3)[iRecoJet]);
	 
	   
	 double recoJet_eta = recoJet3.eta();
	 double recoJet_phi = recoJet3.phi();
	 //cout<<" recoJet_eta "<<recoJet_eta<<" recoJet_phi "<<recoJet_phi<<" recoJet_et "<<recoJet3.et()<<endl;
	 
	 
	 if(fabs(recoJet3.eta()-jet3.eta()) < 0.001
	    && fabs(acos(cos((recoJet3.phi()-jet3.phi())))) < 0.001)
	   {
	     jets_.preL1et_ak5[jets_.nak5] = recoJet3.et();
	     
	     //cout<<"Match found,  recoJet3.et "<<recoJet3.et()<< " recoJet3.eta "<<jet3.eta()<<" recoJet3.phi "<<recoJet3.phi()<<endl;
	     break;
	   }
       }
       if(jets_.preL1et_ak5[jets_.nak5] == -1){

	 
	 //  There's a known issue here.  If the background subtraction oversubtracts I've set the patJet.et() to zero.  That would be fine if I had also set the eta and phi.  We could then recover the pre-subtracted energy.  However, I never bothered to set the eta and phi for theses jets (doh!).  Next time I repass the data I won't be so stupid.
	 

	 
	 if(jet3.et()>0)cout<<"Match *NOT* found,  patJet3.et "<<jet3.et()<< " patJet3.eta "<<jet3.eta()<<" patJet3.phi() "<<jet3.phi()<<endl;
       }
       }
     if(isMC_){
       
       
       if(jet3.genJet()!=0 && jet3.genJet()->pt()>1.0 && jet3.genJet()->pt()<999999){
	 jets_.refpt_ak5[jets_.nak5] = jet3.genJet()->pt();
	 jets_.refeta_ak5[jets_.nak5] = jet3.genJet()->eta();
	 jets_.refphi_ak5[jets_.nak5] = jet3.genJet()->phi();
	 jets_.refy_ak5[jets_.nak5] = jet3.genJet()->eta();
	 
	 jets_.refdrjt_ak5[jets_.nak5] = reco::deltaR(jet3,*(jet3.genJet()));
       }        
       else{
	 jets_.refpt_ak5[jets_.nak5] = 0;
	 jets_.refeta_ak5[jets_.nak5] = -999;
	 jets_.refphi_ak5[jets_.nak5] = -999;
	 jets_.refy_ak5[jets_.nak5] = -999;
       }
       
     }



       jets_.nak5++;
       
       
   }





   for( unsigned icand=0; icand<pfCandidates->size(); icand++ ) {
     
     const reco::PFCandidate& cand = (*pfCandidates)[icand];
     
       float particleEta = cand.eta();
       
       if(fabs(particleEta)>2.5) continue;
       
      

      // PF PID Convention:
      // 1 = Charged Hadrons
      // 2 = Electrons (not included)
      // 3 = Muons
      // 4 = Photons
      // 5 = Neutral Hadrons




       int particleId = (int)cand.particleId();
       float particlePt = cand.pt();
       
       if(particlePt<0.9) continue;
       

       // can use varid thresholds if we want
       //if(particleID==1 && particlePt < 0.9) continue;
       //if(particleID==3 && particlePt < 0.9) continue;
       //if(particleID==4 && particlePt < 0.3) continue;
       //if(particleID==5 && particlePt < 0.9) continue;

       
       

       jets_.candID[jets_.nPFcand] = particleId;
       jets_.candpt[jets_.nPFcand] = particlePt;
       jets_.candeta[jets_.nPFcand] = particleEta;
       jets_.candphi[jets_.nPFcand] = cand.phi();
       jets_.candy[jets_.nPFcand] = cand.y();
       jets_.nPFcand++;

       //cout<<" jets_.nPFcand "<<jets_.nPFcand<<endl;
   }
   


   t->Fill();



   jets_.nicPu5 = 0;
   jets_.nic5 = 0;
   jets_.nak5 = 0;
   jets_.nPFcand = 0;


}

  






				     					
DEFINE_FWK_MODULE(PFJetAnalyzer);
