# include "Element.hpp"
//void profile_tier_level(profilePresentFlag, maxNumSubLayersMinus1) {
//    if (profilePresentFlag) {
//        general_profile_space u(2);
//        general_tier_flag u(1);
//        general_profile_idc u(5);
//        for (j = 0; j < 32; j++)
//            general_profile_compatibility_flag[j] u(1);
//        general_progressive_source_flag       u(1);
//        general_interlaced_source_flag        u(1);
//        general_non_packed_constraint_flag    u(1);
//        general_frame_only_constraint_flag    u(1);
//        if (general_profile_idc = = 4 || general_profile_compatibility_flag[4] ||
//            general_profile_idc = = 5 || general_profile_compatibility_flag[5] ||
//            general_profile_idc = = 6 || general_profile_compatibility_flag[6] ||
//            general_profile_idc = = 7 || general_profile_compatibility_flag[7]
//            ) {
//            /* The number of bits in this syntax structure is not affected by this condition */
//            general_max_12bit_constraint_flag         u(1);
//            general_max_10bit_constraint_flag         u(1);
//            general_max_8bit_constraint_flag          u(1);
//            general_max_422chroma_constraint_flag     u(1);
//            general_max_420chroma_constraint_flag     u(1);
//            general_max_monochrome_constraint_flag    u(1);
//            general_intra_constraint_flag             u(1);
//            general_one_picture_only_constraint_flag  u(1);
//            general_lower_bit_rate_constraint_flag    u(1);
//            general_reserved_zero_34bits              u(34);
//        }
//        else
//            general_reserved_zero_43bits u(43);
//        if ((general_profile_idc >= 1 && general_profile_idc <= 5) ||
//            general_profile_compatibility_flag[1] || general_profile_compatibility_flag[2] ||
//            general_profile_compatibility_flag[3] || general_profile_compatibility_flag[4] ||
//            general_profile_compatibility_flag[5]
//            )
//            /* The number of bits in this syntax structure is not affected by this condition */
//            general_inbld_flag u(1);
//        else
//            general_reserved_zero_bit u(1);
//    }
//    general_level_idc u(8);
//    for (i = 0; i < maxNumSubLayersMinus1; i++) {
//        sub_layer_profile_present_flag[i] u(1);
//        sub_layer_level_present_flag[i] u(1);
//    }
//    if (maxNumSubLayersMinus1 > 0)
//        for (i = maxNumSubLayersMinus1; i < 8; i++)
//            reserved_zero_2bits[i] u(2);
//    for (i = 0; i < maxNumSubLayersMinus1; i++) {
//        if (sub_layer_profile_present_flag[i]) {
//            sub_layer_profile_space[i] u(2);
//            sub_layer_tier_flag[i] u(1);
//            sub_layer_profile_idc[i] u(5);
//            for (j = 0; j < 32; j++)
//                sub_layer_profile_compatibility_flag[i][j] u(1);
//            sub_layer_progressive_source_flag[i] u(1);
//            sub_layer_interlaced_source_flag[i] u(1);
//            sub_layer_non_packed_constraint_flag[i] u(1);
//            sub_layer_frame_only_constraint_flag[i] u(1);
//            if (sub_layer_profile_idc[i] = = 4 || sub_layer_profile_compatibility_flag[i][4] ||
//                sub_layer_profile_idc[i] = = 5 || sub_layer_profile_compatibility_flag[i][5] ||
//                sub_layer_profile_idc[i] = = 6 || sub_layer_profile_compatibility_flag[i][6] ||
//                sub_layer_profile_idc[i] = = 7 || sub_layer_profile_compatibility_flag[i][7])
//            {
//                /* The number of bits in this syntax structure is not affected by this condition */
//                sub_layer_max_12bit_constraint_flag       [i] u(1);
//                sub_layer_max_10bit_constraint_flag       [i] u(1);
//                sub_layer_max_8bit_constraint_flag        [i] u(1);
//                sub_layer_max_422chroma_constraint_flag   [i] u(1);
//                sub_layer_max_420chroma_constraint_flag   [i] u(1);
//                sub_layer_max_monochrome_constraint_flag  [i] u(1);
//                sub_layer_intra_constraint_flag           [i] u(1);
//                sub_layer_one_picture_only_constraint_flag[i] u(1);
//                sub_layer_lower_bit_rate_constraint_flag  [i] u(1);
//                sub_layer_reserved_zero_34bits            [i] u(34);
//            }
//            else
//                sub_layer_reserved_zero_43bits[i] u(43);
//            if ((sub_layer_profile_idc[i] >= 1 && sub_layer_profile_idc[i] <= 5) ||
//                sub_layer_profile_compatibility_flag[1] ||
//                sub_layer_profile_compatibility_flag[2] ||
//                sub_layer_profile_compatibility_flag[3] ||
//                sub_layer_profile_compatibility_flag[4] ||
//                sub_layer_profile_compatibility_flag[5]
//                )
//                /* The number of bits in this syntax structure is not affected by this condition */
//                sub_layer_inbld_flag[i] u(1);
//            else
//                sub_layer_reserved_zero_bit[i] u(1);
//        }
//        if (sub_layer_level_present_flag[i])
//            sub_layer_level_idc               [i] u(8);
//    }
//}
void Profile_tier_level::parse(BitStream& bs, uint8_t profilePresentFlag, size_t maxNumSubLayersMinus1) {
    bs.flush_buf();
    if (profilePresentFlag != 0) {
        general_profile_space   = bs.u(2);
        general_tier_flag       = bs.u(1);
        general_profile_idc = bs.u(5);
        bs.flush_buf();
        for (size_t j = 0; j < 32; ++j) {
            general_profile_compatibility_flag[j] = bs.u(1);
        }
        bs.flush_buf();
        general_progressive_source_flag     = bs.u(1);
        general_interlaced_source_flag      = bs.u(1);
        general_non_packed_constraint_flag  = bs.u(1);
        general_frame_only_constraint_flag  = bs.u(1);
        if (general_profile_idc == 4 || general_profile_compatibility_flag[4] ||
            general_profile_idc == 5 || general_profile_compatibility_flag[5] ||
            general_profile_idc == 6 || general_profile_compatibility_flag[6] ||
            general_profile_idc == 7 || general_profile_compatibility_flag[7]
        ) {
            general_max_12bit_constraint_flag           = bs.u(1); 
            general_max_10bit_constraint_flag           = bs.u(1); 
            general_max_8bit_constraint_flag            = bs.u(1); 
            general_max_422chroma_constraint_flag       = bs.u(1); 
            general_max_420chroma_constraint_flag       = bs.u(1); 
            general_max_monochrome_constraint_flag      = bs.u(1); 
            general_intra_constraint_flag               = bs.u(1); 
            general_one_picture_only_constraint_flag    = bs.u(1); 
            general_lower_bit_rate_constraint_flag      = bs.u(1);
            // general_reserved_zero_bits                  = bs.u(34);
            bs.bitPos += 34;
            bs.flush_buf();
        }
        else {
            bs.bitPos += 43;
            bs.flush_buf();
            //general_reserved_zero_bits = bs.u(43);
        }
        if ((general_profile_idc >= 1 && general_profile_idc <= 5) ||
            general_profile_compatibility_flag[1] || general_profile_compatibility_flag[2] ||
            general_profile_compatibility_flag[3] || general_profile_compatibility_flag[4] ||
            general_profile_compatibility_flag[5]
        ) {
            general_inbld_flag = bs.u(1);
        }
        else {
            // general_reserved_zero_bit u(1);
            bs.u(1);
        }
    }
    general_level_idc = bs.u(8);
    for (size_t i = 0; i < maxNumSubLayersMinus1; ++i) {
        sub_layer_profile_present_flag[i] = bs.u(1);
        sub_layer_level_present_flag[i] = bs.u(1);
        bs.flush_buf();
    }
    if (maxNumSubLayersMinus1 > 0){
        for (size_t i = maxNumSubLayersMinus1; i < 8; i++) {
            reserved_zero_2bits[i] = bs.u(2);
            bs.flush_buf();
        }
    }
    for (size_t i = 0; i < maxNumSubLayersMinus1; ++i) {
        if (sub_layer_profile_present_flag[i]) {
            sub_layer_profile[i].sub_layer_profile_space = bs.u(2);
            sub_layer_profile[i].sub_layer_tier_flag = bs.u(1);
            sub_layer_profile[i].sub_layer_profile_idc = bs.u(5);
            bs.flush_buf();
            for (size_t j = 0; j < 32; ++j) {
                sub_layer_profile[i].sub_layer_profile_compatibility_flag[j] = bs.u(1);
            }
            bs.flush_buf();
            sub_layer_profile[i].sub_layer_progressive_source_flag = bs.u(1);
            sub_layer_profile[i].sub_layer_interlaced_source_flag = bs.u(1);
            sub_layer_profile[i].sub_layer_non_packed_constraint_flag = bs.u(1);
            sub_layer_profile[i].sub_layer_frame_only_constraint_flag = bs.u(1);
            if (sub_layer_profile[i].sub_layer_profile_idc == 4 || sub_layer_profile[i].sub_layer_profile_compatibility_flag[4] ||
                sub_layer_profile[i].sub_layer_profile_idc == 5 || sub_layer_profile[i].sub_layer_profile_compatibility_flag[5] ||
                sub_layer_profile[i].sub_layer_profile_idc == 6 || sub_layer_profile[i].sub_layer_profile_compatibility_flag[6] ||
                sub_layer_profile[i].sub_layer_profile_idc == 7 || sub_layer_profile[i].sub_layer_profile_compatibility_flag[7]
            ) {
                sub_layer_profile[i].sub_layer_max_12bit_constraint_flag = bs.u(1);
                sub_layer_profile[i].sub_layer_max_10bit_constraint_flag = bs.u(1);
                sub_layer_profile[i].sub_layer_max_8bit_constraint_flag = bs.u(1);
                sub_layer_profile[i].sub_layer_max_422chroma_constraint_flag    = bs.u(1);
                sub_layer_profile[i].sub_layer_max_420chroma_constraint_flag    = bs.u(1);
                sub_layer_profile[i].sub_layer_max_monochrome_constraint_flag   = bs.u(1);
                sub_layer_profile[i].sub_layer_intra_constraint_flag            = bs.u(1);
                sub_layer_profile[i].sub_layer_one_picture_only_constraint_flag = bs.u(1);
                sub_layer_profile[i].sub_layer_lower_bit_rate_constraint_flag = bs.u(1);
                //sub_layer_profile[i].sub_layer_reserved_zero_bits               = bs.u(34);
                bs.bitPos += 34;
                bs.flush_buf();
            }
            else {
                //sub_layer_profile[i].sub_layer_reserved_zero_bits = bs.u(43);
                bs.bitPos += 43;
                bs.flush_buf();
            }
            if ( (sub_layer_profile[i].sub_layer_profile_idc >= 1 && sub_layer_profile[i].sub_layer_profile_idc <= 5) ||
                sub_layer_profile[i].sub_layer_profile_compatibility_flag[1] ||
                sub_layer_profile[i].sub_layer_profile_compatibility_flag[2] ||
                sub_layer_profile[i].sub_layer_profile_compatibility_flag[3] ||
                sub_layer_profile[i].sub_layer_profile_compatibility_flag[4] ||
                sub_layer_profile[i].sub_layer_profile_compatibility_flag[5]
            ) {
                sub_layer_profile[i].sub_layer_inbld_flag = bs.u(1);
            }
            else {
                /*sub_layer_profile[i].sub_layer_reserved_zero_bit = */bs.u(1);
            }
        }
        if (sub_layer_level_present_flag[i]){
            sub_layer_profile[i].sub_layer_level_idc = bs.u(8);
        }
    }
}

