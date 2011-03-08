import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContentHeavyIons_cff import FEVTEventContent
skimContent = FEVTEventContent.clone()
#skimContent.outputCommands.append("drop *_MEtoEDMConverter_*_*")
#skimContent.outputCommands.append("drop *_*_*_SKIM")

#####################

from HeavyIonsAnalysis.Configuration.HI_DiJetSkim_cff import *
diJetSkimPath = cms.Path( diJetSkimSequence )
SKIMStreamDiJet = cms.FilteredStream(
    responsible = 'HI PAG',
    name = 'DiJet',
    paths = (diJetSkimPath),
    content = skimContent.outputCommands,
    selectEvents = cms.untracked.PSet(),
    dataTier = cms.untracked.string('RAW-RECO')
    )

#####################

