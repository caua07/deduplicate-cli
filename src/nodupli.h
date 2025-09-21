#ifndef NODUPLI_H
#define NODUPLI_H

#include <filesystem>
#include <vector>
#include <string>
#include <cstdint>
#include <unordered_map>
#include <iostream>
#include <mutex>

namespace fs = std::filesystem;
using PathVector = std::vector<fs::path>;
using SizeMap = std::unordered_map<std::uint64_t, PathVector>;
using HashMap = std::unordered_map<std::string, PathVector>;

extern HashMap hash_map;
extern std::mutex map_mutex;

std::string calculate_sha256_openssl(const fs::path& file_path);
void process_hash(const fs::path& path);
SizeMap build_size_map(const fs::path& target_path);
void interactive_interface(const HashMap& hash_map);

#endif // NODUPLI_H
