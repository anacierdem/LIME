//
// Created by Ali Naci  Erdem on 24/08/16.
//

#include "Util.hpp"

//TODO: Move somewhere else
///
/// Inject an SFML Key Event into a given WebView (https://gist.github.com/khrona/3239125)
///
void Util::handleKeyEvent(Awesomium::WebView* webView, const sf::Event e) {
	if (!(e.type == sf::Event::KeyPressed || e.type == sf::Event::KeyReleased || e.type == sf::Event::TextEntered))
		return;

	Awesomium::WebKeyboardEvent keyEvent;

	if (e.type != sf::Event::TextEntered) {
		keyEvent.type = e.type == sf::Event::KeyPressed ?
			Awesomium::WebKeyboardEvent::kTypeKeyDown :
			Awesomium::WebKeyboardEvent::kTypeKeyUp;

		keyEvent.virtual_key_code = getWebKeyFromSFMLKey(e.key.code);
		keyEvent.modifiers = 0;

		if (e.key.alt)
			keyEvent.modifiers |= Awesomium::WebKeyboardEvent::kModAltKey;
		if (e.key.control)
			keyEvent.modifiers |= Awesomium::WebKeyboardEvent::kModControlKey;
		if (e.key.system)
			keyEvent.modifiers |= Awesomium::WebKeyboardEvent::kModMetaKey;
		if (e.key.shift)
			keyEvent.modifiers |= Awesomium::WebKeyboardEvent::kModShiftKey;
		if (e.key.code == sf::Keyboard::Key::Numpad0 ||
			e.key.code == sf::Keyboard::Key::Numpad1 ||
			e.key.code == sf::Keyboard::Key::Numpad2 ||
			e.key.code == sf::Keyboard::Key::Numpad3 ||
			e.key.code == sf::Keyboard::Key::Numpad4 ||
			e.key.code == sf::Keyboard::Key::Numpad5 ||
			e.key.code == sf::Keyboard::Key::Numpad6 ||
			e.key.code == sf::Keyboard::Key::Numpad7 ||
			e.key.code == sf::Keyboard::Key::Numpad8 ||
			e.key.code == sf::Keyboard::Key::Numpad9)
			keyEvent.modifiers |= Awesomium::WebKeyboardEvent::kModIsKeypad;

		webView->InjectKeyboardEvent(keyEvent);
	}
	else {
		sf::Uint32 chr;
		chr = e.text.unicode;

		keyEvent.text[0] = *(reinterpret_cast<wchar16*>(&chr));
		keyEvent.text[1] = *(reinterpret_cast<wchar16*>(&chr) + 1);

		keyEvent.unmodified_text[0] = *(reinterpret_cast<wchar16*>(&chr));
		keyEvent.unmodified_text[1] = *(reinterpret_cast<wchar16*>(&chr) + 1);

		keyEvent.type = Awesomium::WebKeyboardEvent::kTypeChar;

		keyEvent.virtual_key_code = chr;
		keyEvent.native_key_code = chr;

		webView->InjectKeyboardEvent(keyEvent);
	}
}

