/*
 * cc stands for the commulative c++ files .cc 
 * cpp stands for the header and source c++ files .cpp .hpp
 * 
 * hc:
 *      //####----SECTION_NAME----####//
 * cc:
 *      #hdr SECTION_NAME
 *      #src SECTION_NAME
 */

#include <iostream>
#include <fstream>
#include <experimental/filesystem>
#include <string>
#include <vector>

#define HPP_FILE_EXT ".hpp"
#define CPP_FILE_EXT ".cpp"
#define CC_FILE_EXT ".cc"

namespace fs = std::experimental::filesystem;


void hc_to_cc(std::vector<fs::path>& files, std::string& dest_dir);

void cc_to_hc(std::vector<fs::path>& files, std::string& dest_dir);

void hc_to_cc_file(const fs::path& h_file_path, const fs::path& s_file_path, std::string& dest_dir);

void cc_to_hc_file(const fs::path& file_path, std::string& dest_dir);

void print_usage()
{
    std::cout << "Usage: " << std::endl
            << "    hsc hc/cc dest_dir source_dir" << std::endl;
}


int main(int argc, char **argv)
{
    if(argc < 4)
    {
        print_usage();
        return 0;
    }

    std::string dest_dir = std::string(argv[2]);
    std::string src_dir = std::string(argv[3]);

    if(!fs::is_directory(dest_dir) || !fs::is_directory(src_dir))
    {
        std::cout << "error -> no valid dest_dir[" << dest_dir << "]/source_dir[" << src_dir << "]" << std::endl;
        return 0;
    }

    std::string smode = std::string(argv[1]);
    int mode;
    if(smode.compare("hc") == 0)
    {
        mode = 0;
    }
    else if(smode.compare("cc") == 0)
    {
        mode = 1;
    }
    else
    {
        std::cout << "error -> invalid mode[" << smode << "]" << std::endl;
        return 0;
    }

    std::vector<fs::path> files;

    for(auto& p : fs::recursive_directory_iterator(src_dir))
    {
        if(fs::is_directory(p))
        {
            std::string dir_name = dest_dir + p.path().string().substr(src_dir.size(), p.path().string().size());
            std::cout << "creating dir: " << dir_name << std::endl;

            if(fs::is_directory(fs::path(dir_name)))
                continue;

            if(!fs::create_directory(fs::path(dir_name)))
            {
                std::cout << "error -> creating directory[" << dir_name << "]" << std::endl;
                return 0;
            }
        }
        else if(fs::is_regular_file(p))
        {
            if(mode == 0)
            {
                if(p.path().extension().string().compare(std::string(HPP_FILE_EXT)) == 0 || p.path().extension().string().compare(std::string(CPP_FILE_EXT)))
                    files.push_back(p);
            }
            else if(mode == 1)
            {
                if(p.path().extension().string().compare(std::string(CC_FILE_EXT)) == 0)
                    files.push_back(p);
            }
        }
    }

    if(mode == 0)
        hc_to_cc(files, dest_dir);
    else if(mode == 1)
        cc_to_hc(files, dest_dir);

    return 0;
}

void hc_to_cc(std::vector<fs::path>& files, std::string& dest_dir)
{
    std::map<std::string, std::pair<fs::path, fs::path>> file_map;

    for(auto file : files)
    {
        if(file.extension().string().compare(std::string(HPP_FILE_EXT)))
    }
}

void cc_to_hc(std::vector<fs::path>& files, std::string& dest_dir)
{
    for(auto file : files)
        cc_to_hc_file(file, dest_dir);
}


void hc_to_cc_file(const fs::path& h_file_path, const fs::path& s_file_path, std::string& dest_dir);
{

}

void cc_to_hc_file(const fs::path& file_path, std::string& dest_dir)
{
#define close_files()   input_file.close();     \
                        if(h_file.is_open())    \
                            h_file.close();     \
                        if(c_file.is_open())    \
                            c_file.close();
    
    std::fstream input_file;
    input_file.open(file_path.string(), std::fstream::in);
    if(!input_file.good())
    {
        std::cout << "error -> opening file" << std::endl;
        return;
    }

    std::fstream h_file;
    std::fstream c_file;

    std::fstream *active = nullptr;

#define LINE_LENGTH 256
    char line[LINE_LENGTH];

    while(!input_file.eof())
    {
        input_file.getline(line, LINE_LENGTH);
        if(input_file.bad())
        {
            std::cout << "error -> reading line" << std::endl;
            close_files();
            return;
        }

        std::cout << "line: " << line << std::endl;

        if(std::string("#hdr").compare(0, 4, line, 4) == 0)
        {
            std::cout << "found header line" << std::endl;
            if(!h_file.is_open())
            {}
        }
    }

    close_files();
}

