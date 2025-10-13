#include "myModule.h"
#include "ojServer/servlets/rootServlet.h"
namespace XtenOjCode
{
    static Xten::Logger::ptr g_logger = XTEN_LOG_NAME("root");
    MyModule::MyModule()
        : Module("XtenOjCode", "libXtenOjCode.so", "1.0.0")
    {
    }
    // 命令行参数解析前后执行
    void MyModule::OnBeforeArgsParse(int argc, char **argv)
    {
        XTEN_LOG_INFO(g_logger) << "MyModule OnBeforeArgsParse";
    }
    void MyModule::OnAfterArgsParse(int argc, char **argv)
    {
        XTEN_LOG_INFO(g_logger) << "MyModule OnAfterArgsParse";
    }
    // 加载前后执行
    bool MyModule::OnLoad()
    {
        XTEN_LOG_INFO(g_logger) << "MyModule OnLoad";
        return true;
    }
    bool MyModule::OnUnload()
    {
        XTEN_LOG_INFO(g_logger) << "MyModule OnUnLoad";
        return true;
    }
    // Server准备启动前执行(进行servlet的注册)
    bool MyModule::OnServerReady()
    {
        XTEN_LOG_INFO(g_logger) << "MyModule OnServerReady";
        std::vector<Xten::TcpServer::ptr> HttpServs; //ojServer
        Xten::Application::GetInstance()->GetServersByType("http", HttpServs);
        for (auto &serv : HttpServs)
        {
            auto hs = std::dynamic_pointer_cast<Xten::http::HttpServer>(serv);
            if (hs)
            {
                // 添加Servlet处理函数
                hs->GetServletDispatch()->addServlet("/",std::make_shared<oj_server::rootServlet>());
            }
        }
        return true;
    }
    // Server启动后执行
    bool MyModule::OnServerUp()
    {
        XTEN_LOG_INFO(g_logger) << "MyModule OnServerUp";
        return true;
    }
};
// 创建Module的函数-防止函数名修饰
extern "C"
{
    Xten::Module *CreateModule()
    {
        Xten::Module *mod = new XtenOjCode::MyModule();
        XTEN_LOG_INFO(XtenOjCode::g_logger) << "CreateModule: " << mod;
        return mod;
    }
    void DestoryModule(Xten::Module *ptr)
    {
        delete ptr;
        XTEN_LOG_INFO(XtenOjCode::g_logger) << "DestoryModule: " << ptr;
    }
}