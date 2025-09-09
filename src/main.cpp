#include "nodupli.h"

using namespace std;

int
main(int argc, char* argv[])
{
  fs::path target_path;

  if (argc > 1) {
    std::string arg1 = argv[1];
    if (arg1 == "--help" || arg1 == "-h") {
      std::cout << "Usage: " << argv[0] << " [target_path]\n";
      std::cout << "Scans a directory for duplicate files.\n";
      std::cout << "If no path is provided, the current directory will be scanned.\n";
      return 0;
    }
    target_path = argv[1];
  } else {
    target_path = fs::current_path();
  }

  if (!fs::exists(target_path)) {
    std::cerr << "Error: Path does not exist -> " << target_path.string() << std::endl;
    return 1;
  }
  
  if (!fs::is_directory(target_path)) {
    std::cerr << "Error: Path is not a directory -> " << target_path.string() << std::endl;
    return 1;
  }
  

    std::cout << "Scanning directory: " << target_path.string() << "\n\n"; 

  cout << R"(
      _          _             _ _           _       _       
     | |        | |           | (_)         | |     | |      
   __| | ___  __| |_   _ _ __ | |_  ___ __ _| |_ ___| |      
  / _` |/ _ \/ _` | | | | '_ \| | |/ __/ _` | __/ _ \ |
 | (_| |  __/ (_| | |_| | |_) | | | (_| (_| | ||  __/_|
  \__,_|\___|\__,_|\__,_| .__/|_|_|\___\__,_|\__\___(_)
                        | |                            
                        |_|                            
  )" << '\n';
  cout << '\n';


  SizeMap sizes = build_size_map(target_path);
  PathVector paths_to_hash;

  for(const auto& [size, paths] : sizes){
    if(paths.size() > 1){
      paths_to_hash.insert(paths_to_hash.end(), paths.begin(), paths.end());
    }
  }

  if(paths_to_hash.empty()){
    cout << "No potential duplicates found.\n";
    return 0;
  }

  auto hash_map = build_hash_map(paths_to_hash);

  interactive_interface(hash_map);

  cout << "\n--- End of Report ---";

  return 0;
}
