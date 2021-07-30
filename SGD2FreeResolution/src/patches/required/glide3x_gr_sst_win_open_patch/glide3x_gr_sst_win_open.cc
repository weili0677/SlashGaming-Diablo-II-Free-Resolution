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

#include "glide3x_gr_sst_win_open.hpp"

#include <sgd2mapi.hpp>
#include "../../../game_resolution/game_resolution.hpp"
#include "../../../sgd2mapi_extension/sgd2mapi_extension.hpp"

namespace sgd2fr::patches {
namespace {

static constexpr const wchar_t* kGlide3xPath = L"glide3x.dll";

} // namespace

void __cdecl Sgd2fr_Glide3x_SetWindowWidthAndHeight(
    uint32_t glide_resolution_mode,
    int32_t* width,
    int32_t* height
) {
  GameResolution resolution = GetIngameResolutionFromId(
      glide_resolution_mode - (0x1000 - 3)
  );

  switch (::d2::glide3x_library_version::GetRunning()) {
    case ::d2::glide3x_library_version::kSven1_4_4_21: {
      width = *reinterpret_cast<int32_t**>(
          ::mapi::GameAddress::FromOffset(kGlide3xPath, 0x1C9A0).raw_address()
      );
      height = *reinterpret_cast<int32_t**>(
          ::mapi::GameAddress::FromOffset(kGlide3xPath, 0x1C82C).raw_address()
      );

      break;
    }

    case ::d2::glide3x_library_version::kSven1_4_6_1: {
      width = *reinterpret_cast<int32_t**>(
          ::mapi::GameAddress::FromOffset(kGlide3xPath, 0x1C870).raw_address()
      );
      height = *reinterpret_cast<int32_t**>(
          ::mapi::GameAddress::FromOffset(kGlide3xPath, 0x1C830).raw_address()
      );

      break;
    }

    case ::d2::glide3x_library_version::kSven1_4_8_3: {
      width = *reinterpret_cast<int32_t**>(
          ::mapi::GameAddress::FromOffset(kGlide3xPath, 0x1D870).raw_address()
      );
      height = *reinterpret_cast<int32_t**>(
          ::mapi::GameAddress::FromOffset(kGlide3xPath, 0x1D830).raw_address()
      );

      break;
    }

    case ::d2::glide3x_library_version::kNGlide3_10_0_658: {
      width = reinterpret_cast<int32_t*>(
          ::mapi::GameAddress::FromOffset(kGlide3xPath, 0x169DA4).raw_address()
      );
      height = reinterpret_cast<int32_t*>(
          ::mapi::GameAddress::FromOffset(kGlide3xPath, 0x169F04).raw_address()
      );

      break;
    }
  }

  *width = resolution.width;
  *height = resolution.height;
}

} // namespace sgd2fr::patches