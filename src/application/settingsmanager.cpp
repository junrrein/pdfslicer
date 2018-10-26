#include "settingsmanager.hpp"
#include <giomm/file.h>
#include <glibmm/miscutils.h>
#include <config.hpp>

namespace Slicer {

namespace window_state {
    static const std::string groupName = "window-state";

    static const struct {
        std::string width = "width";
        std::string height = "height";
        std::string isMaximized = "is-maximized";
    } keys;

    static const WindowState defaultWindowState = {800, 600, false};
}

SettingsManager::SettingsManager()
{
    loadConfigFile();
}

SettingsManager::~SettingsManager()
{
    saveConfigFile();
}

WindowState SettingsManager::loadWindowState()
{
    try {
        WindowState result;

        result.width = m_keyFile.get_integer(window_state::groupName, window_state::keys.width);
        result.height = m_keyFile.get_integer(window_state::groupName, window_state::keys.height);
        result.isMaximized = m_keyFile.get_boolean(window_state::groupName, window_state::keys.isMaximized);

        return result;
    }
    catch (...) {
        return window_state::defaultWindowState;
    }
}

void SettingsManager::saveWindowState(const WindowState& windowState)
{
    try {
        m_keyFile.set_integer(window_state::groupName, window_state::keys.width, windowState.width);
        m_keyFile.set_integer(window_state::groupName, window_state::keys.height, windowState.height);
        m_keyFile.set_boolean(window_state::groupName, window_state::keys.isMaximized, windowState.isMaximized);
    }
    catch (...) {
    }
}

void SettingsManager::loadConfigFile()
{
    try {
        m_keyFile.load_from_file(getSettingsFilePath());
    }
    catch (...) {
    }
}

void SettingsManager::saveConfigFile()
{
    try {
        m_keyFile.save_to_file(getSettingsFilePath());
    }
    catch (...) {
    }
}

std::string SettingsManager::getSettingsFilePath()
{
    return Glib::build_filename(config::getConfigDirPath(),
                                "settings.ini");
}

} // namespace Slicer
