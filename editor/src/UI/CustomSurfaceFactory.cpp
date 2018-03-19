#include "CustomSurfaceFactory.hpp"
#include "CustomSurface.hpp"

CustomSurfaceFactory::CustomSurfaceFactory()
{
}

Awesomium::Surface* CustomSurfaceFactory::CreateSurface(Awesomium::WebView* view,
	int width,
	int height) {
	return new CustomSurface(width, height);
}

void CustomSurfaceFactory::DestroySurface(Awesomium::Surface* surface) {
	delete surface;
}