#include <iostream>
#include <vector>
#include <map>
using namespace std;

/*
TODO:
- Add color, location, and size to vars.
- Change X to use a tree of controls.
- All locations should be calculated from the parent rect.
*/

class Control
{
private:
    int x,y,dx,dy;
    Control* _parent;
    vector<Control> _subControls;
    map<string, string> _attributes;
public:
    Control(Control* parent = NULL)
    {
        _parent = parent;
    }
    Control* getParent()
    {
        return _parent;
    }
    Control* add()
    {
        _subControls.push_back(Control(this));
        return &_subControls.at(_subControls.size()-1);
    }
    int count()
    {
        return _subControls.size();
    }
    Control* at(int i)
    {
        return &_subControls[i];
    }
    bool hasAttribute(string name)
    {
        return _attributes.count(name);
    }
    string getAttribute(string name)
    {
        if (!hasAttribute(name))
            return "";
        return _attributes[name];
    }
    void setAttribute(string name, string value)
    {
        _attributes[name] = value;
    }
} root;

string trim(string& input)
{
    int length = input.length();
    int first = 0;
    int last = length-1;
    
    for (; first < length && input[first] == ' '; first++);
    for (; length > first && input[last] == ' '; last--);
    return input.substr(first, last);
}

void writeValues(Control* control)
{
    cout << "#name=" << control->getAttribute("name") << endl;
    cout << "#text=" << control->getAttribute("text") << endl;
    cout << "#content=" << control->getAttribute("content") << endl;
    for (int i = 0; i < control->count(); i++)
    {
        writeValues(control->at(i));
    }
}

void parse(string data)
{
    Control* control = &root;
    string attribute;
    string value;
    string raw;
    int length = data.length();
    int sublength;
    for (int i = 0; i < length; i++)
    {
        raw = "";
        
        // Find first tag
        while (i < length && data[i] != '<')
            raw += data[i++];
        
        raw = trim(raw);
        if (raw.length() > 0)
        {
            // If text was found between the tags, add to previous control
            control->setAttribute("content", trim(raw));
        }
        
        raw = "";
        
        // Read entire content of this control element
        while (i < length)
        {
            i++;
            if (data[i] == '>')
                break;
            raw += data[i];
        }
        
        raw = trim(raw);
        sublength = raw.length();
        
        // Handle this control
        if (raw == "")
        {
            continue;
        }
        else if (sublength > 0 && raw[0] == '/')
        {
            // Go up one level
            control = control->getParent();
            continue;
        }
        else
        {
            control = control->add();
            // Create a new control in the current context
            for (int j = 0; j < sublength; j++)
            {
                attribute = "";
                value = "";
                
                // Ignore white spaces
                while (j < sublength && raw[j] == ' ')
                    j++;
                
                // Get the attribute name
                while (j < sublength && raw[j] != '=' && raw[j] != ' ')
                {
                    attribute += raw[j++];
                }
                
                // Get the value if it exists
                if (raw[j] == '=' & (j < sublength && raw[j+1] == '\"'))
                {
                    while (j < sublength && (raw[j] == '=' || sublength == '\"'))
                        j++;
                    j++;
                    
                    while (j < sublength && raw[j] != '\"')
                    {
                        value += raw[j++];
                    }
                    
                    // Set the attribute
                    control->setAttribute(attribute, value);
                }
                else
                {
                    // Set the attribute to a blank value
                    control->setAttribute(attribute, "");
                }
            }
        }
    }
    writeValues(&root);//TEST
}

#define main(...) int main() { parse(#__VA_ARGS__); return 0; }

main
(
    <test name="TEST" text="Hello world!" onclick="test">
        TEST TEXT!!
        <test2 name="TEST2" text="level 2">//Nothing to see here.
            //Or here.
        </>
    </>
);