#include <ve_engine.h>
#include "project_dialog.h"

int main(int argc, char* argv[])
{
	VE::EngineDesc desc;
	//parse command line arguments
	bool runEngine = false;
	if (argc == 1)
	{
		//show create project dialog.
		ProjectDialog pd;
		runEngine = pd.Run(&desc.projectDetails);
	}
	else if (strcmp(argv[1], "-p") == 0)
	{
		for (const auto& entry : std::filesystem::directory_iterator(argv[2]))
		{
			if (std::filesystem::is_regular_file(entry) && entry.path().extension().string() == (std::string) "." + VE_PROJECT_FILE_EXTENSION)
			{
				desc.projectDetails.path = entry.path();
			}
		}
		runEngine = true;
	}

	if (runEngine && !desc.projectDetails.path.empty())
	{
		VE::Engine* engine = new VE::Engine(desc);
		engine->Run();
		delete engine;
	}
	
	return 0;
}