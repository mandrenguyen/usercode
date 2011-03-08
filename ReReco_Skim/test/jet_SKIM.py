# Revision: 1.232.2.6 
# Source: /cvs_server/repositories/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: centralSkimsHI -s SKIM:DiJet+Photon+ZMM+ZEE --conditions GR10_P_V12::All --scenario HeavyIons --filein=/store/hidata/HIRun2010/HIAllPhysics/RECO/PromptReco-v1/000/150/063/B497BEDB-8BE8-DF11-B09D-0030487A18F2.root --data --no_exec
import FWCore.ParameterSet.Config as cms

process = cms.Process('FILTERRECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

# Other statements
process.GlobalTag.globaltag = 'GR_R_39X_V6B::All'

process.load('MNguyen.ReReco_Skim.Skims_HI_cff')

process.hltJetHI.HLTPaths = ["HLT_HIJet35U"]

process.jetEtFilter.etMin = cms.double(60.0)

process.load('Configuration.EventContent.EventContentHeavyIons_cff')



process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.2 $'),
    annotation = cms.untracked.string('centralSkimsHI nevts:1'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)



# Input source
process.source = cms.Source("PoolSource",
fileNames = cms.untracked.vstring(
'/store/hidata/HIRun2010/HIAllPhysics/RECO/ZS-v2/0004/620269CF-A648-E011-A9F2-003048FEC040.root',
    )
)



process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)

#process.Timing = cms.Service("Timing")



# Output definition

# Additional output definition
process.SKIMStreamDiJet = cms.OutputModule("PoolOutputModule",
                                           process.RECOEventContent,
                                           SelectEvents = cms.untracked.PSet(
    SelectEvents = cms.vstring('diJetSkimPath')
    ),
                                           fileName = cms.untracked.string('Jet35USkim_RECO.root'),
                                           dataset = cms.untracked.PSet(
    filterName = cms.untracked.string('DiJet'),
    dataTier = cms.untracked.string('RECO')
    )
                                           )

process.SKIMStreamDiJet.outputCommands.extend(["keep *_icPu5CaloJetsL2L3_*_*"])




process.diJetSkimPath = cms.Path(process.diJetSkimSequence)

process.SKIMStreamDiJetOutPath = cms.EndPath(process.SKIMStreamDiJet)



# Schedule definition
process.schedule = cms.Schedule(process.diJetSkimPath,process.SKIMStreamDiJetOutPath)


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

