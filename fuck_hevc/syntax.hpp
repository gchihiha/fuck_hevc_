# pragma once
# include "bit_stream.hpp"

# include <iostream>
# include <vector>

# define dbg_show(...)      std::cout<< __FILE__ <<" "<<__LINE__<<" "<<__VA_ARGS__<<std::endl;
# define wrn_show(...)      std::cout<< __FILE__ <<" "<<__LINE__<<" "<<__VA_ARGS__<<std::endl;
# define err_show(...)      std::cout<< __FILE__ <<" "<<__LINE__<<" "<<__VA_ARGS__<<std::endl;
# define detail_show(...)   std::cout<< __FILE__ <<" "<<__LINE__<<" "<<__VA_ARGS__<<std::endl;

struct H265 {

# define undef bs_name
# define undef Enable_REG

# define bs_name h264bs
# define Enable_REG register

    DECL_bitstream;

    struct Nal_unit {
        static constexpr size_t nalu_reserve = 8192;

        uint8_t forbidden_zero_bit;
        uint8_t nal_unit_type;
        uint8_t nuh_layer_id;
        uint8_t nuh_temporal_id_plus1;
        std::vector<uint8_t> data;

        Nal_unit()
            :forbidden_zero_bit(0)
            , nal_unit_type(0)
            , nuh_layer_id(0)
            , nuh_temporal_id_plus1(0)
            , data(0)
        {
            data.reserve(nalu_reserve);
        }
        void read(size_t NumBytesInNalUnit) {
            
            flush_buf_fast;
            bs_f(forbidden_zero_bit, 1);
            bs_u(nal_unit_type, 6);
            bs_u(nuh_layer_id, 6);
            bs_u(nuh_temporal_id_plus1, 3);

            data.clear();
            uint8_t curr_data;

            for (size_t i = 2; i < NumBytesInNalUnit; i++) {
                bs_read(curr_data, 8);
                data.push_back(curr_data);

                if ((i + 2) < NumBytesInNalUnit && bs_next(24) == 0x000003) {
                    bs_read(curr_data, 8);
                    data.push_back(curr_data);
                    i += 2;
                    uint8_t emulation_prevention_three_byte;
                    bs_f(emulation_prevention_three_byte, 8);   /* equal to 0x03 */
                    if (emulation_prevention_three_byte != 0x3) {
                        err_show("emulation_prevention_three_byte");
                    }
                }
                bs_read(curr_data, 8);
                data.push_back(curr_data);
            }

        }

    };

    static void nal_unit(std::vector<uint32_t>&data_) {



    }




};







