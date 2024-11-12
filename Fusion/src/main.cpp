#include <ve_engine.h>
#include <string.h>
int main(int argc, char* argv[])
{
	VE::EngineDesc desc;
	//parse command line arguments
	if (strcmp(argv[1], "-p") == 0)
	{
		desc.projectDetails.path = argv[2];
		desc.runtimeType = VE::RuntimeType::Editor;
	}
	else if (strcmp(argv[1], "-g") == 0)
	{
		TraceLog(LOG_DEBUG, "Game path");
		desc.runtimeType = VE::RuntimeType::Game;
	}
	else 
	{
		TraceLog(LOG_FATAL, "No game or project path provided!");
	}

	VE::Engine* engine = new VE::Engine(desc);
	engine->Run();
	delete engine;
	return 0;
}