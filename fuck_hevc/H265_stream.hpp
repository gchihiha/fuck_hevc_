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
        //该过程的输入由字节的有序流组成，该有序的字节流由一系列字节流NAL单元语法结构组成。
        //该过程的输出由一系列NAL单元语法结构组成。
        //在解码过程的开始，解码器将其在字节流中的当前位置初始化为字节流的开始。
        //然后提取并丢弃每个Leading_zero_8bits语法元素（如果存在），
        //将字节流中的当前位置一次向前移动一个字节，
        //直到字节流中的当前位置使得该位流中的下四个字节形成四个 字节序列0x00000001。
        //然后，解码器重复执行以下逐步过程，以提取和解码字节流中的每个NAL单元语法结构，
        //直到遇到字节流的末尾（由未指定的方式确定）并且已解码了字节流中的最后一个NAL单元：
        //1.当位流中的后四个字节形成四字节序列0x00000001时，
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
                wrn_show("没有找到nal单元");
                return;
            }

            if (*curr_ptr != 0x00) {
                wrn_show("标准规定没遇到开头就必须为0");
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
                wrn_show("文件结束");
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







