methodmap UIDMap < StringMap
{
    public UIDMap()
    {
        return view_as<UIDMap>(new StringMap());
    }

    public bool Set(const char[] uid, int index)
    {
        return this.SetValue(uid, index, true);
    }

    // not found return 0
    public int Get(const char[] uid)
    {
        int client;
        if (!this.GetValue(uid, client))
            client = 0;
        return client;
    }
}

static UIDMap uidMap;

void InitUID()
{
    uidMap = new UIDMap();
}

void InitClient(int client, const char[] uid)
{
    uidMap.Set(uid, client);
}

void RemoveClient(int client)
{
    uidMap.Remove(g_Accounts[client].uid);
}

int FindClient(const char[] uid)
{
    return uidMap.Get(uid);
}