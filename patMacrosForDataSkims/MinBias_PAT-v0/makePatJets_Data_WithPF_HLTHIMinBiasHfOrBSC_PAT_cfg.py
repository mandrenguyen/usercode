import FWCore.ParameterSet.Config as cms

process = cms.Process('HIJETS')


#process.load("HeavyIonsAnalysis.Configuration.Sources.Data2010_Run150476_Express_HLT_MinBias_cff")

process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring("rfio:/castor/cern.ch/user/e/edwenger/skims/mergeCleanDiJetall.root")
                            )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'GR10_P_V12::All'  #Data

#even though we're not doing reconstruction, this is needed for the good collision filter
process.load("Configuration.StandardSequences.ReconstructionHeavyIons_cff")
process.load('Configuration.StandardSequences.GeometryExtended_cff')
#process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')


# --- Pat Stuff --
process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)



process.load("HeavyIonsAnalysis.Configuration.analysisProducers_cff")
process.hiExtra = cms.Sequence(
    process.allTracks 
    )

# Define Jet Algo parameters
process.load('RecoHI.HiJetAlgos.HiRecoJets_cff')

#Now the correct L1 
process.load("RecoHI.HiJetAlgos.HiL1Corrector_cff")


process.ic5sub = process.heavyIonL1SubtractedJets.clone()
process.ic5sub.src = 'ic5CaloJets'
process.ic5sub.rhoTag = 'kt4CaloJets'

process.patJets.embedCaloTowers = cms.bool(False)

process.ic5corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ic5sub"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_IC5Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_IC5Calo"),
                                                                        L5Flavor = cms.string("none")))

process.ic5patJets = process.patJets.clone(jetSource  = cms.InputTag("ic5sub"),
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ic5corr")))

process.ic5patSequence = cms.Sequence(process.ic5CaloJets*process.ic5sub*process.ic5corr*process.ic5patJets)


process.ak5sub = process.heavyIonL1SubtractedJets.clone()
process.ak5sub.src = 'ak5CaloJets'
process.ak5sub.rhoTag = 'kt4CaloJets'


process.ak5corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ak5sub"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_AK5Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_AK5Calo"),
                                                                        L5Flavor = cms.string("none")))
process.ak5patJets = process.patJets.clone(jetSource  = cms.InputTag("ak5sub"),
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ak5corr")))
process.ak5patSequence = cms.Sequence(process.ak5CaloJets*process.ak5sub*process.ak5corr*process.ak5patJets)



process.ak7sub = process.heavyIonL1SubtractedJets.clone()
process.ak7sub.src = 'ak7CaloJets'
process.ak7sub.rhoTag = 'kt4CaloJets'


process.ak7corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ak7sub"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_AK7Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_AK7Calo"),
                                                                        L5Flavor = cms.string("none")))
process.ak7patJets = process.patJets.clone(jetSource  = cms.InputTag("ak7sub"),

                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ak7corr")))
process.ak7patSequence = cms.Sequence(process.ak7CaloJets*process.ak7sub*process.ak7corr*process.ak7patJets)



process.kt4sub = process.heavyIonL1SubtractedJets.clone()
process.kt4sub.src = 'kt4CaloJets'
process.kt4sub.rhoTag = 'kt4CaloJets'


process.kt4corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("kt4sub"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_KT4Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_KT4Calo"),
                                                                        L5Flavor = cms.string("none")))
process.kt4patJets = process.patJets.clone(jetSource  = cms.InputTag("kt4sub"),

                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("kt4corr")))
process.kt4patSequence = cms.Sequence(process.kt4CaloJets*process.kt4sub*process.kt4corr*process.kt4patJets)


process.kt6sub = process.heavyIonL1SubtractedJets.clone()
process.kt6sub.src = 'kt6CaloJets'
process.kt6sub.rhoTag = 'kt4CaloJets'


process.kt6corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("kt6sub"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_KT6Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_KT6Calo"),
                                                                        L5Flavor = cms.string("none")))

process.kt6patJets = process.patJets.clone(jetSource  = cms.InputTag("kt6sub"),
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("kt6corr")))
process.kt6patSequence = cms.Sequence(process.kt6CaloJets*process.kt6sub*process.kt6corr*process.kt6patJets)






process.akPu5corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("akPu5CaloJets"),
                                                    corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_AK5Calo"),
                                                                          L3Absolute = cms.string("HI_L3Absolute_AK5Calo"),
                                                                          L5Flavor = cms.string("none")))
process.akPu5patJets = process.patJets.clone(jetSource  = cms.InputTag("akPu5CaloJets"),
                                             jetCorrFactorsSource = cms.VInputTag(cms.InputTag("akPu5corr")))
process.akPu5patSequence = cms.Sequence(process.akPu5CaloJets*process.akPu5corr*process.akPu5patJets)


