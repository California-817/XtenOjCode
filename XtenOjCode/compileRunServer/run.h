#ifndef __XTENOJCODE_RUN_H__
#define __XTENOJCODE_RUN_H__
namespace XtenOjCode
{
    // 运行模块
    class Run
    {
        public:
        static int run(const char* binFile,const char* input,int cpuTimeLimit,int memoryLimit);
    };
} // namespace XtenOjCode

#endif