import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')
ivars.register('initialEvent',mult=ivars.multiplicity.singleton,info="for testing")

#ivars.files = '/store/user/davidlw/Hydjet_Bass_MinBias_2760GeV/Pyquen_UnquenchedDiJet_Pt50_GEN-SIM-RECO_393_set2/fae6fe9048513d9ac8f476dd10ba6ba7/hiReco_RAW2DIGI_RECO_110_1_fz8.root'
#ivars.files = '/store/user/davidlw/Pyquen_UnquenchedDiJet_Pt50_START39V7HI_GEN_SIM_RAW_RECO_393_v1/Pyquen_UnquenchedDiJet_Pt50_START39V7HI_GEN_SIM_RAW_RECO_393_v1/83851f0cee3ec040350e447a83642bc9/Pyquen_UnquenchedDiJet_Pt50_cfi_py_GEN_SIM_DIGI_L1_DIGI2RAW_HLT_RAW2DIGI_RECO_99_1_PtA.root'
ivars.files = '/store/relval/CMSSW_4_3_0/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/STARTHI43_V1-v1/0125/8C6E49D1-11A1-E011-B2A6-003048678F8E.root'
ivars.output = 'test.root'
ivars.maxEvents = -1
ivars.initialEvent = 1

ivars.parseArguments()

import FWCore.ParameterSet.Config as cms

process = cms.Process('ITER')

#process.Timing = cms.Service("Timing")

#process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
#                                        ignoreTotal=cms.untracked.int32(0),
#                                        oncePerEventMode = cms.untracked.bool(False)
#                                        )

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    ivars.files
    )
                            )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )


#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START43_V4::All'

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("CommonTools.UtilAlgos.TFileService_cfi")


process.HeavyIonGlobalParameters = cms.PSet(
           centralityVariable = cms.string("HFhits"),
           nonDefaultGlauberModel = cms.string("Hydjet_Bass"),
           centralitySrc = cms.InputTag("hiCentrality")
           )

from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)




#  --- Track and muon reconstruction ---
# pixel triplet tracking (HI Tracking)
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
process.load("RecoHI.Configuration.Reconstruction_HI_cff")
process.load("RecoHI.HiTracking.LowPtTracking_PbPb_cff")
# Needed to produce "HcalSeverityLevelComputerRcd" used by CaloTowersCreator/towerMakerPF
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")

#necessary?
process.hiPixelAdaptiveVertex.useBeamConstraint = cms.bool(False)

#Track Reco
process.rechits = cms.Sequence(process.siPixelRecHits * process.siStripMatchedRecHits)
process.hiTrackReco = cms.Sequence(process.rechits * process.heavyIonTracking)


# good track selection w/ annoying hiPostGlobalPrimTracks step removed
process.load("MNguyen.iterTracking.TrackSelections_cff")

process.hiGoodTightTracks.keepAllTracks = False


process.iterativeConePu5CaloJets.doPVCorrection = cms.bool(True)
process.iterativeConePu5CaloJets.srcPVs = 'hiSelectedVertex'
process.iterativeConePu5CaloJets.doPUOffsetCorr = False


#PAT jets

process.load('PhysicsTools.PatAlgos.patHeavyIonSequences_cff')
from PhysicsTools.PatAlgos.tools.heavyIonTools import *

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


#Vanilla jet algo, needed for efficiency analyzer

process.iterativeConePu5CaloJets.doPVCorrection = cms.bool(True)
process.iterativeConePu5CaloJets.srcPVs = 'hiSelectedVertex'
#process.iterativeConePu5CaloJets.doPUOffsetCorr = False

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



#simulated particle stuff
#process.load("SimGeneral.TrackingAnalysis.HiTrackingParticles_cff")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")

