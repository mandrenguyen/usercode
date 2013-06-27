import FWCore.ParameterSet.Config as cms

##############################################################################
def customiseHIRegitForPPrelVal(process):

    #### Additional output definition
    process.higenp.src = cms.InputTag("genParticles")
    #### Remove not-useful processes that cause errors
    process.validationHI.remove(process.hiBasicGenTest);
    process.validation.remove(process.iterativeCone5HiCleanedGenJets);
    
    #### RegIT: replace everywhere the names of the new muon collections
    from PhysicsTools.PatAlgos.tools.helpers import massSearchReplaceAnyInputTag
    massSearchReplaceAnyInputTag(process.dqmoffline_step, 'muons','remuons')
    massSearchReplaceAnyInputTag(process.dqmoffline_step, 'globalMuons','reglobalMuons')

    return process

