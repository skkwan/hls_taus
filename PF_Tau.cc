#include "PF_Tau.hpp"


/*
 * This creates one tau per id_eta x id_phi
 * TODO: 1.) Implememnt a 'sliding window' so that we can have some overlap between regions ()
 * 2.) Implement a data format that assumes the geometry will use 17 segments for eta, 17 segments for phi and the internal geometry is given via a different word
 *    -> Also check this proposed method against the cluster track linker
 * 3.) Implement Ales's (Amin's) sorter!
 */

void tau_three_prong_alg(track_t central_tracks[N_TRACKS], track_t three_prong_tau_cand[3], algo_config_t algo_config){
#pragma HLS PIPELINE II=6 
#pragma HLS ARRAY_PARTITION variable=central_tracks complete dim=0
#pragma HLS ARRAY_PARTITION variable=three_prong_tau_cand complete dim=0

  //These are both look up tables for the detector geometry
  uint8_t detector_grid_eta[11] = {1,2,4,5,6,7,8,9,10,11,12};
  uint8_t detector_grid_phi[11] = {1,2,4,5,6,7,8,9,10,11,12};
  //We can hold the 'fine-level' 3 prong cluster variable here
  uint8_t tau_grid_pt[11][11] = {0};
  uint8_t tau_grid_eta[11][11] = {0};
  uint8_t tau_grid_phi[11][11] = {0};
  uint8_t tau_grid_nprongs[11][11] = {0};

#pragma HLS ARRAY_PARTITION variable=detector_grid_eta complete dim=0
#pragma HLS ARRAY_PARTITION variable=detector_grid_phi complete dim=0
#pragma HLS ARRAY_PARTITION variable=tau_grid_pt complete dim=0
#pragma HLS ARRAY_PARTITION variable=tau_grid_eta complete dim=0
#pragma HLS ARRAY_PARTITION variable=tau_grid_phi complete dim=0


  for (int idx = 0; idx < N_TRACKS; idx++)//note, tracks are already sorted by PT
    {
 #pragma HLS UNROLL

      //int n_found_tracks = 0;
      track_t seedtrack = central_tracks[idx];

      if(seedtrack.et < algo_config.three_prong_seed)
	continue;
      //using eta = 5, phi = 5 keeps this to about 1 minute synthesis
      //using eta = 11, phi = 11 makes this take much longer
      for(int id_eta = 0; id_eta < 5; id_eta++){
#pragma HLS UNROLL
	for(int id_phi = 0; id_phi < 5; id_phi++){
#pragma HLS UNROLL
	  if(seedtrack.eta == detector_grid_eta[id_eta] && seedtrack.phi == detector_grid_phi[id_phi]){
	    if(tau_grid_nprongs[id_eta][id_phi] == 2 && 
	       //This OR statement is needed in case grid_eta - seedtrack_eta is negative.
	       ((tau_grid_eta[id_eta][id_phi] - seedtrack.eta )<2 || (  seedtrack.eta - tau_grid_eta[id_eta][id_phi] <2))){
	      tau_grid_pt[id_eta][id_phi] += seedtrack.et;
	      tau_grid_nprongs[id_eta][id_phi] = 3;
	    }
	    else if(tau_grid_nprongs[id_eta][id_phi] == 1 &&
		    ((tau_grid_eta[id_eta][id_phi] - seedtrack.eta )<2 || (  seedtrack.eta - tau_grid_eta[id_eta][id_phi] <2))){
	      tau_grid_pt[id_eta][id_phi] += seedtrack.et;
	      tau_grid_nprongs[id_eta][id_phi] = 2;
	    }
	    else if(tau_grid_nprongs[id_eta][id_phi] == 0){
	      tau_grid_pt[id_eta][id_phi] += seedtrack.et;
	      tau_grid_eta[id_eta][id_phi] = seedtrack.eta;
	      tau_grid_phi[id_eta][id_phi] = seedtrack.phi;
	      tau_grid_nprongs[id_eta][id_phi] = 1;
	    }
	  }
	}
      }
    }

  //this is included simply so we 'touch' a few of the arrays and
  if(tau_grid_pt[0][0]>tau_grid_pt[1][1] && tau_grid_pt[0][0] > tau_grid_pt[2][2] && tau_grid_pt[0][0] >tau_grid_pt[3][3]){
    three_prong_tau_cand[0].et = tau_grid_pt[0][0];
    three_prong_tau_cand[0].eta = tau_grid_eta[0][0];
    three_prong_tau_cand[0].phi = tau_grid_phi[0][0];
  }



  //sorter for '1 element', takes a long time to compile
  /* 
  for(int id_eta = 0; id_eta < 5; id_eta++){
#pragma HLS UNROLL
    for(int id_phi = 0; id_phi < 5; id_phi++){
#pragma HLS UNROLL
      if(tau_grid_pt[id_eta][id_phi]>three_prong_tau_cand[0].et){
	three_prong_tau_cand[0].et  = tau_grid_pt[id_eta][id_phi];
	three_prong_tau_cand[0].eta = tau_grid_eta[id_eta][id_phi];
	three_prong_tau_cand[0].phi = tau_grid_phi[id_eta][id_phi];
      }
    }
  }
  */
}


