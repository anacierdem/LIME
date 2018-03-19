#pragma once

#include <Awesomium/DataSource.h>
#include <Awesomium/STLHelpers.h>

// A custom Awesomium file source that is used to load svg fonts
// which seem to work OK with Awesomium webkit version
class CustomSource : public Awesomium::DataSource {
public:
	CustomSource();

	void OnRequest(int request_id,
	const Awesomium::ResourceRequest& request,
	const Awesomium::WebString& path) override;
};