#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <time.h>
#include "sys/stat.h"
#if defined(__linux__) || defined(__APPLE__)
    #include <unistd.h>
    #define GetCurrentDir getcwd
#elif __WIN32__
    #include "windows.h"
    #include <direct.h>
    #define GetCurrentDir _getcwd
#endif
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
    std::string path;
    std::string key;
    Directory *root;
    Directory *current;
public:
    FileSystem(std::string _key, std::string path);
    ~FileSystem();
    std::string get_key();
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

//////////////////////////////////////////////////////////////////////////////
// Utility
//////////////////////////////////////////////////////////////////////////////
bool beginsWith(const std::string &input,const std::string &cmp)
{
    if (input.length() < cmp.length())
        return false;
    for (unsigned int i = 0; cmp[i]; i++)
        if (cmp[i] != input[i])
            return false;
    return true;
}

// Split one std::string into multiple strings
static std::vector<std::string> split(std::string v, char d)
{
    std::vector<std::string> result;
    result.push_back("");
    int item = 0;
    int length = v.length();
    
    for (int i = 0; i < length; i++)
    {
        if (v[i] != d)
            result[item] += v[i];
        else
        {
            item++;
            result.push_back("");
        }
    }
    
    return result;
}

// Convert number to std::string for storage
static std::string toString(unsigned long long int v)
{
    std::stringstream ss;
    ss << v;
    return ss.str();
}


// Convert std::string to number for calculations
static unsigned long long int toNum(std::string v)
{
    std::stringstream ss;
    unsigned long long int result;
    ss << v;
    ss >> result;
    
    // Check if valid
    if ((result == 0) && (v != "0") && (v != ""))
    {
        std::cout << "ERROR: Cannot parse std::string as number!";
        exit(1);
    }
    
    return result;
}

std::string replace(std::string& str, char find, char replace)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == find)
            str[i] = replace;
    }
    return str;
}

// Replace invalid characters with ? in strings
std::string sanitize(std::string& str)
{
    for (int i = 0; i < str.length(); i++)
    {
        if (str[i] == '<' || str[i] == '>')
            str[i] = '?';
    }
    return str;
}

//////////////////////////////////////////////////////////////////////////////
// FileSystem
//////////////////////////////////////////////////////////////////////////////
FileSystem::FileSystem(std::string _key, std::string _path)
{
    root = new Directory("", NULL);
    current = root;
    key = _key;
    path = _path;
    if (_path != "")
    {
        std::ifstream fin(_path.c_str(), std::ifstream::binary);
        if (!fin.fail())
        {
            // Read the system block from the file
            char buffer[BLOCK_SIZE+1] = {0};
            fin.seekg(256);
            fin.read(buffer, BLOCK_SIZE-256);
            fin.close();
            std::cout << buffer << std::endl << std::endl << std::endl;
            encryptDecrypt(buffer, _key, BLOCK_SIZE-256);
            std::cout << buffer << std::endl << std::endl << std::endl;
            parse(std::string(buffer));
            std::cout << root->data() << std::endl;
        }
        else
        {
            // Create the system partition
            std::ofstream fout;
            std::string tmp = _key;
            unsigned int length = tmp.length();
            srand(time(NULL));
            
            // Pad key with random values
            if (key.length() > 255)
            {
                tmp = tmp.substr(255);
            }
            for (int i = 0; i < 255; i++)
            {
                if (!tmp[i])
                    tmp += (char)(rand());
                else
                    tmp[i] ^= (0xAA+i);
            }
            
            // Store key in file
            tmp = (char)(length) + tmp;
            fout.open(_path.c_str(), std::ifstream::binary);
            fout.seekp(0);
            fout.write(tmp.c_str(), 256);
            
            // TEST (writing out fake filesystem data) ///////////////////////////////////////////////////////////////
            // Remove when done testing this function
            fout.seekp(256);
            char chars[512] = "DTest<DTest<FTestDoc.wut<0,0>>Fnew document.docx<0,0>Fnew text document.txt<0,0>DTest2<Fhello.png<0,37819>>>DWHAT?<>Ftest.txt<32819,1231289>";
            encryptDecrypt(chars, _key);
            fout.write(chars, 140);
            // END TEST //////////////////////////////////////////////////////////////////////////////////////////////
            
            // Pad system partition with null characters.
            fout.seekp(BLOCK_SIZE-1);
            fout.write("\0", 1);
            fout.close();
        }
    }
}

