#ifndef __XTENOJCODE_COMPILE_H__
#define __XTENOJCODE_COMPILE_H__
#include <unistd.h>
#include <sys/wait.h>
#include <string>
namespace XtenOjCode
{
    // 编译模块
    class Compile
    {
    public:
        static int compile(const std::string &srcFile);
    };
} // namespace XtenOjCode

#endif