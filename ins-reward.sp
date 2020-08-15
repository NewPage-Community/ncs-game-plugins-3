#pragma semicolon 1

#include <ncs/client>

#define REQUIRE_PLUGIN
#include <ncs/backpack>
#include <ncs/money>
#include <ncs/vip>
#include <ncs/pass>
#undef REQUIRE_PLUGIN

#include <ncs/chat>

ConVar cv_round_min_player;
ConVar cv_round_rmb;
ConVar cv_round_pass_point;
ConVar cv_sign_rmb;
ConVar cv_sign_vip_rmb;
ConVar cv_sign_vip_skin;

public Plugin myinfo = 
{
    name        = "Reward (NCS.ver)",
    author      = "Gunslinger",
    description = "Reward manager",
    version     = "1.2",
    url         = "https://blog.new-page.xyz"
};

public void OnPluginStart()
{
    HookEvent("round_end", RoundEnd_Event, EventHookMode_Post);

    cv_round_min_player = CreateConVar("reward_roundwin_min_player", "10", "", 0, true, 0.0);
    cv_round_rmb = CreateConVar("reward_roundwin_rmb", "5", "", 0, true, 0.0);
    cv_round_pass_point = CreateConVar("reward_roundwin_pass_point", "60", "", 0, true, 0.0);
    cv_sign_rmb = CreateConVar("reward_sign_rmb", "10", "10", 0, true, 0.0);
    cv_sign_vip_rmb = CreateConVar("reward_sign_vip_rmb", "10", "", 0, true, 0.0);
    cv_sign_vip_skin = CreateConVar("reward_sign_vip_skin", "11,68,73,99,110", "", 0, true, 0.0);
}

public Action RoundEnd_Event(Event event, const char[] name, bool dontBroadcast)
{
    int winner = GetEventInt(event, "winner");
    if (winner == 2)
    {
        if (GetClientCount(true) >= cv_round_min_player.IntValue)
            RoundReward();
    }
    return Plugin_Continue;
}

void RoundReward()
{
    int rmb = cv_round_rmb.IntValue;
    int point = cv_round_pass_point.IntValue;

    if (rmb <= 0 && point <= 0)
        return;

    for (int client = 1; client < MAXPLAYERS; client++)
    {
        if (IsValidClient(client) && IsClientInGame(client))
        {
            NCS_Chat(client, _, "{blue}通关成功");
            if (rmb > 0)
            {
                NCS_Money_Give(client, rmb, "通关奖励");
                NCS_Chat(client, _, "{blue}奖励: {green}%d软妹币", rmb);
            }
            if (point > 0)
            {
                NCS_Pass_AddPoint(client, point);
                NCS_Chat(client, _, "{blue}奖励: {green}%d点通行证经验", point);
            }
        }
    }
}

public void NCS_Sign_OnUserSigned(int client)
{
    int sign = cv_sign_rmb.IntValue;
    int vipSign = cv_sign_vip_rmb.IntValue;

    NCS_Money_Give(client, sign, "签到奖励");
    NCS_Chat(client, _, "{blue}奖励你 {green}%d软妹币", sign);

    if (NCS_VIP_IsVIP(client))
    {
        // rmb
        NCS_Money_Give(client, vipSign, "VIP福利");
        NCS_Chat(client, _, "{blue}领取VIP福利 {green}%d软妹币", vipSign);

        // skin
        Items rewards = new Items();
        Item skin;
        skin.length = 24*3600;

        char buffer[512], skinList[64][8];
        cv_sign_vip_skin.GetString(buffer, sizeof(buffer));
        int count = ExplodeString(buffer, ",", skinList, sizeof(skinList), sizeof(skinList[]));

        for (int i = 0; i < count; i++)
        {
            skin.id = StringToInt(skinList[i]);
            rewards.Add(skin);
        }
        if (count > 0)
        {
            NCS_Backpack_AddItems(client, rewards);
            NCS_Chat(client, _, "{blue}领取VIP福利 {green}会员限免皮肤(1天)", vipSign);
        }
        delete rewards;
    }
}