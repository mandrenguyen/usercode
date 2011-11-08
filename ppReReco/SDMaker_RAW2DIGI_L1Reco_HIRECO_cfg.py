# Auto generated configuration file
# using: 
# Revision: 1.341 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: step3 --conditions auto:com10 --scenario HeavyIons --process RECO --data --eventcontent RECO -s RAW2DIGI,L1Reco,RECO --datatier RECO --magField AutoFromDBCurrent --filein file:/data_CMS/cms/lamia/PPAllPhysics2760GeV_Raw/0406CA0E-2958-E011-83AA-0030487CD76A.root --fileout file:step3.root -n 100
import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
    fileNames = cms.untracked.vstring('file:/data_CMS/cms/lamia/PPAllPhysics2760GeV_Raw/0406CA0E-2958-E011-83AA-0030487CD76A.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('step3 nevts:100'),
    name = cms.untracked.string('PyReleaseValidation')
)


process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
### JetHI SD
process.hltJetHI = process.hltHighLevel.clone(HLTPaths = ['HLT_Jet40_v1','HLT_Jet60_v1'])
process.filterSdJetHI = cms.Path(process.hltJetHI)

### MuHI SD
process.hltMuHI = process.hltHighLevel.clone(
      HLTPaths = ["HLT_L1SingleMuOpen_v1","HLT_Mu0_v3","HLT_Mu3_v3",
                                "HLT_Mu5_v3","HLT_L1DoubleMu0_v1","HLT_L2DoubleMu0_v2",
                                "HLT_DoubleMu3_v3"],
        throw = False,
        andOr = True)
process.filterSdMuHI = cms.Path(process.hltMuHI)


### MuHI SD
process.hltPhotonHI = process.hltHighLevel.clone(
      HLTPaths = ["HLT_Photon15_CaloIdVL_v1"],
        throw = False,
        andOr = True)
process.filterSdPhotonHI = cms.Path(process.hltPhotonHI)



# Output definition

#process.RECOoutput = cms.OutputModule("PoolOutputModule",
#    splitLevel = cms.untracked.int32(0),
#    eventAutoFlushCompressedSize = cms.untracked.int32(5242880),
#    outputCommands = process.RECOEventContent.outputCommands,
#    fileName = cms.untracked.string('file:step3.root'),                                      
#    dataset = cms.untracked.PSet(
#        filterName = cms.untracked.string(''),
#        dataTier = cms.untracked.string('RECO')
#    )
#)


# customization for 441, but included in 442:
#process.hiSelectedTracks.keepAllTracks = True
#process.globalMuons.TrackerCollectionLabel = cms.InputTag("hiSelectedTracks")
#process.muons.TrackExtractorPSet.inputTrackCollection = cms.InputTag("hiSelectedTracks")
#process.muons.inputCollectionLabels = cms.VInputTag("hiSelectedTracks", "globalMuons", "standAloneMuons:UpdatedAtVtx")
#process.muons.inputCollectionLabels = cms.VInputTag("hiSelectedTracks", "globalMuons", "standAloneMuons:UpdatedAtVtx", "tevMuons:firstHit", "tevMuons:picky","tevMuons:dyt")

process.RECOEventContent.outputCommands.extend(["keep *_ecalDrivenElectronSeeds_*_*",
                                          "keep *_ecalDrivenGsfElectrons_*_*",
                                          "keep recoPFCandidates_*_*_*",
                                          "drop recoPFBlocks_*_*_*",
                                          "drop recoPFClusters_*_*_*",
                                          "drop recoPFRecHits_*_*_*",                                         
                                          "drop *_hiGlobalPrimTracks_*_*"])



### JetHI SD
process.outputSdJetHI = cms.OutputModule("PoolOutputModule",
                                         SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('filterSdJetHI')),
                                         dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('RECO'),
    filterName = cms.untracked.string('SD_JetHI')),
                                         outputCommands = process.RECOEventContent.outputCommands,
                                         fileName = cms.untracked.string('file:/data_CMS/cms/mnguyen/SD_pp_JetHI.root')
                                         )
### MuHI SD
process.outputSdMuHI = cms.OutputModule("PoolOutputModule",
                                        SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('filterSdMuHI')),
                                        dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('RECO'),
    filterName = cms.untracked.string('SD_MuHI')),
                                         outputCommands = process.RECOEventContent.outputCommands,
                                        fileName = cms.untracked.string('file:/data_CMS/cms/mnguyen/SD_pp_MuHI.root')
                                        )
### PhotonHI SD
process.outputSdPhotonHI = cms.OutputModule("PoolOutputModule",
                                            SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('filterSdPhotonHI')),
                                            dataset = cms.untracked.PSet(
    dataTier = cms.untracked.string('RECO'),
    filterName = cms.untracked.string('SD_PhotonHI')),
                                            outputCommands = process.RECOEventContent.outputCommands,
                                            fileName = cms.untracked.string('file:/data_CMS/cms/mnguyen/SD_pp_PhotonHI.root')
                                            )



process.RECOoutput_step = cms.EndPath(
        process.outputSdJetHI+
        process.outputSdMuHI+
        process.outputSdPhotonHI
        )



# Additional output definition

# Other statements
process.GlobalTag.globaltag = 'GR_R_44_V7::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
process.reconstruction_step = cms.Path(process.reconstructionHeavyIons_withPF)
process.endjob_step = cms.EndPath(process.endOfProcess)
#process.RECOoutput_step = cms.EndPath(process.RECOoutput)

# Schedule definition
process.schedule = cms.Schedule(process.filterSdJetHI,process.filterSdMuHI,process.filterSdPhotonHI,process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)

