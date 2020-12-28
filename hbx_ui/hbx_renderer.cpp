
#include "include/cef_app.h"
#include "include/wrapper/cef_message_router.h"

#include <nlohmann/json.hpp>
#include <fmt/format.h>
#include <thread>
#include <chrono>

extern void send_pipe_message(std::string msg);

class method_handler : public CefV8Handler
{
public:
    method_handler(CefRefPtr<CefBrowser> browser, CefRefPtr<CefV8Handler> js_stringify) : browser(browser), js_stringify(js_stringify) {};
    virtual ~method_handler() {};

    virtual bool Execute(const CefString & name, CefRefPtr<CefV8Value> object, const CefV8ValueList & arguments, CefRefPtr<CefV8Value> & retval, CefString & exception) override
    {
        // this is called when one of the set functions in game is called
        // communicate the calling of this function back to the game process for processing
        // potentially intercept any calls for render-side handling? such as retrieving resource assets?

        if (name == "SendJsonMessage")
        {
            CefRefPtr<CefProcessMessage> message = CefProcessMessage::Create(name.c_str());
            CefRefPtr<CefListValue> arglist = message->GetArgumentList();

            message->GetArgumentList()->SetString(0, arguments.begin()->get()->GetStringValue());

            browser->GetMainFrame()->SendProcessMessage(PID_BROWSER, message);
        }
        else if (name == "log")
        {
            send_pipe_message("console.log: " + arguments.begin()->get()->GetStringValue().ToString());
        }

        // Function does not exist.
        return true;
    }

    CefRefPtr<CefBrowser> browser;
    CefRefPtr<CefV8Handler> js_stringify;

    IMPLEMENT_REFCOUNTING(method_handler);
};

class message_task : public CefTask
{
public:
    message_task(CefRefPtr<CefV8Context> js_context, CefRefPtr<CefV8Value> js_object, CefRefPtr<CefV8Value> js_receive_message, std::string name, std::string message)
        : js_context(js_context)
        , js_object(js_object)
        , js_receive_message(js_receive_message)
        , name(name)
        , message(message)
    {}

    virtual void Execute()
    {
//         CefRefPtr<CefV8Value> t = CefV8Value::CreateObject(NULL, NULL);
//         t->CreateString("ReceiveMessage");
//         CefV8ValueList vl;
//         js_object->ExecuteFunctionWithContext(js_context, t, vl);

        js_receive_message->ExecuteFunctionWithContext(js_context, js_receive_message, { CefV8Value::CreateString(name), CefV8Value::CreateString(message) });


//         CefRefPtr<CefV8Value> retval;
//         CefString exception;
//         std::vector<CefRefPtr<CefV8Value>> to_args;
//         to_args.push_back(CefV8Value::CreateString(message));
//         js_receive_message->Execute("ReceiveMessage", nullptr, to_args, retval, exception);
    }

    CefRefPtr<CefV8Context> js_context;
    CefRefPtr<CefV8Value> js_object;
    CefRefPtr<CefV8Value> js_receive_message;
    std::string name;
    std::string message;

    IMPLEMENT_REFCOUNTING(message_task);
};

class hbx_renderer : public CefApp, public CefRenderProcessHandler {
public:
    hbx_renderer() {}

    // CefApp methods:
    CefRefPtr<CefRenderProcessHandler> GetRenderProcessHandler() override
    {
        return this;
    }

    // CefRenderProcessHandler methods:
    void OnWebKitInitialized() override
    {
    }

    void OnContextCreated(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override
    {
        js_context = context;
        js_object = context->GetGlobal();
        js_stringify = js_object->GetValue("JSON")->GetValue("stringify")->GetFunctionHandler();
        js_handler = new method_handler(browser, js_stringify);
        this->browser = browser;

        has_context = true;
        has_receive = false;

//         CefRefPtr<CefV8Value> client = CefV8Value::CreateObject(NULL, NULL);
//         client->SetValue("log", CefV8Value::CreateFunction("log", js_handler), V8_PROPERTY_ATTRIBUTE_NONE);
//         js_object->SetValue("client", client, V8_PROPERTY_ATTRIBUTE_NONE);
        js_object->SetValue("SendJsonMessage", CefV8Value::CreateFunction("SendJsonMessage", js_handler), V8_PROPERTY_ATTRIBUTE_NONE);
        js_object->SetValue("log", CefV8Value::CreateFunction("log", js_handler), V8_PROPERTY_ATTRIBUTE_NONE);
    }

    void OnContextReleased(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefV8Context> context) override
    {
        has_context = false;
        has_receive = false;
    }

    bool OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message) override
    {
        auto name = message->GetName();
        auto args = message->GetArgumentList();
        std::string arg = args->GetString(0);
        //send_pipe_message(fmt::format("RendererApp OnProcessMessageReceived - {} - {}", name.ToString(), arg));

        if (!has_receive)
        {
            using namespace std::chrono_literals;
            js_context->Enter();
            js_receive_message = js_object->GetValue("ReceiveMessage");
            js_context->Exit();
            has_receive = true;
        }
        
        //send_pipe_message(fmt::format("PostTask - {} - {}", name.ToString(), arg));
        js_context->GetTaskRunner()->PostTask(make_scoped_refptr(new message_task(js_context, js_object, js_receive_message, name.ToString(), arg)));

        return true;
    }

    CefRefPtr<CefV8Context> js_context;
    CefRefPtr<CefV8Value> js_object;
    CefRefPtr<CefV8Handler> js_handler;
    CefRefPtr<CefBrowser> browser;
    CefRefPtr<CefV8Handler> js_stringify;
    CefRefPtr<CefV8Value> js_receive_message;

    bool has_receive = false;
    bool has_context = false;

private:
    IMPLEMENT_REFCOUNTING(hbx_renderer);
    DISALLOW_COPY_AND_ASSIGN(hbx_renderer);
};

CefRefPtr<CefApp> CreateRendererProcessApp() {
    return new hbx_renderer();
}
