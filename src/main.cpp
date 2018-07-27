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
#include <string.h>
#include <vector>
#include <map>

#define HPP_FILE_EXT ".hpp"
#define CPP_FILE_EXT ".cpp"
#define CC_FILE_EXT ".cc"

namespace fs = std::experimental::filesystem;


void hc_to_cc(std::string& dest_dir, std::string& src_dir);

void cc_to_hc(std::string& dest_dir, std::string& src_dir);

void hc_to_cc_file(const fs::path& h_file_path, const fs::path& s_file_path, std::string& dest_dir, std::string& src_dir);

void cc_to_hc_file(const fs::path& file_path, std::string& dest_dir, std::string& src_dir);

bool create_directory(std::string& dest_dir, std::string& src_dir, const fs::path& p);

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
    if(smode.compare("hc") == 0)
    {
        hc_to_cc(dest_dir, src_dir);
    }
    else if(smode.compare("cc") == 0)
    {
        cc_to_hc(dest_dir, src_dir);
    }
    else
    {
        std::cout << "error -> invalid mode[" << smode << "]" << std::endl;
        return 0;
    }

    return 0;
}

bool create_directory(std::string& dest_dir, std::string& src_dir, const fs::path& p)
{
    std::string dir_name = dest_dir + p.string().substr(src_dir.size(), p.string().size());
    std::cout << "creating dir: " << dir_name << std::endl;

    if(fs::is_directory(fs::path(dir_name)))
        return true;

    if(!fs::create_directory(fs::path(dir_name)))
    {
        std::cout << "error -> creating directory[" << dir_name << "]" << std::endl;
        return false;
    }
    return true;
}

void hc_to_cc(std::string& dest_dir, std::string& src_dir)
{
    std::map<std::string, std::pair<fs::path, fs::path>> file_map;

    for(auto& p : fs::recursive_directory_iterator(src_dir))
    {
        if(fs::is_directory(p))
        {
            if(!create_directory(dest_dir, src_dir, p.path()))
                return;
        }
        else if(fs::is_regular_file(p))
        {
            std::pair<fs::path, fs::path>& pair = file_map[p.path().stem().string()];

            std::cout << "file" << p << std::endl;

            if(p.path().extension().string().compare(std::string(HPP_FILE_EXT)) == 0)
                pair.first = p.path();
            if(p.path().extension().string().compare(std::string(CPP_FILE_EXT)) == 0)
                pair.second = p.path(); 
        }
    }

    for(auto& element : file_map)
    {
        std::cout << "header: " << element.second.first << " cpp: " << element.second.second << std::endl;
        hc_to_cc_file(element.second.first, element.second.second, dest_dir, src_dir);
    }
}

void cc_to_hc(std::string& dest_dir, std::string& src_dir)
{
    std::vector<fs::path> file_vec;

    for(auto& p : fs::recursive_directory_iterator(src_dir))
    {
        if(fs::is_directory(p))
        {
            if(!create_directory(dest_dir, src_dir, p.path()))
                return;
        }
        else if(fs::is_regular_file(p))
        {
            if(p.path().extension().string().compare(std::string(CC_FILE_EXT)) == 0)
                file_vec.push_back(p.path());
        }
    }


    for(auto& element : file_vec)
    {
        std::cout << "file: " << element << std::endl;
        cc_to_hc_file(element, dest_dir, src_dir);
    }
}

#define LINE_LENGTH 256

