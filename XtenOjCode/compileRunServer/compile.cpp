#include "compile.h"
#include "../util.h"
#include "log.h"
#include<fcntl.h>
#include<unistd.h>
namespace XtenOjCode
{
    static Xten::Logger::ptr g_logger = XTEN_LOG_NAME("root");
    int Compile::compile(const std::string &srcFile)
    {
        // 创建子进程并让g++进行编译
        // 1.fork
        pid_t ret = fork();
        if (ret == 0)
        {
            // 子进程--进程替换为g++
            // 将子进程的stdout和stderr进行重定向到文件
            int myerr=open(OjUtil::FileUtil::AddSuffix(".compileErr",srcFile).c_str(),O_CREAT | O_WRONLY,0644);
            int myout=open(OjUtil::FileUtil::AddSuffix(".compileOut",srcFile).c_str(),O_CREAT | O_WRONLY,0644);
            if(myerr<0 || myout<0)
            {
                XTEN_LOG_ERROR(g_logger)<<"create .err .out file failed!!!";
                exit(-1);
            }
            //重定向
            int ret1=dup2(myerr,2);
            int ret2=dup2(myout,1);
            //进程替换
            execlp("g++","g++","-o",OjUtil::FileUtil::AddSuffix(".exe",srcFile).c_str(),
                    OjUtil::FileUtil::AddSuffix(".cpp",srcFile).c_str(),"-std=c++11",
                        "-D","COMPILER_ONLINE",nullptr);
            //走到这一定是失败了
            XTEN_LOG_ERROR(g_logger)<<"execlp to g++ failed,errstr="<<strerror(errno);
            exit(-2);
        }
        else if (ret > 0)
        {
            // 父进程
            int status = 0;
            int ret = waitpid(ret, &status, 0);
            if (ret > 0 && WIFEXITED(status))
                // 看是否生成了.exe文件
                return OjUtil::FileUtil::HasFile(
                           OjUtil::FileUtil::AddSuffix(".exe", srcFile).c_str())
                           ? 0
                           : -1;
            else
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
