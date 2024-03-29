﻿#define _CRT_SECURE_NO_WARNINGS  // We do NOT support Microsoft's War on Standards.

#include <cstdint> // for int_fast16_t
#ifdef _DEBUG
// The testing function, in debug mode, writes stuff to standard output. 
#include <iostream>
#endif

// #include <immintrin.h>  // Using a union only, no AVX2.

typedef union  __declspec(intrin_type) __declspec(align(32)) __m256i {
    //__int8              m256i_i8[32];
    //__int16             m256i_i16[16];
    //__int32             m256i_i32[8];
    //__int64             m256i_i64[4];
    //unsigned __int8     m256i_u8[32];
    unsigned __int16    m256i_u16[16];
    //unsigned __int32    m256i_u32[8];
    unsigned __int64    m256i_u64[4];
} __m256i;

#include "sixteen_queens_common.h"

namespace qns16cmn
{
    // Using LibreOffice CALC to generate code.
    ALIGN_8Q flags_t row_masks[] = {
        {.m256i_u16 = {0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff, 0 }},
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xffff }},
    };

    static_assert(sizeof(row_masks) / sizeof(row_masks[0]) == maximum_allowed_board_size, "Literal array of wrong size here.");

    // Had an ugly bug here: 15 initializers per line, instead of 16.
    ALIGN_8Q flags_t column_masks[] = {
        {.m256i_u16 = {0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, 0x8000, } },
        {.m256i_u16 = {0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, 0x4000, } },
        {.m256i_u16 = {0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, 0x2000, } },
        {.m256i_u16 = {0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, 0x1000, } },
        {.m256i_u16 = {0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, 0x0800, } },
        {.m256i_u16 = {0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, 0x0400, } },
        {.m256i_u16 = {0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, 0x0200, } },
        {.m256i_u16 = {0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, 0x0100, } },
        {.m256i_u16 = {0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, 0x0080, } },
        {.m256i_u16 = {0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, 0x0040, } },
        {.m256i_u16 = {0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, 0x0020, } },
        {.m256i_u16 = {0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, 0x0010, } },
        {.m256i_u16 = {0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, 0x0008, } },
        {.m256i_u16 = {0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, 0x0004, } },
        {.m256i_u16 = {0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, 0x0002, } },
        {.m256i_u16 = {0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, 0x0001, } },
    };

    static_assert(sizeof(column_masks) / sizeof(column_masks[0]) == maximum_allowed_board_size, "Literal array of wrong size here.");


    ALIGN_8Q flags_t main_diagonal_parallels[] = {
        {.m256i_u16 = {0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, 0x0000, } },
        {.m256i_u16 = {0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, 0x0000, } },
        {.m256i_u16 = {0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001, } },
        {.m256i_u16 = {0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, 0x0008, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, 0x0010, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, 0x0020, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, 0x0040, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 0x0080, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, 0x0800, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, 0x1000, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, 0x2000, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, 0x4000, } },
        {.m256i_u16 = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x8000, } },
    };

    static_assert(sizeof(main_diagonal_parallels) / sizeof(main_diagonal_parallels[0]) == (maximum_allowed_board_size * 2 - 1),
        "Literal array of wrong size here.");

    ALIGN_8Q flags_t second_diagonal_parallels[] = {
        {.m256i_u16 = {0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, 0, } },
        {.m256i_u16 = {0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, 0, } },
        {.m256i_u16 = {0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, 0, } },
        {.m256i_u16 = {0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, 0, } },
        {.m256i_u16 = {0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, 0x8000, } },
        {.m256i_u16 = {0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, 0x4000, } },
        {.m256i_u16 = {0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, 0x2000, } },
        {.m256i_u16 = {0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, 0x1000, } },
        {.m256i_u16 = {0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, 0x0800, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x0400, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, 0x0008, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, 0x0004, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, 0x0002, } },
        {.m256i_u16 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x0001, } },
    };

    static_assert(sizeof(second_diagonal_parallels) / sizeof(second_diagonal_parallels[0]) == (maximum_allowed_board_size * 2 - 1),
        "Literal array of wrong size here.");


    const int sentinel = -1;
    // Save some time in allocations at run time. 
    std::vector<std::vector<int>> safe_indices{
        std::vector<int>(maximum_allowed_board_size, sentinel), //  0
        std::vector<int>(maximum_allowed_board_size, sentinel), //  1 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  2 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  3 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  4 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  5 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  6 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  7 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  8 
        std::vector<int>(maximum_allowed_board_size, sentinel), //  9 
        std::vector<int>(maximum_allowed_board_size, sentinel), // 10 
        std::vector<int>(maximum_allowed_board_size, sentinel), // 11 
        std::vector<int>(maximum_allowed_board_size, sentinel), // 12
        std::vector<int>(maximum_allowed_board_size, sentinel), // 13 
        std::vector<int>(maximum_allowed_board_size, sentinel), // 14 
        std::vector<int>(maximum_allowed_board_size, sentinel), // 15
    };

    const int max_solutions_to_show = 50;
    std::vector<std::vector<int>> solutions(max_solutions_to_show, std::vector<int>(16, -1));

    const std::vector<int>& not_threatened_rows(const map_t& map, int board_size, int current_column)
    {
        return not_threatened_rows_mt(map, board_size, current_column, safe_indices[current_column]);
    }

    const std::vector<int>& not_threatened_rows_mt(const map_t& map, int board_size, int current_column, std::vector<int>& result)
    {
        // PRECONDITION: MASK BEFORE CALLING! const map_t mask = (map & column_masks[current_column]);
        const unsigned short* pRow = map.m256i_u16;
        int j = 0;

        // Manual loop unrolling, ugly as it looks, but fastest thing I found (so far).
        // To allow several board sizes (4-16), we'll use something inspired in the Duff Device.
        int_fast16_t i = -1;
        switch (board_size)
        {
        case 16:
        default:
            if (!pRow[++i]) result[j++] = i;
        case 15:
            if (!pRow[++i]) result[j++] = i;
        case 14:
            if (!pRow[++i]) result[j++] = i;
        case 13:
            if (!pRow[++i]) result[j++] = i;
        case 12:
            if (!pRow[++i]) result[j++] = i;
        case 11:
            if (!pRow[++i]) result[j++] = i;
        case 10:
            if (!pRow[++i]) result[j++] = i;
        case 9:
            if (!pRow[++i]) result[j++] = i;
        case 8:
            if (!pRow[++i]) result[j++] = i;
        case 7:
            if (!pRow[++i]) result[j++] = i;
        case 6:
            if (!pRow[++i]) result[j++] = i;
        case 5:
            if (!pRow[++i]) result[j++] = i;
        case 4:
            if (!pRow[++i]) result[j++] = i;
            if (!pRow[++i]) result[j++] = i;
            if (!pRow[++i]) result[j++] = i;
            if (!pRow[++i]) result[j++] = i;
        }
        result[j] = sentinel;
        return result;
    }
   

