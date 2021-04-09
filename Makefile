all:
	make cb
	make m

m:
	cd build & cmake .. -G "MinGW Makefiles" & make

cb:
	cd build_codeblocks & cmake .. -G "CodeBlocks - MinGW Makefiles"