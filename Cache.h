#include <unordered_map>
#include <string>

using std::unordered_map;
using std::string;

class Cache{
public:
    Cache(int num=10);
    Cache(const Cache &Cache);
    void addElement(const string &key,const string &value);
    void readFromFile(const string &filename);
    void writeToFile(const string &filename);
    void update(const Cache &rhs);
private:
    unordered_map<string,string> _hashMap;//采用hashTable进行缓存
};