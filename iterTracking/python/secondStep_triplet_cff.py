import FWCore.ParameterSet.Config as cms

#################################
# Filter on quality tracks
firstStepFilter = cms.EDProducer("QualityFilter",
                                     TrackQuality = cms.string('highPurity'),
                                     recTracks = cms.InputTag("hiGoodTightTracks")
                                 )

# Remaining clusters
hiScndClusters = cms.EDProducer("TrackClusterRemover",
                               trajectories = cms.InputTag("firstStepFilter"),
                               pixelClusters = cms.InputTag("siPixelClusters"),
                               stripClusters = cms.InputTag("siStripClusters"),
                               Common = cms.PSet(
    maxChi2 = cms.double(30.) # remove none=0, remove all=9999
    ),
                               Strip = cms.PSet(
    maxChi2 = cms.double(30.),
    #maxSize = cms.uint32(3)   # only remove strip clusters with size<=3
    #Yen-Jie's mod
    maxSize = cms.uint32(2)   # only remove strip clusters with size<=2
    )
                               )

# Remake corresponding rechits
import RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi
import RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi
hiScndPixelRecHits = RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi.siPixelRecHits.clone(
        src = cms.InputTag('hiScndClusters')
            )
hiScndStripRecHits = RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi.siStripMatchedRecHits.clone(
        ClusterProducer = 'hiScndClusters'
            )

#################################
# Pixel  Layers
import RecoTracker.TkSeedingLayers.PixelLayerTriplets_cfi
hiScndPixelLayerTriplets = RecoTracker.TkSeedingLayers.PixelLayerTriplets_cfi.pixellayertriplets.clone(
        ComponentName = 'hiScndPixelLayerTriplets',
            )
hiScndPixelLayerTriplets.BPix.HitProducer = 'hiScndPixelRecHits'
hiScndPixelLayerTriplets.FPix.HitProducer = 'hiScndPixelRecHits'


# Pixel Pair Seeding
from RecoTracker.TkSeedGenerator.GlobalSeedsFromTriplets_cff import *
hiScndSeedFromTriplets = RecoTracker.TkSeedGenerator.GlobalSeedsFromTriplets_cff.globalSeedsFromTriplets.clone()
hiScndSeedFromTriplets.RegionFactoryPSet.RegionPSet.ptMin = 4.0
# Ed's original second step value, which seems small.  hipixel tracking and most others use 0.2
hiScndSeedFromTriplets.RegionFactoryPSet.RegionPSet.originRadius = 0.005
#hiScndSeedFromTriplets.RegionFactoryPSet.RegionPSet.originRadius = 2.0
#hiScndSeedFromTriplets.RegionFactoryPSet.RegionPSet.originRadius = 0.05
# This one doesn't exist?
#hiScndSeedFromTriplets.RegionFactoryPSet.RegionPSet.fixedError=0.005
#putting this one
#hiScndSeedFromTriplets.RegionFactoryPSet.RegionPSet.originHalfLength=1.0
hiScndSeedFromTriplets.RegionFactoryPSet.RegionPSet.VertexCollection=cms.InputTag("hiSelectedVertex")
hiScndSeedFromTriplets.OrderedHitsFactoryPSet.SeedingLayers = cms.string('hiScndPixelLayerTriplets')
hiScndSeedFromTriplets.ClusterCheckPSet.doClusterCheck=False
hiScndSeedFromTriplets.OrderedHitsFactoryPSet.mexElement = 5000000
hiScndSeedFromTriplets.OrderedHitsFactoryPSet.GeneratorPSet.maxElement = 5000000
hiScndSeedFromTriplets.ClusterCheckPSet.MaxNumberOfPixelClusters = 5000000

hiScndSeedFromTriplets.ClusterCheckPSet.PixelClusterCollectionLabel = 'hiScndClusters'
hiScndSeedFromTriplets.ClusterCheckPSet.ClusterCollectionLabel = 'hiScndClusters'

#################################
# building
import RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi
hiScndMeasurementTracker = RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi.MeasurementTracker.clone(
        ComponentName = 'hiScndMeasurementTracker',
        pixelClusterProducer = 'hiScndClusters',
         stripClusterProducer = 'hiScndClusters'
        )

import TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi
hiScndTrajectoryFilter = TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi.trajectoryFilterESProducer.clone(
    ComponentName = 'hiScndTrajectoryFilter',
    filterPset = TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi.trajectoryFilterESProducer.filterPset.clone(
    minimumNumberOfHits = 6,
    minPt = 1.0
    )
    )

