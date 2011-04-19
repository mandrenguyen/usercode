cvs co -d       MNguyen/ReReco  UserCode/MNguyen/ReReco

#Particle Flow, only check out if you want to fix muons
addpkg RecoParticleFlow/PFProducer
addpkg RecoParticleFlow/PFRootEvent

## hiGoodMergedTracks and the track analyzer
cvs co -d       edwenger/HiVertexAnalyzer UserCode/edwenger/HiVertexAnalyzer
cvs co -d       edwenger/HiTrkEffAnalyzer UserCode/edwenger/HiTrkEffAnalyzer
cvs co -d      Appeltel/PixelTrackAnalysis  UserCode/Appeltel/PixelTrackAnalysis
cvs co -d      Appeltel/PixelTracksRun2010  UserCode/Appeltel/PixelTracksRun2010

#Centrality
cvs co          CondFormats/HIObjects
cvs co          DataFormats/HeavyIonEvent
cvs co          RecoHI/HiCentralityAlgos
cvs co -d       CmsHi/Analysis2010 UserCode/CmsHi/Analysis2010

cp  MNguyen/ReReco/modifiedFiles/HiHackedAnalyticalTrackSelector.cc edwenger/HiTrkEffAnalyzer/src
cp  MNguyen/ReReco/modifiedFiles/PFBlockProducer.cc RecoParticle/PFProducer/plugins
cp  MNguyen/ReReco/modifiedFiles/PFBlockAlgo.h RecoParticle/PFProducer/interface
cp  MNguyen/ReReco/modifiedFiles/PFRootEventManager.cc RecoParticle/PFRootEvent/src
cp  MNguyen/ReReco/modifiedFiles/PFAlgo.cc RecoParticle/PFProducer/src
cp  MNguyen/ReReco/modifiedFiles/PFMuonAlgo.cc RecoParticle/PFProducer/src
	    

scram b -j4
