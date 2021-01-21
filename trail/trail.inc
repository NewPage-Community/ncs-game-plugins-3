#define MAX_TRAIL_NUM 1024
#define MAX_TRAIL_NAME_LENGTH 64
#define MAX_PARTICLE_NAME_LENGTH 64

enum TrailType
{
    TrailType_Material = 0,
    TrailType_Particle
}

enum struct Trail
{
    int id;
    char name[MAX_TRAIL_NAME_LENGTH];
    char trailPath[PLATFORM_MAX_PATH];
    char particleName[MAX_PARTICLE_NAME_LENGTH];
    TrailType type;

    bool Init(JSONObject data)
    {
        this.id = data.GetInt("id");
        data.GetString("name", this.name, MAX_TRAIL_NAME_LENGTH);

        // attr
        char trail_type[16];
        JSONObject attr = view_as<JSONObject>(data.Get("attributes"));
        attr.GetString("trail_path", this.trailPath, PLATFORM_MAX_PATH);
        attr.GetString("trail_type", trail_type, sizeof(trail_type));
        attr.GetString("particle_name", this.particleName, MAX_PARTICLE_NAME_LENGTH);
        delete attr;

        // type
        if (StrEqual(trail_type, "particle"))
        {
            this.type = TrailType_Particle;
            PrecacheGeneric(this.trailPath, true);
            PrecacheParticleEffect(this.particleName);
        }
        else
            this.type = TrailType_Material;

        if (!FileExists(this.trailPath, true))
        {
            NCS_LogError("Trail", "TrailInit", "File is not exists (%s)", this.trailPath);
            return false;
        }

        return true;
    }

    void Clean()
    {
        this.id = 0;
        this.name[0] = '\0';
        this.trailPath[0] = '\0';
        this.particleName[0] = '\0';
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
        if (!s.Init(data))
            return false;
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
#include "trail/particle"