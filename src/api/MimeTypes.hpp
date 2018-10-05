#ifndef SRC_API_MIMETYPES_HPP
#define SRC_API_MIMETYPES_HPP

#pragma once

#include <Config.hpp>

#include <string>

SAMP_API_BEGIN_NS

/// Convert a file extension into a MIME type.
std::string extension_to_type(const std::string& extension);

SAMP_API_END_NS

#endif
