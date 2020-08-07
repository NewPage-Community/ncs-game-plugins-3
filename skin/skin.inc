#define MAX_SKINS_NUM 1024
#define MAX_SKIN_NAME_LENGTH 64

enum struct Skin
{
    int id;
    char name[MAX_SKIN_NAME_LENGTH];
    char skinPath[PLATFORM_MAX_PATH];
    char armPath[PLATFORM_MAX_PATH];

    void Init(JSONObject data)
    {
        this.id = data.GetInt("id");
        data.GetString("name", this.name, MAX_SKIN_NAME_LENGTH);

        // attr
        JSONObject attr = view_as<JSONObject>(data.Get("attributes"));
        attr.GetString("skin_path", this.skinPath, PLATFORM_MAX_PATH);
        attr.GetString("arm_path", this.armPath, PLATFORM_MAX_PATH);
        delete attr;

        // Precache model
        if (this.skinPath[0] != '\0')
        {
            if (FileExists(this.skinPath, true))
                PrecacheModel(this.skinPath, true);
            else
                NCS_LogError("Skin", "SkinInit", "File is not exists (%s)", this.skinPath);
        }
            
        if (this.armPath[0] != '\0')
        {
            if (FileExists(this.armPath, true))
                PrecacheModel(this.armPath, true);
            else
                NCS_LogError("Skin", "SkinInit", "File is not exists (%s)", this.armPath);
        }
    }

    void Clean()
    {
        this.id = 0;
        this.name[0] = '\0';
        this.skinPath[0] = '\0';
        this.armPath[0] = '\0';
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
        s.Init(data);
        IntToString(s.id, key, sizeof(key));
        return this.SetArray(key, s, sizeof(s), true);
    }

    public bool Get(int skinID, Skin ref)
    {
        char key[8];
        IntToString(skinID, key, sizeof(key));
        return this.GetArray(key, ref, sizeof(ref));
    }

    public bool Remove(int skinID)
    {
        char key[8];
        IntToString(skinID, key, sizeof(key));
        return this.Remove(key);
    }
}

SkinIndex g_Skins;
int g_UsersUsedSkin[MAXPLAYERS+1];

#include "skin/api"
#include "skin/player"
#include "skin/preview"
#include "skin/menu"
#include "skin/cmd"
#include "skin/native"
#include "skin/cookie"