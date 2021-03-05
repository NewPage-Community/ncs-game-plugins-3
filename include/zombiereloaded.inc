/*
 * ============================================================================
 *
 *  Zombie:Reloaded
 *
 *  File:		  zombiereloaded.inc
 *  Type:		  Include
 *  Description:   Main API include file.
 *  Notes:		 Include this file to include the whole ZR API. 
 *
 *  Copyright (C) 2009-2013  Greyscale, Richard Helgeby
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * ============================================================================
 */

#if defined _zr_included
 #endinput
#endif
#define _zr_included

/**
 * Returns true if the player is a zombie, false if not.
 *
 * @param client			The client index.
 *  
 * @return					True if zombie, false if not.
 * @error				   Invalid client index, not connected or not alive.
 */
native bool ZR_IsClientZombie(int client);

/**
 * Returns true if the player is a human, false if not.
 *
 * @param client			The client index.
 *  
 * @return					True if human, false if not.
 * @error				   Invalid client index, not connected or not alive.
 */
native bool ZR_IsClientHuman(int client);

/**
 * Called after a player has become a zombie.
 * 
 * @param client			The client that was infected.
 * @param attacker		  The the infecter. (-1 if there is no infecter)
 * @param motherInfect	  If the client is a mother zombie.
 * @param respawnOverride   True if the respawn cvar was overridden.
 * @param respawn		   The value that respawn was overridden with.
 */
forward void ZR_OnClientInfected(int client, int attacker, bool motherInfect, bool respawnOverride, bool respawn);
