#pragma once
#include "Application.h"

extern "C" {
	extern AppExport App::Application& GetApplication(void);
	extern AppExport void init(int argc, char** argv);
	extern AppExport std::map<std::string, std::string>& Config();
	extern AppExport void runApplication();
}