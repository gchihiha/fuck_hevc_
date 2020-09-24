#pragma once

# include "common.hpp"
# include <cstdint>
# include <cstdlib>

# define bs_name

# define DECL_bitstream \
register size_t bs_name##_buf;\
register size_t bs_name##_buf2;\
register size_t bs_name##_bsPos;\
register size_t* bs_name##_ptr; \
size_t* bs_name##_data; \
size_t bs_name##_data_size;

constexpr size_t cpu_word_rShift_count() {
    switch (sizeof(size_t)) {
    case 1:
        return 3;
    case 2:
        return 4;
    case 4:
        return 5;
    case 8:
        return 6;
    default:
        return 0;
    }
}
constexpr size_t rBitmask(size_t bit_count) {
    return ((size_t)1 << bit_count) - 1;
}

# define word_pos (bs_name##_bsPos >> cpu_word_rShift_count())
# define byte_pos (bs_name##_bsPos >> 3)
# define word_offset (bs_name##_bsPos & rBitmask(cpu_word_rShift_count()))
# define byte_offset (bs_name##_bsPos & 0xFF)


# if defined(_WIN64)
    # define REG_A rax
    # define REG_B rbx
# elif defined(_WIN32)
    # define REG_A eax
    # define REG_B ebx

# endif

# if defined(_MSC_VER)
    # define rzc(ret__, n__) _asm{ bsf REG_A,n__; mov ret__,REG_A; }
    # define lzc(ret__, n__) _asm{ bsr REG_B,n__; mov ret__,REG_A; }

# elif defined(__GNUC__)
    # define rzc(ret__, n__) 
    # define lzc(ret__, n__) 

# endif
// 待办
# define flush_buf bs_name##_buf = bs_name##_data[word_offset];
# define flush_buf2 bs_name##_buf2 = bs_name##_data[word_offset];



/*
* byte_aligned（）指定如下：
–如果位流中的当前位置在字节边界上，即位流中的下一位是字节中的第一位，则byte_aligned（）的返回值等于TRUE。
–否则，byte_aligned（）的返回值等于FALSE。
*/
# define bs_byte_aligned (word_offset == 0)

/*
more_data_in_byte_stream（）指定如下：
–如果字节流中跟随更多数据，则more_data_in_byte_stream（）的返回值等于TRUE。
–否则，more_data_in_byte_stream（）的返回值等于FALSE。
–只能在附件B中指定的字节流NAL单元语法结构中使用
*/
# define bs_more_data_in_byte_stream (byte_pos < bs_name##_data_size)
/*
next_bits（n）提供位流中的下一位用于比较目的，而无需移动位流指针。
提供以n为参数的比特流的下n个位。 当在附件B中指定的字节流格式中使用并且字节流中剩余的位数少于n个时，next_bits（n）返回值0。
*/
# define bs_next(ret__, n__)    

/*
read_bits（n）从比特流中读取下一个n位，并将比特流指针前移n位。 当n等于0时，指定read_bits（n）以返回等于0的值并且不超前位流指针。
*/
# define bs_read(ret__, n__) 

/*具有任何位串模式（8位）的字节。 该描述符的解析过程由函数read_bits（8）的返回值指定。*/
# define bs_b8(ret__) 

/*使用n位的无符号整数。
此描述符的解析过程由函数read_bits（n）的返回值指定，该函数的值解释为无符号整数的二进制表示形式，其中最高有效位先写入。*/
# define bs_u(ret__, n__)

/*
固定模式的位字符串，使用从左到右写入的n位（从左到右）。 该描述符的解析过程由函数read_bits（n）的返回值指定。
*/
# define bs_f(ret__, n__)

/*
使用n位的有符号整数。 
该描述符的解析过程由函数read_bits（n）的返回值指定，该函数的值解释为二进制补码整数表示形式，其中最高有效位先写入。
*/
# define bs_i(ret__, n__)

/*
se（v）：带符号的整数0阶Exp-Golomb编码语法元素，其左位在前。 第9.2节规定了该描述符的解析过程。
*/
# define bs_se(ret__, v__)

/*
ae（v）：上下文自适应算术熵编码的语法元素。 9.3节规定了该描述符的解析过程。
*/
# define bs_ae(ret__, v__) 

/*
st（v）：空终止字符串，编码为ISO / IEC 10646中指定的通用编码字符集（UCS）传输格式8（UTF-8）字符。
解析过程指定如下：
st（v）开始于 比特流中的一个字节对齐位置，
并从比特流中读取并返回一系列字节，
从当前位置开始，一直延续到但不包括下一个等于0x00的字节对齐字节，并将比特流指针前进 （stringLength + 1）* 8位位置，
其中stringLength等于返回的字节数。
*/
# define bs_st(ret__, v__)

/*
ue（v）：无符号整数0阶Exp-Golomb编码语法元素，其左位在前。 
第9.2节规定了该描述符的解析过程。
*/
# define bs_ue(ret__, v__)









/*
more_data_in_payload（）指定如下：
–如果byte_aligned（）等于TRUE并且
    sei_payload（）语法结构中的当前位置是sei_payload（）语法结构开头的8 *有效负载大小位，则more_data_in_payload（）的返回值等于FALSE。
–否则，more_data_in_payload（）的返回值等于TRUE。
*/
# define bs_more_data_in_payload()

/*
more_rbsp_data（）的指定如下：
–如果原始字节序列有效载荷（RBSP）中没有更多数据，则more_rbsp_data（）的返回值等于FALSE。
–否则，将搜索RBSP数据中等于RBSP中存在的最后一位（最低有效，最右边）的位。 给定该位的位置，即rbsp_trailing_bits（）语法结构的第一位（rbsp_stop_one_bit），适用以下规则：
–如果在rbsp_trailing_bits（）语法结构之前RBSP中还有更多数据，则more_rbsp_data（）的返回值等于TRUE。
–否则，more_rbsp_data（）的返回值等于FALSE。
应用程序（或者在附件B中使用字节流格式的应用程序，在附件B中）指定了用于确定RBSP中是否有更多数据的方法。
*/
# define bs_more_rbsp_data ()


/*
more_rbsp_trailing_data（）指定如下：
–如果RBSP中有更多数据，则more_rbsp_trailing_data（）的返回值等于TRUE。
–否则，more_rbsp_trailing_data（）的返回值等于FALSE。
*/
# define bs_more_rbsp_trailing_data ()

/*
payload_extension_present（）指定如下：
–如果sei_payload（）语法结构中的当前位置不是等于1的最后（最低有效，最右边）位的位置，该位置小于语法结构开始处的8 *有效载荷大小位（即 （payload_bit_equal_to_one语法元素的位置），payload_extension_present（）的返回值
等于TRUE。
–否则，payload_extension_present（）的返回值等于FALSE。
*/
# define bs_payload_extension_present ()
