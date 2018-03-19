//
// Created by Ali Naci  Erdem on 31/08/16.
//

#pragma once

#include <Awesomium/WebView.h>
#include "EngineComponent.hpp"

// Handles js calls to C++
class CustomHandler : public Awesomium::JSMethodHandler {
public:
	CustomHandler(std::function<void(NSEngineComponent::EngineCompenentType source,
									 NSEngineComponent::EngineCompenentType,
									 NSEngineComponent::MessageData::MessageList)> cb);

    void OnMethodCall(Awesomium::WebView *caller,
                                       unsigned int remote_object_id,
                                       const Awesomium::WebString &method_name,
                                       const Awesomium::JSArray &args) override;

	Awesomium::JSValue OnMethodCallWithReturnValue(Awesomium::WebView *caller,
                      unsigned int remote_object_id,
                      const Awesomium::WebString &method_name,
					  const Awesomium::JSArray &args) override;

private:
	std::function<void(NSEngineComponent::EngineCompenentType source,
					   NSEngineComponent::EngineCompenentType,
					   NSEngineComponent::MessageData::MessageList)> callback;

};
