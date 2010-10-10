//
// Class to represent event tree data
//

#ifndef PFCANDEVENTDATA_H
#define PFCANDEVENTDATA_H
#include <vector>
#include "TTree.h"
#include "DataFormats/Math/interface/LorentzVector.h"
#include <vector>

const Int_t MAXPARTICLE = 100000;
//
// DiJet ana Event Data Tree definition
//
class TreePFCandEventData
{
  public:
    // ===== Class Methods =====
    void SetDefaults();
    TreePFCandEventData();
    void SetTree(TTree * t) { tree_=t; }
    void SetBranches();
    void Clear();


    Float_t		    jdphi_;


    // -- particle info --
    Int_t		    nPFpart_, nGENpart_;
    Int_t	  	    pfId_[MAXPARTICLE], genPDGId_[MAXPARTICLE];
    Float_t	  	    pfPt_[MAXPARTICLE], genPt_[MAXPARTICLE];
    Float_t	  	    pfEta_[MAXPARTICLE], genEta_[MAXPARTICLE];


  private:
    TTree*                 tree_;
};
#endif // PFCANDEVENTDATA_H
