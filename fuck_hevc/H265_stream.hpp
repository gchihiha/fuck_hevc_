# pragma once
# include "BitStream.hpp"
# include <iostream>
# include <vector>
# include <list>
# include <memory>

# include "common.hpp"
# include "xtable.hpp"
# include "Element.hpp"

struct Rbsp {

    XEnum_decl(Type, Xt_Nal_unit_type);
    using Cb_new_rbsp = std::unique_ptr<Rbsp>(*)(std::vector<uint8_t>& data_);
    static Cb_new_rbsp on_new[Type::e_max_num];

    Type::E type;
    std::vector<uint8_t> data;

    Rbsp(std::vector<uint8_t>& data_) :data(std::move(data_)) {}

    inL static std::unique_ptr<Rbsp>
        get_new(Type::E type_, std::vector<uint8_t>& data_)
    {
        auto uptr = on_new[type_](data_);
        uptr->type = type_;
        return std::move(uptr);
    }
};

struct Rbsp_VPS_NUT :public Rbsp {
    uint8_t vps_video_parameter_set_id;
    uint8_t vps_base_layer_internal_flag;
    uint8_t vps_base_layer_available_flag;
    uint8_t vps_max_layers_minus1;
    uint8_t vps_max_sub_layers_minus1;
    uint8_t vps_temporal_id_nesting_flag;
    uint8_t vps_sub_layer_ordering_info_present_flag;
    uint8_t vps_max_dec_pic_buffering_minus1[8];/* [0, MaxDpbSize-1] */
    uint8_t vps_max_num_reorder_pics[8];/* [0, vps_max_dec_pic_buffering_minus1[i] ] */
    uint8_t vps_max_layer_id;
    uint16_t vps_num_layer_sets_minus1;/* [0,1023] */
    size_t vps_max_latency_increase_plus1[8];
    Profile_tier_level profile_tier_level; // profile_tier_level( 1, vps_max_sub_layers_minus1 )
    uint16_t vps_reserved_0xffff_16bits;

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

    Rbsp_VPS_NUT(std::vector<uint8_t>& data_);
};

# if 1
struct Rbsp_TRAIL_N :public Rbsp {
    Rbsp_TRAIL_N(std::vector<uint8_t>& data_);
};
struct Rbsp_TRAIL_R :public Rbsp {
    Rbsp_TRAIL_R(std::vector<uint8_t>& data_);
};
struct Rbsp_TSA_N :public Rbsp {
    Rbsp_TSA_N(std::vector<uint8_t>& data_);
};
struct Rbsp_TSA_R :public Rbsp {
    Rbsp_TSA_R(std::vector<uint8_t>& data_);
};
struct Rbsp_STSA_N :public Rbsp {
    Rbsp_STSA_N(std::vector<uint8_t>& data_);
};
struct Rbsp_STSA_R :public Rbsp {
    Rbsp_STSA_R(std::vector<uint8_t>& data_);
};
struct Rbsp_RADL_N :public Rbsp {
    Rbsp_RADL_N(std::vector<uint8_t>& data_);
};
struct Rbsp_RADL_R :public Rbsp {
    Rbsp_RADL_R(std::vector<uint8_t>& data_);
};
struct Rbsp_RASL_N :public Rbsp {
    Rbsp_RASL_N(std::vector<uint8_t>& data_);
};
struct Rbsp_RASL_R :public Rbsp {
    Rbsp_RASL_R(std::vector<uint8_t>& data_);
};
struct Rbsp_BLA_W_LP :public Rbsp {
    Rbsp_BLA_W_LP(std::vector<uint8_t>& data_);
};
struct Rbsp_BLA_W_RADL :public Rbsp {
    Rbsp_BLA_W_RADL(std::vector<uint8_t>& data_);
};
struct Rbsp_BLA_N_LP :public Rbsp {
    Rbsp_BLA_N_LP(std::vector<uint8_t>& data_);
};
struct Rbsp_IDR_W_RADL :public Rbsp {
    Rbsp_IDR_W_RADL(std::vector<uint8_t>& data_);
};
struct Rbsp_IDR_N_LP :public Rbsp {
    Rbsp_IDR_N_LP(std::vector<uint8_t>& data_);
};
struct Rbsp_CRA_NUT :public Rbsp {
    Rbsp_CRA_NUT(std::vector<uint8_t>& data_);
};
struct Rbsp_SPS_NUT :public Rbsp {
    Rbsp_SPS_NUT(std::vector<uint8_t>& data_);
};
struct Rbsp_PPS_NUT :public Rbsp {
    Rbsp_PPS_NUT(std::vector<uint8_t>& data_);
};
struct Rbsp_AUD_NUT :public Rbsp {
    Rbsp_AUD_NUT(std::vector<uint8_t>& data_);
};
struct Rbsp_EOS_NUT :public Rbsp {
    Rbsp_EOS_NUT(std::vector<uint8_t>& data_);
};
struct Rbsp_EOB_NUT :public Rbsp {
    Rbsp_EOB_NUT(std::vector<uint8_t>& data_);
};
struct Rbsp_FD_NUT :public Rbsp {
    Rbsp_FD_NUT(std::vector<uint8_t>& data_);
};
struct Rbsp_PREFIX_SEI_NUT :public Rbsp {
    Rbsp_PREFIX_SEI_NUT(std::vector<uint8_t>& data_);
};
struct Rbsp_SUFFIX_SEI_NUT :public Rbsp {
    Rbsp_SUFFIX_SEI_NUT(std::vector<uint8_t>& data_);
};
# endif

# if 1
struct Rbsp_RSV_VCL_N10 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL_R11 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL_N12 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL_R13 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL_N14 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL_R15 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_IRAP_VCL22 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_IRAP_VCL23 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL24 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL25 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL26 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL27 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL28 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL29 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL30 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_VCL31 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_NVCL41 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_NVCL42 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_NVCL43 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_NVCL44 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_NVCL45 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_NVCL46 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_RSV_NVCL47 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC48 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC49 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC50 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC51 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC52 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC53 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC54 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC55 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC56 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC57 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC58 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC59 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC60 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC61 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC62 :public Rbsp { using Rbsp::Rbsp; };
struct Rbsp_UNSPEC63 :public Rbsp { using Rbsp::Rbsp; };
# endif

struct Nal_unit {
    static constexpr size_t nalu_reserve = 8192;

    uint8_t* start_ptr;
    size_t NumBytesInNalUnit;

    uint8_t forbidden_zero_bit;
    uint8_t nal_unit_type;
    uint8_t nuh_layer_id;
    uint8_t nuh_temporal_id_plus1;
    Nal_unit(std::vector<uint8_t>& data_);

    std::unique_ptr<Rbsp> rbsp_uptr;
};


struct H265_stream {
    std::list<Nal_unit> nalu_list;

    H265_stream(std::vector<uint8_t>& data_);
    static void test();
};







