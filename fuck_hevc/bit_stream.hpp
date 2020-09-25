#pragma once

# include "common.hpp"
# include <cstdint>
# include <cstdlib>

# define bs_name
# define Enable_REG 

# define DECL_bitstream_as_param \
size_t bs_name##_buf_, \
size_t bs_name##_buf2_, \
size_t bs_name##_bsPos_

# define USE_bitstream_to_func \
bs_name##_buf_, \
bs_name##_buf2_, \
bs_name##_bsPos_

# define DECL_bitstream_infunc \
Enable_REG size_t bs_name ## _buf = bs_name##_buf_; \
Enable_REG size_t bs_name ## _buf2 = bs_name##_buf2_; \
Enable_REG size_t bs_name ## _bsPos = bs_name##_bsPos_; \

# define DECL_bitstream \
static size_t*  bs_name ## _data; \
static size_t   bs_name ## _data_size; \
static size_t   bs_name ## _buf;\
static size_t   bs_name ## _buf2;\
static size_t   bs_name ## _bsPos;\
\
/*temp for use*/\
static size_t   bs_name ## _pos; \
static size_t   bs_name ## _offset;\
static size_t   bs_name ## _zCount;\

constexpr size_t cpu_bit_count() {
    return sizeof(size_t) * 8;
}
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

# define curr_byte_ptr ((uint8_t*)bs_name##_data)
# define curr_byte ( curr_byte_ptr[byte_pos] )

# define flush_buf_fast \
bs_name##_buf = bs_name##_data[word_pos]; bs_name##_buf=reverse(bs_name##_buf);

# define flush_buf \
bs_name##_pos = word_pos; bs_name##_offset = word_offset; \
bs_name##_buf = bs_name##_data[bs_name##_pos]; bs_name##_buf=reverse(bs_name##_buf); bs_name##_buf<<bs_name##_offset;\
bs_name##_buf2=bs_name##_data[bs_name##_offset+1]; bs_name##_buf |= bs_name##_buf2>>(cpu_bit_count()-bs_name##_offset);


# if defined(_MSC_VER)

    # if defined(_WIN64)
        # define REG_A rax
        # define REG_B rbx
        # define reverse(n__) _byteswap_uint64(n__)
        # define rotr(v__, n__) _lrotr(v__, n__)

    # elif defined(_WIN32)
        # define REG_A eax
        # define REG_B ebx
        # define reverse(n__) _byteswap_ulong(n__)
        # define rotr(v__, n__) _rotr64(v__, n__)
        static_assert(sizeof(unsigned long) == 4, "sizeof(unsigned long) != 4");
    # endif
    # define rzc(ret__, n__) _asm{ bsf REG_A,n__; mov ret__,REG_A; }
    # define lzc(ret__, n__) _asm{ bsr REG_B,n__; mov ret__,REG_A; }

# elif defined(__GNUC__)

    # define rzc(ret__, n__) 
    # define lzc(ret__, n__) 
    # define reverse(n__) 

# endif




/*
* byte_aligned����ָ�����£�
�C���λ���еĵ�ǰλ�����ֽڱ߽��ϣ���λ���е���һλ���ֽ��еĵ�һλ����byte_aligned�����ķ���ֵ����TRUE��
�C����byte_aligned�����ķ���ֵ����FALSE��
*/
# define bs_byte_aligned (word_offset == 0)

/*
more_data_in_byte_stream����ָ�����£�
�C����ֽ����и���������ݣ���more_data_in_byte_stream�����ķ���ֵ����TRUE��
�C����more_data_in_byte_stream�����ķ���ֵ����FALSE��
�Cֻ���ڸ���B��ָ�����ֽ���NAL��Ԫ�﷨�ṹ��ʹ��
*/
# define bs_more_data_in_byte_stream (byte_pos < bs_name##_data_size)
/*
next_bits��n���ṩλ���е���һλ���ڱȽ�Ŀ�ģ��������ƶ�λ��ָ�롣
�ṩ��nΪ�����ı���������n��λ�� ���ڸ���B��ָ�����ֽ�����ʽ��ʹ�ò����ֽ�����ʣ���λ������n��ʱ��next_bits��n������ֵ0��
*/
# define bs_next(n__) (bs_name##_buf & rBitmask(n__))

/*
read_bits��n���ӱ������ж�ȡ��һ��nλ������������ָ��ǰ��nλ�� ��n����0ʱ��ָ��read_bits��n���Է��ص���0��ֵ���Ҳ���ǰλ��ָ�롣
*/
# define bs_read(ret__, n__) ret__ = bs_next( n__); bs_name##_buf>>=n__; bs_name##_bsPos+=n__

/*�����κ�λ��ģʽ��8λ�����ֽڡ� ���������Ľ��������ɺ���read_bits��8���ķ���ֵָ����*/
# define bs_b8(ret__) bs_read(ret__, 8)

