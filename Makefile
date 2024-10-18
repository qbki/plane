SHELL=/bin/bash
CPP_SOURCE_FILES_LIST=$(shell git ls-files | egrep '(\.h|\.cpp|\.ipp)$$')
PROJECT_NAME=plane
BUILD_DIR=$(shell pwd)/build
SRC_DIR=$(shell pwd)/src
CONFIGS_DIR=$(shell pwd)/configs
PACK_DIR=$(BUILD_DIR)/pack
LINUX_BUILD_DIR=$(BUILD_DIR)/x86_64-linux-release
LINUX_BUILD_TESTS_DIR=$(BUILD_DIR)/x86_64-linux-tests
LINUX_BIN_DIR=$(LINUX_BUILD_DIR)/bin
LINUX_DEBUG_BUILD_DIR=$(BUILD_DIR)/x86_64-linux-debug
LINUX_DEBUG_BIN_DIR=$(LINUX_DEBUG_BUILD_DIR)/bin
WASM_BUILD_DIR=$(BUILD_DIR)/wasm32-emscripten-release
WASM_BIN_DIR=$(WASM_BUILD_DIR)/bin
WEB_SHELL_DIR=$(BUILD_DIR)/web-shell


install-tools:
	@sh scripts/install-tools.sh
.PHONY: install-tools


init:
	@cmake --preset x86_64-linux-release
	@cd $(BUILD_DIR) && \
		(ls compile_commands.json 2> /dev/null && \
			echo "A link to compile_commands.json already exists") \
		|| ln -s $(LINUX_BUILD_DIR)/compile_commands.json
.PHONY: init


init-tests:
	@cmake --preset x86_64-linux-tests
.PHONY: init-tests


init-debug:
	@cmake --preset x86_64-linux-debug
.PHONY: init-debug


init-wasm:
	@. ./tools/emsdk/emsdk_env.sh && \
	  cmake --preset wasm32-emscripten-release
	@npm ci
.PHONY: init-wasm


build:
	@cmake --build --preset linux
.PHONY: build


build-tests:
	@cmake --build --preset tests
.PHONY: build-tests


build-wasm:
	@. ./tools/emsdk/emsdk_env.sh && \
		cmake --build --preset web
.PHONY: build-wasm


build-web-shell:
	@npm run build
.PHONY: build-web-shell


build-debug:
	@cmake --build --preset linux-debug
.PHONY: build-debug


run: build
	@$(LINUX_BIN_DIR)/$(PROJECT_NAME)
.PHONY: run


run-debug: build-debug
	@gdb $(LINUX_DEBUG_BIN_DIR)/$(PROJECT_NAME)
.PHONY: run-debug


run-wasm: build-wasm build-web-shell
	@cd $(WEB_SHELL_DIR) && \
		ln -snf $(shell pwd)/assets assets && \
		python3 -m http.server -b 127.0.0.1
.PHONY: run-wasm


cppcheck:
	@cppcheck \
		--enable=all \
		--inconclusive \
		--verbose \
		--quiet \
		--inline-suppr \
		-I ./ \
		--suppress=unusedFunction \
		--suppress=missingIncludeSystem \
		./src
.PHONY: cppcheck

iwyu:
	@iwyu_tool -p "$(BUILD_DIR)" "$(SRC_DIR)" \
		 -- -Xiwyu --mapping_file=$(CONFIGS_DIR)/iwyu/mapping.imp \
				-Xiwyu --no_fwd_decls
.PHONY: iwyu


clang-tidy:
	@clang-tidy $(CPP_SOURCE_FILES_LIST) -p $(BUILD_DIR)
.PHONY: clang-tidy


clang-tidy-verify-config: export SYSTEM_VERSION = `\
	echo \`clang-tidy --version\` | grep -Po '(?<=version )\d+'`
clang-tidy-verify-config: export EXPECTED_VERSION = "17"
clang-tidy-verify-config:
	@if [ "$(SYSTEM_VERSION)" != "$(EXPECTED_VERSION)" ]; then \
		echo "WARNING: Tested on clang-tidy $(EXPECTED_VERSION). Your version is $(SYSTEM_VERSION)."; \
	fi
	@clang-tidy --verify-config -p $(BUILD_DIR)
.PHONY: clang-tidy

check-code-style:
	@clang-format --dry-run $(CPP_SOURCE_FILES_LIST)
.PHONY: format-check

format-code:
	@clang-format -i $(CPP_SOURCE_FILES_LIST)
.PHONY: format-check


tests: build-tests
	@cd $(LINUX_BUILD_TESTS_DIR) && \
		ctest -V --output-on-failure
.PHONY: tests


pack-wasm:
	@cd $(WASM_BUILD_DIR) && cpack
.PHONY: pack-wasm


pack-linux:
	@cd $(LINUX_BUILD_DIR) && cpack
.PHONY: pack-linux


init-build-pack-wasm: install-tools init-wasm build-wasm build-web-shell pack-wasm
.PHONY: init-build-pack-wasm


docker-build-wasm:
	@mkdir -p $(BUILD_DIR)/docker_build && \
	  mkdir -p $(BUILD_DIR)/docker_tools && \
		docker compose run --rm build-wasm make init-build-pack-wasm
.PHONY: docker-build-wasm
