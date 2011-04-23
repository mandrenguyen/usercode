import FWCore.ParameterSet.Config as cms

PFJetAnalyzer = cms.EDAnalyzer("PFJetAnalyzer",
                               jetTag1 = cms.InputTag("icPu5patJets"),
                               jetTag2 = cms.InputTag("ic5PFpatJets"),
                               jetTag3 = cms.InputTag("ak5PFpatJets"),
                               jetTag4 = cms.InputTag("ic5patJets"),
                               recoJetTag1 = cms.InputTag("iterativeConePu5CaloJets"),
                               recoJetTag2 = cms.InputTag("ic5PFJets"),
                               recoJetTag3 = cms.InputTag("ak5PFJets"),
                               recoJetTag4 = cms.InputTag("ic5CaloJets"),
                               genJetTag1 = cms.InputTag("iterativeCone5HiGenJets"),
                               genJetTag2 = cms.InputTag("ak5HiGenJets"),
                               genJetTag3 = cms.InputTag("ak4HiGenJets"),
                               genJetTag4 = cms.InputTag("ak3HiGenJets"),
                               pfCandidatesTag  = cms.InputTag("particleFlow",""),
                               trackTag  = cms.InputTag("hiGoodTracks"),
                               isMC = cms.untracked.bool(True), 
                               genParticleTrhesh = cms.double(0.5),
                               genParticleTag = cms.InputTag("hiGenParticles"),
                               eventInfoTag = cms.InputTag("generator"),
                               useCentrality = cms.untracked.bool(False),
                               hasSimInfo = cms.untracked.bool(False),
                               SimTracks = cms.InputTag("mergedtruth","MergedTrackTruth")
                                       )




# Paths

PFJetAnalyzerSequence = cms.Sequence(PFJetAnalyzer    
                 )



