#ifndef _FREERTOS_BARECTF_BITFIELD_H
#define _FREERTOS_BARECTF_BITFIELD_H

#include <stdint.h>

/*
 * BabelTrace
 *
 * Bitfields read/write functions.
 *
 * Copyright (c) 2010-2020 Mathieu Desnoyers <mathieu.desnoyers@efficios.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 *
 * The following code was generated by barectf v3.1.1
 * on 2023-01-20T16:06:46.153461.
 *
 * For more details, see <https://barectf.org/>.
 */

#include <limits.h>

#ifdef __cplusplus
# define _CAST_PTR(_type, _value) \
	static_cast<_type>(static_cast<void *>(_value))
#else
# define _CAST_PTR(_type, _value)	((void *) (_value))
#endif

/* We can't shift a int from 32 bit, >> 32 and << 32 on int is undefined */
#define _bt_piecewise_rshift(_vtype, _v, _shift) \
do {									\
	unsigned long ___shift = (_shift);				\
	unsigned long sb = (___shift) / (sizeof(_v) * CHAR_BIT - 1);	\
	unsigned long final = (___shift) % (sizeof(_v) * CHAR_BIT - 1); \
									\
	for (; sb; sb--)						\
		_v >>= sizeof(_v) * CHAR_BIT - 1;			\
	_v >>= final;							\
} while (0)

/*
 * bt_bitfield_write - write integer to a bitfield in native endianness
 *
 * Save integer to the bitfield, which starts at the "start" bit, has "len"
 * bits.
 * The inside of a bitfield is from high bits to low bits.
 * Uses native endianness.
 * For unsigned "v", pad MSB with 0 if bitfield is larger than v.
 * For signed "v", sign-extend v if bitfield is larger than v.
 *
 * On little endian, bytes are placed from the less significant to the most
 * significant. Also, consecutive bitfields are placed from lower bits to higher
 * bits.
 *
 * On big endian, bytes are places from most significant to less significant.
 * Also, consecutive bitfields are placed from higher to lower bits.
 */

/* Native byte order: little-endian */

#define _bt_bitfield_write_le(_ptr, type, _start, _length, _vtype, _v)	\
do {									\
	_vtype __v = (_v);						\
	type *__ptr = _CAST_PTR(type *, _ptr);				\
	unsigned long __start = (_start), __length = (_length);		\
	type mask, cmask;						\
	unsigned long ts = sizeof(type) * CHAR_BIT; /* type size */	\
	unsigned long start_unit, end_unit, this_unit;			\
	unsigned long end, cshift; /* cshift is "complement shift" */	\
									\
	if (!__length)							\
		break;							\
									\
	end = __start + __length;					\
	start_unit = __start / ts;					\
	end_unit = (end + (ts - 1)) / ts;				\
									\
	/* Trim v high bits */						\
	if (__length < sizeof(__v) * CHAR_BIT)				\
		__v &= ~((~(_vtype) 0) << __length);			\
									\
	/* We can now append v with a simple "or", shift it piece-wise */ \
	this_unit = start_unit;						\
	if (start_unit == end_unit - 1) {				\
		mask = ~((~(type) 0) << (__start % ts));		\
		if (end % ts)						\
			mask |= (~(type) 0) << (end % ts);		\
		cmask = (type) __v << (__start % ts);			\
		cmask &= ~mask;						\
		__ptr[this_unit] &= mask;				\
		__ptr[this_unit] |= cmask;				\
		break;							\
	}								\
	if (__start % ts) {						\
		cshift = __start % ts;					\
		mask = ~((~(type) 0) << cshift);			\
		cmask = (type) __v << cshift;				\
		cmask &= ~mask;						\
		__ptr[this_unit] &= mask;				\
		__ptr[this_unit] |= cmask;				\
		_bt_piecewise_rshift(_vtype, __v, ts - cshift); 	\
		__start += ts - cshift;					\
		this_unit++;						\
	}								\
	for (; this_unit < end_unit - 1; this_unit++) {			\
		__ptr[this_unit] = (type) __v;				\
		_bt_piecewise_rshift(_vtype, __v, ts); 			\
		__start += ts;						\
	}								\
	if (end % ts) {							\
		mask = (~(type) 0) << (end % ts);			\
		cmask = (type) __v;					\
		cmask &= ~mask;						\
		__ptr[this_unit] &= mask;				\
		__ptr[this_unit] |= cmask;				\
	} else								\
		__ptr[this_unit] = (type) __v;				\
} while (0)

#define bt_bitfield_write_le(ptr, _start, _length, _vtype, _v) \
	_bt_bitfield_write_le(ptr, uint8_t, _start, _length, _vtype, _v)

#endif /* _FREERTOS_BARECTF_BITFIELD_H */
