import FWCore.ParameterSet.Config as cms


#################################
# Filter on quality tracks
secondStepFilter = cms.EDProducer("QualityFilter",
                                  #TrackQuality = cms.string('highPurity'),
                                  TrackQuality = cms.string(''),
                                  recTracks = cms.InputTag("hiScndGoodTightTracks")
                                  #recTracks = cms.InputTag("hiScndGlobalPrimTracks")
                                  )

# Remaining clusters
hiThrdClusters = cms.EDProducer("TrackClusterRemover",
                                oldClusterRemovalInfo = cms.InputTag("hiScndClusters"),
                                trajectories = cms.InputTag("secondStepFilter"),
                                pixelClusters = cms.InputTag("hiScndClusters"),
                                stripClusters = cms.InputTag("hiScndClusters"),
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
hiThrdPixelRecHits = RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi.siPixelRecHits.clone(
        src = cms.InputTag('hiThrdClusters')
            )
hiThrdStripRecHits = RecoLocalTracker.SiStripRecHitConverter.SiStripRecHitConverter_cfi.siStripMatchedRecHits.clone(
        ClusterProducer = 'hiThrdClusters'
            )

#################################
# Pixel  Layers
import RecoTracker.TkSeedingLayers.PixelLayerPairs_cfi
hiThrdPixelLayerPairs = RecoTracker.TkSeedingLayers.PixelLayerPairs_cfi.pixellayerpairs.clone(
        ComponentName = 'hiThrdPixelLayerPairs',
            )
hiThrdPixelLayerPairs.BPix.HitProducer = 'hiThrdPixelRecHits'
hiThrdPixelLayerPairs.FPix.HitProducer = 'hiThrdPixelRecHits'
#hiThrdPixelLayerPairs.BPix.HitProducer = 'siPixelRecHits'
#hiThrdPixelLayerPairs.FPix.HitProducer = 'siPixelRecHits'


# Pixel Pair Seeding
from RecoTracker.TkSeedGenerator.GlobalSeedsFromPairsWithVertices_cff import *
hiThrdSeedFromPairs = RecoTracker.TkSeedGenerator.GlobalSeedsFromPairsWithVertices_cff.globalSeedsFromPairsWithVertices.clone()
hiThrdSeedFromPairs.RegionFactoryPSet.RegionPSet.ptMin = 4.0
# Ed's original second step value, which keeps the timing down, but throws out secondaries
hiThrdSeedFromPairs.RegionFactoryPSet.RegionPSet.originRadius = 0.005
# This one doesn't exist?
#hiThrdSeedFromPairs.RegionFactoryPSet.RegionPSet.fixedError=0.005
# another parameter that could be played with
#hiThrdSeedFromPairs.RegionFactoryPSet.RegionPSet.originHalfLength=1.0
hiThrdSeedFromPairs.RegionFactoryPSet.RegionPSet.VertexCollection=cms.InputTag("hiSelectedVertex")
hiThrdSeedFromPairs.OrderedHitsFactoryPSet.SeedingLayers = cms.string('hiThrdPixelLayerPairs')
hiThrdSeedFromPairs.ClusterCheckPSet.doClusterCheck=False
hiThrdSeedFromPairs.OrderedHitsFactoryPSet.maxElement = 5000000
#hiThrdSeedFromPairs.OrderedHitsFactoryPSet.GeneratorPSet.maxElement = 5000000
hiThrdSeedFromPairs.ClusterCheckPSet.MaxNumberOfPixelClusters = 5000000

hiThrdSeedFromPairs.OrderedHitsFactoryPSet.SeedingLayers = 'hiThrdPixelLayerPairs'
hiThrdSeedFromPairs.ClusterCheckPSet.PixelClusterCollectionLabel = 'hiThrdClusters'
hiThrdSeedFromPairs.ClusterCheckPSet.ClusterCollectionLabel = 'hiThrdClusters'

#################################
# building
import RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi
hiThrdMeasurementTracker = RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi.MeasurementTracker.clone(
        ComponentName = 'hiThrdMeasurementTracker',
        pixelClusterProducer = 'hiThrdClusters',
        stripClusterProducer = 'hiThrdClusters'
        )


import TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi
hiThrdTrajectoryFilter = TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi.trajectoryFilterESProducer.clone(
    ComponentName = 'hiThrdTrajectoryFilter',
    filterPset = TrackingTools.TrajectoryFiltering.TrajectoryFilterESProducer_cfi.trajectoryFilterESProducer.filterPset.clone(
    minimumNumberOfHits = 6,
    minPt = 1.0
    )
    )

import RecoTracker.CkfPattern.CkfTrajectoryBuilderESProducer_cfi
hiThrdCkfTrajectoryBuilder = RecoTracker.CkfPattern.CkfTrajectoryBuilderESProducer_cfi.CkfTrajectoryBuilder.clone(
    ComponentName = 'hiThrdCkfTrajectoryBuilder',
    MeasurementTrackerName = 'hiThrdMeasurementTracker',
    trajectoryFilterName = 'hiThrdTrajectoryFilter',
    intermediateCleaning = False,
    alwaysUseInvalidHits = False,
    # see what this does to timing, default =5
    maxCand=5
    )

from TrackingTools.TrajectoryCleaning.TrajectoryCleanerBySharedHits_cfi import *
import RecoHI.HiTracking.HICkfTrackCandidates_cff
hiThrdTrackCandidates = RecoHI.HiTracking.HICkfTrackCandidates_cff.hiPrimTrackCandidates.clone(
        src = cms.InputTag('hiThrdSeedFromPairs'),
            TrajectoryBuilder = 'hiThrdCkfTrajectoryBuilder',
            TrajectoryCleaner = 'TrajectoryCleanerBySharedHits' ## since there are multiple seeding patterns
            )
hiThrdTrackCandidates.maxNSeeds = 5000000

#fitting
import RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi
hiThrdGlobalPrimTracks = RecoTracker.TrackProducer.CTFFinalFitWithMaterial_cfi.ctfWithMaterialTracks.clone()
hiThrdGlobalPrimTracks.clusterRemovalInfo = 'hiThrdClusters'
hiThrdGlobalPrimTracks.src                = 'hiThrdTrackCandidates'
hiThrdGlobalPrimTracks.TrajectoryInEvent  = cms.bool(True)
hiThrdGlobalPrimTracks.AlgorithmName = 'iter2'


#################################
#selection, can add a new hiGoodTight type selection here
from MNguyen.iterTracking.TrackSelections_cff import *
hiThrdGoodTightTracks = hiGoodTightTracks.clone(
    src = "hiThrdGlobalPrimTracks",
    min_nhits = cms.uint32(14)
    )

#traditional pp track selection

import RecoTracker.FinalTrackSelectors.selectLoose_cfi
import RecoTracker.FinalTrackSelectors.selectTight_cfi
import RecoTracker.FinalTrackSelectors.selectHighPurity_cfi

ppThrdLoose = RecoTracker.FinalTrackSelectors.selectLoose_cfi.selectLoose.clone(
    src = 'hiThrdGlobalPrimTracks',
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
ppThrdTight = RecoTracker.FinalTrackSelectors.selectTight_cfi.selectTight.clone(
    src = 'ppThrdLoose',
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

ppThrdTracks = RecoTracker.FinalTrackSelectors.selectHighPurity_cfi.selectHighPurity.clone(
    src = 'ppThrdTight',
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


# merging tracks with previous iteration
import RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi
hiThrdMergedGlobalTracks = RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi.simpleTrackListMerger.clone(
        TrackProducer1 = 'hiScndMergedGoodTightTracks',
        TrackProducer2 = 'hiThrdGlobalPrimTracks',
        promoteTrackQuality = True,
        copyExtras = True
        )

# merging tracks with previous iteration
import RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi
hiThrdMergedGoodTightTracks = RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi.simpleTrackListMerger.clone(
        TrackProducer1 = 'hiScndMergedGoodTightTracks',
        TrackProducer2 = 'hiThrdGoodTightTracks',
        promoteTrackQuality = True,
        copyExtras = True
        )

# merging tracks with previous iteration
import RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi
hiThrdMergedppTracks = RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi.simpleTrackListMerger.clone(
        TrackProducer1 = 'hiScndMergedGoodTightTracks',
        TrackProducer2 = 'ppThrdTracks',
        promoteTrackQuality = True,
        copyExtras = True
        )

#################################

thirdStep = cms.Sequence(secondStepFilter * hiThrdClusters * hiThrdPixelRecHits * hiThrdStripRecHits *
                          hiThrdSeedFromPairs * hiThrdTrackCandidates * hiThrdGlobalPrimTracks
                          *ppThrdLoose *ppThrdTight *ppThrdTracks
                          *hiThrdGoodTightTracks)
#*hiThrdMergedGlobalTracks * hiThrdMergedGoodTightTracks * hiThrdMergedppTracks)
