
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
'/store/relval/CMSSW_3_9_9/RelValHydjetQ_MinBias_2760GeV/GEN-SIM-RECO/START39_V7HI-v1/0000/40B4255D-5A3D-E011-9D80-001BFCDBD100.root'
)
)

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(2)
                    )

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'START39_V7::All'

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

process.load('RecoMET.Configuration.RecoPFMET_cff')

process.HiParticleFlowRecoNoJets = cms.Sequence(
    process.particleFlowCluster
    * process.trackerDrivenElectronSeeds
    * process.particleFlowReco
    *process.recoPFMET
    )
process.trackerDrivenElectronSeeds.TkColList = cms.VInputTag("hiGoodTracksKeepAll")



process.reReco = cms.Path(
    process.hiTrackReco*
    process.hiGoodTracksSelection*
    process.conformalPixelTrackReco *
    process.hiGoodMergedTracks *
    process.muonRecoPbPb *
    process.HiParticleFlowRecoNoJets 
    )



process.load('Configuration.EventContent.EventContent_cff')
process.load("Configuration.EventContent.EventContentHeavyIons_cff")        #load keep/drop output commands
process.output = cms.OutputModule("PoolOutputModule",
                                  process.RECODEBUGEventContent,
                                  fileName = cms.untracked.string("RERECODEBUG_PFMET.root"),
                                  dataset = cms.untracked.PSet(
    filterName = cms.untracked.string('RECODEBUG'),
    dataTier = cms.untracked.string('RECODEBUG')
    )
                                  )

process.output.outputCommands.extend(["keep *_hiGoodTracks_*_*"])
process.output.outputCommands.extend(["keep *_hiGoodMergedTracks_*_*"])
process.output.outputCommands.extend(["keep recoPFMETs_pfMet_*_*"])

process.out_step = cms.EndPath(process.output)




