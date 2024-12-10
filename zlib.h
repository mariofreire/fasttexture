//---------------------------------------------------------------------------

#ifndef zlibH
#define zlibH
//---------------------------------------------------------------------------

#define ZLIB_VERSION "1.2.3"

#ifndef FAR
#define FAR
#endif
         
#ifndef local
#define local static
#endif

#ifndef OF
#define OF(args)  args
#endif
                     
#ifndef z_off_t
#define z_off_t long
#endif

#define MAX_BITS 15
#define LENGTH_CODES 29
#define LITERALS  256         
#define D_CODES   30
#define BL_CODES  19
#define L_CODES (LITERALS+1+LENGTH_CODES)
#define HEAP_SIZE (2*L_CODES+1)

#define MAX_MEM_LEVEL 9

#ifndef TOO_FAR
#define TOO_FAR 4096
#endif

typedef unsigned char  u8;    
typedef unsigned short  u16;
typedef unsigned long  u32;
typedef unsigned int   uint;
                             
typedef u8  FAR u8f;
typedef u16 FAR u16f;
typedef u32 FAR u32f;

typedef void const *voidpc;
typedef void FAR   *voidpf;
typedef void       *voidp;  


typedef char  FAR charf;
typedef int   FAR intf;
typedef uint  FAR uintf;
                           
typedef u16 Pos;
typedef Pos FAR Posf;
typedef unsigned IPos;

#define Z_OK_              0
#define Z_STREAM_END_      1
#define Z_NEED_DICT_       2
#define Z_ERRNO_         (-1)
#define Z_STREAM_ERROR_  (-2)
#define Z_DATA_ERROR_    (-3)
#define Z_MEM_ERROR_     (-4)
#define Z_BUF_ERROR_     (-5)
#define Z_VERSION_ERROR_ (-6)

/* Calculate maximum compressed length from uncompressed length */
#define Z_COMPRESSMAXDESTLENGTH(n) (n+(((n)/1000)+1)+12)

int zcompress( u8* pDest, long* pnDestLen, const u8* pSrc, long nSrcLen );
int zuncompress( u8* pDest, long* pnDestLen, const u8* pSrc, long nSrcLen );

#define Z_CHECKLENGTH 8192

            
#define Z_NO_COMPRESSION_         0
#define Z_BEST_SPEED_             1
#define Z_BEST_COMPRESSION_       9
#define Z_DEFAULT_COMPRESSION_  (-1)

#define Z_NO_FLUSH_      0
#define Z_PARTIAL_FLUSH_ 1
#define Z_SYNC_FLUSH_    2
#define Z_FULL_FLUSH_    3
#define Z_FINISH_        4
#define Z_BLOCK_         5

#define Z_NULL_  0

#define Z_DEFLATED_   8

#define Z_FILTERED_            1
#define Z_HUFFMAN_ONLY_        2
#define Z_RLE_                 3
#define Z_FIXED_               4
#define Z_DEFAULT_STRATEGY_    0

#define Z_BINARY_   0
#define Z_TEXT_     1
#define Z_ASCII_    Z_TEXT
#define Z_UNKNOWN_  2

#if MAX_MEM_LEVEL >= 8
#  define DEF_MEM_LEVEL 8
#else
#  define DEF_MEM_LEVEL  MAX_MEM_LEVEL
#endif

#define STORED_BLOCK 0
#define STATIC_TREES 1
#define DYN_TREES    2

#define MIN_MATCH  3
#define MAX_MATCH  258

#define PRESET_DICT 0x20

#define INIT_STATE    42
#define EXTRA_STATE   69
#define NAME_STATE    73
#define COMMENT_STATE 91
#define HCRC_STATE   103
#define BUSY_STATE   113
#define FINISH_STATE 666


#define SMALLEST 1

#ifndef DEF_WBITS
#define DEF_WBITS MAX_WBITS
#endif

      
#define MAXBITS 15


typedef voidpf (*alloc_func) OF((voidpf opaque, uint items, uint size));
typedef void   (*free_func)  OF((voidpf opaque, voidpf address));


