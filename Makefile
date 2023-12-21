CPP_SOURCE_FILES_LIST=$(shell git ls-files | egrep '(h|cpp)$$')


init:
	@conan install . \
		--output-folder=build/linux \
		--build=missing \
		--settings=build_type=Release \
		-pr ./configs/profiles/linux
	@cd build/linux \
		&& cmake ../.. \
			-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
			-DCMAKE_BUILD_TYPE=Release \
		&& cd .. \
		&& ((ls compile_commands.json 2> /dev/null && echo "A link to compile_commands.json already exists") \
			|| ln -s linux/compile_commands.json)
.PHONY: init


init-wasm:
	@conan install . \
		--output-folder=build/wasm \
		--build=missing \
		--settings=build_type=Release \
		--profile:build=./configs/profiles/linux \
		--profile:host=./configs/profiles/wasm
	@cd build/wasm && \
		cmake cmake ../.. \
			-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
			-DCMAKE_BUILD_TYPE=Release
.PHONY: init-wasm


init-debug:
	@conan install . \
		--output-folder=build/linux-debug \
		--settings=build_type=Debug \
		--build=missing \
		-pr ./configs/profiles/linux
	@cd build/linux-debug && cmake ../.. \
		-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Debug
.PHONY: init-debug


build:
	@cd ./build/linux && cmake ../.. && cmake --build .
.PHONY: build


build-wasm:
	@cd ./build/wasm && \
		cmake cmake ../.. && \
		cmake --build .
.PHONY: build-wasm


build-debug:
	@cd ./build/linux-debug && \
		cmake ../.. -DCMAKE_CXX_FLAGS="-O0 -g3" && \
		cmake --build .
.PHONY: build-debug


run: build
	@./build/linux/bin/plane
.PHONY: run


run-debug: build-debug
	@gdb ./build/linux-debug/bin/plane
.PHONY: run-debug


cppcheck:
	@cppcheck \
		--enable=all \
		--inconclusive \
		--verbose \
		--quiet \
		--suppress=unusedFunction \
		--suppress=missingIncludeSystem \
		./src
.PHONY: cppcheck


clang-tidy:
	@clang-tidy $(CPP_SOURCE_FILES_LIST) -p ./build/
.PHONY: clang-tidy


check-code-style:
	@clang-format --dry-run $(CPP_SOURCE_FILES_LIST)
.PHONY: format-check


format-code:
	@clang-format -i $(CPP_SOURCE_FILES_LIST)
.PHONY: format-check
