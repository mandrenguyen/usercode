
import FWCore.ParameterSet.Config as cms


process = cms.Process('HIJETS')

#process.Timing = cms.Service("Timing")

#process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
#                                        ignoreTotal=cms.untracked.int32(0),
#                                        oncePerEventMode = cms.untracked.bool(False)
#                                        )

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    '/store/relval/CMSSW_3_9_9/RelValQCD_Pt_80_120/GEN-SIM-RECO/START39_V9-v1/0000/0622F65F-623D-E011-A38D-002618943949.root'
    #'/store/relval/CMSSW_3_9_9/RelValZmumuJets_Pt_20_300_GEN/GEN-SIM-RECO/MC_39Y_V8_PU_E7TeV_AVE_2_BX2808-v1/0000/10AD7A1E-FD3D-E011-AC76-0018F3D095EE.root'
    #'/store/relval/CMSSW_3_9_9/RelValSingleMuPt100/GEN-SIM-RECO/MC_39Y_V8-v1/0000/EEE53664-583D-E011-8433-0018F3D09692.root'
)
                            )

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(-1)
                    )

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START39_V7::All'

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("CommonTools.UtilAlgos.TFileService_cfi")

#JEC from DB

process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.jec = cms.ESSource("PoolDBESSource",
                           DBParameters = cms.PSet(
    messageLevel = cms.untracked.int32(0)
    ),
                           timetype = cms.string('runnumber'),
                           toGet = cms.VPSet(
    cms.PSet(record = cms.string("JetCorrectionsRecord"),
             tag = cms.string("JetCorrectorParametersCollection_HI_PFTowers_hiSelFix_AK3PF"),
             label = cms.untracked.string("AK3PF")
             ),
    cms.PSet(record = cms.string("JetCorrectionsRecord"),
             tag = cms.string("JetCorrectorParametersCollection_HI_PFTowers_hiSelFix_AK4PF"),
             label = cms.untracked.string("AK4PF")
             ),
    
    cms.PSet(record = cms.string("JetCorrectionsRecord"),
             tag = cms.string("JetCorrectorParametersCollection_HI_PFTowers_hiSelFix_AK5PF"),
             label = cms.untracked.string("AK5PF")
             ),
    ),
                           connect = cms.string("sqlite_file:JEC_HI_2011.db"),
                           
                           )


process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')


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

#from PhysicsTools.PatAlgos.tools.jetTools import *
#switchJECSet( process, "Summer09_7TeV_ReReco332")


#  --- Track and muon reconstruction ---
# pixel triplet tracking (HI Tracking)
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
process.load("RecoHI.Configuration.Reconstruction_HI_cff")
process.load("RecoHI.HiTracking.LowPtTracking_PbPb_cff")
# Needed to produce "HcalSeverityLevelComputerRcd" used by CaloTowersCreator/towerMakerPF
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
# keep all the tracks for muon reco, then set high purity flag
process.hiTracksWithLooseQualityKeepAll = process.hiTracksWithLooseQuality.clone()
process.hiTracksWithTightQualityKeepAll = process.hiTracksWithTightQuality.clone()
process.hiSelectedTracksKeepAll = process.hiSelectedTracks.clone()

process.hiTracksWithTightQualityKeepAll.src = cms.InputTag("hiTracksWithLooseQualityKeepAll")
process.hiSelectedTracksKeepAll.src = cms.InputTag("hiTracksWithTightQualityKeepAll")

process.hiTracksWithTightQuality.qualityBit = 'loose'
process.hiSelectedTracks.qualityBit = 'tight'

process.hiTracksWithTightQualityKeepAll.qualityBit = 'loose'
process.hiSelectedTracksKeepAll.qualityBit = 'tight'

process.hiTracksWithLooseQualityKeepAll.keepAllTracks = True
process.hiTracksWithTightQualityKeepAll.keepAllTracks = True
process.hiSelectedTracksKeepAll.keepAllTracks = True


process.heavyIonTracking += process.hiTracksWithLooseQualityKeepAll*process.hiTracksWithTightQualityKeepAll*process.hiSelectedTracksKeepAll


