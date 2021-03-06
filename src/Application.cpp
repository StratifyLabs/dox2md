#include "Application.hpp"
#include <sapi/fs.hpp>
#include <sapi/sys.hpp>
#include <sapi/var.hpp>

#include "Code.hpp"
#include "Doxygen2Markdown.hpp"
#include "MarkdownParser.hpp"

Application::Application() {}

int Application::run() {
  int result;

  {
    PrinterObject po(printer(), "settings");
    settings().initialize();
  }

  if (convert_xml_to_md() < 0) {
    exit(1);
  }

  exit(0);

  String is_parse = cli().get_option(
    "parse",
    Cli::Description("parse markdown in to snippets"));

  if (is_parse.is_empty()) {
    is_parse = "true";
  }

  if (is_parse == "true") {
    printer().open_object("markdown");
    result = parse_markdown();
    printer().close_object();
    if (result < 0) {
      return -1;
    }
  }

  String is_code = cli().get_option(
    "generate",
    Cli::Description("generate code from snippets"));

  if (is_code.is_empty()) {
    is_code = "true";
  }
  if (is_code == "true") {
    printer().open_object("code");
    result = generate_code();
    printer().close_object();
    if (result < 0) {
      return -1;
    }
  }

  return 0;
}

int Application::convert_xml_to_md() {

  for (const auto &project : settings().projects()) {

    ProjectSettings project_settings(settings().project(project));

    StringList input_file_list = project_settings.load_input_files();

    String json_directory_path
      = FileInfo::parent_directory(project_settings.input_directory_path())
        + "/json";

    String md_directory_path
      = FileInfo::parent_directory(project_settings.input_directory_path())
        + "/md";

    printer().info("Json dir is " + json_directory_path);

    if (
      Dir::exists(json_directory_path) == false
      && Dir::create(json_directory_path, Permissions(0777)) < 0) {
      printer().error("failed to create json directory " + json_directory_path);
      return -1;
    }

    if (
      Dir::exists(md_directory_path) == false
      && Dir::create(md_directory_path, Permissions(0777)) < 0) {
      printer().error("failed to create md directory " + md_directory_path);
      return -1;
    }

    for (const auto &input_file : input_file_list) {

      String json_output_file
        = json_directory_path + "/" + FileInfo::base_name(input_file) + ".json";
      printer().info("processing " + input_file + " -> " + json_output_file);

      JsonObject input_file_object = JsonDocument()
                                       .load(
                                         JsonDocument::XmlFilePath(input_file),
                                         JsonDocument::XmlIsFlat(false))
                                       .to_object();

      String md_output_file_path
        = md_directory_path + "/" + FileInfo::base_name(input_file) + ".md";

      printer().info(
        "processing " + json_output_file + " -> " + md_output_file_path);
      String md = Doxygen2Markdown(
                    "doxygen",
                    input_file_object.at("doxygen").to_array())
                    .convert(Doxygen2Markdown::ConvertOptions());

      File md_output_file;
      if (
        md_output_file.create(md_output_file_path, File::IsOverwrite(true)) < 0
        || md_output_file.write(md) < 0) {
        printer().error(
          "failed to create markdown output file %s",
          md_output_file_path.cstring());
      }

      if (!input_file_object.is_valid()) {
        printer().warning("failed to convert " + input_file + " to json");
      } else {

        if (
          JsonDocument().save(
            input_file_object,
            fs::File::Path(json_output_file))
          < 0) {
          printer().warning("failed to save " + json_output_file);
          return -1;
        }
      }
    }
  }

  return 0;
}

int Application::parse_markdown() {
#if 0
  JsonArray markdown = settings().markdown();
  MarkdownParser parser;

  for (u32 i = 0; i < markdown.count(); i++) {
    JsonObject markdown_entry = markdown.at(i).to_object();

    String input_path = markdown_entry.at("input").to_string();

    String output_path = markdown_entry.at("output").to_string();

    output_path << "/snippets";

    printer().open_object(input_path);

    if (
      Dir::create(
        output_path,
        Permissions::all_access(),
        Dir::IsRecursive(true))
      < 0) {

      printer().error(
        "failed to create output directory '%s'",
        output_path.cstring());
    }

    Vector<String> input_file_list = Dir::read_list(input_path);

    for (auto file_path : input_file_list) {

      if (file_path == ".") {
        continue;
      }
      if (file_path == "..") {
        continue;
      }

      JsonObject code_blocks = parser.extract_code_snippets(
        String() << input_path << "/" << file_path,
        markdown_entry);

      if (code_blocks.is_empty() == false) {
        String output_file_path;

        file_path.replace(String::ToErase(".md"), String::ToInsert(""));

        printer().key(
          file_path,
          "%d snippets",
          code_blocks.at("snippets").to_array().count());

        output_file_path << output_path << "/" << file_path << ".json";

        JsonDocument document;

        document.set_flags(JsonDocument::INDENT_2);

        if (document.save(code_blocks, fs::File::Path(output_file_path)) < 0) {
          printer().close_object();
          printer().error(
            "failed to save snippet file as '%s'",
            output_file_path.cstring());
          return -1;
        }
      }
    }
  }
  printer().close_object();
#endif
  return 0;
}

int Application::generate_code() {
#if 0
  JsonArray markdown = settings().markdown();
  Code code;

  for (u32 i = 0; i < markdown.count(); i++) {
    JsonObject markdown_entry = markdown.at(i).to_object();
    String output_path = markdown_entry.at("output").to_string();

    String snippets_path = String() << output_path << "/snippets";

    Vector<String> snippet_files = Dir::read_list(snippets_path);

    for (auto input_file_name : snippet_files) {

      if (input_file_name == ".") {
        continue;
      }
      if (input_file_name == "..") {
        continue;
      }

      String input_file_path;
      input_file_path << snippets_path << "/" << input_file_name;

      printer().debug("generate code for '%s'", input_file_path.cstring());

      code.generate(input_file_path);
    }
  }
#endif
  return 0;
}

int Application::build_code() { return 0; }
