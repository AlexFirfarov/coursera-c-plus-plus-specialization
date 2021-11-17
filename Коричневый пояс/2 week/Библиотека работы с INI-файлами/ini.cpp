#include "ini.h"
using namespace std;

namespace Ini {
    Document Load(istream& input) {
        Document doc;
        while(true) {
            char c;
            input >> c;
            if (c != '[') {
                break;
            }
            string section_name;
            getline(input, section_name, ']');
            Section& new_section = doc.AddSection(move(section_name));
            input.ignore(1);
            while(true) {
                if (c = input.peek(); !isalpha(c)) {
                    break;
                }
                string key, value;
                getline(input, key, '=');
                getline(input, value, '\n');
                new_section[move(key)] = move(value);
            }
        }
        return doc;
    }

    Section& Document::AddSection(string name) {
        return sections[move(name)];
    }

    const Section& Document::GetSection(const string& name) const {
        return sections.at(name);
    }

    size_t Document::SectionCount() const {
        return sections.size();
    }
}
