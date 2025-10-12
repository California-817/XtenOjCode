#ifndef __OJSERVER_ROOT_SERVLET_J__
#define __OJSERVER_ROOT_SERVLET_J__
#include "Xten.h"
namespace XtenOjCode
{
    namespace oj_server
    {
        class rootServlet : public Xten::http::Servlet
        {
        public:
            rootServlet()
                : Servlet("root")
            {
            }
            /**
             * @brief 处理请求
             * @param[in] request HTTP请求
             * @param[in] response HTTP响应
             * @param[in] session HTTP连接
             * @return 是否处理成功
             */

            virtual int32_t handle(Xten::http::HttpRequest::ptr request, Xten::http::HttpResponse::ptr response,
                                   Xten::SocketStream::ptr session) override;
        };
    }
}
#endif
