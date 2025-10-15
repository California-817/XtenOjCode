#include "rootServlet.h"
#include "../../util.h"
#include"../../rockClient.h"
namespace XtenOjCode
{
    namespace oj_server
    {
        int32_t rootServlet::handle(Xten::http::HttpRequest::ptr request, Xten::http::HttpResponse::ptr response,
                                    Xten::SocketStream::ptr session)
        {
            //test OJClient
            OJClient::ptr ojc=std::make_shared<OJClient>();
            ojc->Init(); //初始化
            ojc->Connect(Xten::IPv4Address::Create("127.0.0.1",8062));//链接
            ojc->Start(); //启动读写协程进行发送接收消息
            //发送一个请求--入队列
            ojc->compileRunRpc("#include<iostream>\n"
                   "int main() { std::cout<<\"hello world\"<<std::endl;\n"
                   "return 0; }", "null", 10, 100,5000);
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