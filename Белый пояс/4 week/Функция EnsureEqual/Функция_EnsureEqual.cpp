#include <string>
#include <stdexcept>

using namespace std;


void EnsureEqual(const string& s, const string& t) {
    if (s != t) {
        throw runtime_error(s + " != " + t);
    }
}

int main() {}