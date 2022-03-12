void TranslateCmd(const char[] cmd, char[] buffer, int size)
{
    if (!strcmp(cmd, "affirmative")) strcopy(buffer, size, "肯定");
    else if (!strcmp(cmd, "negative")) strcopy(buffer, size, "否定");
    else if (!strcmp(cmd, "enemy")) strcopy(buffer, size, "发现敌人");
    else if (!strcmp(cmd, "moving")) strcopy(buffer, size, "正在前往");
    else if (!strcmp(cmd, "area_hostile")) strcopy(buffer, size, "有敌袭");
    else if (!strcmp(cmd, "need_backup")) strcopy(buffer, size, "需求支援");
    else if (!strcmp(cmd, "enemy_down")) strcopy(buffer, size, "击毙敌军");
    else if (!strcmp(cmd, "covering")) strcopy(buffer, size, "正在掩护");
    else if (!strcmp(cmd, "area_clear")) strcopy(buffer, size, "区域安全");
    else if (!strcmp(cmd, "ontheway")) strcopy(buffer, size, "正在执行");
    else if (!strcmp(cmd, "flank_left")) strcopy(buffer, size, "攻击左侧");
    else if (!strcmp(cmd, "flank_right")) strcopy(buffer, size, "攻击右侧");
    else if (!strcmp(cmd, "cease_fire")) strcopy(buffer, size, "停火");
    else if (!strcmp(cmd, "watch_area")) strcopy(buffer, size, "注意区域");
    else if (!strcmp(cmd, "go")) strcopy(buffer, size, "GO");
    else if (!strcmp(cmd, "report_status")) strcopy(buffer, size, "报告情况");
    else if (!strcmp(cmd, "stick_together")) strcopy(buffer, size, "保持队形");
    else if (!strcmp(cmd, "spread_out")) strcopy(buffer, size, "散开");
    else if (!strcmp(cmd, "hold_position")) strcopy(buffer, size, "坚守阵地");
    else if (!strcmp(cmd, "cheer")) strcopy(buffer, size, "欢呼");
}