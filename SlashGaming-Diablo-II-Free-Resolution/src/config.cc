/**
 * SlashGaming Diablo II Free Resolution
 * Copyright (C) 2019  Mir Drualga
 *
 * This file is part of SlashGaming Diablo II Free Resolution.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published
 *  by the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *  Additional permissions under GNU Affero General Public License version 3
 *  section 7
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with Diablo II (or a modified version of that game and its
 *  libraries), containing parts covered by the terms of Blizzard End User
 *  License Agreement, the licensors of this Program grant you additional
 *  permission to convey the resulting work. This additional permission is
 *  also extended to any combination of expansions, mods, and remasters of
 *  the game.
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with any Graphics Device Interface (GDI), DirectDraw, Direct3D,
 *  Glide, OpenGL, or Rave wrapper (or modified versions of those
 *  libraries), containing parts not covered by a compatible license, the
 *  licensors of this Program grant you additional permission to convey the
 *  resulting work.
 *
 *  If you modify this Program, or any covered work, by linking or combining
 *  it with any library (or a modified version of that library) that links
 *  to Diablo II (or a modified version of that game and its libraries),
 *  containing parts not covered by a compatible license, the licensors of
 *  this Program grant you additional permission to convey the resulting
 *  work.
 */

#include "config.hpp"

#include <array>
#include <filesystem>
#include <fstream>
#include <mutex>
#include <string>

#include <fmt/format.h>
#include <mjsoni/rapid_json_config_reader.hpp>

