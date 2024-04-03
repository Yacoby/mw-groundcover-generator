#include <wx/msgdlg.h>

#include "GUI.h"
#include "GenThread.h"
#include <vector>
#include <thread>
#include <filesystem>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#endif

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

namespace fs = std::filesystem;


BEGIN_EVENT_TABLE(GUI, wxFrame)
                EVT_MENU(WORKER_UPDATE, GUI::OnThreadStatusUpdate)
                EVT_MENU(WORKER_SUCCESS, GUI::OnThreadSuccess)
                EVT_MENU(WORKER_FAILURE, GUI::OnThreadFailure)
END_EVENT_TABLE()

GUI::GUI(wxWindow *parent) : GrassGen(parent) {
    const std::pair<const char*, const char *> registryPaths[] = {
            std::pair(R"(Software\Bethesda Softworks\Morrowind)", "Installed Path"),
            std::pair(R"(Software\Microsoft\Windows\CurrentVersion\Uninstall\Steam App 22320)", "InstallLocation"),
            std::pair(R"(Software\GOG.com\Games\1440163901)", "path"),
            std::pair(R"(Software\GOG.com\Games\1435828767)", "path"),
    };
    for (const auto &registryPath: registryPaths) {
        const auto standardKey = getRegKey(registryPath.first, registryPath.second);
        if (standardKey.has_value()) {
            const auto gamePath = fs::path(standardKey.value());
            if (fs::exists(gamePath) && fs::is_directory(gamePath)) {
                mMorrowindLoc->SetPath(standardKey.value());
                break;
            }
        }
    }
}

void GUI::OnImportPress(wxCommandEvent &event) {
    if (mMorrowindLoc->GetPath().length() == 0) {
        wxMessageBox("Morrowind location is not set", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    std::string iniPath = fs::absolute(fs::path(mMorrowindLoc->GetPath().utf8_string()) / "Morrowind.ini").string();

    boost::property_tree::ptree pt;
    try {
        boost::property_tree::ini_parser::read_ini(iniPath, pt);
    } catch (boost::property_tree::ini_parser_error& e) {
        wxMessageBox("Could not load the ini file at: " + iniPath + ". " + e.what(), wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    if (pt.find("Game Files") == pt.not_found()) {
        wxMessageBox("Could not find \"Game Files\" section in the ini file", wxT("Something went wrong"),
                     wxICON_ERROR);
        return;
    }

    mModList->Clear();

    for (int idx = 0;; ++idx) {
        auto file = pt.get_optional<std::string>("GameFile" + std::to_string(idx));
        if (!file.has_value()) {
            break;
        }
        mModList->Append(file.value());
    }
}

void GUI::OnAddPress(wxCommandEvent &event) {
    std::string s = wxFileSelector(
            wxFileSelectorPromptStr,
            mMorrowindLoc->GetPath()
    ).utf8_string();
    fs::path path = fs::path(s);
    if (!fs::exists(path) || !fs::is_regular_file(path)) {
        return;
    }

    std::string fileName = fs::path(s).filename().string();
    if (fileName.empty()) {
        return;
    }

    mModList->Append(fileName);
}

void GUI::OnRemovePress(wxCommandEvent &event) {
    int sel = mModList->GetSelection();
    if (sel == wxNOT_FOUND) return;
    mModList->Delete(sel);
}

void GUI::OnGenPress(wxCommandEvent &event) {
    fs::path basePath = fs::absolute(fs::path(mMorrowindLoc->GetPath().utf8_string()));

    std::vector<FileTime> files;
    for (unsigned int x = 0; x < mModList->GetCount(); x++) {
        std::string fileName = mModList->GetString(x).utf8_string();

        FileTime ft;
        ft.file = basePath / fs::path("Data Files") / fs::path(mModList->GetString(x).utf8_string());

        if (!fs::exists(ft.file)) {
            wxMessageBox("Could not find input file \"" + fileName + "\", expected to find it at " + ft.file.string(),
                         wxT("Something went wrong"), wxICON_ERROR);
            return;
        }

        ft.time = fs::last_write_time(ft.file);
        files.push_back(ft);
    }
    sort(files.begin(), files.end(), GUI::timeSort);

    std::vector<fs::path> vals;
    for (unsigned int i = 0; i < files.size(); i++) {
        vals.push_back(files[i].file);
    }

    if (vals.empty()) {
        wxMessageBox("There were no data files to search", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    fs::path iniPath = fs::absolute(fs::path(mIniLoc->GetPath().utf8_string()));
    if (!fs::exists(iniPath)) {
        wxMessageBox("Settings (ini) path " + iniPath.string() + " doesn'thread exist", wxT("Something went wrong"),
                     wxICON_ERROR);
        return;
    }

    fs::path outPath = fs::absolute(basePath / fs::path("Data Files") / fs::path(mOutputFile->GetValue().utf8_string()));
    if (!fs::exists(outPath.parent_path())) {
        wxMessageBox("Output path directory " + outPath.parent_path().string() +
                     " doesn'thread exist. Did you set the Morrowind location", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    int zOffset = std::stoi(mZOffset->GetValue().utf8_string());
    std::thread thread(Generator::generate,
                       [&](int progress, const std::string& message) { return this->sendStatusUpdate(progress, message); },
                       [&]() { return this->sendSuccess(); },
                       [&](const std::string& message) { return this->sendFailure(message); },
                       iniPath,
                       vals,
                       outPath,
                       mID->GetValue().utf8_string(),
                       zOffset,
                       time(nullptr));
    mGenerate->Enable(false);
    thread.detach();
}

std::optional<std::string> GUI::getRegKey(const char* pos, const char* name) {
#if (defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__))
    char value[8192];
    DWORD bufferSize = sizeof(value);
    int result = RegGetValueA(HKEY_LOCAL_MACHINE, pos, name, RRF_RT_REG_SZ, nullptr, value, &bufferSize);
    if ( result == ERROR_SUCCESS ){
        std::string strValue = std::string(value);
        if (!strValue.empty()) {
            return strValue;
        }
    }
    return std::nullopt;
#else
    return std::nullopt;
#endif
}