/*ʹ��nλ���޷���������
���������Ľ��������ɺ���read_bits��n���ķ���ֵָ�����ú�����ֵ����Ϊ�޷��������Ķ����Ʊ�ʾ��ʽ�����������Чλ��д�롣*/
# define bs_u(ret__, n__) bs_read(ret__, n__)

/*
ʹ��nλ���з���������
���������Ľ��������ɺ���read_bits��n���ķ���ֵָ�����ú�����ֵ����Ϊ�����Ʋ���������ʾ��ʽ�����������Чλ��д�롣
*/
# define bs_i(ret__, n__) 

/*
�̶�ģʽ��λ����ʹ�ô�����д���nλ�������ң��� ���������Ľ��������ɺ���read_bits��n���ķ���ֵָ����
*/
# define bs_f(ret__, n__) bs_read(ret__, n__)

/*
st��v��������ֹ�ַ���������ΪISO / IEC 10646��ָ����ͨ�ñ����ַ�����UCS�������ʽ8��UTF-8���ַ���
��������ָ�����£�
st��v����ʼ�� �������е�һ���ֽڶ���λ�ã�
���ӱ������ж�ȡ������һϵ���ֽڣ�
�ӵ�ǰλ�ÿ�ʼ��һֱ����������������һ������0x00���ֽڶ����ֽڣ�����������ָ��ǰ�� ��stringLength + 1��* 8λλ�ã�
����stringLength���ڷ��ص��ֽ�����
*/
# define bs_st(str__) str__ = curr_byte_ptr; bs_name##_bsPos += str__.size()*8


/*
ue��v�����޷�������0��Exp-Golomb�����﷨Ԫ�أ�����λ��ǰ��
��9.2�ڹ涨�˸��������Ľ������̡�
*/
# define bs_ue(ret__)\
bs_name##_zCount = bs_name##_bsPos; \
while(true) {\
size_t count = 0; \
rzc(count,bs_name##_buf); \
bs_name##_bsPos+=count; \
flush_buf; \
if(count < cpu_bit_count()){ break; } \
} \
bs_name##_zCount = bs_name##_bsPos-bs_name##_lz; \
ret__ = ((1<<bs_name##_zCount)-1) + bs_read(bs_name##_zCount)


/*
se��v���������ŵ�����0��Exp-Golomb�����﷨Ԫ�أ�����λ��ǰ�� ��9.2�ڹ涨�˸��������Ľ������̡�
*/
# define bs_se(ret__) bs_ue(ret__); (*(size_t*)&ret__) = rotr((size_t)ret__,1);











/*
ae��v��������������Ӧ�����ر�����﷨Ԫ�ء� 9.3�ڹ涨�˸��������Ľ������̡� ���� û����9.3
*/
# define bs_ae(ret__, v__) 

/*
more_data_in_payload����ָ�����£�
�C���byte_aligned��������TRUE����
    sei_payload�����﷨�ṹ�еĵ�ǰλ����sei_payload�����﷨�ṹ��ͷ��8 *��Ч���ش�Сλ����more_data_in_payload�����ķ���ֵ����FALSE��
�C����more_data_in_payload�����ķ���ֵ����TRUE��
*/
# define bs_more_data_in_payload()

/*
more_rbsp_data������ָ�����£�
�C���ԭʼ�ֽ�������Ч�غɣ�RBSP����û�и������ݣ���more_rbsp_data�����ķ���ֵ����FALSE��
�C���򣬽�����RBSP�����е���RBSP�д��ڵ����һλ�������Ч�����ұߣ���λ�� ������λ��λ�ã���rbsp_trailing_bits�����﷨�ṹ�ĵ�һλ��rbsp_stop_one_bit�����������¹���
�C�����rbsp_trailing_bits�����﷨�ṹ֮ǰRBSP�л��и������ݣ���more_rbsp_data�����ķ���ֵ����TRUE��
�C����more_rbsp_data�����ķ���ֵ����FALSE��
Ӧ�ó��򣨻����ڸ���B��ʹ���ֽ�����ʽ��Ӧ�ó����ڸ���B�У�ָ��������ȷ��RBSP���Ƿ��и������ݵķ�����
*/
# define bs_more_rbsp_data ()


/*
more_rbsp_trailing_data����ָ�����£�
�C���RBSP���и������ݣ���more_rbsp_trailing_data�����ķ���ֵ����TRUE��
�C����more_rbsp_trailing_data�����ķ���ֵ����FALSE��
*/
# define bs_more_rbsp_trailing_data ()

/*
payload_extension_present����ָ�����£�
�C���sei_payload�����﷨�ṹ�еĵ�ǰλ�ò��ǵ���1����������Ч�����ұߣ�λ��λ�ã���λ��С���﷨�ṹ��ʼ����8 *��Ч�غɴ�Сλ���� ��payload_bit_equal_to_one�﷨Ԫ�ص�λ�ã���payload_extension_present�����ķ���ֵ
����TRUE��
�C����payload_extension_present�����ķ���ֵ����FALSE��
*/
# define bs_payload_extension_present ()
