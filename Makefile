init:
	@conan install . \
		--output-folder=build \
		--build=missing
		-s cppstd=20
	@cd build && cmake .. \
		-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Release
.PHONY: init

init-debug:
	@conan install . \
		--output-folder=debug \
		--build=missing
	@cd debug && cmake .. \
		-DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake \
		-DCMAKE_BUILD_TYPE=Debug
.PHONY: init-debug

build:
	@cd ./build && cmake .. && cmake --build .
.PHONY: build

build-debug:
	@cd ./debug && \
		cmake -DCMAKE_BUILD_TYPE=Release .. && \
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
