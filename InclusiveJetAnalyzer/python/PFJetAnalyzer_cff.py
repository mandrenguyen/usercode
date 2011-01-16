import FWCore.ParameterSet.Config as cms

PFJetAnalyzer = cms.EDAnalyzer("PFJetAnalyzer",
                               jetTag = cms.InputTag("icPu5patJets"),
                               jetTag2 = cms.InputTag("ic5PFpatJets"),
                               jetTag3 = cms.InputTag("ak5PFpatJets"),
                               jetTag4 = cms.InputTag("ic5patJets"),
                               recoJetTag = cms.InputTag("iterativeConePu5CaloJets"),
                               recoJetTag2 = cms.InputTag("ic5PFJets"),
                               recoJetTag3 = cms.InputTag("ak5PFJets"),
                               recoJetTag4 = cms.InputTag("ic5CaloJets"),
                               pfCandidatesTag  = cms.InputTag("particleFlow",""),
                               trackTag  = cms.InputTag("hiGoodMergedTracks"),
                               isMC = cms.untracked.bool(False), 
                               genParticleTag = cms.InputTag("hiGenParticles"),
                               useCentrality = cms.untracked.bool(True),
                               hasSimInfo = cms.untracked.bool(False),
                               SimTracks = cms.InputTag("mergedtruth","MergedTrackTruth")
                               )




# Paths

PFJetAnalyzerSequence = cms.Sequence(PFJetAnalyzer    
                 )



