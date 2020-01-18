#include "Doxygen2Markdown.hpp"


String Doxygen2Markdown::to_markdown(){

	if( m_key == "memberdef" ){ return memberdef_to_markdown(); }
	if( m_key == "para" ){ return para_to_markdown(); }
	if( m_key == "detaileddescription" ){ return para_to_markdown(); }
	if( m_key == "highlight" ){ return highlight_to_markdown(); }
	if( m_key == "programlisting" ){ return programlisting_to_markdown(); }
	if( m_key == "codeline" ){ return codeline_to_markdown(); }
	if( m_key == "small" ){ return small_to_markdown(); }
	if( m_key == "bold" ){ return bold_to_markdown(); }
	if( m_key == "param" ){ return param_to_markdown(); }

	if( m_key == "enum" ){ return enum_to_markdown(); }
	if( m_key == "function" ){ return function_to_markdown(); }
	if( m_key == "variable" ){ return variable_to_markdown(); }
	if( m_key == "class" ){ return class_to_markdown(); }
	if( m_key == "struct" ){ return struct_to_markdown(); }

	return String();
}

String Doxygen2Markdown::children_to_markdown(){
	String result;
	if( m_value.is_object() ){
		Vector<String> keys = m_value.to_object().keys();
		if( keys.at(0) == "ref" || keys.at(0) == "#text" ){
			result += ref_text_to_markdown(
						m_value.to_object().at("ref"),
						m_value.to_object().at("#text")
						);
		} else {
			for(const auto & key: keys){
				result += Doxygen2Markdown(key, m_value.to_object().at(key), m_is_brief).to_markdown();
			}
		}
	} else if( m_value.is_array() ){
		for(u32 i=0; i < m_value.to_array().count(); i++){
			printf("process array %d of %d (%s)\n", i+1, m_value.to_array().count(), m_key.cstring());
			result += Doxygen2Markdown(m_key, m_value.to_array().at(i), m_is_brief).to_markdown();
		}
	} else {
		result += m_value.to_string();
	}
	return result;
}

String Doxygen2Markdown::memberdef_to_markdown(){
	String result;

	if( m_value.is_array() ){
		printf("check memberdef children\n");
		result += children_to_markdown();
	} else {
		String kind = m_value.to_object().at("@kind").to_string();
		printf("convert memberdef %s\n", kind.cstring());
		result += Doxygen2Markdown(kind, m_value, m_is_brief).to_markdown();
		result += "\n";
	}
	return result;
}

String Doxygen2Markdown::enum_to_markdown(){
	JsonObject enum_object = m_value.to_object();
	String name = enum_object.at("name").to_string();
	if( name.find("@") == 0 ){ name = ""; }
	JsonValue enum_value = enum_object.at("enumvalue");
	JsonArray enum_value_array;
	if( enum_value.is_array() ){
		enum_value_array = enum_value.to_array();
	} else {
		enum_value_array.append( enum_value );
	}

	String result;

	if( m_is_brief ){
		String id = enum_object.at("@id").to_string();
		if( id.is_empty() ){
			result = "**enum " + name + "**\n";
		} else {
			result = "**enum [" + name + "](#" + id + "]**\n";
		}
		for(u32 i=0; i < enum_value_array.count(); i++){
			JsonObject enum_value_object = enum_value_array.at(i).to_object();
			result += "- `" + enum_value_object.at("name").to_string() + "` ";
			result += enum_value_object.at("briefdescription").to_string();
		}
		result += "\n";
	}
	return result;
}

String Doxygen2Markdown::function_to_markdown(){
	JsonObject object = m_value.to_object();
	String result;
	if( m_is_brief ){
		result = "- [" + object.at("name").to_string() + "()](" + ")\n";
	} else {
		result = "***" +
				object.at("name").to_string() +
				"()" +
				"***\n\n";

		String argsstring = object.at("argsstring").to_string();
		if( argsstring.length() > 3 ){
			argsstring = argsstring.create_sub_string(
						String::Position(1),
						String::Length(argsstring.length()-2)
						);
		}

		argsstring.replace(
					String::ToErase(", "),
					String::ToInsert(",\n  ")
					);

		result += "```\n" +
				object.at("definition").to_string() + "(\n  " +
				argsstring +
				"\n);\n```\n";

		result += Doxygen2Markdown("detaileddescription", object.at("detaileddescription"), m_is_brief).to_markdown();
		result += "\n";
		result += "***Parameters***\n";
		result += Doxygen2Markdown("param", object.at("param"), m_is_brief).to_markdown();
	}

	return result;
}

