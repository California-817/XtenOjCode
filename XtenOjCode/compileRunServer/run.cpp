#include "run.h"
#include "log.h"
#include "../util.h"
#include <fcntl.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
namespace XtenOjCode
{
    static Xten::Logger::ptr g_logger = XTEN_LOG_NAME("root");
    static void setLimit(int cpu_limit, int memory_limit)
    {
        // 限制CPU时间（秒）
        struct rlimit cpu_rlim;
        cpu_rlim.rlim_cur = cpu_limit; // 软限制
        cpu_rlim.rlim_max = cpu_limit; // 硬限制
        setrlimit(RLIMIT_CPU, &cpu_rlim);

        // 限制内存使用（字节）
        struct rlimit mem_rlim;
        mem_rlim.rlim_cur = memory_limit * 1024 * 1024; // 转换为字节
        mem_rlim.rlim_max = memory_limit * 1024 * 1024;
        setrlimit(RLIMIT_AS, &mem_rlim);
    }
    int Run::run(const char *binFile, const char *input, int cpuTimeLimit, int memoryLimit)
    {
        // run有三种情况
        // 1.运行中途崩溃 2.运行完结果错误 3.运行完结果正确
        //  创建子进程进行进程替换
        pid_t ret = fork();
        if (ret == 0)
        {
            // 子进程--进程替换为编译后的程序
            setLimit(cpuTimeLimit,memoryLimit);
            // 将子进程的stdout和stderr进行重定向到文件
            int myerr = open(OjUtil::FileUtil::AddSuffix(".runErr", binFile).c_str(), O_CREAT | O_WRONLY, 0644);
            int myout = open(OjUtil::FileUtil::AddSuffix(".runOut", binFile).c_str(), O_CREAT | O_WRONLY, 0644);
            if (myerr < 0 || myout < 0)
            {
                XTEN_LOG_ERROR(g_logger) << "create .err .out file failed!!!";
                exit(-1);
            }
            // 重定向
            int ret1 = dup2(myerr, 2);
            int ret2 = dup2(myout, 1);
            // 进程替换
            execlp(OjUtil::FileUtil::AddSuffix(".exe", binFile).c_str(),
                   OjUtil::FileUtil::AddSuffix(".exe", binFile).c_str(), nullptr);
            // 走到这一定是失败了
            XTEN_LOG_ERROR(g_logger) << "execlp to " << OjUtil::FileUtil::AddSuffix(".exe", binFile) << " failed,errstr=" << strerror(errno);
            exit(-2);
        }
        else if (ret > 0)
        {
            // 父进程
            int status = 0;
            int ret = waitpid(ret, &status, 0);
            if (ret > 0)
            {
                if(WIFEXITED(status))
                {
                    //程序正常退出
                    return 0;
                }
                else
                {
                    //程序运行时崩溃被信号中断--返回中断信号编号
                    return WTERMSIG(status);
                }
            }
            return -2;
        }
        else
        {
            XTEN_LOG_ERROR(g_logger) << "fork child process to compile failed!!!";
            // fork失败
            return -3;
        }
    }
} // namespace XtenOjCode
