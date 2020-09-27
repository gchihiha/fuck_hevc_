#pragma once
# include "BitStream.hpp"

static constexpr size_t set_maxNumSubLayersMinus1 = 64;

struct Profile_tier_level {
    uint8_t general_profile_space                       ;
    uint8_t general_tier_flag                           ;
    uint8_t general_profile_idc                         ;
    uint8_t general_profile_compatibility_flag[32]      ;
    uint8_t general_progressive_source_flag             ;
    uint8_t general_interlaced_source_flag              ;
    uint8_t general_non_packed_constraint_flag          ;
    uint8_t general_frame_only_constraint_flag          ;

    uint8_t general_max_12bit_constraint_flag           ;
    uint8_t general_max_10bit_constraint_flag           ;
    uint8_t general_max_8bit_constraint_flag            ;
    uint8_t general_max_422chroma_constraint_flag       ;
    uint8_t general_max_420chroma_constraint_flag       ;
    uint8_t general_max_monochrome_constraint_flag      ;
    uint8_t general_intra_constraint_flag               ;
    uint8_t general_one_picture_only_constraint_flag    ;
    uint8_t general_lower_bit_rate_constraint_flag      ;
    uint64_t general_reserved_zero_bits                 ;

    uint8_t general_inbld_flag;

    uint8_t general_level_idc;

    uint8_t sub_layer_profile_present_flag[set_maxNumSubLayersMinus1];
    uint8_t sub_layer_level_present_flag[set_maxNumSubLayersMinus1];

    uint8_t reserved_zero_2bits[8];

    struct Sub_layer_profile {
        uint8_t sub_layer_profile_space;
        uint8_t sub_layer_tier_flag;
        uint8_t sub_layer_profile_idc;
        uint8_t sub_layer_profile_compatibility_flag[32];
        uint8_t sub_layer_progressive_source_flag;
        uint8_t sub_layer_interlaced_source_flag;
        uint8_t sub_layer_non_packed_constraint_flag;
        uint8_t sub_layer_frame_only_constraint_flag;

        uint8_t sub_layer_max_12bit_constraint_flag       ;
        uint8_t sub_layer_max_10bit_constraint_flag       ;
        uint8_t sub_layer_max_8bit_constraint_flag        ;
        uint8_t sub_layer_max_422chroma_constraint_flag   ;
        uint8_t sub_layer_max_420chroma_constraint_flag   ;
        uint8_t sub_layer_max_monochrome_constraint_flag  ;
        uint8_t sub_layer_intra_constraint_flag           ;
        uint8_t sub_layer_one_picture_only_constraint_flag;
        uint8_t sub_layer_lower_bit_rate_constraint_flag  ;
        uint64_t sub_layer_reserved_zero_bits             ;

        uint8_t sub_layer_inbld_flag;
        uint8_t sub_layer_level_idc;
    };
    Sub_layer_profile sub_layer_profile[set_maxNumSubLayersMinus1];

    void parse(BitStream&, uint8_t, size_t);
};

struct Hrd_parameters {

    uint8_t nal_hrd_parameters_present_flag;
    uint8_t vcl_hrd_parameters_present_flag;
    uint8_t sub_pic_hrd_params_present_flag;
    uint8_t tick_divisor_minus2;
    uint8_t du_cpb_removal_delay_increment_length_minus1;
    uint8_t sub_pic_cpb_params_in_pic_timing_sei_flag;
    uint8_t dpb_output_delay_du_length_minus1;
    uint8_t bit_rate_scale;
    uint8_t cpb_size_scale;
    uint8_t cpb_size_du_scale;
    uint8_t initial_cpb_removal_delay_length_minus1;
    uint8_t au_cpb_removal_delay_length_minus1;
    uint8_t dpb_output_delay_length_minus1;
    
    struct Sub_parameters {
        uint8_t fixed_pic_rate_general_flag;
        uint8_t fixed_pic_rate_within_cvs_flag;
        uint8_t low_delay_hrd_flag;
        uint8_t cpb_cnt_minus1; /* 0 to 31*/
        uint16_t elemental_duration_in_tc_minus1;/* [0, 2047] */

        struct Data {
            uint32_t bit_rate_value_minus1;
            uint32_t cpb_size_value_minus1;
            uint32_t cpb_size_du_value_minus1;
            uint32_t bit_rate_du_value_minus1;
            uint8_t cbr_flag;
        };
        Data sub_layer_nal_hrd_parameters[32];
        Data sub_layer_vcl_hrd_parameters[32];
    };
    Sub_parameters sub_parameters[set_maxNumSubLayersMinus1];

    void parse(BitStream&,uint8_t ,size_t);
};

