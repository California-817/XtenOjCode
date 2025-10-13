#include "util.h"
namespace XtenOjCode
{
    namespace OjUtil
    {
        // 读取html文件到string
        static Xten::Logger::ptr g_logger = XTEN_LOG_NAME("root");
        // 存放html文件最新修改时间
        static std::unordered_map<std::string, uint64_t> &GetHtmlModifyTime()
        {
            static std::unordered_map<std::string, uint64_t> s_htmlModTimeMap;
            return s_htmlModTimeMap;
        }
        const std::string &ReadHtml::ReadHtml2String(const char *filePath)
        {
            struct stat st;
            lstat(filePath, &st);
            Xten::Mutex::Lock lock(_mtx);
            // 1.从内存获取
            auto iter = _htmlMap.find(filePath);
            if (iter == _htmlMap.end() || GetHtmlModifyTime()[filePath] != (uint64_t)st.st_mtim.tv_sec)
            {
                // 没找到 or 不是最新 ---> 加载到内存并记录修改时间
                _htmlMap[filePath] = readHtmlFile(filePath);
                return _htmlMap[filePath];
            }
            // 找到了并且是最新
            return iter->second;
        }
        std::string ReadHtml::readHtmlFile(const char *filePath)
        {
            XTEN_LOG_DEBUG(g_logger) << "read HtmlFile";
            std::ifstream ifs(filePath);
            if (!ifs.is_open())
            {
                // log
                XTEN_LOG_ERROR(g_logger) << "load html " << filePath << " failed!!!";
                return "";
            }
            // 打开成功
            // 1.更新文件修改时间map
            struct stat st;
            lstat(filePath, &st);
            GetHtmlModifyTime()[filePath] = (uint64_t)st.st_mtim.tv_sec;
            std::stringstream ss;
            ss << ifs.rdbuf();
            ifs.close();
            return ss.str();
        }
    } // namespace OjUtil

} // namespace XtenOjCode
