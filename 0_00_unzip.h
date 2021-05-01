#pragma once
#include "0.h"
namespace pass0
{
    // doesn't really unzip until bz2 support

    Pass <nothing, str> unzip = [](auto &, auto & output)
    {
        if (std::filesystem::exists(path_out)) return;

        FILE* file = fopen(path_in, "rb"); if (!file)
        {
            print("failed to fopen ", path_in);
            return;
        }

        std::uintmax_t bytes_read = 0;
        std::uintmax_t bytes_said = 0;

        const
        size_t buffer_size = 8*1024*1024;
        size_t bytes;

        while (true)
        {
            str buffer;
            buffer.resize(buffer_size);
            bytes = fread((unsigned char*)buffer.data(), 1, buffer_size, file);
            if (bytes == 0) break;

            buffer.resize ((int)(bytes));
            output.push(std::move(buffer));

            bytes_read += bytes; if (bytes_read > bytes_said + 1024*1024*512) {
            bytes_said = bytes_read; print("unzip ", bytes_read>>20, " MB "); }
        }
        fclose (file);
    };
}


