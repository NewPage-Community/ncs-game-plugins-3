bool IsAdmin(int client)
{
    char auth[32];
    AdminId admin;
    GetClientAuthId(client, AuthId_Steam2, auth, sizeof(auth));

    if((admin = FindAdminByIdentity(AUTHMETHOD_STEAM, auth)) != INVALID_ADMIN_ID)
        return false;

    return admin.HasFlag(Admin_Generic, Access_Real);
}

bool IsOwner(int client)
{
    char auth[32];
    AdminId admin;
    GetClientAuthId(client, AuthId_Steam2, auth, sizeof(auth));

    if((admin = FindAdminByIdentity(AUTHMETHOD_STEAM, auth)) != INVALID_ADMIN_ID)
        return false;

    return admin.HasFlag(Admin_Root, Access_Real);
}