#include <cstdio>

#include <sapi/fs.hpp>
#include <sapi/sys.hpp>
#include <sapi/var.hpp>
#include "ApplicationPrinter.hpp"
#include "Application.hpp"
#include "DoxygenScraper.hpp"
#include "Doxygen2Markdown.hpp"

#define VERSION "0.1"

int main(int argc, char * argv[]){
	sys::Cli cli(argc, argv);
	cli.set_publisher("Stratify Labs, Inc");

	//DoxygenScraper().generate_code("../StratifyAPI/json");

	JsonObject object = JsonDocument().load(
				fs::File::Path("../StratifyAPI/json/classcalc_1_1_base64.json")
				).to_object();


	String result = Doxygen2Markdown(
				"memberdef",
				object.at("doxygen").to_object()
				.at("compounddef").to_object()
				.at("sectiondef").to_array()
				.at(0).to_object().at("memberdef"),
				false
				).to_markdown();

	printf("%s", result.cstring());

	exit(1);

	Object::set_cli(cli);

	ApplicationPrinter::printer().set_verbose_level(
				cli.get_option(
					"verbose",
					Cli::Description("verbosity level use <debug|message|info|warning|error|trace>")
					)
				);

	if( cli.get_option(
			 "version",
			 Cli::Description("show the md2code version information")
			 ) == "true" ){
		ApplicationPrinter::printer().open_object("about");
		ApplicationPrinter::printer().key("publisher", "Stratify Labs, Inc");
		ApplicationPrinter::printer().key("version", VERSION);
		ApplicationPrinter::printer().key("gitHash", SOS_GIT_HASH);
		ApplicationPrinter::printer().key("apiVersion", api::ApiInfo::version());
		ApplicationPrinter::printer().key("apiGitHash", api::ApiInfo::git_hash());
		exit(0);
	}


	Application().run();





	return 0;
}

