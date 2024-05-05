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
    string pattern_volume, pattern_chapter;

    string extractVolume(const string& filename);
    string extractChapter(const string& filename);
    bool unzip(const string& zip_filename, const string& extract_dir);
public:
    void start(const string& directory, const string& pv, const string& pch);
};