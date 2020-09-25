#pragma once

# include <cstdint>
# include <cassert>
# include "common.hpp"

struct BitStream {

    size_t* data;
    size_t bitPos;
    size_t buf;
    size_t size;

protected:
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

    inL size_t byte_pos() {
        return bitPos >> 3;
    }
    inL size_t byte_offset() {
        return bitPos & rBitmask(3);
    }

    inL size_t word_pos() {
        return bitPos >> cpu_word_rShift_count();
    }
    inL size_t word_offset() {
        return bitPos & rBitmask(cpu_word_rShift_count());
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
# define reverse(n__) _byteswap_ulong(n__)
# define rotr(v__, n__) _rotr(v__, n__)
    static_assert(sizeof(unsigned long) == 4, "sizeof(unsigned long) != 4");

# endif

# define rzc(ret__, n__) _asm{ bsf REG_A,n__; mov ret__,REG_A; }
# define lzc(ret__, n__) _asm{ bsr REG_B,n__; mov ret__,REG_A; }

# elif defined(__GNUC__)

# define rzc(ret__, n__) 
# define lzc(ret__, n__) 
# define reverse(n__) 

# endif

public:
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
        buf = *(size_t*)byte_ptr();
    }
    inL void flush_buf() {
        auto pos = word_pos();
        auto offset = word_offset();

        buf = data[pos];
        buf = reverse(buf);
        buf <<= offset;
        buf |= data[offset + 1] >> (cpu_bit_count() - offset);
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
                mov ebx, buf;
                bsf eax, ebx;
                mov count, eax;
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

};