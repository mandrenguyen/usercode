import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')
ivars.register('initialEvent',mult=ivars.multiplicity.singleton,info="for testing")


ivars.files='/store/mc/Spring11/QCD_Pt_30_TuneZ2_2760GeV-pythia6/GEN-SIM-RAWDEBUG/START311_V2A-v1/0006/805FCC09-D65A-E011-97C6-003048F5ADEE.root'
ivars.output = 'test2.root'
ivars.maxEvents = -1
ivars.initialEvent = 1

ivars.parseArguments()

import FWCore.ParameterSet.Config as cms

isMC = True
hiReco = True

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')

if hiReco:
    process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
    process.load('Configuration.EventContent.EventContentHeavyIons_cff')
else:
    process.load('Configuration.StandardSequences.Reconstruction_cff')
    process.load('Configuration.EventContent.EventContent_cff')
    
if isMC:
    process.load('SimGeneral.MixingModule.mixNoPU_cfi')
    process.load('Configuration.StandardSequences.RawToDigi_cff')
else:
    process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
    
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.L1Reco_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(
    ivars.maxEvents
    )
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring(),
                            fileNames = cms.untracked.vstring(
    #'file:/hadoop/store/mc/Spring11/QCD_Pt_30_TuneZ2_2760GeV-pythia6/GEN-SIM-RAWDEBUG/START311_V2A-v1/0006/00D55E62-D75A-E011-B4AA-003048344C1A.root'
    ivars.files
    ),
                            #eventsToProcess = cms.untracked.VEventRange('1:318939-1:318939'),
                            #eventsToProcess = cms.untracked.VEventRange('1:1408579-1:1408579'),
)

process.options = cms.untracked.PSet(

)

# Additional output definition

# Other statements
if isMC:
    if hiReco:
        process.GlobalTag.globaltag = 'STARTHI44_V7::All'
    else:
        process.GlobalTag.globaltag = 'START44_V7::All'
else:
    process.GlobalTag.globaltag = 'GR_R_44_V10::All'

######################
# Hi specific reco
######################
# load centrality
from CmsHi.Analysis2010.CommonFunctions_cff import *
overrideCentrality(process)

if isMC:
    process.HeavyIonGlobalParameters = cms.PSet(
	centralityVariable = cms.string("HFtowers"),
	nonDefaultGlauberModel = cms.string("Hydjet_Drum"),
	centralitySrc = cms.InputTag("hiCentrality")
	)
else:
    process.HeavyIonGlobalParameters = cms.PSet(
	centralityVariable = cms.string("HFtowers"),
	nonDefaultGlauberModel = cms.string(""),
	centralitySrc = cms.InputTag("hiCentrality")
	)

# need to produce centrality for other analyzers
process.load("RecoHI.HiCentralityAlgos.HiCentrality_cfi")
process.hiCentrality.producePixelTracks = cms.bool(False)
process.hiCentrality.produceETmidRapidity = cms.bool(False)
process.hiCentrality.producePixelhits = cms.bool(False)
process.hiCentrality.produceEcalhits = cms.bool(False)
process.hiCentrality.produceZDChits = cms.bool(False)
#process.hiCentrality.srcVertex = cms.InputTag("hiSelectedVertex")
process.hiCentrality.produceBasicClusters = cms.bool(False)
process.hiCentrality.produceHFhits = cms.bool(True)
process.hiCentrality.produceTracks = cms.bool(False)

'''
if isMC==False:
    
    import PhysicsTools.PythonAnalysis.LumiList as LumiList
    import FWCore.ParameterSet.Types as CfgTypes
    myLumis = LumiList.LumiList(filename = 'json.txt').getCMSSWString().split(',')
    process.source.lumisToProcess = CfgTypes.untracked(CfgTypes.VLuminosityBlockRange())
    process.source.lumisToProcess.extend(myLumis)
'''


