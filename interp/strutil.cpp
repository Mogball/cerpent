#include <algorithm>
#include <string>
#include <vector>

#include <cassert>

using namespace std;

static auto s_findSpace = [](int c) {
    return !isspace(c);
}


static inline void leftTrim(std::string *s) {
    s->erase(s->begin(), find_if(s->begin(), s->end(), s_findSpace));
}

static inline void rightTrim(std::string *s) {
    s->erase(find_if(s->rbegin(), s->rend(), s_findSpace).base(), s->end());
}


void trim(std::string *s) {
    assert(NULL != s);

    leftTrim(s);
    rightTrim(s);
}

string join(const vector<string> &l, char c) {
    string ret;
    if (0 == l.size()) {
        return ret;
    }
    size_t total = l.size() - 1;
    for (const string &s : l) {
        total += s.size();
    }
    ret.reserve(total);
    auto it = l.begin();
    ret += *it;
    ++it;
    for (; it != l.end(); ++it) {
        ret += c;
        ret += *it;
    }
    return ret;
}
