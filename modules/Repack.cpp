#include "Repack.h"

// Function to extract volume name from archive name
string Repack::extractVolume(const string& filename) {
    regex pattern(pattern_volume.c_str());
    smatch matches; // object for storing found matches

    if (regex_search(filename, matches, pattern) && matches.size() > 1) {
        return matches[1].str();
    }
    return "";
}

// Function to extract chapter number from archive name
string Repack::extractChapter(const string& filename) {
    regex pattern(pattern_chapter.c_str());
    smatch matches; // object for storing found matches
    if (regex_search(filename, matches, pattern) && matches.size() > 1) {
        return matches[1].str();
    }
    return "";
}

// Function to unzip a file
bool Repack::unzip(const string& zip_filename, const string& extract_dir) {
    mz_zip_archive zip_archive;
    memset(&zip_archive, 0, sizeof(zip_archive));
    if (!mz_zip_reader_init_file(&zip_archive, zip_filename.c_str(), 0)) {
        cerr << "Failed to open zip file: " << zip_filename << endl;
        return false;
    }

    // Get the number of files in the ZIP archive
    int num_files = mz_zip_reader_get_num_files(&zip_archive);
    if (num_files <= 0) {
        cerr << "No files found in zip archive: " << zip_filename << endl;
        mz_zip_reader_end(&zip_archive);
        return false;
    }

    // Iterate through each file in the ZIP archive and extract it
    for (int i = 0; i < num_files; ++i) {
        mz_zip_archive_file_stat file_stat;
        if (!mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
            cerr << "Failed to get file info from zip archive: " << zip_filename << endl;
            mz_zip_reader_end(&zip_archive);
            return false;
        }

        // Extract the file
        if (!mz_zip_reader_extract_to_file(&zip_archive, i, (extract_dir + "/" + file_stat.m_filename).c_str(), 0)) {
            cerr << "Failed to extract file from zip archive: " << file_stat.m_filename << endl;
            mz_zip_reader_end(&zip_archive);
            return false;
        }
    }

    // Cleanup
    mz_zip_reader_end(&zip_archive);
    return true;
}

// Process directory function
void Repack::start(const string& directory, const string& pv, const string& pch) {
    pattern_volume = pv;
    pattern_chapter = pch;
    // Loop through all files with .zip extension in the current directory
    for (const auto& entry : fs::directory_iterator(directory)) {
        // Check whether the element is a regular file and whether it has a ".zip" extension.
        if (fs::is_regular_file(entry) && entry.path().extension() == ".zip") {
            // Extracting the volume number and chapter number from the archive file name
            string volume = extractVolume(entry.path().filename().string());
            string chapter = extractChapter(entry.path().filename().string());

            if (!volume.empty() && !chapter.empty()) {
                string volume_dir = directory + "/Manga/Volume " + volume;
                string chapter_dir = volume_dir + "/Chapter " + chapter;

                // Create the necessary directories if they don't exist
                if (!fs::exists(volume_dir)) {
                    fs::create_directories(volume_dir);
                }
                if (!fs::exists(chapter_dir)) {
                    fs::create_directories(chapter_dir);
                }

                // Unzip the archive into the corresponding chapter directory
                if (unzip(entry.path().string(), chapter_dir)) {
                    cout << "Extraction successful for: " << entry.path().filename() << endl;
                } else {
                    cerr << "Extraction failed for: " << entry.path().filename() << endl;
                }
            } else {
                cerr << "Failed to extract volume or chapter from filename: " << entry.path().filename() << endl;
            }
        }
    }
}