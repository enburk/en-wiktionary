#pragma once
#include "0.h"
namespace pass0
{
    auto path_src = "enwiktionary-pages-articles.xml.gz";
    auto path_dst = "enwiktionary-pages-articles.gz";

    Pass <nothing, str> unzip = [](auto &, auto & output)
    {
        if (std::filesystem::exists(path_dst)) return;

        gzFile file = gzopen(path_src, "rb");
        if (!file) { print("failed to gzopen ", path_src); return; }
        gzbuffer(file, 1024*1024);
        std::uintmax_t bytes_said = 0;
        std::uintmax_t bytes_read = 0;
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

    Pass <entry, entry> rezip = [](auto & input, auto & output)
    {
        if (std::filesystem::exists(path_dst))
        {
            gzFile file = gzopen(path_dst, "rb");
            if (!file) { print("failed to gzopen ", path_src); return; }
            gzbuffer(file, 1024*1024);
            std::uintmax_t bytes_said = 0;
            std::uintmax_t bytes_read = 0;
            const
            unsigned int buffer_size = 8*1024*1024;
            unsigned int bytes;
            str buffer;

            using I = str::const_iterator;
            const auto searcher = std::boyer_moore_horspool_searcher(esc.begin(), esc.end());
            auto find = [searcher](I f, I l) { return std::search(f, l, searcher); };
            str s; s.reserve(16*1024*1024);
            I i = s.begin();
            I j = s.begin();
            str title;
            str topic;
            int mode = 0;

            while (true)
            {
                I found = find(j, s.end());

                if (found == s.end())
                {
                    j = s.end() - (esc.size() - 1);

                    auto offset = i - s.begin();
            
                    if (offset > (int) s.capacity() / 2)
                    {
                        s.upto((int) offset).erase(); offset = 0;
                        j = s.begin() + offset + (j - i);
                        i = s.begin() + offset;
                    }

                    buffer.resize(buffer_size);
                    bytes = gzread(file, (unsigned char*)buffer.data(), buffer_size);
                    if (bytes == 0) break;
                    buffer.resize (bytes);
                    // bytes_read += bytes; if (bytes_read > bytes_said + 1024*1024*32) {
                    // bytes_said = bytes_read; print("rezip ", bytes_read>>20, " MB "); }

                    s += buffer;
                    j = s.begin() + offset + (j - i);
                    i = s.begin() + offset;
                    continue;
                }

                switch (mode) {
                case 0: found += esc.size() + 1; i = j = found; mode = 1; break;
                case 1: title = str  (i, found); i = j = found; mode = 2; break;
                case 2: found += esc.size() + 1; i = j = found; mode = 3; break;
                case 3: topic = str  (i, found); i = j = found; mode = 0; break;
                }

                if (mode == 1 && title.size () > 0)
                {
                    title.truncate();
                    output.push(std::move(entry{std::move(title), std::move(topic)}));
                    title = "";
                    topic = "";
                }
            }
            gzclose(file);
        }
        else
        {
            gzFile file = gzopen(path_dst, "wb");
            if (!file) { print("failed to gzopen ", path_dst); return; }
            gzbuffer(file, 64*1024*1024);

            for (auto && [title, topic] : input)
            {
                static int64_t nn = 0; if (++nn % 500'000 == 0) print("rezip ", nn, " entries ", input.cargo, " cargo ");

                entry e {std::move(title), std::move(topic)}; output.push(e);

                std::ostringstream s; s << std::move(e);

                gzputs(file, s.str().c_str());
            }
            gzputs (file, esc.c_str());
            gzclose(file);
        }
    };
}


