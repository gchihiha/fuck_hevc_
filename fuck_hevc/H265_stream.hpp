# pragma once
# include "BitStream.hpp"
# include <iostream>
# include <vector>
# include <list>
# include <memory>

# include "common.hpp"
# include "xtable.hpp"

struct H265_stream:public BitStream {

    struct Test {
        struct Fuck;
    };
    struct Test::Fuck {

    };


    struct Nal_unit {
        static constexpr size_t nalu_reserve = 8192;
        
        uint8_t* start_ptr;
        size_t NumBytesInNalUnit;

        uint8_t forbidden_zero_bit;
        uint8_t nal_unit_type;
        uint8_t nuh_layer_id;
        uint8_t nuh_temporal_id_plus1;

        struct Rbsp {
            XEnum_decl(Type, Xt_Nal_unit_type);

            uint8_t* start;
            size_t size;

            Rbsp(uint8_t* start_, size_t size_)
                : start(start_)
                , size(size_)
            {}
            virtual void parse() = 0;
        };
# define X_rbsp_struct_decl(name__) struct Rbsp_##name__;
        Xt_Nal_unit_type(X_rbsp_struct_decl);


        Nal_unit()
            :forbidden_zero_bit(0)
            , nal_unit_type(0)
            , nuh_layer_id(0)
            , nuh_temporal_id_plus1(0)
            , start_ptr(nullptr)
            , NumBytesInNalUnit(0)
        { }
        Nal_unit(uint8_t* start_, size_t size_)
            : start_ptr(start_)
            , NumBytesInNalUnit(size_)
        {
            parse();
        }

        std::unique_ptr<Rbsp> rbsp_uptr;
        void parse();
    };

    std::list<Nal_unit> nalu_list;
    void parse(std::vector<uint8_t>& data_) {
        //�ù��̵��������ֽڵ���������ɣ���������ֽ�����һϵ���ֽ���NAL��Ԫ�﷨�ṹ��ɡ�
        //�ù��̵������һϵ��NAL��Ԫ�﷨�ṹ��ɡ�
        //�ڽ�����̵Ŀ�ʼ���������������ֽ����еĵ�ǰλ�ó�ʼ��Ϊ�ֽ����Ŀ�ʼ��
        //Ȼ����ȡ������ÿ��Leading_zero_8bits�﷨Ԫ�أ�������ڣ���
        //���ֽ����еĵ�ǰλ��һ����ǰ�ƶ�һ���ֽڣ�
        //ֱ���ֽ����еĵ�ǰλ��ʹ�ø�λ���е����ĸ��ֽ��γ��ĸ� �ֽ�����0x00000001��
        //Ȼ�󣬽������ظ�ִ�������𲽹��̣�����ȡ�ͽ����ֽ����е�ÿ��NAL��Ԫ�﷨�ṹ��
        //ֱ�������ֽ�����ĩβ����δָ���ķ�ʽȷ���������ѽ������ֽ����е����һ��NAL��Ԫ��
        //1.��λ���еĺ��ĸ��ֽ��γ����ֽ�����0x00000001ʱ��
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
        auto end_ptr = curr_ptr + data_.size();
        uint8_t* start = nullptr;
        uint8_t* end = nullptr;

        while (true) {
            if (*(size_t*)curr_ptr == 0x01000000) {
                curr_ptr += 4;
                start = curr_ptr;
                break;
            }
            else if (*(size_t*)curr_ptr == 0x010000) {
                curr_ptr += 3;
                start = curr_ptr;
                break;
            }
            else if (curr_ptr >= end_ptr) {
                wrn_show("û���ҵ�nal��Ԫ");
                return;
            }

            if (*curr_ptr != 0x00) {
                wrn_show("��׼�涨û������ͷ�ͱ���Ϊ0");
            }

            ++curr_ptr;
        }
        while (true) {
            if (*(size_t*)curr_ptr == 0x01000000) {
                nalu_list.emplace_back(start, curr_ptr - start);
                curr_ptr += 4;
                start = curr_ptr;
            }
            else if (*(size_t*)curr_ptr == 0x010000) {
                nalu_list.emplace_back(start, curr_ptr - start);
                curr_ptr += 3;
                start = curr_ptr;
            }
            else if (curr_ptr >= end_ptr) {
                wrn_show("�ļ�����");
                if (curr_ptr > start) {
                    nalu_list.emplace_back(start, curr_ptr - start);
                }
                break;
            }
            ++curr_ptr;
        }
    }



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







