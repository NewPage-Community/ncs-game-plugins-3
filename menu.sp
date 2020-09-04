#pragma semicolon 1

#include <ncs/client>

public Plugin myinfo = 
{
    name        = "NCS menu fusion",
    author      = "Gunslinger",
    description = "",
    version     = "1.0",
    url         = "https://blog.new-page.xyz"
};

public void OnPluginStart()
{
    RegConsoleCmd("sm_menu", Command_Menu, "[NCS fusion] 菜单");
}

public Action Command_Menu(int client, int args)
{
    if (!IsValidClient(client))
        return Plugin_Handled;

    Menu menu = new Menu(MenuHandle);
    menu.SetTitle("NCS系统菜单");
    menu.AddItem("", "每日签到(!qd)");
    menu.AddItem("", "商店(!store)");
    menu.AddItem("", "个人中心");
    menu.AddItem("", "个性化设置");
    menu.Display(client, MENU_TIME_FOREVER);
    return Plugin_Handled;
}

public int MenuHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Select)
    {
        switch (slot)
        {
            case 0: FakeClientCommandEx(client, "sm_sign");
            case 1: FakeClientCommandEx(client, "sm_store");
            case 2: PersonalMenu(client);
            case 3: SettingsMenu(client);
        }
    }
}

void PersonalMenu(int client)
{
    if (!IsValidClient(client))
        return;

    Menu menu = new Menu(PersonalMenuHandle);
    menu.SetTitle("NCS系统--个人中心");
    menu.AddItem("", "个人信息(!account)");
    menu.AddItem("", "通行证(!pass)");
    menu.AddItem("", "会员(!vip)");
    menu.AddItem("", "软妹币流水账(!moneyrecords)");
    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int PersonalMenuHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
        Command_Menu(client, 0);
    else if (action == MenuAction_Select)
    {
        switch (slot)
        {
            case 0: FakeClientCommandEx(client, "sm_account");
            case 1: FakeClientCommandEx(client, "sm_pass");
            case 2: FakeClientCommandEx(client, "sm_vip");
            case 3: FakeClientCommandEx(client, "sm_moneyrecords");
        }
    }
}

void SettingsMenu(int client)
{
    if (!IsValidClient(client))
        return;

    Menu menu = new Menu(SettingsMenuHandle);
    menu.SetTitle("NCS系统--系统设置");
    menu.AddItem("", "皮肤(!skin)");
    menu.AddItem("", "头衔(!title)");
    menu.AddItem("", "足迹(!trail)");
    menu.AddItem("", "光环(!aura)");
    menu.AddItem("", "喷漆(!spray)");
    menu.ExitBackButton = true;
    menu.Display(client, MENU_TIME_FOREVER);
}

public int SettingsMenuHandle(Menu menu, MenuAction action, int client, int slot)
{
    if (action == MenuAction_End)
        delete menu;
    else if (action == MenuAction_Cancel && slot == MenuCancel_ExitBack)
        Command_Menu(client, 0);
    else if (action == MenuAction_Select)
    {
        switch (slot)
        {
            case 0: FakeClientCommandEx(client, "sm_skin");
            case 1: FakeClientCommandEx(client, "sm_title");
            case 2: FakeClientCommandEx(client, "sm_trail");
            case 3: FakeClientCommandEx(client, "sm_aura");
            case 4: FakeClientCommandEx(client, "sm_spray");
        }
    }
}