# Muon Reco
from RecoHI.HiMuonAlgos.HiRecoMuon_cff import * 
process.globalMuons.TrackerCollectionLabel = cms.InputTag("hiGoodTracksKeepAll")
muons.JetExtractorPSet.JetCollectionLabel = cms.InputTag("iterativeConePu5CaloJets")
#muons.JetExtractorPSet.JetCollectionLabel = cms.InputTag("iterativeCone5CaloJets")
muons.TrackExtractorPSet.inputTrackCollection = cms.InputTag("hiGoodTracksKeepAll")
muons.inputCollectionLabels = cms.VInputTag("hiGoodTracksKeepAll", "globalMuons", "standAloneMuons:UpdatedAtVtx")
process.muonRecoPbPb = muonRecoPbPb

#Track Reco
process.rechits = cms.Sequence(process.siPixelRecHits * process.siStripMatchedRecHits)
process.hiTrackReco = cms.Sequence(process.rechits * process.heavyIonTracking)

# for PF
process.load("RecoHI.Configuration.Reconstruction_hiPF_cff")

# good track selection
process.load("edwenger.HiTrkEffAnalyzer.TrackSelections_cff")
process.hiGoodTracksKeepAll = process.hiGoodTracks.clone()
process.hiGoodTracksKeepAll.keepAllTracks = True
process.hiGoodTracksSelection += process.hiGoodTracksKeepAll
# merge with pixel tracks
process.load('Appeltel.PixelTracksRun2010.HiLowPtPixelTracksFromReco_cff')
process.load('Appeltel.PixelTracksRun2010.HiMultipleMergedTracks_cff')
process.hiGoodMergedTracks.src = cms.InputTag("hiGoodTracks")

# particle-flow stuff

process.HiParticleFlowRecoNoJets = cms.Sequence(
    process.particleFlowCluster
    * process.trackerDrivenElectronSeeds
    * process.particleFlowReco
    )
process.trackerDrivenElectronSeeds.TkColList = cms.VInputTag("hiGoodTracksKeepAll")


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
process.load('RecoHI.HiJetAlgos.HiRecoPFJets_cff')

process.patJets.addGenPartonMatch   = True
process.patJets.addGenJetMatch      = True


process.patJets.embedCaloTowers = cms.bool(False)


process.iterativeConePu5CaloJets.jetPtMin = 10.0
process.ak5PFJets.jetPtMin = 10.0


process.load("RecoHI.HiJetAlgos.ParticleTowerProducer_cff")
process.PFTowers = process.particleTowerProducer.clone()



process.iterativeConePu5CaloJets.doPVCorrection = cms.bool(True)
process.iterativeConePu5CaloJets.srcPVs = 'hiSelectedVertex'
process.iterativeConePu5CaloJets.doPUOffsetCorr = False


#Vanilla algo, but PU turned off

process.icPu5corr = process.patJetCorrFactors.clone(src = cms.InputTag("iterativeConePu5CaloJets"),
                                                    levels = cms.vstring('L2Relative','L3Absolute'),
                                                    payload = cms.string('IC5Calo')
                                                    )
process.icPu5clean = process.heavyIonCleanedGenJets.clone(src = cms.InputTag('iterativeCone5HiGenJets'))
process.icPu5match = process.patJetGenJetMatch.clone(src = cms.InputTag("iterativeConePu5CaloJets"),
                                                     matched = cms.InputTag("icPu5clean"))
process.icPu5parton = process.patJetPartonMatch.clone(src = cms.InputTag("iterativeConePu5CaloJets"))
process.icPu5patJets = process.patJets.clone(jetSource  = cms.InputTag("iterativeConePu5CaloJets"),
                                             genJetMatch = cms.InputTag("icPu5match"),
                                             genPartonMatch= cms.InputTag("icPu5parton"),
                                             jetCorrFactorsSource = cms.VInputTag(cms.InputTag("icPu5corr")))
process.icPu5patSequence = cms.Sequence(process.iterativeConePu5CaloJets*process.icPu5corr*process.icPu5clean*process.icPu5match*process.icPu5parton*process.icPu5patJets)


