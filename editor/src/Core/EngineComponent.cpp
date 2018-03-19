//
// Created by Ali Naci  Erdem on 01/09/16.
//

#include "EngineComponent.hpp"
using namespace Awesomium;
using namespace NSEngineComponent;

EngineComponent::EngineComponent()
{
	myType = NONE;
};

void EngineComponent::receiveMessage(EngineCompenentType, EngineCompenentType, MessageData::MessageList) {};
void EngineComponent::setMessageInterface(std::function<void(EngineCompenentType, EngineCompenentType, MessageData::MessageList)> iface) {
	using namespace std::placeholders;
	//Pass myType automatically
	sendMessage = std::bind(iface, myType, _1, _2);
};