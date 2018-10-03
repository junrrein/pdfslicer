#ifndef SETTINGSMANAGER_HPP
#define SETTINGSMANAGER_HPP

#include <glibmm/keyfile.h>

namespace Slicer {

struct WindowState {
    int width;
    int height;
    bool isMaximized;
};

class SettingsManager {
public:
    SettingsManager();
    ~SettingsManager();

    WindowState loadWindowState();
    void saveWindowState(const WindowState& windowState);

private:
    Glib::KeyFile m_keyFile;
    bool wasSuccessfullyLoaded = false;

    void loadConfigFile();
    void saveConfigFile();

    std::string getSettingsParentPath() const;
    std::string getSettingsFilePath() const;
};

} // namespace Slicer

#endif // SETTINGSMANAGER_HPP
