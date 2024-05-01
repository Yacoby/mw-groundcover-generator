
#include "GUI.h"
#include <vector>
#include <thread>
#include <filesystem>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
#include <windows.h>
#endif

#include <wx/msgdlg.h>

#include <boost/property_tree/ini_parser.hpp>

#include "logic/GameConfiguration.h"
#include "logic/FixPosition.h"
#include "logic/Generator.h"

namespace fs = std::filesystem;


BEGIN_EVENT_TABLE(GUI, wxFrame)
                EVT_MENU(GEN_WORKER_UPDATE, GUI::OnThreadStatusUpdate)
                EVT_MENU(GEN_WORKER_SUCCESS, GUI::OnThreadSuccess)
                EVT_MENU(GEN_WORKER_FAILURE, GUI::OnThreadFailure)

                EVT_MENU(REGEN_WORKER_UPDATE, GUI::OnThreadStatusUpdate)
                EVT_MENU(REGEN_WORKER_SUCCESS, GUI::OnThreadSuccess)
                EVT_MENU(REGEN_WORKER_FAILURE, GUI::OnThreadFailure)

                EVT_MENU(FIX_WORKER_UPDATE, GUI::OnThreadStatusUpdate)
                EVT_MENU(FIX_WORKER_SUCCESS, GUI::OnThreadSuccess)
                EVT_MENU(FIX_WORKER_FAILURE, GUI::OnThreadFailure)
END_EVENT_TABLE()

GUI::GUI(wxWindow *parent, std::shared_ptr<spdlog::logger> logger) : GrassGen(parent), logger(logger), loadOrder(std::unique_ptr<LastModifiedLoadOrder>(new LastModifiedLoadOrder())) {
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
                logger->info(R"(Found Morrowind at "{}" from ini "{}")", gamePath.string(), standardKey.value());
                morrowindDirectory = gamePath;
                break;
            }
        }
    }

    if (!morrowindDirectory.empty()) {
        mOutputFile->SetPath((morrowindDirectory / "Data Files" / "Grass.esp").string());
    }

    boost::property_tree::ptree settings;
    try {
        boost::property_tree::ini_parser::read_ini(SETTINGS_FILE, settings);
    } catch (boost::property_tree::ini_parser_error& e) {
        logger->error("Failed to read ini, got error: {}", e.what());
    }

    auto genConfigLocation = settings.get_optional<std::string>("Generate.Configuration");
    if (genConfigLocation.has_value() && fs::exists(fs::path(genConfigLocation.value()))) {
        mIniLoc->SetPath(genConfigLocation.value());
    }
    auto genConfig = settings.get_optional<std::string>("Generate.Output");
    if (genConfig.has_value()) {
        mOutputFile->SetPath(genConfig.value());
    }

    auto regenConfig = settings.get_optional<std::string>("Regenerate.Configuration");
    if (regenConfig.has_value() && fs::exists(fs::path(regenConfig.value()))) {
        regenerateConfigurationPicker->SetPath(regenConfig.value());
    }

}

