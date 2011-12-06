import FWCore.ParameterSet.Config as cms

electronAnalyzer = cms.EDAnalyzer("ElectronAnalyzer",
                                  eleTag = cms.InputTag("ecalDrivenGsfElectrons"),
                                  mcTag = cms.InputTag("generator"),
                                  isMC = cms.untracked.bool(True),
                                  useVtx = cms.untracked.bool(False),
                                  useCentrality = cms.untracked.bool(False),
                                  L1gtReadout = cms.InputTag("gtDigis"),
                                  hltTrgResults = cms.untracked.string("TriggerResults::HLT"),
                                  #put some triggers you care about
                                  hltTrgNames  = cms.untracked.vstring('HLT_HIMinBiasHfOrBSC_Core',
                                                                       'HLT_HIJet35U',
                                                                       'HLT_HIJet35U_Core',
                                                                       'HLT_HIJet50U_Core')
                                  )


