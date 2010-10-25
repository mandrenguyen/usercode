import FWCore.ParameterSet.Config as cms

process = cms.Process("PFCand")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.load("Saved.DiJetAna.eventSelection_cff")
process.load("Saved.DiJetAna.dijetAna_cff")

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

#process.load("MNguyen.PFCandAnalyzer.Sources.source_HiPAT_10files_cff")
#process.load("MNguyen.PFCandAnalyzer.Sources.source_HydJet_Quenched_MinBias_2760GeV_Spring10_cff")
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring(
#          'rfio:/castor/cern.ch/user/m/mnguyen/CMSSW370patch2/HIPAT/QCDFlat_Spring10_HiReco_MyL2L3_WithPFMuons/PAT_HIReco_9_1_DYo.root'
#'rfio:/castor/cern.ch/user/m/mnguyen//CMSSW370patch2/HIPAT/Hydjet_Quenched_MinBias_2760GeV_Spring10/PAT_HIReco_101_1_3HL.root'
'rfio:/castor/cern.ch/user/m/mnguyen//CMSSW370patch2/HIRECO_BadResponseSkim/QCDFlat_Spring10_HiReco_MyL2L3_WithPFMuons/filteredEvents_AK5PFRspLT0p3_GenPtGT100_99_1_EJ1.root'
      )
    )

process.load("MNguyen.PFCandAnalyzer.pfcandAnalyzer_cff")

process.TFileService = cms.Service('TFileService',
    fileName = cms.string("pfCandidateTree.root")
    )
# Look at pflow w/ pp Reco
#process.pfcand.pfCandidateLabel = cms.InputTag("particleFlow","","RECO")

process.ana  = cms.Path(process.pfcand )

process.schedule = cms.Schedule(process.ana )
