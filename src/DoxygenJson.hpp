#ifndef DOXYGENJSON_HPP
#define DOXYGENJSON_HPP

#include "ApplicationPrinter.hpp"
#include <sapi/var.hpp>

class DoxygenJson : public ApplicationPrinter {
public:
	DoxygenJson(const String & key, JsonObject object){
		m_key = key;
		m_object = object;
	}

	String to_markdown();

private:

	//primitive doxygen types
	String para_to_markdown();
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


	String m_key;
	JsonObject m_object;

};


#endif // DOXYGENJSON_HPP
