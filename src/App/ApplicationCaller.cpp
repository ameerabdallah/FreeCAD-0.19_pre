#include "ApplicationCaller.h"

//Gets Application
//Import into Namespace App
App::Application& GetApplication()
{
	return App::GetApplication();
}

//Init Applicaiton
//Import into Class App::Application
//Static
 void init(int argc, char** argv) {
	App::Application::init(argc, argv);
}

/*
* Get mConfig from App::Application
* Returns mConfig - std::map<string,string>
* Static
*/
 std::map<std::string, std::string>&Config() {
	return App::Application::Config();
}

//Run Application - Static
void runApplication() {
	App::Application::runApplication();
}