import FWCore.ParameterSet.Config as cms

pfcand = cms.EDAnalyzer('PFCandAnalyzer',

        pfCandidateLabel = cms.InputTag("particleFlow","","HIJETS"),
        genLabel = cms.InputTag("hiGenParticles"),
                        # debug
    pfPtMin = cms.double(0.5),
    genPtMin = cms.double(0.5),

    verbosity = cms.untracked.int32(0)
    )

