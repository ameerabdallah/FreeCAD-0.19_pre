#pragma once
#include "Application.h"
#include <map>

extern "C" {
	extern AppExport App::Application& GetApplication(void);
	extern AppExport void init(int argc, char** argv);
	extern AppExport std::map<std::string, std::string>& Config();
	extern AppExport void RunApplication();
	extern AppExport void CloseAllDocuments();
}