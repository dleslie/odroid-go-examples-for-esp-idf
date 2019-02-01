#include <odroid_go.h>
#include "example.h"

std::shared_ptr<Examples::Example> Examples::example_list;
std::shared_ptr<Examples::Example> Examples::current;

void Examples::next_example() {
  if (Examples::current != nullptr) {
    Examples::current->takedown();
    Examples::current = Examples::current->next;
  }
  if (Examples::current == nullptr)
    Examples::current = Examples::example_list;
  if (Examples::current != nullptr) {
    Serial.printf("Loading example: %s\n", Examples::current->name());
    Examples::current->setup();
  }
}

Examples::Example::~Example() {
  if (prev != nullptr)
    prev->next = next;
  if (next != nullptr)
    next->prev = prev;
  if (Examples::example_list == nullptr || Examples::example_list.get() == this)
    Examples::example_list = next;
  if (Examples::current == nullptr || Examples::current.get() == this)
    Examples::current = next;  
}

void Examples::register_example(std::shared_ptr<Example> &example) {
  if (Examples::example_list == nullptr)
    Examples::example_list = example;
  else {
    auto cur = Examples::example_list;
    while (cur->next != nullptr)
      cur = cur->next;
    cur->next = example;
    example->prev = cur;
  }
}

const char * Examples::Example::name() {
  return "Unnamed Example";
}

void Examples::Example::setup() {}
void Examples::Example::loop() {}
void Examples::Example::takedown() {}

