import FWCore.ParameterSet.Config as cms

process = cms.Process("PFCand")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("Saved.DiJetAna.eventSelection_cff")
process.load("Saved.DiJetAna.dijetAna_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(12000) )

process.load("Saved.DiJetAna.Sources.source_HiPAT_cff")
#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring(
#          'rfio:/castor/cern.ch/user/m/mnguyen/CMSSW370patch2/HIPAT/QCDFlat_Spring10_HiReco_MyL2L3_WithPFMuons/PAT_HIReco_9_1_DYo.root'
#      )
#    )

process.load("MNguyen.PFCandAnalyzer.pfcandAnalyzer_cff")

process.TFileService = cms.Service('TFileService',
    fileName = cms.string("/tmp/mnguyen/pfCandidateTree.root")
    )
# Look at pflow w/ pp Reco
process.pfcand.pfCandidateLabel = cms.InputTag("particleFlow","","RECO")

process.ana  = cms.Path(process.pfcand )

process.schedule = cms.Schedule(process.ana )
