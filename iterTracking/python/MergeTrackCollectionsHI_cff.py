import FWCore.ParameterSet.Config as cms

import RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi
# Track filtering and quality.
#   input:    zeroStepTracksWithQuality,preMergingFirstStepTracksWithQuality,secStep,thStep,pixellessStep
#   output:   generalTracks
#   sequence: trackCollectionMerging

#

iterTracks = RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi.simpleTrackListMerger.clone(
    TrackProducer1 = 'hiScndGoodTightTracks',
    TrackProducer2 = 'hiThrdGoodTightTracks',
    promoteTrackQuality = True
    )


hiGeneralTracks = RecoTracker.FinalTrackSelectors.simpleTrackListMerger_cfi.simpleTrackListMerger.clone(
    TrackProducer1 = 'hiGoodTightTracks',
    TrackProducer2 = 'iterTracks',
    promoteTrackQuality = True,
    copyExtras = True,
    makeReKeyedSeeds = cms.untracked.bool(True)
    )


trackCollectionMerging = cms.Sequence(
    iterTracks*
    hiGeneralTracks
    )
