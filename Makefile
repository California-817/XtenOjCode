#这个makefile的作用就是编译整个项目，包括业务模块的动态库以及服务器框架代码
.PHONY: xx

"":
	if [ -d "build" ]; then \
		cd build && make -j2; \
	else \
		mkdir build; \
		cd build && cmake -DCMAKE_CXX_COMPILER:FILEPATH=$(shell which g++) -DCMAKE_C_COMPILER:FILEPATH=$(shell which gcc) ..; \
	fi

%:
	if [ -d "build" ]; then \
		cd build && make $@; \
	else \
		mkdir build; \
		cd build && cmake -DCMAKE_CXX_COMPILER:FILEPATH=$(shell which g++) -DCMAKE_C_COMPILER:FILEPATH=$(shell which gcc) $@ ..; \
	fi
