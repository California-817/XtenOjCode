#ifndef __XTENOJCODE_UTIL_H__
#define __XTENOJCODE_UTIL_H__
#include<string>
#include<fstream>
#include<sstream>
namespace XtenOjCode
{
    namespace OjUtil
    {
        //读取html文件到string
        //优化：内存存储 --文件修改则重新加载 否则内存直接读取
        class ReadHtml
        {
            public:
            static std::string ReadHtml2String(const char* filePath)
            {
                std::ifstream ifs(filePath);
                if(!ifs.is_open())
                {
                    //log
                }
                //打开成功
                std::stringstream ss;
                ss<<ifs.rdbuf();
                ifs.close();
                return ss.str();
            } 
        };
    } // namespace OjUtil
} // namespace XtenOjCode

#endif