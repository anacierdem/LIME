//
// Created by Ali Naci  Erdem on 01/09/16.
//

#ifndef EDITOR_ENGINECOMPONENT_HPP
#define EDITOR_ENGINECOMPONENT_HPP

#include <functional>
#include <memory>
#include <iostream>
#include <Awesomium/JSArray.h>

#include "../Misc/VectorExtension.hpp"

namespace NSEngineComponent {

	// All engine compoenents should be registered here
    enum EngineCompenentType {
        NONE, WEB_VIEW, ENGINE_MANAGER, GAME_RENDERER, MINI_MAP_MANAGER, PALETTE_MANAGER
    };

	//http://stackoverflow.com/questions/8627625/is-it-possible-to-make-function-that-will-accept-multiple-data-types-for-given-a
	//http://stackoverflow.com/questions/26580791/pass-any-data-type-to-function-in-c

	// A messaging scheme to pass messages around components. The upside is that
	// there is no type and argument count restriction. Type is checked dynamically
	// and will throw if receiver attempts to get an incorrect type.
	class MessageData {
	public:
		MessageData() {};
		typedef std::unique_ptr<MessageData> MessageList[];
		virtual ~MessageData() {};
	};

	template<typename T>
	class Message : public MessageData {
		T val;
	public:
		template<typename U>
		friend U GetMessage(std::unique_ptr<MessageData>& in);

		Message(T i)
		{
			val = i;
		};

		~Message()
		{
		};
	};

	template<typename T>
	T GetMessage(std::unique_ptr<MessageData>& in) {
		Message<T>* tmp = dynamic_cast<Message<T>*>(in.get());
		if (tmp) {
			return tmp->val;
		}
		std::cout << "Incorrect type!" << std::endl;
		throw "Incorrect type!";
	};

	template<typename T>
	std::unique_ptr<Message<T>> MakeMessage(T val)
	{
		return std::make_unique<Message<T>>(val);
	};

	// Interface to define game engine components that can receive global events
	// EngineManager will call its public functions on critical engine events
    class EngineComponent {
    public:
        EngineComponent();

        virtual void update(sf::Time deltaTime) = 0;

        virtual void injectEvent(sf::Event e) = 0;

        virtual void resize(sf::Vector2i resolution) = 0;

        virtual void draw() = 0;

        virtual void receiveMessage(EngineCompenentType source, EngineCompenentType, MessageData::MessageList) = 0;

        void setMessageInterface(std::function<void(EngineCompenentType source, EngineCompenentType target,
                                                    MessageData::MessageList)> iface);

    protected:
        std::function<void(EngineCompenentType, MessageData::MessageList)> sendMessage;
        EngineCompenentType myType;
    };

}
#endif //EDITOR_ENGINECOMPONENT_HPP
