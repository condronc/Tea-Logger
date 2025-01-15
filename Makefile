CXX = g++
CXXFLAGS = `pkg-config --cflags gtkmm-4.0` -std=c++17
LDFLAGS = `pkg-config --libs gtkmm-4.0` -lsqlite3
SOURCES = src/main.cpp src/tea_logger.cpp src/db/db_handler.cpp src/utility/utility.cpp
TARGET = main

$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean
