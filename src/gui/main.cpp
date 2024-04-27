#include <wx/wxprec.h>
#include <wx/wx.h>
#include <wx/msgdlg.h>

#include "spdlog/async.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "GUI.h"

class MWGrassApp : public wxApp {
    std::shared_ptr<spdlog::logger> logger = nullptr;
public:
    virtual bool OnInit() {
        spdlog::set_pattern("[%H:%M:%S.%e %L] %v");
        try {
            logger = spdlog::basic_logger_mt<spdlog::async_factory>("async_file_logger", "MWMeshGenLog.txt", true);
        } catch (const spdlog::spdlog_ex& ex) {
            logger = spdlog::stdout_color_mt("console");
            logger->warn("Using fallback logger because: " + std::string(ex.what()));
        }

        GUI *ui = new GUI(nullptr, logger);
        ui->Show();
        return true;
    }

    virtual bool OnExceptionInMainLoop() {
        try {
            throw;
        } catch (std::exception &e) {
            wxMessageBox(e.what(), wxT("Something went wrong"), wxICON_ERROR);

            if (logger) {
                logger->critical("Got exception in main loop: {}", e.what());
                logger->flush();
            }
        }
        return false;
    }
};

IMPLEMENT_APP(MWGrassApp)