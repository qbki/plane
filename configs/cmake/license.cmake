function (append_license_title result_string lib_file_path)
  file(READ ${lib_file_path} file_content)
  string(JSON vcpkg_name GET ${file_content} name)
  string(REGEX REPLACE "^(.+):.*$" "\\1" lib_name ${vcpkg_name})
  string(REGEX REPLACE "^.+@([0-9\.\-]+).*$" "\\1" lib_version ${vcpkg_name})
  string(APPEND ${result_string} "***** ${lib_name} ${lib_version} *****\n\n")
  return(PROPAGATE ${result_string})
endfunction()

function(append_license_file result_string copyright_file_path)
  set(file_content "")
  file(READ ${copyright_file_path} file_content)
  string(APPEND ${result_string} "${file_content}\n")
  return(PROPAGATE ${result_string})
endfunction()

function(generate_licenses_file)
  set(licenses "")
  string(APPEND licenses
    "This file was generated semiautomatically and it\n"
    "might contain errors or lack information. Please\n"
    "check the official sites of these libraries to get\n"
    "current legal information.\n\n"
    "This software is distributed with further described\n"
    "libraries but is not limited. If your library\n"
    "or/and license is missing in the file, please\n"
    "address it to the maintainers of Plane Game Engine.\n\n")

  file(GLOB lib_dirs "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/share/*")
  foreach (lib_dir ${lib_dirs})
    set(vcpkg_meta "${lib_dir}/vcpkg.spdx.json")
    set(vcpkg_copyright "${lib_dir}/copyright")
    if(EXISTS ${vcpkg_meta} AND EXISTS ${vcpkg_copyright})
      append_license_title(licenses ${vcpkg_meta})
      append_license_file(licenses ${vcpkg_copyright})
    endif()
  endforeach()

  file(WRITE ${THIRD_PARTY_LICENSES_FILE} ${licenses})
endfunction()
