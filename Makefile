CPP_SOURCE_FILES_LIST=$(shell git ls-files | egrep '(h|cpp)$$')
PROJECT_NAME=plane
BUILD_DIR=$(shell pwd)/build
PACK_DIR=$(BUILD_DIR)/pack
LINUX_BUILD_DIR=$(BUILD_DIR)/linux
LINUX_BIN_DIR=$(LINUX_BUILD_DIR)/bin
LINUX_DEBUG_BUILD_DIR=$(BUILD_DIR)/linux-debug
LINUX_DEBUG_BIN_DIR=$(LINUX_DEBUG_BUILD_DIR)/bin
WASM_BUILD_DIR=$(BUILD_DIR)/wasm
WASM_BIN_DIR=$(WASM_BUILD_DIR)/bin


init:
	@conan install . \
		--output-folder=$(LINUX_BUILD_DIR) \
		--build=missing \
		--lockfile=./linux.lock \
		--profile:build=./configs/profiles/linux \
		--profile:host=./configs/profiles/linux
	@cd $(LINUX_BUILD_DIR) \
		&& cmake ../.. \
			-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
			-DCMAKE_BUILD_TYPE=Release \
		&& cd .. \
		&& ((ls compile_commands.json 2> /dev/null && echo "A link to compile_commands.json already exists") \
			|| ln -s linux/compile_commands.json)
.PHONY: init


init-wasm:
	@conan install . \
		--output-folder=$(WASM_BUILD_DIR) \
		--build=missing \
		--lockfile=./wasm.lock \
		--profile:build=./configs/profiles/linux \
		--profile:host=./configs/profiles/wasm
	@cd $(WASM_BUILD_DIR) && \
		cmake cmake ../.. \
			-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
			-DCMAKE_BUILD_TYPE=Release
.PHONY: init-wasm


init-debug:
	@conan install . \
		--output-folder=$(LINUX_DEBUG_BUILD_DIR) \
		--settings=build_type=Debug \
		--build=missing \
		-pr ./configs/profiles/linux
	@cd $(LINUX_DEBUG_BUILD_DIR) && \
		cmake ../.. \
			-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
			-DCMAKE_BUILD_TYPE=Debug
.PHONY: init-debug


build:
	@cd $(LINUX_BUILD_DIR) && \
		cmake ../.. \
			-DBUILD_TESTS=OFF \
			-DBUILD_EXECUTABLE=ON && \
		cmake --build .
.PHONY: build


build-tests:
	@cd $(LINUX_BUILD_DIR) && \
		cmake ../.. \
			-DBUILD_TESTS=ON \
			-DBUILD_EXECUTABLE=OFF && \
		cmake --build .
.PHONY: build-tests


build-wasm:
	@cd $(WASM_BUILD_DIR) && \
		cmake ../.. \
			-DBUILD_TESTS=OFF \
			-DBUILD_EXECUTABLE=ON && \
		cmake --build .
.PHONY: build-wasm


build-wasm-tests:
	@cd $(WASM_BUILD_DIR) && \
		cmake ../.. \
			-DBUILD_TESTS=ON \
			-DBUILD_EXECUTABLE=OFF && \
		cmake --build .
.PHONY: build-wasm-tests


build-debug:
	@cd $(LINUX_DEBUG_BUILD_DIR) && \
		cmake ../.. \
			-DBUILD_TESTS=OFF \
			-DBUILD_EXECUTABLE=ON \
			-DCMAKE_CXX_FLAGS="-O0 -g3" && \
		cmake --build .
.PHONY: build-debug


run: build
	@$(LINUX_BIN_DIR)/$(PROJECT_NAME)
.PHONY: run


run-debug: build-debug
	@gdb $(LINUX_DEBUG_BIN_DIR)/$(PROJECT_NAME)
.PHONY: run-debug


run-wasm: build-wasm
	@cd $(WASM_BIN_DIR) && \
		python3 -m http.server -b 127.0.0.1
.PHONY: run-wasm


cppcheck:
	@cppcheck \
		--enable=all \
		--inconclusive \
		--verbose \
		--quiet \
		-I ./ \
		--suppress=unusedFunction \
		--suppress=missingIncludeSystem \
		./src
.PHONY: cppcheck


clang-tidy:
	@clang-tidy $(CPP_SOURCE_FILES_LIST) -p $(BUILD_DIR)
.PHONY: clang-tidy


check-code-style:
	@clang-format --dry-run $(CPP_SOURCE_FILES_LIST)
.PHONY: format-check


format-code:
	@clang-format -i $(CPP_SOURCE_FILES_LIST)
.PHONY: format-check


tests: build-tests
	@cd $(LINUX_BUILD_DIR) && \
		ctest --output-on-failure
.PHONY: tests


tests-wasm: build-wasm-tests
	@cd $(WASM_BUILD_DIR) && \
		ctest --output-on-failure
.PHONY: tests-wasm


pack-wasm:
	@cd $(WASM_BUILD_DIR) && cpack
.PHONY: pack-wasm


pack-linux:
	@cd $(LINUX_BUILD_DIR) && cpack
.PHONY: pack-linux


init-build-pack-wasm: init-wasm build-wasm pack-wasm
.PHONY: init-build-pack-wasm


docker-build-wasm:
	@mkdir -p $(BUILD_DIR)/docker_build && \
	  mkdir -p $(BUILD_DIR)/docker_conan && \
		docker compose run --rm build-wasm make init-build-pack-wasm
.PHONY: docker-build-wasm


lock-files:
	@conan lock create . -s os=Linux \
		--lockfile-out=linux.lock \
		--profile=./configs/profiles/linux \
		--lockfile-clean
	@conan lock create . -s os=Emscripten \
		--lockfile-out=wasm.lock \
		--profile=./configs/profiles/wasm \
		--lockfile-clean
.PHONY: lock-files