#ifdef _DEBUG
    namespace dbg
    {
        void show_map(const map_t& map, const std::vector<int>& solution, int board_size)
        {
            using std::cout;
            using std::endl;
            for (int row = 0; row < 16; ++row)
            {
                unsigned short current_row = map.m256i_u16[row];
                for (int col = 0; col < 16; ++col)
                {
                    if (col < solution.size() && solution[col] == row)
                    {
                        cout << 'Q';
                        continue;
                    }
                    unsigned short flag = column_masks[col].m256i_u16[0];
                    if ((flag & current_row) == flag)
                    {
                        cout << '*';
                        continue;
                    }
                    if (row >= board_size || col >= board_size)
                    {
                        cout << 'X';
                        continue;
                    }
                    cout << ((row + col) % 2 ? '+' : '-');
                }
                cout << endl;
            }
        }
    }
#endif

    void test()
    {

#ifdef _DEBUG
        constexpr int board_size = 16;
        std::vector<int> solution(board_size, -1);

        std::cout << "Column masks [0]" << std::endl;
        dbg::show_map(column_masks[0], solution, board_size);

        std::cout << "Column masks [1]" << std::endl;
        dbg::show_map(column_masks[1], solution, board_size);

        std::cout << "Column masks [2]" << std::endl;
        dbg::show_map(column_masks[2], solution, board_size);

        std::cout << "main_diagonal_parallels[0]" << std::endl;
        dbg::show_map(main_diagonal_parallels[0], solution, board_size);

        std::cout << "main_diagonal_parallels[1]" << std::endl;
        dbg::show_map(main_diagonal_parallels[1], solution, board_size);

        std::cout << "main_diagonal_parallels[2]" << std::endl;
        dbg::show_map(main_diagonal_parallels[2], solution, board_size);

        std::cout << "main_diagonal_parallels[7]" << std::endl;
        dbg::show_map(main_diagonal_parallels[7], solution, board_size);

        std::cout << "main_diagonal_parallels[8]" << std::endl;
        dbg::show_map(main_diagonal_parallels[8], solution, board_size);

        std::cout << "main_diagonal_parallels[15]" << std::endl;
        dbg::show_map(main_diagonal_parallels[15], solution, board_size);



        std::cout << "second_diagonal_parallels[0]" << std::endl;
        dbg::show_map(second_diagonal_parallels[0], solution, board_size);

        std::cout << "second_diagonal_parallels[1]" << std::endl;
        dbg::show_map(second_diagonal_parallels[1], solution, board_size);

        std::cout << "second_diagonal_parallels[2]" << std::endl;
        dbg::show_map(second_diagonal_parallels[2], solution, board_size);

        std::cout << "second_diagonal_parallels[7]" << std::endl;
        dbg::show_map(second_diagonal_parallels[7], solution, board_size);

        std::cout << "second_diagonal_parallels[8]" << std::endl;
        dbg::show_map(second_diagonal_parallels[8], solution, board_size);

        std::cout << "second_diagonal_parallels[15]" << std::endl;
        dbg::show_map(second_diagonal_parallels[15], solution, board_size);

#endif // _DEBUG
    }
} // namespace qns16cmn
