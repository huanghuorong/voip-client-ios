/*
 * Copyright (c) 2010 Mans Rullgard <mans@mansr.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVUTIL_ARM_INTMATH_H
#define AVUTIL_ARM_INTMATH_H

#include <stdint.h>

#include "config.h"
#include "libavutil/attributes.h"

#if HAVE_INLINE_ASM

#if HAVE_ARMV6

#define FASTDIV FASTDIV
static av_always_inline av_const int FASTDIV(int a, int b)
{
    int r, t;
    __asm__ volatile("cmp     %3, #2               \n\t"
                     "ldr     %1, [%4, %3, lsl #2] \n\t"
                     "lsrle   %0, %2, #1           \n\t"
                     "smmulgt %0, %1, %2           \n\t"
                     : "=&r"(r), "=&r"(t) : "r"(a), "r"(b), "r"(ff_inverse));
    return r;
}

#define av_clip_uint8 av_clip_uint8_arm
static av_always_inline av_const uint8_t av_clip_uint8_arm(int a)
{
    unsigned x;
    __asm__ volatile ("usat %0, #8,  %1" : "=r"(x) : "r"(a));
    return x;
}

#define av_clip_int8 av_clip_int8_arm
static av_always_inline av_const uint8_t av_clip_int8_arm(int a)
{
    unsigned x;
    __asm__ volatile ("ssat %0, #8,  %1" : "=r"(x) : "r"(a));
    return x;
}

#define av_clip_uint16 av_clip_uint16_arm
static av_always_inline av_const uint16_t av_clip_uint16_arm(int a)
{
    unsigned x;
    __asm__ volatile ("usat %0, #16, %1" : "=r"(x) : "r"(a));
    return x;
}

#define av_clip_int16 av_clip_int16_arm
static av_always_inline av_const int16_t av_clip_int16_arm(int a)
{
    int x;
    __asm__ volatile ("ssat %0, #16, %1" : "=r"(x) : "r"(a));
    return x;
}

#else /* HAVE_ARMV6 */

#define FASTDIV FASTDIV
static av_always_inline av_const int FASTDIV(int a, int b)
{
    int r, t;
    __asm__ volatile("umull %1, %0, %2, %3"
                     : "=&r"(r), "=&r"(t) : "r"(a), "r"(ff_inverse[b]));
    return r;
}

#endif /* HAVE_ARMV6 */

#define av_clipl_int32 av_clipl_int32_arm
static av_always_inline av_const int32_t av_clipl_int32_arm(int64_t a)
{
    int x, y;
    union { uint64_t a; uint32_t hl[2]; } tmp_a;
    tmp_a.a=a;
    __asm__ volatile ("adds   %1, %2, %3, lsr #31  \n\t"
                      "mvnne  %1, #1<<31             \n\t"
                      "eorne  %0, %1,  %2, asr #31  \n\t"
                      : "=r"(x), "=&r"(y) : "r"(tmp_a.hl[0]),"r"(tmp_a.hl[1]));
    return x;
}

#endif /* HAVE_INLINE_ASM */

#endif /* AVUTIL_ARM_INTMATH_H */
