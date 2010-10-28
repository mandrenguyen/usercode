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





# --- Pat Stuff --

# is this just setting defaults that we don't use anyway?  What does DisableMonteCarloDeps doing?  
process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *
configureHeavyIons(process)

#from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet( process, "Summer09_7TeV_ReReco332")

process.heavyIon

#  --- Track and muon reconstruction ---
process.load('Configuration/StandardSequences/ReconstructionHeavyIons_cff')

# Muon Reco
from RecoHI.HiMuonAlgos.HiRecoMuon_cff import * 
muons.JetExtractorPSet.JetCollectionLabel = cms.InputTag("iterativeConePu5CaloJets")

#Track Reco
process.rechits = cms.Sequence(process.siPixelRecHits * process.siStripMatchedRecHits)
process.hiTrackReco = cms.Sequence(process.rechits * process.heavyIonTracking * muonRecoPbPb)

# for PF
process.load("RecoHI.Configuration.Reconstruction_hiPF_cff")


# particle-flow stuff

process.HiParticleFlowRecoNoJets = cms.Sequence(
    process.particleFlowCluster
    * process.trackerDrivenElectronSeeds
    * process.particleFlowReco
    )

process.load("HeavyIonsAnalysis.Configuration.analysisProducers_cff")
process.hiExtra = cms.Sequence(
    process.allTracks +
    process.heavyIon
    )
# --- Gen stuff ---
process.load('PhysicsTools.HepMCCandAlgos.HiGenParticles_cfi')
process.load('RecoJets.Configuration.GenJetParticles_cff')
process.load('RecoHI.HiJetAlgos.HiGenJets_cff')

process.load('RecoJets.Configuration.GenJetParticles_cff')
process.load('RecoHI.HiJetAlgos.HiGenJets_cff')


process.hiGen = cms.Sequence(
    #Careful when using embedded samples, need to switch the name of gen particle collection
    #process.hiGenParticles +
    #process.hiGenParticlesForJets *
    #process.hiRecoGenJets +
    process.genPartons *
    process.hiPartons
    )



# Define Jet Algo parameters

process.load('RecoHI.HiJetAlgos.HiRecoPFJets_cff')

process.patJets.addGenPartonMatch   = True
process.patJets.addGenJetMatch      = True


process.ak5PFcorr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("ak5PFJets"),
                                                    corrLevels = cms.PSet(L2Relative = cms.string("HI_L2Relative_AK5PF"),
                                                                          L3Absolute = cms.string("HI_L3Absolute_AK5PF"),
                                                                          L5Flavor = cms.string("none")))
process.ak5PFclean = process.heavyIonCleanedGenJets.clone(src = cms.InputTag('ak5HiGenJets'))
process.ak5PFmatch = process.patJetGenJetMatch.clone(src = cms.InputTag("ak5PFJets"),
                                                     matched = cms.InputTag("ak5PFclean"))
process.ak5PFparton = process.patJetPartonMatch.clone(src = cms.InputTag("ak5PFJets"))
process.ak5PFpatJets = process.patJets.clone(jetSource  = cms.InputTag("ak5PFJets"),
                                             genJetMatch = cms.InputTag("ak5PFmatch"),
                                             genPartonMatch= cms.InputTag("ak5PFparton"),
                                             jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ak5PFcorr")))
process.ak5PFpatSequence = cms.Sequence(process.ak5PFJets*process.ak5PFcorr*process.ak5PFclean*process.ak5PFmatch*process.ak5PFparton*process.ak5PFpatJets)


process.runAllJets = cms.Sequence(
    process.ak5PFpatSequence 
)


# put it all together

process.path = cms.Path(process.hiTrackReco*
                        process.HiParticleFlowRecoNoJets*
                        process.hiExtra*
                        process.hiGen*
                        process.runAllJets
                        )

process.load("HeavyIonsAnalysis.Configuration.analysisEventContent_cff")

process.output = cms.OutputModule("PoolOutputModule",
                                  process.jetTrkSkimContentMC,
                                  fileName = cms.untracked.string(ivars.output)
                                  )

process.output.outputCommands.extend(["drop *_towerMkter_*_*"])
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

