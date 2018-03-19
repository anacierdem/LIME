//
// Created by Ali Naci  Erdem on 31/08/16.
//

#include "CustomHandler.hpp"
#include <Awesomium/STLHelpers.h>

using namespace Awesomium;
using namespace NSEngineComponent;

CustomHandler::CustomHandler(std::function<void(EngineCompenentType, EngineCompenentType, MessageData::MessageList)> cb) : JSMethodHandler()
{
    callback = cb;
}

void CustomHandler::OnMethodCall(WebView* caller,
                                   unsigned int remote_object_id,
                                   const WebString& method_name,
                                   const JSArray& args)
{
	//Map js objects to components
	if (args.size() > 0) {

		EngineCompenentType target = NONE;

		if (method_name == WSLit("engineManager"))
		{
			target = ENGINE_MANAGER;
		}
		else if (method_name == WSLit("gameRenderer"))
		{
			target = GAME_RENDERER;
		}
		else if (method_name == WSLit("miniMapManager"))
		{
			target = MINI_MAP_MANAGER;
		}
		else if (method_name == WSLit("paletteManager"))
		{
			target = PALETTE_MANAGER;
		}

		callback(WEB_VIEW, target, MessageData::MessageList {
			MakeMessage(args)
		});
	}
}

//Not preferred (synchronous)
//TODO: can use for synced behaviour?
JSValue CustomHandler::OnMethodCallWithReturnValue(WebView *caller,
                                                unsigned int remote_object_id,
                                                const WebString &method_name,
                                                const JSArray &args)
{
	//Map js objects to components
	if (args.size() > 0) {

		EngineCompenentType target = NONE;

		if (method_name == WSLit("engineManager"))
		{
			target = ENGINE_MANAGER;
		}
		else if (method_name == WSLit("gameRenderer"))
		{
			target = GAME_RENDERER;
		}
		else if (method_name == WSLit("miniMapManager"))
		{
			target = MINI_MAP_MANAGER;
		}
		else if (method_name == WSLit("paletteManager"))
		{
			target = PALETTE_MANAGER;
		}

		callback(WEB_VIEW, target, MessageData::MessageList {
			MakeMessage(args)
		});
	}

    return JSValue(true);
}