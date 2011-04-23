import FWCore.ParameterSet.Config as cms

process = cms.Process('HIRERECO')


process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
                                "/store/data/Run2011A/AllPhysics2760/RECO/PromptReco-v2/000/161/439/FA05D306-855A-E011-8245-003048F118E0.root",
                                   '/store/data/Run2011A/AllPhysics2760/RECO/PromptReco-v2/000/161/450/ACCFA0A0-6A5A-E011-8CBB-0030487A18F2.root',
        '/store/data/Run2011A/AllPhysics2760/RECO/PromptReco-v2/000/161/450/AA4F1522-795A-E011-8DDF-0030486780A8.root',
        '/store/data/Run2011A/AllPhysics2760/RECO/PromptReco-v2/000/161/450/AA4A07CD-775A-E011-990E-001617C3B6E2.root'
    
    )
                            )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(100)
    )

#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.GlobalTag.globaltag = 'GR_P_V17::All'#GR_P_V14::All'

process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')


#  --- Track and muon reconstruction ---
# pixel triplet tracking (HI Tracking)
process.load("RecoLocalTracker.Configuration.RecoLocalTracker_cff")
process.load("RecoHI.Configuration.Reconstruction_HI_cff")
# Needed to produce "HcalSeverityLevelComputerRcd" used by CaloTowersCreator/towerMakerPF
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")

# Muon Reco
from RecoHI.HiMuonAlgos.HiRecoMuon_cff import * 
process.muonRecoPbPb = muonRecoPbPb

#Track Reco
process.rechits = cms.Sequence(process.siPixelRecHits * process.siStripMatchedRecHits)
process.hiTrackReco = cms.Sequence(process.rechits * process.heavyIonTracking)

# Load Heavy Ion "Good" Track Selection, merged with pixel tracks 
process.load("edwenger.HiTrkEffAnalyzer.TrackSelections_cff")
#process.load('Appeltel.PixelTracksRun2010.HiLowPtPixelTracksFromReco_cff')
#process.load('Appeltel.PixelTracksRun2010.HiMultipleMergedTracks_cff')

# for PF
process.load("RecoHI.Configuration.Reconstruction_hiPF_cff")


process.HiParticleFlowRecoNoJets = cms.Sequence(
    process.particleFlowCluster
    * process.pfTrack   
    * process.particleFlowReco
    )



# add the lines below also to  Reconstruction_hiPF
process.pfTrack.UseQuality = cms.bool(True)
process.pfTrack.TrackQuality = cms.string('highPurity')
process.pfTrack.TkColList = cms.VInputTag("hiGoodTracks")
process.pfTrack.GsfTracksInEvents = cms.bool(False)

process.particleFlowBlock.useNuclear = cms.bool(False)   ##Daniele: to be added because nuclear will be switched on. 

from RecoHI.HiMuonAlgos.HiRecoMuon_cff import *
# replace jet label
process.muons.JetExtractorPSet.JetCollectionLabel = cms.InputTag("iterativeCone5CaloJets")



process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
### JetHI SD
process.hltJetHI = process.hltHighLevel.clone(HLTPaths = ['HLT_Jet20_v1', 'HLT_Jet40_v1','HLT_Jet60_v1'])
#process.hltJetHI = process.hltHighLevel.clone(HLTPaths = [ 'HLT_Jet40_v1','HLT_Jet60_v1'])
process.filterSdJetHI = cms.Path(#process.bscOrHfCoinc *
                                 #process.collisionEventSelection *
                                 process.hltJetHI)
### MuHI SD
process.hltMuHI = process.hltHighLevel.clone(
  HLTPaths = ["HLT_L1SingleMuOpen_v1","HLT_Mu0_v3","HLT_Mu3_v3",
              "HLT_Mu5_v3","HLT_L1DoubleMu0_v1","HLT_L2DoubleMu0_v2",
              "HLT_DoubleMu3_v3"],
  throw = False,
  andOr = True)
process.filterSdMuHI = cms.Path(#process.bscOrHfCoinc *
                                #process.collisionEventSelection *
                                process.hltMuHI)



process.recoPath = cms.Path(
    process.hiTrackReco*
    process.hiGoodTracksSelection*
    #process.conformalPixelTrackReco *
    #process.hiGoodMergedTracks*
    process.muonRecoPbPb*
    process.HiParticleFlowRecoNoJets
    )



############ Output Modules ##########
process.load('Configuration.EventContent.EventContent_cff')
process.load("Configuration.EventContent.EventContentHeavyIons_cff")        #load keep

### JetHI SD
process.outputSdJetHI = cms.OutputModule("PoolOutputModule",
                                         SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('filterSdJetHI')),                               
                                         dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('RECO'),
    filterName = cms.untracked.string('SD_JetHI')),
                                         outputCommands = process.RECOEventContent.outputCommands,
                                         fileName = cms.untracked.string('/tmp/mnguyen/SD_pp_JetHI.root')
                                         )
### MuHI SD
process.outputSdMuHI = cms.OutputModule("PoolOutputModule",
                                        SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('filterSdMuHI')),                               
                                        dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('RECO'),
    filterName = cms.untracked.string('SD_MuHI')),
                                        outputCommands = cms.untracked.vstring(    'drop *'),
                                        fileName = cms.untracked.string('/tmp/mnguyen/SD_pp_MuHI.root')
                                        )


process.outputSdJetHI.outputCommands.extend(["keep *_hiGoodTracks_*_*"])
#process.outputSdJetHI.outputCommands.extend(["keep *_hiGoodMergedTracks_*_*"])

process.outputSdMuHI.outputCommands.extend(process.RECOEventContent.outputCommands)
process.outputSdMuHI.outputCommands.extend(["drop *_*_*_RECO"])
process.outputSdMuHI.outputCommands.extend(cms.untracked.vstring(
    'keep *_*_*_HLT',
    'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*',
    'keep *_hiSelectedVertex_*_*',
    'keep *_heavyIon_*_*',
    'keep *_hiCentrality_*_*',
    'keep *_hiEvtPlane_*_*',
    'keep *_offlineBeamSpot_*_*',
    'keep *_hiSelectedTracks_*_*',
    'keep *_towerMaker_*_*',
    'keep *_hiGlobalPrimTracks_*_*',
    'keep *_hiPixel3PrimTracks_*_*'))




process.out_step = cms.EndPath(
    process.outputSdJetHI      +
    process.outputSdMuHI       
)


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

process.MessageLogger.cerr.FwkReport.reportEvery = 1









