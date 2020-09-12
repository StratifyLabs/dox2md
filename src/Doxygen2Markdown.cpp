#include "Doxygen2Markdown.hpp"

String Doxygen2Markdown::convert(const ConvertOptions &options) {

  PRINTER_TRACE(printer(), "convert key " + m_key);

  if (m_key == "memberdef") {
    return memberdef_to_markdown(options);
  }

  if (m_key == "sectiondef") {
    return sectiondef_to_markdown(options);
  }

  if (m_key == "para") {
    return para_to_markdown(options);
  }
  if (m_key == "highlight") {
    return highlight_to_markdown(options);
  }
  if (m_key == "programlisting") {
    return programlisting_to_markdown(options);
  }

  if (m_key == "small") {
    return small_to_markdown(options);
  }

  if (m_key == "simplesect") {
    return simplesect_to_markdown(options);
  }

  if (m_key == "bold") {
    return bold_to_markdown(options);
  }

  if (m_key == "param") {
    return param_to_markdown(options);
  }

  if (m_key == "enum") {
    return enum_to_markdown(options);
  }

  if (m_key == "memberdef") {
    return memberdef_to_markdown(options);
  }
  if (m_key == "variable") {
    return variable_to_markdown(options);
  }
  if (m_key == "compounddef") {
    return compounddef_to_markdown(options);
  }
  if (m_key == "struct") {
    return struct_to_markdown(options);
  }

  if (m_key == "ref") {
    return ref_to_markdown(options);
  }

  return children_to_markdown(options);
}

String Doxygen2Markdown::sectiondef_to_markdown(const ConvertOptions &options) {
  String result;

  result += children_to_markdown(options);

  return result;
}

String Doxygen2Markdown::children_to_markdown(const ConvertOptions &options) {
  String result;

  for (u32 i = 0; i < m_node.count(); i++) {

    String key = m_node.get_key(i);

    PRINTER_TRACE(printer(), "process child " + key);
    JsonValue value = m_node.at(i).to_object().at(key);

    if (value.is_array()) {
      result += Doxygen2Markdown(key, value.to_array()).convert(options);
    } else if ((key == "#text") || (key == "array") || (key == "type")) {
      result += value.to_string() + " ";
    } else if (key == "emphasis") {
      result += "*" + value.to_string() + "* ";
    }
  }

  return result;
}

String Doxygen2Markdown::enum_to_markdown(const ConvertOptions &options) {
#if 0
  JsonObject enum_object = m_value.to_object();
  String name = enum_object.at("name").to_string();
  if (name.find("@") == 0) {
    name = "";
  }
  JsonValue enum_value = enum_object.at("enumvalue");
  JsonArray enum_value_array;
  if (enum_value.is_array()) {
    enum_value_array = enum_value.to_array();
  } else {
    enum_value_array.append(enum_value);
  }

  String result;

  if (m_is_brief) {
    String id = enum_object.at("@id").to_string();
    if (id.is_empty()) {
      result = "**enum " + name + "**\n";
    } else {
      result = "**enum [" + name + "](#" + id + "]**\n";
    }
    for (u32 i = 0; i < enum_value_array.count(); i++) {
      JsonObject enum_value_object = enum_value_array.at(i).to_object();
      result += "- `" + enum_value_object.at("name").to_string() + "` ";
      result += enum_value_object.at("briefdescription").to_string();
    }
    result += "\n";
  }
  return result;
#endif
  return String();
}

String Doxygen2Markdown::memberdef_to_markdown(const ConvertOptions &options) {
  String result;

  String kind = get_kind();
  bool is_function = kind == "function";

  if (options.is_brief()) {
    result += "- [" + m_node.get_string("name") + "()](" + ")\n";
  } else {
    result += "## ***" + m_node.get_string("name") + (is_function ? "()" : "")
              + "***\n\n";

    String argsstring = m_node.get_string("argsstring");
    if (argsstring == "null") {
      argsstring = String();
    }

    bool is_zero_args = argsstring.find("()") != String::npos;

    argsstring.replace(String::ToErase(", "), String::ToInsert(",\n  "));

    if (is_zero_args == false) {
      argsstring.replace(String::ToErase("("), String::ToInsert("(\n  "));
      argsstring.replace(String::ToErase(")"), String::ToInsert("\n  )"));
    }

    result += "```c++\n" + m_node.get_string("definition")
              + (is_function ? (!is_zero_args ? argsstring : "()") : "")
              + "\n```\n";

    if (is_function && !is_zero_args) {

      result += "\n**Parameters**\n\n";

      for (u32 i = 0; i < m_node.count(); i++) {
        if (m_node.get_key(i) == "param") {
          result
            += Doxygen2Markdown("param", m_node.get_array(i)).convert(options);
        }
      }
    }

    result += "\n";

    result += Doxygen2Markdown(
                "detaileddescription",
                m_node.get_array("detaileddescription"))
                .convert(options);
    result += "\n";
  }

  return result;
}

String Doxygen2Markdown::simplesect_to_markdown(const ConvertOptions &options) {
  String result;

  result = "\n **" + get_kind() + "**: ";
  result += Doxygen2Markdown("para", m_node.get_array("para")).convert(options);

  return result;
}

