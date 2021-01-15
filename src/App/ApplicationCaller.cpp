#include "ApplicationCaller.h"

//Gets Application
//Import into Namespace App
App::Application& GetApplication() //reference 
{
	return App::GetApplication();
}

void CloseAllDocuments()
{
	 App::GetApplication().closeAllDocuments();
}

//Init Applicaiton
//Import into Class App::Application
//Static
 void Init(int argc, char** argv) {
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
 //Init Application

//Run Application - Static
void RunApplication() {
	App::Application::runApplication();
}