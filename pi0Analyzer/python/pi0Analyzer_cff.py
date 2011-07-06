import FWCore.ParameterSet.Config as cms

pi0Analyzer = cms.EDAnalyzer("pi0Analyzer",
                             pfCandidatesTag  = cms.InputTag("particleFlow",""),
                             isMC = cms.untracked.bool(True), 
                             useCentrality = cms.untracked.bool(True)
                             )


