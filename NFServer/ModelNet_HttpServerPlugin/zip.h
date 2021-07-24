#include <string>
#include <memory>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include "zlib.h"

int CompressString(const char* in_str, size_t in_len,
    std::string& out_str, int level);

int DecompressString(const char* in_str, size_t in_len, std::string& out_str);

std::string CompressString_1(const std::string& str, int compressionlevel = Z_BEST_COMPRESSION);
std::string DecompressString_1(const std::string& str);
