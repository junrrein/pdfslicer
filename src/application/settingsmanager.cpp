// PDF Slicer
// Copyright (C) 2018 Julián Unrrein

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "settingsmanager.hpp"
#include <giomm/file.h>
#include <glibmm/miscutils.h>
#include <config.hpp>
#include <logger.hpp>

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

namespace misc {
    static const std::string groupName = "misc";

    static const struct {
        std::string zoomLevel = "zoom-level";
    } keys;

    static const unsigned int defaultZoomLevel = 0;
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
        WindowState result{};

        result.width = m_keyFile.get_integer(window_state::groupName, window_state::keys.width);
        result.height = m_keyFile.get_integer(window_state::groupName, window_state::keys.height);
        result.isMaximized = m_keyFile.get_boolean(window_state::groupName, window_state::keys.isMaximized);

        return result;
    }
    catch (const Glib::Error& e) {
        Logger::logWarning("Error while loading window state: " + e.what());

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
    catch (const Glib::Error& e) {
        Logger::logWarning("Error while writing window state: " + e.what());
    }
}

unsigned int SettingsManager::loadZoomLevel()
{
    try {
        return static_cast<unsigned>(m_keyFile.get_integer(misc::groupName, misc::keys.zoomLevel));
    }
    catch (const Glib::Error& e) {
        Logger::logWarning("Error while loading zoom level: " + e.what());

        return misc::defaultZoomLevel;
    }
}

void SettingsManager::saveZoomLevel(unsigned int zoomLevel)
{
    try {
        m_keyFile.set_integer(misc::groupName, misc::keys.zoomLevel, static_cast<int>(zoomLevel));
    }
    catch (const Glib::Error& e) {
        Logger::logWarning("Error while writing zoom level: " + e.what());
    }
}

void SettingsManager::loadConfigFile()
{
    try {
        m_keyFile.load_from_file(getSettingsFilePath());
    }
    catch (const Glib::Error& e) {
        Logger::logWarning("Error while loading config file: " + e.what());
    }
}

void SettingsManager::saveConfigFile()
{
    try {
        m_keyFile.save_to_file(getSettingsFilePath());
    }
    catch (const Glib::Error& e) {
        Logger::logWarning("Error while saving config file: " + e.what());
    }
}

std::string SettingsManager::getSettingsFilePath()
{
    return Glib::build_filename(config::getConfigDirPath(),
                                "settings.ini");
}

} // namespace Slicer
