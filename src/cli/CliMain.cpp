#include <vector>
#include <string>
#include <filesystem>

#include <boost/program_options.hpp>

#include "spdlog/sinks/stdout_color_sinks.h"

#include "logic/Generator.h"

namespace po = boost::program_options;

using namespace std;

enum Action {GENERATE,};
vector<Action> ACTIONS = {GENERATE,};

std::istream& operator>>(std::istream& in, Action& action) {
    std::string token;
    in >> token;
    if (token == "generate") {
        action = GENERATE;
    } else {
        in.setstate(std::ios_base::failbit);
    }
    return in;
}

std::ostream& operator<<(std::ostream& out, const Action& cmd) {
    if (cmd == Action::GENERATE) {
        out << "generate";
    }
    return out;
}

int doGenerate(const std::shared_ptr<spdlog::logger> logger, const vector<string>& opts) {
    po::options_description options("generate options");
    options.add_options()
            ("help,h", "Prints this message")
            ("configuration", po::value<string>()->required(), "Path to configuration file")
            ("plugin", po::value<vector<string>>()->required(), "Path to plugin(s)")
            ("output", po::value<string>()->required(), "Path to the output file")
            ("seed", po::value<unsigned>()->default_value(time(nullptr)), "Random seed")
            ;

    po::variables_map vm;
    try {
        po::store(po::command_line_parser(opts).options(options).run(), vm);
    }  catch (po::error& e) {
        cerr << e.what() << endl;
        cerr << options << "\n";
        return 1;
    }

    if (vm.count("help")) {
        cout << options << "\n";
        return 1;
    }
    try {
        po::notify(vm);
    } catch (po::error& e) {
        cerr << e.what() << endl;
        cerr << options << "\n";
        return 1;
    }

    auto& cfgStr = vm["configuration"].as<string>();
    if (!filesystem::exists(filesystem::path(cfgStr))) {
        cerr << cfgStr << " doesn't exist" << endl;
        return 1;
    }

    auto& pluginsStr = vm["plugin"].as<vector<string>>();
    vector<filesystem::path> plugins;
    plugins.reserve(pluginsStr.size());
    for (const auto& pluginStr: pluginsStr) {
        if (!filesystem::exists(filesystem::path(pluginStr))) {
            cerr << pluginStr << " doesn't exist" << endl;
            return 1;
        }
        plugins.push_back(pluginStr);
    }

    Generator generator(
            logger,
            [](int, const std::string&) {},
            [](const std::string&) {},
            [](const std::exception& err) { throw err; },
            filesystem::path(vm["configuration"].as<string>()),
            plugins,
            filesystem::path(vm["output"].as<string>()),
            vm["seed"].as<unsigned >()
    );

    try {
        generator.generate();
    } catch (exception& e) {
        cerr << e.what() << endl;
        return 2;
    }

    return 0;
}

int cliMain(int argc, char* argv[]) {
    std::shared_ptr<spdlog::logger> logger = spdlog::stdout_color_mt("console");

    po::options_description options("options");
    options.add_options()
            ("help,h", "Print this help message")
            ;

    po::options_description hidden;
    hidden.add_options()
            ("action", po::value<Action>()->required(), "action to execute")
            ("subargs", po::value<vector<string>>(), "arguments for the action")
            ;

    po::options_description combinedOptions;
    combinedOptions.add(options).add(hidden);

    po::positional_options_description pos;
    pos.add("action", 1).add("subargs", -1);

    po::variables_map vm;
    po::parsed_options parsed = po::command_line_parser(argc, argv)
            .options(combinedOptions)
            .positional(pos)
            .allow_unregistered()
            .run();
    try {
        po::store(parsed, vm);
    } catch (po::error& e) {
        cerr << e.what() << endl;
        return 1;
    }

    if (vm.count("help") && vm["action"].empty()) {
        cout << options << "\n";
        return 1;
    } else if (vm["action"].empty()) {
        cerr << "A action is required as the first argument. Valid actions:" << endl;
        for (const auto& action: ACTIONS) {
            cerr << "- " << action << endl;
        }
        return 1;
    }
    try {
        po::notify(vm);
    } catch (po::required_option& e) {
        cerr << e.what() << endl;
        cerr << options << "\n";
        return 1;
    }

    vector<string> opts = po::collect_unrecognized(parsed.options, po::include_positional);
    opts.erase(opts.begin()); // erase the (positional) action name

    auto action = vm["action"].as<Action>();
    if (action == Action::GENERATE) {
        return doGenerate(logger, opts);
    }
    return 1;
}
