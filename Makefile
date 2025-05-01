# Makefile at project root

# Get list of staged .cpp and .h files
STAGED_FILES := $(shell git diff --cached --name-only --diff-filter=ACM | grep -E '\.(cpp|h)$$')

.PHONY: pre-commit

pre-commit:
	@if [ -z "$(STAGED_FILES)" ]; then \
		echo "No staged C++ files to format."; \
	else \
		echo "Formatting staged files with clang-format:"; \
		echo "$(STAGED_FILES)"; \
		clang-format -i $(STAGED_FILES); \
		git add $(STAGED_FILES); \
		echo "Formatting complete. Files updated in index (not committed)."; \
	fi

configure:
	cmake -B build -DCMAKE_BUILD_TYPE=Debug -DBUILD_PLUGIN_TESTS=ON

build:
	cmake --build build 

test: configure
	cmake --build build --target AudioProcessorTest
	./build/test/AudioProcessorTest

vst3: configure
	cmake --build build --target AudioPlugin_VST3
	./build/test/AudioProcessorTest

all: configure
	cmake --build build --target AudioPlugin AudioProcessorTest