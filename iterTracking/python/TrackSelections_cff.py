import FWCore.ParameterSet.Config as cms


hiGoodTracks = cms.EDProducer("HiHackedAnalyticalTrackSelector",

    src = cms.InputTag("hiSelectedTracks"),
    keepAllTracks = cms.bool(False), ## if set to true tracks failing this filter are kept in the output
    beamspot = cms.InputTag("offlineBeamSpot"),

    vertices = cms.InputTag("hiSelectedVertex"),
    vtxNumber = cms.int32(0),
    vtxTracks = cms.uint32(0), ## at least 3 tracks
    vtxChi2Prob = cms.double(0.0), ## at least 1% chi2nprobability (if it has a chi2)

    copyTrajectories = cms.untracked.bool(True),
    copyExtras = cms.untracked.bool(False), ## set to false on AOD

    cutOnTightQuality = cms.bool(False),
    qualityBit = cms.string('highPurity'), ## set to '' or comment out if you don't want to set the bit

    # parameters for cutting on pterror/pt and number of valid hits
    min_relpterr = cms.double(0.05),
    min_nhits = cms.uint32(12),                           

    # parameters for adapted optimal cuts on chi2 and primary vertex compatibility
    chi2n_par = cms.double(99999.), # already applied in hiSelectedTracks
    res_par = cms.vdouble(99999., 99999.),
    d0_par1 = cms.vdouble(99999., 0.0),
    dz_par1 = cms.vdouble(99999., 0.0),
    d0_par2 = cms.vdouble(3, 0.0),
    dz_par2 = cms.vdouble(3, 0.0),
    # Boolean indicating if adapted primary vertex compatibility cuts are to be applied.
    applyAdaptedPVCuts = cms.bool(True),

    # Impact parameter absolute cuts.
    max_d0 = cms.double(1000),
    max_z0 = cms.double(1000),

    # Cuts on numbers of layers with hits/3D hits/lost hits. 
    minNumberLayers = cms.uint32(0),
    minNumber3DLayers = cms.uint32(0),
    maxNumberLostLayers = cms.uint32(99999)
)

hiGoodTightTracks = hiGoodTracks.clone(src = cms.InputTag("hiGlobalPrimTracks"),
                                       keepAllTracks = cms.bool(True),
                                       qualityBit = cms.string('highPurity'),
                                       min_nhits = cms.uint32(13),
                                       chi2n_par = cms.double(0.15),
                                       # 0.05 is the "correct" value
                                       min_relpterr = cms.double(0.05),
                                       # used previously by mistake
                                       #min_relpterr = cms.double(0.06),
                                       cutOnTightQuality = cms.bool(False)
                                       )

# this selection is used for calo-compatible tracks 
hiLooseTracks = hiGoodTracks.clone(src = cms.InputTag("hiGlobalPrimTracks"),
                                   qualityBit = cms.string('loose'),
                                   min_relpterr = cms.double(0.10),
                                   min_nhits = cms.uint32(10),
                                   chi2n_par = cms.double(0.15),
                                   d0_par2 = cms.vdouble(8.0, 0.0),
                                   dz_par2 = cms.vdouble(8.0, 0.0)
                                   )

hiGoodTracksSelection = cms.Sequence(hiGoodTracks)
hiGoodTightTracksSelection = cms.Sequence(hiGoodTightTracks)
