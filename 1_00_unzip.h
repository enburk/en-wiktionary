Pass <nothing, str> unzip = [](auto &, auto & output)
{
    // FILE* file = fopen("enwiktionary-pages-articles.xml.bz2", "rb");
    // if (!file) { print("enwiktionary-pages-articles.xml.bz2 couldn't be open"); return; }
    // 
    // const
    // unsigned int CHUNK = 1024;//*1024;
    // 
    // int ret;
    // unsigned have;
    // z_stream strm;
    // unsigned char in[CHUNK];
    // unsigned char out[CHUNK];
    // 
    // strm.zalloc = Z_NULL;
    // strm.zfree = Z_NULL;
    // strm.opaque = Z_NULL;
    // strm.avail_in = 0;
    // strm.next_in = Z_NULL;
    // //if (inflateInit(&strm) != Z_OK) { print("inflateInit failed"); return; }
    // if (inflateInit2(&strm, -MAX_WBITS) != Z_OK) { print("inflateInit failed"); return; }
    // 
    // do /* decompress until deflate stream ends or end of file */
    // {
    //     strm.avail_in = (int)fread(in, 1, CHUNK, file);
    //     if (ferror(file)) {
    //         (void)inflateEnd(&strm);
    //         print("read error");
    //         break;
    //     }
    //     if (strm.avail_in == 0)
    //         break;
    //     strm.next_in = in;
    // 
    //     do /* run inflate() on input until output buffer not full */
    //     {
    //         strm.avail_out = CHUNK;
    //         strm.next_out = out;        
    //     
    //         ret = inflate(&strm, Z_NO_FLUSH);
    //         assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
    //         switch (ret) {
    //         case Z_NEED_DICT : (void)inflateEnd(&strm); print("inflate failed: Z_NEED_DICT "); return;
    //         case Z_DATA_ERROR: (void)inflateEnd(&strm); print("inflate failed: Z_DATA_ERROR"); return;
    //         case Z_MEM_ERROR : (void)inflateEnd(&strm); print("inflate failed: Z_MEM_ERROR "); return;
    //         }        
    //     
    //         have = CHUNK - strm.avail_out;
    // 
    //         output.push(std::string((char*) out, (char*) out + have));
    //     }
    //     while (strm.avail_out == 0);
    // }
    // while (ret != Z_STREAM_END); // /* done when inflate() says it's done */
    // 
    // (void)inflateEnd(&strm);
    // 
    // fclose(file);



    auto path = "enwiktionary-pages-articles.xml.gz";
    std::uintmax_t bytes_said = 0;
    std::uintmax_t bytes_read = 0;
    gzFile file = gzopen(path, "rb");
    if (!file) { print("failed to gzopen ", path); return; }
    gzbuffer(file, 1024*1024);
    
    const
    unsigned int buffer_size = 1024;//*1024;
    unsigned char buffer[buffer_size];
    unsigned int bytes;
    while (true)
    {
        bytes = gzread(file, buffer, buffer_size); if (bytes == 0) break;

        bytes_read += bytes; if (bytes_read > bytes_said + 1024*1024*512) {
        bytes_said = bytes_read; print("unzip ", bytes_read>>20, " MB "); }
                                
        output.push(std::string((char*) buffer, (char*) buffer + bytes));
    }
    
    gzclose(file);


    // print("unzip start");
    // 
    // std::filesystem::path path("enwiktionary-pages-articles.xml");
    // std::ifstream input (path);
    // 
    // std::uintmax_t bytes_total = std::filesystem::file_size(path);
    // std::uintmax_t bytes_read = 0;
    // 
    // std::string buffer, s;
    // const int chunk_size = 1024;//*1024;
    // buffer.reserve (chunk_size);
    // 
    // int64_t chunks = 0;
    // int64_t nn = 0;
    // size_t max = 0;
    // 
    // while (std::getline (input, s))
    // {
    //     bytes_read += s.size() + 1;
    // 
    //     max = std::max (max, s.size ());
    // 
    //     if (++nn % 1'000'000 == 0) print("unzip ", chunks, " chunks ", bytes_read>>20, " of ", bytes_total>>20, " MB ", nn, " lines ", max, " max");
    // 
    //     if (buffer.size() + s.size() + 1 > chunk_size)
    //     {
    //         output.push(buffer);
    //         buffer.clear();
    //         buffer.reserve(chunk_size);
    //         chunks++;
    //     }
    // 
    //     buffer += s;
    //     buffer += "\n";
    // }
    // 
    // output.push(buffer);
    // 
    // print("unzip finish");

    /*
    // unzip doesn't supported yet

    // Boost still doesn't work with bz2
    // Simple patch is 4 years old for now:
    // https://svn.boost.org/trac10/ticket/9749#no1
    // https://svn.boost.org/trac10/attachment/ticket/9749/bzip2.patch
    /// #pragma warning(push)
    /// #pragma warning(disable: 4275)
    /// #include <boost/iostreams/device/file.hpp>
    /// #include <boost/iostreams/filtering_stream.hpp>
    /// #include <boost/iostreams/filter/bzip2.hpp>
    /// #pragma warning(pop)

    auto source = "d:/workn/en-wiktionary/enwiktionary-pages-articles.xml.bz2";

    boost::iostreams::filtering_istream  in; in.push(
    boost::iostreams::bzip2_decompressor()); in.push(
    boost::iostreams::file_source (source));
    
    std::filesystem::path unzip = source; unzip.replace_extension (".unzip");

    //while (in.getline ())

    std::ofstream out (unzip); // << result;

    int64_t bytes = 0;
    int64_t total = 0;
    char buffer[1024];
    while (in) {
        in.read(buffer, sizeof(buffer));
        auto nn = in.gcount();
        cout << nn << " read bytes" << endl;
        if (nn <= 0) break;
        out.write(buffer, nn);
        total += nn;
        bytes += nn; 
        if (bytes < 100*1024*1024) continue; else bytes = 0;
        cout << total << " bytes" << endl;
    }
    */
};
