import FWCore.ParameterSet.Config as cms

PFJetAnalyzer = cms.EDAnalyzer("PFJetAnalyzer",
                               jetTag1 = cms.InputTag("icPu5patJets"),
                               jetTag2 = cms.InputTag("akPu5PFpatJets"),
                               jetTag3 = cms.InputTag("akPu4PFpatJets"),
                               jetTag4 = cms.InputTag("akPu3PFpatJets"),
                               recoJetTag1 = cms.InputTag("iterativeConePu5CaloJets"),
                               recoJetTag2 = cms.InputTag("akPu5PFJets"),
                               recoJetTag3 = cms.InputTag("akPu4PFJets"),
                               recoJetTag4 = cms.InputTag("akPu3PFJets"),
                               genJetTag1 = cms.InputTag("iterativeCone5HiGenJets"),
                               genJetTag2 = cms.InputTag("ak5HiGenJets"),
                               genJetTag3 = cms.InputTag("ak4HiGenJets"),
                               genJetTag4 = cms.InputTag("ak3HiGenJets"),
                               pfCandidatesTag  = cms.InputTag("particleFlow",""),
                               trackTag  = cms.InputTag("hiGoodTracks"),
                               
                               isMC = cms.untracked.bool(True), 
                               writeGenParticles = cms.untracked.bool(True), 
                               genParticleTag = cms.InputTag("hiGenParticles"),
                               eventInfoTag = cms.InputTag("generator"),
                               useCentrality = cms.untracked.bool(True),
                               hasSimInfo = cms.untracked.bool(False),
                               SimTracks = cms.InputTag("mergedtruth","MergedTrackTruth")

                               )




# Paths

PFJetAnalyzerSequence = cms.Sequence(PFJetAnalyzer    
                 )



