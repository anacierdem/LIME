//
// Created by Ali Naci  Erdem on 31/08/16.
//

#pragma once

#include <Awesomium/WebURL.h>
#include <Awesomium/WebViewListener.h>
#include <SFML/Graphics.hpp>

// A custom Awesomium listener
class CustomListener : public Awesomium::WebViewListener::View
{
sf::Window* window;

public:
	CustomListener(sf::Window* win);

	void OnChangeTitle(Awesomium::WebView* caller,
		const Awesomium::WebString& title);

	/// This event occurs when the page URL has changed.
	void OnChangeAddressBar(Awesomium::WebView* caller,
		const Awesomium::WebURL& url);

	/// This event occurs when the tooltip text has changed. You
	/// should hide the tooltip when the text is empty.
	void OnChangeTooltip(Awesomium::WebView* caller,
		const Awesomium::WebString& tooltip);

	/// This event occurs when the target URL has changed. This
	/// is usually the result of hovering over a link on a page.
	void OnChangeTargetURL(Awesomium::WebView* caller,
		const Awesomium::WebURL& url);

	/// This event occurs when the cursor has changed. This is
	/// is usually the result of hovering over different content.
	void OnChangeCursor(Awesomium::WebView* caller,
		Awesomium::Cursor cursor);

	/// This event occurs when the focused element changes on the page.
	/// This is usually the result of textbox being focused or some other
	/// user-interaction event.
	void OnChangeFocus(Awesomium::WebView* caller,
		Awesomium::FocusedElementType focused_type);

	/// This event occurs when a message is added to the console on the page.
	/// This is usually the result of a JavaScript error being encountered
	/// on a page.
	void OnAddConsoleMessage(Awesomium::WebView* caller,
		const Awesomium::WebString& message,
		int line_number,
		const Awesomium::WebString& source);

	/// This event occurs when a WebView creates a new child WebView
	/// (usually the result of window.open or an external link). It
	/// is your responsibility to display this child WebView in your
	/// application. You should call Resize on the child WebView
	/// immediately after this event to make it match your container
	/// size.
	///
	/// If this is a child of a Windowed WebView, you should call
	/// WebView::set_parent_window on the new view immediately within
	/// this event.
	///
	void OnShowCreatedWebView(Awesomium::WebView* caller,
		Awesomium::WebView* new_view,
		const Awesomium::WebURL& opener_url,
		const Awesomium::WebURL& target_url,
		const Awesomium::Rect& initial_pos,
		bool is_popup);
};
