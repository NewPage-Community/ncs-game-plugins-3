#define MAX_AURA_NUM 1024
#define MAX_AURA_NAME_LENGTH 64
#define MAX_PARTICLE_NAME_LENGTH 64

enum struct Aura
{
    int id;
    char name[MAX_AURA_NAME_LENGTH];
    char auraPath[PLATFORM_MAX_PATH];
    char particleName[MAX_PARTICLE_NAME_LENGTH];

    bool Init(JSONObject data)
    {
        this.id = data.GetInt("id");
        data.GetString("name", this.name, MAX_AURA_NAME_LENGTH);

        // attr
        JSONObject attr = view_as<JSONObject>(data.Get("attributes"));
        attr.GetString("aura_path", this.auraPath, PLATFORM_MAX_PATH);
        attr.GetString("particle_name", this.particleName, MAX_PARTICLE_NAME_LENGTH);
        delete attr;

        PrecacheGeneric(this.auraPath, true);
        PrecacheParticleEffect(this.particleName);

        if (!FileExists(this.auraPath, true))
        {
            NCS_LogError("Aura", "AuraInit", "File is not exists (%s)", this.auraPath);
            return false;
        }

        return true;
    }

    void Clean()
    {
        this.id = 0;
        this.name[0] = '\0';
        this.auraPath[0] = '\0';
        this.particleName[0] = '\0';
    }
}

methodmap AuraIndex < StringMap
{
    public AuraIndex()
    {
        return view_as<AuraIndex>(new StringMap());
    }
    
    public bool Add(JSONObject data)
    {
        char key[8];
        Aura s;
        if (!s.Init(data))
            return false;
        IntToString(s.id, key, sizeof(key));
        return this.SetArray(key, s, sizeof(s), true);
    }

    public bool Get(int id, Aura ref)
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

AuraIndex auras;
int g_UsersUsedAura[MAXPLAYERS+1];

// Library
bool isZombieReloadedLoaded = false;

void InitLibrary()
{
    isZombieReloadedLoaded = LibraryExists("zombiereloaded");
}

#include "aura/api"
#include "aura/player"
#include "aura/cookie"
#include "aura/cmd"
#include "aura/menu"
#include "aura/native"
#include "aura/particle"