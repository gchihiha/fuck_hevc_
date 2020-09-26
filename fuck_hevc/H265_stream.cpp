# include "H265_stream.hpp"

XEnum_def(H265_stream::Nal_unit::Rbsp::Type, Xt_Nal_unit_type);

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
        rbsp_uptr = std::make_unique<Rbsp>(rbsp_start, rbsp_end - rbsp_start);
        rbsp_uptr->parse();
    }
}
