# include "H265_stream.hpp"
# include "Element.hpp"
# include <memory>


H265_stream::H265_stream(std::vector<uint8_t>& data_) {
    //�ù��̵��������ֽڵ���������ɣ���������ֽ�����һϵ���ֽ���NAL��Ԫ�﷨�ṹ��ɡ�
    //�ù��̵������һϵ��NAL��Ԫ�﷨�ṹ��ɡ�
    //�ڽ�����̵Ŀ�ʼ���������������ֽ����еĵ�ǰλ�ó�ʼ��Ϊ�ֽ����Ŀ�ʼ��
    //Ȼ����ȡ������ÿ��Leading_zero_8bits�﷨Ԫ�أ�������ڣ���
    //���ֽ����еĵ�ǰλ��һ����ǰ�ƶ�һ���ֽڣ�
    //ֱ���ֽ����еĵ�ǰλ��ʹ�ø�λ���е����ĸ��ֽ��γ��ĸ� �ֽ�����0x00000001��
    //Ȼ�󣬽������ظ�ִ�������𲽹��̣�����ȡ�ͽ����ֽ����е�ÿ��NAL��Ԫ�﷨�ṹ��
    //ֱ�������ֽ�����ĩβ����δָ���ķ�ʽȷ���������ѽ������ֽ����е����һ��NAL��Ԫ��
    //1.��λ���еĺ��ĸ��ֽ��γ����ֽ����� 0x 00 00 00 01ʱ��
    //    ����ȡ�������ֽ����е���һ���ֽڣ�����zero_byte�﷨Ԫ�أ���
    //    �����ֽ����еĵ�ǰλ������Ϊ���ڸö����ֽ�֮����ֽ�λ�á�
    //2.��ȡ�������ֽ����е���һ�����ֽ����У���start_code_prefix_one_3bytes����
    //    �����ֽ����еĵ�ǰλ������Ϊ���ڴ����ֽ����к���ֽ�λ�á�
    //3. NumBytesInNalUnit����Ϊ���ڴ��ֽ����е�ǰλ�õ��ֽڿ�ʼ,
    //    ֱ������һ������������λ��֮ǰ�����һ���ֽڵ��ֽ�����
    //        �C�������ֽڶ�������ֽ����У�����0x000000��
    //        �C�������ֽڶ�������ֽ����У�����0x000001��
    //        �C�ֽ�����ĩβ����δָ���ķ�ʽȷ����
    //4. NumBytesInNalUnit�ֽڴ�λ����ɾ�����ֽ����еĵ�ǰλ������NumBytesInNalUnit�ֽڡ�
    //    ���ֽ�������nal_unit��NumBytesInNalUnit������ʹ��NAL��Ԫ������̽��н��롣
    //5.���ֽ����еĵ�ǰλ�ò����ֽ�����ĩβʱ����δָ���ķ�ʽȷ������
    //    �����ֽ����е���һ���ֽڲ��Ե���0x000001�����ֽ����п�ͷ��
    //    ��һ���ֽ�����ֽ����е��ֽ����в��Ե���0x00000001���ĸ��ֽ����п�ͷ��
    //    ���������ȡ������ÿ��Trailing_zero_8bits�﷨Ԫ�أ�
    //    ���ֽ����еĵ�ǰλ��һ����ǰ�ƶ�һ���ֽڣ�ֱ���ֽ����еĵ�ǰλ��Ϊ������
    //    �ֽ����е���һ���ֽھ��γ����ĸ��ֽڵ�����0x00000001���������ֽ�����ĩβ����δָ���ķ�ʽȷ������
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
    �ֽ������������󣬽��������Լ������ı����������к��������ֽ�����0x000001��0x000003��
    ����⵽���ֽ�����0x000001ʱ������һ����ʼ����ǰ׺��
    ����⵽���ֽ�����0x000003ʱ���������ֽڣ�0x03����emulation_prevention_three_byte�������յ�7.4.2�ڵĹ涨������

    ����⵽�������﷨�еĴ���ʱ�����������Խ���⵽��������Ϊ ָʾ�ֽڶ�������Ѿ���ʧ
    ���ҿ��ܻᶪ�����б��������ݣ�ֱ���ڱ����������ڱ������еĽϺ�λ�ü�⵽�ֽڶ���Ϊֹ��
    �����磬��7.4.2���н�ֹ��forbidden_zero_bit�ķ���ֵ�����ֽڻ����ֽ�����֮һ��

    NAL��Ԫ�����һ���ֽڲ��õ���0x00��
    ��NAL��Ԫ�ڣ����������ֽڵ����в��ó������κ��ֽڶ����λ�ã�
    �C 0x000000
    �C 0x000001
    �C 0x000002
    ��NAL��Ԫ�ڣ������������⣬�κ���0x000003��ͷ�����ֽ����ж���Ӧ�������κ��ֽڶ���λ�ã�
    �C 0x00000300
    �C 0x00000301
    �C 0x00000302
    �C 0x00000303

    ===================================
    0x 00 00 00 01  �ָ�
    0x 00 00 01     �ָ�

    0x 00 00 02     ��ֹ
    0x 00 00 03 01  ��ֹ
    0x 00 00 03 02  ��ֹ
    0x 00 00 03 03  ��ֹ
    0x 00 00 03 xx  ȥ��03
    */

    // Ѱ�ҿ�ͷ
    size_t buf;
    while (true) {
    }




    info_show(*(size_t*)curr_ptr << ": �� ��:" << lz << " �� ��:" << rz);

    // Ѱ�ҽ�β ͬʱȥ��000003���ҶԽ�ֹ�����ݽ��б���
    // ��size_t����

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
    //        wrn_show("û���ҵ�nal��Ԫ");
    //        return;
    //    }
    //    if (*curr_ptr != 0x00) {
    //        wrn_show("��׼�涨û������ͷ�ͱ���Ϊ0");
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
    //        wrn_show("�ļ�����");
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