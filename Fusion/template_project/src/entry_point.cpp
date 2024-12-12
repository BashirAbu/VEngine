#ifndef VE_EDITOR
#include "ve_engine.h"
#include "ve_assets_manager.h"
#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif
#ifdef __EMSCRIPTEN__

void EMSCRIPTEN_RUN() 
{
	VE::Engine::GetSingleton()->Run();
}

#endif
int main(int argc, char* argv[])
{
	VE::EngineDesc desc;
	desc.projectDetails.path = ".VEProject";
	VE::Engine* engine = new VE::Engine(desc);
#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(EMSCRIPTEN_RUN, 0, 1);
#else
	engine->Run();
#endif
	delete engine;
	return 0;
}

#endif