FileSystem::~FileSystem()
{
    delete root;
}

void FileSystem::close()
{            
    std::ofstream fout;

    // Write the fileSystem to the partition
    fout.seekp(256);
    //char *chars;
    //chars = (char*)root->data();
    //encryptDecrypt(chars, key);
    //fout.write(chars, 1791);
    fout.close();
}

Directory* FileSystem::get_root()
{
    return root;
}

Directory* FileSystem::get_current()
{
    return current;
}

std::string FileSystem::get_key()
{
    return key;
}

void FileSystem::set_current(Directory *dir)
{
    current = dir;
}

// Convert an input std::string to a directory structure
// Bad data will likely result in a program crash
void FileSystem::parse(const std::string &input)
{
    std::string data;
    std::string filename;
    std::vector<std::string> addresses;
    Directory *dir = root;
    dir->clean();
    for(int i = 0; i < input.length(); i++)
    {
        data = "";
        if (input[i] == 'D')
        {
            i++;
            for (int j = 0; j < 100 && input[i] != '<'; j++)
                data += input[i++];
            
            dir = dir->add(new Directory(data, dir));
        }
        else if (input[i] == 'F')
        {
            i++;
            for (int j = 0; j < 100 && input[i] != '<'; j++)
                data += input[i++];
            filename = data;
            data = "";
            i++;
            for (int j = 0; j < 100 && input[i] != '>'; j++)
                data += input[i++];
            addresses = split(data, DELIMITER);
            dir->add(new File(filename, toNum(addresses[0]), toNum(addresses[1])));
        }
        else if (input[i] == '>')
            dir = (dir->get_parent());
        else
            break;
    }
    current = root;
}

std::string FileSystem::filesAtPath(const std::string &path)
{
    return current->get(path)->file_list();
}

std::string FileSystem::directoriesAtPath(const std::string &path)
{
    return current->get(path)->directory_list();
}

// Encrypt/Decrypt strings using a key
void FileSystem::encryptDecrypt(char* toEncrypt, std::string key, int size) 
{
    // Get size of std::string and code key
    int keyLength = key.size();
    int r = 1;
    
    for (unsigned int i = 0; i < keyLength; i++)
        key[i] += 3*i;
    
    // Perform encryption/decryption for this std::string
    for (unsigned int i = 0, j = 0; i < size; i++)
    {
        toEncrypt[i] ^= (++key[j] + i);
        if (j >= keyLength || j <= 0)
            r = -r;
        j += r;
    }
}

// Get the currently active directory on the host
std::string FileSystem::getWorkingDirectory()
{
    char cCurrentPath[FILENAME_MAX];

    if (!GetCurrentDir(cCurrentPath, sizeof(cCurrentPath)))
    {
        return "";
    }

    cCurrentPath[sizeof(cCurrentPath) - 1] = '\0'; /* not really required */

    return cCurrentPath;
}