process.raw2digi_step = cms.Path(process.RawToDigi)
process.L1Reco_step = cms.Path(process.L1Reco)
if hiReco:
    process.reconstruction_step = cms.Path(process.reconstructionHeavyIons_withPF)
    #iterative tracking
    process.load("RecoHI.HiTracking.hiIterTracking_cff")
    process.heavyIonTracking *= process.hiIterTracking
    # PF configuration
    process.particleFlowTmp.postMuonCleaning = False
    process.particleFlowBlock.RecMuons = 'muons'
    process.particleFlowClusterPS.thresh_Pt_Seed_Endcap = cms.double(99999.)
    process.pfTrack.UseQuality = True 
    process.pfTrack.TrackQuality = cms.string('highPurity')    
    process.pfTrack.TkColList = cms.VInputTag("hiGeneralTracks")
    process.pfTrack.GsfTracksInEvents = cms.bool(False)

else:
    process.reconstruction_step = cms.Path(process.reconstruction)


# clone one jet reco from the other
if hiReco:
    process.load("CmsHi.JetAnalysis.ExtraJetReco_cff")
    process.akPu3PFJets.src = 'particleTowerProducer'
    process.ak5PFJets = process.akPu3PFJets.clone(
        jetType = 'PFJet',
        src = 'particleFlowTmp',
        rParam = 0.5
        )

else:

    process.ak5PFJets.doAreaFastjet = False
    process.ak5PFJets.jetPtMin = 10.
    # 'Pu' in the name, just means towered
    process.akPu3PFJets = process.ak5PFJets.clone(
        jetType = 'BasicJet',
        src = 'particleTowerProducer',
        rParam = 0.3
        )

process.akPu3PFJets.doPUOffsetCorr = False
process.ak5PFJets.doPUOffsetCorr = False

process.load('RecoHI.HiJetAlgos.ParticleTowerProducer_cff')


process.reco_extra_jet    = cms.Path(         
    process.particleTowerProducer*
    process.akPu3PFJets
    )

if hiReco:
    process.reco_extra_jet *= process.ak5PFJets


# add gen step for HI

if isMC:
    process.load('CmsHi.JetAnalysis.ExtraGenReco_cff')
    process.HiGenParticleAna = cms.EDAnalyzer("HiGenAnalyzer")
    process.HiGenParticleAna.src= cms.untracked.InputTag("generator")    
    process.hiGenParticles.srcVector = cms.vstring('generator')

    process.higen_step          = cms.Path(     
        process.hiGenParticles * process.hiGenParticlesForJets * process.genPartons * process.hiPartons * process.hiRecoGenJets #* process.HiGenParticleAna
        )
    
########  ADD EXTRA RECO WITH REGIT

