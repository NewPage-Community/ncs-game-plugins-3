#define MAX_SKINS_NUM 1024
#define MAX_SKIN_NAME_LENGTH 64

enum struct Skin
{
    int id;
    char name[MAX_SKIN_NAME_LENGTH];
    char skinPath[PLATFORM_MAX_PATH];
    char armPath[PLATFORM_MAX_PATH];
    int price;

    void Init(JSONObject data)
    {
        this.id = data.GetInt("id");
        data.GetString("name", this.name, MAX_SKIN_NAME_LENGTH);
        data.GetString("skin_path", this.skinPath, PLATFORM_MAX_PATH);
        data.GetString("arm_path", this.armPath, PLATFORM_MAX_PATH);
        this.price = data.GetInt("price");

        // Precache model
        if (this.skinPath[0] != '\0')
            PrecacheModel(this.skinPath, true);
        if (this.armPath[0] != '\0')
            PrecacheModel(this.armPath, true);
    }

    void Clean()
    {
        this.id = 0;
        this.name = "";
        this.skinPath = "";
        this.armPath = "";
        this.price = 0;
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

#include <ncs/chat>
#include "skin/api"
#include "skin/player"
#include "skin/preview"
#include "skin/menu"
#include "skin/cmd"
#include "skin/native"