# include "H265_stream.hpp"
# include "Element.hpp"
# include <memory>


H265_stream::H265_stream(std::vector<uint8_t>& data_) {
    //该过程的输入由字节的有序流组成，该有序的字节流由一系列字节流NAL单元语法结构组成。
    //该过程的输出由一系列NAL单元语法结构组成。
    //在解码过程的开始，解码器将其在字节流中的当前位置初始化为字节流的开始。
    //然后提取并丢弃每个Leading_zero_8bits语法元素（如果存在），
    //将字节流中的当前位置一次向前移动一个字节，
    //直到字节流中的当前位置使得该位流中的下四个字节形成四个 字节序列0x00000001。
    //然后，解码器重复执行以下逐步过程，以提取和解码字节流中的每个NAL单元语法结构，
    //直到遇到字节流的末尾（由未指定的方式确定）并且已解码了字节流中的最后一个NAL单元：
    //1.当位流中的后四个字节形成四字节序列 0x 00 00 00 01时，
    //    将提取并丢弃字节流中的下一个字节（它是zero_byte语法元素），
    //    并将字节流中的当前位置设置为等于该丢弃字节之后的字节位置。
    //2.提取并丢弃字节流中的下一个三字节序列（即start_code_prefix_one_3bytes），
    //    并将字节流中的当前位置设置为等于此三字节序列后的字节位置。
    //3. NumBytesInNalUnit设置为等于从字节流中当前位置的字节开始,
    //    直到以下一个或多个条件的位置之前的最后一个字节的字节数：
    //        –后续的字节对齐的三字节序列，等于0x000000，
    //        –后续的字节对齐的三字节序列，等于0x000001，
    //        –字节流的末尾，由未指定的方式确定。
    //4. NumBytesInNalUnit字节从位流中删除，字节流中的当前位置增加NumBytesInNalUnit字节。
    //    该字节序列是nal_unit（NumBytesInNalUnit），并使用NAL单元解码过程进行解码。
    //5.当字节流中的当前位置不在字节流的末尾时（由未指定的方式确定），
    //    并且字节流中的下一个字节不以等于0x000001的三字节序列开头，
    //    下一个字节如果字节流中的字节序列不以等于0x00000001的四个字节序列开头，
    //    则解码器提取并丢弃每个Trailing_zero_8bits语法元素，
    //    将字节流中的当前位置一次向前移动一个字节，直到字节流中的当前位置为这样，
    //    字节流中的下一个字节就形成了四个字节的序列0x00000001或遇到了字节流的末尾（由未指定的方式确定）。
    //    byte_stream_nal_unit( NumBytesInNalUnit ) {
    //        while( next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001 )
    //            leading_zero_8bits // equal to 0x00  f(8)
    //        if( next_bits( 24 ) != 0x000001 )
    //            zero_byte /* equal to 0x00  f(8)
    //        start_code_prefix_one_3bytes /* equal to 0x000001  f(24)
    //        nal_unit( NumBytesInNalUnit )
    //        while( more_data_in_byte_stream( ) && next_bits( 24 ) != 0x000001 && next_bits( 32 ) != 0x00000001 )
    //            trailing_zero_8bits /* equal to 0x00  f(8)
    //    }
    auto curr_ptr = data_.data();
    auto end_ptr = curr_ptr+data_.size();
    uint8_t* start = nullptr;
    uint8_t* end = nullptr;
    /*
    字节与比特流对齐后，解码器可以检查输入的比特流数据中后续的三字节序列0x000001和0x000003。
    当检测到三字节序列0x000001时，这是一个起始代码前缀。
    当检测到三字节序列0x000003时，第三个字节（0x03）是emulation_prevention_three_byte，将按照第7.4.2节的规定丢弃。

    当检测到比特流语法中的错误时，解码器可以将检测到的条件视为 指示字节对齐可能已经丢失
    并且可能会丢弃所有比特流数据，直到在本节中上述在比特流中的较后位置检测到字节对齐为止。
    （例如，在7.4.2节中禁止的forbidden_zero_bit的非零值或三字节或四字节序列之一）

    NAL单元的最后一个字节不得等于0x00。
    在NAL单元内，以下三个字节的序列不得出现在任何字节对齐的位置：
    – 0x000000
    – 0x000001
    – 0x000002
    在NAL单元内，除以下序列外，任何以0x000003开头的四字节序列都不应出现在任何字节对齐位置：
    – 0x00000300
    – 0x00000301
    – 0x00000302
    – 0x00000303

    ===================================
    0x 00 00 00 01  分隔
    0x 00 00 01     分隔

    0x 00 00 02     禁止
    0x 00 00 03 01  禁止
    0x 00 00 03 02  禁止
    0x 00 00 03 03  禁止
    0x 00 00 03 xx  去除03
    */

    // 寻找开头
    size_t buf;
    while (true) {
    }




    info_show(*(size_t*)curr_ptr << ": 左 零:" << lz << " 右 零:" << rz);

    // 寻找结尾 同时去除000003并且对禁止的数据进行报错
    // 按size_t拷贝

    //while (true) {
    //    if (*(size_t*)curr_ptr == 0x01000000) {
    //        curr_ptr += 4;
    //        start = curr_ptr;
    //        break;
    //    }
    //    else if (*(size_t*)curr_ptr == 0x010000) {
    //        curr_ptr += 3;
    //        start = curr_ptr;
    //        break;
    //    }
    //    else if (curr_ptr >= end_ptr) {
    //        wrn_show("没有找到nal单元");
    //        return;
    //    }
    //    if (*curr_ptr != 0x00) {
    //        wrn_show("标准规定没遇到开头就必须为0");
    //    }
    //    ++curr_ptr;
    //}
    //while (true) {
    //    if (*(size_t*)curr_ptr == 0x01000000) {
    //        //nalu_list.emplace_back(start, curr_ptr - start);
    //        curr_ptr += 4;
    //        start = curr_ptr;
    //    }
    //    else if (*(size_t*)curr_ptr == 0x010000) {
    //        //nalu_list.emplace_back(start, curr_ptr - start);
    //        curr_ptr += 3;
    //        start = curr_ptr;
    //    }
    //    else if (curr_ptr >= end_ptr) {
    //        wrn_show("文件结束");
    //        if (curr_ptr > start) {
    //            //nalu_list.emplace_back(start, curr_ptr - start);
    //        }
    //        break;
    //    }
    //    ++curr_ptr;
    //}
}


