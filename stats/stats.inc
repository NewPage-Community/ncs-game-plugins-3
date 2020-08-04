#define  Stats_Service "game/stats"

enum struct StatsInfo
{
    char range[8];
    char stats_name[8];
    char version[8];
    char uid[MAX_UID_LENGTH];
    float increment;

    void Key(char[] buffer, int size)
    {
        Format(buffer, size, "ncs_%s_%s_%s", this.range, this.stats_name, this.version);
    }
}

methodmap Stats < ArrayList
{
    public Stats()
    {
        StatsInfo t;
        return view_as<Stats>(new ArrayList(sizeof(t), 0));
    }

    public int Add(StatsInfo data)
    {
        return this.PushArray(data, sizeof(data));
    }

    public JSONObject JSON()
    {
        JSONObject data = new JSONObject();
        JSONArray array = new JSONArray();

        int length = this.Length;
        for (int i = 0; i < length; i++)
        {
            StatsInfo stats_raw;
            JSONObject stats_json = new JSONObject();
            this.GetArray(i, stats_raw, sizeof(stats_raw));
            stats_json.SetString("range", stats_raw.range);
            stats_json.SetString("stats_name", stats_raw.stats_name);
            stats_json.SetString("version", stats_raw.version);
            stats_json.SetInt64("uid", stats_raw.uid);
            stats_json.SetFloat("increment", stats_raw.increment);
            array.Push(stats_json);
            delete stats_json;
        }

        data.Set("stats", array);
        delete array;
        return data;
    }
}

Stats StatsList;

#include "stats/api"
#include "stats/native"
#include "stats/cache"