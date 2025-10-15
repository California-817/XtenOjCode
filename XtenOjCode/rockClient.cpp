#include "rockClient.h"
namespace XtenOjCode
{
    Xten::Logger::ptr g_logger = XTEN_LOG_NAME("root");
    // 初始化函数--设置回调函数,调度器
    void OJClient::Init()
    {
        SetConnectCb(OJClient::onConnect);
        SetDisConnectCb(OJClient::onDisConnect);
        SetNotifyHandleCb(OJClient::onNotify);
        SetIOWorker(Xten::IOManager::GetThis());
        SetProcessWorker(Xten::IOManager::GetThis());
    }
    void OJClient::UnInit() // 取消回调
    {
        SetConnectCb(nullptr);
        SetDisConnectCb(nullptr);
        SetNotifyHandleCb(nullptr);
    }
    // 进行一次compileRun的rpc调用
    std::shared_ptr<XtenOjCode::CompileRunRsp> OJClient::compileRunRpc(
        const std::string &code, const std::string &input, const int &cpuTimeLimit, const int &memoryLimit, uint64_t timeout_ms)
    {
        XtenOjCode::CompileRunReq req;
        req.set_code(code);
        req.set_input(input);
        req.set_cputimelimit(cpuTimeLimit);
        req.set_memorylimit(memoryLimit);
        return callRpc<XtenOjCode::CompileRunReq, XtenOjCode::CompileRunRsp>(req, OjRockCmd::RockCmd::CompileRun, timeout_ms);
    }
    // 这个模版函数不会在该模块之外被调用，因此可以写到cpp文件中
    template <class ReqType, class RspType>
    std::shared_ptr<RspType> OJClient::callRpc(ReqType req, OjRockCmd::RockCmd cmd, uint64_t timeout_ms)
    {
        // 1.创建rock请求
        Xten::RockRequest::ptr rockreq = std::make_shared<Xten::RockRequest>();
        rockreq->SetCmd((uint32_t)cmd);
        rockreq->SetDataAsProtoBuf(req);
        auto result = Request(rockreq, timeout_ms); //这里进行rpc请求不会阻塞当前请求协程
        do
        {
            XTEN_LOG_DEBUG(g_logger) << result->toString();
            if (!result->response)
            {
                // 没有响应
                XTEN_LOG_ERROR(g_logger) << "rock rpc call no rsp";
                break;
            }
            Xten::RockResponse::ptr rockrsp = result->response;
            std::shared_ptr<RspType> rsp = rockrsp->GetDataAsProtoBuf<RspType>();
            if (!rsp)
            {
                XTEN_LOG_ERROR(g_logger) << "rockrsp data is not vaild protobuf data";
                break;
            }
            return rsp;
        } while (false);
        return nullptr;
    }
    bool OJClient::onNotify(Xten::RockNotify::ptr notify, Xten::RockStream::ptr stream)
    {
        //服务端主动向客户端发送的通知消息---直接处理即可
        //todo---后期业务实现
        XTEN_LOG_INFO(g_logger)<<"OJClient success recv RockNotify from RockServer , message="<<notify->ToString();
        return true;
    }
    bool OJClient::onConnect(Xten::AsyncSocketStream::ptr astream)
    {
        XTEN_LOG_INFO(g_logger)<<"OJClient success connect to RockServer";
        return true;
    }
    bool OJClient::onDisConnect(Xten::AsyncSocketStream::ptr astream)
    {
        XTEN_LOG_INFO(g_logger)<<"OJClient cut down connect with RockServer";
        return true;
    }
} // namespace XtenOjCode