# define X_def_Enum_Rbsp_type(name__, comment__) "[ "#name__## " ] " #comment__ ,
XEnumEx_def(Rbsp::Type, Xt_Nal_unit_type, X_def_Enum_Rbsp_type);

Rbsp::Cb_new_rbsp Rbsp::on_new[Rbsp::Type::e_max_num];
static bool is_init = []() {

# define X_def_new_rbsp(name__) \
    Rbsp::on_new[Rbsp::Type::e_##name__] = \
        [](std::vector<uint8_t>& data_)->std::unique_ptr<Rbsp>{\
            return std::make_unique<Rbsp_##name__>(data_);\
        };

    Xt_Nal_unit_type(X_def_new_rbsp);
    return true;
}();

Nal_unit::Nal_unit(std::vector<uint8_t>& data_){
    BitStream bs;
    if (data_.empty()) {
        wrn_show("nal unit no data!!");
        return;
    }
    bs.init(data_.data(), data_.size());
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
    rbsp_uptr = Rbsp::get_new((Rbsp::Type::E)nal_unit_type, data_);
}


Rbsp_VPS_NUT::Rbsp_VPS_NUT(std::vector<uint8_t>& data_) :Rbsp(data_) {
//    BitStream bs;
//    bs.init(data_.data(), data_.size());
//
//    /*
//    video_parameter_set_rbsp( ) {
//        vps_video_parameter_set_id                              u(4)
//        vps_base_layer_internal_flag                            u(1)
//        vps_base_layer_available_flag                           u(1)
//        vps_max_layers_minus1                                   u(6)
//        vps_max_sub_layers_minus1                               u(3)
//        vps_temporal_id_nesting_flag                            u(1)
//        vps_reserved_0xffff_16bits                              u(16)
//        profile_tier_level( 1, vps_max_sub_layers_minus1 )
//        vps_sub_layer_ordering_info_present_flag                u(1)
//        for(i = ( vps_sub_layer_ordering_info_present_flag ? 0 : vps_max_sub_layers_minus1 );
//            i <= vps_max_sub_layers_minus1;
//            i++
//        ) {
//            vps_max_dec_pic_buffering_minus1[ i ]               ue(v)
//            vps_max_num_reorder_pics[ i ]                       ue(v)
//            vps_max_latency_increase_plus1[ i ]                 ue(v)
//        }
//        vps_max_layer_id                                        u(6)
//        vps_num_layer_sets_minus1                               ue(v)
//        for( i = 1; i <= vps_num_layer_sets_minus1; i++ )
//            for( j = 0; j <= vps_max_layer_id; j++ )
//                layer_id_included_flag[ i ][ j ]                u(1)
//                vps_timing_info_present_flag                    u(1)
//                if( vps_timing_info_present_flag ) {
//                    vps_num_units_in_tick                       u(32)
//                vps_time_scale                                  u(32)
//                vps_poc_proportional_to_timing_flag             u(1)
//                if( vps_poc_proportional_to_timing_flag )
//                    vps_num_ticks_poc_diff_one_minus1           ue(v)
//                vps_num_hrd_parameters                          ue(v)
//                for( i = 0; i < vps_num_hrd_parameters; i++ ) {
//                    hrd_layer_set_idx[ i ]                      ue(v)
//                    if( i > 0 )
//                        cprms_present_flag[ i ]                 u(1)
//                    hrd_parameters( cprms_present_flag[ i ], vps_max_sub_layers_minus1 )
//                }
//            }
//        vps_extension_flag                                      u(1)
//        if( vps_extension_flag )
//            while( more_rbsp_data( ) )
//                vps_extension_data_flag                         u(1)
//        rbsp_trailing_bits( )
//    }
//    */
//    vps_video_parameter_set_id = bs.u(4);
//    vps_base_layer_internal_flag = bs.u(1);
//    vps_base_layer_available_flag = bs.u(1);
//    vps_max_layers_minus1 = bs.u(6);
//    vps_max_sub_layers_minus1 = bs.u(3);
//    vps_temporal_id_nesting_flag = bs.u(1);
//    vps_reserved_0xffff_16bits = bs.u(16);
//    profile_tier_level.parse(bs, 1, vps_max_sub_layers_minus1);
//    vps_sub_layer_ordering_info_present_flag = bs.u(1);
//    for (size_t i = (vps_sub_layer_ordering_info_present_flag ? 0 : vps_max_sub_layers_minus1);
//        i <= vps_max_sub_layers_minus1;
//        i++
//    ) {
//        bs.flush_buf();
//        vps_max_dec_pic_buffering_minus1[i] = bs.ue();
//        bs.flush_buf();
//        vps_max_num_reorder_pics[i] = bs.ue();
//        bs.flush_buf();
//        vps_max_latency_increase_plus1[i] = bs.ue();
//    }
//    bs.flush_buf();
//    vps_max_layer_id = bs.u(6);
//    bs.flush_buf();
//    vps_num_layer_sets_minus1 = bs.ue();
//    for (size_t i = 1; i <= vps_num_layer_sets_minus1; i++) {
//        size_t j = 0;
//        for (size_t j = 0; j <= vps_max_layer_id; j++) {
//            bs.flush_buf();
//            layer_id_included_flag[i][j] = bs.u(1);
//        }
//    }
//    bs.flush_buf();
//    vps_timing_info_present_flag = bs.u(1);
//    if (vps_timing_info_present_flag) {
//        bs.flush_buf();
//        vps_num_units_in_tick = bs.u(32);
//        bs.flush_buf();
//        vps_time_scale = bs.u(32);
//        bs.flush_buf();
//        vps_poc_proportional_to_timing_flag = bs.u(1);
//        if (vps_poc_proportional_to_timing_flag) {
//            bs.flush_buf();
//            vps_num_ticks_poc_diff_one_minus1 = bs.ue();
//        }
//        bs.flush_buf();
//        vps_num_hrd_parameters = bs.ue();
//        for (size_t i = 0; i < vps_num_hrd_parameters; i++) {
//            bs.flush_buf();
//            vps_hrd_parameters[i].hrd_layer_set_idx = bs.ue();
//            if (i > 0) {
//                bs.flush_buf();
//                vps_hrd_parameters[i].cprms_present_flag = bs.u(1);
//            }
//            vps_hrd_parameters[i].hrd_parameters.parse(bs, vps_hrd_parameters[i].cprms_present_flag, vps_max_sub_layers_minus1);
//        }
//    }
//    bs.flush_buf();
//    vps_extension_flag = bs.u(1);
//    vps_extension_data_flag.clear();
//    if (vps_extension_flag) {
//        while (bs.more_rbsp_data()) {
//            vps_extension_data_flag.push_back(bs.u(1) != 0);
//        }
//    }
//    bs.flush_buf();
//    bs.rbsp_trailing_bits();
}
                        











Rbsp_TRAIL_N            ::Rbsp_TRAIL_N          (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_TRAIL_R            ::Rbsp_TRAIL_R          (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_TSA_N              ::Rbsp_TSA_N            (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_TSA_R              ::Rbsp_TSA_R            (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_STSA_N             ::Rbsp_STSA_N           (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_STSA_R             ::Rbsp_STSA_R           (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_RADL_N             ::Rbsp_RADL_N           (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_RADL_R             ::Rbsp_RADL_R           (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_RASL_N             ::Rbsp_RASL_N           (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_RASL_R             ::Rbsp_RASL_R           (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_BLA_W_LP           ::Rbsp_BLA_W_LP         (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_BLA_W_RADL         ::Rbsp_BLA_W_RADL       (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_BLA_N_LP           ::Rbsp_BLA_N_LP         (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_IDR_W_RADL         ::Rbsp_IDR_W_RADL       (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_IDR_N_LP           ::Rbsp_IDR_N_LP         (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_CRA_NUT            ::Rbsp_CRA_NUT          (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_SPS_NUT            ::Rbsp_SPS_NUT          (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_PPS_NUT            ::Rbsp_PPS_NUT          (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_AUD_NUT            ::Rbsp_AUD_NUT          (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_EOS_NUT            ::Rbsp_EOS_NUT          (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_EOB_NUT            ::Rbsp_EOB_NUT          (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_FD_NUT             ::Rbsp_FD_NUT           (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_PREFIX_SEI_NUT     ::Rbsp_PREFIX_SEI_NUT   (std::vector<uint8_t>& data_):Rbsp(data_){
    
}
Rbsp_SUFFIX_SEI_NUT     ::Rbsp_SUFFIX_SEI_NUT   (std::vector<uint8_t>& data_):Rbsp(data_){
    
}