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
    char uid[MAX_SKIN_NAME_LENGTH];
    char skinPath[PLATFORM_MAX_PATH];
    char armPath[PLATFORM_MAX_PATH];
    // char deathSound[PLATFORM_MAX_PATH];
    ArrayList bodys;
    StringMap sounds;
    int initBody;

    bool Init(JSONObject data)
    {
        if (this.bodys != INVALID_HANDLE) delete this.bodys;
        this.bodys = new ArrayList(sizeof(BodysData));
        this.sounds = new StringMap();

        this.id = data.GetInt("id");
        data.GetString("name", this.name, MAX_SKIN_NAME_LENGTH);

        // attr
        JSONObject attr = view_as<JSONObject>(data.Get("attributes"));
        attr.GetString("uid", this.uid, MAX_SKIN_NAME_LENGTH);
        attr.GetString("skin_path", this.skinPath, PLATFORM_MAX_PATH);
        if (attr.HasKey("arm_path"))
            attr.GetString("arm_path", this.armPath, PLATFORM_MAX_PATH);
        // if (attr.HasKey("skin_death_sound"))
        //     attr.GetString("skin_death_sound", this.deathSound, PLATFORM_MAX_PATH);

        if (attr.HasKey("bodys"))
        {
            static BodysData tmp_body;
            JSONObject bodys = view_as<JSONObject>(attr.Get("bodys"));
            JSONObjectKeys bodysKeys = bodys.Keys();
            while (bodysKeys.ReadKey(tmp_body.name, sizeof(BodysData::name)))
            {
                tmp_body.index = bodys.GetInt(tmp_body.name);
                this.bodys.PushArray(tmp_body);
            }
            delete bodysKeys;
            delete bodys;

            this.initBody = attr.GetInt("initBody");
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

        // if (this.deathSound[0] != '\0')
        // {
        //     char path[PLATFORM_MAX_PATH];
        //     Format(path, sizeof(path), "sound/%s", this.deathSound);
        //     if (FileExists(path, true))
        //         PrecacheSound(this.deathSound);
        //     else
        //         NCS_LogError("Skin", "SkinInit", "File is not exists (%s)", path);
        // }

        // Load sound
        this.LoadSounds();

        return true;
    }

    void Clean()
    {
        this.id = 0;
        this.name[0] = '\0';
        this.skinPath[0] = '\0';
        this.armPath[0] = '\0';
        // this.deathSound[0] = '\0';
        delete this.bodys;
        delete this.sounds;
    }

    void LoadSounds()
    {
        static char path[PLATFORM_MAX_PATH];
        Format(path, sizeof(path), "sound/newpage/models/%s", this.uid);
        DirectoryListing dir = OpenDirectory(path);
        if (dir)
        {
            FileType type;
            while (dir.GetNext(path, sizeof(path), type))
            {
                int indexs[8], size;
                char key[16], num[4], ext[6];

                // Not found
                if (path[0] == '.' || type == FileType_Unknown) continue;

                // Get sound info
                Regex regex = new Regex("([A-Za-z_]+)([0-9]*).(mp3|ogg|wav)");
                if (regex.Match(path) <= 0) continue;
                regex.GetSubString(1, key, sizeof(key));
                regex.GetSubString(2, num, sizeof(num));
                regex.GetSubString(3, ext, sizeof(ext));
                delete regex;

                static char buffer[32];
                Format(buffer, sizeof(buffer), "%s_files", key);
                this.sounds.GetArray(buffer, indexs, sizeof(indexs), size);
                if (size >= sizeof(indexs)) {
                    NCS_LogError("Skin", "Skin.LoadSounds", "%s has too many sound files", this.uid);
                    continue;
                }
                indexs[size] = StringToInt(num);
                this.sounds.SetArray(buffer, indexs, size+1);
                Format(buffer, sizeof(buffer), "%s_ext", key);
                this.sounds.SetString(buffer, ext);
            }
        }
        delete dir;
    }

    // Get sound randomly 
    bool GetSound(const char[] key, char[] buffer, int size)
    {
        int indexs[8], length;
        Format(buffer, size, "%s_files", key);
        this.sounds.GetArray(buffer, indexs, sizeof(indexs), length);
        if (!length) return false;

        // Get extension
        Format(buffer, size, "%s_ext", key);
        this.sounds.GetString(buffer, buffer, size);
        Format(buffer, size, ".%s", buffer);

        // Get random files
        int index = GetURandomInt() % length;
        if (index > 0) Format(buffer, size, "%d%s", index, buffer);
        Format(buffer, size, "newpage/models/%s/%s%s", this.uid, key, buffer);
        return true;
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

#include "skin/sound"

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