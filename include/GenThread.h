#include "GUI.h"

#pragma once

namespace fs = boost::filesystem;

class GenThread : public wxThread {
    std::string mOut, mIdBase, mIniLoc;
    int mOffset;
    std::vector<std::string> mFiles;
    GUI *mGUI;

    std::string getMesh(const std::list<GrassIni2::GrassMesh> &meshList, const std::string &cat);

    void sendStatusUpdate(int progressPercent, const std::string &message);

    void sendSuccess();

    void sendFailure(const std::string &message);

protected:
    ExitCode Entry();

    ExitCode Generate();
public:

    GenThread(GUI *gui,
              const std::string &out,
              const std::string &idBase,
              const std::string &iniLoc,
              std::vector<std::string> files,
              int offset);
};