String Doxygen2Markdown::param_to_markdown(const ConvertOptions &options) {
  String result;

  result += "- ";

  if (m_node.get_string("type") == "[array]") {
    result
      += Doxygen2Markdown("type", m_node.get_array("type")).convert(options);
  } else {
    result += m_node.get_string("type");
  }

  result += " **" + m_node.get_string("declname") + "** ";

  String default_value = m_node.get_string("defval");

  if (default_value.is_empty() == false) {
    result += " = " + default_value + " ";
  }

  String brief_description = m_node.get_string("briefdescription");
  if (brief_description.is_empty() == false) {
    result += ": " + brief_description;
  }
  result += "\n";

  return result;
}

String Doxygen2Markdown::variable_to_markdown(const ConvertOptions &options) {
  String result;
  return result;
}

String
Doxygen2Markdown::compounddef_to_markdown(const ConvertOptions &options) {
  PRINTER_TRACE(printer(), "Convert compound def");
  String result;
  String kind = get_kind();

  result = "# " + kind + " " + m_node.get_string("compoundname") + "\n";

  Node base_compound = Node(m_node.get_array("basecompoundref"));
  if (base_compound.is_valid()) {
    result += "Base : "
              + Doxygen2Markdown("ref", m_node.get_array("basecompoundref"))
                  .convert(options)
              + "\n\n";
  }

  result += "```c++\n";
  result += kind + " " + m_node.get_string("compoundname") + "\n";
  if (base_compound.is_valid()) {
    result
      += "  : "
         + (base_compound.get_string("@virt") != "non-virtual" ? String("virtual ") : String())
         + base_compound.get_string("@prot") + " "
         + base_compound.get_string("#text") + "\n";
  }
  result += "```\n\n";

  if (kind == "namespace") {
    result += "## class members\n\n";
    for (u32 i = 0; i < m_node.count(); i++) {
      if (m_node.get_key(i) == "innerclass") {
        Node innerclass = Node(m_node.get_array(i));
        result += "- " + innerclass.get_string("@prot") + " ["
                  + innerclass.get_string("#text") + "]("
                  + innerclass.get_string("@refid") + ")\n";
      }
    }
    result += "\n";

  } else {

    // check if kind is visible
#if 0
  bool is_visible =
    (options.is_private_type_visible() && kind == "private-type")
    || (options.is_public_type_visible() && kind == "public-type")
    || (options.is_public_func_visible() && kind == "public-func")
    || (options.is_public_static_func_visible() && kind == "public-static-func")
    || (options.is_private_static_func_visible() && kind == "private-static-func")
    || (options.is_private_func_visible() && kind == "private-func")
    || (options.is_private_attrib_visible() && kind == "private-attrib")
    ;
#endif

    if (options.is_public_static_func_visible()) {
      for (u32 i = 0; i < m_node.count(); i++) {
        if (m_node.get_key(i) == "sectiondef") {
          if (
            Node(m_node.get_array(i)).get_string("@kind")
            == "public-static-func") {
            result += "\n## public static members\n\n";
            result += Doxygen2Markdown("sectiondef", m_node.get_array(i))
                        .convert(options);
          }
        }
      }
    }

    if (options.is_public_func_visible()) {
      for (u32 i = 0; i < m_node.count(); i++) {
        if (m_node.get_key(i) == "sectiondef") {
          if (Node(m_node.get_array(i)).get_string("@kind") == "public-func") {
            result += "\n## public members\n\n";
            result += Doxygen2Markdown("sectiondef", m_node.get_array(i))
                        .convert(options);
          }
        }
      }
    }
  }

  return result;
}

String Doxygen2Markdown::struct_to_markdown(const ConvertOptions &options) {
  String result;
  return result;
}

// primitive doxygen types
String Doxygen2Markdown::para_to_markdown(const ConvertOptions &options) {
  String result;
  result = "\n";
  result += children_to_markdown(options);
  result += "\n";
  return result;
}

String Doxygen2Markdown::small_to_markdown(const ConvertOptions &options) {
  String result;
  result = "<small>";
  result += children_to_markdown(options);
  result += "</small>";
  return result;
}

String Doxygen2Markdown::bold_to_markdown(const ConvertOptions &options) {
  String result;
  result = "**";
  result += children_to_markdown(options);
  result += "**";
  return result;
}

String Doxygen2Markdown::ulink_to_markdown(const ConvertOptions &options) {
  String result;

  result
    = "["
      + Doxygen2Markdown("#text", m_node.get_value("#text")).convert(options)
      + "]("
      + Doxygen2Markdown("@url", m_node.get_value("@url")).convert(options)
      + ")";

  return result;
}

String Doxygen2Markdown::title_to_markdown(const ConvertOptions &options) {
  String result;
  result = "# ";
  result += children_to_markdown(options);
  result += "\n";
  return result;
}

String
Doxygen2Markdown::itemizedlist_to_markdown(const ConvertOptions &options) {
  String result;
  return result;
}

String Doxygen2Markdown::listitem_to_markdown(const ConvertOptions &options) {
  String result;
  return result;
}

String
Doxygen2Markdown::programlisting_to_markdown(const ConvertOptions &options) {
  String result = "\n```c++\n";
  result += children_to_markdown(options);
  result += "```\n";
  return result;
}

String Doxygen2Markdown::highlight_to_markdown(const ConvertOptions &options) {
  String result;
  for (u32 i = 0; i < m_node.count(); i++) {
    if (m_node.get_key(i) == "#text") {
      result += m_node.at(i).to_object().at("#text").to_string() + " ";
    }
  }
  if (result.length()) {
    result.pop_back();
  }
  result += "\n";
  return result;
}

String Doxygen2Markdown::ref_to_markdown(const ConvertOptions &options) {

  return "[" + m_node.get_string("#text") + "](" + m_node.get_string("@refid")
         + ".md) ";
}
