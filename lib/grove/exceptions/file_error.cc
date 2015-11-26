/*
** Copyright 2014-2015 Robert Fratto. See the LICENSE.txt file at the top-level
** directory of this distribution.
**
** Licensed under the MIT license <http://opensource.org/licenses/MIT>. This file
** may not be copied, modified, or distributed except according to those terms.
*/

#include <grove/exceptions/file_error.h>
#include <grove/Module.h>

#include <util/assertions.h>

const char* file_error::what() const noexcept 
{
	std::stringstream ss;
	ss << "fatal: could not open file " <<
		m_module->getFile() << " for reading.";
	
	return ss.str().c_str();
}

file_error::file_error(Module* mod)
{
	assertExists(mod, "mod cannot be nullptr");
	m_module = mod;
}