//hrd_parameters(commonInfPresentFlag, maxNumSubLayersMinus1) {
//    if (commonInfPresentFlag) {
//        nal_hrd_parameters_present_flag u(1);
//        vcl_hrd_parameters_present_flag u(1);
//        if (nal_hrd_parameters_present_flag | | vcl_hrd_parameters_present_flag) {
//            sub_pic_hrd_params_present_flag u(1);
//            if (sub_pic_hrd_params_present_flag) {
//                tick_divisor_minus2 u(8);
//                du_cpb_removal_delay_increment_length_minus1 u(5);
//                sub_pic_cpb_params_in_pic_timing_sei_flag u(1);
//                dpb_output_delay_du_length_minus1 u(5);
//            }
//            bit_rate_scale u(4);
//            cpb_size_scale u(4);
//            if (sub_pic_hrd_params_present_flag)
//                cpb_size_du_scale u(4);
//            initial_cpb_removal_delay_length_minus1 u(5);
//            au_cpb_removal_delay_length_minus1 u(5);
//            dpb_output_delay_length_minus1 u(5);
//        }
//    }
//    for (i = 0; i <= maxNumSubLayersMinus1; i++) {
//        fixed_pic_rate_general_flag[i] u(1);
//        if (!fixed_pic_rate_general_flag[i])
//            fixed_pic_rate_within_cvs_flag[i] u(1);
//        if (fixed_pic_rate_within_cvs_flag[i])
//            elemental_duration_in_tc_minus1[i] ue(v);
//        else
//            low_delay_hrd_flag[i] u(1);
//        if (!low_delay_hrd_flag[i])
//            cpb_cnt_minus1[i] ue(v);
//        if (nal_hrd_parameters_present_flag)
//            sub_layer_hrd_parameters(i);
//        if (vcl_hrd_parameters_present_flag)
//            sub_layer_hrd_parameters(i);
//    }
//}

