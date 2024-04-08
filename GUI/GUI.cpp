#include <wx/msgdlg.h>

#include "GUI.h"
#include "GenThread.h"
#include <vector>
#include <thread>
#include <filesystem>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#endif

#include <boost/property_tree/ini_parser.hpp>

#include "GameConfiguration.h"

namespace fs = std::filesystem;


BEGIN_EVENT_TABLE(GUI, wxFrame)
                EVT_MENU(WORKER_UPDATE, GUI::OnThreadStatusUpdate)
                EVT_MENU(WORKER_SUCCESS, GUI::OnThreadSuccess)
                EVT_MENU(WORKER_FAILURE, GUI::OnThreadFailure)
END_EVENT_TABLE()

GUI::GUI(wxWindow *parent) : GrassGen(parent), loadOrder(std::unique_ptr<LastModifiedLoadOrder>(new LastModifiedLoadOrder())) {
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
                morrowindDirectory = gamePath;
                break;
            }
        }
    }

    if (!morrowindDirectory.empty()) {
        mOutputFile->SetPath((morrowindDirectory / "Data Files" / "Grass.esp").string());
    }
}

void GUI::OnImportPress(wxCommandEvent &event) {
    fs::path iniPath;
    if (fs::exists(morrowindDirectory)) {
        iniPath = morrowindDirectory / "Morrowind.ini";
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
        morrowindDirectory = iniPath.parent_path();
    }

    std::vector<std::string> plugins;
    try {
        plugins = loadMwIni(iniPath);
    } catch (boost::property_tree::ini_parser_error& e) {
        wxMessageBox("Could not load the ini file at: " + iniPath.string() + ". " + e.what(), wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    std::vector<fs::path> pluginPaths;
    for (const auto &plugin: plugins) {
        auto filePath = morrowindDirectory / "Data Files" / plugin;
        if (!fs::exists(filePath)) {
            wxMessageBox(plugin + " was referenced in the ini file, but it doesn't exist at the path: " + filePath.string(), wxT("Something went wrong"),
                         wxICON_ERROR);
            return;
        }
        pluginPaths.push_back(morrowindDirectory / "Data Files" / plugin);
    }

    mModList->Clear();
    loadOrder = std::unique_ptr<LoadOrder>(new LastModifiedLoadOrder());
    mAddPluginFromFile->Enable();
    loadOrder->insert(pluginPaths);
    for (const auto &item: loadOrder->get()) {
        mModList->Append(item->fileName);
    }
    assert(loadOrder->get().size() == mModList->GetCount());
}

void GUI::OnImportOpenMwPress(wxCommandEvent &event) {
    auto cfgPath = getOpenMwCfgPath();
    if (cfgPath.empty()) {
        auto dialog = wxFileDialog(
                this,
                "Select your OpenMW openmw.cfg",
                wxEmptyString,
                wxEmptyString,
                _("OpenMW openmw.cfg  (*.cfg)|*.cfg|"),
                wxFD_OPEN
        );
        if (dialog.ShowModal() != wxID_OK) {
            return;
        }
        cfgPath = dialog.GetPath().utf8_string();
    }

    auto cfg = loadOpenMwCfg(cfgPath);
    auto paths = filterSupportedPlugins(resolveOpenMwPluginPaths(cfg));

    loadOrder = std::unique_ptr<LoadOrder>(new FixedLoadOrder());
    mAddPluginFromFile->Enable(false);
    loadOrder->insert(paths);
    for (const auto &item: loadOrder->get()) {
        mModList->Append(item->fileName);
    }
    assert(mModList->GetCount() == loadOrder->get().size());
}

void GUI::OnAddPress(wxCommandEvent &event) {
    auto dialog = wxFileDialog(
            this,
            wxASCII_STR(wxFileSelectorPromptStr),
            morrowindDirectory.string(),
            wxEmptyString,
            _("Morrowind plugin files (*.esp, *.esm, *.omwaddon)|*.esp;*.esm;*.omwaddon|"),
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
    loadOrder->insert(fsPaths);

    int i = 0;
    for (const auto &item: loadOrder->get()) {
        if (mModList->FindString(item->fileName) == -1) {
            mModList->Insert(item->fileName, i);
        }
        ++i;
    }
    assert(mModList->GetCount() == loadOrder->get().size());
}

void GUI::OnRemovePress(wxCommandEvent &event) {
    int sel = mModList->GetSelection();
    if (sel == wxNOT_FOUND) return;
    loadOrder->remove(mModList->GetString(sel).utf8_string());
    mModList->Delete(sel);
    assert(mModList->GetCount() == loadOrder->get().size());
}

void GUI::OnResetPress(wxCommandEvent &event) {
    loadOrder = std::unique_ptr<LoadOrder>(new LastModifiedLoadOrder());
    mAddPluginFromFile->Enable();
    mModList->Clear();
    assert(mModList->GetCount() == loadOrder->get().size());
}


void GUI::OnGenPress(wxCommandEvent &event) {
    std::vector<fs::path> files;
    for (const auto &item: loadOrder->get()) {
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

    fs::path outPath = fs::absolute(mOutputFile->GetPath().utf8_string());
    if (!fs::exists(outPath.parent_path())) {
        wxMessageBox("Output path directory " + outPath.parent_path().string() +
                     " doesn't exist. Did you set it correctly?", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    std::thread thread(Generator::generate,
                       [&](int progress, const std::string& message) { return this->sendStatusUpdate(progress, message); },
                       [&](int duration) { return this->sendSuccess(duration); },
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