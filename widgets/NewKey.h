/* vi: set sw=4 ts=4:
 *
 * Copyright (C) 2001 - 2007 Christian Hohnstaedt.
 *
 * All rights reserved.
 */

#ifndef __NEWKEY_H
#define __NEWKEY_H

#include "ui_NewKey.h"

class NewKey: public QDialog, public Ui::NewKey
{
	Q_OBJECT

	public:
		NewKey(QWidget *parent, QString name);
		int getKeytype();
		int getKeysize();
		int getKeyCurve_nid();
	public slots:
		void on_keyType_currentIndexChanged(int);

};
#endif