import FWCore.ParameterSet.Config as cms

electronAnalyzer = cms.EDAnalyzer("electronAnalyzer",
                                      eleTag = cms.InputTag("ecalDrivenGsfElectrons"),
                                      eventInfoTag = cms.InputTag("generator"),
                                      isMC = cms.untracked.bool(True), 
                                      useCentrality = cms.untracked.bool(True),
                                      L1gtReadout = cms.InputTag("gtDigis"),
                                      hltTrgResults = cms.untracked.string("TriggerResults::HLT"),
                                      #put some triggers you care about
                                      hltTrgNames  = cms.untracked.vstring('HLT_HIMinBiasHfOrBSC_Core',
                                                                           'HLT_HIJet35U',
                                                                           'HLT_HIJet35U_Core',
                                                                           'HLT_HIJet50U_Core')
                                      )


