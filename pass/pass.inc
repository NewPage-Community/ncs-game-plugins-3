enum struct PassInfo
{
    char uid[MAX_UID_LENGTH];
    int index;
    int point;

    bool IsValid()
    {
        if (this.index == 0 || this.uid[0] == '\0')
            return false;
        return IsValidClient(this.index);
    }

    void Init(int index, const char[] uid)
    {
        this.index = index;
        strcopy(this.uid, MAX_UID_LENGTH, uid);
        this.point = 0;
    }

    void Clear()
    {
        this.uid = "\0";
        this.index = 0;
        this.point = 0;
    }

    void AddPoint(int point)
    {
        this.point += point;
    }

    void PushJSONArray(JSONArray array)
    {
        JSONObject pass = new JSONObject();
        pass.SetInt64("uid", this.uid);
        pass.SetInt("point", this.point);
        array.Push(pass);
        delete pass;
    }
}

PassInfo player[MAXPLAYERS+1];

#include "pass/api"
#include "pass/cache"
#include "pass/player"
#include "pass/native"