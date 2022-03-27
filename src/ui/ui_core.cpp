
#include "ui_core.hpp"
#include "../Game.h"

namespace ui
{

ui_game::ui_game(CGame * _game, sf::WindowHandle _handle)
    : game(_game)
    , handle(_handle)
{
    core = new ui_core(_handle, _game);
}

ui_panel * ui_game::create_panel(std::string name, std::string url, int x, int y, int width, int height)
{
    //     if (this->panels.find(name) != this->panels.end()) {
    //         return this->panels.at(name);
    //     }
    if (panel)
        return panel;

    ui_view * view = core->create_view(width, height, url, true, this);
    //ui_panel * container = new ui_panel(view, url, x, y, width, height);

    //this->panels[name] = container;

    //return container;

    panel = new ui_panel(view, url, x, y, width, height);
    core->panel = panel;
    return panel;
}

void ui_game::run_cef_thread()
{
    CefSettings settings;
    settings.multi_threaded_message_loop = false;
    CefString(&settings.log_file).FromASCII("log.txt");
    // settings.single_process = sSingleProcess;
    settings.windowless_rendering_enabled = true;
    settings.external_message_pump = true;
    settings.remote_debugging_port = 9222;
    settings.background_color = cef_color_t(0x00ffffff);

    CefString(&settings.browser_subprocess_path).FromASCII(fmt::format("{}\\bin\\{}", std::filesystem::current_path().string(), "hbx_ui.exe").c_str());

    CefMainArgs sArgs(CefMainArgs(GetModuleHandle(NULL)));

    CefInitialize(sArgs, settings, CefRefPtr<ui::ui_core>(core), NULL);

    while (is_running)
    {
        CefDoMessageLoopWork();

        std::this_thread::sleep_for(std::chrono::milliseconds(game->ui_delay));

        if (core->create_browser)
        {
            core->add_browser_to_interface(core->view);
            core->create_browser = false;
        }

        if (core->view)
            core->view->send_emitters();

        if (begin_shutdown)
        {
            core->view->browser->GetHost()->CloseBrowser(true);
            is_running = false;
        }
    }

    CefShutdown();
}

void ui_core::OnLoadStart(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, TransitionType transition_type)
{

}

void ui_core::OnLoadEnd(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, int httpStatusCode)
{

}

void ui_core::OnLoadError(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, ErrorCode errorCode, const CefString & errorText, const CefString & failedUrl)
{

}

void ui_core::GetViewRect(CefRefPtr<CefBrowser> browser, CefRect & rect)
{
    if (!view)
        rect = CefRect(0, 0, 800, 600);
    else
        rect = CefRect(0, 0, view->get_width(), view->get_height());
}

void ui_core::OnPaint(CefRefPtr<CefBrowser> browser, PaintElementType type, const RectList & dirtyRects, const void * buffer, int width, int height)
{
    if (!view)
        return;

    if (!buffer)
        return;

    if (type == PET_VIEW)
    {
        int old_width = view->texture_width;
        int old_height = view->texture_height;

        //Retrieve current size of browser view.
        view->texture_width = width;
        view->texture_height = height;

        //Check if we need to resize the texture before drawing to it.
        if (old_width != view->texture_width || old_height != view->texture_height)
        {
            // Does nothing
        }
        else
        {
            //We want to work on the buffer byte by byte so get a pointer with a new type.
            char * bitmap = (char *)(buffer);

            sf::Lock lock(view->sfmutex);
            //Update the dirty rectangles.
            CefRenderHandler::RectList::const_iterator i = dirtyRects.begin();
            for (; i != dirtyRects.end(); ++i)
            {
                const CefRect & rect = *i;
                //Create a rect sized buffer for the new rectangle data.
                char * rectBuffer = new char[rect.width * (rect.height + 1) * 4];

                for (int jj = 0; jj < rect.height; jj++)
                {
                    //Copy the new rectangle data out of the full size buffer into our rect sized one.  
                    memcpy(
                        rectBuffer + jj * rect.width * 4,
                        bitmap + ((rect.x + ((rect.y + jj) * view->texture_width)) * 4),
                        rect.width * 4
                    );
                }

                //Convert BGRA to RGBA
                unsigned int * pTmpBuf = (unsigned int *)rectBuffer;
                const int numPixels = rect.width * rect.height;
                for (int i = 0; i < numPixels; i++)
                {
                    pTmpBuf[i] = (pTmpBuf[i] & 0xFF00FF00) | ((pTmpBuf[i] & 0x00FF0000) >> 16) | ((pTmpBuf[i] & 0x000000FF) << 16);
                }

                if (!rectBuffer)
                    continue;
                //Update the texture with the new data.  
                //This can be interrupted if the main thread calls a draw on a sprite which uses this texture
                // as the texture is bound by openGL calls.  
                //To rectify this we have the redundancy updating system.  
                view->texture->update((sf::Uint8 *)rectBuffer, rect.width, rect.height, rect.x, rect.y);

                delete[] rectBuffer;
                //Here we need to add the data required for the update to the queue for redundancy updates.  
//                 view->update_rects.push(update_rect());
//                 view->update_rects.back().buffer = rectBuffer;
//                 view->update_rects.back().rect = rect;
            }
        }
    }
}

bool ui_core::GetScreenPoint(CefRefPtr<CefBrowser> browser, int viewX, int viewY, int & screenX, int & screenY)
{
    screenX = viewX - panel->x;
    screenY = viewY - panel->y;
    return true;
}

bool ui_core::OnKeyEvent(CefRefPtr<CefBrowser> browser, const CefKeyEvent & e, CefEventHandle os_event)
{
    if (e.is_system_key && e.modifiers == 4)
    {
        if (e.character == 'V')
            browser->GetFocusedFrame()->Paste();
        else if (e.character == 'X')
            browser->GetFocusedFrame()->Cut();
        else if (e.character == 'C')
            browser->GetFocusedFrame()->Copy();
        else if (e.character == 'Z')
            browser->GetFocusedFrame()->Undo();
        else if (e.character == 'Y')
            browser->GetFocusedFrame()->Redo();
        else if (e.character == 'A')
            browser->GetFocusedFrame()->SelectAll();
    }

    return true;
}

void ui_core::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{

}

void ui_core::OnBeforeClose(CefRefPtr<CefBrowser> browser)
{
    if (view && view->browser)
    {
        view->clear_browser();
        view->Release();
    }
    cv.notify_all();
}

void ui_core::OnBeforeChildProcessLaunch(CefRefPtr<CefCommandLine> command_line)
{

}

void ui_core::OnContextInitialized()
{
    /*
    const CefString & startup_url = GetStartupURL();
    shared::CreateBrowser(new Client(startup_url), startup_url,
        CefBrowserSettings());



    if (use_views) {
        // Create the BrowserView.
        CefRefPtr<CefBrowserView> browser_view = CefBrowserView::CreateBrowserView(
            client, startup_url, settings, NULL, NULL, NULL);

        // Create the Window. It will show itself after creation.
        CefWindow::CreateTopLevelWindow(new WindowDelegate(browser_view));
    }
    else {
        // Information used when creating the native window.
        CefWindowInfo window_info;

        // Create the browser window.
        CefBrowserHost::CreateBrowser(window_info, client, startup_url, settings,
            NULL, NULL);
    }
    */
}

bool ui_core::OnProcessMessageReceived(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefProcessId source_process, CefRefPtr<CefProcessMessage> message)
{
    CefString name = message->GetName();

    // js callback from renderer - always has a single string as an argument. the JSON.stringify-ied object parameter
    if (name.ToString() == "SendJsonMessage")
    {
        auto arg = message->GetArgumentList()->GetString(0);

        {
            std::unique_lock l(game->ui_event_m);
            try
            {
                game->ui_events.emplace(name, std::move(json::parse(arg.ToString())));
            }
            catch (std::exception & ex)
            {
                std::cout << "Error parsing json : " << arg.ToString() << '\n';
            }
        }
    }
    return false;
}

void ui_core::OnBeforeContextMenu(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, CefRefPtr<CefMenuModel> model)
{
    // ignore all context menus
    model->Clear();
}

bool ui_core::OnContextMenuCommand(CefRefPtr<CefBrowser> browser, CefRefPtr<CefFrame> frame, CefRefPtr<CefContextMenuParams> params, int command_id, EventFlags event_flags)
{
    // throw error if one is called anyway
    MessageBox(browser->GetHost()->GetWindowHandle(), L"The requested action is not supported", L"Unsupported Action", MB_OK | MB_ICONINFORMATION);
    return false;
}

void ui_core::add_browser_to_interface(ui_view * view)
{
    CefWindowInfo info;

    info.SetAsWindowless(NULL);// , view->is_transparent());
    CefBrowserSettings browser_settings;
    browser_settings.javascript_access_clipboard = STATE_ENABLED;
    browser_settings.javascript = STATE_ENABLED;
    browser_settings.universal_access_from_file_urls = STATE_ENABLED;
    browser_settings.file_access_from_file_urls = STATE_ENABLED;
    browser_settings.local_storage = STATE_ENABLED;
    browser_settings.webgl = STATE_ENABLED;


    view->browser = CefBrowserHost::CreateBrowserSync(info, this, view->get_current_url(), browser_settings, NULL, NULL);

    //sViews[browser->GetIdentifier()] = view;
}

ui_view * ui_core::create_view(int width, int height, const std::string & url, bool transparent, ui_game * ui)
{
    view = new ui_view(width, height, url, transparent, ui);

    view->handle = handle;
    //swapped
    //HTMLUICore::GetInstance()->AddBrowserToInterface(view);
    //add_browser_to_interface(view);

    create_browser = true;

    //view_queue.push(view);

    return view;
}

}
