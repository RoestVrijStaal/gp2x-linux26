/*
 * lnbp21.h - driver for lnb supply and control ic lnbp21
 *
 * Copyright (C) 2006 Oliver Endriss
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 * Or, point your browser to http://www.gnu.org/copyleft/gpl.html
 *
 *
 * the project's page is at http://www.linuxtv.org
 */
#include <linux/delay.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/string.h>
#include <linux/slab.h>

#include "dvb_frontend.h"
#include "lnbp21.h"

struct lnbp21 {
	u8			config;
	u8			override_or;
	u8			override_and;
	struct i2c_adapter	*i2c;
	void			(*release_chain)(struct dvb_frontend* fe);
};

static int lnbp21_set_voltage(struct dvb_frontend *fe, fe_sec_voltage_t voltage)
{
	struct lnbp21 *lnbp21 = (struct lnbp21 *) fe->misc_priv;
	struct i2c_msg msg = {	.addr = 0x08, .flags = 0,
				.buf = &lnbp21->config,
				.len = sizeof(lnbp21->config) };

	lnbp21->config &= ~(LNBP21_VSEL | LNBP21_EN);

	switch(voltage) {
	case SEC_VOLTAGE_OFF:
		break;
	case SEC_VOLTAGE_13:
		lnbp21->config |= LNBP21_EN;
		break;
	case SEC_VOLTAGE_18:
		lnbp21->config |= (LNBP21_EN | LNBP21_VSEL);
		break;
	default:
		return -EINVAL;
	};

	lnbp21->config |= lnbp21->override_or;
	lnbp21->config &= lnbp21->override_and;

	return (i2c_transfer(lnbp21->i2c, &msg, 1) == 1) ? 0 : -EIO;
}

static int lnbp21_enable_high_lnb_voltage(struct dvb_frontend *fe, long arg)
{
	struct lnbp21 *lnbp21 = (struct lnbp21 *) fe->misc_priv;
	struct i2c_msg msg = {	.addr = 0x08, .flags = 0,
				.buf = &lnbp21->config,
				.len = sizeof(lnbp21->config) };

	if (arg)
		lnbp21->config |= LNBP21_LLC;
	else
		lnbp21->config &= ~LNBP21_LLC;

	lnbp21->config |= lnbp21->override_or;
	lnbp21->config &= lnbp21->override_and;

	return (i2c_transfer(lnbp21->i2c, &msg, 1) == 1) ? 0 : -EIO;
}

static void lnbp21_release(struct dvb_frontend *fe)
{
	struct lnbp21 *lnbp21 = (struct lnbp21 *) fe->misc_priv;

	/* LNBP power off */
	lnbp21_set_voltage(fe, SEC_VOLTAGE_OFF);

	/* free data & call next release routine */
	fe->ops.release = lnbp21->release_chain;
	kfree(fe->misc_priv);
	fe->misc_priv = NULL;
	if (fe->ops.release)
		fe->ops.release(fe);
}

int lnbp21_attach(struct dvb_frontend *fe, struct i2c_adapter *i2c, u8 override_set, u8 override_clear)
{
	struct lnbp21 *lnbp21 = kmalloc(sizeof(struct lnbp21), GFP_KERNEL);
	if (!lnbp21)
		return -ENOMEM;

	/* default configuration */
	lnbp21->config = LNBP21_ISEL;
	lnbp21->i2c = i2c;
	fe->misc_priv = lnbp21;

	/* bits which should be forced to '1' */
	lnbp21->override_or = override_set;

	/* bits which should be forced to '0' */
	lnbp21->override_and = ~override_clear;

	/* detect if it is present or not */
	if (lnbp21_set_voltage(fe, SEC_VOLTAGE_OFF)) {
		kfree(lnbp21);
		fe->misc_priv = NULL;
		return -EIO;
	}

	/* install release callback */
	lnbp21->release_chain = fe->ops.release;
	fe->ops.release = lnbp21_release;

	/* override frontend ops */
	fe->ops.set_voltage = lnbp21_set_voltage;
	fe->ops.enable_high_lnb_voltage = lnbp21_enable_high_lnb_voltage;

	return 0;
}
EXPORT_SYMBOL(lnbp21_attach);

MODULE_DESCRIPTION("Driver for lnb supply and control ic lnbp21");
MODULE_AUTHOR("Oliver Endriss");
MODULE_LICENSE("GPL");