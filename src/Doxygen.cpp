#include "Doxygen.hpp"

#include <sapi/fs.hpp>
#include <sapi/sys.hpp>

Doxygen::Doxygen(const String &file_name) {
  m_object = JsonDocument()
               .load(fs::File::Path(file_name))
               .to_object()
               .at("doxygen")
               .to_object();

  m_compound_definition_object = m_object.at("compounddef").to_object();
}