process.akPu7corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("akPu7CaloJets"),
                                                    corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_AK7Calo"),
                                                                          L3Absolute = cms.string("HI_L3Absolute_AK7Calo"),
                                                                          L5Flavor = cms.string("none")))
process.akPu7patJets = process.patJets.clone(jetSource  = cms.InputTag("akPu7CaloJets"),
                                             jetCorrFactorsSource = cms.VInputTag(cms.InputTag("akPu7corr")))
process.akPu7patSequence = cms.Sequence(process.akPu7CaloJets*process.akPu7corr*process.akPu7patJets)



process.ktPu4corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ktPu4CaloJets"),
                                                    corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_KT4Calo"),
                                                                          L3Absolute = cms.string("HI_L3Absolute_KT4Calo"),
                                                                          L5Flavor = cms.string("none")))
process.ktPu4patJets = process.patJets.clone(jetSource  = cms.InputTag("ktPu4CaloJets"),
                                          
                                             jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ktPu4corr")))
process.ktPu4patSequence = cms.Sequence(process.ktPu4CaloJets*process.ktPu4corr*process.ktPu4patJets)


process.ktPu6corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ktPu6CaloJets"),
                                                    corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_KT6Calo"),
                                                                          L3Absolute = cms.string("HI_L3Absolute_KT6Calo"),
                                                                          L5Flavor = cms.string("none")))
process.ktPu6patJets = process.patJets.clone(jetSource  = cms.InputTag("ktPu6CaloJets"),
                                             jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ktPu6corr")))
process.ktPu6patSequence = cms.Sequence(process.ktPu6CaloJets*process.ktPu6corr*process.ktPu6patJets)

process.icPu5corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("iterativeConePu5CaloJets"),
                                                    corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_IC5Calo"),
                                                                          L3Absolute = cms.string("HI_L3Absolute_IC5Calo"),
                                                                          L5Flavor = cms.string("none")))
process.icPu5patJets = process.patJets.clone(jetSource  = cms.InputTag("iterativeConePu5CaloJets"),
                                             jetCorrFactorsSource = cms.VInputTag(cms.InputTag("icPu5corr")))
process.icPu5patSequence = cms.Sequence(process.iterativeConePu5CaloJets*process.icPu5corr*process.icPu5patJets)


#begin small cones

process.ak3CaloJets = process.ak5CaloJets.clone()
process.ak3CaloJets.rParam       = cms.double(0.3)
process.ak3CaloJets.radiusPU = 0.3



process.ak3sub = process.heavyIonL1SubtractedJets.clone()
process.ak3sub.src = 'ak3CaloJets'
process.ak3sub.rhoTag = 'kt4CaloJets'


process.ak3corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ak3sub"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_AK3Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_AK3Calo"),
                                                                        L5Flavor = cms.string("none")))
process.ak3patJets = process.patJets.clone(jetSource  = cms.InputTag("ak3sub"),
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ak3corr")))
process.ak3patSequence = cms.Sequence(process.ak3CaloJets*process.ak3sub*process.ak3corr*process.ak3patJets)

process.ak4CaloJets = process.ak5CaloJets.clone()
process.ak4CaloJets.rParam       = cms.double(0.4)
process.ak4CaloJets.radiusPU = 0.4


process.ak4sub = process.heavyIonL1SubtractedJets.clone()
process.ak4sub.src = 'ak4CaloJets'
process.ak4sub.rhoTag = 'kt4CaloJets'


process.ak4corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ak4sub"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_AK4Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_AK4Calo"),
                                                                        L5Flavor = cms.string("none")))
process.ak4patJets = process.patJets.clone(jetSource  = cms.InputTag("ak4sub"),
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ak4corr")))
process.ak4patSequence = cms.Sequence(process.ak4CaloJets*process.ak4sub*process.ak4corr*process.ak4patJets)

process.kt3CaloJets = process.kt4CaloJets.clone()
process.kt3CaloJets.rParam       = cms.double(0.3)
process.kt3CaloJets.radiusPU = 0.3


process.kt3sub = process.heavyIonL1SubtractedJets.clone()
process.kt3sub.src = 'kt3CaloJets'
process.kt3sub.rhoTag = 'kt4CaloJets'

process.kt3corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("kt3sub"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_KT3Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_KT3Calo"),
                                                                        L5Flavor = cms.string("none")))
process.kt3patJets = process.patJets.clone(jetSource  = cms.InputTag("kt3sub"),
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("kt3corr")))
process.kt3patSequence = cms.Sequence(process.kt3CaloJets*process.kt3sub*process.kt3corr*process.kt3patJets)

process.ic3CaloJets = process.ic5CaloJets.clone()
process.ic3CaloJets.rParam       = cms.double(0.3)
process.ic3CaloJets.radiusPU = 0.3



process.ic3sub = process.heavyIonL1SubtractedJets.clone()
process.ic3sub.src = 'ic3CaloJets'
process.ic3sub.rhoTag = 'kt4CaloJets'

