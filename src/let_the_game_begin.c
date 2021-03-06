/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   let_the_game_begin.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aybouras <aybouras@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/02/15 11:23:32 by aybouras          #+#    #+#             */
/*   Updated: 2021/02/15 11:24:00 by aybouras         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/cor.h"

void	exec_operation()
{

}

void kill_cursor(t_vm *vm, t_cursor **this, t_cursor **prev)
{
	t_cursor *temp;

	if (vm->cursors == *this)
	{
		vm->cursors = (*this)->next;
		temp = vm->cursors;
	}
	else if (*prev)
	{
		(*prev)->next = (*this)->next;
		temp = (*prev)->next;

	}
	free(*this);
	*this = NULL;
	*this = temp;
	vm->cursors_counter--;
}

void	check_who_is_alive(t_vm *vm)
{
	t_cursor *this;
	t_cursor *prev;

	this = vm->cursors;
	prev = NULL;
	while (this)
	{
		if (!this->is_alive)
			kill_cursor(&vm, &this, &prev);
		else
		{
			this->is_alive = FALSE;
			prev = this;
			this = this->next;
		}
	}
}

void loop_through_cursors(t_vm *vm)
{
	t_cursor	*cursor;
	int			adr;

	cursor = vm->cursors;
	while (cursor)
	{
		// convert from hexa to int
		adr = vm->colosseum[cursor->current_addr];
		if (adr && adr <= REG_NUMBER)
		{
			if (cursor->wait_cycles == -1)
				cursor->wait_cycles = op_tab[adr - 1].cycles_to_wait;// opcode starts from 0
			if (!cursor->wait_cycles)
				exec_operation();
			else
				cursor->wait_cycles--;
		}
		else
			cursor->current_addr++;
		cursor = cursor->next;
	}
}

void	performe_check(t_vm *vm)
{
	if ((vm->cycles == vm->cycles_last_check + vm->cycles_to_die) || vm->cycles_to_die <= 0)
	{
		vm->cycles_last_check = vm->cycles;
		vm->count_live_checks++;
		check_who_is_alive();
		if (vm->lives_counter >= NBR_LIVE || vm->count_live_checks == MAX_CHECKS)
		{
			vm->count_live_checks = 0;
			vm->lives_counter = 0;
			vm->cycles_to_die -= CYCLE_DELTA;
		}
	}
}

void let_the_game_begin(t_vm *vm)
{
	while (++vm->cycles)
	{
		loop_through_cursors(vm);
		performe_check(vm);

		if (vm->cycles == 100)
		{
			printf("100 cycles \n");
			exit(0);
		}
	}
}

// If -dump flag is present The memory must be dumped in the hexadecimal
// format with 32 octets per line.

// if ((int)vm->current_cycle == vm->dump_cycle)
// 			dump_memory(vm);

// ***********//
// if (g_total_cycle == g_dump_core)
// 		hexdumper(g_memory);


