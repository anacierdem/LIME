//
// Created by Ali Naci  Erdem on 31/08/16.
//

#pragma once

#include "CustomListener.hpp"

CustomListener::CustomListener(sf::Window* win) {
	window = win;
}

void CustomListener::OnChangeTitle(Awesomium::WebView* caller,
	const Awesomium::WebString& title) {

}

/// This event occurs when the page URL has changed.
void CustomListener::OnChangeAddressBar(Awesomium::WebView* caller,
	const Awesomium::WebURL& url) {

}

/// This event occurs when the tooltip text has changed. You
/// should hide the tooltip when the text is empty.
void CustomListener::OnChangeTooltip(Awesomium::WebView* caller,
	const Awesomium::WebString& tooltip) {

}

/// This event occurs when the target URL has changed. This
/// is usually the result of hovering over a link on a page.
void CustomListener::OnChangeTargetURL(Awesomium::WebView* caller,
	const Awesomium::WebURL& url) {

}

/// This event occurs when the cursor has changed. This is
/// is usually the result of hovering over different content.
void CustomListener::OnChangeCursor(Awesomium::WebView* caller,
	Awesomium::Cursor cursor) {

}

/// This event occurs when the focused element changes on the page.
/// This is usually the result of textbox being focused or some other
/// user-interaction event.
void CustomListener::OnChangeFocus(Awesomium::WebView* caller,
	Awesomium::FocusedElementType focused_type) {

}

/// This event occurs when a message is added to the console on the page.
/// This is usually the result of a JavaScript error being encountered
/// on a page.
void CustomListener::OnAddConsoleMessage(Awesomium::WebView* caller,
	const Awesomium::WebString& message,
	int line_number,
	const Awesomium::WebString& source) {

}

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
void CustomListener::OnShowCreatedWebView(Awesomium::WebView* caller,
	Awesomium::WebView* new_view,
	const Awesomium::WebURL& opener_url,
	const Awesomium::WebURL& target_url,
	const Awesomium::Rect& initial_pos,
	bool is_popup) {

}

