#pragma semicolon 1

#include <ncs/client>
#include <ncs/backpack>
#include <ncs/money>
#include <ncs/vip>
#include <ncs/pass>
#include <ncs/chat>

#define CHAT_PREFIX "[奖励]"

// round
ConVar cv_round_min_player;
ConVar cv_round_rmb;
ConVar cv_round_pass_point;
// sign
ConVar cv_sign_rmb;
ConVar cv_sign_vip_rmb;
ConVar cv_sign_vip_skin;
ConVar cv_sign_vip_point;
// maintain
ConVar cv_maintain_rmb;
ConVar cv_maintain_starttime;
ConVar cv_maintain_endtime;
// holiday
ConVar cv_holiday_rmb;
ConVar cv_holiday_starttime;
ConVar cv_holiday_endtime;

public Plugin myinfo = 
{
    name        = "Reward (NCS.ver)",
    author      = "Gunslinger",
    description = "Reward manager",
    version     = "1.3",
    url         = "https://blog.new-page.xyz"
};

public void OnPluginStart()
{
    HookEvent("round_end", RoundEnd_Event, EventHookMode_Post);

    // round
    cv_round_min_player = CreateConVar("reward_roundwin_min_player", "10", "", 0, true, 0.0);
    cv_round_rmb = CreateConVar("reward_roundwin_rmb", "5", "", 0, true, 0.0);
    cv_round_pass_point = CreateConVar("reward_roundwin_pass_point", "60", "", 0, true, 0.0);
    // sign
    cv_sign_rmb = CreateConVar("reward_sign_rmb", "10", "", 0, true, 0.0);
    cv_sign_vip_rmb = CreateConVar("reward_sign_vip_rmb", "10", "", 0, true, 0.0);
    cv_sign_vip_skin = CreateConVar("reward_sign_vip_skin", "75,11,93", "", 0, true, 0.0);
    cv_sign_vip_point = CreateConVar("reward_sign_vip_point", "22", "", 0, true, 0.0);
    // maintain
    cv_maintain_rmb = CreateConVar("reward_maintain_rmb", "100", "", 0, true, 0.0);
    cv_maintain_starttime = CreateConVar("reward_maintain_starttime", "0", "", 0, true, 0.0);
    cv_maintain_endtime = CreateConVar("reward_maintain_endtime", "1598630400", "", 0, true, 0.0);
    // holiday
    cv_holiday_rmb = CreateConVar("reward_holiday_rmb", "100", "", 0, true, 0.0);
    cv_holiday_starttime = CreateConVar("reward_holiday_starttime", "0", "", 0, true, 0.0);
    cv_holiday_endtime = CreateConVar("reward_holiday_endtime", "1598630400", "", 0, true, 0.0);
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
            NCS_Chat(client, CHAT_PREFIX, "{blue}通关成功");
            if (rmb > 0 && LibraryExists("NCS-Money"))
            {
                NCS_Money_Give(client, rmb, "通关奖励");
                NCS_Chat(client, CHAT_PREFIX, "{blue}通关奖励: {green}%d软妹币", rmb);
            }
            if (point > 0 && LibraryExists("NCS-Pass"))
            {
                NCS_Pass_AddPoint(client, point);
                NCS_Chat(client, CHAT_PREFIX, "{blue}通关奖励: {green}%d点通行证经验", point);
            }
        }
    }
}

public void NCS_Sign_OnUserSigned(int client)
{
    SignGiveRMB(client);
    SignGiveVIPPoint(client);
    SignGiveBackpackItems(client);
    SignMaintain(client);
    SignHoliday(client);
}

void SignGiveRMB(int client)
{
    int sign = cv_sign_rmb.IntValue;
    int vipSign = cv_sign_vip_rmb.IntValue;

    if (!LibraryExists("NCS-Money"))
        return;

    NCS_Money_Give(client, sign, "签到奖励");
    NCS_Chat(client, CHAT_PREFIX, "{blue}签到奖励: {green}%d软妹币", sign);

    if (LibraryExists("NCS-VIP"))
    {
        if (NCS_VIP_IsVIP(client))
        {
            // rmb
            NCS_Money_Give(client, vipSign, "VIP福利");
            NCS_Chat(client, CHAT_PREFIX, "{blue}VIP签到奖励: {green}%d软妹币", vipSign);
        }
    }
}

void SignGiveVIPPoint(int client)
{
    int vipPoint = cv_sign_vip_point.IntValue;

    if (!LibraryExists("NCS-VIP"))
        return;

    if (NCS_VIP_IsVIP(client))
    {
        // vip point
        NCS_VIP_AddPoint(client, vipPoint);
        NCS_Chat(client, CHAT_PREFIX, "{blue}VIP签到奖励: {green}%d成长值", vipPoint);
    }
}

void SignGiveBackpackItems(int client)
{
    if (!LibraryExists("NCS-Money") || !LibraryExists("NCS-Backpack"))
        return;

    if (NCS_VIP_IsVIP(client))
    {
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
            NCS_Chat(client, CHAT_PREFIX, "{blue}VIP签到奖励: {green}mur猫(1天)、香蕉人(1天)、pedobear【原版】(1天)");
        }
        delete rewards;
    }
}

void SignMaintain(int client)
{
    int rmb = cv_maintain_rmb.IntValue;
    int start = cv_maintain_starttime.IntValue;
    int end = cv_maintain_endtime.IntValue;

    if (!LibraryExists("NCS-Money"))
        return;
    
    int now = GetTime();
    if (start <= now && now < end)
    {
        NCS_Money_Give(client, rmb, "维护补贴");
        NCS_Chat(client, CHAT_PREFIX, "{blue}维护补贴: {green}%d软妹币", rmb);
    }
}

void SignHoliday(int client)
{
    int rmb = cv_holiday_rmb.IntValue;
    int start = cv_holiday_starttime.IntValue;
    int end = cv_holiday_endtime.IntValue;

    if (!LibraryExists("NCS-Money"))
        return;
    
    int now = GetTime();
    if (start <= now && now < end)
    {
        NCS_Money_Give(client, rmb, "节日福利");
        NCS_Chat(client, CHAT_PREFIX, "{blue}节假日福利: {green}%d软妹币", rmb);
    }
}