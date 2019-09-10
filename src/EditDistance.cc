#include <string.h>

#include "../include/EditDistance.h"
#include <string>
#include <stdio.h>

using std::cout;
using std::endl;
using std::string;
  
/// 获取一个字节高位开头为1的个数
size_t nBytesCode(const char ch)//utf-8编码下字符中的第一个字节高位所含1的个数就是字符所占的字节数
{
	if(ch & (1 << 7))//先和10000000相与
	{
		int nBytes = 1;
		for(int idx = 0; idx != 6; ++idx)
		{
			if(ch & (1 << (6 - idx)))
			{
				++nBytes;	
			}
			else
				break;
		}
		return nBytes;
	}
	return 1;
}  
  

std::size_t length(const std::string &str)//获得一个字符串中所含的字符个数（能区别中文和英文）
{
	std::size_t ilen = 0;
	for(std::size_t idx = 0; idx != str.size(); ++idx)
	{
		int nBytes = nBytesCode(str[idx]);
		idx += (nBytes - 1);
		++ilen;//每找到一个字符，则ilen加一
	}
	return ilen;
}

int triple_min(const int &a, const int &b, const int &c)
{
	return a < b ? (a < c ? a : c) : (b < c ? b : c);
}

int editDistance(const std::string & lhs, const std::string &rhs)
{//计算最小编辑距离-包括处理中英文
	size_t lhs_len = length(lhs);//计算两个字符串各自的长度
	size_t rhs_len = length(rhs);
	int editDist[lhs_len + 1][rhs_len + 1];//二维数组用来计算最小编辑距离
	for(size_t idx = 0; idx <= lhs_len; ++idx)//第一行和第一列初始化
	{
		editDist[idx][0] = idx;
	}

	for(size_t idx = 0; idx <= rhs_len; ++idx)
	{
		editDist[0][idx] = idx;
	}
	
	std::string sublhs, subrhs;
	for(std::size_t dist_i = 1, lhs_idx = 0; dist_i <= lhs_len; ++dist_i, ++lhs_idx)//依次遍历左字符串的每一个字符，dist下标从1开始
	{
		size_t nBytes = nBytesCode(lhs[lhs_idx]);//获取第一个字符长度
		sublhs = lhs.substr(lhs_idx, nBytes);//得到第一个字符
		lhs_idx += (nBytes - 1);//当前指针移至下一个字符

		for(std::size_t dist_j = 1, rhs_idx = 0; dist_j <= rhs_len; ++dist_j, ++rhs_idx)
		{
			nBytes = nBytesCode(rhs[rhs_idx]);
			subrhs = rhs.substr(rhs_idx, nBytes);
			rhs_idx += (nBytes - 1);
			if(sublhs == subrhs)//若当前所遍历的两个字符相同
			{
				editDist[dist_i][dist_j] = editDist[dist_i - 1][dist_j - 1];//则其相应位置上的值即为其左上位置的值
			}
			else//否则，其值为其上面的值、左边的值和左上的值中最小的一个
			{
				editDist[dist_i][dist_j] = triple_min(
					editDist[dist_i][dist_j - 1] + 1,
					editDist[dist_i - 1][dist_j] + 1,
					editDist[dist_i - 1][dist_j - 1] + 1);
			}
		}
	}
	return editDist[lhs_len][rhs_len];//二维数组中存的最后一位元素的值就是所求两个字符串的最小编辑距离
}


