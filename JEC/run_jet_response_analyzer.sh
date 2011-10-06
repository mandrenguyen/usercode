jet_response_analyzer_x  fine_binning_ptRebin_noHF.config -input ../combinePtHatBins/merged_pthat_pp_hiGoodTightTracks_Escale_tuneZ2.root -useweight true -algs ak3pf ak4pf ak5pf -output jra_PFTowers_hiGoodTightTracks_tuneZ2.root
jet_response_analyzer_x  fine_binning_ptRebin.config -input ../combinePtHatBins/merged_pthat_pp_hiGoodTightTracks_Escale_tuneZ2.root -useweight true -algs ic5calo -output jra_Calo_hiGoodTightTracks_tuneZ2.root

