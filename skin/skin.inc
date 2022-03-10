#define MAX_SKINS_NUM 1024
#define MAX_SKIN_NAME_LENGTH 64

EngineVersion engine;

enum struct BodysData {
	char name[32];
	int index;
}

enum struct Skin
{
    int id;
    char name[MAX_SKIN_NAME_LENGTH];
    char skinPath[PLATFORM_MAX_PATH];
    char armPath[PLATFORM_MAX_PATH];
    char deathSound[PLATFORM_MAX_PATH];
    ArrayList bodys;
    ArrayList sounds;

    bool Init(JSONObject data)
    {
        if (this.bodys != INVALID_HANDLE) delete this.bodys;
        this.bodys = new ArrayList(sizeof(BodysData));

        this.id = data.GetInt("id");
        data.GetString("name", this.name, MAX_SKIN_NAME_LENGTH);

        // attr
        JSONObject attr = view_as<JSONObject>(data.Get("attributes"));
        if (data.HasKey("skin_death_sound"))
            attr.GetString("skin_path", this.skinPath, PLATFORM_MAX_PATH);
        if (data.HasKey("skin_death_sound"))
            attr.GetString("arm_path", this.armPath, PLATFORM_MAX_PATH);
        if (data.HasKey("skin_death_sound"))
            attr.GetString("skin_death_sound", this.deathSound, PLATFORM_MAX_PATH);

        if (data.HasKey("bodys"))
        {
            static BodysData tmp_body;
            JSONObject bodys = view_as<JSONObject>(data.Get("bodys"));
            JSONObjectKeys bodysKeys = bodys.Keys();
            while (bodysKeys.ReadKey(tmp_body.name, sizeof(BodysData::name)))
            {
                tmp_body.index = bodys.GetInt(tmp_body.name);
                this.bodys.PushArray(tmp_body);
            }
            delete bodysKeys;
            delete bodys;
        }

        delete attr;

        // Precache model
        if (this.skinPath[0] != '\0')
        {
            if (FileExists(this.skinPath, true))
            {
#if !defined GAME_INSURGENCY
                PrecacheModel(this.skinPath, IsSDK2013Mod() ? true : false);
#endif
            }
            else
            {
                NCS_LogError("Skin", "SkinInit", "File is not exists (%s)", this.skinPath);
                return false;
            }
        }

        if (this.armPath[0] != '\0' && engine == Engine_CSGO)
        {
            if (FileExists(this.armPath, true))
            {
#if !defined GAME_INSURGENCY
                PrecacheModel(this.armPath, IsSDK2013Mod() ? true : false);
#endif
            }
            else
            {
                NCS_LogError("Skin", "SkinInit", "File is not exists (%s)", this.armPath);
                return false;
            }
        }

        if (this.deathSound[0] != '\0')
        {
            char path[PLATFORM_MAX_PATH];
            Format(path, sizeof(path), "sound/%s", this.deathSound);
            if (FileExists(path, true))
                PrecacheSound(this.deathSound);
            else
                NCS_LogError("Skin", "SkinInit", "File is not exists (%s)", path);
        }

        return true;
    }

    void Clean()
    {
        this.id = 0;
        this.name[0] = '\0';
        this.skinPath[0] = '\0';
        this.armPath[0] = '\0';
        this.deathSound[0] = '\0';
        delete this.bodys;
    }
}

methodmap SkinIndex < StringMap
{
    public SkinIndex()
    {
        return view_as<SkinIndex>(new StringMap());
    }

    public bool Add(JSONObject data)
    {
        char key[8];
        Skin s;
        if (!s.Init(data))
            return false;
        IntToString(s.id, key, sizeof(key));
        return this.SetArray(key, s, sizeof(s), true);
    }

    public bool Get(int skinID, Skin ref)
    {
        char key[8];
        IntToString(skinID, key, sizeof(key));
        return this.GetArray(key, ref, sizeof(ref));
    }

}

SkinIndex g_Skins;
int g_UsersUsedSkin[MAXPLAYERS+1];

// Library
#if !defined GAME_INSURGENCY
bool isZombieReloadedLoaded = false;

void InitLibrary()
{
    isZombieReloadedLoaded = LibraryExists("zombiereloaded");
}
#endif

void InitSkin()
{
    engine = GetEngineVersion();
}

#if defined GAME_INSURGENCY
#include "skin/player.ins"
#else
#include "skin/player"
#endif

#include "skin/api"
#include "skin/preview"
#include "skin/menu"
#include "skin/cmd"
#include "skin/native"
#include "skin/cookie"