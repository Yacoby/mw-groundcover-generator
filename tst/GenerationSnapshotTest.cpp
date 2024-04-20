#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <fstream>
#include <memory>
#include <vector>

#include "logic/Generator.h"

#include "TestHelpers.h"

namespace fs = std::filesystem;

std::vector<std::string> getSnapshotDirectories() {
    std::vector<std::string> snapshotDirectories;
    for (auto const& entry : fs::directory_iterator{fs::path("generation_snapshots")}) {
        if (entry.is_directory()) {
            snapshotDirectories.push_back(entry.path().filename().string());
        }
    }
    return snapshotDirectories;
}

BOOST_AUTO_TEST_SUITE(snapshot_tests);

    BOOST_DATA_TEST_CASE(
            TestSnapshot,
            boost::unit_test::data::make(getSnapshotDirectories()),
            name
    ) {
        Generator generator(
                makeNullLogger(),
                [](int, const std::string&) {},
                [](const std::string&) {},
                [](const std::exception& err) { throw err; },
                fs::path("generation_snapshots") / fs::path(name) / fs::path("configuration.ini"),
                std::vector<fs::path>({fs::path("generation_snapshots") / fs::path("GrassTestBase.esp")}),
                fs::path("output") / fs::path(name + ".esp"),
                0
        );
        generator.generate();

        BOOST_TEST_CONTEXT("Snapshot test for: " + name) {
            verifyEspsMatch(
                    fs::path("output") / fs::path(name + ".esp"),
                    fs::path("generation_snapshots") / fs::path(name) / fs::path("expected.esp")
            );
        }
    }

    BOOST_DATA_TEST_CASE(
            TestSnapshotWithOverriddenBase,
            boost::unit_test::data::make(getSnapshotDirectories()),
            name
    ) {
        Generator generator(
                makeNullLogger(),
                [](int, const std::string&) {},
                [](const std::string&) {},
                [](const std::exception& err) { throw err; },
                fs::path("generation_snapshots") / fs::path(name) / fs::path("configuration.ini"),
                std::vector<fs::path>({
                                              fs::path("generation_snapshots") / fs::path("GrassTestBaseToBeOverridden.esp"),
                                              fs::path("generation_snapshots") / fs::path("GrassTestBase.esp"),
                                      }),
                fs::path("output") / fs::path(name + ".esp"),
                0
        );
        generator.generate();

        BOOST_TEST_CONTEXT("Snapshot test for: " + name) {
            verifyEspsMatch(
                    fs::path("output") / fs::path(name + ".esp"),
                    fs::path("generation_snapshots") / fs::path(name) / fs::path("expected.esp")
            );
        }
    }

BOOST_AUTO_TEST_SUITE_END();
