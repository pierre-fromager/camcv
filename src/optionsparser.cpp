
#include <optionsparser.h>

namespace bpo = boost::program_options;

OptionsParser::OptionsParser(int argc, char **argv) : m_argc(argc), m_argv(argv)
{
}

OptionsParser::~OptionsParser()
{
}

ui_t OptionsParser::parse(cmd_options_t &cmdopts)
{
    bpo::options_description desc(CMD_OPT_USAGE);
    desc.add_options()(CMD_OPT_HLP, CMD_OPT_HLP_OPTS);
    desc.add_options()(CMD_OPT_D, bpo::value<int>(), CMD_OPT_D_HLP);
    desc.add_options()(CMD_OPT_T, bpo::value<int>(), CMD_OPT_T_HLP);
    desc.add_options()(CMD_OPT_W, bpo::value<int>(), CMD_OPT_W_HLP);
    desc.add_options()(CMD_OPT_V, bpo::value<int>(), CMD_OPT_V_HLP);
    desc.add_options()(CMD_OPT_I, bpo::value<int>(), CMD_OPT_I_HLP);
    bpo::variables_map vm;
    bpo::store(bpo::parse_command_line(m_argc, m_argv, desc), vm);
    bpo::notify(vm);
    if (vm.count(CMD_OPT_HLP))
    {
        std::cout << desc << std::endl;
        return EXIT_FAILURE;
    }
    if (vm.count(CMD_OPT_D))
        cmdopts.deviceId = vm[CMD_OPT_D].as<int>();
    if (vm.count(CMD_OPT_T))
        cmdopts.threshold = vm[CMD_OPT_T].as<int>();
    if (vm.count(CMD_OPT_W))
        cmdopts.width = vm[CMD_OPT_W].as<int>();
    if (vm.count(CMD_OPT_V))
        cmdopts.verbosity = vm[CMD_OPT_V].as<int>();
    if (vm.count(CMD_OPT_I))
        cmdopts.cintval = vm[CMD_OPT_I].as<int>();
    return EXIT_SUCCESS;
}