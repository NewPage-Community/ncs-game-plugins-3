#pragma semicolon 1

#include <ncs/client>

#define REQUIRE_PLUGIN
#include <ncs/backpack>
#include <ncs/money>
#undef REQUIRE_PLUGIN

#include <ncs/chat>

#define P_NAME P_PRE ... " - Ins round reward"
#define P_DESC "Ins round reward plugin"

ConVar cv_min_player;
ConVar cv_reward_rmb;

public Plugin myinfo = 
{
    name        = "Reward (NCS.ver)",
    author      = "Gunslinger",
    description = "Give reward to player when they won",
    version     = "1.1",
    url         = "https://blog.new-page.xyz"
};

public void OnPluginStart()
{
    HookEvent("round_end", RoundEnd_Event, EventHookMode_Post);

    cv_min_player = CreateConVar("reward_min_player", "10", "Min player count for reward", 0, true, 0.0);
    cv_reward_rmb = CreateConVar("reward_rmb", "5", "RMB amount for reward", 0, true, 0.0);
}

public Action RoundEnd_Event(Event event, const char[] name, bool dontBroadcast)
{
    int winner = GetEventInt(event, "winner");
    if (winner == 2)
    {
        if (GetClientCount(true) >= cv_min_player.IntValue)
            Reward();
    }
    return Plugin_Continue;
}

void Reward()
{
    int rmb = cv_reward_rmb.IntValue;
    if (rmb <= 0)
    {
        return;
    }

    Item passbox;
    passbox.id = 1000;
    passbox.amount = 1;
    Items rewards = new Items();
    rewards.Add(passbox);

    for (int client = 1; client < MAXPLAYERS; client++)
    {
        if (IsValidClient(client) && IsClientInGame(client))
        {
            NCS_Money_Give(client, rmb, "通关奖励");
            NCS_Backpack_AddItems(client, rewards);
            NCS_Chat(client, _, "{blue} 通关成功！奖励：{red}%d{blue} 软妹币, {red}1个{blue} 通行证升级箱", rmb);
        }
    }

    delete rewards;
}
