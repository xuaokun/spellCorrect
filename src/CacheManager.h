#include <string>
#include <vector>

using std::string;
using std::vector;

class CacheManager{
public:
    static void init(size_t,const string &filename);//从磁盘文件中读取缓存信息
    static Cache &getCache(size_t idx);//获取某个缓存
    static void periodicUpdateCaches();
private:
    static vector<Cache> _cacheList;//缓存的数量与线程个数一致
    
};