CXX=g++
CXXFLAGS += -Wall -Werror -g3 -pthread -luuid
srcs := $(shell find . -name \*.cpp)

.PHONY: all

all: $(srcs:%.cpp=%)
.PHONY: clean

clean:
	rm -f *.o *.bin
	find * -type f -executable -delete

% : %.o ; @$(LINK.cpp) $(OUTPUT_OPTION) $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
py:
	python3.9 cf.py

asm:
	nasm cf.asm -f bin -o cf.bin

r_asm:
	qemu-system-x86_64 -fda cf.bin

fwrd_qemu:
	socat tcp-l:5050,fork,reuseaddr tcp:127.0.0.1:5900

format:
	clang-format -i --style=file \
		$(shell find -name '*.h' -o -name '*.hpp' -o -name '*.tcc' -o -name '*.c' -o -name '*.cpp')

new:
	cp tmpl cf.cpp
	sed -i "s/@date/@date $(shell date +%F)./" cf.cpp

pre:
	$(CXX) -E cf.cpp

save:
ifeq ($(NAME),)
	cp cf.cpp saves/cf-$(shell date '+%Y%m%d').cpp
else
	cp cf.cpp saves/$(NAME).cpp
endif

load:
ifneq ($(NAME),)
	cp saves/$(NAME).cpp cf.cpp
endif
