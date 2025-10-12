#include"rootServlet.h"
#include"../../util.hpp"
namespace XtenOjCode
{
    namespace oj_server
    {
        int32_t rootServlet::handle(Xten::http::HttpRequest::ptr request, Xten::http::HttpResponse::ptr response,
                       Xten::SocketStream::ptr session)
        {
                std::string rspBody=OjUtil::ReadHtml::ReadHtml2String("../XtenOjCode/ojServer/wwwroot/index.html");
                response->setBody(rspBody);
                response->setStatus(Xten::http::HttpStatus::OK);
                response->setHeader("Content-Type", "text/html;charset=utf-8");
                return 0;
        }
    } // namespace oj_server

}