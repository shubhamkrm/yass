#include <iostream>
#include <string>

#include "config.h"
#include "parser.h"
#include "processor.h"
#include "theme.h"

int main(int argc, char **argv) {
    auto site_config = yass::ReadConfig("config.toml");
    yass::Parser parser;
    yass::Theme theme("default", site_config);
    yass::Processor processor(parser, theme);
    processor.ProcessContent();
    return 0;
}

