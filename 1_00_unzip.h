Pass <nothing, str> unzip = [](auto &, auto & output)
{
    auto path = "enwiktionary-pages-articles.xml.gz";
    std::uintmax_t bytes_said = 0;
    std::uintmax_t bytes_read = 0;
    gzFile file = gzopen(path, "rb");
    if (!file) { print("failed to gzopen ", path); return; }
    gzbuffer(file, 1024*1024);
    
    const
    unsigned int buffer_size = 8*1024*1024;
    unsigned int bytes;
    while (true)
    {
        str buffer; buffer.resize(buffer_size);
        bytes = gzread(file, (unsigned char*)buffer.data(), buffer_size);
        if (bytes == 0) break;
        buffer.resize (bytes);
        output.push(std::move(buffer));

        bytes_read += bytes; if (bytes_read > bytes_said + 1024*1024*512) {
        bytes_said = bytes_read; print("unzip ", bytes_read>>20, " MB "); }
    }
    
    gzclose(file);
};
