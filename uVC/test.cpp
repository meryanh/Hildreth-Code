#include "uVC.h"

int main()
{
    //std::string data = "DTest<DTest<FTestDoc.wut<0,0>>Fnew document.docx<0,0>Fnew text document.txt<0,0>DTest2<Fhello.png<0,37819>>>DWHAT?<>Ftest.txt<32819,1231289>";
    FileSystem system = FileSystem("Test key", "data.txt");
    std::cout << system.get_root()->data() << std::endl;
    //system.parse(data);
    std::string input;
    //File f = File("TEST!", 0, 0);
    //f.importFile("C:/Users/Ryan/Desktop/vars.cpp");
    while (1)
    {
        std::cout << system.get_current()->path() << "> ";
        getline(std::cin,input);
        if (input == "q")
            break;
        else if (beginsWith(input, "open"))
        {
            if (beginsWith(input, "cd ") && input.length() > 3)
            {
                //system.set_current(system.get_current()->get(input.substr(3)));
                std::cout << "File \"" + input.substr(3) + "\" does not exist";
                continue;
            }
            else
            {
                std::cout << "USAGE: open [FILE]\n";
                continue;
            }
        }
        else if (beginsWith(input, "cd"))
        {
            if (beginsWith(input, "cd ") && input.length() > 3)
            {
                system.set_current(system.get_current()->get(input.substr(3)));
                continue;
            }
            else
            {
                std::cout << system.get_current()->path() << std::endl;    
                //std::cout << "USAGE: cd [PATH]\n";
                continue;
            }
        }
        else if (input == "ls" || input == "pwd" || input == "dir" )
        {
            std::cout << system.get_current()->path() << std::endl << std::endl;
            std::cout << system.get_current()->directory_list() << std::endl;
            std::cout << system.get_current()->file_list() << std::endl;
            continue;
        }
        else if (input[0])
        {
            std::cout << "Unknown command: " << input << std::endl;
            continue;
        }
    }
    system.close();
    return 0;
}