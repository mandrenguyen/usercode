import FWCore.ParameterSet.Config as cms

# HLT jet trigger
import HLTrigger.HLTfilters.hltHighLevel_cfi
hltJetHI = HLTrigger.HLTfilters.hltHighLevel_cfi.hltHighLevel.clone()
hltJetHI.HLTPaths = ["HLT_HIJet35U"]
hltJetHI.throw = False
hltJetHI.andOr = True

# selection of valid vertex
#primaryVertexFilterForJets = cms.EDFilter("VertexSelector",
#    src = cms.InputTag("hiSelectedVertex"),
#    cut = cms.string("!isFake && abs(z) <= 25 && position.Rho <= 2"), 
#    filter = cms.bool(True),   # otherwise it won't filter the events
#    )

from HeavyIonsAnalysis.Configuration.collisionEventSelection_cff import *


# jet energy correction (L2+L3) ??
from JetMETCorrections.Configuration.JetCorrectionServicesAllAlgos_cff import *
icPu5CaloJetsL2L3 = cms.EDProducer('CaloJetCorrectionProducer',
    src = cms.InputTag('iterativeConePu5CaloJets'),
                                   correctors = cms.vstring('ic5CaloL2L3')
                                   )

# leading jet E_T filter
jetEtFilter = cms.EDFilter("EtMinCaloJetCountFilter",
    src = cms.InputTag("icPu5CaloJetsL2L3"),
    etMin = cms.double(200.0),
    minNumber = cms.uint32(1)
    )

# dijet E_T filter
#dijetEtFilter = cms.EDFilter("EtMinCaloJetCountFilter",
#    src = cms.InputTag("icPu5CaloJetsL2L3"),
#    etMin = cms.double(50.0),
#    minNumber = cms.uint32(2)
#    )

#from RecoHI.HiEgammaAlgos.hiEcalSpikeFilter_cfi import *
from CmsHi.PhotonAnalysis.hiEcalRecHitSpikeFilter_cfi import *
hiEcalRecHitSpikeFilter.minEt = 50.0


# jet skim sequence
jetSkimSequence = cms.Sequence(hltJetHI
                                 * collisionEventSelection
                                 #* icPu5CaloJetsL2L3
                                 #* jetEtFilter
                                 #* dijetEtFilter
                                 * hiEcalRecHitSpikeFilter
                                 )
#jetSkimPath = cms.Path(hltJetHI
#                                 * collisionEventSelection
#                                 #* icPu5CaloJetsL2L3
#                                 #* jetEtFilter
#                                 #* dijetEtFilter
#                                 * hiEcalRecHitSpikeFilter
#                                 )
