import FWCore.ParameterSet.Config as cms

jetFilter = cms.EDFilter("JetFilter",
                         # The collections to read
                         patJetLabel = cms.InputTag("ak5patJets"),
                         genJetLabel = cms.InputTag("ak5HiGenJets"),
                         verbose = cms.bool(True),
                         # Keep events where the Pt is larger than this value
                         minPt = cms.double(50.),
                         )
