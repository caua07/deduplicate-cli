#include "nodupli.h"
#include <fstream>
#include <openssl/evp.h>
#include <sstream>
#include <iomanip>
#include <execution> 
#include <mutex>

using namespace std;

std::mutex map_mutex;
HashMap hash_map;

std::string 
calculate_sha256_openssl(const fs::path& file_path) 
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << file_path.string() << '\n';
        return "";
    }

    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (mdctx == NULL) {
        std::cerr << "Error: EVP_MD_CTX_new failed.\n";
        return "";
    }

    const EVP_MD* md = EVP_sha256();
    if (1 != EVP_DigestInit_ex(mdctx, md, NULL)) {
        std::cerr << "Error: EVP_DigestInit_ex failed.\n";
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    char buffer[4096];
    while (file.read(buffer, sizeof(buffer))) {
        if (1 != EVP_DigestUpdate(mdctx, buffer, file.gcount())) {
            std::cerr << "Error: EVP_DigestUpdate failed.\n";
            EVP_MD_CTX_free(mdctx);
            return "";
        }
    }

    if (file.gcount() > 0) {
        if (1 != EVP_DigestUpdate(mdctx, buffer, file.gcount())) {
            std::cerr << "Error: EVP_DigestUpdate failed.\n";
            EVP_MD_CTX_free(mdctx);
            return "";
        }
    }

    unsigned char hash_output[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    if (1 != EVP_DigestFinal_ex(mdctx, hash_output, &hash_len)) {
        std::cerr << "Error: EVP_DigestFinal_ex failed.\n";
        EVP_MD_CTX_free(mdctx);
        return "";
    }

    EVP_MD_CTX_free(mdctx);

    std::stringstream ss;
    for (unsigned int i = 0; i < hash_len; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash_output[i];
    }

    return ss.str();
}


void
process_hash(const fs::path& path)
{
  auto sha = calculate_sha256_openssl(path);

  if(!sha.empty()){

    std::lock_guard<std::mutex> guard(map_mutex);

    hash_map[sha].push_back(path);
  }
}


SizeMap
build_size_map(const fs::path& target_path)
{
  SizeMap size_map;

  for(const auto& entry : fs::recursive_directory_iterator(target_path)){
    if(entry.is_regular_file()){
      std::error_code ec;
      std::uint64_t file_size = entry.file_size(ec);

      if(ec){
        std::cerr << "Filesystem error in reading size of " << entry.path() << '\n';
        continue;
      }
      
      size_map[file_size].push_back(entry.path());
    }
  }

  return size_map;
}

void
interactive_interface(const HashMap& hash_map)
{
  PathVector files_to_delete;
  size_t groupsQ = hash_map.size();

  cout << "--- Iteractive deletion session ---\n";
  cout << "you will be presented equal-file groups one by one.\n\n";

  for(const auto& [sha, paths] : hash_map){
    if(paths.size() <= 1){
      continue;
    }

    bool group_decision_made = false;
    
    while(group_decision_made == false){

      cout << "------------------------------------------------\n";
      cout << "Found a group of " << paths.size() << " equal files\n\n";

      for(size_t i = 0; i < paths.size(); i++){
        cout << " [" << i + 1 << "] - " << paths[i] << "\n";
      }
      cout << '\n';

      cout << "Choose an action for this group:\n";
      cout << "[1-3] keep file number #, delete others    (k)eep all    (d)elete all    (q)uit\n";
      cout << "> ";
      
      std::string choice;
      cin >> choice;

      if (choice == "q" || choice == "Q"){
        cout << "quiting interactive session\n";
        goto end_review_phase;
      } else if(choice == "k" || choice == "K"){
        cout << "Keep all from this group\n";
        group_decision_made = true;
      } else if (choice == "d" || choice == "D"){
        cout << "Marked " << paths.size() << " files to delete\n";
        for(const auto& path : paths){
          files_to_delete.push_back(path);
        }
        group_decision_made = true;
      } else {
        try{
          int num_to_keep = std::stoi(choice);
          if (num_to_keep >= 1 && num_to_keep <= paths.size()){
            for(size_t i = 0; i < paths.size(); ++i){
              if(i+1 != num_to_keep){
                files_to_delete.push_back(paths[i]);
              }
            }
            std::cout << "Marked " << paths.size() - 1 << " files to delete\n";
            group_decision_made = true;
          } else {
            cout << "invalid number, you must insert a number between 1 and " << paths.size();
          }
        } catch (const std::invalid_argument& e){
          cout << "invalid input, please, enter a number or a command character\n";
        }
      }
      cout << '\n';
    }
  }
end_review_phase:;

    if (files_to_delete.empty()) {
      std::cout << "No files were selected for deletion.\n";
      return;
    }

    std::cout << "-------------------------------------------\n";
    std::cout << "REVIEW COMPLETE\n";
    std::cout << "You have selected " << files_to_delete.size() << " files to be permanently deleted.\n";
    std::cout << "This is your final confirmation. This action CANNOT be undone.\n";
    std::cout << "Are you absolutely sure you want to proceed? (yes/no): ";

    std::string final_confirmation;
    std::cin >> final_confirmation;

    if (final_confirmation == "yes") {
        size_t deleted_count = 0;
        for (const auto& path : files_to_delete) {
            std::error_code ec;
            if (fs::remove(path, ec)) {
                std::cout << "Deleted: " << path.string() << "\n";
                deleted_count++;
            } else {
                std::cerr << "Error deleting " << path.string() << ": " << ec.message() << "\n";
            }
        }
        std::cout << "\nSuccessfully deleted " << deleted_count << " files.\n";
    } else {
        std::cout << "Aborting. No files have been deleted.\n";
    }    
}
