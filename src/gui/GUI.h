#ifndef __GUI__
#define __GUI__

#define GEN_WORKER_BASE_EVT_ID 60000
#define GEN_WORKER_UPDATE GEN_WORKER_BASE_EVT_ID + 1
#define GEN_WORKER_SUCCESS GEN_WORKER_BASE_EVT_ID + 2
#define GEN_WORKER_FAILURE GEN_WORKER_BASE_EVT_ID + 3

#define REGEN_WORKER_BASE_EVT_ID 70000
#define REGEN_WORKER_UPDATE REGEN_WORKER_BASE_EVT_ID + 1
#define REGEN_WORKER_SUCCESS REGEN_WORKER_BASE_EVT_ID + 2
#define REGEN_WORKER_FAILURE REGEN_WORKER_BASE_EVT_ID + 3

#define FIX_WORKER_BASE_EVT_ID 80000
#define FIX_WORKER_UPDATE FIX_WORKER_BASE_EVT_ID + 1
#define FIX_WORKER_SUCCESS FIX_WORKER_BASE_EVT_ID + 2
#define FIX_WORKER_FAILURE FIX_WORKER_BASE_EVT_ID + 3

#include <filesystem>
#include <optional>

#include <wx/msgdlg.h>
#include <boost/algorithm/string/join.hpp>
#include "spdlog/spdlog.h"

#include "logic/LoadOrder.h"

#include "GrassGen.h"


/** Implementing GrassGen */
class GUI : public GrassGen {
private:
    constexpr static const char* SETTINGS_FILE = "MWGroundcoverGenSettings.ini";

    std::shared_ptr<spdlog::logger> logger;

    std::filesystem::path morrowindDirectory;
    std::unique_ptr<LoadOrder> loadOrder;

    std::optional<std::string> getRegKey(const char* pos, const char* name);

    std::vector<std::string> getNestedExceptionMessages(const std::exception& e);

protected:
    void sendStatusUpdate(int baseId, int progressPercent, const std::string &message) {
        wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, baseId + 1);
        evt.SetInt(progressPercent);
        evt.SetString(message);
        wxPostEvent(this, evt);
    }

    void sendSuccess(int baseId, const std::string& message) {
        wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, baseId + 2);
        evt.SetString(message);
        wxPostEvent(this, evt);
    }

    void sendFailure(int baseId, const std::exception& exception) {
        wxCommandEvent evt(wxEVT_COMMAND_MENU_SELECTED, baseId + 3);
        auto messages = getNestedExceptionMessages(exception);
        auto stringMessage = boost::algorithm::join(messages, "\n");

        logger->error("Saw failure from thread:\n {}", stringMessage);
        logger->flush();

        evt.SetString(stringMessage);
        wxPostEvent(this, evt);
    }

    std::pair<wxGauge*, wxButton*> getProgressBarAndButton(const wxEvent& event) {
        switch (event.GetId()) {
            case GEN_WORKER_UPDATE:
            case GEN_WORKER_SUCCESS:
            case GEN_WORKER_FAILURE:
                return std::pair<wxGauge*, wxButton*>(mProgBar, mGenerate);
            case REGEN_WORKER_UPDATE:
            case REGEN_WORKER_SUCCESS:
            case REGEN_WORKER_FAILURE:
                return std::pair<wxGauge*, wxButton*>(regenerateProgressBar, regenerateButton);
            case FIX_WORKER_UPDATE:
            case FIX_WORKER_SUCCESS:
            case FIX_WORKER_FAILURE:
                return std::pair<wxGauge*, wxButton*>(fixProgressBar, fixButton);
        }
        throw std::runtime_error("unknown event id");
    }

    void OnThreadStatusUpdate(wxCommandEvent &event) {
        getProgressBarAndButton(event).first->SetValue(event.GetInt());
        mStatus->SetStatusText(event.GetString());
    }

    void OnThreadSuccess(wxCommandEvent &event) {
        auto barAndButton = getProgressBarAndButton(event);
        barAndButton.first->SetValue(100);
        mStatus->SetStatusText(event.GetString());
        barAndButton.second->Enable(true);
    }

    void OnThreadFailure(wxCommandEvent &event) {
        auto barAndButton = getProgressBarAndButton(event);
        barAndButton.second->Enable(true);
        barAndButton.first->SetValue(0);
        mStatus->SetStatusText("Failed");

        wxMessageBox(event.GetString(), wxT("Something went wrong"), wxICON_ERROR);
    }

    // Handlers for GrassGen events.
    void OnImportPress(wxCommandEvent &event) override;
    void OnImportOpenMwPress(wxCommandEvent &event) override;

    void OnAddPress(wxCommandEvent &event) override;

    void OnRemovePress(wxCommandEvent &event) override;
    void OnResetPress(wxCommandEvent &event) override;

    void OnGenPress(wxCommandEvent &event) override;

    void OnRegenerateSetTarget( wxCommandEvent& event ) override {
        wxArrayInt selections;
        mModList->GetSelections(selections);
        if (selections.empty()) {
            wxMessageBox("Nothing selected in the data file list", wxT("Something went wrong"), wxICON_ERROR);
            return;
        } else if (selections.size() > 1) {
            wxMessageBox("Multiple items selected in the data file list. Only one can be a target", wxT("Something went wrong"), wxICON_ERROR);
            return;
        }

        auto& path = loadOrder->get().at(selections[0])->path;
        auto output = path.parent_path() / fs::path(path.filename().stem().string() + "Patched" + path.filename().extension().string());

        regenerateTargetPicker->SetPath(path.string());
        regenerateOutputPicker->SetPath(output.string());
    }

    void OnRegenerateStart( wxCommandEvent& event ) override;

    void OnFixSetTarget( wxCommandEvent& event ) override {
        wxArrayInt selections;
        mModList->GetSelections(selections);
        if (selections.empty()) {
            wxMessageBox("Nothing selected in the data file list", wxT("Something went wrong"), wxICON_ERROR);
            return;
        } else if (selections.size() > 1) {
            wxMessageBox("Multiple items selected in the data file list. Only one can be a target", wxT("Something went wrong"), wxICON_ERROR);
            return;
        }

        auto& path = loadOrder->get().at(selections[0])->path;
        auto output = path.parent_path() / fs::path(path.filename().stem().string() + "Patched" + path.filename().extension().string());

        fixTargetPicker->SetPath(path.string());
        fixOutputPicker->SetPath(output.string());
    }

    void OnFixStart( wxCommandEvent& event ) override;

    void OnClose(wxCloseEvent& event) override;

DECLARE_EVENT_TABLE()

public:
    /** Constructor */
    GUI(wxWindow *parent, std::shared_ptr<spdlog::logger> logger);
};

#endif // __GUI__