if hiReco:
    process.akPu3PFSelectedJets = cms.EDFilter("CandViewSelector",
                                               src = cms.InputTag("akPu3PFJets"),
                                               cut = cms.string("pt > 40. && eta > -2. && eta < 2")
                                               )

    process.load("RecoHI.HiTracking.hiRegitTracking_cff")

    process.hiRegitInitialStepSeeds.RegionFactoryPSet.RegionPSet.originRadius = 0.02
    process.hiRegitLowPtTripletStepSeeds.RegionFactoryPSet.RegionPSet.originRadius = 0.02
    process.hiRegitPixelPairStepSeeds.RegionFactoryPSet.RegionPSet.originRadius = 0.015
    process.hiRegitDetachedTripletStepSeeds.RegionFactoryPSet.RegionPSet.originRadius = 1.5
    process.hiRegitMixedTripletStepSeedsA.RegionFactoryPSet.RegionPSet.originRadius = 1.0
    process.hiRegitMixedTripletStepSeedsB.RegionFactoryPSet.RegionPSet.originRadius = 0.5

    process.hiRegitInitialStepSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 0.02
    process.hiRegitLowPtTripletStepSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 0.02
    process.hiRegitPixelPairStepSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 0.015
    process.hiRegitDetachedTripletStepSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 1.5
    process.hiRegitMixedTripletStepSeedsA.RegionFactoryPSet.RegionPSet.originHalfLength = 1.0
    process.hiRegitMixedTripletStepSeedsB.RegionFactoryPSet.RegionPSet.originHalfLength = 0.5

    # test opening up half length
    '''
    process.hiRegitInitialStepSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 15.
    process.hiRegitLowPtTripletStepSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 15.
    process.hiRegitPixelPairStepSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 15.
    process.hiRegitDetachedTripletStepSeeds.RegionFactoryPSet.RegionPSet.originHalfLength = 15.
    process.hiRegitMixedTripletStepSeedsA.RegionFactoryPSet.RegionPSet.originHalfLength = 15.
    process.hiRegitMixedTripletStepSeedsB.RegionFactoryPSet.RegionPSet.originHalfLength = 15.
    '''
    process.hiRegitInitialStepSeeds.RegionFactoryPSet.RegionPSet.deltaPhiRegion = 0.3
    process.hiRegitInitialStepSeeds.RegionFactoryPSet.RegionPSet.deltaEtaRegion = 0.3
    process.hiRegitLowPtTripletStepSeeds.RegionFactoryPSet.RegionPSet.deltaPhiRegion = 0.3
    process.hiRegitLowPtTripletStepSeeds.RegionFactoryPSet.RegionPSet.deltaEtaRegion = 0.3
    process.hiRegitPixelPairStepSeeds.RegionFactoryPSet.RegionPSet.deltaPhiRegion = 0.3
    process.hiRegitPixelPairStepSeeds.RegionFactoryPSet.RegionPSet.deltaEtaRegion = 0.3
    process.hiRegitDetachedTripletStepSeeds.RegionFactoryPSet.RegionPSet.deltaPhiRegion = 0.3
    process.hiRegitDetachedTripletStepSeeds.RegionFactoryPSet.RegionPSet.deltaEtaRegion = 0.3
    process.hiRegitMixedTripletStepSeedsA.RegionFactoryPSet.RegionPSet.deltaPhiRegion = 0.3
    process.hiRegitMixedTripletStepSeedsA.RegionFactoryPSet.RegionPSet.deltaEtaRegion = 0.3
    process.hiRegitMixedTripletStepSeedsB.RegionFactoryPSet.RegionPSet.deltaPhiRegion = 0.3
    process.hiRegitMixedTripletStepSeedsB.RegionFactoryPSet.RegionPSet.deltaEtaRegion = 0.3

    process.hiRegitInitialStepSeeds.RegionFactoryPSet.RegionPSet.JetSrc = cms.InputTag("akPu3PFSelectedJets")
    process.hiRegitLowPtTripletStepSeeds.RegionFactoryPSet.RegionPSet.JetSrc = cms.InputTag("akPu3PFSelectedJets")
    process.hiRegitPixelPairStepSeeds.RegionFactoryPSet.RegionPSet.JetSrc = cms.InputTag("akPu3PFSelectedJets")
    process.hiRegitDetachedTripletStepSeeds.RegionFactoryPSet.RegionPSet.JetSrc = cms.InputTag("akPu3PFSelectedJets")
    process.hiRegitMixedTripletStepSeedsA.RegionFactoryPSet.RegionPSet.JetSrc = cms.InputTag("akPu3PFSelectedJets")
    process.hiRegitMixedTripletStepSeedsB.RegionFactoryPSet.RegionPSet.JetSrc = cms.InputTag("akPu3PFSelectedJets")

    process.load("RecoHI.HiTracking.MergeRegit_cff")
    
    process.regGlobalMuons = process.globalMuons.clone(
        TrackerCollectionLabel = "hiGeneralAndRegitTracks"
        )
    process.regGlbTrackQual = process.glbTrackQual.clone(
        InputCollection = "regGlobalMuons",
        InputLinksCollection = "regGlobalMuons"
        )
    process.regMuons = process.muons.clone()
    process.regMuons.TrackExtractorPSet.inputTrackCollection = "hiGeneralAndRegitTracks"
    process.regMuons.globalTrackQualityInputTag = "regGlbTrackQual"
    process.regMuons.inputCollectionLabels = cms.VInputTag("hiGeneralAndRegitTracks", "regGlobalMuons", "standAloneMuons:UpdatedAtVtx", "tevMuons:firstHit", "tevMuons:picky",
                                                           "tevMuons:dyt")
    
    
    process.regMuonReco = cms.Sequence(
        process.regGlobalMuons*
        process.regGlbTrackQual*
        process.regMuons
        )
        
    process.pfRegTrack = process.pfTrack.clone(TkColList = cms.VInputTag("hiGeneralAndRegitTracks"))
    process.pfRegBlock = process.particleFlowBlock.clone()
    process.regParticleFlow = process.particleFlowTmp.clone()
    
    
    process.pfRegTrack.GsfTracksInEvents = False
    process.regParticleFlow.usePFElectrons = False
    process.regParticleFlow.muons = "regMuons"
    
    
    process.hiRegPF =  cms.Sequence(
        process.pfRegTrack
        *process.pfRegBlock
        *process.regParticleFlow
    )
    
    process.load("RecoHI.HiTracking.HICaloCompatibleTracks_cff")
    process.hiGeneralAndRegitCaloMatchedTracks = process.hiCaloCompatibleTracks.clone(    
        srcTracks = 'hiGeneralAndRegitTracks',
        srcPFCands = 'regParticleFlow'
        )
    
    
    process.hiCaloMatchFilteredTracks = cms.EDFilter("TrackSelector",
                                                     src = cms.InputTag("hiGeneralAndRegitCaloMatchedTracks"),
                                                     cut = cms.string(
        'quality("highPuritySetWithPV") && pt > 1')                                                                                            
                                                     )
    
    
    process.regionalTracking = cms.Path(
        process.akPu3PFSelectedJets *
        process.hiRegitTracking *
        process.hiGeneralAndRegitTracks *
        process.regMuonReco *
        process.hiRegPF *
        process.hiGeneralAndRegitCaloMatchedTracks *
        process.hiCaloMatchFilteredTracks
        )
    
    

