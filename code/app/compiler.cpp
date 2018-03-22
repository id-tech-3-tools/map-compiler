#include "argh.h"
#include "compiler.hpp"
#include "compiler_api.h"
#include <iostream>
#include <string>
#include "tinyformat.h"
#include "path.h"
#include "help.hpp"

int setupCompiler(int argc, char** argv)
{
	if (argc < 2)
	{
		filesystem::path path{ argv[0] };
		tfm::printfln("Usage: %s [general options] [options] mapfile", path.filename());
		tfm::printfln("Help:  %s --help", path.filename());
		return 0;
	}

	argh::parser cmdl{ "help" };
	cmdl.parse(argc, argv, argh::parser::Mode::PREFER_PARAM_FOR_UNREG_OPTION);

	std::string helpArgument;
	if (cmdl["help"] || cmdl("help") >> helpArgument)
	{
		HelpPrinter *help = new CompilerHelpPrinter();
		help->print(helpArgument);
		delete help;
	} else
	{
		return compilerMain(argc, argv);
	}

	return 0;
}