//sub_layer_hrd_parameters(subLayerId) {
//    for (i = 0; i <= CpbCnt; i++) {
//        bit_rate_value_minus1[i] ue(v);
//        cpb_size_value_minus1[i] ue(v);
//        if (sub_pic_hrd_params_present_flag) {
//            cpb_size_du_value_minus1[i] ue(v);
//            bit_rate_du_value_minus1[i] ue(v);
//        }
//        cbr_flag[i] u(1);
//    }
//}
void Hrd_parameters::parse(BitStream& bs, uint8_t commonInfPresentFlag, size_t maxNumSubLayersMinus1) {
    bs.flush_buf();
    if (commonInfPresentFlag) {
        nal_hrd_parameters_present_flag = bs.u(1);
        vcl_hrd_parameters_present_flag = bs.u(1);
        if (nal_hrd_parameters_present_flag || vcl_hrd_parameters_present_flag) {
            sub_pic_hrd_params_present_flag = bs.u(1);
            if (sub_pic_hrd_params_present_flag) {
                tick_divisor_minus2 = bs.u(8);
                du_cpb_removal_delay_increment_length_minus1 = bs.u(5);
                sub_pic_cpb_params_in_pic_timing_sei_flag = bs.u(1);
                dpb_output_delay_du_length_minus1 = bs.u(5);
            }
            bit_rate_scale = bs.u(4);
            cpb_size_scale = bs.u(4);
            bs.flush_buf();
            if (sub_pic_hrd_params_present_flag)
                cpb_size_du_scale = bs.u(4);
            initial_cpb_removal_delay_length_minus1 = bs.u(5);
            au_cpb_removal_delay_length_minus1 = bs.u(5);
            dpb_output_delay_length_minus1 = bs.u(5);
        }
    }
    for (size_t i = 0; i <= maxNumSubLayersMinus1; i++) {
        bs.flush_buf();
        sub_parameters[i].fixed_pic_rate_general_flag = bs.u(1);
        if (!sub_parameters[i].fixed_pic_rate_general_flag) {
            sub_parameters[i].fixed_pic_rate_within_cvs_flag = bs.u(1);
        }
        if (sub_parameters[i].fixed_pic_rate_within_cvs_flag) {
            sub_parameters[i].elemental_duration_in_tc_minus1 = bs.ue();
        }
        else {
            sub_parameters[i].low_delay_hrd_flag = bs.u(1);
        }
        if (!sub_parameters[i].low_delay_hrd_flag) {
            bs.flush_buf();
            sub_parameters[i].cpb_cnt_minus1 = bs.ue();
        }
        if (nal_hrd_parameters_present_flag) {
            for (size_t j = 0; j < sub_parameters[i].cpb_cnt_minus1; ++j) {
                bs.flush_buf();
                sub_parameters[i].sub_layer_nal_hrd_parameters[j].bit_rate_value_minus1 = bs.ue();
                bs.flush_buf();
                sub_parameters[i].sub_layer_nal_hrd_parameters[j].cpb_size_value_minus1 = bs.ue();
                if (sub_pic_hrd_params_present_flag) {
                    bs.flush_buf();
                    sub_parameters[i].sub_layer_nal_hrd_parameters[j].cpb_size_du_value_minus1 = bs.ue();
                    bs.flush_buf();
                    sub_parameters[i].sub_layer_nal_hrd_parameters[j].bit_rate_du_value_minus1 = bs.ue();
                }
                sub_parameters[i].sub_layer_nal_hrd_parameters[j].cbr_flag = bs.u(1);
            }
        }
        if (vcl_hrd_parameters_present_flag) {
            for (size_t j = 0; j < sub_parameters[i].cpb_cnt_minus1; ++j) {
                bs.flush_buf();
                sub_parameters[i].sub_layer_vcl_hrd_parameters[j].bit_rate_value_minus1 = bs.ue();
                bs.flush_buf();
                sub_parameters[i].sub_layer_vcl_hrd_parameters[j].cpb_size_value_minus1 = bs.ue();
                if (sub_pic_hrd_params_present_flag) {
                    bs.flush_buf();
                    sub_parameters[i].sub_layer_vcl_hrd_parameters[j].cpb_size_du_value_minus1 = bs.ue();
                    bs.flush_buf();
                    sub_parameters[i].sub_layer_vcl_hrd_parameters[j].bit_rate_du_value_minus1 = bs.ue();
                }
                sub_parameters[i].sub_layer_vcl_hrd_parameters[j].cbr_flag = bs.u(1);
            }
        }
    }
}

