bin_PROGRAMS 	= yass
yass_SOURCES 	= yass_main.cc config.cc parser.cc processor.cc theme.cc tokenizer.cc atom.cc
yass_SOURCES   += vendor/md4c/entity.c vendor/md4c/md4c-html.c vendor/md4c/md4c.c
yass_LDADD   	= -lctemplate -lsystem

AM_CXXFLAGS  	= -std=c++20 -fpermissive -Wall
