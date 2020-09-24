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
// ����
# define flush_buf bs_name##_buf = bs_name##_data[word_offset];
# define flush_buf2 bs_name##_buf2 = bs_name##_data[word_offset];



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
# define bs_next(ret__, n__)    

/*
read_bits��n���ӱ������ж�ȡ��һ��nλ������������ָ��ǰ��nλ�� ��n����0ʱ��ָ��read_bits��n���Է��ص���0��ֵ���Ҳ���ǰλ��ָ�롣
*/
# define bs_read(ret__, n__) 

/*�����κ�λ��ģʽ��8λ�����ֽڡ� ���������Ľ��������ɺ���read_bits��8���ķ���ֵָ����*/
# define bs_b8(ret__) 

/*ʹ��nλ���޷���������
���������Ľ��������ɺ���read_bits��n���ķ���ֵָ�����ú�����ֵ����Ϊ�޷��������Ķ����Ʊ�ʾ��ʽ�����������Чλ��д�롣*/
# define bs_u(ret__, n__)

/*
�̶�ģʽ��λ�ַ�����ʹ�ô�����д���nλ�������ң��� ���������Ľ��������ɺ���read_bits��n���ķ���ֵָ����
*/
# define bs_f(ret__, n__)

/*
ʹ��nλ���з��������� 
���������Ľ��������ɺ���read_bits��n���ķ���ֵָ�����ú�����ֵ����Ϊ�����Ʋ���������ʾ��ʽ�����������Чλ��д�롣
*/
# define bs_i(ret__, n__)

/*
se��v���������ŵ�����0��Exp-Golomb�����﷨Ԫ�أ�����λ��ǰ�� ��9.2�ڹ涨�˸��������Ľ������̡�
*/
# define bs_se(ret__, v__)

/*
ae��v��������������Ӧ�����ر�����﷨Ԫ�ء� 9.3�ڹ涨�˸��������Ľ������̡�
*/
# define bs_ae(ret__, v__) 

/*
st��v��������ֹ�ַ���������ΪISO / IEC 10646��ָ����ͨ�ñ����ַ�����UCS�������ʽ8��UTF-8���ַ���
��������ָ�����£�
st��v����ʼ�� �������е�һ���ֽڶ���λ�ã�
���ӱ������ж�ȡ������һϵ���ֽڣ�
�ӵ�ǰλ�ÿ�ʼ��һֱ����������������һ������0x00���ֽڶ����ֽڣ�����������ָ��ǰ�� ��stringLength + 1��* 8λλ�ã�
����stringLength���ڷ��ص��ֽ�����
*/
# define bs_st(ret__, v__)

/*
ue��v�����޷�������0��Exp-Golomb�����﷨Ԫ�أ�����λ��ǰ�� 
��9.2�ڹ涨�˸��������Ľ������̡�
*/
# define bs_ue(ret__, v__)









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