# here comes pat
process.load('CmsHi.JetAnalysis.PatAna_cff')

# wow pat reconfigures the reco, how annoying
process.interestingEcalDetIdEB.basicClustersLabel = cms.InputTag("hybridSuperClusters","hybridBarrelBasicClusters")
process.interestingEcalDetIdEE.basicClustersLabel = cms.InputTag("hybridSuperClusters","hybridBarrelBasicClusters")

# Jet energy correction
process.jec = cms.ESSource("PoolDBESSource",
	DBParameters = cms.PSet(messageLevel = cms.untracked.int32(0)),
	timetype = cms.string('runnumber'),
	toGet = cms.VPSet(
		cms.PSet(record = cms.string("JetCorrectionsRecord"),
                         tag = cms.string("JetCorrectorParametersCollection_HI_Calo_hiGoodTightTracks_D6T_413_IC5Calo"),
                         label = cms.untracked.string("IC5Calo")),
                
		cms.PSet(record = cms.string("JetCorrectionsRecord"),
                         tag    = cms.string('JetCorrectorParametersCollection_HI_PFTowers_hiGoodTightTracks_D6T_413_AK3PF'),
                         label = cms.untracked.string("AK3PF")),
                
                ),
                           connect = cms.string("sqlite_file:JEC_HI_PFTower_413patch2_2011_v3.db"),
                           )
process.es_prefer_jec = cms.ESPrefer('PoolDBESSource','jec')

process.akPu3PFcorr.payload = cms.string('AK3PF')

if hiReco:
    process.akPu3PFJetTracksAssociatorAtVertex.tracks = cms.InputTag("hiCaloMatchFilteredTracks")
    #print "Using regit tracks, straight-up "
    #process.akPu3PFJetTracksAssociatorAtVertex.tracks = cms.InputTag("hiGeneralAndRegitTracks")
    process.akPu3PFImpactParameterTagInfos.primaryVertex = "hiSelectedVertex"
else:
    process.akPu3PFJetTracksAssociatorAtVertex.tracks = cms.InputTag("generalTracks")
    process.akPu3PFImpactParameterTagInfos.primaryVertex = "offlinePrimaryVertices"

process.akPu3PFSecondaryVertexTagInfos.trackSelection.qualityClass = 'any'


process.akPu3PFpatJets.addAssociatedTracks = True
process.akPu3PFpatJets.addTagInfos = True
process.akPu3PFpatJets.addBTagInfo         = True
process.akPu3PFpatJets.addDiscriminators   = True
process.akPu3PFpatJets.getJetMCFlavour     = True


process.akPu3PFpatJets.tagInfoSources = cms.VInputTag(
    cms.InputTag("akPu3PFImpactParameterTagInfos"),
    #cms.InputTag("akPu3PFGhostTrackVertexTagInfos"),
    cms.InputTag("akPu3PFSecondaryVertexTagInfos"),
    cms.InputTag("akPu3PFSoftMuonTagInfos"),
    )

