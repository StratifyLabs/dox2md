#include "Settings.hpp"
#include <sapi/var.hpp>


int Settings::initialize(){

	printer().debug(
				"load settings from " + settings_file_name()
				);

	JsonDocument document;
	m_settings = document.load(
				fs::File::Path(settings_file_name())
				).to_object();

	if( m_settings.is_empty() ){
		printer().debug(
					"error is " +	document.error().text()
							 );
		printer().error(
					"failed to load settings file " + settings_file_name()
					);
		return -1;
	}

	printer() << m_settings;

	return 0;
}
