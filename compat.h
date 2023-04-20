#ifdef __FreeBSD__
#  include <sys/endian.h>
#elif defined(__APPLE_CC_) || defined(__MACH__)  /* MacOS/X support */
#  include <machine/endian.h>

#if    __DARWIN_BYTE_ORDER == __DARWIN_LITTLE_ENDIAN
#  define	htole16(x) (x)
#  define	le32toh(x) (x)
#elif  __DARWIN_BYTE_ORDER == __DARWIN_BIG_ENDIAN
#  define	htole16(x) __DARWIN_OSSwapInt16(x)
#  define	le32toh(x) __DARWIN_OSSwapInt32(x)
#else
#  error "Endianness is undefined"
#endif


#elif !defined(__WIN32__)
#  include <endian.h>
#endif

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <limits.h>
#ifndef __WIN32__
#include <sys/uio.h>
#endif

#ifdef __ANDROID__
#define le32toh letoh32
#endif

#if defined(__WIN32__) && defined(SG)
struct iovec {
	void *iov_base;	/* Pointer to data.  */
	size_t iov_len;	/* Length of data.  */
};
#endif

#if defined(__GNUC__) || defined(__clang__)
#  define ALWAYS_INLINE inline __attribute__((always_inline))
#elif defined(_MSC_VER)
#  define ALWAYS_INLINE __forceinline
#else
#  define ALWAYS_INLINE inline
#endif

#define DECLARE_GET_UNALIGNED(tx) \
static ALWAYS_INLINE tx get_unaligned_##tx(const void *x) { \
    tx ret; \
    memcpy(&ret, x, sizeof(tx)); \
    return ret; }

#define DECLARE_PUT_UNALIGNED(tx) \
static ALWAYS_INLINE void *put_unaligned_##tx(tx v, void *x) { \
    tx _v = (v); \
	return memcpy((x), &_v, sizeof(tx)); }

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned u32;
typedef unsigned long long u64;

DECLARE_GET_UNALIGNED(u16) // get_unaligned_u16
DECLARE_PUT_UNALIGNED(u16) // put_unaligned_u16
DECLARE_GET_UNALIGNED(u32) // get_unaligned_u32
DECLARE_PUT_UNALIGNED(u32) // put_unaligned_u32
DECLARE_GET_UNALIGNED(u64) // get_unaligned_u64
DECLARE_PUT_UNALIGNED(u64) // put_unaligned_u64

#define get_unaligned_le32(x) (le32toh(get_unaligned_u32(x)))
#define put_unaligned_le16(v,x) (put_unaligned_u16(htole16(v), x))

#define BUG_ON(x) assert(!(x))

#define vmalloc(x) malloc(x)
#define vfree(x) free(x)

#define EXPORT_SYMBOL(x)

#define ARRAY_SIZE(x) (sizeof(x) / sizeof(*(x)))

#if defined(_MSC_VER)
#  define likely(x) ((x) == 1)
#  define unlikely(x) ((x) == 0)
static __forceinline int __builtin_clz(u32 x)
{
    unsigned long index;
    _BitScanReverse(&index, x);
    return 31 - (int) index;
}
static __forceinline int __builtin_ctz(u32 x)
{
    unsigned long index;
    _BitScanForward(&index, x);
    return (int) index;
}
static __forceinline int __builtin_ctzll(u64 x)
{
    unsigned long index;
    _BitScanForward64(&index, x);
    return (int) index;
}
#else
#  define likely(x) __builtin_expect((x), 1)
#  define unlikely(x) __builtin_expect((x), 0)
#endif

#define min_t(t,x,y) ((x) < (y) ? (x) : (y))
#define max_t(t,x,y) ((x) > (y) ? (x) : (y))

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define __LITTLE_ENDIAN__ 1
#endif

#if __LITTLE_ENDIAN__ == 1 && (defined(__LSB_VERSION__) || defined(__WIN32__))
#define htole16(x) (x)
#define le32toh(x) (x)
#endif

#define BITS_PER_LONG (__SIZEOF_LONG__ * 8)
