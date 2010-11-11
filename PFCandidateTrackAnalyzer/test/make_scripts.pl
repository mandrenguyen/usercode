#!/usr/bin/perl



#@files = `ls /pnfs/cmsaf.mit.edu/t2bat/cms/store/himc/Fall10/Hydjet_Quenched_MinBias_2760GeV/GEN-SIM-RECODEBUG/Pyquen_DiJet_Pt80_START39_V4HI-v1/*/*.root`;
#@files = `ls /pnfs/cmsaf.mit.edu/t2bat/cms/store/himc/Fall10/Hydjet_Quenched_MinBias_2760GeV/GEN-SIM-RECODEBUG/Pyquen_DiJet_Pt80_MC_38Y_V12-v2/0008/*.root`;
@files = `ls /pnfs/cmsaf.mit.edu/t2bat/cms/store/user/davidlw/Hydjet_Quenched_MinBias_2760GeV/Fall10-START39_V4HI-391redigi-v5-GEN-SIM-RECO/597653989fad95507f68df8cea7f0ee4/hiReco_RAW2DIGI_RECO_*.root`;

chomp(@files);

$i=0;
$j=0;

foreach $file (@files)
{

    #print $file."\n";


    print $outputfile;


    #if(`ls /net/hisrv0001/home/mnguyen/scratch/PatFilesWithPF_MyL2L3/AMPT_Pyquen_UnquenchedDiJet_Pt80/Pat_$i.root`)    
    #if(`ls /net/hisrv0001/home/mnguyen/scratch/PatFilesWithPF_MyL2L3/Hydjet_Pyquen_UnquenchedDiJet_Pt80/Pat_$i.root`)
    #{
    #}
    #else
    #{
	
	
    $outfile="/net/hisrv0001/home/mnguyen/scratch/CMSSW_3_9_1_patch1/src/edwenger/TrkEffAnalyzer/test/exec/run_".$j++.".sh";
    open(OUT,"> $outfile") or die "$outfile\n";
    print OUT "source ~/osg_cmssw_set_basic.sh\n";
    print OUT "eval `scramv1 runtime -sh`\n";
    print OUT "cmsRun analyzePFCandidatesTrack_cfg.py files=dcache:".$file." output=dcache:/pnfs/cmsaf.mit.edu/t2bat/cms/store/user/mnguyen/PFTrkEffAnalyzer/Hydjet_Quenched_MinBias_2760GeV_Fall10-START39_V4HI-391redigi-v5/pfntuple_hiSelectedTracks_".$i.".root\n";
    #print OUT "cmsRun hiAnalyzePFCandidates_cfg.py files=dcache:".$file." output=pfntuple_".$i.".root\n";
    
    #}
    
    $i++;
}

`chmod 777 exec/*`;