process.ic3corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ic3sub"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_IC3Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_IC3Calo"),
                                                                        L5Flavor = cms.string("none")))
process.ic3patJets = process.patJets.clone(jetSource  = cms.InputTag("ic3sub"),
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ic3corr")))
process.ic3patSequence = cms.Sequence(process.ic3CaloJets*process.ic3sub*process.ic3corr*process.ic3patJets)


process.ic4CaloJets = process.ic5CaloJets.clone()
process.ic4CaloJets.rParam       = cms.double(0.4)
process.ic4CaloJets.radiusPU = 0.4


process.ic4sub = process.heavyIonL1SubtractedJets.clone()
process.ic4sub.src = 'ic4CaloJets'
process.ic4sub.rhoTag = 'kt4CaloJets'


process.ic4corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ic4sub"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_IC4Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_IC4Calo"),
                                                                        L5Flavor = cms.string("none")))

process.ic4patJets = process.patJets.clone(jetSource  = cms.InputTag("ic4sub"),
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ic4corr")))
process.ic4patSequence = cms.Sequence(process.ic4CaloJets*process.ic4sub*process.ic4corr*process.ic4patJets)




# Run kT4 first if you want to use the rho for other algorithms
process.runAllJets = cms.Sequence(    
    process.kt4patSequence +
    process.ic5patSequence +
    process.kt6patSequence +
    process.ak5patSequence +
    process.ak7patSequence +
    process.icPu5patSequence +
    process.ak3patSequence +
    process.ak4patSequence +
    process.kt3patSequence +
    process.ic3patSequence +
    process.ic4patSequence 
)



process.load("HeavyIonsAnalysis.Configuration.HI_DiJetSkim_cff")
process.hltJetHI.HLTPaths = ["HLT_HIMinBiasHfOrBSC_Core"]

process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")

process.jetSkimPath = cms.Path(
    process.hltJetHI*
    process.collisionEventSelection
    )


#process.load("RecoHI.HiEgammaAlgos.hiEcalSpikeFilter_cfi")

process.path = cms.Path(
#    process.hiEcalSpikeFilter*
    process.hltJetHI*
    process.collisionEventSelection*
    process.hiExtra*
    process.runAllJets
    )

process.load("HeavyIonsAnalysis.Configuration.analysisEventContent_cff")




process.output = cms.OutputModule("PoolOutputModule",
                                  process.jetTrkSkimContent,
                                  SelectEvents = cms.untracked.PSet(
                                      SelectEvents = cms.vstring('jetSkimPath')
                                    ),
                                  fileName = cms.untracked.string("PAT.root"),
                                  dataset = cms.untracked.PSet(
                                      filterName = cms.untracked.string('jetSkimPath'),
                                  dataTier = cms.untracked.string('PAT')
                                  )
                                )


# Save some extra stuff
#?
process.output.outputCommands.extend(["keep *_heavyIon_*_*"])
# triggger
process.output.outputCommands.extend(["keep *_TriggerResults_*_*"])
#tracks
#process.output.outputCommands.extend(["keep *_hiSelectedTracks_*_*"])
#process.output.outputCommands.extend(["keep *_hiGlobalPrimTracks_*_*"])
# reco jets
process.output.outputCommands.extend(["keep recoCaloJets_*_*_*"])
process.output.outputCommands.extend(["keep recoPFJets_*_*_*"])
#calorimeter stuff
process.output.outputCommands.extend(["keep *_towerMaker_*_*"])
process.output.outputCommands.extend(["keep *_caloTowers_*_*"])
process.output.outputCommands.extend(["keep *_hcalnoise_*_*"])
process.output.outputCommands.extend(["keep *_hbhereco_*_*"])
process.output.outputCommands.extend(["keep *_horeco_*_*"])
process.output.outputCommands.extend(["keep *_hfreco_*_*"])
process.output.outputCommands.extend(["keep *_ecalRecHit_*_*"])




# Save all RECO!
#process.output.outputCommands.extend(["keep *_*_*_RECO"])

process.out_step = cms.EndPath(process.output)

# And the logger
process.load("FWCore.MessageLogger.MessageLogger_cfi")
process.options = cms.untracked.PSet(
    makeTriggerResults = cms.untracked.bool(True),
    wantSummary = cms.untracked.bool(True),
    Rethrow = cms.untracked.vstring('Unknown', 
        'ProductNotFound', 
        'DictionaryNotFound', 
        'InsertFailure', 
        'Configuration', 
        'LogicError', 
        'UnimplementedFeature', 
        'InvalidReference', 
        'NullPointerError', 
        'NoProductSpecified', 
        'EventTimeout', 
        'EventCorruption', 
        'ModuleFailure', 
        'ScheduleExecutionFailure', 
        'EventProcessorFailure', 
        'FileInPathError', 
        'FatalRootError', 
        'NotFound')
)

process.MessageLogger.cerr.FwkReport.reportEvery = 100
