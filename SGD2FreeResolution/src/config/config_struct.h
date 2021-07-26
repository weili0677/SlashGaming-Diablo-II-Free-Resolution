/**
 * SlashGaming Diablo II Free Resolution
 * Copyright (C) 2019-2021  Mir Drualga
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

#ifndef SGD2FR_CONFIG_CONFIG_STRUCT_H_
#define SGD2FR_CONFIG_CONFIG_STRUCT_H_

#include <stddef.h>

#include <mdc/std/wchar.h>

#define CONFIG_METADATA "Metadata"

#define CONFIG_METADATA_VERSION "Version"

#define CONFIG_METADATA_VERSION_MAJOR_HIGH "Major High"
#define CONFIG_METADATA_VERSION_MAJOR_HIGH_DEFAULT 3
#define CONFIG_METADATA_VERSION_MAJOR_LOW "Major Low"
#define CONFIG_METADATA_VERSION_MAJOR_LOW_DEFAULT 0
#define CONFIG_METADATA_VERSION_MINOR_HIGH "Minor High"
#define CONFIG_METADATA_VERSION_MINOR_HIGH_DEFAULT 2
#define CONFIG_METADATA_VERSION_MINOR_LOW "Minor Low"
#define CONFIG_METADATA_VERSION_MINOR_LOW_DEFAULT 1

#define CONFIG_INGAME_RESOLUTION_MODE "Ingame Resolution Mode"
#define CONFIG_INGAME_RESOLUTION_MODE_DEFAULT 3
#define CONFIG_INGAME_RESOLUTIONS "Ingame Resolutions"

#define CONFIG_MAIN_MENU_RESOLUTION "Main Menu Resolution"
#define CONFIG_MAIN_MENU_RESOLUTION_DEFAULT { 800, 600 }

#define CONFIG_IS_ENABLE_SCREEN_BORDER_FRAME "Enable Screen Border Frame?"
#define CONFIG_IS_ENABLE_SCREEN_BORDER_FRAME_DEFAULT 0
#define CONFIG_IS_USE_ORIGINAL_SCREEN_BORDER_FRAME \
    "Use Original Screen Border Frame?"
#define CONFIG_IS_USE_ORIGINAL_SCREEN_BORDER_FRAME_DEFAULT 0
#define CONFIG_IS_USE_800_INTERFACE_BAR "Use 800 Interface Bar?"
#define CONFIG_IS_USE_800_INTERFACE_BAR_DEFAULT 0

#define CONFIG_CUSTOM_MPQ_PATH "Custom MPQ Path"
#define CONFIG_CUSTOM_MPQ_PATH_DEFAULT "SGD2FreeRes.mpq"

struct GameResolution {
  int width;
  int height;
};

struct Config_Implmentation;

struct Config {
  struct Config_Metadata {
    struct Config_Metadata_Version {
      int major_high;
      int major_low;
      int minor_high;
      int minor_low;
    } version;
  } metadata;

  struct Config_Implmentation* impl;

  const wchar_t* custom_mpq_path;

  size_t ingame_resolution_mode;

  size_t ingame_resolutions_count;
  const GameResolution* ingame_resolutions;

  int is_enable_screen_border_frame;
  int is_use_original_screen_border_frame;
  int is_use_800_interface_bar;

  struct GameResolution main_menu_resolution;
};

#endif /* SGD2FR_CONFIG_CONFIG_STRUCT_H_ */
