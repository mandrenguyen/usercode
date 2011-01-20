import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')
ivars.register('initialEvent',mult=ivars.multiplicity.singleton,info="for testing")

#ivars.files = "/store/relval/CMSSW_3_9_3/RelValHydjetQ_B0_2760GeV/GEN-SIM-RECO/MC_39Y_V6-v2/0084/EAA23156-B0F4-DF11-A741-003048678B12.root"
ivars.files = "rfio:/castor/cern.ch/user/y/yilmaz/mix/MinBias_DijetUnquenched30_d20110118_Offset_110_RECO/Mixed_110_RECO_Mixed_110_zsDigisForDM_100_FE622F66-EBF0-DF11-8447-003048F118D4.root"

ivars.register('outputNtuple',
                                "Ntuple.root",
                                VarParsing.VarParsing.multiplicity.singleton,
                                VarParsing.VarParsing.varType.string,
                                "Ntuples for Jet Analysis")

ivars.outputNtuple = 'Ntuple.root'

ivars.output = 'JetTree.root'
ivars.maxEvents = 100
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
process.GlobalTag.globaltag = 'START39_V7HI::All'

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')


# --- Pat Stuff --

# is this just setting defaults that I don't use anyway?  What is DisableMonteCarloDeps doing?  
process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *

# This confusing script don't work no mo
#configureHeavyIons(process)

#let's do it by hand

process.patJets.jetSource  = cms.InputTag("iterativeConePu5CaloJets")


process.patJets.addBTagInfo         = False
process.patJets.addTagInfos         = False
process.patJets.addDiscriminators   = False
process.patJets.addAssociatedTracks = False
process.patJets.addJetCharge        = False
process.patJets.addJetID            = False
process.patJets.getJetMCFlavour     = False
process.patJets.addGenPartonMatch   = True
process.patJets.addGenJetMatch      = True
process.patJets.embedGenJetMatch    = True
process.patJets.embedGenPartonMatch   = True

process.patJetPartonMatch.matched = cms.InputTag("hiGenParticles")

process.load("HeavyIonsAnalysis.Configuration.analysisProducers_cff")
process.hiExtra = cms.Sequence(
            process.heavyIon
            )
# --- Gen stuff ---
# Note that we need to re-run gen jets b/c until AK-IC bug fix propagates to production
process.load('PhysicsTools.HepMCCandAlgos.HiGenParticles_cfi')

process.load('RecoJets.Configuration.GenJetParticles_cff')
process.load('RecoHI.HiJetAlgos.HiGenJets_cff')


#This runs GenHIEventProducer
process.heavyIon

#process.hiGenParticles.srcVector = cms.vstring('hiSignal')

process.hiGen = cms.Sequence(
#Careful when using embedded samples
    process.hiGenParticles +
    process.hiGenParticlesForJets *
    process.hiRecoGenJets +
    process.genPartons *
    process.hiPartons
    )



# Define Jet Algo parameters

process.load('RecoHI.HiJetAlgos.HiRecoJets_cff')


# set up vertex matching
process.iterativeConePu5CaloJets.doPVCorrection = cms.bool(True)
process.iterativeConePu5CaloJets.srcPVs = 'hiSelectedVertex'

process.icPu5corr = process.patJetCorrFactors.clone(jetSource = cms.InputTag("iterativeConePu5CaloJets"),
                                                    corrLevels = cms.PSet(L2Relative = cms.string("L2Relative_IC5Calo"),
                                                                          L3Absolute = cms.string("L3Absolute_IC5Calo"),
                                                                          L5Flavor = cms.string("none"),
                                                                          L7Parton = cms.string("none")                                                                                                                                                    )
                                                    )
process.icPu5clean = process.heavyIonCleanedGenJets.clone(src = cms.InputTag('iterativeCone5HiGenJets'))
process.icPu5match = process.patJetGenJetMatch.clone(src = cms.InputTag("iterativeConePu5CaloJets"),
                                                     matched = cms.InputTag("icPu5clean"))
process.icPu5parton = process.patJetPartonMatch.clone(src = cms.InputTag("iterativeConePu5CaloJets"))
process.icPu5patJets = process.patJets.clone(jetSource  = cms.InputTag("iterativeConePu5CaloJets"),
                                             genJetMatch = cms.InputTag("icPu5match"),
                                             genPartonMatch= cms.InputTag("icPu5parton"),
                                             jetCorrFactorsSource = cms.VInputTag(cms.InputTag("icPu5corr")))
#process.icPu5patSequence = cms.Sequence(process.iterativeConePu5CaloJets*process.icPu5corr*process.icPu5clean*process.icPu5match*process.icPu5parton*process.icPu5patJets)
process.icPu5patSequence = cms.Sequence(process.icPu5corr*process.icPu5clean*process.icPu5match*process.icPu5parton*process.icPu5patJets)



process.runAllJets = cms.Sequence(
    process.icPu5patSequence 
)

process.load("MNguyen.InclusiveJetAnalyzer.inclusiveJetAnalyzer_cff")
process.load("MNguyen.InclusiveJetAnalyzer.PFJetAnalyzer_cff")

process.inclusiveJetAnalyzer.useCentrality = cms.untracked.bool(False)
process.inclusiveJetAnalyzer.isMC = cms.untracked.bool(True) 

process.inclusiveJetAnalyzerSequence = cms.Sequence(
    process.inclusiveJetAnalyzer
    )


#for tree output
process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string(ivars.output))


# put it all together

process.path = cms.Path(
    process.hiExtra*
    process.hiGen*
    process.runAllJets
    *process.inclusiveJetAnalyzerSequence
    )