# overloading akPu5 with ak5
process.akPu5PFcorr.src = "ak5PFJets"
process.akPu5PFJetTracksAssociatorAtVertex.jets   = "ak5PFJets"
process.akPu5PFSoftMuonTagInfos.jets           = "ak5PFJets"
process.akPu5PFPatJetPartonAssociation.jets    = "ak5PFJets"
process.akPu5PFmatch.src      = "ak5PFJets"
process.akPu5PFparton.src      = "ak5PFJets"
process.akPu5PFpatJets.jetSource            = "ak5PFJets"
                                                                     
process.akPu5PFcorr.payload = cms.string('AK5PF')

if hiReco:
    process.akPu5PFJetTracksAssociatorAtVertex.tracks = cms.InputTag("hiCaloMatchFilteredTracks")
    process.akPu5PFImpactParameterTagInfos.primaryVertex = "hiSelectedVertex"
else:
    process.akPu5PFJetTracksAssociatorAtVertex.tracks = cms.InputTag("generalTracks")
    process.akPu5PFImpactParameterTagInfos.primaryVertex = "offlinePrimaryVertices"

process.akPu5PFSecondaryVertexTagInfos.trackSelection.qualityClass = 'any'


process.akPu5PFpatJets.addAssociatedTracks = True
process.akPu5PFpatJets.addTagInfos = True
process.akPu5PFpatJets.addBTagInfo         = True
process.akPu5PFpatJets.addDiscriminators   = True
process.akPu5PFpatJets.getJetMCFlavour     = True


process.akPu5PFpatJets.tagInfoSources = cms.VInputTag(
    cms.InputTag("akPu5PFImpactParameterTagInfos"),
    #cms.InputTag("akPu5PFGhostTrackVertexTagInfos"),
    cms.InputTag("akPu5PFSecondaryVertexTagInfos"),
    cms.InputTag("akPu5PFSoftMuonTagInfos"),
    )


if isMC:
    process.pat_step          = cms.Path(        
        process.akPu3PFpatSequence_withBtagging
        *process.akPu5PFpatSequence_withBtagging
        )
else:
    process.pat_step          = cms.Path(        
        process.akPu3PFpatSequence_withBtagging_data
        *process.akPu5PFpatSequence_withBtagging_data
        )

#######################
#   Analyzers
########################

# Jet stuff

process.load('CmsHi.JetAnalysis.JetAnalyzers_cff')
#process.akPu3PFJetAnalyzer.eventInfoTag = 'hiSignal'
process.akPu3PFJetAnalyzer.saveBfragments = True
process.akPu3PFJetAnalyzer.eventInfoTag = 'generator'
process.akPu3PFJetAnalyzer.hltTrgResults = cms.untracked.string('TriggerResults::HLT')
process.akPu3PFJetAnalyzer.useVtx = cms.untracked.bool(True)
if hiReco:
    process.akPu3PFJetAnalyzer.vtxTag = 'hiSelectedVertex'
else:
    process.akPu3PFJetAnalyzer.vtxTag = 'offlinePrimaryVertices'
process.akPu3PFJetAnalyzer.useCentrality = cms.untracked.bool(False) 
process.akPu3PFJetAnalyzer.hltTrgNames = cms.untracked.vstring('HLT_HIMinBiasHfOrBSC_Core',
                                                               'HLT_Jet20_v1',
                                                               'HLT_Jet40_v1',
                                                               'HLT_Jet60_v1')
if isMC:
    process.akPu3PFJetAnalyzer.isMC = True
else:
    process.akPu3PFJetAnalyzer.isMC = False
        
process.akPu3PFJetAnalyzer.doLifeTimeTagging = True

process.ak5PFJetAnalyzer = process.akPu3PFJetAnalyzer.clone(
    jetTag = cms.InputTag("akPu5PFpatJets"),
    genjetTag = cms.InputTag("ak5HiGenJets")
    )


#  Track Analyzers

process.load("MitHig.PixelTrackletAnalyzer.trackAnalyzer_cff")

if hiReco:
    #process.anaTrack.trackSrc = 'hiGeneralAndRegitTracks'
    process.anaTrack.trackSrc = 'hiGeneralAndRegitCaloMatchedTracks'
    process.anaTrack.vertexSrc = cms.vstring('hiSelectedVertex')
    process.anaTrack.qualityString = "highPuritySetWithPV"
