# pragma once
# include "BitStream.hpp"
# include <iostream>
# include <vector>
# include <list>
# include <memory>

# include "common.hpp"
# include "xtable.hpp"
# include "Element.hpp"


struct H265_stream:public BitStream {
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
            using Cb_new_rbsp = std::unique_ptr<Rbsp>(*)(uint8_t* start_, size_t size_);
            static Cb_new_rbsp on_new[Type::e_max_num];
            inL static std::unique_ptr<Rbsp> get_new(Type::E type_, uint8_t* start_, size_t size_) {
                auto uptr = on_new[type_](start_, size_);
                uptr->type = type_;
                return std::move(uptr);
            }

            Type::E type;

            uint8_t* start;
            size_t size;

            Rbsp(uint8_t* start_, size_t size_)
                : start(start_)
                , size(size_)
            {}
        };

        struct Rbsp_VPS_NUT :public Rbsp {
            uint8_t vps_video_parameter_set_id   ;
            uint8_t vps_base_layer_internal_flag ;
            uint8_t vps_base_layer_available_flag;
            uint8_t vps_max_layers_minus1        ;
            uint8_t vps_max_sub_layers_minus1    ;
            uint8_t vps_temporal_id_nesting_flag ;
            uint8_t vps_sub_layer_ordering_info_present_flag;
            uint8_t vps_max_dec_pic_buffering_minus1[8];/* [0, MaxDpbSize-1] */
            uint8_t vps_max_num_reorder_pics[8];/* [0, vps_max_dec_pic_buffering_minus1[i] ] */
            uint8_t vps_max_layer_id;
            uint16_t vps_num_layer_sets_minus1;/* [0,1023] */
            size_t vps_max_latency_increase_plus1[8];
            Profile_tier_level profile_tier_level; // profile_tier_level( 1, vps_max_sub_layers_minus1 )
            uint16_t vps_reserved_0xffff_16bits  ;

            uint8_t layer_id_included_flag[1024][64];
            uint8_t vps_timing_info_present_flag;
            uint8_t vps_poc_proportional_to_timing_flag;
            uint32_t vps_num_units_in_tick;
            uint32_t vps_time_scale;
            uint32_t vps_num_ticks_poc_diff_one_minus1;

            uint16_t vps_num_hrd_parameters;/*[0, vps_num_layer_sets_minus1+1]*/
            struct Vps_hrd_parameters {
                uint16_t hrd_layer_set_idx;
                uint8_t cprms_present_flag;
                Hrd_parameters hrd_parameters;
            };
            Vps_hrd_parameters vps_hrd_parameters[1024];

            uint8_t vps_extension_flag;
            std::vector<bool> vps_extension_data_flag;

            Rbsp_VPS_NUT(uint8_t* start_, size_t size);
        };


        struct Rbsp_TRAIL_N	      :public Rbsp{
            Rbsp_TRAIL_N(uint8_t* start_, size_t size);
        };
        struct Rbsp_TRAIL_R	      :public Rbsp{
            Rbsp_TRAIL_R(uint8_t* start_, size_t size);
        };
        struct Rbsp_TSA_N	      :public Rbsp{
            Rbsp_TSA_N(uint8_t* start_, size_t size);
        };
        struct Rbsp_TSA_R	      :public Rbsp{
            Rbsp_TSA_R(uint8_t* start_, size_t size);
        };
        struct Rbsp_STSA_N	      :public Rbsp{
            Rbsp_STSA_N(uint8_t* start_, size_t size);
        };
        struct Rbsp_STSA_R	      :public Rbsp{
            Rbsp_STSA_R(uint8_t* start_, size_t size);
        };
        struct Rbsp_RADL_N	      :public Rbsp{
            Rbsp_RADL_N(uint8_t* start_, size_t size);
        };
        struct Rbsp_RADL_R	      :public Rbsp{
            Rbsp_RADL_R(uint8_t* start_, size_t size);
        };
        struct Rbsp_RASL_N	      :public Rbsp{
            Rbsp_RASL_N(uint8_t* start_, size_t size);
        };
        struct Rbsp_RASL_R	      :public Rbsp{
            Rbsp_RASL_R(uint8_t* start_, size_t size);
        };
        struct Rbsp_BLA_W_LP	  :public Rbsp{
            Rbsp_BLA_W_LP(uint8_t* start_, size_t size);
        };
        struct Rbsp_BLA_W_RADL	  :public Rbsp{
            Rbsp_BLA_W_RADL(uint8_t* start_, size_t size);
        };
        struct Rbsp_BLA_N_LP	  :public Rbsp{
            Rbsp_BLA_N_LP(uint8_t* start_, size_t size);
        };
        struct Rbsp_IDR_W_RADL	  :public Rbsp{
            Rbsp_IDR_W_RADL(uint8_t* start_, size_t size);
        };
        struct Rbsp_IDR_N_LP	  :public Rbsp{
            Rbsp_IDR_N_LP(uint8_t* start_, size_t size);
        };
        struct Rbsp_CRA_NUT	      :public Rbsp{
            Rbsp_CRA_NUT(uint8_t* start_, size_t size);
        };
        struct Rbsp_SPS_NUT	      :public Rbsp{
            Rbsp_SPS_NUT(uint8_t* start_, size_t size);
        };
        struct Rbsp_PPS_NUT	      :public Rbsp{
            Rbsp_PPS_NUT(uint8_t* start_, size_t size);
        };
        struct Rbsp_AUD_NUT	      :public Rbsp{
            Rbsp_AUD_NUT(uint8_t* start_, size_t size);
        };
        struct Rbsp_EOS_NUT	      :public Rbsp{
            Rbsp_EOS_NUT(uint8_t* start_, size_t size);
        };
        struct Rbsp_EOB_NUT	      :public Rbsp{
            Rbsp_EOB_NUT(uint8_t* start_, size_t size);
        };
        struct Rbsp_FD_NUT	      :public Rbsp{
            Rbsp_FD_NUT(uint8_t* start_, size_t size);
        };
        struct Rbsp_PREFIX_SEI_NUT:public Rbsp{
            Rbsp_PREFIX_SEI_NUT(uint8_t* start_, size_t size);
        };
        struct Rbsp_SUFFIX_SEI_NUT:public Rbsp{
            Rbsp_SUFFIX_SEI_NUT(uint8_t* start_, size_t size);
        };

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


# if 1
        struct Rbsp_RSV_VCL_N10 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL_R11 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL_N12 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL_R13 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL_N14 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL_R15 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_IRAP_VCL22 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_IRAP_VCL23 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL24 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL25 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL26 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL27 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL28 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL29 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL30 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_VCL31 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_NVCL41 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_NVCL42 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_NVCL43 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_NVCL44 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_NVCL45 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_NVCL46 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_RSV_NVCL47 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC48 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC49 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC50 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC51 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC52 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC53 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC54 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC55 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC56 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC57 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC58 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC59 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC60 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC61 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC62 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
        struct Rbsp_UNSPEC63 :public H265_stream::Nal_unit::Rbsp { using Rbsp::Rbsp; };
# endif
    };

    using BitStream::BitStream;
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












    void test();
};







