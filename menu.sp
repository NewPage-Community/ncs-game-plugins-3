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
    menu.SetTitle("NCS系统融合菜单");
    menu.AddItem("", "每日签到");
    menu.AddItem("", "商店菜单");
    menu.AddItem("", "通行证菜单");
    menu.AddItem("", "会员菜单");
    menu.AddItem("", "皮肤设置");
    menu.AddItem("", "头衔设置");
    menu.AddItem("", "足迹设置");
    menu.AddItem("", "光环设置");
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
            case 2: FakeClientCommandEx(client, "sm_pass");
            case 3: FakeClientCommandEx(client, "sm_vip");
            case 4: FakeClientCommandEx(client, "sm_skin");
            case 5: FakeClientCommandEx(client, "sm_title");
            case 6: FakeClientCommandEx(client, "sm_trail");
            case 7: FakeClientCommandEx(client, "sm_aura");
        }
    }
}