///////////////
//  imports  //
///////////////
#include "character_components.hpp"

Class::CLASS string_to_class(std::string str)
{
	static const std::map<std::string, Class::CLASS> string_to_class_map = {
		{"WARRIOR", Class::WARRIOR},
		{"MAGE", Class::MAGE},
		{"RANGER", Class::RANGER},
	};

	auto it = string_to_class_map.find(str);

	if (it != string_to_class_map.end()) {
		return it->second;
	}

	// Handle the case where the string doesn't map to any enum value
	throw std::invalid_argument("Invalid enum string: " + str);
}