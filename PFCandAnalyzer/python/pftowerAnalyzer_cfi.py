import FWCore.ParameterSet.Config as cms

pftowerAna = cms.EDAnalyzer('PFTowerAnalyzer',                        
                        pfTowerLabel = cms.InputTag("particleTowerProducer"),
                        pfPtMin = cms.double(0.5),
                        verbosity = cms.untracked.int32(0)
                        )

