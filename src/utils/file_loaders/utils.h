#include <string>

void
throw_error_if_file_not_found(const std::string& file_name);

void
throw_error_if_file_is_not_readable(const std::istream& stream,
                                    const std::string& file_name);
