#pragma once

#include <Awesomium/WebCore.h>

class CustomSurfaceFactory : public Awesomium::SurfaceFactory
{
public:
	CustomSurfaceFactory();

	///
	/// This event will be called whenever a WebView needs to create a new
	/// Surface. You should return your own Surface implementation here.
	///
	/// @param  The WebView requesting the Surface.
	///
	/// @param  width  The initial width of the Surface.
	///
	/// @param  height  The initial height of the Surface.
	///
	/// @return You should return your own Surface implementation here. The
	///         WebView will call SurfaceFactory::DestroySurface when it is done
	///         using the Surface.
	///
	Awesomium::Surface* CreateSurface(Awesomium::WebView* view,
		int width,
		int height) override;

	///
	/// This event will be called whenever a WebView needs to destroy a Surface.
	/// You should cast this Surface to your own implementation and destroy it.
	///
	/// @param  surface  The surface to destroy (you should cast this to your
	///                  own implementation).
	///
	void DestroySurface(Awesomium::Surface* surface) override;
};

