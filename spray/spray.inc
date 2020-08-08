#define MAX_SPRAY_NUM 1024
#define MAX_SPRAY_NAME_LENGTH 64
#define MAX_PARTICLE_NAME_LENGTH 64

enum struct Spray
{
    int id;
    char name[MAX_SPRAY_NAME_LENGTH];
    char sprayPath[PLATFORM_MAX_PATH];
    int decal;

    void Init(JSONObject data)
    {
        this.id = data.GetInt("id");
        data.GetString("name", this.name, MAX_SPRAY_NAME_LENGTH);

        // attr
        JSONObject attr = view_as<JSONObject>(data.Get("attributes"));
        attr.GetString("spray_path", this.sprayPath, PLATFORM_MAX_PATH);
        delete attr;

        this.decal = PrecacheDecal(this.sprayPath, true);
    }

    void Clean()
    {
        this.id = 0;
        this.name[0] = '\0';
        this.sprayPath[0] = '\0';
        this.decal = 0;
    }
}

methodmap SprayIndex < StringMap
{
    public SprayIndex()
    {
        return view_as<SprayIndex>(new StringMap());
    }
    
    public bool Add(JSONObject data)
    {
        char key[8];
        Spray s;
        s.Init(data);
        IntToString(s.id, key, sizeof(key));
        return this.SetArray(key, s, sizeof(s), true);
    }

    public bool Get(int id, Spray ref)
    {
        char key[8];
        IntToString(id, key, sizeof(key));
        return this.GetArray(key, ref, sizeof(ref));
    }

    public bool Remove(int id)
    {
        char key[8];
        IntToString(id, key, sizeof(key));
        return this.Remove(key);
    }
}

SprayIndex sprays;
int g_UsersUsedSpray[MAXPLAYERS+1];

#include "spray/api"
#include "spray/player"
#include "spray/cookie"
#include "spray/cmd"
#include "spray/menu"
#include "spray/native"