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


  cout<<" jet collection : "<<jetTag_<<endl;
  cout<<" jet collection 2: "<<jetTag2_<<endl;
  cout<<" jet collection 3: "<<jetTag3_<<endl;
  cout<<" jet collection 4: "<<jetTag4_<<endl;

   jets_.nicPu5 = 0;
   jets_.nic5 = 0;
   jets_.nic5FJ = 0;
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
  t->Branch("nicPu5",&jets_.nicPu5,"nicPu5/I");
  t->Branch("nic5",&jets_.nic5,"nic5/I");
  t->Branch("nic5FJ",&jets_.nic5FJ,"nic5FJ/I");
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

 // IC5FJ

  t->Branch("rawpt_ic5FJ",jets_.rawpt_ic5FJ,"rawpt_ic5FJ[nic5FJ]/F");
  t->Branch("jtpt_ic5FJ",jets_.jtpt_ic5FJ,"jtpt_ic5FJ[nic5FJ]/F");
  t->Branch("jteta_ic5FJ",jets_.jteta_ic5FJ,"jteta_ic5FJ[nic5FJ]/F");
  t->Branch("jty_ic5FJ",jets_.jty_ic5FJ,"jty_ic5FJ[nic5FJ]/F");
  t->Branch("jtphi_ic5FJ",jets_.jtphi_ic5FJ,"jtphi_ic5FJ[nic5FJ]/F");
  t->Branch("preL1et_ic5FJ",jets_.preL1et_ic5FJ,"preL1et_ic5FJ[nic5FJ]/F");
  t->Branch("L2_ic5FJ",jets_.L2_ic5FJ,"L2_ic5FJ[nic5FJ]/F");
  t->Branch("L3_ic5FJ",jets_.L3_ic5FJ,"L3_ic5FJ[nic5FJ]/F");
  t->Branch("area_ic5FJ",jets_.area_ic5FJ,"area_ic5FJ[nic5FJ]/F");

  if(isMC_){
    t->Branch("refpt_ic5FJ",jets_.refpt_ic5FJ,"refpt_ic5FJ[nic5FJ]/F");
    t->Branch("refeta_ic5FJ",jets_.refeta_ic5FJ,"refeta_ic5FJ[nic5FJ]/F");
    t->Branch("refy_ic5FJ",jets_.refy_ic5FJ,"refy_ic5FJ[nic5FJ]/F");
    t->Branch("refphi_ic5FJ",jets_.refphi_ic5FJ,"refphi_ic5FJ[nic5FJ]/F");
    t->Branch("refdrjt_ic5FJ",jets_.refdrjt_ic5FJ,"refdrjt_ic5FJ[nic5FJ]/F");
  }

  t->Branch("nPFcand",&jets_.nPFcand,"nPFcand/I");
  t->Branch("candID",jets_.candID,"candID[nPFcand]/I");
  t->Branch("candpt",jets_.candpt,"candpt[nPFcand]/F");
  t->Branch("candeta",jets_.candeta,"candeta[nPFcand]/F");
  t->Branch("candy",jets_.candy,"candy[nPFcand]/F");
  t->Branch("candphi",jets_.candphi,"candphi[nPFcand]/F");

  t->Branch("ntrack",&jets_.ntrack,"ntrack/I");
  t->Branch("tracknhits",jets_.tracknhits,"tracknhits[ntrack]/I");
  t->Branch("trackpt",jets_.trackpt,"trackpt[ntrack]/F");
  t->Branch("tracketa",jets_.tracketa,"tracketa[ntrack]/F");
  t->Branch("trackphi",jets_.trackphi,"trackphi[ntrack]/F");
  t->Branch("tracksumecal",jets_.tracksumecal,"tracksumecal[ntrack]/F");
  t->Branch("tracksumhcal",jets_.tracksumhcal,"tracksumhcal[ntrack]/F");


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
   // default is icPu5

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





   jets_.nic5FJ = 0;
   

   
   for(unsigned int j = 0 ; j < jets4->size(); ++j){
     const pat::Jet& jet4 = (*jets4)[j];

     //cout<<" jet pt "<<jet4.pt()<<endl;
     //if(jet4.pt() < jetPtMin) continue;
     jets_.rawpt_ic5FJ[jets_.nic5FJ]=jet4.correctedJet("raw").pt();
     jets_.jtpt_ic5FJ[jets_.nic5FJ] = jet4.pt();                            
     jets_.jteta_ic5FJ[jets_.nic5FJ] = jet4.eta();
     jets_.jtphi_ic5FJ[jets_.nic5FJ] = jet4.phi();
     jets_.jty_ic5FJ[jets_.nic5FJ] = jet4.eta();
     //  cout<<" abs corr "<<jet4.corrFactor("abs")<<endl;
     //cout<<" abs corr "<<jet4.corrFactor("L3Absolute")<<endl;


       float L2Corr = jet4.correctedJet("rel").pt()/jet4.correctedJet("raw").pt();
       float L3Corr = jet4.correctedJet("abs").pt()/jet4.correctedJet("rel").pt();
       

       jets_.L2_ic5FJ[jets_.nic5FJ] = L2Corr;
       jets_.L3_ic5FJ[jets_.nic5FJ] = L3Corr;

       jets_.area_ic5FJ[jets_.nic5FJ] = jet4.jetArea();

       // Match to reco jet to find unsubtracted jet energy
       if(1==0){
	 int recoJetSize4 = recoJetColl4->size();
	 
	 jets_.preL1et_ic5FJ[jets_.nic5FJ] = -1;
	 
	 //cout<<" patJet_eta "<<jet4.eta()<<" patJet_phi "<<jet4.phi()<<" patJet_et "<<jet4.et()<<endl;
	 
	 for(int iRecoJet = 0; iRecoJet < recoJetSize4; ++iRecoJet){
	   
	   reco::CaloJet recoJet4 = ((*recoJetColl4)[iRecoJet]);
	   
	   
	   double recoJet_eta = recoJet4.eta();
	   double recoJet_phi = recoJet4.phi();
	   //cout<<" recoJet_eta "<<recoJet_eta<<" recoJet_phi "<<recoJet_phi<<" recoJet_et "<<recoJet4.et()<<endl;
	   
	   
	   if(fabs(recoJet4.eta()-jet4.eta()) < 0.001
	      && fabs(acos(cos((recoJet4.phi()-jet4.phi())))) < 0.001)
	     {
	       jets_.preL1et_ic5FJ[jets_.nic5FJ] = recoJet4.et();
	       
	       //cout<<"Match found,  recoJet4.et "<<recoJet4.et()<< " recoJet4.eta "<<jet4.eta()<<" recoJet4.phi "<<recoJet4.phi()<<endl;
	       break;
	     }
	 }
	 if(jets_.preL1et_ic5FJ[jets_.nic5FJ] == -1){
	   
	   
	   //There's a known issue here.  If the background subtraction oversubtracts I've set the patJet.et() to zero.  That would be fine if I had also set the eta and phi.  We could then recover the pre-subtracted energy.  However, I never bothered to set the eta and phi for theses jets (doh!).  Next time I repass the data I won't be so stupid.
	   
	   
	   
	   if(jet4.et()>0)cout<<"Match *NOT* found,  patJet4.et "<<jet4.et()<< " patJet4.eta "<<jet4.eta()<<" patJet4.phi() "<<jet4.phi()<<endl;
	 }
       }
     if(isMC_){
       
       
       if(jet4.genJet()!=0 && jet4.genJet()->pt()>1.0 && jet4.genJet()->pt()<999999){
	 jets_.refpt_ic5FJ[jets_.nic5FJ] = jet4.genJet()->pt();
	 jets_.refeta_ic5FJ[jets_.nic5FJ] = jet4.genJet()->eta();
	 jets_.refphi_ic5FJ[jets_.nic5FJ] = jet4.genJet()->phi();
	 jets_.refy_ic5FJ[jets_.nic5FJ] = jet4.genJet()->eta();
	 
	 jets_.refdrjt_ic5FJ[jets_.nic5FJ] = reco::deltaR(jet4,*(jet4.genJet()));
       }        
       else{
	 jets_.refpt_ic5FJ[jets_.nic5FJ] = 0;
	 jets_.refeta_ic5FJ[jets_.nic5FJ] = -999;
	 jets_.refphi_ic5FJ[jets_.nic5FJ] = -999;
	 jets_.refy_ic5FJ[jets_.nic5FJ] = -999;
       }
       
     }

     
     jets_.nic5FJ++;
     
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
   
   for(unsigned int it=0; it<tracks->size(); ++it){
     const reco::Track & track = (*tracks)[it];

     // Could makes some track selection here
     jets_.tracknhits[jets_.ntrack] = track.numberOfValidHits();
     jets_.trackpt[jets_.ntrack] = track.pt();
     jets_.tracketa[jets_.ntrack] = track.eta();
     jets_.trackphi[jets_.ntrack] = track.phi();

     jets_.tracksumecal[jets_.ntrack] = 0.;
     jets_.tracksumhcal[jets_.ntrack] = 0.;


     reco::TrackRef trackref=reco::TrackRef(tracks,it);

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
	 

	 //reco::Track track_pfCand = cand.track();
	 // working with 2 different track collections so this doesn't work
	 // if(cand.trackRef() != trackref) continue;

	 if(fabs(cand.pt()-track.pt())>0.001||fabs(cand.eta()-track.eta())>0.001||fabs(acos(cos(cand.phi()-track.phi())))>0.001) continue;

	 pfCandMatchFound = 1;
	 cout<<" found matching pf cand "<<endl;
    
	 cout<<" elements in block = "<<cand.elementsInBlocks().size()<<endl;
	 for(unsigned iblock=0; iblock<cand.elementsInBlocks().size(); iblock++) {
	   cout<<" getting block ref "<<endl;
	   PFBlockRef blockRef = cand.elementsInBlocks()[iblock].first;
      	   cout<<" got block ref "<<endl;
      
	   cout<<" getting block index "<<endl;
	   unsigned indexInBlock = cand.elementsInBlocks()[iblock].second;
	   cout<<" block index = "<<indexInBlock<<endl;
	   if(cand.elements()) cout<<" yes "<<endl;
	   else cout<<" no "<<endl;
	   
	   const edm::OwnVector<  reco::PFBlockElement>&  elements = (*blockRef).elements();
	   cout<<" hello "<<endl;
	   //This tells you what type of element it is:
	   cout<<" block type"<<elements[indexInBlock].type()<<endl;
	   
	   switch (elements[indexInBlock].type()) {
	     
	   case PFBlockElement::ECAL: {
	     reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
	     double eet = clusterRef->energy()/cosh(clusterRef->eta());
	     //if(verbose_)cout<<" ecal energy "<<clusterRef->energy()<<endl;
	     cout<<" ecal energy "<<clusterRef->energy()<<endl;
	     jets_.tracksumecal[jets_.ntrack] += eet;
	     break;
	   }
	     
	   case PFBlockElement::HCAL: {
	     reco::PFClusterRef clusterRef = elements[indexInBlock].clusterRef();
	     double eet = clusterRef->energy()/cosh(clusterRef->eta());
	     //if(verbose_)cout<<" hcal energy "<<clusterRef->energy()<<endl;
	     cout<<" hcal energy "<<clusterRef->energy()<<endl;
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
   
   
   t->Fill();



   jets_.nicPu5 = 0;
   jets_.nic5 = 0;
   jets_.nic5FJ = 0;
   jets_.nak5 = 0;
   jets_.nPFcand = 0;
   jets_.ntrack = 0;

}

  






				     					
DEFINE_FWK_MODULE(PFJetAnalyzer);
