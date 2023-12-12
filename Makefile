init:
	@conan install . \
		--output-folder=build \
		--build=missing
		-pr ./configs/profiles/linux
	@cd build && cmake .. \
		-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Release
.PHONY: init


init-wasm:
	@conan install . \
		--output-folder=wasm \
		--build=missing \
		--profile:host=wasm \
		--profile:build=default
	@cd wasm && \
		cmake cmake .. \
			-DCMAKE_TOOLCHAIN_FILE=wasm/conan_toolchain.cmake \
			-DCMAKE_BUILD_TYPE=Release
.PHONY: init-wasm


init-debug:
	@conan install . \
		--output-folder=debug \
		--build=missing
		-pr ./configs/profiles/linux
	@cd debug && cmake .. \
		-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Debug
.PHONY: init-debug


build:
	@cd ./build && cmake .. && cmake --build .
.PHONY: build


build-wasm:
	@cd ./wasm && \
		cmake cmake .. && \
		cmake --build .
.PHONY: build-wasm


build-debug:
	@cd ./debug && \
		cmake .. -DCMAKE_CXX_FLAGS="-O0 -g3" && \
		cmake --build .
.PHONY: build-debug


run: build
	@./build/plane
.PHONY: run


run-debug: build-debug
	@gdb ./debug/plane
.PHONY: run-debug


static-check:
	@cppcheck \
		--enable=all \
		--inconclusive \
		--verbose \
		--quiet \
		--suppress=unusedFunction \
		--suppress=missingIncludeSystem \
		./src
.PHONY: static-check
