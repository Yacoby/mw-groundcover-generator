#include <boost/test/unit_test.hpp>
#include <boost/test/parameterized_test.hpp>
#include <boost/test/data/test_case.hpp>

#include <fstream>
#include <memory>
#include <vector>

#include "logic/FixPosition.h"
#include "TestHelpers.h"

namespace fs = std::filesystem;

BOOST_AUTO_TEST_SUITE(fix_tests);

    std::vector<std::string> getSnapshotDirectories() {
        std::vector<std::string> snapshotDirectories;
        for (auto const& entry : fs::directory_iterator{fs::path("fix_snapshots")}) {
            if (entry.is_directory()) {
                snapshotDirectories.push_back(entry.path().filename().string());
            }
        }
        return snapshotDirectories;
    }

    BOOST_DATA_TEST_CASE(
            Update,
            boost::unit_test::data::make(getSnapshotDirectories()),
            snapshotName
    ) {
        PositionUpdater updater(
                makeNullLogger(),
                [](int, const std::string&) {},
                [](const std::string&) {},
                [](const std::exception& err) { throw err; },
                std::vector<fs::path>({fs::path("fix_snapshots") / fs::path("UpdatedBase.esp")}),
                fs::path("fix_snapshots") / snapshotName / fs::path("input.esp"),
                fs::path("output") / fs::path(snapshotName + ".fix.update.output.esp")
        );
        updater.fix(false);

        BOOST_TEST_CONTEXT("Snapshot test for: " + snapshotName) {
            verifyEspsMatch(
                    fs::path("output") / fs::path(snapshotName + ".fix.update.output.esp"),
                    fs::path("fix_snapshots") / snapshotName / fs::path("expected_update.esp")
            );
        }
    }

    BOOST_DATA_TEST_CASE(
            Delete,
            boost::unit_test::data::make(getSnapshotDirectories()),
            snapshotName
    ) {
        PositionUpdater updater(
                makeNullLogger(),
                [](int, const std::string&) {},
                [](const std::string&) {},
                [](const std::exception& err) { throw err; },
                std::vector<fs::path>({fs::path("fix_snapshots") / fs::path("UpdatedBase.esp")}),
                fs::path("fix_snapshots") / snapshotName / fs::path("input.esp"),
                fs::path("output") / fs::path(snapshotName + ".fix.delete.output.esp")
        );
        updater.fix(true);

        BOOST_TEST_CONTEXT("Snapshot test for: " + snapshotName) {
            verifyEspsMatch(
                    fs::path("output") / fs::path(snapshotName + ".fix.delete.output.esp"),
                    fs::path("fix_snapshots") / snapshotName / fs::path("expected_delete.esp")
            );
        }
    }

BOOST_AUTO_TEST_SUITE_END();