#process.load("PbPbTrackingTools.HiTrackValidator.hitrackvalidator_cfi")
#process.hitrkvalidator.trklabel=cms.untracked.InputTag("hiGoodTracks")
#
#
#process.hitrkvalidator.neededCentBins = [0,1,3,11,19,35]
#
#process.hiGoodTightTrkVal = process.hitrkvalidator.clone(trklabel=cms.untracked.InputTag("hiGoodTightTracks"))
#process.hiGlobalPrimTrkVal = process.hitrkvalidator.clone(trklabel=cms.untracked.InputTag("hiGlobalPrimTracks"))
#process.hiScndGoodTightTrkVal = process.hitrkvalidator.clone(trklabel=cms.untracked.InputTag("hiScndGoodTightTracks"))
#process.hiThrdGoodTightTrkVal = process.hitrkvalidator.clone(trklabel=cms.untracked.InputTag("hiThrdGoodTightTracks"))
#process.hiGeneralTrkVal = process.hitrkvalidator.clone(trklabel=cms.untracked.InputTag("hiGeneralTracks"))
#
#
#process.hiCentrality.produceHFhits = cms.bool(True)
#process.hiCentrality.produceHFtowers = cms.bool(False)
#process.hiCentrality.produceEcalhits = cms.bool(False)
#process.hiCentrality.produceBasicClusters = cms.bool(False)
#process.hiCentrality.produceZDChits = cms.bool(False)
#process.hiCentrality.produceETmidRapidity = cms.bool(False)
#process.hiCentrality.producePixelhits = cms.bool(False)
#process.hiCentrality.produceTracks = cms.bool(False)
#process.hiCentrality.producePixelTracks = cms.bool(False)
#process.hiCentrality.doPixelCut = cms.bool(True)
#
#process.hiValidatorSequence = cms.Sequence(
#    process.hiCentrality*
#    process.hiGoodTightTrkVal*
#    process.hiGlobalPrimTrkVal*
#    process.hiScndGoodTightTrkVal*
#    process.hiThrdGoodTightTrkVal*
#    process.hiGeneralTrkVal
#)

#process.load('edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cff')

#from Saved.QM11Ana.Analyzers_cff import cutsTPForFak
#from Saved.QM11Ana.Analyzers_cff import cutsTPForEff
#from Saved.QM11Ana.Analyzers_cff import hitrkEffAnalyzer_akpu3pf
#
#
#process.hitrkEffAnalyzer_hiGoodTight = hitrkEffAnalyzer_akpu3pf
#process.hitrkEffAnalyzer_hiGoodTight.jets = 'icPu5patJets'
#process.hitrkEffAnalyzer_hiGoodTight.tracks = 'hiGoodTightTracks'
##process.hitrkEffAnalyzer_hiGoodTight.fillNtuples = True
#process.hitrkEffAnalyzer_hiGoodTight.useQaulityStr = cms.untracked.bool(False)
#
#
#process.hitrkEffAnalyzer_hiGlobalPrim = process.hitrkEffAnalyzer_hiGoodTight.clone(tracks = 'hiGlobalPrimTracks')
#process.hitrkEffAnalyzer_hiScndGoodTight = process.hitrkEffAnalyzer_hiGoodTight.clone(tracks = 'hiScndGoodTightTracks')
#process.hitrkEffAnalyzer_hiThrdGoodTight = process.hitrkEffAnalyzer_hiGoodTight.clone(tracks = 'hiThrdGoodTightTracks')
#process.hitrkEffAnalyzer_hiGeneralTracks = process.hitrkEffAnalyzer_hiGoodTight.clone(tracks = 'hiGeneralTracks')
#
#process.cutsTPForFak = cutsTPForFak
#process.cutsTPForEff = cutsTPForEff
#
#process.hitrkEffAna = cms.Sequence(process.cutsTPForFak*process.cutsTPForEff*
#                                   process.hitrkEffAnalyzer_hiGlobalPrim*
#                                   process.hitrkEffAnalyzer_hiGoodTight*
#                                   process.hitrkEffAnalyzer_hiScndGoodTight*
#                                   process.hitrkEffAnalyzer_hiThrdGoodTight*
#                                   process.hitrkEffAnalyzer_hiGeneralTracks
#                                   )
#



#for tree output
process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string(ivars.output)
                                   )


process.load("MNguyen.iterTracking.secondStep_triplet_cff")
process.load("MNguyen.iterTracking.thirdStep_pairs_cff")
process.load("MNguyen.iterTracking.MergeTrackCollectionsHI_cff")

# put it all together
process.trackRecoAndSelection = cms.Path(
    process.hiTrackReco*
    process.hiGoodTightTracks
    *process.secondStep
    *process.thirdStep
    *process.trackCollectionMerging
    )

process.jetReco = cms.Path(
    process.icPu5patSequence
)

#process.trackAna = cms.Path(
    #process.hiValidatorSequence*
#    process.hitrkEffAna
#    )

process.load('RecoHI.HiEgammaAlgos.HiElectronSequence_cff')

process.hiElectrons = cms.Path(
    process.hiElectronSequence
    )   

process.hiElectrons.remove(process.towerMakerPF)

process.schedule = cms.Schedule(process.trackRecoAndSelection, process.hiElectrons)
#process.schedule = cms.Schedule(process.trackRecoAndSelection)
#process.schedule = cms.Schedule(process.trackRecoAndSelection, process.jetReco, process.trackAna)

