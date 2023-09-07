CXX=g++
CXXFLAGS += -Wall -Werror -g3 -pthread -luuid
srcs := $(shell find ! -path "./esp*/*" \( -name "*.cpp" -o -name "*.c" \))

.PHONY: all

all: $(srcs:%.cpp=%)
.PHONY: clean

clean:
	rm -f *.o *.bin
	find * -type f -executable -delete
	cd esp_cf && idf.py fullclean && cd ..

% : %.o ; @$(LINK.cpp) $(OUTPUT_OPTION) $^ $(LDLIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $^
py:
	python3.9 cf.py

asm:
	nasm cf.asm -f bin -o cf.bin

esp:
	cd esp_cf && idf.py build && cd ..

flash:
	cd esp_cf && idf.py flash monitor && cd ..

erase_flash:
	cd esp_cf && idf.py erase_flash && cd ..

menuconfig:
	cd esp_cf && idf.py menuconfig && cd ..

r_asm:
	qemu-system-x86_64 -fda cf.bin

fwrd_qemu:
	socat tcp-l:5050,fork,reuseaddr tcp:127.0.0.1:5900

format:
	clang-format -i --style=file \
		$(shell find . ! -path "./*/build/*" \( -name '*.h' -o -name '*.hpp' -o -name '*.tcc' -o -name '*.c' -o -name '*.cpp' \))

new: clean
	cp tmpl cf.cpp
	rm -rf esp_cf/*
	cp -r esp_tmpl/* esp_cf/
	sed -i "s/@date/@date $(shell date +%F)./" cf.cpp esp_cf/main/*.c esp_cf/include/*.h

pre:
	$(CXX) -E cf.cpp

save:
ifeq ($(NAME),)
	cp cf.cpp saves/cf-$(shell date '+%Y%m%d').cpp
else
	cp cf.cpp saves/$(NAME).cpp
endif

save_esp:
ifeq ($(NAME),)
	cp -r esp_cf/ saves/esp_cf-$(shell date '+%Y%m%d')/
else
	cp -r esp_cf/ saves/$(NAME)/
endif

load: clean
ifneq ($(NAME),)
	cp saves/$(NAME).cpp cf.cpp
endif

load_esp: clean
ifneq ($(NAME),)
	rm -rf esp_cf/*
	cp -r saves/$(NAME)/* esp_cf/
endif