void hc_to_cc_file(const fs::path& h_file_path, const fs::path& s_file_path, std::string& dest_dir, std::string& src_dir)
{
#define hc_close_files()    if(h_input_file.is_open())  \
                                h_input_file.close();   \
                            if(s_input_file.is_open())  \
                                s_input_file.close();   \
                            if(out_file.is_open())      \
                                out_file.close();

#define convert_tag(line)   std::string(line).substr(10, strlen(line) - 10)

    std::fstream h_input_file;
    std::fstream s_input_file;
    std::fstream out_file;

    bool header = h_file_path.string().empty() ? false : true;
    bool source = s_file_path.string().empty() ? false : true;

    if(!header || !source)
    {
        std::cout << "no header or source files" << std::endl;
        return;
    }

    std::fstream *active_file = nullptr;

    std::string out_dir; 
    std::string file_stem;
    if(header)
    {
        out_dir = h_file_path.parent_path().string().substr(src_dir.size(), h_file_path.parent_path().string().size());
        file_stem = h_file_path.stem().string();
    }
    else if(source)
    {
        out_dir = s_file_path.parent_path().string().substr(src_dir.size(), s_file_path.parent_path().string().size());
        file_stem = s_file_path.stem().string();
    }

    std::string out_file_name = dest_dir + out_dir + "/" + file_stem + CC_FILE_EXT;
    std::cout << "out_file: " << out_file_name << std::endl;

    out_file.open(out_file_name, std::fstream::out | std::fstream::trunc);
    if(!out_file.good())
    {
        std::cout << "error -> opening output file" << std::endl;;
        return;
    }

    if(header && !source)
    {
        h_input_file.open(h_file_path.string(), std::fstream::in);
        if(!h_input_file.good())
        {
            std::cout << "error -> opening header input file" << std::endl;
            hc_close_files()
            return;
        }
        while(!h_input_file.eof())
        {
            char line[LINE_LENGTH];
            h_input_file.getline(line, LINE_LENGTH);
            if(std::string("//####----").compare(0,10,line,10) == 0)
            {
                out_file << "#hdr " << convert_tag(line) << std::endl;    
                continue;
            }
            out_file << line << std::endl;
        }
    }
    else if(source && !header)
    {
        s_input_file.open(s_file_path.string(), std::fstream::in);
        if(!s_input_file.good())
        {
            std::cout << "error -> opening source input file" << std::endl;
            hc_close_files();
            return;
        }
        while(!s_input_file.eof())
        {
            char line[LINE_LENGTH];
            s_input_file.getline(line, LINE_LENGTH);
            if(std::string("//####----").compare(0,10,line,10) == 0)
            {
                out_file << "#src " << convert_tag(line) << std::endl;
                continue;
            }
            out_file << line << std::endl;
        }
    }
    else if(source && header)
    {
        h_input_file.open(h_file_path.string(), std:fstream::in);
        if(!h_input_file.good())
        {
            std::cout << "error -> opening header input file" << std::endl;
            hc_close_files();
            return;
        }
        s_input_file.open(s_file_path.string(), std::fstream::in);
        if(!s_input_file.good())
        {
            std::cout << "error -> opening source input file" << std::endl;
            hc_close_files();
            return;
        }

        active_file = &h_input_file;
        bool in_header = true;
        while(!h_input_file.eof() || !s_input_file.eof())
        {
            if(h_input_file.eof())
            {
                active_file = &s_input_file;
                in_header = true;
            }
            else if(s_input_file.eof())
            {
                active_file = &h_input_file;
                in_header = false;
            }
                
            char line[LINE_LENGTH];
            active_file->getline(line, LINE_LENGTH);
            if(std::string("//####----").compare(0,10,line,10) == 0)
            {
                out_file << ""
            }
        }
    }


    hc_close_files();
}

void cc_to_hc_file(const fs::path& file_path, std::string& dest_dir, std::string& src_dir)
{
#define cc_close_files()    if(input_file.is_open())    \
                                input_file.close();     \
                            if(h_file.is_open())        \
                                h_file.close();         \
                            if(s_file.is_open())        \
                                s_file.close();
    
    std::fstream input_file;
    input_file.open(file_path.string(), std::fstream::in);
    if(!input_file.good())
    {
        std::cout << "error -> opening file" << std::endl;
        return;
    }

    std::string h_file_name = dest_dir + 
                                file_path.parent_path().string().substr(src_dir.size(), file_path.parent_path().string().size()) + "/" + 
                                file_path.stem().string() + HPP_FILE_EXT;
    std::string s_file_name = dest_dir + 
                                file_path.parent_path().string().substr(src_dir.size(), file_path.parent_path().string().size()) + "/" +
                                file_path.stem().string() + CPP_FILE_EXT;

    std::cout << "headerf: " + h_file_name << std::endl;
    std::cout << "sourcef: " + h_file_name << std::endl;

    std::fstream h_file;
    std::fstream s_file;

    std::fstream *active_file = nullptr;

    char line[LINE_LENGTH];

    while(!input_file.eof())
    {
        input_file.getline(line, LINE_LENGTH);
        if(input_file.bad())
        {
            std::cout << "error -> reading line" << std::endl;
            cc_close_files();
            return;
        }

        std::cout << "line: " << line << std::endl;

        bool marker = false;
        if(std::string("#hdr").compare(0, 4, line, 4) == 0)
        {
            std::cout << "found header line" << std::endl;
            if(!h_file.is_open())
            {
                h_file.open(h_file_name, std::fstream::out | std::fstream::trunc);
                if(!h_file.good())
                {
                    std::cout << "error -> opening output file" << std::endl;
                    cc_close_files();
                    return;
                }
            }
            active_file = &h_file;
            marker = true;
        }
        else if(std::string("#src").compare(0,4, line, 4) == 0)
        {
            std::cout << "found source line" << std::endl;
            if(!s_file.is_open())
            {
                s_file.open(s_file_name, std::fstream::out | std::fstream::trunc);
                if(!s_file.good())
                {
                    std::cout << "error -> opening output file" << std::endl;
                    cc_close_files();
                    return;
                }
            }
            active_file = &s_file;
            marker = true;
        }

        if(marker)
        {
            char *sec;
            for(sec = &line[5]; *sec != 0 && *sec == ' '; sec++);
            for(char *c = sec; *c != 0; c++)
                if(*c == ' ' || *c == '\r' || *c == '\n')
                    *c = 0;

            if(sec != 0)
            {
                *active_file << "//####----" << sec << "----####//" << std::endl;
            }
            continue;
        }

        if(active_file != nullptr)
        {
            active_file->write(line, strlen(line));
            *active_file << std::endl;
        }
    }

    cc_close_files();
}

