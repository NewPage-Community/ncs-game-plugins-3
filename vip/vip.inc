enum struct vip
{
    int index;
    int point;
    char expireDate[INT64_LENGTH];

    int Level() {
        if (!this.IsValid())
            return 0;
        return this.point / 200 + 1;
    }

    bool IsValid() {
        if (this.index == 0)
            return false;
        return IsValidClient(this.index);
    }

    bool IsVIP() {
        if (!this.IsValid())
            return false;
        char now[INT64_LENGTH];
        GetTimeStamp(now, sizeof(now))
        return (strcmp(now, this.expireDate) == -1);
    }

    void Init(int index, int point, const char[] expireDate) {
        this.index = index;
        this.point = point;
        strcopy(this.expireDate, INT64_LENGTH, expireDate);
    }

    void Renewal(const char[] expireDate) {
        strcopy(this.expireDate, INT64_LENGTH, expireDate);
    }

    void Clean() {
        this.index = 0;
        this.point = 0;
        this.expireDate[0] = '\0';
    }
}

vip g_VIPs[MAXPLAYERS + 1];

#include "vip/api"
#include "vip/native"