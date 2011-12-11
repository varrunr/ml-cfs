# John Calandrino - LinSched Makefile

PROGS = linsched
CC = gcc
LINUXDIR = linux-2.6.23.14
CFLAGS = -g -D__KERNEL__ -D__LINSCHED__ -Wall -Wundef -Wstrict-prototypes -fno-common -Werror-implicit-function-declaration -I${LINUXDIR}/include -I${LINUXDIR}/include/asm-i386/mach-default -include ${LINUXDIR}/include/linux/autoconf.h -include linux_linsched.h -Wno-pointer-sign
CLEANFILES = *.o *.out *~
OBJFILES = linsched.o linux_linsched.o ${LINUXDIR}/kernel/sched.o ${LINUXDIR}/kernel/fork.o ${LINUXDIR}/kernel/exit.o ${LINUXDIR}/kernel/time.o ${LINUXDIR}/arch/i386/kernel/init_task.o ${LINUXDIR}/init/main.o ${LINUXDIR}/lib/rbtree.o ${LINUXDIR}/lib/div64.o ${LINUXDIR}/lib/cpumask.o ${LINUXDIR}/lib/bitmap.o ${LINUXDIR}/lib/list_debug.o ${LINUXDIR}/lib/find_next_bit.o ${LINUXDIR}/lib/hweight.o ${LINUXDIR}/lib/reciprocal_div.o

all:		${PROGS}

linsched:	${OBJFILES}
		${CC} ${LFLAGS} -o $@ ${OBJFILES}

%.o:		%.c
		${CC} -o $*.o ${CFLAGS} -c $*.c

clean:
		rm -f ${PROGS} ${OBJFILES} ${CLEANFILES}

bkup:
		tar cvjf linsched_${LINUXDIR}_bkup.bz2 *
