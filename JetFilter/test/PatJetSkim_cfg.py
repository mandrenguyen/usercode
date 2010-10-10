import FWCore.ParameterSet.Config as cms
 
process = cms.Process("PatJetSkim")

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
 
#process.load("PFAnalyses.PFCandidate.Run133533AndUp_cff")
process.source = cms.Source (
    "PoolSource",
    fileNames=cms.untracked.vstring(
    #"file:/tmp/mnguyen/hww2l_RECOSIM.root",
       "rfio:/castor/cern.ch/user/m/mnguyen/CMSSW370patch2/HIPAT/QCDFlat_Spring10_HiReco_WithPFMuons/PAT_HIReco_9_1_7OZ.root",
    ),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck')
)


process.load("MNguyen.JetFilter.JetFilter_cfi")

process.patJetFilter =cms.Path(process.jetFilter)

process.options = cms.untracked.PSet(
 wantSummary = cms.untracked.bool(True)
)

process.outputSkim = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring('keep *'),
    fileName = cms.untracked.string("LargeTrackError.root"),
    dataset = cms.untracked.PSet(
      dataTier = cms.untracked.string('GEN-SIM-RECO'),
#      dataTier = cms.untracked.string('RAW-RECO'),
      filterName = cms.untracked.string('trackerFilter')
      ),
   SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('patJetFilter'))
)
process.outpath = cms.EndPath(process.outputSkim)

# And the logger
#process.load("FWCore.MessageLogger.MessageLogger_cfi")
#process.options = cms.untracked.PSet(
#    fileMode = cms.untracked.string('NOMERGE'),
#    makeTriggerResults = cms.untracked.bool(True),
#    wantSummary = cms.untracked.bool(True),
#    Rethrow = cms.untracked.vstring('Unknown', 
#        'ProductNotFound', 
#        'DictionaryNotFound', 
#        'InsertFailure', 
#        'Configuration', 
#        'LogicError', 
#        'UnimplementedFeature', 
#        'InvalidReference', 
#        'NullPointerError', 
#        'NoProductSpecified', 
#        'EventTimeout', 
#        'EventCorruption', 
#        'ModuleFailure', 
#        'ScheduleExecutionFailure', 
#        'EventProcessorFailure', 
#        'FileInPathError', 
#        'FatalRootError', 
#        'NotFound')
#)
#process.MessageLogger.cerr.FwkReport.reportEvery = 100

