CXX = g++
STATIC_CXXFLAGS = -O2 -static
CXXFLAGS = -O2
STATIC_LIBS = -lcurl -lssl -lcrypto -lz -ldl -pthread -lrt -static-libgcc -static-libstdc++
LIBS = -lcurl 

# Target binary
TARGET = cpoi-cli

all: $(TARGET)

$(TARGET): main.o tools.o
	$(CXX) $(CXXFLAGS) -o $(TARGET) main.o tools.o $(LIBS)

main.o: main.cc
	$(CXX) $(CXXFLAGS) -c main.cc

tools.o: tools.cc
	$(CXX) $(CXXFLAGS) -c tools.cc

clean:
	rm -f *.o $(TARGET)