#ifndef JetFilter_H
#define JetFilter_H

// author: Florent Lacroix (UIC)
// date: 07/14/2009

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Utilities/interface/InputTag.h"

class JetFilter: public edm::EDFilter{
 public:

  explicit JetFilter(const edm::ParameterSet&);
  virtual ~JetFilter();

  virtual bool filter(edm::Event&, const edm::EventSetup&);
  virtual void beginJob();
  virtual void endJob();
  bool checkInput();

 private:

  ///InputTags for physics objects
  edm::InputTag patJetLabel_;
  edm::InputTag genJetLabel_;

  bool verbose_;
  bool cleaned_;

  double minPt_;

};

#endif // JetFilter_H