String Doxygen2Markdown::param_to_markdown(){
	String result;

	if( m_value.is_array() ){
		for(u32 i=0; i < m_value.to_array().count(); i++){
			result += Doxygen2Markdown("param", m_value.to_array().at(i), m_is_brief).to_markdown();
		}
	} else {
		JsonObject object = m_value.to_object();
		result += "- " +
				object.at("declname").to_string() +
				" (" +
				object.at("type").to_string();
		String default_value = object.at("defval").to_string();
		if( default_value.is_empty() == false ){
			result += " = " + default_value;
		}
		result += ")";
		String brief_description = object.at("briefdescription").to_string();
		if( brief_description.is_empty() == false ){
			result += ": " + brief_description;
		}
		result += "\n";
	}


	return result;
}

String Doxygen2Markdown::variable_to_markdown(){
	String result;
	return result;
}

String Doxygen2Markdown::class_to_markdown(){
	String result;

	printf("Convert class\n");

	return result;
}

String Doxygen2Markdown::struct_to_markdown(){
	String result;
	return result;
}

//primitive doxygen types
String Doxygen2Markdown::para_to_markdown(){
	String result;
	JsonValue ref = m_value.to_object().at("ref");
	JsonValue text = m_value.to_object().at("#text");
	result = "\n";
	result += children_to_markdown();
	result += "\n";
	return result;
}

String Doxygen2Markdown::ref_text_to_markdown(
		JsonValue ref,
		JsonValue text
		){
	JsonArray ref_array;

	if( !text.is_valid() && ref.is_valid() == true ){
		return Doxygen2Markdown("ref", ref, m_is_brief).to_markdown();
	}

	if( text.is_valid() && !ref.is_valid() ){
		return text.to_string();
	}

	if( ref.is_array() ){
		ref_array = ref.to_array();
	} else {
		ref_array.append(ref);
	}

	String result;
	for(u32 i=0; i < text.to_array().count(); i++){
		result += text.to_array().at(i).to_string();
		if( ref_array.count() > i ){
			result += Doxygen2Markdown("ref", ref_array.at(i), m_is_brief).to_markdown();
		}
	}
	return result;
}

String Doxygen2Markdown::ref_to_markdown(){
	String result;
	result = "[" +
			Doxygen2Markdown("#text", m_value.to_object().at("#text")).to_markdown() +
			"](#" +
			//need to parse all files and generate a reference map
			")";
	return result;
}

String Doxygen2Markdown::small_to_markdown(){
	String result;
	result = "<small>";
	result += children_to_markdown();
	result += "</small>";
	return result;
}

String Doxygen2Markdown::bold_to_markdown(){
	String result;
	result = "**";
	result += children_to_markdown();
	result += "**";
	return result;
}

String Doxygen2Markdown::ulink_to_markdown(){
	String result;

	result = "[" +
			Doxygen2Markdown("#text", m_value.to_object().at("#text")).to_markdown() +
			"](" +
			Doxygen2Markdown("@url", m_value.to_object().at("@url")).to_markdown() +
			")";

	return result;
}

String Doxygen2Markdown::text_to_markdown(){
	return children_to_markdown();
}


String Doxygen2Markdown::title_to_markdown(){
	String result;
	result = "# ";
	result += children_to_markdown();
	result += "\n";
	return result;
}

String Doxygen2Markdown::itemizedlist_to_markdown(){
	String result;
	return result;
}

String Doxygen2Markdown::listitem_to_markdown(){
	String result;
	return result;
}

String Doxygen2Markdown::programlisting_to_markdown(){
	String result = "```\n";
	result += children_to_markdown();
	result += "```\n";
	return result;
}

String Doxygen2Markdown::codeline_to_markdown(){
	JsonArray codeline_array = m_value.to_array();
	String result;
	for(u32 i=0; i < codeline_array.count(); i++){
		result += Doxygen2Markdown("highlight", codeline_array.at(i).to_object().at("highlight")).to_markdown();
		result += "\n";
	}
	return result;
}

String Doxygen2Markdown::highlight_to_markdown(){
	JsonValue text = m_value.to_object().at("#text");
	String result;
	if( text.is_array() ){
		for(u32 i=0; i < text.to_array().count(); i++){
			result += text.to_array().at(i).to_string();
		}
	} else {
		result = text.to_string();
	}
	return result;
}


