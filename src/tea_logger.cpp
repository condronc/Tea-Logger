#include "tea_logger.hpp"

#include <iostream>

TeaLogger::TeaLogger() : m_button("Tea Logger") {
  m_button.set_margin(10);
  m_button.signal_clicked().connect(
      sigc::mem_fun(*this, &TeaLogger::on_button_clicked));
  set_child(m_button);
}

TeaLogger::~TeaLogger() {}

void TeaLogger::on_button_clicked() { std::cout << "Hello World" << std::endl; }