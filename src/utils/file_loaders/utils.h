#include <string>

#include "src/utils/result.h"

Result<const std::string>
does_file_exist(const std::string& file_path);
