#include <iostream>
#include <string>

#include "atom.h"
#include "config.h"
#include "parser.h"
#include "processor.h"
#include "theme.h"

int main(int argc, char **argv) {
  auto site_config = yass::ReadConfig("config.toml");
  yass::Parser parser;
  yass::Theme theme(site_config);
  yass::Atom atom(*site_config);
  yass::Processor processor(parser, theme, atom);
  processor.ProcessContent();
  return 0;
}
