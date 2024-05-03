#include <iostream>
#include <boost/filesystem.hpp>
#include <string>
#include <regex>
#include <fstream>
#include "Miniz.h"

using namespace std;
namespace fs = boost::filesystem;

class Repack {
private:
    static string extractVolume(const string& filename);
    static string extractChapter(const std::string& filename);
    static bool unzip(const std::string& zip_filename, const std::string& extract_dir);
public:
    static void start(const std::string& directory);
};