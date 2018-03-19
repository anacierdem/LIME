#include <fstream>
#include "CustomSource.hpp"
#include "../globals.hpp"
#include <memory>

using namespace std;
using namespace Awesomium;

CustomSource::CustomSource() { };

//TODO: will use DataPakSource
void CustomSource::OnRequest(int request_id,
	const Awesomium::ResourceRequest& request,
	const Awesomium::WebString& path) {

	std::string str;
	std::string file_contents;

	ifstream myReadFile;

	std::string base = BASE_PATH"";
	WebString finalPath = WSLit(base.c_str()).Append(path);
	int len = finalPath.ToUTF8(nullptr, 0);
	std::unique_ptr<char[]> file_name (new char[len + 1]);
	finalPath.ToUTF8(file_name.get(), len+1);

	std::string nameStr(file_name.get());
	std::string mimeType = "text/html";

	if (nameStr.find(".woff") != std::string::npos)
		mimeType = "application/font-woff";

	myReadFile.open(nameStr, std::ios::in);
	
	if (myReadFile.is_open()) {
		while (std::getline(myReadFile, str))
		{
			file_contents += str;
			file_contents.push_back('\n');
		}
	}

	myReadFile.close();

	SendResponse(request_id,
		file_contents.length(),
	    reinterpret_cast<const unsigned char*>(file_contents.c_str()),
		WSLit(mimeType.c_str()));
}
