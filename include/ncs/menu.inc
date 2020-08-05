methodmap MenuEx < Menu
{
	public MenuEx(MenuHandler handler, MenuAction actions=MENU_ACTIONS_DEFAULT)
	{
		return view_as<MenuEx>(new Menu(handler, actions));
	}

	public bool AddItemEx(int style, const char[] info, const char[] fmt, any ...)
	{
		char vf[256];
		VFormat(vf, sizeof(vf), fmt, 5);
		return this.AddItem(info, vf, style);
	}
}

methodmap PanelEx < Panel
{
	public PanelEx(Handle hStyle = null)
	{
		return view_as<PanelEx>(new Panel(hStyle));
	}

	public bool DrawTextEx(const char[] fmt, any ...)
	{
		char vf[256];
		VFormat(vf, sizeof(vf), fmt, 3);
		return this.DrawText(vf);
	}

	public int DrawItemEx(int style, const char[] fmt, any ...)
	{
		char vf[256];
		VFormat(vf, sizeof(vf), fmt, 4);
		return this.DrawItem(vf, style);
	}
}