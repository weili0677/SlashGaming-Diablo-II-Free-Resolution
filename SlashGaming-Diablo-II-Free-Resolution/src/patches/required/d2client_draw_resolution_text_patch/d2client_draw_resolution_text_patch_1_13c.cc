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

#include "d2client_draw_resolution_text_patch_1_13c.hpp"

#include "../../../asm_x86_macro.h"
#include "d2client_draw_resolution_text.hpp"

namespace sgd2fr::patches::d2client {
namespace {

__declspec(naked) void __cdecl InterceptionFunc01() {
  ASM_X86(push ebp);
  ASM_X86(mov ebp, esp);

  // Original code
  ASM_X86(add edx, 230);

  ASM_X86(push eax);
  ASM_X86(push ecx);
  ASM_X86(push edx);

  ASM_X86(push ebx);
  ASM_X86(push edx);
  ASM_X86(push esi);
  ASM_X86(push eax);
  ASM_X86(call ASM_X86_FUNC(Sgd2fr_D2Client_DrawResolutionText));
  ASM_X86(add esp, 16);

  // If the correct pieces executed, then remove the pushed arguments, then
  // set the return address to a place after the original draw function call.
  ASM_X86(test eax, eax);

  ASM_X86(pop edx);
  ASM_X86(pop ecx);
  ASM_X86(pop eax);

  ASM_X86(leave);

  ASM_X86(jnz DidDrawUnicodeText);
  ASM_X86(ret);

DidDrawUnicodeText:
  ASM_X86(add dword ptr [esp], 6);
  ASM_X86(ret 16);
}

} // namespace

DrawResolutionTextPatch_1_13C::DrawResolutionTextPatch_1_13C()
  : patches_(MakePatches()) {
}

void DrawResolutionTextPatch_1_13C::Apply() {
  for (auto& patch : this->patches_) {
    patch.Apply();
  }
}

void DrawResolutionTextPatch_1_13C::Remove() {
  for (auto& patch : this->patches_) {
    patch.Apply();
  }
}

std::vector<mapi::GamePatch>
DrawResolutionTextPatch_1_13C::MakePatches() {
  std::vector<mapi::GamePatch> patches;

  PatchAddressAndSize patch_address_and_size_01 =
      GetPatchAddressAndSize01();
  patches.push_back(
      mapi::GamePatch::MakeGameBranchPatch(
          patch_address_and_size_01.first,
          mapi::BranchType::kCall,
          &InterceptionFunc01,
          patch_address_and_size_01.second
      )
  );

  return patches;
}

DrawResolutionTextPatch_1_13C::PatchAddressAndSize
DrawResolutionTextPatch_1_13C::GetPatchAddressAndSize01() {
  ::d2::GameVersion running_game_version = ::d2::game_version::GetRunning();

  switch (running_game_version) {
    case ::d2::GameVersion::k1_13C: {
      return PatchAddressAndSize(
        ::mapi::GameAddress::FromOffset(
            ::d2::DefaultLibrary::kD2Client,
            0x65407
        ),
        0x6540D - 0x65407
      );
    }
  }
}

} // namespace sgd2fr::patches::d2client