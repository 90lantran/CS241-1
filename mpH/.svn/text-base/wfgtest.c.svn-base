#include <stdio.h>
#include "libwfg.h"

int main()
{
	wfg_t wfg;

	wfg_init(&wfg);
	wfg_print_graph(&wfg);
	printf("-\n");

	wfg_add_wait_edge(&wfg, 2, 1);
	wfg_print_graph(&wfg);
	printf("-\n");
	/* 2 -> r1 */

	wfg_add_hold_edge(&wfg, 2, 1);
	wfg_print_graph(&wfg);
	printf("-\n");
	/* r1 -> 2 */

	wfg_add_wait_edge(&wfg, 1, 2);
	wfg_print_graph(&wfg);
	printf("-\n");
	/* r1 -> 2, 1 -> r2 */

	wfg_add_hold_edge(&wfg, 1, 2);
	wfg_print_graph(&wfg);
	printf("-\n");
	/* r1 -> 2, r2 -> 1 */
        

	wfg_add_wait_edge(&wfg, 2, 2);
	wfg_print_graph(&wfg);
	printf("-\n");
	/* r1 -> 2, r2 -> 1, 2 -> r2 */
	/* r1 -> 2 -> r2 -> 1 */

	wfg_add_wait_edge(&wfg, 1, 1);
	wfg_print_graph(&wfg);
	printf("-\n");
	/* r1 -> 2 -> r2 -> 1 -> r1 */

	wfg_remove_edge(&wfg, 1, 1);
	wfg_print_graph(&wfg);
	printf("-\n");

	wfg_destroy(&wfg);

	return 0;
}