/*
void tau_three_prong_alg(track_t central_tracks[N_TRACKS], track_t three_prong_tau_cand[3], algo_config_t algo_config){

	for (int idx = 0; idx < N_TRACKS; idx++)	//note, tracks are already sorted by PT
	{
#pragma HLS UNROLL

		int n_found_tracks = 0;
		track_t seedtrack = central_tracks[idx];

		if(seedtrack.et < algo_config.three_prong_seed)
		  continue;

		three_prong_tau_cand[0] = seedtrack;
		n_found_tracks++;

		for (int jdx = 0; jdx < N_TRACKS; jdx++)
		{
		
		for(ap_uint<4> i = 0; i < N_TAUS ; i++){
		#pragma HLS UNROLL
		tau_cands_O[i] = tau_cands[i];
		
			if(Delta_R(seedtrack.eta, seedtrack.phi, temptrack.eta, temptrack.phi, algo_config.three_prong_delta_r)){// 0x6 corresponds to deltaR of 0.12
				if(n_found_tracks<3){
					n_found_tracks++;
					three_prong_tau_cand[2] = temptrack;
					break;
					}

				if(n_found_tracks<2){
					n_found_tracks++;
					three_prong_tau_cand[1] = temptrack;
					}


			}
		}
	}

}
*/
void merge_strip_algo(cluster_t cluster_1, pf_charged_t electron_1, cluster_t cluster_2, pf_charged_t electron_2, strip_t &strip, algo_config_t algo_config){
  cluster_t temp_cluster_1;
  temp_cluster_1.et = cluster_1.et + electron_1.et;
  temp_cluster_1.phi = weighted_avg_phi_c_p(cluster_1, electron_1);
  temp_cluster_1.eta = weighted_avg_eta_c_p(cluster_1, electron_1);

  cluster_t temp_cluster_2;
  temp_cluster_2.et = cluster_2.et + electron_2.et;
  temp_cluster_2.phi = weighted_avg_phi_c_p(cluster_2, electron_2);
  temp_cluster_2.eta = weighted_avg_eta_c_p(cluster_2, electron_2);

  // Requirement that it is a photon is checking if E/H is high for the cluster
  if(temp_cluster_1.is_photon > 0 && temp_cluster_2.is_photon > 0
     && delta_r_cluster(temp_cluster_1, temp_cluster_2) < algo_config.eg_strip_merge
     && strip.et < temp_cluster_1.et + temp_cluster_2.et){
  }
   //TODO: FINISH ISOLATION CALCULATION
    
}

