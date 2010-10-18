import FWCore.ParameterSet.Config as cms

process = cms.Process("PFMuon")

process.load("FWCore.MessageService.MessageLogger_cfi")
process.MessageLogger.cerr.FwkReport.reportEvery = 100

process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )

process.load("MNguyen.MuonPFAnalysis.Sources.source_highPtMuonSkim_QCDFlat_390pre2ReReco_cff")
#process.load("MNguyen.MuonPFAnalysis.Sources.source_InclusiveMuon100pTHat250_390pre2ReReco_cff")
#process.source = cms.Source("PoolSource",
#    fileNames = cms.untracked.vstring(
#'rfio:/castor/cern.ch/user/m/mnguyen//CMSSW390pre2/RERECO/InclusiveMuon100pTHat250/InclusiveMu5_Pt250/reco_0.root'
#)
#)

process.load("MNguyen.MuonPFAnalysis.pfMuonAnalyzer_cff")

process.TFileService = cms.Service('TFileService',
    fileName = cms.string("pfMuonHistos.root")
    )

process.ana  = cms.Path(process.pfmuon )

process.schedule = cms.Schedule(process.ana )
