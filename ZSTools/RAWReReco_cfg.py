# Auto generated configuration file
# using: 
# Revision: 1.232.2.6.2.2 
# Source: /cvs/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: test -n 2 -s RAW2DIGI,RECO,DQM,ALCA:HcalCalMinBias,REPACK:DigiToSplitRawRepack --eventcontent RECO,REPACKRAW,DQM --datatier RECO,RAW,DQM --data --scenario HeavyIons --no_exec --conditions GR_R_39X_V6B::All

import FWCore.ParameterSet.VarParsing as VarParsing

ivars = VarParsing.VarParsing('standard')
ivars.files = 'dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/yetkin/mixedSimPionRD_Cent_0_2p5_RAW///Mix_1.root'

ivars.output = 'bambu.root'
ivars.maxEvents = -1

ivars.register ('randomNumber',
                mult=ivars.multiplicity.singleton,
                info="for testing")

ivars.randomNumber=5
ivars.parseArguments()


import FWCore.ParameterSet.Config as cms

process = cms.Process('RECO')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('Configuration.StandardSequences.RawToDigi_cff')
process.load('Configuration.StandardSequences.ReconstructionHeavyIons_cff')
process.load('Configuration.StandardSequences.EndOfProcess_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.load('Configuration.EventContent.EventContentHeavyIons_cff')

process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.1 $'),
    annotation = cms.untracked.string('test nevts:2'),
    name = cms.untracked.string('PyReleaseValidation')
)
process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)

# Input source
process.source = cms.Source("PoolSource",
                            fileNames= cms.untracked.vstring(ivars.files)   
                            #  fileNames = cms.untracked.vstring('file:~/MattMix/Mix_9.root'
                            #                                   #'file:ivan_95_1_VOF.root'
                            #                                   )
                            )

process.options = cms.untracked.PSet(
    
    )

# Output definition

process.RECOoutput = cms.OutputModule("PoolOutputModule",
    splitLevel = cms.untracked.int32(0),
    outputCommands = process.FEVTDEBUGHLTEventContent.outputCommands,
                                      fileName = cms.untracked.string(ivars.output),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string(''),
        dataTier = cms.untracked.string('RECO')
    )
)




# Other statements
#process.GlobalTag.globaltag = 'GR_R_39X_V6B::All'
process.GlobalTag.globaltag = 'START42_V13::All'
#process.GlobalTag.globaltag = 'GR_R_42_V18::All'

# Path and EndPath definitions
process.raw2digi_step = cms.Path(process.RawToDigi)
process.reconstruction_step = cms.Path(process.reconstructionHeavyIons)
process.endjob_step = cms.EndPath(process.endOfProcess)
process.RECOoutput_step = cms.EndPath(process.RECOoutput)



# Schedule definition
process.schedule = cms.Schedule(process.raw2digi_step,process.reconstruction_step,process.endjob_step,process.RECOoutput_step)


def customiseBeamSpot(process):
    process.GlobalTag.toGet = cms.VPSet(
        cms.PSet(record = cms.string("BeamSpotObjectsRcd"),
                 tag = cms.string("Realistic2.76ATeVCollisions_STARTUP_v0_mc"),
                 connect = cms.untracked.string("frontier://FrontierProd/CMS_COND_31X_BEAMSPOT")
                 )
        )
    return process

def customiseSiStripConditions(process):
    process.stripConditions = cms.ESSource("PoolDBESSource",
                                           process.CondDBSetup,
                                           timetype = cms.untracked.string('runnumber'),
                                           connect = cms.string('frontier://FrontierProd/CMS_COND_31X_STRIP'),
                                           toGet = cms.VPSet(
        cms.PSet(
        record = cms.string('SiStripNoisesRcd'),
        tag = cms.string('SiStripNoise_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripPedestalsRcd'),
        tag = cms.string('SiStripPedestals_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripFedCablingRcd'),
        tag = cms.string('SiStripFedCabling_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripBadChannelRcd'),
        tag = cms.string('SiStripBadChannel_FromOnline_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripLatencyRcd'),
        tag = cms.string('SiStripLatency_GR10_v2_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripThresholdRcd'),
        tag = cms.string('SiStripThreshold_GR10_v1_hlt')
        ),
        cms.PSet(
        record = cms.string('SiStripBadFiberRcd'),
        tag = cms.string('SiStripBadChannel_FromOfflineCalibration_GR10_v2_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripBadModuleRcd'),
        tag = cms.string('SiStripBadChannel_FromEfficiencyAnalysis_GR10_v1_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripConfObjectRcd'),
        tag = cms.string('SiStripShiftAndCrosstalk_GR10_v1_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripLorentzAngleRcd'),
        tag = cms.string('SiStripLorentzAngle_GR10_v1_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripApvGain2Rcd'),
        tag = cms.string('SiStripApvGain_FromParticles_GR10_v2_offline')
        ),
        cms.PSet(
        record = cms.string('SiStripApvGainRcd'),
        tag = cms.string('SiStripApvGain_GR10_v1_hlt')
        )
        )
                                           )
    
    process.es_prefer_strips = cms.ESPrefer("PoolDBESSource","stripConditions")
    return process



def customiseDummyVtx(process):
    # Dummy GEN -> RECO vertex
    process.hiSelectedVertex = cms.EDProducer("GenToRecoVtxProducer",
                                              signalLabel=cms.InputTag("generator"),                                            
                                              dummyVtxError=cms.vdouble(0.0,0.0,0.0),
                                              useBkgdVtxError=cms.bool(True),
                                              bkgdVtxLabel=cms.InputTag("hiSelectedVertex"),
                                              smearVtx=cms.bool(True)
                                              )
    return process

def customiseBeamRaw(process):
    customiseBeamSpot(process)
    customiseSiStripConditions(process)
    customiseDummyVtx(process)
    return process

process = customiseBeamRaw(process)