// Get a list of all files/folders in the current directory
std::vector<std::string> FileSystem::GetFilesInDirectory(const std::string &directory)
{
#ifdef __WIN32__
    std::vector<std::string> out;
    HANDLE dir;
    WIN32_FIND_DATA file_data;

    if ((dir = FindFirstFile((directory + "/*").c_str(), &file_data)) == INVALID_HANDLE_VALUE)
        return out; /* No files found */

    do {
        const std::string file_name = file_data.cFileName;
        const std::string full_file_name = directory + "/" + file_name;
        const bool is_directory = (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;

        //if (file_name[0] == '.')
            //continue;

        if (is_directory)
        {
            out.push_back("D" + full_file_name);
            continue;
        }

        out.push_back("F" + full_file_name);
    } while (FindNextFile(dir, &file_data));

    FindClose(dir);
    return out;
#else
    DIR *dir;
    class dirent *ent;
    class stat st;

    dir = opendir(directory);
    while ((ent = readdir(dir)) != NULL) {
        const std::string file_name = ent->d_name;
        const std::string full_file_name = directory + "/" + file_name;

        if (file_name[0] == '.')
            continue;

        if (stat(full_file_name.c_str(), &st) == -1)
            continue;

        const bool is_directory = (st.st_mode & S_IFDIR) != 0;

        if (is_directory)
            continue;

        out.push_back(full_file_name);
    }
    closedir(dir);
#endif
}

// Open a file using the system's default program
void FileSystem::open(std::string file)
{
    // Exit if there is no std::string to parse
    if (file.length() < 1 || file == "/")
        return;
#ifdef __linux__
    /*** For Linux systems ***/
    system(((std::string)("xdg-open \"/" + file + "\"")).c_str());
#elif __APPLE__
    /*** For Mac OS systems ***/
    system(((std::string)("open \"/" + file + "\"")).c_str());
#elif __WIN32__ 
    /*** For Windows systems ***/
    system(((std::string)("START \"\" \"" + file + "\"")).c_str());
#endif
}

// Create a directory at the given path
void FileSystem::mkdir(std::string dir)
{
    // Exit if there is no std::string to parse
    if (dir.length() < 1 || dir == "/")
        return;
#if defined(__linux__) || defined(__APPLE__)
    /*** For Nix-based systems ***/
    std::string tmp_dir = "";
    
    // Create each directory
    for (int i = 0; i < dir.length(); i++)
    {
        if (dir[i] == '/')
            mkdir(tmp_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        tmp_dir += dir[i];
    }
    
    // Create final directory
    mkdir(tmp_dir.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

#elif __WIN32__ 
    /*** For Windows systems ***/
    std::string tmp_dir = "";
    
    // Replace slash with backslash for Windows path
    for (int i = 0; i < dir.length(); i++)
        if (dir[i] == '/')
            dir[i] = '\\';
    
    
    // Get working directory (no paths longer than 256 chars)
    char currentdir[256];
    GetCurrentDirectory(256, currentdir);
    
    // Create each directory
    for (int i = 0; i < dir.length(); i++)
    {
        if (dir[i] == '\\')
            CreateDirectory((currentdir + (std::string)"\\" + tmp_dir).c_str(), NULL);
        tmp_dir += dir[i];
    }
    
    // Create final directory
    CreateDirectory((currentdir + (std::string)"\\" + tmp_dir).c_str(), NULL);
#endif
}

//////////////////////////////////////////////////////////////////////////////
// File
//////////////////////////////////////////////////////////////////////////////
unsigned long long int File::getFileSize(std::string _filename)
{
    struct stat stat_buf;
    unsigned long long int rc = stat(_filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

File::File(std::string _filename, unsigned long long int _address, unsigned long long int _length)
{
    filename = sanitize(_filename);
    address = _address;
    length = _length;
}
std::string File::data()
{
    return ("F" + filename + "<" + toString(address) + "," + toString(length) + ">");
}

std::string File::get_filename()
{
    return filename;
}

unsigned long long int File::get_size()
{
    return length;
}

unsigned long long int File::get_address()
{
    return address;
}

void File::importFile(std::string path)
{
    // Read file
    std::ifstream fin(path.c_str(), std::ifstream::binary);
    char buffer[BLOCK_SIZE+1] = {0};
    unsigned long long int size = getFileSize(path);
    
    // Perform read until read fails
    while (fin.read(buffer, BLOCK_SIZE))
    {
        sys->encryptDecrypt(buffer, "Some Key");
        // Write buffer to archive at offset
        sys->encryptDecrypt(buffer, "Some Key");    // DEBUG
        std::cout << buffer;                // DEBUG
    }
    
    // Clean up garbage data
    int last_size = fin.gcount();
    for (int i = last_size; i < BLOCK_SIZE; i++)
        buffer[i] = 0;
        
    // Perform action with left-over data
    sys->encryptDecrypt(buffer, "Some Key");
    // Write buffer to archive at offset
    sys->encryptDecrypt(buffer, "Some Key"); // DEBUG
    std::cout << buffer;                // DEBUG

    // Store the new size of the file
    length = size;
    
    // Clean up
    fin.close();
}

void File::exportFile(std::string path)
{
    // ?? sys.mkdir(...); ?? Probably already made by sys.
    // Read from archive at address by BLOCK_SIZE and write to output file
}

// Create any needed filesystem directories (handled by previous functions)

// Open archive for read
// Open output file for write
// Seek to address

// For the length of the file...
//   Read one block of data from the archive
//   Decrypt this data using the private key

//   If the address is exceeded, then...
//      Shorten std::string to the correct length
//      *OR* Always write the filesystem data out to a "hidden" file/folder
//        and only store the address of this file.
//   Write the std::string to the output file (including NULL characters)
//   

//////////////////////////////////////////////////////////////////////////////
// Directory
//////////////////////////////////////////////////////////////////////////////
Directory::Directory(std::string _title, Directory *_parent)
{
    title = sanitize(_title);
    parent = _parent;
}

Directory::~Directory()
{
    for (int i = 0; i < directories.size(); i++)
        delete directories[i];
    for (int i = 0; i < files.size(); i++)
        delete files[i];
}

// Add a new file reference
File* Directory::add(File *file)
{
    files.push_back(file);
    return file;
}

// Add a new sub-directory
Directory* Directory::add(Directory *dir)
{
    directories.push_back(dir);
    return dir;
}

// Get the parent of this directory
Directory* Directory::get_parent()
{
    return parent;
}

// Get the title of this directory
std::string Directory::get_title()
{
    return title;
}

std::string Directory::path()
{
    Directory *dir = this;
    std::string result;
    while (dir->get_parent())
    {
        result = result + "/" + dir->get_title();
        dir = dir->get_parent();
        if (!dir->get_parent())
            break;
    }
    if (!result[0])
        return "/";
    else
        return result;
}

// Get the list of files in this directory
std::string Directory::file_list()
{
    std::string result = "";
    for (int i = 0; i < files.size(); i++)
    {
        if (i > 0)
            result += '\n';
        result += files[i]->get_filename();
    }
    return result;
}

// Get the list of files in this directory
std::string Directory::directory_list()
{
    std::string result = "";
    for (int i = 0; i < directories.size(); i++)
    {
        if (i > 0)
            result += '\n';
        result += directories[i]->title;
    }
    return result;
}

// Find a directory by path
Directory* Directory::get(std::string path)
{
    // Return pointer to this if path is empty
    if (path.length() == 0)
        return this;
    
    // Get next item in path
    std::string toFind = path.substr(0, path.find('/'));

    // Step up a level if needed
    if (toFind == "..")
    {
        if (parent == NULL)
            return this;
        else if (path == "..")
            return parent;
        else
            return parent->get(path.substr(path.find('/')+1));
    }
    
    // Find matching directory
    for (int i = 0; i < directories.size(); i++)
        if (directories[i]->title == toFind)
        {
            if (path == directories[i]->title)
                return directories[i];
            else
                return directories[i]->get(path.substr(path.find('/')+1));
        }
        
    return this;
}

// Remove all data from this directory
void Directory::clean()
{
    directories.clear();
    files.clear();
}

// Get the raw filesystem data to write out to file
std::string Directory::data()
{
    std::string result= "";
    if (parent != NULL) result += "D" + title + "<";
    for (int i = 0; i < directories.size(); i++)
    {
        result += directories[i]->data();
    }
    for (int i = 0; i < files.size(); i++)
    {
        result += files[i]->data();
    }
    if (parent != NULL) result += ">";
    return result;
}