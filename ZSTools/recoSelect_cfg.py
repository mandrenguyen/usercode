import FWCore.ParameterSet.Config as cms

process = cms.Process('EVTSELECT')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')

process.load("HeavyIonsAnalysis.Configuration.collisionEventSelection_cff")


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(50)
)
process.options = cms.untracked.PSet(
)

################################################################################
# Input Variable Parsing
#

import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('python')

ivars.inputFiles = "dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/icali/HICorePhysics/HICorePhysics_Skims_RAW/b1ed71284bce538242f1c51a90564ef9/SD_Jet35HI_9_1_w90.root"

ivars.outputFile = 'selected_events.root'

ivars.parseArguments()


# Input source
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring( ivars.inputFiles )
)

# Output definition
process.output = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.FEVTEventContent.outputCommands,
    SelectEvents = cms.untracked.PSet(
                SelectEvents = cms.vstring('reco_step')
                ),
    fileName = cms.untracked.string( ivars.outputFile )
)

import HLTrigger.HLTfilters.hltHighLevel_cfi
process.hltMinBiasHF = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
process.hltMinBiasHF.HLTPaths = ["HLT_HIMinBiasHfOrBSC_Core"]

# Other statements
process.GlobalTag.globaltag = 'GR_R_42_V18::All'

# Path and EndPath definitions

process.reco_step = cms.Path( 
                   process.RawToDigi *
                   process.reconstructionHeavyIons *
                   process.hltMinBiasHF * 
                   process.collisionEventSelection 
)

process.endjob_step = cms.Path(process.endOfProcess)
process.out_step = cms.EndPath(process.output)

process.schedule = cms.Schedule(
    process.reco_step,
    process.endjob_step,
    process.out_step
)

