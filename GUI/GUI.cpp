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
    fs::path morrowindPath = mMorrowindLoc->GetPath().utf8_string();
    fs::path iniPath;
    if (fs::exists(morrowindPath)) {
        iniPath = fs::path(mMorrowindLoc->GetPath().utf8_string()) / "Morrowind.ini";
    } else {
        auto dialog = wxFileDialog(
                this,
                "Select your Morrowind ini file",
                wxEmptyString,
                wxEmptyString,
                _("Morrowind ini file (*.ini)|*.ini|"),
                wxFD_OPEN
        );
        if (dialog.ShowModal() != wxID_OK) {
            return;
        }
        iniPath = dialog.GetPath().utf8_string();
        morrowindPath = iniPath.parent_path();
    }

    boost::property_tree::ptree pt;
    try {
        boost::property_tree::ini_parser::read_ini(iniPath, pt);
    } catch (boost::property_tree::ini_parser_error& e) {
        wxMessageBox("Could not load the ini file at: " + iniPath.string() + ". " + e.what(), wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    if (pt.find("Game Files") == pt.not_found()) {
        wxMessageBox("Could not find \"Game Files\" section in the ini file", wxT("Something went wrong"),
                     wxICON_ERROR);
        return;
    }

    std::vector<fs::path> gameFiles;
    for (int idx = 0;; ++idx) {
        auto iniKey = "Game Files.GameFile" + std::to_string(idx);
        auto file = pt.get_optional<std::string>(iniKey);
        if (!file.has_value()) {
            break;
        }
        auto filePath = morrowindPath / "Data Files" / file.value();
        if (!fs::exists(filePath)) {
            wxMessageBox(iniKey + "=" + file.value() + " was referenced in the ini file, but it doesn't exist at the path: " + filePath.string(), wxT("Something went wrong"),
                         wxICON_ERROR);
            return;
        }
        gameFiles.push_back(morrowindPath / "Data Files" / file.value());
    }

    loadOrder.clear();
    mModList->Clear();
    loadOrder.insert(gameFiles);
    for (const auto &item: loadOrder.get()) {
        mModList->Append(item->fileName);
    }
    assert(loadOrder.get().size() == mModList->GetCount());
}

void GUI::OnAddPress(wxCommandEvent &event) {
    auto dialog = wxFileDialog(
            this,
            wxASCII_STR(wxFileSelectorPromptStr),
            mMorrowindLoc->GetPath(),
            wxEmptyString,
            _("Morrowind plugin files (*.esp, *.esm)|*.esp;*.esm|"),
            wxFD_OPEN | wxFD_MULTIPLE
    );
    if (dialog.ShowModal() != wxID_OK) {
        return;
    }

    wxArrayString paths;
    dialog.GetPaths(paths);

    std::vector<std::filesystem::path> fsPaths;
    for (const auto &s: paths) {
        fsPaths.push_back(s.utf8_string());
    }
    loadOrder.insert(fsPaths);

    int i = 0;
    for (const auto &item: loadOrder.get()) {
        if (mModList->FindString(item->fileName) == -1) {
            mModList->Insert(item->fileName, i);
        }
        ++i;
    }
    assert(mModList->GetCount() == loadOrder.get().size());
}

void GUI::OnRemovePress(wxCommandEvent &event) {
    int sel = mModList->GetSelection();
    if (sel == wxNOT_FOUND) return;
    loadOrder.remove(mModList->GetString(sel).utf8_string());
    mModList->Delete(sel);
    assert(mModList->GetCount() == loadOrder.get().size());
}

void GUI::OnGenPress(wxCommandEvent &event) {
    fs::path basePath = fs::absolute(fs::path(mMorrowindLoc->GetPath().utf8_string()));

    std::vector<fs::path> files;
    for (const auto &item: loadOrder.get()) {
        files.push_back(item->path);
    }

    if (files.empty()) {
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

    std::thread thread(Generator::generate,
                       [&](int progress, const std::string& message) { return this->sendStatusUpdate(progress, message); },
                       [&]() { return this->sendSuccess(); },
                       [&](const std::string& message) { return this->sendFailure(message); },
                       iniPath,
                       files,
                       outPath,
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