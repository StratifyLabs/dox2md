#include <cstdio>

#include "Application.hpp"
#include "ApplicationPrinter.hpp"
#include "Doxygen2Markdown.hpp"
#include "DoxygenScraper.hpp"
#include <sapi/fs.hpp>
#include <sapi/sys.hpp>
#include <sapi/var.hpp>

#define VERSION "0.1"

int main(int argc, char *argv[]) {
  sys::Cli cli(argc, argv);
  cli.set_publisher("Stratify Labs, Inc");

  ApplicationPrinter::printer().set_verbose_level(cli.get_option("verbose"));
  // DoxygenScraper().generate_code("../StratifyAPI/json");

#if 0
  JsonObject object
    = JsonDocument()
        .load(fs::File::Path("../StratifyOS/json/structadc__info__t.json"))
        .to_object();

  String result = Doxygen2Markdown("doxygen", object.at("doxygen"))
                    .convert(Doxygen2Markdown::ConvertOptions());

  printf("%s", result.cstring());

  exit(1);
#endif

  Object::set_cli(cli);

  ApplicationPrinter::printer().set_verbose_level(cli.get_option(
    "verbose",
    Cli::Description(
      "verbosity level use <debug|message|info|warning|error|trace>")));

  if (
    cli.get_option(
      "version",
      Cli::Description("show the md2code version information"))
    == "true") {
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