typedef struct z_stream_s {
    u8    *next_in;
    uint     avail_in;
    u32    total_in;
    u8    *next_out;
    uint     avail_out;
    u32    total_out;
    char     *msg;
    struct internal_state FAR *state;
    alloc_func zalloc;
    free_func  zfree;
    voidpf     opaque;
    int     data_type;
    u32   adler;
    u32   reserved;
} z_stream;    
typedef z_stream FAR *z_streamp;


typedef struct gz_header_s {
    int     text;
    u32   time;
    int     xflags;
    int     os;
    u8f   *extra;
    uint    extra_len;
    uint    extra_max;
    u8f   *name;
    uint    name_max;
    u8f   *comment;
    uint    comm_max;
    int     hcrc;
    int     done;
} gz_header;

typedef gz_header FAR *gz_headerp;

int deflate OF((z_streamp strm, int flush));

int deflateEnd OF((z_streamp strm));

int inflate OF((z_streamp strm, int flush));

int inflateEnd OF((z_streamp strm));

int deflateSetDictionary OF((z_streamp strm,
                                             const u8f *dictionary,
                                             uint  dictLength));

int deflateCopy OF((z_streamp dest,
                                    z_streamp source));

int deflateReset OF((z_streamp strm));

int deflateParams OF((z_streamp strm,
                                      int level,
                                      int strategy));

int deflateTune OF((z_streamp strm,
                                    int good_length,
                                    int max_lazy,
                                    int nice_length,
                                    int max_chain));

u32 deflateBound OF((z_streamp strm,
                                       u32 sourceLen));

int deflatePrime OF((z_streamp strm,
                                     int bits,
                                     int value));

int inflateSetDictionary OF((z_streamp strm,
                                             const u8f *dictionary,
                                             uint  dictLength));

int inflateSync OF((z_streamp strm));

int inflateCopy OF((z_streamp dest,
                                    z_streamp source));

int inflateReset OF((z_streamp strm));

int inflatePrime OF((z_streamp strm,
                                     int bits,
                                     int value));

int inflateGetHeader OF((z_streamp strm,
                                         gz_headerp head));


typedef unsigned (*in_func) OF((void FAR *, u8 FAR * FAR *));
typedef int (*out_func) OF((void FAR *, u8 FAR *, unsigned));

int inflateBack OF((z_streamp strm,
                                    in_func in, void FAR *in_desc,
                                    out_func out, void FAR *out_desc));


int inflateBackEnd OF((z_streamp strm));

u32 compressBound OF((u32 sourceLen));

u32 adler32 OF((u32 adler, const u8f *buf, uint len));

u32 adler32_combine OF((u32 adler1, u32 adler2,
                                          z_off_t len2));

u32 crc32   OF((u32 crc, const u8f *buf, uint len));

u32 crc32_combine OF((u32 crc1, u32 crc2, z_off_t len2));

int deflateInit_ OF((z_streamp strm, int level,
                                     const char *version, int stream_size));
int inflateInit_ OF((z_streamp strm,
                                     const char *version, int stream_size));
int deflateInit2_ OF((z_streamp strm, int  level, int  method,
                                      int windowBits, int memLevel,
                                      int strategy, const char *version,
                                      int stream_size));
int inflateInit2_ OF((z_streamp strm, int  windowBits,
                                      const char *version, int stream_size));
int inflateBackInit_ OF((z_streamp strm, int windowBits,
                                         u8 FAR *window,
                                         const char *version,
                                         int stream_size));
#define deflateInit(strm, level) \
        deflateInit_((strm), (level),       ZLIB_VERSION, sizeof(z_stream))
#define inflateInit(strm) \
        inflateInit_((strm),                ZLIB_VERSION, sizeof(z_stream))
#define deflateInit2(strm, level, method, windowBits, memLevel, strategy) \
        deflateInit2_((strm),(level),(method),(windowBits),(memLevel),\
                      (strategy),           ZLIB_VERSION, sizeof(z_stream))
#define inflateInit2(strm, windowBits) \
        inflateInit2_((strm), (windowBits), ZLIB_VERSION, sizeof(z_stream))
#define inflateBackInit(strm, windowBits, window) \
        inflateBackInit_((strm), (windowBits), (window), \
        ZLIB_VERSION, sizeof(z_stream))


//---------------------------------------------------------------------------
#endif
