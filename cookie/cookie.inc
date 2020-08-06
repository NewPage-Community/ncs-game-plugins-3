#define MAX_COOKIE_KEY_LENGTH 64
#define MAX_COOKIE_VALUE_LENGTH 256

enum struct CookieInfo
{
    int index;
    StringMap cookie;

    void Init(int index, JSONObject data)
    {
        this.index = index;
        if (this.cookie != INVALID_HANDLE)
            delete this.cookie;
        this.cookie = new StringMap();

        char key[MAX_COOKIE_KEY_LENGTH];
        JSONObjectKeys keys = data.Keys();
        while (keys.ReadKey(key, sizeof(key)))
        {
            char value[MAX_COOKIE_VALUE_LENGTH];
            data.GetString(key, value, sizeof(value));
            this.cookie.SetString(key, value, true);
        }
        delete keys;
    }

    bool Get(const char[] key, char[] value, int maxlen)
    {
        return this.cookie.GetString(key, value, maxlen);
    }

    bool Set(const char[] key, const char[] value)
    {
        return this.cookie.SetString(key, value, true);
    }

    void Clear()
    {
        this.index = 0;
        if (this.cookie != INVALID_HANDLE)
            delete this.cookie;
    }
}

CookieInfo player[MAXPLAYERS+1];

#include "cookie/api"
#include "cookie/native"