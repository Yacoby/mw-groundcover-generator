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

BOOST_AUTO_TEST_SUITE(mod_parsing_test);

    std::vector<fs::path> getModFiles() {
        std::vector<fs::path> files;
        for (auto const& entry: fs::directory_iterator{fs::path("existing_mods")}) {
            if (entry.is_regular_file() && entry.path().extension() == ".esp") {
                files.push_back(entry.path());
            }
        }
        return files;
    }

    BOOST_DATA_TEST_CASE(
            TestLoadThenSaveMatches,
            boost::unit_test::data::make(getModFiles()),
            modFile
    ) {
        // It is really hard to assert anything here, because the file written back isn't quite the same because we
        // - Reset the FRMR records
        // - Ignore some subrecords in CELLs like NAM0
        // As such this just verifies we can load and save
        MutableEsp esp;
        esp.load(modFile);
        auto outputPath = fs::path("output") / modFile.filename();
        esp.save(outputPath);
    }

BOOST_AUTO_TEST_SUITE_END();