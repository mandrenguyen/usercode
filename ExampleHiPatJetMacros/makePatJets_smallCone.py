import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')
ivars.register('initialEvent',mult=ivars.multiplicity.singleton,info="for testing")

ivars.files = "dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/davidlw/Hydjet_Quenched_MinBias_2760GeV_STARTUP39V2_GEN_SIM_RAWDEBUG_390/Hydjet_Quenched_MinBias_2760GeV_STARTUP39V2_GEN_SIM_RECODEBUG_390/e91272bc2b91d5f6eefc8e1f2c112a67/hiReco_RAW2DIGI_RECO_9_1_klj.root"

ivars.output = 'pat.root'
ivars.maxEvents = -1
ivars.initialEvent = 1

ivars.parseArguments()

import FWCore.ParameterSet.Config as cms


process = cms.Process('HIJETS')

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(ivars.files)
                            )

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(ivars.maxEvents)
                    )

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'MC_39Y_V2::All'  #39X
process.GlobalTag.globaltag = 'START39_V2::All'  #39X

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')

process.load('PhysicsTools.HepMCCandAlgos.HiGenParticles_cfi')




# --- Pat Stuff --

# is this just setting defaults that we don't use anyway?  What does DisableMonteCarloDeps doing?  
process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)

#from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet( process, "Summer09_7TeV_ReReco332")

process.heavyIon


process.load("HeavyIonsAnalysis.Configuration.analysisProducers_cff")
process.hiExtra = cms.Sequence(
        process.allTracks +
            process.heavyIon
            )
# --- Gen stuff ---
process.load('PhysicsTools.HepMCCandAlgos.HiGenParticles_cfi')

process.load('RecoJets.Configuration.GenJetParticles_cff')
process.load('RecoHI.HiJetAlgos.HiGenJets_cff')

process.hiGen = cms.Sequence(
    # Re-run gen jets if narrow cones were not already run
    #Careful when using embedded samples, need to switch the name of gen particle collection
    process.hiGenParticles +
    process.hiGenParticlesForJets *
#    process.hiRecoGenJets +
    process.ak3HiGenJets +
    process.genPartons *
    process.hiPartons
    )



# Define Jet Algo parameters

process.load('RecoHI.HiJetAlgos.HiRecoJets_cff')

process.patJets.addGenPartonMatch   = True
process.patJets.addGenJetMatch      = True

process.ak3CaloJets = process.ak5CaloJets.clone()
process.ak3CaloJets.rParam       = cms.double(0.3)

# If you're using the calorimeter-based PU subtraction you'll also want to do something like
#process.icPu3CaloJets.radiusPU = 0.3

process.ak3corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ak3CaloJets"),
                                                  corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_AK3Calo"),
                                                                        L3Absolute = cms.string("HI_L3Absolute_AK3Calo"),
                                                                        L5Flavor = cms.string("none")))
process.ak3clean = process.heavyIonCleanedGenJets.clone(src = cms.InputTag('ak3HiGenJets'))
process.ak3match = process.patJetGenJetMatch.clone(src = cms.InputTag("ak3CaloJets"),
                                                   matched = cms.InputTag("ak3clean"))
process.ak3parton = process.patJetPartonMatch.clone(src = cms.InputTag("ak3CaloJets"))
process.ak3patJets = process.patJets.clone(jetSource  = cms.InputTag("ak3CaloJets"),
                                           genJetMatch = cms.InputTag("ak3match"),
                                           genPartonMatch= cms.InputTag("ak3parton"),                                       
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ak3corr")))
process.ak3patSequence = cms.Sequence(process.ak3CaloJets*process.ak3corr*process.ak3clean*process.ak3match*process.ak3parton*process.ak3patJets)




process.runAllJets = cms.Sequence(
    process.ak3patSequence 
)


# put it all together

process.path = cms.Path(process.hiExtra*
                        process.hiGen*
                        process.runAllJets
                        )

process.load("HeavyIonsAnalysis.Configuration.analysisEventContent_cff")

process.output = cms.OutputModule("PoolOutputModule",
                                  process.jetTrkSkimContentMC,
                                  fileName = cms.untracked.string(ivars.output)
                                  )

process.output.outputCommands.extend(["keep *_hiSelectedTracks_*_*"])
process.output.outputCommands.extend(["keep *_particleFlow_*_*"])
process.output.outputCommands.extend(["keep *_mergedtruth_*_*"])
process.output.outputCommands.extend(["keep double*_*PF*_*_*"])
process.output.outputCommands.extend(["keep *_heavyIon_*_*"])
process.output.outputCommands.extend(["keep *_generator_*_*"])
process.output.outputCommands.extend(["keep *_hiSignal_*_*"])
process.output.outputCommands.extend(["keep *_genParticles_*_*"])
process.output.outputCommands.extend(["keep *_hiGenParticles_*_*"])
process.output.outputCommands.extend(["keep *_TriggerResults_*_*"])


process.out_step = cms.EndPath(process.output)

