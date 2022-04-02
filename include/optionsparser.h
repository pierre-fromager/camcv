
#ifndef OPTIONS_PARSER_H
#define OPTIONS_PARSER_H

#include <iostream>
#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/cmdline.hpp>
#include <boost/program_options/errors.hpp>
#include <boost/program_options/option.hpp>
#include <boost/program_options/value_semantic.hpp>
#include <boost/program_options/version.hpp>
#include <options.h>

#define CMD_OPT_USAGE "Usage camcv [options]"
#define CMD_OPT_HLP "help"
#define CMD_OPT_HLP_OPTS "help options"
#define CMD_OPT_D "d"
#define CMD_OPT_D_HLP "set device id"
#define CMD_OPT_T "t"
#define CMD_OPT_T_HLP "set threshold"
#define CMD_OPT_W "w"
#define CMD_OPT_W_HLP "set width"
#define CMD_OPT_V "v"
#define CMD_OPT_V_HLP "set verbosity"
#define CMD_OPT_I "i"
#define CMD_OPT_I_HLP "set capture interval"
#define CMD_OPT_S "s"
#define CMD_OPT_S_HLP "save image"
#define CMD_OPT_FT "ft"
#define CMD_OPT_FT_HLP "filter type"
#define CMD_OPT_FV "fv"
#define CMD_OPT_FV_HLP "filter value"

namespace bpo = boost::program_options;

class OptionsParser
{
public:
    explicit OptionsParser(int argc, char **argv);
    ~OptionsParser();
    ui_t parse(cmd_options_t &cmdopts);
    void debug(cmd_options_t &cmdopts);

private:
    int m_argc;
    char **m_argv;
};

#endif