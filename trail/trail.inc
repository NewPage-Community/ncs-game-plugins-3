#define MAX_TRAIL_NUM 1024
#define MAX_TRAIL_NAME_LENGTH 64

enum struct Trail
{
    int id;
    char name[MAX_TRAIL_NAME_LENGTH];
    char trailPath[PLATFORM_MAX_PATH];

    void Init(JSONObject data)
    {
        this.id = data.GetInt("id");
        data.GetString("name", this.name, MAX_TRAIL_NAME_LENGTH);

        // attr
        JSONObject attr = view_as<JSONObject>(data.Get("attributes"));
        attr.GetString("trail_path", this.trailPath, PLATFORM_MAX_PATH);
        delete attr;

        // Precache
        if (this.trailPath[0] != '\0')
            PrecacheGeneric(this.trailPath, true);
    }

    void Clean()
    {
        this.id = 0;
        this.name[0] = '\0';
        this.trailPath[0] = '\0';
    }
}

methodmap TrailIndex < StringMap
{
    public TrailIndex()
    {
        return view_as<TrailIndex>(new StringMap());
    }
    
    public bool Add(JSONObject data)
    {
        char key[8];
        Trail s;
        s.Init(data);
        IntToString(s.id, key, sizeof(key));
        return this.SetArray(key, s, sizeof(s), true);
    }

    public bool Get(int trailID, Trail ref)
    {
        char key[8];
        IntToString(trailID, key, sizeof(key));
        return this.GetArray(key, ref, sizeof(ref));
    }

    public bool Remove(int trailID)
    {
        char key[8];
        IntToString(trailID, key, sizeof(key));
        return this.Remove(key);
    }
}

TrailIndex trails;
int g_UsersUsedTrail[MAXPLAYERS+1];

#include "trail/api"
#include "trail/player"
#include "trail/cookie"
#include "trail/cmd"
#include "trail/menu"
#include "trail/native"