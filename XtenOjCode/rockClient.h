#ifndef __XTENOJCODE_ROCKCLIENT_H__
#define __XTENOJCODE_ROCKCLIENT_H__
#include <string>
#include "rock/rock_stream.h"
#include "proto/compileRun.pb.h"
#include "util.h"
// ojServer用于与compileRunServer进行rock分布式通信的客户端
namespace XtenOjCode
{
    class OJClient : public Xten::RockConnection
    {
    public:
        typedef std::shared_ptr<OJClient> ptr;
        OJClient() = default;
        virtual ~OJClient() = default;
        // 初始化函数--设置回调函数,调度器
        void Init();
        void UnInit(); // 取消回调
        // 进行一次compileRun的rpc调用
        std::shared_ptr<XtenOjCode::CompileRunRsp> compileRunRpc(
            const std::string &code, const std::string &input, const int &cpuTimeLimit, const int &memoryLimit,uint64_t timeout_ms = 0);
    private:
        template<class ReqType,class RspType>
        std::shared_ptr<RspType> callRpc(ReqType req,OjRockCmd::RockCmd cmd,uint64_t timeout_ms = 0);
    // 回调函数
        static bool onNotify(Xten::RockNotify::ptr notify, Xten::RockStream::ptr stream);
        static bool onConnect(Xten::AsyncSocketStream::ptr astream);
        static bool onDisConnect(Xten::AsyncSocketStream::ptr astream);
    };
} // namespace XtenOjCode
#endif
