import FWCore.ParameterSet.Config as cms

inclusiveJetAnalyzer = cms.EDAnalyzer("InclusiveJetAnalyzer",
                                              jetTag = cms.InputTag("icPu5patJets"),
                                              isMC = cms.untracked.bool(True), 
                                              useCentrality = cms.untracked.bool(True),
                                              L1gtReadout = cms.InputTag("gtDigis"),
                                              hltTrgResults = cms.untracked.string("TriggerResults::HLT"),
                                              hltTrgNames  = cms.untracked.vstring('HLT_HIMinBiasHfOrBSC_Core',
                                                                                   'HLT_HIJet35U',
                                                                                   'HLT_HIJet35U_Core',
                                                                                   'HLT_HIJet50U_Core')
                                              )


ic3JetAnalyzer = inclusiveJetAnalyzer.clone()
ic3JetAnalyzer.jetTag = 'ic3patJets'

ic4JetAnalyzer = inclusiveJetAnalyzer.clone()
ic4JetAnalyzer.jetTag = 'ic4patJets'

ic5JetAnalyzer = inclusiveJetAnalyzer.clone()
ic5JetAnalyzer.jetTag = 'ic5patJets'


ak3JetAnalyzer = inclusiveJetAnalyzer.clone()
ak3JetAnalyzer.jetTag = 'ak3patJets'

ak4JetAnalyzer = inclusiveJetAnalyzer.clone()
ak4JetAnalyzer.jetTag = 'ak4patJets'

ak5JetAnalyzer = inclusiveJetAnalyzer.clone()
ak5JetAnalyzer.jetTag = 'ak5patJets'

ak7JetAnalyzer = inclusiveJetAnalyzer.clone()
ak7JetAnalyzer.jetTag = 'ak7patJets'

kt4JetAnalyzer = inclusiveJetAnalyzer.clone()
kt4JetAnalyzer.jetTag = 'kt4patJets'

ic3PFJetAnalyzer = inclusiveJetAnalyzer.clone()
ic3PFJetAnalyzer.jetTag = 'ic3PFpatJets'

ic4PFJetAnalyzer = inclusiveJetAnalyzer.clone()
ic4PFJetAnalyzer.jetTag = 'ic4PFpatJets'

ic5PFJetAnalyzer = inclusiveJetAnalyzer.clone()
ic5PFJetAnalyzer.jetTag = 'ic5PFpatJets'


ak3PFJetAnalyzer = inclusiveJetAnalyzer.clone()
ak3PFJetAnalyzer.jetTag = 'ak3PFpatJets'

ak4PFJetAnalyzer = inclusiveJetAnalyzer.clone()
ak4PFJetAnalyzer.jetTag = 'ak4PFpatJets'

ak5PFJetAnalyzer = inclusiveJetAnalyzer.clone()
ak5PFJetAnalyzer.jetTag = 'ak5PFpatJets'

ak7PFJetAnalyzer = inclusiveJetAnalyzer.clone()
ak7PFJetAnalyzer.jetTag = 'ak7PFpatJets'

icPu5JPTJetAnalyzer = inclusiveJetAnalyzer.clone()
icPu5JPTJetAnalyzer.jetTag = 'jpticPu5patJets'


allJetAnalyzers = cms.Sequence(                         inclusiveJetAnalyzer
                                                        *ic3JetAnalyzer
                                                        *ic4JetAnalyzer
                                                        *ic5JetAnalyzer
                                                        *ak3JetAnalyzer
                                                        *ak4JetAnalyzer
                                                        *ak5JetAnalyzer
                                                        *kt4JetAnalyzer
                                                        *ak7JetAnalyzer
                                                        *ic3PFJetAnalyzer
                                                        *ic4PFJetAnalyzer
                                                        *ic5PFJetAnalyzer
                                                        *ak3PFJetAnalyzer
                                                        *ak4PFJetAnalyzer
                                                        *ak5PFJetAnalyzer
                                                        *ak7PFJetAnalyzer
                                                        *icPu5JPTJetAnalyzer
                                                        )



