#include "MNguyen/PFCandAnalyzer/interface/TreePFCandEventData.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "TMath.h"
using namespace std;


// constructors
TreePFCandEventData::TreePFCandEventData(){

}


// set branches
void TreePFCandEventData::SetBranches()
{
  // --event level--
  // -- particle info --
  tree_->Branch("nPFpart",&(this->nPFpart_),"nPFpart/I");
  tree_->Branch("pfId",this->pfId_,"pfId[nPFpart]/I");
  tree_->Branch("pfPt",this->pfPt_,"pfPt[nPFpart]/F");
  tree_->Branch("pfEta",this->pfEta_,"pfEta[nPFpart]/F");

  tree_->Branch("nGENpart",&(this->nGENpart_),"nGENpart/I");
  tree_->Branch("genPDGId",this->genPDGId_,"genPDGId[nGENpart]/I");
  tree_->Branch("genPt",this->genPt_,"genPt[nGENpart]/F");
  tree_->Branch("genEta",this->genEta_,"genEta[nGENpart]/F");


}
void TreePFCandEventData::Clear()
{
  // event

  nPFpart_      = 0;
  nGENpart_      = 0;
}

