# include "H265_stream.hpp"

using Nal_unit = H265_stream::Nal_unit;
using Rbsp = Nal_unit::Rbsp;

XEnum_def(Rbsp::Type, Xt_Nal_unit_type);
# define X_def_new_rbsp(name__) \
Rbsp::on_new[Rbsp::Type::e_##name__] = \
[](uint8_t* start_, size_t size_){ return std::make_unique<Rbsp_##name__>(start_, size_); };

void H265_stream::Nal_unit::parse(){
    BitStream bs;
    bs.init(start_ptr, NumBytesInNalUnit);
    forbidden_zero_bit = bs.f(1);
    nal_unit_type = bs.u(6);
    info_show("nalu type:" << (size_t)nal_unit_type);
    nuh_layer_id = bs.u(6);
    nuh_temporal_id_plus1 = bs.u(3);

    //    nal_unit( NumBytesInNalUnit ) {
    //         nal_unit_header( )
    //         NumBytesInRbsp = 0
    //         for( i = 2; i < NumBytesInNalUnit; i++ )
    //             if( i + 2 < NumBytesInNalUnit  &&  next_bits( 24 )  = =  0x000003 ) {
    //                 rbsp_byte[ NumBytesInRbsp++ ]
    //                 rbsp_byte[ NumBytesInRbsp++ ]
    //                 i  +=  2
    //                 emulation_prevention_three_byte  // equal to 0x03 
    //            }
    //            else
    //                rbsp_byte[NumBytesInRbsp++]
    //    }
    bs.flush_buf();
    uint8_t* rbsp_start = bs.byte_ptr();
    uint8_t* rbsp_end = rbsp_start;
    for (size_t i = 2; i < NumBytesInNalUnit; ++i) {
        if (i + 2 < NumBytesInNalUnit && (*(size_t*)rbsp_end << 8) == 0x03000000) {
            rbsp_end += 2;
            i += 2;
        }
        else {
            ++rbsp_end;
        }
    }
    if (rbsp_end > rbsp_start) {
        rbsp_uptr = Rbsp::get_new( (Rbsp::Type::E)nal_unit_type, rbsp_start, rbsp_end - rbsp_start);
    }
}


Nal_unit::Rbsp_VPS_NUT::Rbsp_VPS_NUT(uint8_t* start_, size_t size)
    :Rbsp(start_,size)
{
    type = Rbsp::Type::e_VPS_NUT;
    BitStream bs;
    bs.init(start_, size);

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
                    vps_num_units_in_tick                       u(32)
                vps_time_scale                                  u(32)
                vps_poc_proportional_to_timing_flag             u(1)
                if( vps_poc_proportional_to_timing_flag )
                    vps_num_ticks_poc_diff_one_minus1           ue(v)
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



}


