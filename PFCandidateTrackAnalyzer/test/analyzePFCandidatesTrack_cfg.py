import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')
ivars.register('initialEvent',mult=ivars.multiplicity.singleton,info="for testing")

ivars.files = "dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/davidlw/Hydjet_Quenched_MinBias_2760GeV/Fall10-START39_V4HI-391redigi-v5-GEN-SIM-RECO/597653989fad95507f68df8cea7f0ee4/hiReco_RAW2DIGI_RECO_44_1_nqc.root"

ivars.output = 'pfntuple.root'
ivars.maxEvents = -1
ivars.initialEvent = 1

ivars.parseArguments()

import FWCore.ParameterSet.Config as cms
process = cms.Process("ANALYSIS")

# Services
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration/StandardSequences/ReconstructionHeavyIons_cff')
process.load("RecoHI.Configuration.Reconstruction_hiPF_cff")
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = 'MC_39Y_V2::All'

process.MessageLogger.cerr.FwkReport.reportEvery=1
#process.options = cms.untracked.PSet(wantSummary=cms.untracked.bool(True))

#process.Timing = cms.Service("Timing")

#Input source
process.source = cms.Source (
    "PoolSource",    
    fileNames = cms.untracked.vstring(ivars.files),
    secondaryFileNames = cms.untracked.vstring(),
    noEventSort = cms.untracked.bool(True),
    duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
)

process.maxEvents = cms.untracked.PSet(
            input = cms.untracked.int32(ivars.maxEvents)
)

#Centrality Filter
process.load("RecoHI.HiCentralityAlgos.CentralityFilter_cfi")

process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("HeavyIonRcd"),
                              tag = cms.string("CentralityTable_HFhits40_Hydjet2760GeV_v1_mc"),
                              connect = cms.untracked.string("frontier://FrontierPrep/CMS_COND_PHYSICSTOOLS")
                              )
            )

process.centralityFilter.selectedBins = [0,1,2,3,4,5,6,7]


# Filter on PF candidate pt
process.clones = cms.EDFilter("PtMinCandViewCloneSelector",
    src=cms.InputTag("particleFlow",""),
                              ptMin=cms.double(15.0),
    filter=cms.bool(True)
)

# Filter on reco::Track pt
process.trkfilter = cms.EDFilter("PtMinTrackSelector",
                                 src = cms.InputTag("hiSelectedTracks"),
                                 ptMin = cms.double(15.0),
                                 filter = cms.bool(True),
                                 )

process.rechits = cms.Sequence(process.siPixelRecHits * process.siStripMatchedRecHits)
process.pfreco = cms.Sequence(process.rechits * process.heavyIonTracking * process.HiParticleFlowReco)
#process.filter_seq = cms.Sequence(process.centralityFilter * process.trkfilter * process.pfreco * process.clones)
process.filter_seq = cms.Sequence( process.trkfilter * process.pfreco * process.clones)




# track association
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")
process.TrackAssociatorByHits.SimToRecoDenominator = cms.string('reco')


# PF candidate analyzer
process.pfCandidateAnalyzer = cms.EDAnalyzer("PFCandidateTrackAnalyzer",
    PFCandidates = cms.InputTag("particleFlow",""),
    Vertices = cms.InputTag("hiSelectedVertex"),                                         
    verbose = cms.untracked.bool(False), ## print candidate info
    printBlocks = cms.untracked.bool(True), ## print block/element info
    ptMin = cms.untracked.double(15.0), ## of PF candidate
    SimTracks = cms.InputTag("mergedtruth","MergedTrackTruth"),
    Tracks = cms.InputTag("hiSelectedTracks"),                                         
    hasSimInfo = cms.untracked.bool(True),
    minHits = cms.untracked.double(5),
    maxPtErr = cms.untracked.double(0.05),
    maxD0 = cms.untracked.double(0.2),                                         
    maxDZ = cms.untracked.double(0.2),
    maxD0Norm = cms.untracked.double(3.0),
    maxDZNorm = cms.untracked.double(3.0),
    pixelSeedOnly = cms.untracked.bool(True)                                         
)

# edm output
process.aod = cms.OutputModule("PoolOutputModule",
    outputCommands=cms.untracked.vstring('drop *',
                                         'keep recoPFClusters_*CAL_*_*',
                                         'keep recoPFBlocks_*_*_*', 
                                         'keep recoPFCandidates_*__*',
                                         'keep recoTracks_generalTracks_*_*'),
    fileName = cms.untracked.string('aod.root'),
    SelectEvents = cms.untracked.PSet(SelectEvents = cms.vstring('filter'))
)

# ntuple output
process.TFileService = cms.Service("TFileService",
                                  fileName=cms.string(ivars.output))
# Paths

#process.myTrackAssociatorByHitsRecoDenom = cms.Sequence(TrackAssociatorByHitsRecoDenom)


process.trackerDrivenElectronSeeds.UseQuality = cms.bool(True)
#process.trackerDrivenElectronSeeds.UseQuality = cms.bool(False)
#process.trackerDrivenElectronSeeds.TrackQuality = cms.string('highPurity')
process.trackerDrivenElectronSeeds.TkColList = cms.VInputTag("hiSelectedTracks")

process.filter = cms.Path(process.filter_seq)
process.p = cms.Path(process.filter_seq*
                     #process.myTrackAssociatorByHitsRecoDenom*
                     process.pfCandidateAnalyzer)
#process.outpath = cms.EndPath(process.aod)

# Schedule
process.schedule = cms.Schedule(process.filter,
                                process.p,
                                #process.outpath
                                )


def customiseMC(process):
    #fileName=cms.string(ivars.output))
    #process.TFileService.fileName="pftupleMC.root"     # new ntuple name
    #process.source.fileNames= [
        #'/store/relval/CMSSW_3_9_0/RelValHydjetQ_B0_2760GeV/GEN-SIM-RECO/MC_39Y_V2-v1/0050/1A554290-13D8-DF11-A34B-002618943920.root'
        #'/store/relval/CMSSW_3_9_0/RelValPyquen_DiJet_pt80to120_2760GeV/GEN-SIM-RECO/MC_39Y_V2-v1/0052/6CA5E6DF-1CD9-DF11-A3E4-002618943849.root'
        #'/store/relval/CMSSW_3_9_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/MC_39Y_V2-v1/0049/62E08581-F2D7-DF11-8475-002618943986.root'
        #'dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/himc/Fall10/Hydjet_Quenched_MinBias_2760GeV/GEN-SIM-RECODEBUG/Pyquen_DiJet_Pt80_MC_38Y_V12-v2/0006/4EE677A1-DBE0-DF11-BE92-001D0967D314.root'
        #]

    #process.trkfilter.src = "generalTracks"
    process.trkfilter.src = "hiSelectedTracks"
    #process.pfCandidateAnalyzer.Vertices = "offlinePrimaryVertices"
    #process.pfCandidateAnalyzer.Tracks = "generalTracks"
    process.pfCandidateAnalyzer.hasSimInfo=cms.untracked.bool(True)
    
    return process

def customiseLooseCuts(process):
    process.pfCandidateAnalyzer.minHits=0
    process.pfCandidateAnalyzer.maxPtErr=99999.
    process.pfCandidateAnalyzer.maxD0=99999.0
    process.pfCandidateAnalyzer.maxDZ=99999.0
    process.pfCandidateAnalyzer.maxD0Norm=99999.0
    process.pfCandidateAnalyzer.maxDZNorm=99999.0
    process.pfCandidateAnalyzer.pixelSeedOnly=False

    return process


#process = customiseMC(process)
process = customiseLooseCuts(process)
