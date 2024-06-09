#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>

#include <filesystem>

#include "spdlog/spdlog.h"

#include "cli/CliMain.h"

#include "TestHelpers.h"


namespace fs = std::filesystem;
using namespace std;

struct Fixture {
    ~Fixture() {
        spdlog::drop("console");
    }
};

BOOST_FIXTURE_TEST_SUITE(cli_test, Fixture);

    BOOST_AUTO_TEST_CASE(WithNoAction) {
        const char* argv[] = {"ProgramName"};
        BOOST_TEST(cliMain(sizeof(argv)/sizeof(*argv), const_cast<char**>(argv)) == 1);
    }

    BOOST_AUTO_TEST_CASE(WithInvalidAction) {
        const char* argv[] = {"ProgramName", "foo"};
        BOOST_TEST(cliMain(sizeof(argv)/sizeof(*argv), const_cast<char**>(argv)) == 1);
    }

    BOOST_AUTO_TEST_CASE(WithNoActionParams) {
        const char* argv[] = {"ProgramName", "generate"};
        BOOST_TEST(cliMain(sizeof(argv)/sizeof(*argv), const_cast<char**>(argv)) == 1);
    }

    BOOST_AUTO_TEST_CASE(WithMissingArgumentValue) {
        const char* argv[] = {"ProgramName", "generate", "--configuration"};
        BOOST_TEST(cliMain(sizeof(argv)/sizeof(*argv), const_cast<char**>(argv)) == 1);
    }

    BOOST_AUTO_TEST_CASE(WithInvalidArgumentValue) {
        const char* argv[] = {"ProgramName", "generate", "--cfg"};
        BOOST_TEST(cliMain(sizeof(argv)/sizeof(*argv), const_cast<char**>(argv)) == 1);
    }

    BOOST_AUTO_TEST_CASE(WithValidGenerateCommand) {
        auto snapshotName = string("basic");

        auto cfgPath = (fs::path("generation_snapshots") / fs::path(snapshotName) / fs::path("configuration.ini"));
        auto basePluginPath = fs::path("generation_snapshots") / fs::path("GrassTestBaseToBeOverridden.esp");
        auto overridePath = fs::path("generation_snapshots") / fs::path("GrassTestBase.esp");
        auto outputPath = fs::path("output") / fs::path(snapshotName + ".cli.esp");

        const char* argv[] = {
                "ProgramName", "generate",
                "--configuration", cfgPath.c_str(),
                "--plugin", basePluginPath.c_str(),
                "--plugin", overridePath.c_str(),
                "--output", outputPath.c_str(),
                "--seed", "0",
        };
        BOOST_TEST(cliMain(sizeof(argv)/sizeof(*argv), const_cast<char**>(argv)) == 0);

        verifyEspsMatch(
                fs::path("output") / fs::path(snapshotName + ".cli.esp"),
                fs::path("generation_snapshots") / fs::path(snapshotName) / fs::path("expected.esp")
        );
    }

BOOST_AUTO_TEST_SUITE_END();