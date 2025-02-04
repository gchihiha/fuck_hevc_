#pragma once

# include <cstdint>
# include <cassert>
# include "common.hpp"
# include <bitset>

struct BitStream {

    size_t* data;
    size_t bitPos;
    size_t buf;
    size_t size;

protected:
    static constexpr size_t cpu_bit_count() {
        return sizeof(size_t) * 8;
    }
    static constexpr size_t rBitmask(size_t bit_count) {
        return ((size_t)1 << bit_count) - 1;
    }

    inL size_t byte_pos() {
        return bitPos >> 3;
    }
    inL size_t byte_offset() {
        return bitPos & 0x07;
    }

    inL size_t word_pos() {
        return bitPos / (8 * sizeof(size_t));
    }
    inL size_t word_offset_mask() {
        return (8 * sizeof(size_t) - 1);
    }
    inL size_t word_offset() {
        return bitPos & word_offset_mask();
    }


# if defined(_MSC_VER)

# if defined(_WIN64)
# define REG_A rax
# define REG_B rbx
# define reverse(n__) _byteswap_uint64(n__)
# define rotr(v__, n__) _lrotr(v__, n__)

    

# elif defined(_WIN32)
# define REG_A eax
# define REG_B ebx
# define REG_C ecx

# define reverse(n__) _byteswap_ulong(n__)
# define rotr(v__, n__) _rotr(v__, n__)
    static_assert(sizeof(unsigned long) == 4, "sizeof(unsigned long) != 4");

    using uw = uint64_t;
# endif

# define rzc(ret__, n__) _asm{ bsf REG_A,n__; mov ret__,REG_A; }
# define lzc(ret__, n__) _asm{ bsr REG_B,n__; mov ret__,REG_A; }

# elif defined(__GNUC__)

# define rzc(ret__, n__) 
# define lzc(ret__, n__) 
# define reverse(n__) 

# endif

public:
    /*
    data_地址应满足cpu对内存地址对齐的要求
    如果不满足 则data_指向前面对齐的地址
    并且可访问空间大于等于实际空间+4字节
    */
    inL void init(uint8_t* data_, size_t size_) {
        data = (size_t*)((size_t)data_ & ~word_offset_mask());
        size = size_;
        bitPos = ((size_t)data_ & word_offset_mask()) * 8;
        flush_buf_when_byte_aligned();
    }
    inL uint8_t* byte_ptr() {
        return ((uint8_t*)data) + byte_pos();
    }
    inL uint8_t* end_byte_ptr() {
        return ((uint8_t*)&data[size]) + (sizeof(size_t) - 1);
    }
    inL void set_bitPos(uint8_t* ptr_) {
        assert(ptr_ > (uint8_t*)data);
        bitPos = (ptr_ - (uint8_t*)data) * 8;
    }

    inL bool byte_aligned() {
        return byte_offset() == 0;
    }
    inL void flush_buf_when_byte_aligned() {
        assert(byte_aligned());
        buf = reverse(*(size_t*)byte_ptr());
    }
    inL void flush_buf() {
        volatile auto pos = word_pos();
        volatile uint8_t lOffset = word_offset();
        volatile uint8_t rOffset = cpu_bit_count() - lOffset;
        auto temp1 = data[pos];
        auto temp2 = data[pos + 1];
        size_t ret = 0;
        __asm {
            push eax;
            push ebx;
            push ecx;

            mov eax, temp1;
            mov ebx, temp2;
            bswap eax;
            bswap ebx;

            xor cx, cx;
            mov ch, lOffset;
            xor cl, ch;
            shr ch, 5;
            sub cl, ch;
            shl eax, cl;
            mov cl, ch;
            shl eax, cl;

            xor cx, cx;
            mov ch, rOffset;
            xor cl, ch;
            shr ch, 5;
            sub cl, ch;
            shr ebx, cl;
            mov cl, ch;
            shr ebx, cl;

            or eax, ebx;
            mov ret, eax;

            pop ecx;
            pop ebx;
            pop eax;
        }
        buf = ret;
    }

    inL size_t word_side_find_byte0(size_t c_) {

    }
    inL size_t find_next_byte0() {

    }


    inL size_t next(size_t c_) {
        return buf >> (cpu_bit_count() - c_);
    }
    inL void skip(size_t c_) {
        buf <<= c_;
    }
    inL size_t read(size_t c_) {
        size_t ret = next(c_);
        buf <<= c_;
        bitPos += c_;
        return ret;
    }
    inL size_t b8() {
        return read(8);
    }
    inL size_t u(size_t c_) {
        return read(c_);
    }
    inL size_t f(size_t c_) {
        return read(c_);
    }
    inL ptrdiff_t i(size_t c_) {
        ptrdiff_t ret = 0;
        static_assert(sizeof(ptrdiff_t) == sizeof(size_t), "");
        ret |= (read(1) << (cpu_bit_count() - 1));
        ret |= read(c_ - 1);
        return ret;
    }



    inL void st(std::string& str_) {
        assert(byte_aligned());
        str_ = ((char*)data) + byte_pos();
        bitPos += 8 * (str_.size() + 1);
    }
    inL size_t ue() {
        auto z_count = bitPos;
        while (true) {
            size_t count = 0;
            __asm {
                push REG_A;
                push REG_B;
                mov REG_B, buf;
                bsf REG_A, REG_B;
                mov count, REG_A;
                pop REG_B;
                pop REG_A;
            }
            bitPos += count;
            flush_buf();
            if (count < cpu_bit_count()) {
                break;
            }
        }
        z_count = bitPos - z_count;
        // 是否该包含首个1?
        return ((1 << z_count) - 1) + read(z_count);
    }

    inL size_t se() {
        // 暂时没看懂 手册9.3
        return 0;
    }


    inL bool more_rbsp_data() {
        /*
        more_rbsp_data（）的指定如下：
            –如果原始字节序列有效载荷（RBSP）中没有更多数据，则more_rbsp_data（）的返回值等于FALSE。
            –否则，将搜索RBSP数据中等于RBSP中存在的最后一位（最低有效，最右边）的位。
                给定该位的位置，即rbsp_trailing_bits（）语法结构的第一位（rbsp_stop_one_bit），
                适用以下规则：
                –如果在rbsp_trailing_bits（）语法结构之前RBSP中还有更多数据 返回TRUE。
                –否则，返回 FALSE。
        应用程序（或者在附件B中使用字节流格式的应用程序，在附件B中）指定了用于确定RBSP中是否有更多数据的方法。
        */
        if (size * 8 <= bitPos) {
            return false;
        }
        BitStream temp = *this;
        return temp.rbsp_trailing_bits();
    }

    inL bool rbsp_trailing_bits() {
        uint8_t rbsp_stop_one_bit /* equal to 1 */ = f(1);
        if (rbsp_stop_one_bit != 1) {
            err_show("rbsp_stop_one_bit not equal to 1");
            return false;
        }
        while (!byte_aligned()) {
            uint8_t rbsp_alignment_zero_bit /* equal to 0 */ = f(1);
            if (rbsp_alignment_zero_bit != 0) {
                err_show("rbsp_alignment_zero_bit not equal to 0 ");
                return false;
            }
        }
        return true;
    }

};