#include "rootServlet.h"
#include "../../util.h"
#include "../../rockClient.h"
namespace XtenOjCode
{
    static Xten::Logger::ptr g_logger = XTEN_LOG_NAME("root");
    struct XX
    {
        XX()
        {
            // test OJClient
            ojc = std::make_shared<OJClient>();
            ojc->Init();                                                // 初始化
            ojc->Connect(Xten::IPv4Address::Create("127.0.0.1", 8062)); // 链接--会涉及到协程的挂起
            // 客户端默认自动重连，用户上层只要进行初始的第一次start，后续如果由于长时间没发包给服务端导致链接被服务器断开，客户端的这个RockConnect不会被销毁[即使链接不存在]
            // 客户端首先会通过innerclose关闭此次连接的读写协程，然后在读协程内部通过定时器立即调用start函数进行重连，若start失败则又会按照一定规则累加的超时时间重连服务端
            // 直到最终重连成功，重新开启两个读写协程进行io操作，若在链接未建立过程发起request，则直接返回超时结果
            // 对于服务端RockSession，连接长时间未交互，则通过read超时使得读协程退出并调用innerclose，唤醒并间接退出写协程，由于auto_connect参数为false，由于此时只有读写
            // 协程带有RockSession的智能指针，并且不自动重连，服务端将tcp链接以及框架层的RockSession全部销毁，服务端会调用～RockStream
            ojc->Start(); // 启动读写协程进行发送接收消息
        }
        OJClient::ptr ojc;
    };
    static bool is_init = false;
    static XX *xx = nullptr;

    OJClient::ptr conn(new OJClient);
    void run()
    {
        Xten::Address::ptr addr = Xten::Address::LookupAny("127.0.0.1:8062");
        if (!conn->Connect(Xten::IPv4Address::Create("127.0.0.1", 8062)))
        {
            XTEN_LOG_INFO(g_logger) << "connect " << *addr << " false";
        }
        conn->Init();                                              
        conn->Start();

        Xten::IOManager::GetThis()->addTimer(1000, []()
                                             {
        Xten::RockRequest::ptr req(new Xten::RockRequest);
        static uint32_t s_sn = 0;
                                
        req->SetCmd(100);
        req->SetData("hello world sn=" + std::to_string(s_sn++));

        auto rsp = conn->Request(req, 300);
        if(rsp->response) {
            XTEN_LOG_INFO(g_logger) << rsp->response->ToString();
        } else {
            XTEN_LOG_INFO(g_logger) << "error result=" << rsp->resultStr;
        } }, true);
    }
    namespace oj_server
    {
        int32_t rootServlet::handle(Xten::http::HttpRequest::ptr request, Xten::http::HttpResponse::ptr response,
                                    Xten::SocketStream::ptr session)
        {
            if (!is_init)
            {
                xx = new XX();
                // run();
                is_init = true;
            }
            // 发送一个请求--入队列
            XTEN_LOG_DEBUG(g_logger) << "rootServlet";
            auto ret = xx->ojc->compileRunRpc("#include<iostream>\n"
                                              "int main() { std::cout<<\"hello world\"<<std::endl;\n"
                                              "return 0; }",
                                              "null", 10, 100);
            if (ret)
                XTEN_LOG_INFO(g_logger) << ret->reason() << " " << ret->status() << " " << ret->stderr() << " " << ret->stdout();
            std::string rspBody = OjUtil::ReadHtml::GetInstance()->ReadHtml2String("../XtenOjCode/ojServer/wwwroot/index.html");
            if (rspBody == "")
            {
                response->setBody("<html><head><title>404 Not Found"
                                  "</title></head><body><center><h1>404 Not Found</h1></center>"
                                  "<hr><center>" +
                                  m_name + "</center></body></html>");
            }
            else
            {
                response->setBody(rspBody);
            }
            response->setStatus(Xten::http::HttpStatus::OK);
            response->setHeader("Content-Type", "text/html;charset=utf-8");
            return 0;
        }
    } // namespace oj_server

}