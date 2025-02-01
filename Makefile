CXX = g++
CXXFLAGS = `pkg-config --cflags gtkmm-4.0` -std=c++17
LDFLAGS = `pkg-config --libs gtkmm-4.0` -lsqlite3
SOURCES = src/main.cpp src/app.cpp src/appwindow.cpp src/models/tea_list.cpp src/models/tea_entry.cpp src/helpers/helper.cpp
TARGET = main

$(TARGET): $(SOURCES)
	$(CXX) $(SOURCES) -o $(TARGET) $(CXXFLAGS) $(LDFLAGS)

clean:
	rm -f $(TARGET)

.PHONY: clean
