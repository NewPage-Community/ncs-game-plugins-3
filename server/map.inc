ConVar changeMapNotify;

void InitMap()
{
    changeMapNotify = CreateConVar("ncs_server_change_map_notify", "1", "Change map notify");
}

void MapNotify()
{
    if (changeMapNotify.BoolValue)
    {
        char map[PLATFORM_MAX_PATH];
        GetCurrentMap(map, sizeof(map));
        ReqMapNotify(ServerID, map);
    }
}