import FWCore.ParameterSet.Config as cms

pfcand = cms.EDAnalyzer('PFCandAnalyzer',

        pfCandidateLabel = cms.InputTag("particleFlow","","HIJETS"),
        jetLabel = cms.InputTag("ak5patJets","","HIJETS"),
        genLabel = cms.InputTag("hiGenParticles"),
                        # debug
    pfPtMin = cms.double(0.5),
    genPtMin = cms.double(0.5),
    jetPtMin = cms.double(20.0),

    verbosity = cms.untracked.int32(0)
    )

