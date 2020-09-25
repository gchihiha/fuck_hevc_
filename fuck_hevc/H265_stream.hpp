# pragma once
# include "BitStream.hpp"
# include <iostream>
# include <vector>

struct H265_stream:public BitStream {

    struct Nal_unit {
        static constexpr size_t nalu_reserve = 8192;

        uint8_t forbidden_zero_bit;
        uint8_t nal_unit_type;
        uint8_t nuh_layer_id;
        uint8_t nuh_temporal_id_plus1;
        
        uint8_t* start_ptr;
        uint8_t* end_ptr;

        Nal_unit()
            :forbidden_zero_bit(0)
            , nal_unit_type(0)
            , nuh_layer_id(0)
            , nuh_temporal_id_plus1(0)
            , start_ptr(nullptr)
            , end_ptr(nullptr)
        { }
        void read(H265_stream& h265_) {
            assert(h265_.byte_aligned());

            forbidden_zero_bit = h265_.f(1);
            nal_unit_type = h265_.u(6);
            nuh_layer_id = h265_.u(6);
            nuh_temporal_id_plus1 = h265_.u(3);

            /*
            nal_unit( NumBytesInNalUnit ) {
                 nal_unit_header( )
                 NumBytesInRbsp = 0
                 for( i = 2; i < NumBytesInNalUnit; i++ )
                     if( i + 2 < NumBytesInNalUnit  &&  next_bits( 24 )  = =  0x000003 ) {
                         rbsp_byte[ NumBytesInRbsp++ ]
                         rbsp_byte[ NumBytesInRbsp++ ]
                         i  +=  2
                         emulation_prevention_three_byte  // equal to 0x03 
                    }
                    else
                        rbsp_byte[NumBytesInRbsp++]
            }
            */
            uint8_t* curr_ptr = h265_.byte_ptr();
            auto start_ptr = curr_ptr;
            auto NumBytesInNalUnit = h265_.end_byte_ptr() - start_ptr;
            for (size_t i = 2; i < NumBytesInNalUnit; i++) {
                if (i + 2 < NumBytesInNalUnit) {
                    h265_.set_bitPos(curr_ptr);
                    auto x = h265_.read(24);
                    if (x == 0x3) {
                        curr_ptr += 2;
                        h265_.set_bitPos(curr_ptr);
                        i += 2;
                        auto emulation_prevention_three_byte = h265_.f(8);
                        if (emulation_prevention_three_byte != 0x3) {
                            err_show("emulation_prevention_three_byte not equal 0x03!");
                        }
                    }
                }
                else {
                    ++curr_ptr;
                }
            }
            h265_.set_bitPos(curr_ptr);
        }
    };

    struct video_parameter_set_rbsp {
        /*
        video_parameter_set_rbsp( ) {
            vps_video_parameter_set_id                              u(4)
            vps_base_layer_internal_flag                            u(1)
            vps_base_layer_available_flag                           u(1)
            vps_max_layers_minus1                                   u(6)
            vps_max_sub_layers_minus1                               u(3)
            vps_temporal_id_nesting_flag                            u(1)
            vps_reserved_0xffff_16bits                              u(16)
            profile_tier_level( 1, vps_max_sub_layers_minus1 )
            vps_sub_layer_ordering_info_present_flag                u(1)
            for(i = ( vps_sub_layer_ordering_info_present_flag ? 0 : vps_max_sub_layers_minus1 );
                i <= vps_max_sub_layers_minus1; 
                i++ 
            ) {
                vps_max_dec_pic_buffering_minus1[ i ]               ue(v)
                vps_max_num_reorder_pics[ i ]                       ue(v)
                vps_max_latency_increase_plus1[ i ]                 ue(v)
            }
            vps_max_layer_id                                        u(6)
            vps_num_layer_sets_minus1                               ue(v)
            for( i = 1; i <= vps_num_layer_sets_minus1; i++ )
                for( j = 0; j <= vps_max_layer_id; j++ )
                    layer_id_included_flag[ i ][ j ]                u(1)
                    vps_timing_info_present_flag                    u(1)
                    if( vps_timing_info_present_flag ) {
                    vps_num_units_in_tick                           u(32)
                    vps_time_scale                                  u(32)
                    vps_poc_proportional_to_timing_flag             u(1)
                    if( vps_poc_proportional_to_timing_flag )
                    vps_num_ticks_poc_diff_one_minus1               ue(v)
                    vps_num_hrd_parameters                          ue(v)
                    for( i = 0; i < vps_num_hrd_parameters; i++ ) {
                        hrd_layer_set_idx[ i ]                      ue(v)
                        if( i > 0 )
                        cprms_present_flag[ i ]                     u(1)
                        hrd_parameters( cprms_present_flag[ i ], vps_max_sub_layers_minus1 )
                    }
                }
            vps_extension_flag                                      u(1)
            if( vps_extension_flag )
                while( more_rbsp_data( ) )
                    vps_extension_data_flag                         u(1)
            rbsp_trailing_bits( )
        }
        */
    };
















    void test();
};







