#ifndef DOXYGEN2MARKDOWN_HPP
#define DOXYGEN2MARKDOWN_HPP

#include "ApplicationPrinter.hpp"
#include <sapi/var.hpp>

class Doxygen2Markdown : public ApplicationPrinter {
public:
  class Node : public JsonArray {
  public:
    Node(const JsonArray &array) : JsonArray(array) {}

    JsonValue get_value(const String &key) const {
      for (u32 i = 0; i < count(); i++) {
        if (at(i).to_object().at(key).is_valid()) {
          return at(i).to_object().at(key);
        }
      }
      return JsonValue();
    }

    JsonArray get_array(const String &key) const {
      return get_value(key).to_array();
    }

    String get_string(const String &key) const {
      return get_value(key).to_string();
    }

    String get_key(u32 i) const {
      if (i < count()) {
        StringList key_list = at(i).to_object().keys();
        if (key_list.count()) {
          return key_list.front();
        }
      }
      return String();
    }

    JsonArray get_array(u32 i) const {
      String key = get_key(i);
      return at(i).to_object().at(key);
    }
  };

  Doxygen2Markdown(const String &key, JsonArray value) : m_node(value) {
    m_key = key;
    m_node = value;
  }

  class ConvertOptions {
    API_AB(ConvertOptions, brief, false);
    API_AB(ConvertOptions, public_static_func_visible, true);
    API_AB(ConvertOptions, public_func_visible, true);
    API_AB(ConvertOptions, private_static_func_visible, false);
    API_AB(ConvertOptions, private_func_visible, false);
    API_AB(ConvertOptions, public_type_visible, true);
    API_AB(ConvertOptions, private_type_visible, false);
    API_AB(ConvertOptions, public_attrib_visible, true);
    API_AB(ConvertOptions, private_attrib_visible, false);
    API_AB(ConvertOptions, enum_visible, true);
    API_AB(ConvertOptions, define_visible, true);
  };

  String convert(const ConvertOptions &options);

private:
  class Sibling {
    API_AC(Sibling, JsonValue, value);
    API_AC(Sibling, String, key);
  };

  API_AC(Doxygen2Markdown, Sibling, sibling);

  String m_key;
  Node m_node;

  String get_kind() const { return m_node.get_string("@kind"); }

  // primitive doxygen types

  String enum_to_markdown(const ConvertOptions &options);
  String memberdef_to_markdown(const ConvertOptions &options);
  String variable_to_markdown(const ConvertOptions &options);
  String compounddef_to_markdown(const ConvertOptions &options);
  String struct_to_markdown(const ConvertOptions &options);

  String para_to_markdown(const ConvertOptions &options);
  String param_to_markdown(const ConvertOptions &options);
  String small_to_markdown(const ConvertOptions &options);
  String bold_to_markdown(const ConvertOptions &options);
  String ulink_to_markdown(const ConvertOptions &options);
  String title_to_markdown(const ConvertOptions &options);
  String itemizedlist_to_markdown(const ConvertOptions &options);
  String simplesect_to_markdown(const ConvertOptions &options);
  String listitem_to_markdown(const ConvertOptions &options);
  String programlisting_to_markdown(const ConvertOptions &options);
  String highlight_to_markdown(const ConvertOptions &options);
  String sectiondef_to_markdown(const ConvertOptions &options);
  String ref_to_markdown(const ConvertOptions &options);

  String children_to_markdown(const ConvertOptions &options);
  String hash_text(JsonValue value);
};

#endif // DOXYGEN2MARKDOWN_HPP
