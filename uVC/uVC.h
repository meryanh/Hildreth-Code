#include <vector>
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include "sys/stat.h"
#if defined(__linux__) || defined(__APPLE__)
    #include <sys/stat.h>
    #include <unistd.h>
    #define GetCurrentDir getcwd
#elif __WIN32__
    #include "windows.h"
    #include <direct.h>
    #define GetCurrentDir _getcwd
#endif
#include <stdio.h>
#define BLOCK_SIZE 2048
#define DELIMITER ','

class File;
class Directory;
class FileSystem;

static std::vector<std::string> split(std::string v, char d);
static std::string toString(unsigned long long int v);
static unsigned long long int toNum(std::string v);
std::string replace(std::string& str, char find, char replace);
std::string sanitize(std::string& str);
bool beginsWith(const std::string &input,const std::string &cmp);
    
class FileSystem
{
private:
    std::vector<unsigned long long int> empty;
    unsigned long long int end;
    Directory *root;
    Directory *current;
public:
    FileSystem();
    ~FileSystem();
    Directory *get_root();
    Directory *get_current();
    void set_current(Directory *dir);
    void open(std::string path);
    void close();
    void parse(const std::string &input);
    std::string filesAtPath(const std::string &path);
    std::string directoriesAtPath(const std::string &path);
    void mkdir(std::string dir);
    std::vector<std::string> GetFilesInDirectory(const std::string &directory);
    void encryptDecrypt(char* toEncrypt, std::string key, int size = BLOCK_SIZE);
    std::string getWorkingDirectory();
};

class File
{
private:
    std::string filename;
    FileSystem* sys;
    unsigned long long int address;
    unsigned long long int length;
    unsigned long long int getFileSize(std::string _filename);
public:
    File(std::string _filename, unsigned long long int _address, unsigned long long int _length = 0);
    std::string data();
    std::string get_filename();
    unsigned long long int get_size();
    unsigned long long int get_address();
    void importFile(std::string path);
    void exportFile(std::string path);
};

class Directory
{
private:
    std::string title;
    Directory *parent;
    FileSystem* sys;
    std::vector<Directory*> directories;
    std::vector<File*> files;
public:
    Directory(std::string _title, Directory *_parent);
    ~Directory();
    File* add(File *file);
    Directory* add(Directory *dir);
    Directory* get_parent();
    std::string get_title();
    std::string path();
    std::string file_list();
    std::string directory_list();
    Directory* get(std::string path);
    void clean();
    std::string data();
};
