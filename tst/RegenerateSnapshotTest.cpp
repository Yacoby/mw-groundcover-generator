#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <fstream>
#include <memory>
#include <vector>

#include "logic/Generator.h"

#include "TestHelpers.h"

namespace fs = std::filesystem;

BOOST_AUTO_TEST_SUITE(regenerate_tests);

    std::vector<std::string> getSnapshotDirectories() {
        std::vector<std::string> snapshotDirectories;
        for (auto const& entry : fs::directory_iterator{fs::path("regenerate_snapshots")}) {
            if (entry.is_directory()) {
                snapshotDirectories.push_back(entry.path().filename().string());
            }
        }
        return snapshotDirectories;
    }

    BOOST_DATA_TEST_CASE(
            Regen,
            boost::unit_test::data::make(getSnapshotDirectories()),
            snapshotName
    ) {
        Generator generator(
                makeNullLogger(),
                [](int, const std::string&) {},
                [](const std::string&) {},
                [](const std::exception& err) { throw err; },
                fs::path("regenerate_snapshots") / fs::path(snapshotName) / fs::path("configuration.ini"),
                std::vector<fs::path>({
                                              fs::path("regenerate_snapshots") / fs::path("UpdatedBase.esp")
                                      }),
                fs::path("output") / fs::path(snapshotName + ".regen.esp"),
                0
        );
        generator.generateFromExisting(fs::path("regenerate_snapshots") / fs::path(snapshotName) / fs::path("input.esp"));

        BOOST_TEST_CONTEXT("Snapshot test for: " + snapshotName) {
            verifyEspsMatch(
                    fs::path("output") / fs::path(snapshotName + ".regen.esp"),
                    fs::path("regenerate_snapshots") / fs::path(snapshotName) / fs::path("expected.esp")
            );
        }
    }

BOOST_AUTO_TEST_SUITE_END();
