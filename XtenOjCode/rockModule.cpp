#include "rockModule.h"
#include "util.h"
#include "proto/compileRun.pb.h"
#include "compileRunServer/compile.h"
#include "compileRunServer/run.h"
namespace XtenOjCode
{
    static Xten::Logger::ptr g_logger = XTEN_LOG_NAME("root");
    CompileRunModule::CompileRunModule()
        : RockModule("CompileRun", "libCompileRun.so", "1.0.0")
    {
    }
    // 错误码转string
    static const char *ErrorCodeToString(CompileRunModule::ErrorCode code)
    {
        switch (code)
        {
#define XX(CODE)                            \
    case CompileRunModule::ErrorCode::CODE: \
        return "ErrorCode=" #CODE;          \
        break;
            XX(success);
            XX(makeDirFailed);
            XX(compileFailed);
            XX(runFailed);
            XX(srcCodeNull);
            XX(writeSrcCodeFailed);
#undef XX
        default:
            break;
        }
        return "ErrorCode=UnKnown";
    }
    // 删除一次编译运行产生的临时文件
    static void UnLinkTmpFile(const char *filePath)
    {
#define XX(suffix)                                                                        \
    if (OjUtil::FileUtil::HasFile(OjUtil::FileUtil::AddSuffix(suffix, filePath).c_str())) \
    {                                                                                     \
        unlink(OjUtil::FileUtil::AddSuffix(suffix, filePath).c_str());                    \
    }
        XX(".cpp");
        XX(".exe");
        XX(".compileOut");
        XX(".compileErr");
        XX(".runOut");
        XX(".runErr");
#undef XX
    }
    // 处理Rock请求函数
    bool CompileRunModule::OnHandleRockRequest(Xten::RockRequest::ptr req, Xten::RockResponse::ptr rsp, Xten::RockStream::ptr stream)
    {
        // 记录请求次数 todo
        OjRockCmd::RockCmd cmd = (OjRockCmd::RockCmd)req->GetCmd();
        switch (cmd)
        {
        case OjRockCmd::RockCmd::CompileRun:
            /* code */
            return handleCompileRun(req, rsp, stream);
            break;
        // other cmd ...
        default:
            XTEN_LOG_ERROR(g_logger) << "rock req cmd is invaild!!!";
            break;
        }
        return false;
    }
    // 编译运行处理函数
    bool CompileRunModule::handleCompileRun(Xten::RockRequest::ptr req, Xten::RockResponse::ptr rsp, Xten::RockStream::ptr stream)
    {
        ErrorCode error = ErrorCode::success;
        std::string filename = "./temp/";
        do
        {
            // 1.先拿到req中的data
            auto reqData = req->GetDataAsProtoBuf<XtenOjCode::CompileRunReq>(); // 集成protobuf序列化机制
            // 2.将code写入临时src文件中
            if (reqData->code() == "")
            {
                error = ErrorCode::srcCodeNull;
                break;
            }
            if (!Xten::FileUtil::MakeDir(filename))
            {
                XTEN_LOG_ERROR(g_logger) << "mkdir " << filename << " failed!!!";
                error = ErrorCode::makeDirFailed;
                break;
            }
            filename += OjUtil::FileUtil::GetUniqueFileName(); // ./temp/489243_11
            // 创建cpp文件并写入code
            if (-1 == OjUtil::FileUtil::Write(OjUtil::FileUtil::AddSuffix(".cpp", filename), reqData->code()))
            {
                // 写入code失败
                XTEN_LOG_ERROR(g_logger) << "write srcCode to cppFile failed!!!";
                error = ErrorCode::writeSrcCodeFailed;
                break;
            }
            // 3.编译
            int cret = Compile::compile(filename); // ./temp/489243
            if (cret)
            {
                // 编译出错
                error = ErrorCode::compileFailed;
                break;
            }
            // 4.运行
            int rret = Run::run(filename.c_str(), reqData->input().c_str(),
                                reqData->cputimelimit(), reqData->memorylimit()); // ./temp/489243
            if (rret > 0)
            {
                // 程序运行崩溃
                error = ErrorCode::runFailed;
                break;
            }
            else if (rret == 0)
            {
                // success
                break;
            }
            else
            {
                // fork失败
                XTEN_LOG_ERROR(g_logger) << "fork child process failed!!!";
                error = ErrorCode::runFailed;
                break;
            }
        } while (false);
        // 设置响应信息
        XtenOjCode::CompileRunRsp rspData;
        rspData.set_status(error);
        rspData.set_reason(ErrorCodeToString(error)); // code2reason
        // from file
        std::string errFile;
        std::string outFile;
        switch (error)
        {
        case ErrorCode::compileFailed:
            errFile = OjUtil::FileUtil::AddSuffix(".compileErr", filename);
            outFile = OjUtil::FileUtil::AddSuffix(".compileOut", filename);
            break;
        case ErrorCode::success:
        case ErrorCode::runFailed:
            errFile = OjUtil::FileUtil::AddSuffix(".runErr", filename);
            outFile = OjUtil::FileUtil::AddSuffix(".runOut", filename);
            break;
        default:
            break;
        }
        rspData.set_stderr(OjUtil::FileUtil::Read(errFile));
        rspData.set_stdout(OjUtil::FileUtil::Read(outFile));
        UnLinkTmpFile(filename.c_str()); // 删除临时文件
        return rsp->SetDataAsProtoBuf(rspData) &&
               (error == ErrorCode::compileFailed || error == ErrorCode::runFailed || error == ErrorCode::success);
    }

    // 命令行参数解析前后执行
    void CompileRunModule::OnAfterArgsParse(int argc, char **argv)
    {
        // nothing
    }
    void CompileRunModule::OnBeforeArgsParse(int argc, char **argv)
    {
        // nothing
    }
    // 加载前后执行
    bool CompileRunModule::OnLoad()
    {
        return true;
    }
    bool CompileRunModule::OnUnload()
    {
        return true;
    }
    // Server准备启动前执行(进行servlet的注册)
    bool CompileRunModule::OnServerReady()
    {
        return true;
    }
    // Server启动后执行
    bool CompileRunModule::OnServerUp()
    {
        return true;
    }
    // 链接建立的函数
    bool CompileRunModule::OnConnect(Xten::Stream::ptr stream)
    {
        XTEN_LOG_INFO(g_logger) << "Rock OnConnect!!!";
        return true;
    }
    // 连接断开函数
    bool CompileRunModule::OnDisConnect(Xten::Stream::ptr stream)
    {
        XTEN_LOG_INFO(g_logger) << "Rock OnDisConnect!!!";
        return true;
    }

    // 处理Rock通知函数(对服务端基本无意义)
    bool CompileRunModule::OnHandleRockNotify(Xten::RockNotify::ptr notify, Xten::RockStream::ptr stream)
    {
        return true;
    }
} // namespace XtenOjCode

// 创建Module的函数-防止函数名修饰
extern "C"
{
    Xten::Module *CreateModule()
    {
        Xten::Module *mod = new XtenOjCode::CompileRunModule();
        XTEN_LOG_INFO(XtenOjCode::g_logger) << "CreateModule: " << mod;
        return mod;
    }
    void DestoryModule(Xten::Module *ptr)
    {
        delete ptr;
        XTEN_LOG_INFO(XtenOjCode::g_logger) << "DestoryModule: " << ptr;
    }
}
