#ifndef DOXYGENSCRAPER_HPP
#define DOXYGENSCRAPER_HPP

#include "ApplicationPrinter.hpp"
#include <sapi/var.hpp>

class DoxygenScraperKey {
public:
  DoxygenScraperKey(const String &key, const String &kind, bool is_array) {
    m_key = key;
    m_kind = kind;
    m_is_array = is_array;
  }

  const String &key() const { return m_key; }
  const String &kind() const { return m_kind; }
  bool is_array() const { return m_is_array; }

private:
  String m_key;
  String m_kind;
  bool m_is_array = false;
};

class DoxygenScraperObject {
public:
  DoxygenScraperObject(const String &name) { m_name = name; }

  bool operator==(const DoxygenScraperObject &a) const {
    return m_name == a.m_name;
  }

  void add_key(const String &key, const String &kind, bool is_array = false) {
    for (size_t i = 0; i < m_keys.count(); i++) {
      if (m_keys.at(i).key() == key) {
        // key already exists
        return;
      }
    }
    m_keys.push_back(DoxygenScraperKey(key, kind, is_array));
  }

  const String &name() const { return m_name; }

  String constructors() const {
    String result;
    for (auto const &key : m_keys) {
      String output_key = to_output_key(key.key());
      if (key.is_array()) {
        result += "    {\n";
        result += "      JsonArray json_array = object.at(\"" + key.key()
                  + "\").to_array();\n";
        result += "      for(u32 i=0; i < json_array.count(); i++){\n";
        result += "        m_" + translate_name(output_key) + ".push_back("
                  + key.kind() + "(json_array.at(i).to_object()));\n";
        result += "      }\n";
        result += "    }\n";
      } else if (key.kind() == "String") {
        result += "    m_" + translate_name(output_key) + " = object.at(\""
                  + key.key() + "\").to_string();\n";
      } else {
        result += "    m_" + translate_name(output_key) + " = " + key.kind()
                  + "(object.at(\"" + key.key() + "\").to_object());\n";
      }
    }
    return result;
  }

  String accessors() const {
    String result;
    for (auto const &key : m_keys) {
      String output_key = to_output_key(key.key());
      result += "  const " + key.kind() + "& " + translate_name(output_key)
                + "() const { return m_" + translate_name(output_key) + "; }\n";
    }
    return result;
  }

  String members() const {
    String result;
    for (auto const &key : m_keys) {
      String output_key = to_output_key(key.key());
      if (key.is_array()) {
        result += "  Vector<" + key.kind() + "> m_" + translate_name(output_key)
                  + ";\n";
      } else {
        result
          += "  " + key.kind() + " m_" + translate_name(output_key) + ";\n";
      }
    }
    return result;
  }

  static String translate_name(const String &name, bool is_class = false);

  const Vector<DoxygenScraperKey> &keys() const { return m_keys; }

private:
  String to_output_key(const String &key) const {
    String result = key;
    result.replace(String::ToErase("@"), String::ToInsert("a_"));

    result.replace(String::ToErase("#"), String::ToInsert("h_"));

    result.replace(String::ToErase(":"), String::ToInsert("_"));
    return result;
  }
  String m_name;
  Vector<DoxygenScraperKey> m_keys;
};

class DoxygenScraper : public ApplicationPrinter {
public:
  DoxygenScraper();
  void generate_code(const String &file);

private:
  int generate_code_object(
    const String &object_key,
    const JsonObject &object,
    int depth);
  Vector<DoxygenScraperObject> m_objects;
};

#endif // DOXYGENSCRAPER_HPP
