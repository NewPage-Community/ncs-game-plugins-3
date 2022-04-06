#include <files>

public void CheckLogsDirectory()
{
    char path[128];
    BuildPath(Path_SM, path, 128, "logs/NewPage");
    if(!DirExists(path))
        CreateDirectory(path, 755);
}

stock void NCS_LogError(const char[] module = "Global", const char[] func = "Invalid Function", const char[] msg, any ...)
{
    CheckLogsDirectory();

    char error[2048];
    VFormat(error, 2048, msg, 4);
    
    static char path[128];
    BuildPath(Path_SM, path, 128, "logs/NewPage/%s_err.log", module);
    
    LogToFileEx(path, "[%s] -> %s", func, error);
}

stock void NCS_LogMessage(const char[] module = "Global", const char[] func = "Invalid Function", const char[] msg, any ...)
{
    CheckLogsDirectory();
    
    char message[2048];
    VFormat(message, 2048, msg, 4);
    
    static char path[128];
    BuildPath(Path_SM, path, 128, "logs/NewPage/%s_msg.log", module);
    
    LogToFileEx(path, "[%s] -> %s", func, message);
}