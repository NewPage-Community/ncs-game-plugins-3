#include <handles>

// SourceMOD native hack
stock void DrawPanelTextEx(Handle panel, const char[] fmt, any ...)
{
	char vf[256];
	VFormat(vf, 256, fmt, 3);
	DrawPanelText(panel, vf);
}

stock bool AddMenuItemEx(Handle menu, int style, const char[] info, const char[] display, any ...)
{
	char m_szBuffer[256];
	VFormat(m_szBuffer, 256, display, 5);
	return AddMenuItem(menu, info, m_szBuffer, style);
}