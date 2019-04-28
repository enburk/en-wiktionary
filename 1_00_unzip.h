Pass <nothing, str> unzip = [](auto &, auto & output)
{
    print("unzip start");

    std::filesystem::path path("enwiktionary-pages-articles.xml");
    std::ifstream input (path);

    std::uintmax_t bytes_total = std::filesystem::file_size(path);
    std::uintmax_t bytes_read = 0;

    std::string buffer, s;
    const int chunk_size = 1024;//*1024;
    buffer.reserve (chunk_size);

    int64_t chunks = 0;
    int64_t nn = 0;
    size_t max = 0;

    while (std::getline (input, s))
    {
        bytes_read += s.size() + 1;

        max = std::max (max, s.size ());

        if (++nn % 1'000'000 == 0) print("unzip ", chunks, " chunks ", bytes_read>>20, " of ", bytes_total>>20, " MB ", nn, " lines ", max, " max");

        if (buffer.size() + s.size() + 1 > chunk_size)
        {
            output.push(buffer);
            buffer.clear();
            buffer.reserve(chunk_size);
            chunks++;
        }

        buffer += s;
        buffer += "\n";
    }

    output.push(buffer);

    print("unzip finish");

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
