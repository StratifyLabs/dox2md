#ifndef DOXYGEN2MARKDOWN_HPP
#define DOXYGEN2MARKDOWN_HPP

#include <sapi/var.hpp>
#include "ApplicationPrinter.hpp"

class Doxygen2Markdown : public ApplicationPrinter {
public:
	Doxygen2Markdown(const String& key, JsonValue value, bool is_brief = true){
		m_key = key;
		m_value = value;
		m_is_brief = is_brief;
	}

	Doxygen2Markdown& set_brief(bool value = true){
		m_is_brief = value;
		return *this;
	}

	String to_markdown();

private:
	String m_key;
	JsonValue m_value;
	bool m_is_brief;

	//primitive doxygen types
	String memberdef_to_markdown();


	String enum_to_markdown();
	String function_to_markdown();
	String variable_to_markdown();
	String class_to_markdown();
	String struct_to_markdown();

	String para_to_markdown();
	String param_to_markdown();
	String small_to_markdown();
	String bold_to_markdown();
	String ulink_to_markdown();
	String ref_to_markdown();
	String text_to_markdown();
	String title_to_markdown();
	String itemizedlist_to_markdown();
	String listitem_to_markdown();
	String programlisting_to_markdown();
	String codeline_to_markdown();
	String highlight_to_markdown();

	String children_to_markdown();
	String hash_text(JsonValue value);

	String ref_text_to_markdown(
			JsonValue ref,
			JsonValue text
			);
};

#endif // DOXYGEN2MARKDOWN_HPP
