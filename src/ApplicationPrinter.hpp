#ifndef APPLICATIONPRINTER_HPP
#define APPLICATIONPRINTER_HPP

#include <sapi/sys/YamlPrinter.hpp>

class ApplicationPrinter {
public:
	static sys::YamlPrinter & printer(){ return m_printer; }

private:
	static sys::YamlPrinter m_printer;

};

#endif // APPLICATIONPRINTER_HPP
