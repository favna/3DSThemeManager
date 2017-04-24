// Themely - home menu manager
// Copyright (c) 2017 Erman SAYIN

#pragma once

template <typename T>
std::string to_string(T value) {
	std::ostringstream os;
	os << value;
	return os.str();
}
