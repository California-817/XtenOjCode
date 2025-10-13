#include "rootServlet.h"
#include "../../util.h"
namespace XtenOjCode
{
    namespace oj_server
    {
        int32_t rootServlet::handle(Xten::http::HttpRequest::ptr request, Xten::http::HttpResponse::ptr response,
                                    Xten::SocketStream::ptr session)
        {
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