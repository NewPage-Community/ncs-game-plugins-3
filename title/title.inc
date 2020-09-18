enum struct TitleData
{
    char title[MAX_TITLE_LENGTH];
    int type;
    bool isWaitingForPlayerTitle;
}

TitleData player[MAXPLAYERS+1];

#include "title/menu"
#include "title/player"
#include "title/cookie"
