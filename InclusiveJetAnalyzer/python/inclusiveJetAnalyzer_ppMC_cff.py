import FWCore.ParameterSet.Config as cms

inclusiveJetAnalyzer_ppMC = cms.EDAnalyzer("InclusiveJetAnalyzer",
                                      jetTag = cms.InputTag("icPu5patJets"),
                                      genjetTag = cms.InputTag("iterativeCone5HiGenJets"),
                                      isMC = cms.untracked.bool(False), 
                                      useCentrality = cms.untracked.bool(True),
                                      L1gtReadout = cms.InputTag("gtDigis"),
                                      hltTrgResults = cms.untracked.string("TriggerResults::HLT"),
                                      hltTrgNames  = cms.untracked.vstring('HLT_HIMinBiasHfOrBSC_Core',
                                                                           'HLT_HIJet35U',
                                                                           'HLT_HIJet35U_Core',
                                                                           'HLT_HIJet50U_Core')
                                      )


ic3JetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ic3JetAnalyzer_ppMC.jetTag = 'ic3patJets'
ic3JetAnalyzer_ppMC.genjetTag = 'iterativeCone3HiGenJets'

ic4JetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ic4JetAnalyzer_ppMC.jetTag = 'ic4patJets'
ic4JetAnalyzer_ppMC.genjetTag = 'iterativeCone4HiGenJets'

ic5JetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ic5JetAnalyzer_ppMC.jetTag = 'ic5patJets'
ic4JetAnalyzer_ppMC.genjetTag = 'iterativeCone5HiGenJets'

ak3JetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ak3JetAnalyzer_ppMC.jetTag = 'ak3patJets'
ak3JetAnalyzer_ppMC.genjetTag = 'ak3HiGenJets'

ak4JetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ak4JetAnalyzer_ppMC.jetTag = 'ak4patJets'
ak4JetAnalyzer_ppMC.genjetTag = 'ak4HiGenJets'

ak5JetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ak5JetAnalyzer_ppMC.jetTag = 'ak5patJets'
ak5JetAnalyzer_ppMC.genjetTag = 'ak5HiGenJets'

ak7JetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ak7JetAnalyzer_ppMC.jetTag = 'ak7patJets'
ak7JetAnalyzer_ppMC.genjetTag = 'ak7HiGenJets'

kt4JetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
kt4JetAnalyzer_ppMC.jetTag = 'kt4patJets'
kt4JetAnalyzer_ppMC.genjetTag = 'kt4HiGenJets'

ic3PFJetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ic3PFJetAnalyzer_ppMC.jetTag = 'ic3PFpatJets'
ic3PFJetAnalyzer_ppMC.genjetTag = 'iterativeCone3HiGenJets'

ic4PFJetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ic4PFJetAnalyzer_ppMC.jetTag = 'ic4PFpatJets'
ic4PFJetAnalyzer_ppMC.genjetTag = 'iterativeCone4HiGenJets'

ic5PFJetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ic5PFJetAnalyzer_ppMC.jetTag = 'ic5PFpatJets'
ic5PFJetAnalyzer_ppMC.genjetTag = 'iterativeCone5HiGenJets'

ak3PFJetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ak3PFJetAnalyzer_ppMC.jetTag = 'ak3PFpatJets'
ak3PFJetAnalyzer_ppMC.genjetTag = 'ak3HiGenJets'

ak4PFJetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ak4PFJetAnalyzer_ppMC.jetTag = 'ak4PFpatJets'
ak4PFJetAnalyzer_ppMC.genjetTag = 'ak4HiGenJets'

ak5PFJetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ak5PFJetAnalyzer_ppMC.jetTag = 'ak5PFpatJets'
ak5PFJetAnalyzer_ppMC.genjetTag = 'ak5HiGenJets'

ak7PFJetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
ak7PFJetAnalyzer_ppMC.jetTag = 'ak7PFpatJets'
ak7PFJetAnalyzer_ppMC.genjetTag = 'ak7HiGenJets'

icPu5JPTJetAnalyzer_ppMC = inclusiveJetAnalyzer_ppMC.clone()
icPu5JPTJetAnalyzer_ppMC.jetTag = 'jpticPu5patJets'
icPu5JPTJetAnalyzer_ppMC.genjetTag = 'iterativeCone5HiGenJets'

allJetAnalyzers_ppMC = cms.Sequence(                         inclusiveJetAnalyzer_ppMC
                                                        *ic3JetAnalyzer_ppMC
                                                        *ic4JetAnalyzer_ppMC
                                                        *ic5JetAnalyzer_ppMC
                                                        *ak3JetAnalyzer_ppMC
                                                        *ak4JetAnalyzer_ppMC
                                                        *ak5JetAnalyzer_ppMC
                                                        *kt4JetAnalyzer_ppMC
                                                        *ak7JetAnalyzer_ppMC
                                                        *ic3PFJetAnalyzer_ppMC
                                                        *ic4PFJetAnalyzer_ppMC
                                                        *ic5PFJetAnalyzer_ppMC
                                                        *ak3PFJetAnalyzer_ppMC
                                                        *ak4PFJetAnalyzer_ppMC
                                                        *ak5PFJetAnalyzer_ppMC
                                                        *ak7PFJetAnalyzer_ppMC
                                                        *icPu5JPTJetAnalyzer_ppMC
                                                        )