else:
    process.anaTrack.trackSrc = 'generalTracks'
    process.anaTrack.vertexSrc = cms.vstring('offlinePrimaryVertices')
    process.anaTrack.qualityString = "highPurity"

process.anaTrack.trackPtMin = 0
process.anaTrack.useQuality = False
process.anaTrack.doPFMatching = True
process.anaTrack.doSimTrack = True
process.anaTrack.useCentrality = False

process.load("edwenger.HiTrkEffAnalyzer.hitrkEffAnalyzer_cff")
#process.hitrkEffAnalyzer.tracks = 'generalTracks'
                                                               
# Muons
process.load("HiMuonAlgos.HLTMuTree.hltMuTree_cfi")
process.muonTree = process.hltMuTree.clone()
process.muonTree.doGen = cms.untracked.bool(True)
if hiReco:
    process.muonTree.vertices = 'hiSelectedVertex'
else:
    process.muonTree.vertices = 'offlinePrimaryVertices'


                          
process.trackAnalyzers = cms.Sequence(
    process.anaTrack
    #*process.hitrkEffAnalyzer
    )            


process.ana_step          = cms.Path(         
    process.hiCentrality *
    process.akPu3PFJetAnalyzer *
    process.ak5PFJetAnalyzer *
    process.cutsTPForEff*
    process.cutsTPForFak*
    process.trackAnalyzers*
    process.muonTree
    )


# trigger requirment

process.load('CmsHi.JetAnalysis.EventSelection_cff')
process.load("HLTrigger.HLTfilters.hltHighLevel_cfi")
    
process.hltJetHI.HLTPaths = ['HLT_Jet40_v1']

process.superFilterSequence = cms.Sequence(
    process.hltJetHI
    )

if isMC==False:    
    process.superFilterPath = cms.Path(process.superFilterSequence) 


if hiReco:
    process.primaryVertexFilter.src = 'hiSelectedVertex'
else:
    process.primaryVertexFilter.src = 'offlinePrimaryVertices'
    
process.evtSel = cms.Sequence(
    process.primaryVertexFilter*
    process.HBHENoiseFilter*
    process.hiEcalRecHitSpikeFilter
    #*process.collisionEventSelection
    #*process.hcalTimingFilter
    )

# include some event selection bits

process.load('CmsHi.HiHLTAlgos.hltanalysis_cff')

process.hltanalysis.hltresults = cms.InputTag("TriggerResults")
process.hltAna = cms.Path(process.hltanalysis)
process.pAna = cms.EndPath(process.skimanalysis)
process.skimanalysis.hltresults = cms.InputTag("TriggerResults")


process.TFileService = cms.Service("TFileService",
                                   fileName=cms.string(
    #'bTagAnalyzers.root'
    ivars.output
    )
                                   )



# Schedule definition
if hiReco:
    if isMC:
        process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.reco_extra_jet,process.regionalTracking,process.higen_step,process.pat_step,process.ana_step,process.hltAna,process.pAna)
    else:
        process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.reco_extra_jet,process.regionalTracking,process.pat_step,process.ana_step,process.hltAna,process.pAna)
else:
    if isMC:
        process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.reco_extra_jet,process.higen_step,process.pat_step,process.ana_step,process.hltAna,process.pAna)
    else:
        process.schedule = cms.Schedule(process.raw2digi_step,process.L1Reco_step,process.reconstruction_step,process.reco_extra_jet,process.pat_step,process.ana_step,process.hltAna,process.pAna)

                                

# apply event selection after reco
if isMC==False:
    process.reco_extra_jet._seq = process.evtSel*process.reco_extra_jet._seq
    process.pat_step._seq = process.evtSel*process.pat_step._seq
    process.ana_step._seq = process.evtSel*process.ana_step._seq
    process.hltAna._seq = process.evtSel*process.hltAna._seq
    process.pAna._seq = process.evtSel*process.pAna._seq
    if hiReco:
        process.regionalTracking._seq = process.evtSel*process.regionalTracking._seq

    # apply trigger selection to all paths
    for path in process.paths:
        print getattr(process,path)._seq
        getattr(process,path)._seq = process.superFilterSequence*getattr(process,path)._seq

