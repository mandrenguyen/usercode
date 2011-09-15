import FWCore.ParameterSet.VarParsing as VarParsing

#ivars = VarParsing.VarParsing('standard')
#ivars.register('initialEvent',mult=ivars.multiplicity.singleton,info="for testing")

#ivars.files = 'dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/yetkin/MixCent0_2p5_NewZS/Mixed_Mix_0.root'

#ivars.output = 'test.root'
#ivars.maxEvents = -1
#ivars.initialEvent = 1

#ivars.parseArguments()

import FWCore.ParameterSet.Config as cms

process = cms.Process('TRACKANA')

#process.Timing = cms.Service("Timing")

#process.SimpleMemoryCheck = cms.Service('SimpleMemoryCheck',
#                                        ignoreTotal=cms.untracked.int32(0),
#                                        oncePerEventMode = cms.untracked.bool(False)
#                                        )

# Input source
process.source = cms.Source("PoolSource",
                            fileNames = cms.untracked.vstring(
    'file:/tmp/mnguyen/FA856D6D-C6ED-DF11-B7D0-0030487C778E.root'
    )
                            )

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
    )


#load some general stuff
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
#process.GlobalTag.globaltag = 'START42_V13::All'
process.GlobalTag.globaltag = 'GR_R_39X_V6B::All'

process.load('Configuration.StandardSequences.Services_cff')
process.load('Configuration.StandardSequences.GeometryExtended_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')

process.load('FWCore.MessageService.MessageLogger_cfi')
process.load("CommonTools.UtilAlgos.TFileService_cfi")



process.HeavyIonGlobalParameters = cms.PSet(
           centralityVariable = cms.string("HFhits"),
           nonDefaultGlauberModel = cms.string(""),
           centralitySrc = cms.InputTag("hiCentrality")
           )

from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)



process.load("edwenger.HiTrkEffAnalyzer.TrackSelections_cff")


#simulated particle stuff
#process.load("SimGeneral.TrackingAnalysis.HiTrackingParticles_cff")
process.load("SimTracker.TrackAssociation.TrackAssociatorByHits_cfi")
process.load("SimTracker.TrackAssociation.trackingParticleRecoTrackAsssociation_cfi")

process.load("PbPbTrackingTools.HiTrackValidator.hitrackvalidator_cfi")
process.hitrkvalidator.trklabel=cms.untracked.InputTag("hiGoodTightTracks")


process.hitrkvalidator.neededCentBins = [0,1,3,11,19,35]


process.load("RecoHI.HiCentralityAlgos.HiCentrality_cfi")

process.hiCentrality.produceHFhits = cms.bool(True)
process.hiCentrality.produceHFtowers = cms.bool(False)
process.hiCentrality.produceEcalhits = cms.bool(False)
process.hiCentrality.produceBasicClusters = cms.bool(False)
process.hiCentrality.produceZDChits = cms.bool(False)
process.hiCentrality.produceETmidRapidity = cms.bool(False)
process.hiCentrality.producePixelhits = cms.bool(False)
process.hiCentrality.produceTracks = cms.bool(False)
process.hiCentrality.producePixelTracks = cms.bool(False)
process.hiCentrality.doPixelCut = cms.bool(True)
process.hiCentrality.srcTracks = 'hiGoodTightTracks'

process.hiValidatorSequence = cms.Sequence(
    process.hiCentrality*
    process.hitrkvalidator
)

process.load('edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cff')
process.hitrkEffAnalyzer.useJetEtMode = 0
process.hitrkEffAnalyzer.hasSimInfo = cms.untracked.bool(False)

process.load('MitHig.PixelTrackletAnalyzer.trackAnalyzer_cff')
process.anaTrack.doSimTrack = cms.untracked.bool(False)
process.anaTrack.doPFMatching = cms.untracked.bool(False)
process.anaTrack.trackSrc = cms.InputTag('hiGoodTightTracks')

process.simpleTrack = cms.EDAnalyzer('SimpleTrackAnalyzer',
                                     trackSrc = cms.InputTag("hiGoodTightTracks"),
                                     vertexSrc = cms.InputTag("hiSelectedVertex")
)




#for tree output
process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string("trackAnalysis.root")
                                   )


process.trackSel = cms.Path(
    process.hiPostGlobalPrimTracks*
    process.hiGoodTightTracksSelection
)

process.trackAna = cms.Path(
    #process.cutsTPForFak*
    #process.cutsTPForEff*    
    process.hiValidatorSequence*
    process.hitrkEffAnalyzer*
    process.anaTrack*
    process.simpleTrack
    )

process.schedule = cms.Schedule(process.trackSel,process.trackAna)