// Simple function to check 
//1.) Check to see if candidate prong matches deltaR requirements to be a signal prong
//2.) Check to see if candidate prong can be put in place [2] or [2]
//3.) Check to see if the seed cand can be put in the isolation cone
//    If yes then increment isolation cone
 void find_tau_prongs(  ap_uint<3> &n_prongs_found, pf_charged_t prong_cands[3], pf_charged_t pf_charged_hadron_signal_cand, pf_charged_t seed_hadron, ap_uint<8> seed_cand_dr,ap_uint<12> iso_sum_charged_hadron, algo_config_t algo_config){
 #pragma HLS ARRAY_PARTITION variable=prong_cands complete dim=0
	   n_prongs_found++;
	   
	   //If charged hadron matches deltaR requirements then it can be one of the prongs for a 3 prong tau
	   if(seed_cand_dr < algo_config.three_prong_delta_r ){
	       //if this is the third prong found then place it at [2]
	     if(n_prongs_found==2){
	       prong_cands[2] = pf_charged_hadron_signal_cand;
	       n_prongs_found++;
	     }
	       //if this is the second prong found then place it at [2]
	     if(n_prongs_found==1){
	       prong_cands[1] = pf_charged_hadron_signal_cand;
	       n_prongs_found++;
	       
	     }
	   } // check to make sure that the delta_r is less than 5 -> Then the charged hadron goes in the isolation cone.
	   else if(seed_cand_dr < algo_config.isolation_delta_r ){
	     
	     // Sum Charged Hadron Isolation
	     iso_sum_charged_hadron += pf_charged_hadron_signal_cand.et;
	     
	   }// Less than isolation_delta_r
	   
 }
 // This needs the ieta and iphi from deltar function as an input
 void build_electron_grid(pf_charged_t electron_cand, ap_uint<8> seed_cand_dr, pf_charged_t seed_hadron, pf_charged_t electron_grid[N_TAUS][5][5], ap_uint<4> n_taus){
 #pragma HLS ARRAY_PARTITION variable=electron_grid complete dim=0

   ap_uint<7> index_eta = seed_cand_dr>>2;
   ap_uint<8> index_phi = seed_cand_dr>>2;
	 //keep from breaking things, This check is to be removed when code is validated
	 if(index_eta > 4 || index_phi > 4){
	   index_eta = 0;
	   index_phi = 0;
	 }
	 electron_grid[n_taus][index_eta][index_phi] = electron_cand;
 }

 // Check if each tau_phi slice is strip like
 // This module looks at each strip starting from the top cluster and going to the bottom
 //  -> Geometry outlined above for index
 // grid for strip creation is below
 // strips can be at most two towers in phi, potentially 2 towers in eta if crystals pass neighbors requirements.
 //   -----------
 //   |0|0|0|0|0|
 //   |1|1|1|1|1|
 //   |2|2|2|2|2|
 //   |3|3|3|3|3|
 //   |4|4|4|4|4|
 //   -----------
 void strip_alg(pftau_t &tau_cand, pf_charged_t electron_grid[5][5],  cluster_t neutral_clusters[N_CLUSTERS], algo_config_t algo_config){
 #pragma HLS ARRAY_PARTITION variable=electron_grid complete dim=0
 #pragma HLS ARRAY_PARTITION variable=neutral_clusters complete dim=0
 //#pragma HLS PIPELINE II=6

   ap_uint<7> tau_eta      = tau_cand.eta;
   ap_uint<1> tau_eta_side = tau_cand.eta_side;
   ap_uint<8> tau_phi      = tau_cand.phi;

     ap_uint<12> index[5];
 #pragma HLS ARRAY_PARTITION variable=index complete dim=0

     cluster_t neutral_cluster_grid[5][5];
 #pragma HLS ARRAY_PARTITION variable=neutral_cluster_grid complete dim=0

     for(ap_uint<3> i = 0; i<5; i++){
 #pragma HLS UNROLL
	   for(ap_uint<3> j = 0; j<5; j++){
 #pragma HLS UNROLL
	     ap_uint<7> grid_eta = tau_eta + i>>2;
	     ap_uint<7> grid_phi = tau_phi + j>>2;
		   neutral_cluster_grid[i][j] = find_matched_cluster(neutral_clusters, grid_eta, grid_phi);
	   }
     }

     strip_t temp_strip[5];    
 #pragma HLS ARRAY_PARTITION variable=temp_strip complete dim=0
     strip_t final_strip;

     cluster_t cluster;
     pf_charged_t charged;

     ap_uint<8> index1; ap_uint<8> index2;

     for(ap_uint<3> i = 0; i<5; i++){
 #pragma HLS UNROLL
       for(ap_uint<3> j = 0; j<4; j++){
 #pragma HLS UNROLL
	   ap_uint<3> jp = j+1;
	   merge_strip_algo(neutral_cluster_grid[i][j], electron_grid[i][j], neutral_cluster_grid[i][jp], electron_grid[i][jp], temp_strip[i], algo_config);
       }
     }

     final_strip = temp_strip[0];
     for(ap_uint<3> j = 1; j < 5; j++){
 #pragma HLS UNROLL
       //first check if strip j is greater than final strip
       if(temp_strip[j].et > final_strip.et){
	      final_strip = temp_strip[j];
       }

       //merge strips if two are close by
       if(delta_r_strip( temp_strip[j], temp_strip[j-1]) < algo_config.max_neighbor_strip_dist){
	     if(temp_strip[j].et + temp_strip[j-1].et > final_strip.et){

		 ap_uint<12> et  = temp_strip[j].et + temp_strip[j-1].et;
		 final_strip.et  = et ;
		 final_strip.eta = weighted_avg_eta_s_s(temp_strip[j], temp_strip[j-1]);
		 final_strip.phi = weighted_avg_phi_s_s(temp_strip[j], temp_strip[j-1]);

	     }
       }
     }

   //find 1 prong pi0's by combining the 1 prong and the pi0's
     if(tau_cand.tau_type == 0 && final_strip.et > algo_config.min_strip){
       //take care of eta_side
       ap_uint<12> temp_tau_et;
       ap_uint<8>  temp_tau_eta;
       ap_uint<8>  temp_tau_phi;
       temp_tau_et  = tau_cand.et + final_strip.et;
       temp_tau_eta = weighted_avg_eta_t_s(tau_cand,final_strip);
       temp_tau_phi = weighted_avg_phi_t_s(tau_cand,final_strip);

       tau_cand.et       = temp_tau_et;
       tau_cand.eta      = temp_tau_eta;
       tau_cand.phi      = temp_tau_phi;
       tau_cand.tau_type = 1;
     }
 }




 ap_uint<1> Delta_R(ap_uint<8> eta_1, ap_uint<8> phi_1, ap_uint<8> eta_2, ap_uint<8> phi_2, ap_uint<8> maximum_delta_R){
	 ap_uint<1> output;
	 ap_uint<8> delta_eta = 0;
	 ap_uint<8> delta_phi = 0;
	 if(eta_2>eta_1)
		 delta_eta = eta_2 - eta_1;
	 else
		 delta_eta = eta_1 - eta_2;

	 if(phi_2>phi_1)
		 delta_phi = phi_2 - phi_1;
	 else
		 delta_phi = phi_1 - phi_2;

	 if(delta_phi + delta_eta < maximum_delta_R)
		 output = 1;
	 else
		 output = 0;

	 return output;
 }

 ap_uint<10> delta_r_strip(strip_t strip1, strip_t strip2){
   ap_uint<8> delta_eta = 0;
   ap_uint<8> delta_phi = 0;
   if(strip1.eta > strip2.eta)
     delta_eta = strip1.eta = strip2.eta;

   if(strip1.eta > strip2.eta)
     delta_eta = strip1.eta = strip2.eta;
   else
     delta_eta = strip2.eta = strip1.eta;

   if(strip1.phi > strip2.phi)
     delta_phi = strip1.phi = strip2.phi;
   else
     delta_phi = strip2.phi = strip1.phi;

   return(delta_eta + delta_phi);
 }

 ap_uint<10> delta_r_cluster(cluster_t cluster1, cluster_t cluster2){
   ap_uint<8> delta_eta = 0;
   ap_uint<8> delta_phi = 0;
   if(cluster1.eta > cluster2.eta)
     delta_eta = cluster1.eta = cluster2.eta;

   if(cluster1.eta > cluster2.eta)
     delta_eta = cluster1.eta = cluster2.eta;
   else
     delta_eta = cluster2.eta = cluster1.eta;

   if(cluster1.phi > cluster2.phi)
     delta_phi = cluster1.phi = cluster2.phi;
   else
     delta_phi = cluster2.phi = cluster1.phi;

   return(delta_eta + delta_phi);
 }

 //fix me - check if enough bits are allocated during operations
 ap_uint<7> weighted_avg_eta_t_s(pftau_t strip1, strip_t strip2){

   ap_uint<7> output_eta = (strip1.eta + strip2.eta)>>1;
   return output_eta;
 }

 ap_uint<8> weighted_avg_phi_t_s(pftau_t strip1, strip_t strip2){
   //ap_uint<12> phi1 = strip1.phi*strip1.et;
   //ap_uint<12> phi2 = strip2.phi*strip2.et;
   //ap_uint<12> sum_et = strip1.et + strip2.et;
   //ap_uint<7> output_phi = (phi1 + phi2)/sum_et;

   ap_uint<7> output_phi = (strip1.phi+strip2.phi)>>1;
   return output_phi;
 }

 //fix me - check if enough bits are allocated during operations
 ap_uint<7> weighted_avg_eta_s_s(strip_t strip1, strip_t strip2){
   //ap_uint<12> eta1 = strip1.eta*strip1.et;
   //ap_uint<12> eta2 = strip2.eta*strip2.et;
   //ap_uint<12> sum_et = strip1.et + strip2.et;
   //ap_uint<7> output_eta = (eta1 + eta2)/sum_et;
   ap_uint<7> output_eta = (strip1.eta + strip2.eta)>>1;
   return output_eta;
 }

 ap_uint<8> weighted_avg_phi_s_s(strip_t strip1, strip_t strip2){
   //ap_uint<12> phi1 = strip1.phi*strip1.et;
   //ap_uint<12> phi2 = strip2.phi*strip2.et;
   //ap_uint<12> sum_et = strip1.et + strip2.et;
   //ap_uint<7> output_phi = (phi1 + phi2)/sum_et;
   ap_uint<7> output_phi  = (strip1.phi+ strip2.phi)>>1;
   return output_phi;
 }

 ap_uint<7> weighted_avg_eta_c_p(cluster_t cluster1, pf_charged_t cluster2){
   //ap_uint<12> eta1 = cluster1.eta*cluster1.et;
   //ap_uint<12> eta2 = cluster2.eta*cluster2.et;
   //ap_uint<12> sum_et = cluster1.et + cluster2.et;
   //ap_uint<7> output_eta = (eta1 + eta2)/sum_et;

   ap_uint<7> output_eta = (cluster1.eta + cluster2.eta)>>1;
   return output_eta;
 }

 ap_uint<8> weighted_avg_phi_c_p(cluster_t cluster1, pf_charged_t cluster2){
   //ap_uint<12> phi1 = cluster1.phi*cluster1.et;
   //ap_uint<12> phi2 = cluster2.phi*cluster2.et;
   //ap_uint<12> sum_et = cluster1.et + cluster2.et;
   ap_uint<8> output_phi = (cluster1.phi + cluster2.phi)>>1;
   return output_phi;
 }

 ap_uint<7> weighted_avg_eta_c_c(cluster_t cluster1, cluster_t cluster2){
   //ap_uint<12> eta1 = cluster1.eta*cluster1.et;
   //ap_uint<12> eta2 = cluster2.eta*cluster2.et;
   // ap_uint<12> sum_et = cluster1.et + cluster2.et;
   // ap_uint<7> output_eta = (eta1 + eta2)/sum_et;

   //return output_eta;

   ap_uint<7> output_eta = (cluster1.eta + cluster2.eta)>>1;
   return output_eta;
 }

 ap_uint<8> weighted_avg_phi_c_c(cluster_t cluster1, cluster_t cluster2){
   //ap_uint<12> phi1 = cluster1.phi*cluster1.et;
   //ap_uint<12> phi2 = cluster2.phi*cluster2.et;
   //ap_uint<12> sum_et = cluster1.et + cluster2.et;
   //ap_uint<7> output_phi = (phi1 + phi2)/sum_et;

   //return output_phi;

   ap_uint<8> output_phi = (cluster1.phi + cluster2.phi)>>1;
   return output_phi;
 }

 ap_uint<7> weighted_avg_eta_p_s(pf_charged_t strip1, strip_t strip2){
   //ap_uint<12> eta1 = strip1.eta*strip1.et;
   //ap_uint<12> eta2 = strip2.eta*strip2.et;
   //ap_uint<12> sum_et = strip1.et + strip2.et;
   //ap_uint<7> output_eta = (eta1 + eta2)/sum_et;

   ap_uint<7> output_eta = (strip1.eta + strip2.eta)>>1;
   return output_eta;
 }

 ap_uint<8> weighted_avg_phi_p_s(pf_charged_t strip1, strip_t strip2){
   //ap_uint<12> phi1 = strip1.phi*strip1.et;
   //ap_uint<12> phi2 = strip2.phi*strip2.et;
   //ap_uint<12> sum_et = strip1.et + strip2.et;
   //ap_uint<7> output_phi = (phi1 + phi2)/sum_et;
   ap_uint<7> output_phi = (strip1.phi + strip2.phi)>>1;
   return output_phi;
 }

 ap_uint<7> weighted_avg_eta_c_s(cluster_t cluster1, strip_t strip2){
   //ap_uint<12> eta1 = cluster1.eta*cluster1.et;
   //ap_uint<12> eta2 = strip2.eta*strip2.et;
   //ap_uint<12> sum_et = cluster1.et + strip2.et;
   //ap_uint<7> output_eta = (eta1 + eta2)/sum_et;
   ap_uint<7> output_eta = (cluster1.eta + strip2.eta)>>1;
   return output_eta;
 }

 ap_uint<8> weighted_avg_phi_c_s(cluster_t cluster1, strip_t strip2){
   //ap_uint<12> phi1 = cluster1.phi*cluster1.et;
   //ap_uint<12> phi2 = strip2.phi*strip2.et;
   //ap_uint<12> sum_et = cluster1.et + strip2.et;
   //ap_uint<7> output_phi = (phi1 + phi2)/sum_et;
   ap_uint<7> output_phi = (cluster1.phi+strip2.phi)>>1;
   return output_phi;
 }

 ap_uint<7> weighted_avg_eta_p_p_p(pf_charged_t pf1, pf_charged_t pf2, pf_charged_t pf3){
   //ap_uint<12> eta1 = pf1.eta*pf1.et;
   //ap_uint<12> eta2 = pf2.eta*pf2.et;
   //ap_uint<12> eta3 = pf3.eta*pf3.et;
   //ap_uint<12> sum_et = pf1.et + pf2.et + pf3.et;
   //ap_uint<7> output_eta = (eta1 + eta2 + eta3)/sum_et;

   ap_uint<7> output_eta = (pf1.eta + ((pf2.eta + pf3.eta)>>1 ))>>1;
   return output_eta;
 }

 ap_uint<8> weighted_avg_phi_p_p_p(pf_charged_t pf1, pf_charged_t pf2, pf_charged_t pf3){
   //ap_uint<12> phi1 = pf1.phi*pf1.et;
   //ap_uint<12> phi2 = pf2.phi*pf2.et;
   //ap_uint<12> phi3 = pf3.phi*pf3.et;
   //ap_uint<12> sum_et = pf1.et + pf2.et + pf3.et;
   ap_uint<8> output_phi = (pf1.phi + ((pf2.phi + pf3.phi)>>1))>>1;

   return output_phi;
 }

 ap_uint<8> delta_r_pf_charged(pf_charged_t pf_1, pf_charged_t pf_2){
	 ap_uint<8> delta_eta = 0;
	 ap_uint<8> delta_phi = 0;
	 ap_uint<8> eta_1 = pf_1.eta;
	 ap_uint<8> eta_2 = pf_2.eta;
	 ap_uint<8> phi_1 = pf_1.phi;
	 ap_uint<8> phi_2 = pf_2.phi;

	 if(eta_2>eta_1)
		 delta_eta = eta_2 - eta_1;
	 else
		 delta_eta = eta_1 - eta_2;

	 if(phi_2>phi_1)
		 delta_phi = phi_2 - phi_1;
	 else
		 delta_phi = phi_1 - phi_2;

	 return (delta_eta + delta_phi);
 }

 ap_uint<8> delta_r_c_p(cluster_t pf_1, pf_charged_t pf_2){
	 ap_uint<8> delta_eta = 0;
	 ap_uint<8> delta_phi = 0;
	 ap_uint<8> eta_1 = pf_1.eta;
	 ap_uint<8> eta_2 = pf_2.eta;
	 ap_uint<8> phi_1 = pf_1.phi;
	 ap_uint<8> phi_2 = pf_2.phi;

	 if(eta_2>eta_1)
		 delta_eta = eta_2 - eta_1;
	 else
		 delta_eta = eta_1 - eta_2;

	 if(phi_2>phi_1)
		 delta_phi = phi_2 - phi_1;
	 else
		 delta_phi = phi_1 - phi_2;

	 return (delta_eta + delta_phi);
 }


 ap_uint<7> ieta_diff(pf_charged_t cand_1, pf_charged_t cand_2){
   //fix me for eta side
   if(cand_1.eta > cand_2.eta){
     return(cand_1.eta - cand_2.eta);
   }

    return(cand_2.eta - cand_1.eta);

 }

 ap_uint<8> iphi_diff(pf_charged_t cand_1, pf_charged_t cand_2){
   //fix me for phi side
   if(cand_1.phi > cand_2.phi){
     return(cand_1.phi - cand_2.phi);
   }

     return(cand_2.phi - cand_1.phi);

 }
 /*
  * Find the index given the crystal location input
  * First divide by 5 for 5 crystals per tower
  * Then match the detector geometry as seen above
  */

 ap_uint<12> find_the_index_crys( ap_uint<7> eta, ap_uint<1> eta_side, ap_uint<8> phi){
		 // First go from cyrstal to tower
		 //eta = eta/5;
		 //phi = phi/5;
	 return 5;
		 ap_uint<5> eta_calc = eta/5;
		 ap_uint<5> phi_calc = phi/5;
	  //temporary caution while code is integrated
	  //the max number of towers is 20... put this in a define
		 ap_uint<12> index;
		 /*
		if(eta > 20)
		    return 0;

		if(eta == 0 && phi == 0)
		    return N_CLUSTERS-1;

		if(eta == 0 && phi == 1)
		    return N_CLUSTERS-2;
 */
		if(eta_side < 1){
		    index = (20-eta_calc)*72+phi_calc-2;
		}
		else{
		    index = (20+eta_calc)*72+phi_calc;
		}

		return index;
 }


 cluster_t find_matched_cluster(cluster_t neutral_clusters[N_CLUSTERS], ap_uint<8> eta_1, ap_uint<8> phi_1){
 #pragma HLS ARRAY_PARTITION variable=neutral_clusters complete dim=0
	 cluster_t cluster_to_return;
	 cluster_to_return = neutral_clusters[0];
   for(ap_uint<9> i =0; i<N_CLUSTERS; i++){
 #pragma HLS UNROLL    
     if(Delta_R(neutral_clusters[i].eta, neutral_clusters[i].phi, eta_1, phi_1, 4))
	 cluster_to_return = neutral_clusters[i];
   }
   //fix me
   return cluster_to_return;

 }

 // Offset by -2 in eta and -2 in phi, special geometry to grab the grid
 // IMPLEMENTE ME We are now making the cluster grid 2^7 in phi (128)
 ap_uint<12> find_index_crys_offset( ap_uint<7> eta, ap_uint<1> eta_side, ap_uint<8> phi, ap_uint<3> eta_offset, ap_uint<3> phi_offset){
				 // First go from cyrstal to tower
   ///IMPLEMENT ME this needs to take into account the edges, i.e. return 0 if on the edge
   //#pragma HLS PIPELINE II=1
   if(eta > 2 ){
     eta = (eta + eta_offset - 2)>>2;
   }
   else{
     eta = 0;
   }

   if(phi> 2){
     phi = (phi + phi_offset - 2)>>2;
   }
   else{
     phi = 0;
   }
   //the max number of towers is 20... put this in a define
   ap_uint<12> index;
   if(eta > 20)
     return 0;

   if(eta == 0 && phi == 0)
     return N_CLUSTERS-1;
   
   if(eta == 0 && phi == 1)
     return N_CLUSTERS-2;
   
   if(eta_side < 1)
     index = (20-eta)<<7+phi-2;
   else
     index = (20+eta)<<7+phi;
   
   return index;
 }
