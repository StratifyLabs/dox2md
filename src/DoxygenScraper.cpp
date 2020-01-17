#include <sapi/fs.hpp>

#include "DoxygenScraper.hpp"

DoxygenScraper::DoxygenScraper()
{

}

void DoxygenScraper::generate_code(const String & directory_path){

	auto directory_list = Dir::read_list(directory_path);

	for(const auto & file: directory_list){
		JsonObject doxygen = JsonDocument().load(
					fs::File::Path(directory_path + "/" + file)
					).to_object();
		printf("processing %s\n", file.cstring());
		generate_code_object("doxygen", doxygen.at("doxygen").to_object());
	}


	for(const auto & object: m_objects){
		String class_name =  DoxygenScraperObject::translate_name(object.name(), true);
		printf("class %s {\n", class_name.cstring());
		printf("public:\n");
		printf("%s(){\n%s\n}\n\n", class_name.cstring(), object.constructors().cstring());
		printf("//Accessors:\n%s\n", object.accessors().cstring());
		printf("private:\n%s\n", object.members().cstring());
		printf("};\n");
	}

}


int DoxygenScraper::generate_code_object(const String& object_key, const JsonObject & object){

	size_t offset = m_objects.find(object_key);
	if( offset == m_objects.count() ){
		//printf("Add object %s %d\n", object_key.cstring(), m_objects.count());
		m_objects.push_back(
					DoxygenScraperObject(object_key)
					);
		//printf("offset for new object is %d == %d\n", offset, m_objects.count());
	} else {
		//printf("refer to object at %d\n", offset);
	}

	DoxygenScraperObject & current_object = m_objects.at(offset);
	Vector<String> keys = object.keys();

	for(const auto & key: keys){
		JsonValue value = object.at(key);

		if( value.is_string() ){
			current_object.add_key(key, "String");
		} else if( value.is_object() ){
			printf("Add key %s\n", key.cstring());
#if 0
			current_object.add_key(
						key,
						"Object"
						);
#endif
			printf("done\n");
			generate_code_object(key, value.to_object());
		} else if( value.is_array() ){
			printf("%s is an array %d (%d)\n", key.cstring(), value.to_array().count(), current_object.keys().count());
			for(u32 i=0; i < value.to_array().count(); i++){
				current_object.add_key(
							key,
							"Array"
							);
				printf("Added key as array %d\n", current_object.keys().count());
				if( value.to_array().at(i).is_object() ){
					generate_code_object(
								key, value.to_array().at(i).to_object()
								);
				}
			}
		}
	}

	return 0;
}

String DoxygenScraperObject::translate_name(const String& name, bool is_class){
	if( name == "compoundname"){ return is_class ? "CompoundName" : "compound_name"; }
	if( name == "compounddef"){ return is_class ? "CompoundDefinition" : "compound_definition"; }
	if( name == "basecompoundname"){ return is_class ? "BaseCompoundName" : "base_compound_name"; }
	if( name == "basecompoundref"){ return is_class ? "BaseCompoundReference" : "base_compound_reference"; }
	if( name == "includes"){ return is_class ? "Includes" : "includes"; }
	if( name == "sectiondef"){ return is_class ? "SectionDefinition" : "section_definition"; }
	if( name == "memberdef"){ return is_class ? "MemberDefinition" : "member_definition"; }
	if( name == "declname"){ return is_class ? "DeclarationName" : "declartion_name"; }
	if( name == "param"){ return is_class ? "Paramter" : "parameter"; }
	if( name == "type"){ return is_class ? "Type" : "type"; }
	if( name == "parameterlist"){ return is_class ? "ParameterList" : "parameter_list"; }
	if( name == "parameteritem"){ return is_class ? "ParameterItem" : "parameter_item"; }
	if( name == "parametername"){ return is_class ? "ParameterName" : "parameter_name"; }
	if( name == "parameterdescription"){ return is_class ? "ParameterDescription" : "parameter_description"; }
	if( name == "briefdescription"){ return is_class ? "BriefDescription" : "brief_description"; }
	if( name == "inheritancegraph"){ return is_class ? "InheritanceGraph" : "inheritance_graph"; }
	if( name == "node"){ return is_class ? "Node" : "node"; }
	if( name == "link"){ return is_class ? "Link" : "link"; }
	if( name == "childnode"){ return is_class ? "ChildNode" : "child_node"; }
	if( name == "collaborationgraph"){ return is_class ? "CollaborationGraph" : "collaboration_graph"; }
	if( name == "listofallmembers"){ return is_class ? "ListOfAllMembers" : "list_of_all_members"; }
	if( name == "innerclass"){ return is_class ? "InnerClass" : "inner_class"; }
	if( name == "reimplement"){ return is_class ? "Reimplement" : "reimplement"; }
	if( name == "programlisting"){ return is_class ? "ProgramListing" : "program_listing"; }
	if( name == "highlight"){ return is_class ? "highlight" : "highlight"; }
	if( name == "templateparamlist"){ return is_class ? "TemplateParamList" : "template_parameter_list"; }
	if( name == "derivedcompoundref"){ return is_class ? "DerivedCompoundRef" : "derived_compound_reference"; }
	if( name == "reimplementedby"){ return is_class ? "ReimplementedBy" : "reimplemented_by"; }
	if( name == "itemizedlist"){ return is_class ? "ItemizedList" : "itemized_list"; }
	if( name == "listitem"){ return is_class ? "ListItem" : "list_item"; }
	if( name == "ulink"){ return is_class ? "ulink" : "ulink"; }
	if( name == "small"){ return is_class ? "small" : "small"; }
	if( name == "sect1"){ return is_class ? "SectionOne" : "section_one"; }
	if( name == "xrefsect"){ return is_class ? "CrossReferenceSection" : "cross_reference_sect"; }
	if( name == "xrefdescription"){ return is_class ? "CrossReferenceDescription" : "cross_reference_description"; }
	if( name == "blockquote"){ return is_class ? "BlockQuote" : "block_quote"; }
	if( name == "heading"){ return is_class ? "Heading" : "heading"; }
	if( name == "doxygen"){ return is_class ? "Doxygen" : "doxygen"; }
	if( name == "para"){ return is_class ? "Paragraph" : "paragraph"; }
	if( name == "edgelabel"){ return is_class ? "EdgeLabel" : "edge_label"; }
	if( name == "defname"){ return is_class ? "DefinitionName" : "definition_name"; }
	if( name == "defval"){ return is_class ? "DefinitionValue" : "definition_value"; }
	if( name == "@kindref"){ return is_class ? "EdgeLabel" : "edge_label"; }
	if( name == "computeroutput"){ return is_class ? "ComputerOutput" : "computer_output"; }
	if( name == "@bodyfile"){ return is_class ? "BodyFile" : "body_file"; }
	if( name == "enumvalue"){ return is_class ? "EnumerationValue" : "enumeration_value"; }
	if( name == "@bodyfile"){ return is_class ? "BodyFile" : "body_file"; }
	if( name == "@bodyfile"){ return is_class ? "BodyFile" : "body_file"; }
	return name;
}

