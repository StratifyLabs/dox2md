#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include "ApplicationPrinter.hpp"
#include "Doxygen.hpp"
#include <sapi/fs.hpp>
#include <sapi/var.hpp>

class ProjectSettings : public ApplicationPrinter {
public:
  ProjectSettings(JsonObject project) { m_project = project; }

  String input_directory_path() const {
    return m_project.at("input").to_string();
  }
  String output_directory_path() const {
    return m_project.at("output").to_string();
  }

  StringList pages() const {
    JsonArray pages_array = m_project.at("pages").to_array();
    Vector<String> result;
    for (u32 i = 0; i < pages_array.count(); i++) {
      result.push_back(pages_array.at(i).to_string());
    }
    return result;
  }

  StringList load_input_files() const {
    return Dir::read_list(input_directory_path(), [&](const String &entry) {
      if (entry.find(".xml") != String::npos) {
        // only convert files that are part of the documentation pages output
        if (Doxygen::is_file_name_match_for_pages(entry, pages())) {
          return input_directory_path() + "/" + entry;
        }
      }
      return String();
    });
  }

private:
  JsonObject m_project;
};

class Settings : public ApplicationPrinter {
public:
  int initialize();

  Vector<String> projects() const { return m_settings.keys(); }

  JsonObject project(const String &name) const {
    return m_settings.at(name).to_object();
  }

private:
  JsonObject m_settings;
  const String settings_file_name() { return "dox2md.json"; }
};

#endif // SETTINGS_HPP
