# include "H265_stream.hpp"
# include "Element.hpp"
# include <memory>

using Nal_unit = H265_stream::Nal_unit;
using Rbsp = Nal_unit::Rbsp;

# define X_def_Enum_Rbsp_type(name__, comment__) "[ "#name__## " ] " #comment__ ,
XEnumEx_def(Rbsp::Type, Xt_Nal_unit_type, X_def_Enum_Rbsp_type);

Rbsp::Cb_new_rbsp Rbsp::on_new[Rbsp::Type::e_max_num];
static bool is_init = []() {

# define X_def_new_rbsp(name__) \
    Rbsp::on_new[Rbsp::Type::e_##name__] = \
        [](uint8_t* start_, size_t size_)->std::unique_ptr<Rbsp>{\
            return std::make_unique<Nal_unit::Rbsp_##name__>(start_, size_);\
        };

    Xt_Nal_unit_type(X_def_new_rbsp);
    return true;
}();

void H265_stream::Nal_unit::parse(){
    BitStream bs;
    bs.init(start_ptr, NumBytesInNalUnit);
    forbidden_zero_bit = bs.f(1);
    nal_unit_type = bs.u(6);
    info_show("nalu type:" << (size_t)nal_unit_type <<" "<< Rbsp::Type::mean[nal_unit_type]);
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
    rbsp_uptr = Rbsp::get_new( (Rbsp::Type::E)nal_unit_type, rbsp_start, rbsp_end - rbsp_start);
}


Nal_unit::Rbsp_VPS_NUT          ::Rbsp_VPS_NUT(uint8_t* start_, size_t size)
    :Rbsp(start_,size)
{
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
                        cprms_present_flag[ i ]                 u(1)
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
    vps_video_parameter_set_id = bs.u(4);
    vps_base_layer_internal_flag = bs.u(1);
    vps_base_layer_available_flag = bs.u(1);
    vps_max_layers_minus1 = bs.u(6);
    vps_max_sub_layers_minus1 = bs.u(3);
    vps_temporal_id_nesting_flag = bs.u(1);
    vps_reserved_0xffff_16bits = bs.u(16);
    profile_tier_level.parse(bs, 1, vps_max_sub_layers_minus1);
    vps_sub_layer_ordering_info_present_flag = bs.u(1);
    for (size_t i = (vps_sub_layer_ordering_info_present_flag ? 0 : vps_max_sub_layers_minus1);
        i <= vps_max_sub_layers_minus1;
        i++
    ) {
        bs.flush_buf();
        vps_max_dec_pic_buffering_minus1[i] = bs.ue();
        bs.flush_buf();
        vps_max_num_reorder_pics[i] = bs.ue();
        bs.flush_buf();
        vps_max_latency_increase_plus1[i] = bs.ue();
    }
    bs.flush_buf();
    vps_max_layer_id = bs.u(6);
    bs.flush_buf();
    vps_num_layer_sets_minus1 = bs.ue();
    for (size_t i = 1; i <= vps_num_layer_sets_minus1; i++) {
        size_t j = 0;
        for (size_t j = 0; j <= vps_max_layer_id; j++) {
            bs.flush_buf();
            layer_id_included_flag[i][j] = bs.u(1);
        }
    }
    bs.flush_buf();
    vps_timing_info_present_flag = bs.u(1);
    if (vps_timing_info_present_flag) {
        bs.flush_buf();
        vps_num_units_in_tick = bs.u(32);
        bs.flush_buf();
        vps_time_scale = bs.u(32);
        bs.flush_buf();
        vps_poc_proportional_to_timing_flag = bs.u(1);
        if (vps_poc_proportional_to_timing_flag) {
            bs.flush_buf();
            vps_num_ticks_poc_diff_one_minus1 = bs.ue();
        }
        bs.flush_buf();
        vps_num_hrd_parameters = bs.ue();
        for (size_t i = 0; i < vps_num_hrd_parameters; i++) {
            bs.flush_buf();
            vps_hrd_parameters[i].hrd_layer_set_idx = bs.ue();
            if (i > 0) {
                bs.flush_buf();
                vps_hrd_parameters[i].cprms_present_flag = bs.u(1);
            }
            vps_hrd_parameters[i].hrd_parameters.parse(bs, vps_hrd_parameters[i].cprms_present_flag, vps_max_sub_layers_minus1);
        }
    }
    bs.flush_buf();
    vps_extension_flag = bs.u(1);
    vps_extension_data_flag.clear();
    if (vps_extension_flag) {
        while (bs.more_rbsp_data()) {
            vps_extension_data_flag.push_back(bs.u(1) != 0);
        }
    }
    bs.flush_buf();
    bs.rbsp_trailing_bits();
}


Nal_unit::Rbsp_TRAIL_N          ::Rbsp_TRAIL_N       (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_TRAIL_R          ::Rbsp_TRAIL_R       (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_TSA_N            ::Rbsp_TSA_N         (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_TSA_R            ::Rbsp_TSA_R         (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_STSA_N           ::Rbsp_STSA_N        (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_STSA_R           ::Rbsp_STSA_R        (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_RADL_N           ::Rbsp_RADL_N        (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_RADL_R           ::Rbsp_RADL_R        (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_RASL_N           ::Rbsp_RASL_N        (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_RASL_R           ::Rbsp_RASL_R        (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_BLA_W_LP         ::Rbsp_BLA_W_LP      (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_BLA_W_RADL       ::Rbsp_BLA_W_RADL    (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_BLA_N_LP         ::Rbsp_BLA_N_LP      (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_IDR_W_RADL       ::Rbsp_IDR_W_RADL    (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_IDR_N_LP         ::Rbsp_IDR_N_LP      (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_CRA_NUT          ::Rbsp_CRA_NUT       (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_SPS_NUT          ::Rbsp_SPS_NUT       (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_PPS_NUT          ::Rbsp_PPS_NUT       (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_AUD_NUT          ::Rbsp_AUD_NUT       (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_EOS_NUT          ::Rbsp_EOS_NUT       (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_EOB_NUT          ::Rbsp_EOB_NUT       (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_FD_NUT           ::Rbsp_FD_NUT        (uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_PREFIX_SEI_NUT   ::Rbsp_PREFIX_SEI_NUT(uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}
Nal_unit::Rbsp_SUFFIX_SEI_NUT   ::Rbsp_SUFFIX_SEI_NUT(uint8_t* start_, size_t size_):Rbsp(start_,size_){
    
}