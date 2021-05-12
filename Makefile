.PHONY: build cb

build:
	cd build & cmake .. -G "MinGW Makefiles" -DCMAKE_C_FLAGS="-mwindows" & make

cb:
	cd build & cmake .. -G "CodeBlocks - MinGW Makefiles"
