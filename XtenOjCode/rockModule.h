#ifndef __XTENOJCODE_ROCK_H__
#define __XTENOJCODE_ROCK_H__
#include "Xten.h"
namespace XtenOjCode
{
    class CompileRunModule : public Xten::RockModule
    {
    public:
        enum ErrorCode
        {
            success=0,
            makeDirFailed=-1,//创建temp目录失败
            compileFailed=-2, //编译失败
            runFailed=-3, //运行失败
            srcCodeNull=-4, //没有源代码
            writeSrcCodeFailed=-5, //写code到file失败
        };
        typedef std::shared_ptr<CompileRunModule> ptr;
        CompileRunModule();
        // 命令行参数解析前后执行
        virtual void OnBeforeArgsParse(int argc, char **argv) override;
        virtual void OnAfterArgsParse(int argc, char **argv) override;
        // 加载前后执行
        virtual bool OnLoad() override;
        virtual bool OnUnload() override;
        // Server准备启动前执行(进行servlet的注册)
        virtual bool OnServerReady() override;
        // Server启动后执行
        virtual bool OnServerUp() override;
         // 链接建立的函数
        virtual bool OnConnect(Xten::Stream::ptr stream) override;
        // 连接断开函数
        virtual bool OnDisConnect(Xten::Stream::ptr stream) override;
        // 处理Rock请求函数
        virtual bool OnHandleRockRequest(Xten::RockRequest::ptr req, Xten::RockResponse::ptr rsp, Xten::RockStream::ptr stream) override;
        // 处理Rock通知函数(对服务端基本无意义)
        virtual bool OnHandleRockNotify(Xten::RockNotify::ptr notify, Xten::RockStream::ptr stream) override;
    private:
        //处理编译运行的cmd
        bool handleCompileRun(Xten::RockRequest::ptr req, Xten::RockResponse::ptr rsp, Xten::RockStream::ptr stream);
        
    };
} // namespace XtenOjCode

#endif