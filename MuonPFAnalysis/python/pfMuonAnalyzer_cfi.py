import FWCore.ParameterSet.Config as cms

pfmuon = cms.EDAnalyzer('PFMuonAnalyzer',

    pfCandidateLabel = cms.InputTag("particleFlow"),
    recoMuonLabel = cms.InputTag("muons"),
    beamSpotLabel = cms.InputTag("offlineBeamSpot"),
    genLabel = cms.InputTag("genParticles"),

    verbosity = cms.untracked.int32(0)
    )