### AK5, PF  ###

process.ak5PFJets.src = 'PFTowers'
process.ak5PFJets.jetType = 'BasicJet'

process.ak5PFcorr = process.patJetCorrFactors.clone(src = cms.InputTag("ak5PFJets"),
                                                    levels = cms.vstring('L2Relative','L3Absolute'),
                                                    payload = cms.string('AK5PF')
                                                    )
process.ak5PFclean = process.heavyIonCleanedGenJets.clone(src = cms.InputTag('ak5HiGenJets'))
process.ak5PFmatch = process.patJetGenJetMatch.clone(src = cms.InputTag("ak5PFJets"),
                                                     matched = cms.InputTag("ak5PFclean"))
process.ak5PFparton = process.patJetPartonMatch.clone(src = cms.InputTag("ak5PFJets"))
process.ak5PFpatJets = process.patJets.clone(jetSource  = cms.InputTag("ak5PFJets"),
                                             genJetMatch = cms.InputTag("ak5PFmatch"),
                                             genPartonMatch= cms.InputTag("ak5PFparton"),
                                             jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ak5PFcorr")))
process.ak5PFpatSequence = cms.Sequence(process.ak5PFJets*process.ak5PFcorr*process.ak5PFclean*process.ak5PFmatch*process.ak5PFparton*process.ak5PFpatJets)




#begin small cones

process.ak3PFJets = process.ak5PFJets.clone()
process.ak3PFJets.rParam       = cms.double(0.3)

process.ak3PFcorr = process.patJetCorrFactors.clone(src = cms.InputTag("ak3PFJets"),
                                                    levels = cms.vstring('L2Relative','L3Absolute'),
                                                    payload = cms.string('AK3PF')
                                                    )
process.ak3PFclean = process.heavyIonCleanedGenJets.clone(src = cms.InputTag('ak3HiGenJets'))
process.ak3PFmatch = process.patJetGenJetMatch.clone(src = cms.InputTag("ak3PFJets"),
                                                   matched = cms.InputTag("ak3PFclean"))
process.ak3PFparton = process.patJetPartonMatch.clone(src = cms.InputTag("ak3PFJets"))
process.ak3PFpatJets = process.patJets.clone(jetSource  = cms.InputTag("ak3PFJets"),
                                           genJetMatch = cms.InputTag("ak3PFmatch"),
                                           genPartonMatch= cms.InputTag("ak3PFparton"),                                       
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ak3PFcorr")))
process.ak3PFpatSequence = cms.Sequence(process.ak3PFJets*process.ak3PFcorr*process.ak3PFclean*process.ak3PFmatch*process.ak3PFparton*process.ak3PFpatJets)

process.ak4PFJets = process.ak5PFJets.clone()
process.ak4PFJets.rParam       = cms.double(0.4)

process.ak4PFcorr = process.patJetCorrFactors.clone(src = cms.InputTag("ak4PFJets"),
                                                    levels = cms.vstring('L2Relative','L3Absolute'),
                                                    payload = cms.string('AK4PF')
                                                    )
process.ak4PFclean = process.heavyIonCleanedGenJets.clone(src = cms.InputTag('ak4HiGenJets'))
process.ak4PFmatch = process.patJetGenJetMatch.clone(src = cms.InputTag("ak4PFJets"),
                                                   matched = cms.InputTag("ak4PFclean"))
process.ak4PFparton = process.patJetPartonMatch.clone(src = cms.InputTag("ak4PFJets"))
process.ak4PFpatJets = process.patJets.clone(jetSource  = cms.InputTag("ak4PFJets"),
                                           genJetMatch = cms.InputTag("ak4PFmatch"),
                                           genPartonMatch= cms.InputTag("ak4PFparton"),                                       
                                           jetCorrFactorsSource = cms.VInputTag(cms.InputTag("ak4PFcorr")))
process.ak4PFpatSequence = cms.Sequence(process.ak4PFJets*process.ak4PFcorr*process.ak4PFclean*process.ak4PFmatch*process.ak4PFparton*process.ak4PFpatJets)





process.runAllJets = cms.Sequence(
    process.icPu5patSequence +
    process.ak5PFpatSequence +
    process.ak3PFpatSequence +
    process.ak4PFpatSequence 
)

process.load("MNguyen.InclusiveJetAnalyzer.inclusiveJetAnalyzer_ppMC_cff")
process.inclusiveJetAnalyzer.useCentrality = cms.untracked.bool(False)

process.inclusiveJetAnalyzerSequence = cms.Sequence(
    process.inclusiveJetAnalyzer
    *process.ak3PFJetAnalyzer
    *process.ak4PFJetAnalyzer
    *process.ak5PFJetAnalyzer
    )

process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.load("MNguyen.InclusiveJetAnalyzer.PFJetAnalyzer_ppMC_cff")
process.PFJetAnalyzer.trackTag  = cms.InputTag("hiGoodMergedTracks")
process.PFJetAnalyzer.hasSimInfo = cms.untracked.bool(True)
process.PFJetAnalyzer.writeGenParticles = cms.untracked.bool(True)
process.PFJetAnalyzer.jetTag2 = cms.InputTag("ak5PFpatJets")
process.PFJetAnalyzer.recoJetTag2 = cms.InputTag("ak5PFJets")
process.PFJetAnalyzer.jetTag3 = cms.InputTag("ak4PFpatJets")
process.PFJetAnalyzer.recoJetTag3 = cms.InputTag("ak4PFJets")
process.PFJetAnalyzer.jetTag4 = cms.InputTag("ak3PFpatJets")
process.PFJetAnalyzer.recoJetTag4 = cms.InputTag("ak3PFJets")


#Frank's analyzer
#import to avoid conflicts with inclusiveJetAnalyzer
from Saved.QM11Ana.Analyzers_cff import trkAnalyzer
from Saved.QM11Ana.Analyzers_cff import genpAnalyzer

from Saved.QM11Ana.Analyzers_cff import cutsTPForFak
from Saved.QM11Ana.Analyzers_cff import cutsTPForEff
from Saved.QM11Ana.Analyzers_cff import hitrkEffAnalyzer_akpu3pf_j1
from Saved.QM11Ana.Analyzers_cff import hitrkEffAnalyzer_akpu3pf_j2
from Saved.QM11Ana.Analyzers_cff import hitrkEffAnalyzer_akpu3pf

from Saved.QM11Ana.Analyzers_cff import cutsTPForFakPxl
from Saved.QM11Ana.Analyzers_cff import cutsTPForEffPxl
from Saved.QM11Ana.Analyzers_cff import hipixtrkEffAnalyzer_akpu3pf_j1
from Saved.QM11Ana.Analyzers_cff import hipixtrkEffAnalyzer_akpu3pf_j2
from Saved.QM11Ana.Analyzers_cff import hipixtrkEffAnalyzer_akpu3pf



process.trkAnalyzer = trkAnalyzer
process.cutsTPForFak = cutsTPForFak
process.cutsTPForEff = cutsTPForEff
process.hitrkEffAnalyzer_akpu3pf_j1 = hitrkEffAnalyzer_akpu3pf_j1
process.hitrkEffAnalyzer_akpu3pf_j2 = hitrkEffAnalyzer_akpu3pf_j2
process.hitrkEffAnalyzer_akpu3pf = hitrkEffAnalyzer_akpu3pf 
process.cutsTPForFakPxl = cutsTPForFakPxl
process.cutsTPForEffPxl = cutsTPForEffPxl
process.hipixtrkEffAnalyzer_akpu3pf_j1 = hipixtrkEffAnalyzer_akpu3pf_j1
process.hipixtrkEffAnalyzer_akpu3pf_j2 = hipixtrkEffAnalyzer_akpu3pf_j2
process.hipixtrkEffAnalyzer_akpu3pf = hipixtrkEffAnalyzer_akpu3pf 
process.genpAnalyzer = genpAnalyzer

process.trkAnalyzer.trackSrc = cms.InputTag("hiGoodMergedTracks")
process.trkAnalyzer.trackPtMin = 0.5
process.genpAnalyzer.ptMin = 0.5



process.hitrkEffAna_akpu3pf = cms.Sequence(process.cutsTPForFak*process.cutsTPForEff*process.hitrkEffAnalyzer_akpu3pf*process.hitrkEffAnalyzer_akpu3pf_j1*process.hitrkEffAnalyzer_akpu3pf_j2)
process.hipixtrkEffAna_akpu3pf = cms.Sequence(process.cutsTPForFakPxl*process.cutsTPForEffPxl*process.hipixtrkEffAnalyzer_akpu3pf*process.hipixtrkEffAnalyzer_akpu3pf_j1*process.hipixtrkEffAnalyzer_akpu3pf_j2)


process.franksAnalyzers = cms.Sequence(process.trkAnalyzer*process.hitrkEffAna_akpu3pf*process.hipixtrkEffAna_akpu3pf*process.genpAnalyzer)


#for tree output
process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string("JetAnalysisTTrees_hiGoodTracks_v1.root"))


# put it all together

process.jetReco = cms.Path(
    process.hiTrackReco*
    process.hiGoodTracksSelection*
    process.conformalPixelTrackReco *
    process.hiGoodMergedTracks *
    process.muonRecoPbPb *
    process.HiParticleFlowRecoNoJets *
    process.hiExtra*
    process.hiGen*
    process.PFTowers*
    process.runAllJets
    )
process.jetAna = cms.Path(
    process.franksAnalyzers*
    process.PFJetAnalyzerSequence
    )

process.schedule = cms.Schedule(process.jetReco, process.jetAna)

#process.load("HeavyIonsAnalysis.Configuration.analysisEventContent_cff")
#
#process.output = cms.OutputModule("PoolOutputModule",
#                                  process.jetTrkSkimContentMC,
#                                  fileName = cms.untracked.string("/tmp/mnguyen/PAT.root")
#                                  )
#
#process.output.outputCommands.extend(["keep *_hiGoodTracks_*_*"])
##process.output.outputCommands.extend(["keep *_hiGoodMergedTracks_*_*"])
#process.output.outputCommands.extend(["keep *_particleFlow_*_*"])
#process.output.outputCommands.extend(["keep *_mergedtruth_*_*"])
#process.output.outputCommands.extend(["keep double*_*PF*_*_*"])
#process.output.outputCommands.extend(["keep *_heavyIon_*_*"])
#process.output.outputCommands.extend(["keep *_generator_*_*"])
#process.output.outputCommands.extend(["keep *_hiSignal_*_*"])
#process.output.outputCommands.extend(["keep *_genParticles_*_*"])
#process.output.outputCommands.extend(["keep *_hiGenParticles_*_*"])
#process.output.outputCommands.extend(["keep *_TriggerResults_*_*"])
#process.output.outputCommands.extend(["keep *_heavyIon_*_*"])
## reco jets
#process.output.outputCommands.extend(["keep recoCaloJets_*_*_*"])
#process.output.outputCommands.extend(["keep recoPFJets_*_*_*"])
##particle flow
#process.output.outputCommands.extend(["keep recoPFClusters_*_*_*"])
#process.output.outputCommands.extend(["keep recoPFRecHits_*_*_*"])
##fast jet pf stuff
#process.output.outputCommands.extend(["keep doubles_*PF*_*_*"])
##calorimeter stuff
#process.output.outputCommands.extend(["keep *_towerMaker_*_*"])
#process.output.outputCommands.extend(["keep *_PFTowers_*_*"])
#process.output.outputCommands.extend(["keep *_caloTowers_*_*"])
#process.output.outputCommands.extend(["keep *_hcalnoise_*_*"])
#process.output.outputCommands.extend(["keep *_hbhereco_*_*"])
#process.output.outputCommands.extend(["keep *_horeco_*_*"])
#process.output.outputCommands.extend(["keep *_hfreco_*_*"])
#process.output.outputCommands.extend(["keep *_ecalRecHit_*_*"])
#
#process.output.outputCommands.extend(["keep *_muons_*_*"])
#
#
#process.out_step = cms.EndPath(process.output)