import RecoTracker.CkfPattern.CkfTrajectoryBuilderESProducer_cfi
hiScndCkfTrajectoryBuilder = RecoTracker.CkfPattern.CkfTrajectoryBuilderESProducer_cfi.CkfTrajectoryBuilder.clone(
    ComponentName = 'hiScndCkfTrajectoryBuilder',
    MeasurementTrackerName = 'hiScndMeasurementTracker',
    trajectoryFilterName = 'hiScndTrajectoryFilter',
    intermediateCleaning = False,
    alwaysUseInvalidHits = False,
    # see what this does to timing, default =5
    maxCand=5
    )

from TrackingTools.TrajectoryCleaning.TrajectoryCleanerBySharedHits_cfi import *
import RecoHI.HiTracking.HICkfTrackCandidates_cff
hiScndTrackCandidates = RecoHI.HiTracking.HICkfTrackCandidates_cff.hiPrimTrackCandidates.clone(
        src = cms.InputTag('hiScndSeedFromTriplets'),
            TrajectoryBuilder = 'hiScndCkfTrajectoryBuilder',
            TrajectoryCleaner = 'TrajectoryCleanerBySharedHits' ## since there are multiple seeding patterns
            )
hiScndTrackCandidates.maxNSeeds = 5000000

#fitting
import RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi
hiScndGlobalPrimTracks = RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi.ctfWithMaterialTracks.clone()
hiScndGlobalPrimTracks.clusterRemovalInfo = 'hiScndClusters'
hiScndGlobalPrimTracks.src                = 'hiScndTrackCandidates'
hiScndGlobalPrimTracks.TrajectoryInEvent  = cms.bool(True)
hiScndGlobalPrimTracks.AlgorithmName = 'iter1'


#################################
#selection, can add a new hiGoodTight type selection here
from MNguyen.iterTracking.TrackSelections_cff import *
hiScndGoodTightTracks = hiGoodTightTracks.clone(
    src = "hiScndGlobalPrimTracks"
    )

#traditional pp track selection

import RecoTracker.FinalTrackSelectors.selectLoose_cfi
import RecoTracker.FinalTrackSelectors.selectTight_cfi
import RecoTracker.FinalTrackSelectors.selectHighPurity_cfi

ppScndLoose = RecoTracker.FinalTrackSelectors.selectLoose_cfi.selectLoose.clone(
    src = 'hiScndGlobalPrimTracks',
    keepAllTracks = False,
    copyExtras = False,
    copyTrajectories = True,
    applyAdaptedPVCuts = False,
    chi2n_par = 99.,
    minNumberLayers = 5,
    minNumber3DLayers = 0,
    vertices = 'hiSelectedVertex',
    vertexCut = ''
    )
ppScndTight = RecoTracker.FinalTrackSelectors.selectTight_cfi.selectTight.clone(
    src = 'ppScndLoose',
    keepAllTracks = False,
    copyExtras = False,
    copyTrajectories = True,
    applyAdaptedPVCuts = False,
    chi2n_par = 99.,
    minNumberLayers = 10,
    minNumber3DLayers = 3,
    vertices = 'hiSelectedVertex',
    vertexCut = ''
    )

ppScndTracks = RecoTracker.FinalTrackSelectors.selectHighPurity_cfi.selectHighPurity.clone(
    src = 'ppScndTight',
    keepAllTracks = False,
    copyExtras = False,
    copyTrajectories = True,
    applyAdaptedPVCuts = False,
    chi2n_par = 99.,
    minNumberLayers = 10,
    minNumber3DLayers = 3,
    vertices = 'hiSelectedVertex',
    vertexCut = ''
    )


## merging tracks with previous iteration
#import RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi
#hiScndMergedGlobalTracks = RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi.simpleTrackListMerger.clone(
#        TrackProducer1 = 'hiGoodTightTracks',
#        TrackProducer2 = 'hiScndGlobalPrimTracks',
#        promoteTrackQuality = True,
#        copyExtras = True
#        )
#
## merging tracks with previous iteration
#import RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi
#hiScndMergedGoodTightTracks = RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi.simpleTrackListMerger.clone(
#        TrackProducer1 = 'hiGoodTightTracks',
#        TrackProducer2 = 'hiScndGoodTightTracks',
#        promoteTrackQuality = True,
#        copyExtras = True
#        )
#
## merging tracks with previous iteration
#import RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi
#hiScndMergedppTracks = RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi.simpleTrackListMerger.clone(
#        TrackProducer1 = 'hiGoodTightTracks',
#        TrackProducer2 = 'ppScndTracks',
#        promoteTrackQuality = True,
#        copyExtras = True
#        )

#################################

secondStep = cms.Sequence(firstStepFilter*
                          hiScndClusters*
                          hiScndPixelRecHits * hiScndStripRecHits *
                          hiScndSeedFromTriplets*
                          hiScndTrackCandidates * hiScndGlobalPrimTracks
                          *ppScndLoose *ppScndTight *ppScndTracks
                          *hiScndGoodTightTracks)
#*hiScndMergedGlobalTracks * hiScndMergedGoodTightTracks * hiScndMergedppTracks)
