#include "GameData.h"
#include "main.h"


GameData::CharacterData *GameData::GetCharData(ForeignClass pChar)
{
    for (const auto& ch : GetMain()->GetGameData()->objData.charDataList) {
        if (ch && pChar == ch->pCharacter) {
            return ch.get();
        }
    }
    return NULL;
}