RISCV_PATH=~/RISCV/bin
RISCV_GCC=$(RISCV_PATH)/riscv64-unknown-elf-gcc
RISCV_GXX=$(RISCV_PATH)/riscv64-unknown-elf-g++
RISCV_SIM=~/riscv-isa-sim/build/spike

clean:
	rm -f test1_sim ma_test

build: clean
	$(RISCV_GXX) $(CXXFLAGS) -std=c++11 ma_test.cpp -o ma_test -I utest.h

test: build
	$(RISCV_SIM) pk ma_test

debug: CXXFLAGS:= -D DEBUG -D N=2 -D M=2 -D P=2
debug: build
	$(RISCV_SIM) pk ma_test
