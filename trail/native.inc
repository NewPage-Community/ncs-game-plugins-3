void InitNative()
{
	CreateNative("NCS_Trail_Preview", Native_Preview);
}

public int Native_Preview(Handle plugin, int numParams)
{
	int client = GetNativeCell(1);
	int trailID = GetNativeCell(2);
	SetClientTrail(client, trailID, true);
	return 0;
}