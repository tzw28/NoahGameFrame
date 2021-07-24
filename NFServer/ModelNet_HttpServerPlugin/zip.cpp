
#include "zip.h"

#define CHUNK 16384

/* Compress from file source to file dest until EOF on source.
  def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
  allocated for processing, Z_STREAM_ERROR if an invalid compression
  level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
  version of the library linked do not match, or Z_ERRNO if there is
  an error reading or writing the files. */
int CompressString(const char* in_str, size_t in_len,
    std::string& out_str, int level)
{
    if (!in_str)
        return Z_DATA_ERROR;

    int ret, flush;
    unsigned have;
    z_stream strm;

    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    std::shared_ptr<z_stream> sp_strm(&strm, [](z_stream* strm) {
        (void)deflateEnd(strm);
    });
    const char* end = in_str + in_len;

    size_t pos_index = 0;
    size_t distance = 0;
    /* compress until end of file */
    do {
        distance = end - in_str;
        strm.avail_in = (distance >= CHUNK) ? CHUNK : distance;
        strm.next_in = (Bytef*)in_str;

        // next pos
        in_str += strm.avail_in;
        flush = (in_str == end) ? Z_FINISH : Z_NO_FLUSH;

        /* run deflate() on input until output buffer not full, finish
          compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);  /* no bad return value */
            if (ret == Z_STREAM_ERROR)
                break;
            have = CHUNK - strm.avail_out;
            out_str.append((const char*)out, have);
        } while (strm.avail_out == 0);
        if (strm.avail_in != 0);   /* all input will be used */
        break;

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    if (ret != Z_STREAM_END) /* stream will be complete */
        return Z_STREAM_ERROR;

    /* clean up and return */
    return Z_OK;
}

/* Decompress from file source to file dest until stream ends or EOF.
  inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
  allocated for processing, Z_DATA_ERROR if the deflate data is
  invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
  the version of the library linked do not match, or Z_ERRNO if there
  is an error reading or writing the files. */
int DecompressString(const char* in_str, size_t in_len, std::string& out_str)
{
    if (!in_str)
        return Z_DATA_ERROR;

    int ret;
    unsigned have;
    z_stream strm;
    unsigned char out[CHUNK];

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;
    ret = inflateInit(&strm);
    if (ret != Z_OK)
        return ret;

    std::shared_ptr<z_stream> sp_strm(&strm, [](z_stream* strm) {
        (void)inflateEnd(strm);
    });

    const char* end = in_str + in_len;

    size_t pos_index = 0;
    size_t distance = 0;

    int flush = 0;
    /* decompress until deflate stream ends or end of file */
    do {
        distance = end - in_str;
        strm.avail_in = (distance >= CHUNK) ? CHUNK : distance;
        strm.next_in = (Bytef*)in_str;

        // next pos
        in_str += strm.avail_in;
        flush = (in_str == end) ? Z_FINISH : Z_NO_FLUSH;

        /* run inflate() on input until output buffer not full */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm, Z_NO_FLUSH);
            if (ret == Z_STREAM_ERROR) /* state not clobbered */
                break;
            switch (ret) {
            case Z_NEED_DICT:
                ret = Z_DATA_ERROR;   /* and fall through */
            case Z_DATA_ERROR:
            case Z_MEM_ERROR:
                return ret;
            }
            have = CHUNK - strm.avail_out;
            out_str.append((const char*)out, have);
        } while (strm.avail_out == 0);

        /* done when inflate() says it's done */
    } while (flush != Z_FINISH);

    /* clean up and return */
    return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


std::string CompressString_1(const std::string& str,
    int compressionlevel)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));

    if (deflateInit(&zs, compressionlevel) != Z_OK)
        throw(std::runtime_error("deflateInit failed while compressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();           // set the z_stream's input

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // retrieve the compressed bytes blockwise
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = deflate(&zs, Z_FINISH);

        if (outstring.size() < zs.total_out) {
            // append the block to the output string
            outstring.append(outbuffer,
                zs.total_out - outstring.size());
        }
    } while (ret == Z_OK);

    deflateEnd(&zs);

    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib compression: (" << ret << ") " << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}

std::string DecompressString_1(const std::string& str)
{
    z_stream zs;                        // z_stream is zlib's control structure
    memset(&zs, 0, sizeof(zs));
    const int windowBits = 15;
    const int GZIP_ENCODING = 16;

    deflateInit2(&zs, Z_DEFAULT_COMPRESSION, Z_DEFLATED,
        windowBits | GZIP_ENCODING, 8,
        Z_DEFAULT_STRATEGY);


    if (inflateInit(&zs) != Z_OK)
        throw(std::runtime_error("inflateInit failed while decompressing."));

    zs.next_in = (Bytef*)str.data();
    zs.avail_in = str.size();

    int ret;
    char outbuffer[32768];
    std::string outstring;

    // get the decompressed bytes blockwise using repeated calls to inflate
    do {
        zs.next_out = reinterpret_cast<Bytef*>(outbuffer);
        zs.avail_out = sizeof(outbuffer);

        ret = inflate(&zs, 0);

        if (outstring.size() < zs.total_out) {
            outstring.append(outbuffer,
                zs.total_out - outstring.size());
        }

    } while (ret == Z_OK);

    inflateEnd(&zs);

    if (ret != Z_STREAM_END) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression: (" << ret << ") "
            << zs.msg;
        throw(std::runtime_error(oss.str()));
    }

    return outstring;
}
