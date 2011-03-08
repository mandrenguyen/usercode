import FWCore.ParameterSet.Config as cms


# Coincidence of HF towers above threshold
from HeavyIonsAnalysis.Configuration.hfCoincFilter_cff import *

# Selection of at least a two-track fitted vertex
primaryVertexFilter = cms.EDFilter("VertexSelector",
    src = cms.InputTag("hiSelectedVertex"),
    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2 && tracksSize >= 2"), 
    filter = cms.bool(True),   # otherwise it won't filter the events
)

# Cluster-shape filter re-run offline
from RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi import *
from HLTrigger.special.hltPixelClusterShapeFilter_cfi import *
hltPixelClusterShapeFilter.inputTag = "siPixelRecHits"

# Reject BSC beam halo L1 technical bits
from L1TriggerConfig.L1GtConfigProducers.L1GtTriggerMaskTechTrigConfig_cff import *
from HLTrigger.HLTfilters.hltLevel1GTSeed_cfi import hltLevel1GTSeed
noBSChalo = hltLevel1GTSeed.clone(
    L1TechTriggerSeeding = cms.bool(True),
    L1SeedsLogicalExpression = cms.string('NOT (36 OR 37 OR 38 OR 39)')
)

# HLT jet trigger
import HLTrigger.HLTfilters.hltHighLevel_cfi
hltJetHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
hltJetHI.HLTPaths = ["HLT_HIJet35U"]
hltJetHI.throw = False
hltJetHI.andOr = True


# jet energy correction (L2+L3)
from JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff import *
icPu5CaloJetsL2L3 = cms.EDProducer('CaloJetCorrectionProducer',
    src = cms.InputTag('iterativeConePu5CaloJets'),
    correctors = cms.vstring('ic5CaloL2L3')
    )

# leading jet E_T filter
jetEtFilter = cms.EDFilter("EtMinCaloJetCountFilter",
    src = cms.InputTag("icPu5CaloJetsL2L3"),
    etMin = cms.double(60.0),
    minNumber = cms.uint32(1)
    )



diJetSkimSequence = cms.Sequence(noBSChalo *
                                 hfCoincFilter3 *
                                 primaryVertexFilter *
                                 hltJetHI 
                                 * icPu5CaloJetsL2L3
                                 * jetEtFilter
                                 )
