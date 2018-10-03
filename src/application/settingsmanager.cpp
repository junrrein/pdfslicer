#include "settingsmanager.hpp"
#include <giomm/file.h>
#include <glibmm/miscutils.h>
#include <config.hpp>

static const std::string windowStateGroup = "window-state";

namespace Slicer {

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
    if (!wasSuccessfullyLoaded)
        return {800, 600, false};

    WindowState result;

    result.width = m_keyFile.get_integer(windowStateGroup, "window-width");
    result.height = m_keyFile.get_integer(windowStateGroup, "window-height");
    result.isMaximized = m_keyFile.get_boolean(windowStateGroup, "is-maximized");

    return result;
}

void SettingsManager::saveWindowState(const WindowState& windowState)
{
    m_keyFile.set_integer(windowStateGroup, "window-width", windowState.width);
    m_keyFile.set_integer(windowStateGroup, "window-height", windowState.height);
    m_keyFile.set_boolean(windowStateGroup, "is-maximized", windowState.isMaximized);
}

void SettingsManager::loadConfigFile()
{
    try {
        m_keyFile.load_from_file(getSettingsFilePath());
        wasSuccessfullyLoaded = true;
    }
    catch (...) {
    }
}

void SettingsManager::saveConfigFile()
{
    Glib::RefPtr<Gio::File> settingsDirectory = Gio::File::create_for_path(getSettingsParentPath());

    if (!settingsDirectory->query_exists())
        settingsDirectory->make_directory_with_parents();

    m_keyFile.save_to_file(getSettingsFilePath());
}

std::string SettingsManager::getSettingsParentPath() const
{
    return Glib::build_filename(std::vector<std::string>{Glib::get_user_cache_dir(),
                                                         config::APPLICATION_ID});
}

std::string SettingsManager::getSettingsFilePath() const
{
    return Glib::build_filename(std::vector<std::string>{getSettingsParentPath(),
                                                         "settings.ini"});
}

} // namespace Slicer