namespace sgd2fr::config {
namespace {

// Global config entries.
constexpr std::string_view kGlobalEntryKey = "!!!Globals!!!";

constexpr std::string_view kConfigTabWidthKey = "Config Tab Width";
constexpr int kDefaultConfigTabWidth = 4;

// Main config entries.
constexpr std::string_view kMainEntryKey = "SlashGaming Diablo II Free Resolution";
constexpr std::string_view kMetaDataKey = "!!!Metadata (Do not modify)!!!";

// Note that this signifies the last version where the config formatting and
// entries were updated. These values do not need to change with respect to API
// file version!
constexpr std::string_view kMajorVersionAKey = "Major Version A";
constexpr int kMajorVersionAValue = 3;
constexpr std::string_view kMajorVersionBKey = "Major Version B";
constexpr int kMajorVersionBValue = 0;
constexpr std::string_view kMinorVersionAKey = "Minor Version A";
constexpr int kMinorVersionAValue = 0;
constexpr std::string_view kMinorVersionBKey = "Minor Version B";
constexpr int kMinorVersionBValue = 0;

// Resolution variables.
constexpr std::string_view kIngameResolutionsKey = "Ingame Resolutions";
constexpr std::array<std::string_view, 2> kDefaultIngameResolutions = {
  "640x480",
  "800x600"
};

std::map<std::string, std::once_flag> once_flags_by_json_keys;

const std::filesystem::path& GetConfigPath() {
  static std::filesystem::path kConfigPath = "SGD2FreeRes.json";
  return kConfigPath;
}

template <typename ...Args>
std::once_flag& GetOnceFlag(const Args&... json_keys) {
  // Calculate concat key size.
  std::size_t concat_key_size = 0;
  for (const auto& json_key : { json_keys...}) {
    concat_key_size += json_key.length() + 1;
  }

  // Concat all keys together.
  std::string concat_key(concat_key_size, '\0');

  std::size_t i = 0;
  for (const auto& json_key : { json_keys...}) {
    concat_key += json_key;
    concat_key += static_cast<char>(i + 1);
    
    i = (i + 1) % 0x32;
  }

  return once_flags_by_json_keys[std::move(concat_key)];
}

bool AddMissingConfigEntries(
    mjsoni::RapidJsonConfigReader& config_reader
) {
  // Check that the actual config version is less than or equal to the expected
  // config version. If the actual is larger, then do not add any new entries.
  // If there are any breaking config changes, then the program will most
  // likely crash.
  int major_version_a = config_reader.GetIntOrDefault(
      std::numeric_limits<int>::min(),
      kMainEntryKey,
      kMetaDataKey,
      kMajorVersionAKey
  );

  if (major_version_a < kMajorVersionAValue) {
    major_version_a = kMajorVersionAValue;

    config_reader.SetDeepInt(
        kMajorVersionAValue,
        kMainEntryKey,
        kMetaDataKey,
        kMajorVersionAKey
    );

    config_reader.SetDeepInt(
        std::numeric_limits<int>::min(),
        kMainEntryKey,
        kMetaDataKey,
        kMajorVersionBKey
    );

    config_reader.SetDeepInt(
        std::numeric_limits<int>::min(),
        kMainEntryKey,
        kMetaDataKey,
        kMinorVersionAKey
    );

    config_reader.SetDeepInt(
        std::numeric_limits<int>::min(),
        kMainEntryKey,
        kMetaDataKey,
        kMinorVersionBKey
    );
  } else if (major_version_a > kMajorVersionAValue) {
    return true;
  }

  int major_version_b = config_reader.GetIntOrDefault(
      std::numeric_limits<int>::min(),
      kMetaDataKey,
      kMajorVersionBKey
  );

  if (major_version_b < kMajorVersionBValue) {
    major_version_b = kMajorVersionBValue;

    config_reader.SetDeepInt(
        kMajorVersionBValue,
        kMainEntryKey,
        kMetaDataKey,
        kMajorVersionBKey
    );

    config_reader.SetDeepInt(
        std::numeric_limits<int>::min(),
        kMainEntryKey,
        kMetaDataKey,
        kMinorVersionAKey
    );

    config_reader.SetDeepInt(
        std::numeric_limits<int>::min(),
        kMainEntryKey,
        kMetaDataKey,
        kMinorVersionBKey
    );
  } else if (major_version_b > kMajorVersionBValue) {
    return true;
  }

  int minor_version_a = config_reader.GetIntOrDefault(
      std::numeric_limits<int>::min(),
      kMainEntryKey,
      kMetaDataKey,
      kMinorVersionAKey
  );

  if (minor_version_a < kMinorVersionAValue) {
    minor_version_a = kMinorVersionAValue;

    config_reader.SetDeepInt(
        kMinorVersionAValue,
        kMainEntryKey,
        kMetaDataKey,
        kMinorVersionAKey
    );

    config_reader.SetDeepInt(
        std::numeric_limits<int>::min(),
        kMainEntryKey,
        kMetaDataKey,
        kMinorVersionBKey
    );
  } else if (minor_version_a > kMinorVersionAValue) {
    return true;
  }

  int minor_version_b = config_reader.GetIntOrDefault(
      std::numeric_limits<int>::min(),
      kMainEntryKey,
      kMetaDataKey,
      kMinorVersionBKey
  );

  if (minor_version_b < kMinorVersionBValue) {
    minor_version_b = kMinorVersionBValue;

    config_reader.SetDeepInt(
        kMinorVersionBValue,
        kMainEntryKey,
        kMetaDataKey,
        kMinorVersionBKey
    );
  } else if (minor_version_b > kMinorVersionBValue) {
    return true;
  }

  // The user's config version is less or equal, so add defaults if missing.
  if (!config_reader.HasInt(kGlobalEntryKey, kConfigTabWidthKey)) {
    config_reader.SetDeepInt(
        kDefaultConfigTabWidth,
        kGlobalEntryKey,
        kConfigTabWidthKey
    );
  }

  if (!config_reader.HasVector(kMainEntryKey, kIngameResolutionsKey)) {
    config_reader.SetDeepArray(
        kDefaultIngameResolutions.cbegin(),
        kDefaultIngameResolutions.cend(),
        kMainEntryKey,
        kIngameResolutionsKey
    );
  }

  // Write to the config file any new default values.
  int indent_width = config_reader.GetInt(
      kGlobalEntryKey,
      kConfigTabWidthKey
  );

  return config_reader.Write(indent_width);
}

mjsoni::RapidJsonConfigReader ReadConfig(
    const std::filesystem::path& config_file_path
) {
  // Read the config file for processing, if all other conditions are good.
  mjsoni::RapidJsonConfigReader config_reader(config_file_path);

  bool is_read = config_reader.Read();
  if (!is_read) {
    std::wstring failed_to_read_message = fmt::format(
        L"Failed to read config in: {}",
        config_file_path.wstring().data()
    );

    MessageBoxW(
        nullptr,
        failed_to_read_message.data(),
        L"Failed to Read Config",
        MB_OK | MB_ICONERROR
    );

    std::exit(0);
  }

  bool is_missing_entry_added = AddMissingConfigEntries(config_reader);
  if (!is_missing_entry_added) {
    std::wstring failed_to_read_message = fmt::format(
        L"Failed add missing entries to the config in: {}",
        config_file_path.wstring().data()
    );

    MessageBoxW(
        nullptr,
        failed_to_read_message.data(),
        L"Failed to Add Missing Entries to Config",
        MB_OK | MB_ICONERROR
    );

    std::exit(0);
  }

  return config_reader;
}

mjsoni::RapidJsonConfigReader& GetConfigReader() {
  static mjsoni::RapidJsonConfigReader config_reader =
      ReadConfig(GetConfigPath());
  return config_reader;
}

} // namespace

const std::vector<std::tuple<int, int>>& GetIngameResolutions() {
  static std::vector<std::tuple<int, int>> ingame_resolutions;

  std::call_once(
      GetOnceFlag(kMainEntryKey, kIngameResolutionsKey),
      [=] () {
        std::vector<std::string> ingame_resolutions_str = GetConfigReader()
            .GetVector<std::string>(
                kMainEntryKey,
                kIngameResolutionsKey
            );

        std::set<std::tuple<int, int>> sorted_distinct_ingame_resolutions;

        for (const std::string& resolution_str : ingame_resolutions_str) {
          std::string::size_type split_index = resolution_str.find('x');

          int resolution_width = std::stoi(
              resolution_str.substr(0, split_index)
          );

          int resolution_height = std::stoi(
              resolution_str.substr(split_index + 1)
          );

          sorted_distinct_ingame_resolutions.insert(
              std::make_tuple(resolution_width, resolution_height)
          );
        }

        std::move(
            sorted_distinct_ingame_resolutions.begin(),
            sorted_distinct_ingame_resolutions.end(),
            ingame_resolutions.begin()
        );
      }
  );

  return ingame_resolutions;
}

bool LoadConfig() {
  GetConfigReader() = ReadConfig(GetConfigPath());
  once_flags_by_json_keys.clear();

  return true;
}

} // namespace sgd2fr::config