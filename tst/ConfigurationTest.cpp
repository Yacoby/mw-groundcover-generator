#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <fstream>
#include <memory>
#include <vector>

#include "logic/Generator.h"
#include "TestHelpers.h"

using namespace boost::unit_test;
namespace fs = std::filesystem;

BOOST_AUTO_TEST_SUITE(configuration_tests);

    std::vector<fs::path> getModConfiguration() {
        std::vector<fs::path> iniFiles;
        for (auto const& entry: fs::recursive_directory_iterator{fs::path("existing_mod_configs")}) {
            if (entry.is_regular_file() && entry.path().extension() == ".ini") {
                iniFiles.push_back(entry.path());
            }
        }
        return iniFiles;
    }

    BOOST_DATA_TEST_CASE(
            Configuration,
            boost::unit_test::data::make(getModConfiguration()),
            configPath
    ) {
        // Tests configuration from existing mods loads as expected. We don't verify the output, just the lack of failure
        Generator generator(
                makeNullLogger(),
                [](int, const std::string&) {},
                [](const std::string&) {},
                [](const std::exception& exception) { throw exception; },
                configPath,
                std::vector<fs::path>({fs::path("generation_snapshots") / fs::path("GrassTestBase.esp")}),
                fs::path("output") / fs::path("config_test.esp"),
                0
        );
        generator.generate();
    }

BOOST_AUTO_TEST_SUITE_END();