void GUI::OnClose(wxCloseEvent& event) {
    boost::property_tree::ptree settings;
    settings.add("Generate.Configuration", mIniLoc->GetPath().utf8_string());
    settings.add("Generate.Output", mOutputFile->GetPath().utf8_string());
    settings.add("Regenerate.Configuration", regenerateConfigurationPicker->GetPath().utf8_string());

    try {
        boost::property_tree::ini_parser::write_ini(SETTINGS_FILE, settings);
    } catch (boost::property_tree::ini_parser_error& e) {
        logger->error("Failed to write ini, got error: {}", e.what());
    }

    try {
        logger->flush();
    } catch (std::exception& e) {

    }

    event.Skip();
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
    mModList->Clear();
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
    wxArrayInt selections;
    mModList->GetSelections(selections);

    // sort by descending index
    selections.Sort([](int* a, int* b) {
        if (*a > *b) { return -1; }
        else if (*a < *b) { return 1; }
        else { return 0; }
    });

    for (const auto& sel: selections) {
        loadOrder->remove(mModList->GetString(sel).utf8_string());
        mModList->Delete(sel);
    }
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
        wxMessageBox("There are no data files", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    fs::path iniPath = fs::absolute(fs::path(mIniLoc->GetPath().utf8_string()));
    if (!fs::exists(iniPath)) {
        wxMessageBox("Settings (ini) path " + iniPath.string() + " doesn't exist", wxT("Something went wrong"),
                     wxICON_ERROR);
        return;
    }

    fs::path outPath = fs::absolute(mOutputFile->GetPath().utf8_string());
    if (!fs::exists(outPath.parent_path())) {
        wxMessageBox("Output path directory " + outPath.parent_path().string() +
                     " doesn't exist. Did you set it correctly?", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    mGenerate->Enable(false);
    std::thread thread([=, this] {
        Generator generator(
                logger,
                [&](int progress, const std::string& message) { return this->sendStatusUpdate(GEN_WORKER_BASE_EVT_ID, progress, message); },
                [&](const std::string& message) { return this->sendSuccess(GEN_WORKER_BASE_EVT_ID, message); },
                [&](const std::exception& exception) { return this->sendFailure(GEN_WORKER_BASE_EVT_ID, exception); },
                iniPath,
                files,
                outPath,
                time(nullptr));
        generator.generate();
    });
    thread.detach();
}

void GUI::OnRegenerateStart(wxCommandEvent &event) {
    std::vector<fs::path> files;
    for (const auto &item: loadOrder->get()) {
        files.push_back(item->path);
    }

    fs::path iniPath = fs::absolute(fs::path(regenerateConfigurationPicker->GetPath().utf8_string()));
    if (!fs::exists(iniPath)) {
        wxMessageBox("Settings (ini) path " + iniPath.string() + " doesn't exist", wxT("Something went wrong"),
                     wxICON_ERROR);
        return;
    }

    if (regenerateTargetPicker->GetPath().empty()) {
        wxMessageBox("Target not set", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    if (regenerateOutputPicker->GetPath().empty()) {
        wxMessageBox("Output not set", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    if (!fs::exists(fs::path(regenerateTargetPicker->GetPath().utf8_string()))) {
        wxMessageBox(std::format("Target file ({}) does not exist", regenerateTargetPicker->GetPath().utf8_string()), wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    auto plugin = fs::path(regenerateTargetPicker->GetPath().utf8_string());
    auto output = fs::path(regenerateOutputPicker->GetPath().utf8_string());;

    if (fs::exists(output)) {
        auto dialog = wxMessageDialog(
                this,
                "Output file already exists. Overwrite?",
                "Overwrite existing file",
                wxOK | wxCANCEL
        );
        if (dialog.ShowModal() != wxID_OK) {
            return;
        }
    }

    RegenerateOptions regenerateOptions;
    for (size_t i = 0; i < regenerateBasePlugins->GetCount(); ++i ){
        regenerateOptions.basePlugins.insert(regenerateBasePlugins->GetString(i).utf8_string());
    }
    regenerateOptions.regenerateIfFloatingGroundcover = regenerateIfFloatingCheckbox->GetValue();
    regenerateOptions.regenerateWhenEmpty = regenerateIfEmpty->GetValue();

    regenerateButton->Enable(false);
    std::thread thread([=, this] {
        Generator generator(
                logger,
                [&](int progress, const std::string& message) { return this->sendStatusUpdate(REGEN_WORKER_BASE_EVT_ID, progress, message); },
                [&](const std::string& message) { return this->sendSuccess(REGEN_WORKER_BASE_EVT_ID, message); },
                [&](const std::exception& exception) { return this->sendFailure(REGEN_WORKER_BASE_EVT_ID, exception); },
                iniPath,
                files,
                output,
                time(nullptr));
        generator.generateFromExisting(plugin, regenerateOptions);
    });
    thread.detach();
}


void GUI::OnFixStart( wxCommandEvent& event ) {
    std::vector<fs::path> files;
    for (const auto &item: loadOrder->get()) {
        files.push_back(item->path);
    }

    if (fixTargetPicker->GetPath().empty()) {
        wxMessageBox("Target not set", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    if (fixOutputPicker->GetPath().empty()) {
        wxMessageBox("Output not set", wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    if (!fs::exists(fs::path(fixTargetPicker->GetPath().utf8_string()))) {
        wxMessageBox(std::format("Target file ({}) does not exist", fixTargetPicker->GetPath().utf8_string()), wxT("Something went wrong"), wxICON_ERROR);
        return;
    }

    auto operationIsDelete = fixOptions->GetSelection() == 1;

    auto plugin = fs::path(fixTargetPicker->GetPath().utf8_string());
    auto output = fs::path(fixOutputPicker->GetPath().utf8_string());;

    if (fs::exists(output)) {
        auto dialog = wxMessageDialog(
                this,
                "Output file already exists. Overwrite?",
                "Overwrite existing file",
                wxOK | wxCANCEL
        );
        if (dialog.ShowModal() != wxID_OK) {
            return;
        }
    }

    fixButton->Enable(false);
    std::thread thread([=, this] {
        PositionUpdater positionUpdater(
                logger,
                [&](int progress, const std::string& message) { return this->sendStatusUpdate(FIX_WORKER_BASE_EVT_ID, progress, message); },
                [&](const std::string& message) { return this->sendSuccess(FIX_WORKER_BASE_EVT_ID, message); },
                [&](const std::exception& exception) { return this->sendFailure(FIX_WORKER_BASE_EVT_ID, exception); },
                files,
                plugin,
                output
        );

        positionUpdater.fix(operationIsDelete);
    });
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

std::vector<std::string> GUI::getNestedExceptionMessages(const std::exception& e) {
    std::vector<std::string> messages;
    messages.push_back(e.what());
    try {
        std::rethrow_if_nested(e);
    } catch(const std::exception& ne) {
        auto subMessages = getNestedExceptionMessages(ne);
        messages.insert(messages.end(), subMessages.begin(), subMessages.end());
    } catch(...) {

    }
    return messages;
}
