#include <unordered_map>
#include <string>
#include <set>
#include <vector>
#include <utility>
using std::unordered_map;
using std::string;
using std::set;
using std::vector;
using std::pair;

class GeneIndexFile{
public:
    void geneEnIndex(const char* filePath);
    void geneCnIndex(const char* filePath);
    size_t nBytesCode(const char ch);
    void store();
private:
    unordered_map<string,set<int>> _index;//索引表
    vector<pair<string,int>> _dic;//单词和词频
};