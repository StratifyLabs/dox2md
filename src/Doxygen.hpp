#ifndef DOXYGEN_HPP
#define DOXYGEN_HPP

#include <sapi/var.hpp>

#include "ApplicationPrinter.hpp"

class DoxygenObject {
public:
	DoxygenObject(JsonObject object){
		m_object = object;
		m_id = m_object.at("@id").to_string();
		m_language = m_object.at("@language").to_string();
		m_protection = m_object.at("@prot").to_string();

		m_reference_id = m_object.at("@refid").to_string();
		m_text = m_object.at("#text").to_string();
		m_local = m_object.at("@local").to_string();
		m_virtual = m_object.at("@virt").to_string();
		m_name = m_object.at("name").to_string();
		m_brief_description = m_object.at("briefdescription").to_string();
		m_detailed_description = m_object.at("detaileddescription").to_string();
	}

protected:
	JsonObject m_object;

	String m_id;
	String m_reference_id;
	String m_kind;
	String m_language;
	String m_protection;
	String m_text;
	String m_local;
	String m_virtual;
	String m_name;
	String m_brief_description;
	String m_detailed_description;

};

class DoxygenLocation {
public:
	DoxygenLocation(JsonObject object){
		m_object = object;
		m_file = m_object.at("@file").to_string();
		m_line = m_object.at("@line").to_string();
		m_column = m_object.at("@column").to_string();
	}

	const String& file() const { return m_file; }
	int line() const { return m_line.to_integer(); }
	int column() const { return m_column.to_integer(); }

protected:
	JsonObject m_object;

	String m_file;
	String m_line;
	String m_column;
};

class DoxygenParagraph {
public:

private:

};

class DoxygenMember: public DoxygenObject {
	DoxygenMember(JsonObject object) :
		DoxygenObject(object),
		m_location(object.at("location").to_object()){
		m_const = m_object.at("@const").to_string();
		m_explicit = m_object.at("@explicit").to_string();
		m_inline = m_object.at("@inline").to_string();
		m_type = m_object.at("type").to_string();
		m_definition = m_object.at("definition").to_string();
		m_argument_string = m_object.at("argsstring").to_string();
		m_inbody_description = m_object.at("inbodydescription").to_string();
	}

private:
	String m_const;
	String m_explicit;
	String m_inline;
	String m_type;
	String m_definition;
	String m_argument_string;
	String m_inbody_description;
	DoxygenLocation m_location;
};

class DoxygenSection : public DoxygenObject {
public:
	DoxygenSection(JsonObject object) : DoxygenObject(object){

	}
};

class DoxygenCompound : public DoxygenObject {
public:
	DoxygenCompound(JsonObject object) :
		DoxygenObject(object),
		m_base_compound_reference(object.at("basecompoundref").to_object()),
		m_derived_compound_reference(object.at("derivedcompoundref").to_object()),
		m_includes(object.at("includes").to_object())
		{

	}

protected:
	DoxygenObject m_base_compound_reference;
	DoxygenObject m_derived_compound_reference;
	DoxygenObject m_includes;
	Vector<DoxygenSection> m_sections;

};



class DoxygenInnerClass {
public:
	DoxygenInnerClass(JsonObject object){
		m_object = object;
		m_text = m_object.at("#text").to_string();
		m_reference_id = m_object.at("#text").to_string();
		m_protection = m_object.at("#text").to_string();
	}

	const String& text() const { return m_text; }
	const String& reference_id() const { return m_reference_id; }
	const String& protection() const { return m_protection; }

private:
	JsonObject m_object;
	String m_text;
	String m_reference_id;
	String m_protection;

};



class Doxygen : public ApplicationPrinter {
public:

	Doxygen(const String & file_name);

	static bool is_file_name_match_for_pages(
			const String & file_name,
			const Vector<String> & pages){

		if( pages.find(
					lookup_file_category(file_name)
					) < pages.count() ){
			return true;
		}

		return false;

	}

	static String lookup_file_category(const String & file_name){
		if( (file_name.find("8hpp.xml") != String::npos) ||
				(file_name.find("8h.xml") != String::npos)
				){
			return "header";
		}

		if( (file_name.find("8c.xml") != String::npos) ||
				(file_name.find("8cpp.xml") != String::npos) ){
			return "code";
		}

		if( file_name.find("group") == 0 ){
			return "group";
		}

		if( file_name.find("class") == 0 ){
			return "class";
		}

		if( file_name.find("index") == 0 ){
			return "index";
		}

		if( file_name.find("todo") == 0 ){
			return "todo";
		}

		if( file_name.find("namespace") == 0 ){
			return "namespace";
		}

		if( file_name.find("struct") == 0 ){
			return "struct";
		}

		return "";

	}


	String kind() const {
		return m_compound_definition_object.at("@kind").to_string();
	}

	String id() const {
		return m_compound_definition_object.at("@id").to_string();
	}

	String language() const {
		return m_compound_definition_object.at("@language").to_string();
	}

	String compound_name() const {
		return m_compound_definition_object.at("compoundname").to_string();
	}

	String brief_description() const {
		return m_compound_definition_object.at("briefdescription").to_string();
	}


	Vector<DoxygenInnerClass> inner_classes() const {
		Vector<DoxygenInnerClass> result;
		if( kind() != "namespace" ){
			return result;
		}

		JsonArray inner_class_array =
				m_compound_definition_object.at("innerclass").to_array();
		//for(u32 i=0; i < inner_class_array.count(); i++){
		//	result.push_back(DoxygenInnerClass(inner_class_array.at(i)));
		//}
		return result;
	}


private:


	JsonObject m_object;
	JsonObject m_compound_definition_object;


};

#endif // DOXYGEN_HPP