/// Get an Awesomium KeyCode from an SDLKey Code
int Util::getWebKeyFromSFMLKey(sf::Keyboard::Key key) {
	switch (key) {
	case sf::Keyboard::Key::BackSpace: return Awesomium::KeyCodes::AK_BACK;
	case sf::Keyboard::Key::Tab: return Awesomium::KeyCodes::AK_TAB;
	case sf::Keyboard::Key::Return: return Awesomium::KeyCodes::AK_RETURN;
	case sf::Keyboard::Key::Pause: return Awesomium::KeyCodes::AK_PAUSE;
	case sf::Keyboard::Key::Escape: return Awesomium::KeyCodes::AK_ESCAPE;
	case sf::Keyboard::Key::Space: return Awesomium::KeyCodes::AK_SPACE;
	case sf::Keyboard::Key::Menu: return Awesomium::KeyCodes::AK_MENU;
	case sf::Keyboard::Key::Num0: return Awesomium::KeyCodes::AK_0;
	case sf::Keyboard::Key::Num1: return Awesomium::KeyCodes::AK_1;
	case sf::Keyboard::Key::Num2: return Awesomium::KeyCodes::AK_2;
	case sf::Keyboard::Key::Num3: return Awesomium::KeyCodes::AK_3;
	case sf::Keyboard::Key::Num4: return Awesomium::KeyCodes::AK_4;
	case sf::Keyboard::Key::Num5: return Awesomium::KeyCodes::AK_5;
	case sf::Keyboard::Key::Num6: return Awesomium::KeyCodes::AK_6;
	case sf::Keyboard::Key::Num7: return Awesomium::KeyCodes::AK_7;
	case sf::Keyboard::Key::Num8: return Awesomium::KeyCodes::AK_8;
	case sf::Keyboard::Key::Num9: return Awesomium::KeyCodes::AK_9;
	case sf::Keyboard::Key::A: return Awesomium::KeyCodes::AK_A;
	case sf::Keyboard::Key::B: return Awesomium::KeyCodes::AK_B;
	case sf::Keyboard::Key::C: return Awesomium::KeyCodes::AK_C;
	case sf::Keyboard::Key::D: return Awesomium::KeyCodes::AK_D;
	case sf::Keyboard::Key::E: return Awesomium::KeyCodes::AK_E;
	case sf::Keyboard::Key::F: return Awesomium::KeyCodes::AK_F;
	case sf::Keyboard::Key::G: return Awesomium::KeyCodes::AK_G;
	case sf::Keyboard::Key::H: return Awesomium::KeyCodes::AK_H;
	case sf::Keyboard::Key::I: return Awesomium::KeyCodes::AK_I;
	case sf::Keyboard::Key::J: return Awesomium::KeyCodes::AK_J;
	case sf::Keyboard::Key::K: return Awesomium::KeyCodes::AK_K;
	case sf::Keyboard::Key::L: return Awesomium::KeyCodes::AK_L;
	case sf::Keyboard::Key::M: return Awesomium::KeyCodes::AK_M;
	case sf::Keyboard::Key::N: return Awesomium::KeyCodes::AK_N;
	case sf::Keyboard::Key::O: return Awesomium::KeyCodes::AK_O;
	case sf::Keyboard::Key::P: return Awesomium::KeyCodes::AK_P;
	case sf::Keyboard::Key::Q: return Awesomium::KeyCodes::AK_Q;
	case sf::Keyboard::Key::R: return Awesomium::KeyCodes::AK_R;
	case sf::Keyboard::Key::S: return Awesomium::KeyCodes::AK_S;
	case sf::Keyboard::Key::T: return Awesomium::KeyCodes::AK_T;
	case sf::Keyboard::Key::U: return Awesomium::KeyCodes::AK_U;
	case sf::Keyboard::Key::V: return Awesomium::KeyCodes::AK_V;
	case sf::Keyboard::Key::W: return Awesomium::KeyCodes::AK_W;
	case sf::Keyboard::Key::X: return Awesomium::KeyCodes::AK_X;
	case sf::Keyboard::Key::Y: return Awesomium::KeyCodes::AK_Y;
	case sf::Keyboard::Key::Z: return Awesomium::KeyCodes::AK_Z;
	case sf::Keyboard::Key::Delete: return Awesomium::KeyCodes::AK_DELETE;
	case sf::Keyboard::Key::Numpad0: return Awesomium::KeyCodes::AK_NUMPAD0;
	case sf::Keyboard::Key::Numpad1: return Awesomium::KeyCodes::AK_NUMPAD1;
	case sf::Keyboard::Key::Numpad2: return Awesomium::KeyCodes::AK_NUMPAD2;
	case sf::Keyboard::Key::Numpad3: return Awesomium::KeyCodes::AK_NUMPAD3;
	case sf::Keyboard::Key::Numpad4: return Awesomium::KeyCodes::AK_NUMPAD4;
	case sf::Keyboard::Key::Numpad5: return Awesomium::KeyCodes::AK_NUMPAD5;
	case sf::Keyboard::Key::Numpad6: return Awesomium::KeyCodes::AK_NUMPAD6;
	case sf::Keyboard::Key::Numpad7: return Awesomium::KeyCodes::AK_NUMPAD7;
	case sf::Keyboard::Key::Numpad8: return Awesomium::KeyCodes::AK_NUMPAD8;
	case sf::Keyboard::Key::Numpad9: return Awesomium::KeyCodes::AK_NUMPAD9;
	case sf::Keyboard::Key::Divide: return Awesomium::KeyCodes::AK_DIVIDE;
	case sf::Keyboard::Key::Multiply: return Awesomium::KeyCodes::AK_MULTIPLY;
	case sf::Keyboard::Key::Subtract: return Awesomium::KeyCodes::AK_SUBTRACT;
	case sf::Keyboard::Key::Add: return Awesomium::KeyCodes::AK_ADD;
	case sf::Keyboard::Key::Up: return Awesomium::KeyCodes::AK_UP;
	case sf::Keyboard::Key::Down: return Awesomium::KeyCodes::AK_DOWN;
	case sf::Keyboard::Key::Right: return Awesomium::KeyCodes::AK_RIGHT;
	case sf::Keyboard::Key::Left: return Awesomium::KeyCodes::AK_LEFT;
	case sf::Keyboard::Key::Insert: return Awesomium::KeyCodes::AK_INSERT;
	case sf::Keyboard::Key::Home: return Awesomium::KeyCodes::AK_HOME;
	case sf::Keyboard::Key::End: return Awesomium::KeyCodes::AK_END;
	case sf::Keyboard::Key::PageUp: return Awesomium::KeyCodes::AK_PRIOR;
	case sf::Keyboard::Key::PageDown: return Awesomium::KeyCodes::AK_NEXT;
	case sf::Keyboard::Key::F1: return Awesomium::KeyCodes::AK_F1;
	case sf::Keyboard::Key::F2: return Awesomium::KeyCodes::AK_F2;
	case sf::Keyboard::Key::F3: return Awesomium::KeyCodes::AK_F3;
	case sf::Keyboard::Key::F4: return Awesomium::KeyCodes::AK_F4;
	case sf::Keyboard::Key::F5: return Awesomium::KeyCodes::AK_F5;
	case sf::Keyboard::Key::F6: return Awesomium::KeyCodes::AK_F6;
	case sf::Keyboard::Key::F7: return Awesomium::KeyCodes::AK_F7;
	case sf::Keyboard::Key::F8: return Awesomium::KeyCodes::AK_F8;
	case sf::Keyboard::Key::F9: return Awesomium::KeyCodes::AK_F9;
	case sf::Keyboard::Key::F10: return Awesomium::KeyCodes::AK_F10;
	case sf::Keyboard::Key::F11: return Awesomium::KeyCodes::AK_F11;
	case sf::Keyboard::Key::F12: return Awesomium::KeyCodes::AK_F12;
	case sf::Keyboard::Key::F13: return Awesomium::KeyCodes::AK_F13;
	case sf::Keyboard::Key::F14: return Awesomium::KeyCodes::AK_F14;
	case sf::Keyboard::Key::F15: return Awesomium::KeyCodes::AK_F15;
	case sf::Keyboard::Key::RShift: return Awesomium::KeyCodes::AK_RSHIFT;
	case sf::Keyboard::Key::LShift: return Awesomium::KeyCodes::AK_LSHIFT;
	case sf::Keyboard::Key::RControl: return Awesomium::KeyCodes::AK_RCONTROL;
	case sf::Keyboard::Key::LControl: return Awesomium::KeyCodes::AK_LCONTROL;
	case sf::Keyboard::Key::RAlt: return Awesomium::KeyCodes::AK_RMENU;
	case sf::Keyboard::Key::LAlt: return Awesomium::KeyCodes::AK_LMENU;
	case sf::Keyboard::Key::RSystem: return Awesomium::KeyCodes::AK_LWIN;
	case sf::Keyboard::Key::LSystem: return Awesomium::KeyCodes::AK_RWIN;

	case sf::Keyboard::Key::Unknown: return Awesomium::KeyCodes::AK_UNKNOWN;
	case sf::Keyboard::Key::Quote: return Awesomium::KeyCodes::AK_UNKNOWN;
	case sf::Keyboard::Key::Tilde: return Awesomium::KeyCodes::AK_UNKNOWN;
	case sf::Keyboard::Key::Dash: return Awesomium::KeyCodes::AK_UNKNOWN;
	case sf::Keyboard::Key::LBracket: return Awesomium::KeyCodes::AK_UNKNOWN;
	case sf::Keyboard::Key::RBracket: return Awesomium::KeyCodes::AK_UNKNOWN;
	case sf::Keyboard::Key::SemiColon: return Awesomium::KeyCodes::AK_UNKNOWN;
	case sf::Keyboard::Key::Equal: return Awesomium::KeyCodes::AK_UNKNOWN;
	case sf::Keyboard::Key::Comma: return Awesomium::KeyCodes::AK_UNKNOWN;
	case sf::Keyboard::Key::Period: return Awesomium::KeyCodes::AK_UNKNOWN;
	case sf::Keyboard::Key::Slash: return Awesomium::KeyCodes::AK_UNKNOWN;

	default:
		return Awesomium::KeyCodes::AK_UNKNOWN;
	}
}
