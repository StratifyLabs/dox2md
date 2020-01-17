#ifndef DOXYGENSCRAPER_HPP
#define DOXYGENSCRAPER_HPP

#include <sapi/var.hpp>
#include "ApplicationPrinter.hpp"


class DoxygenScraperObject {
public:

	DoxygenScraperObject(const String& name){
		m_name = name;
	}

	bool operator == (const DoxygenScraperObject & a) const {
		return m_name == a.m_name;
	}

	void add_key(const String& key, const String& kind){
		printf("add key to %d\n", m_keys.count());
		for(size_t i = 0; i < m_keys.count(); i++){
			if( m_keys.at(i).at(0) == key ){
				//key already exists
				return;
			}
		}
		Array<String, 2> new_key;
		new_key.at(0) = key;
		new_key.at(1) = kind;
		m_keys.push_back(new_key);
	}

	const String& name() const {
		return m_name;
	}

	String constructors() const {
		String result;
		for(auto const & key: m_keys){
			String output_key = to_output_key(key.at(0));
			result += "    m_" + translate_name(output_key) + " = object.at(\"" + key.at(0) + "\").to_string();\n";
		}
		return result;
	}


	String accessors() const {
		String result;
		for(auto const & key: m_keys){
			String output_key = to_output_key(key.at(0));
			result += "  const String& " + translate_name(output_key) + "() const { return m_" + translate_name(output_key) + "; }\n";
		}
		return result;
	}


	String members() const {
		String result;
		for(auto const & key: m_keys){
			String output_key = to_output_key(key.at(0));
			result += "  String m_" + translate_name(output_key) + ";\n";
		}
		return result;
	}

	static String translate_name(const String& name, bool is_class = false);

	const Vector<Array<String, 2>>& keys() const {
		return m_keys;
	}


private:
	String to_output_key(const String& key) const {
		String result = key;
		result.replace(
					String::ToErase("@"),
					String::ToInsert("a_")
					);

		result.replace(
					String::ToErase("#"),
					String::ToInsert("h_")
					);

		result.replace(
					String::ToErase(":"),
					String::ToInsert("_")
					);
		return result;
	}
	String m_name;
	Vector<Array<String, 2>> m_keys;
};

class DoxygenScraper : public ApplicationPrinter {
public:
	DoxygenScraper();
	void generate_code(const String & file);


private:

	int generate_code_object(const String& object_key, const JsonObject & object);
	Vector<DoxygenScraperObject> m_objects;

};

#endif // DOXYGENSCRAPER_HPP
