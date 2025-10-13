#ifndef __XTENOJCODE_UTIL_H__
#define __XTENOJCODE_UTIL_H__
#include <string>
#include <fstream>
#include <sstream>
#include "Xten.h"
#include <unordered_map>
namespace XtenOjCode
{
    // rpc请求的操作码
    namespace OjRockCmd
    {
        enum class RockCmd : char
        {
            CompileRun = 0, // 编译运行
        };
    }
    namespace OjUtil
    {
        // 优化：内存存储 --文件修改则重新加载 否则内存直接读取----单例
        class ReadHtml : public Xten::singleton<ReadHtml>
        {
        public:
            const std::string &ReadHtml2String(const char *filePath);
            // 删除指定路径的html文件
            const void DelHtml(const char *filePath)
            {
                Xten::Mutex::Lock lock(_mtx);
                _htmlMap.erase(filePath);
            }

        private:
            // 文件读取
            std::string readHtmlFile(const char *filePath);

        private:
            std::unordered_map<std::string, std::string> _htmlMap; // 存放缓存的html文件
            Xten::Mutex _mtx;                                      // 可能多线程访问
        };
        // 文件的工具
        class FileUtil
        {
        public:
            // 获取唯一文件name
            static std::string GetUniqueFileName()
            {
                // 时间戳+原子变量
                std::stringstream ss;
                static std::atomic<uint64_t> s_count{0};
                auto timestamp = std::time(nullptr);
                auto curCount = s_count.fetch_add(1);
                ss << timestamp << "_" << curCount;
                return ss.str();
            }
            // 添加文件后缀
            static std::string AddSuffix(const char *suffix, const std::string &file)
            {
                std::string str(file);
                str += suffix;
                return str;
            }
            // 写code到文件 不存在文件则创建
            static int Write(const std::string &file, const std::string &code)
            {
                std::ofstream ofs;
                ofs.open(file, std::ios::out | std::ios::trunc);
                if (!ofs.is_open())
                {
                    return -1;
                }
                ofs << code;
                ofs.close();
                return 0;
            }
            // read输出结果到string中
            static std::string Read(const std::string &filePath)
            {
                std::ifstream ifs(filePath);
                if (!ifs.is_open())
                {
                    // log
                    XTEN_LOG_ERROR(g_logger) << "load " << filePath << " failed!!!";
                    return "";
                }
                // 打开成功
                // 1.更新文件修改时间map
                std::stringstream ss;
                ss << ifs.rdbuf();
                ifs.close();
                return ss.str();
            }
            // 是否有指定文件
            static bool HasFile(const char *filename)
            {
                struct stat buffer;
                return (stat(filename, &buffer) == 0);
            }
        };
    } // namespace OjUtil
} // namespace XtenOjCode

#endif