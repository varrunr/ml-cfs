/* LinSched -- The Linux Scheduler Simulator
 * Copyright (C) 2008  John M. Calandrino
 * E-mail: jmc@cs.unc.edu
 *
 * Example scheduling simulation. Tasks are created and then the simulation
 * is run for some number of ticks.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see COPYING); if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "linsched.h"
#include <stdio.h>
#include <linux/time.h>
void my_func()
{

	
	//long t = get_seconds();
	int i;
	//printf("curret time is %lu", t);
}



int main(int argc, char **argv)
{
	/* Initialize linsched. */
	struct elf_binary bin = { 400, 400, 400, &linsched_announce_callback};
	
	linsched_init();

	/* Create some tasks with "callbacks" that should be called
	 * every scheduling decision.
	 */
	linsched_create_normal_task_binary(&bin,0);
	linsched_create_normal_task(&linsched_announce_callback, 0);
	//linsched_create_normal_task(&linsched_announce_callback_1, 5);
	//linsched_create_normal_task(&linsched_announce_callback, 0);
	//linsched_create_normal_task(&linsched_announce_callback, -5);
	//linsched_create_normal_task(&linsched_announce_callback, 5);
	//linsched_create_normal_task(&linsched_announce_callback, 0);
	//linsched_create_batch_task(&linsched_announce_callback, 15);
	//linsched_create_batch_task(&linsched_announce_callback, -1);
	//linsched_create_RTfifo_task(&linsched_announce_callback, 90);
	//linsched_create_RTrr_task(&linsched_announce_callback, 55);
	// create more tasks here...

	/* Run simulation for 500 ticks. */
	linsched_run_sim(500);

	/* Force migrations between two CPUs, and allow migrations
	 * afterwards (so load balancing will kick in eventually).
	 */
	//linsched_force_migration(linsched_get_task(3), 0, 1);
	//linsched_force_migration(linsched_get_task(7), 2, 1);
	
	/* Run simulation to completion. */
	//linsched_run_sim(LINSCHED_TICKS-500); 
	return 0;

}
