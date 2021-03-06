/*
 * Copyright (C) 2010-2014 Junjiro R. Okajima
 */

/*
 * dynamically customizable operations (for regular files only)
 */

#ifndef __AUFS_DYNOP_H__
#define __AUFS_DYNOP_H__

#ifdef __KERNEL__

#include "inode.h"

enum {AuDy_AOP, AuDyLast};

struct au_dynop {
	int						dy_type;
	union {
		const void				*dy_hop;
		const struct address_space_operations	*dy_haop;
	};
};

struct au_dykey {
	union {
		struct list_head	dk_list;
		struct rcu_head		dk_rcu;
	};
	struct au_dynop		dk_op;

	/*
	 * during I am in the branch local array, kref is gotten. when the
	 * branch is removed, kref is put.
	 */
	struct kref		dk_kref;
};

/* stop unioning since their sizes are very different from each other */
struct au_dyaop {
	struct au_dykey			da_key;
	struct address_space_operations	da_op; /* not const */
	int (*da_get_xip_mem)(struct address_space *, pgoff_t, int,
			      void **, unsigned long *);
};

/* ---------------------------------------------------------------------- */

/* dynop.c */
struct au_branch;
void au_dy_put(struct au_dykey *key);
int au_dy_iaop(struct inode *inode, aufs_bindex_t bindex,
		struct inode *h_inode);
int au_dy_irefresh(struct inode *inode);
void au_dy_arefresh(int do_dio);

void __init au_dy_init(void);
void au_dy_fin(void);

#endif /* __KERNEL__ */
#endif /* __AUFS_DYNOP_H__ */
