/*-
 * Copyright (c) 2011-2013 Hans Petter Selasky <hps@bitfrost.no>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "qmbox.h"

QFont font_fixed (QString("Courier New"));

QMBoxAddDialog :: QMBoxAddDialog()
{
	setWindowTitle(tr("Configure Carry and Modulus"));

	gridMain = new QGridLayout(this);

	lblCarry = new QLabel(tr("Carry (... -3,-1,1,3,5 ...)"));
	ledCarry = new QLineEdit();

	lblMod = new QLabel(tr("Modulus (... -2,-1,0,1,2,3 ...)"));
	ledMod = new QLineEdit();

	lblBase = new QLabel(tr("Base (... 2,3,4,5 ...)"));
	ledBase = new QLineEdit();

	btnOK = new QPushButton(tr("OK"));
	connect(btnOK, SIGNAL(released()), this, SLOT(accept()));

	btnCancel = new QPushButton(tr("Cancel"));
	connect(btnCancel, SIGNAL(released()), this, SLOT(reject()));

	gridMain->addWidget(lblCarry, 0, 0);
	gridMain->addWidget(ledCarry, 0, 1);

	gridMain->addWidget(lblMod, 1, 0);
	gridMain->addWidget(ledMod, 1, 1);

	gridMain->addWidget(lblBase, 2, 0);
	gridMain->addWidget(ledBase, 2, 1);

	gridMain->addWidget(btnOK, 3, 0);
	gridMain->addWidget(btnCancel, 3, 1);
}

QMBoxAddDialog :: ~QMBoxAddDialog()
{

}

QMBoxCrcDialog :: QMBoxCrcDialog()
{
	setWindowTitle(tr("Configure CRC"));

	gridMain = new QGridLayout(this);

	lblCrcBase = new QLabel(tr("CRC BASE (2,3,4...)"));
	lblCrcPoly = new QLabel(tr("CRC POLYNOM"));
	lblCrcMod = new QLabel(tr("CRC CIRCLE LENGTH"));
	lblCrcOddxor = new QLabel(tr("CRC ODD XOR"));

	ledCrcBase = new QLineEdit();
	ledCrcPoly = new QLineEdit();
	ledCrcMod = new QLineEdit();
	ledCrcOddxor = new QLineEdit();

	btnOK = new QPushButton(tr("OK"));
	connect(btnOK, SIGNAL(released()), this, SLOT(accept()));

	btnCancel = new QPushButton(tr("Cancel"));
	connect(btnCancel, SIGNAL(released()), this, SLOT(reject()));

	gridMain->addWidget(lblCrcBase, 0, 0);
	gridMain->addWidget(lblCrcPoly, 1, 0);
	gridMain->addWidget(lblCrcMod, 2, 0);
	gridMain->addWidget(lblCrcOddxor, 3, 0);

	gridMain->addWidget(ledCrcBase, 0, 1);
	gridMain->addWidget(ledCrcPoly, 1, 1);
	gridMain->addWidget(ledCrcMod, 2, 1);
	gridMain->addWidget(ledCrcOddxor, 3, 1);

	gridMain->addWidget(btnOK, 4, 0);
	gridMain->addWidget(btnCancel, 4, 1);
}

QMBoxCrcDialog :: ~QMBoxCrcDialog()
{

}

QMBoxPwrDialog :: QMBoxPwrDialog()
{
	setWindowTitle(tr("Configure PWR"));

	gridMain = new QGridLayout(this);

	lblPwr = new QLabel(tr("POWER"));

	ledPwr = new QLineEdit(tr("1"));

	btnOK = new QPushButton(tr("OK"));
	connect(btnOK, SIGNAL(released()), this, SLOT(accept()));

	btnCancel = new QPushButton(tr("Cancel"));
	connect(btnCancel, SIGNAL(released()), this, SLOT(reject()));

	gridMain->addWidget(lblPwr, 0, 0);

	gridMain->addWidget(ledPwr, 0, 1);

	gridMain->addWidget(btnOK, 1, 0);
	gridMain->addWidget(btnCancel, 1, 1);
}

QMBoxPwrDialog :: ~QMBoxPwrDialog()
{

}

QMBoxMainWindow :: QMBoxMainWindow(QWidget *parent)
  : QWidget(parent)
{
	uint32_t x;

	/* Init variables */

	viewMode = TYPE_NORMAL;
	exprMode = TYPE_NORMAL;
	sortMode = SORT_INC;
	usedPtr = 0;
	dataPtr = 0;
	exprPtr = 0;

	addCarry = 1;
	addMod = 0;
	addBase = 2;

	crcBase = 2;
	crcPolyMsb = 0;
	crcPoly = 0;
	crcModLen = 0;
	crcOddxor = 0;
	crcLimit = 1ULL << 63;
	xformPwr = 1;

	/* Setup GUI */

	gridMain = new QGridLayout(this);

	for (x = 0; x != QMBOX_MAX_INVERT; x++) {
		cbxInvert[x] = new QCheckBox();
		cbxAdd[x] = new QCheckBox();
		lblInvert[x] = new QLabel(QChar('a' + x));

		gridMain->addWidget(lblInvert[x], 2, 3 + x, 1, 1);
		gridMain->addWidget(cbxInvert[x], 3, 3 + x, 1, 1);
		gridMain->addWidget(cbxAdd[x], 4, 3 + x, 1, 1);

		connect(cbxInvert[x], SIGNAL(stateChanged(int)), this, SLOT(handleCbxChanged(int)));
		connect(cbxAdd[x], SIGNAL(stateChanged(int)), this, SLOT(handleCbxChanged(int)));
	}

	butCrcConfig = new QPushButton(tr("CRC CONFIG"));
	connect(butCrcConfig, SIGNAL(released()), this, SLOT(handleCrcConfig(void)));
	gridMain->addWidget(butCrcConfig, 4, 5 + QMBOX_MAX_INVERT, 1, 1);

	butAddConfig = new QPushButton(tr("ADD CONFIG"));
	gridMain->addWidget(butAddConfig, 4, 6 + QMBOX_MAX_INVERT, 1, 1);
	connect(butAddConfig, SIGNAL(released()), this, SLOT(handleAddConfig(void)));

	lblValue = new QLabel(tr("Value"));
	gridMain->addWidget(lblValue, 0, 0, 1, 1);

	lblExpression = new QLabel(tr("Expression"));
	gridMain->addWidget(lblExpression, 1, 0, 1, 1);

	lblStatus = new QLabel(QString());
	gridMain->addWidget(lblStatus, 1, 4 + (QMBOX_MAX_INVERT / 2), 1, (QMBOX_MAX_INVERT / 2) + 1);

	lblXorBits = new QLabel(tr("Inverted bits"));
	gridMain->addWidget(lblXorBits, 3, 1, 1, 1, Qt::AlignRight);

	lblAddBits = new QLabel(tr("Added bits"));
	gridMain->addWidget(lblAddBits, 4, 1, 1, 1, Qt::AlignRight);

	ledValue = new QLineEdit();
	gridMain->addWidget(ledValue, 0, 1, 1, 3 + QMBOX_MAX_INVERT);

	ledExpression = new QLineEdit();
	gridMain->addWidget(ledExpression, 1, 1, 1, (QMBOX_MAX_INVERT / 2) + 2);
	connect(ledExpression, SIGNAL(textChanged(const QString &)), this, SLOT(handleExprChanged(const QString &)));

	butDiff = new QPushButton(tr("Diff"));
	gridMain->addWidget(butDiff, 0, 7 + QMBOX_MAX_INVERT, 1, 1);
	connect(butDiff, SIGNAL(pressed()), this, SLOT(handleDiff()));

	butInteg = new QPushButton(tr("Integ"));
	gridMain->addWidget(butInteg, 1, 7 + QMBOX_MAX_INVERT, 1, 1);
	connect(butInteg, SIGNAL(pressed()), this, SLOT(handleInteg()));

	butSortInc = new QPushButton(tr("SortInc"));
	gridMain->addWidget(butSortInc, 0, 6 + QMBOX_MAX_INVERT, 1, 1);
	connect(butSortInc, SIGNAL(pressed()), this, SLOT(handleSortInc()));

	butSortDec = new QPushButton(tr("SortDec"));
	gridMain->addWidget(butSortDec, 1, 6 + QMBOX_MAX_INVERT, 1, 1);
	connect(butSortDec, SIGNAL(pressed()), this, SLOT(handleSortDec()));

	butViewMode = new QPushButton(tr("View Normal"));
	gridMain->addWidget(butViewMode, 2, 6 + QMBOX_MAX_INVERT, 1, 1);
	connect(butViewMode, SIGNAL(pressed()), this, SLOT(handleViewMode()));

	butExprMode = new QPushButton(tr("Expr Normal"));
	gridMain->addWidget(butExprMode, 3, 6 + QMBOX_MAX_INVERT, 1, 1);
	connect(butExprMode, SIGNAL(pressed()), this, SLOT(handleExprMode()));

	butPolMode = new QPushButton(tr("DePolarise"));
	gridMain->addWidget(butPolMode, 1, 5 + QMBOX_MAX_INVERT, 1, 1);
	connect(butPolMode, SIGNAL(pressed()), this, SLOT(handlePolMode()));

	butCleanup = new QPushButton(tr("Cleanup Unused"));
	gridMain->addWidget(butCleanup, 0, 5 + QMBOX_MAX_INVERT, 1, 1);
	connect(butCleanup, SIGNAL(pressed()), this, SLOT(handleCleanup()));

	butPwrConfig = new QPushButton(tr("PWR CONFIG"));
	connect(butPwrConfig, SIGNAL(released()), this, SLOT(handlePwrConfig(void)));
	gridMain->addWidget(butPwrConfig, 2, 0, 1, 1);

	butClearInput = new QPushButton(tr("Clear Input"));
	gridMain->addWidget(butClearInput, 3, 0, 1, 1);
	connect(butClearInput, SIGNAL(pressed()), this, SLOT(handleClearInput()));

	butAddExpression = new QPushButton(tr("&Add Expr"));
	gridMain->addWidget(butAddExpression, 2, 5 + QMBOX_MAX_INVERT, 1, 1);
	connect(butAddExpression, SIGNAL(pressed()), this, SLOT(handleAddExpression()));

	butSubExpression = new QPushButton(tr("&Sub Expr"));
	gridMain->addWidget(butSubExpression, 3, 5 + QMBOX_MAX_INVERT, 1, 1);
	connect(butSubExpression, SIGNAL(pressed()), this, SLOT(handleSubExpression()));

	butImport = new QPushButton(tr("->>"));
	gridMain->addWidget(butImport, 4, 0, 1, 1);
	connect(butImport, SIGNAL(pressed()), this, SLOT(handleImport()));

	edtInput = new QPlainTextEdit();
	edtInput->setFont(font_fixed);
	edtInput->setLineWrapMode(QPlainTextEdit::NoWrap);
	gridMain->addWidget(edtInput, 5, 0, 1, 2);

	edtOutput = new QPlainTextEdit();
	edtOutput->setFont(font_fixed);
	edtOutput->setLineWrapMode(QPlainTextEdit::NoWrap);
	gridMain->addWidget(edtOutput, 5, 2, 1, 6 + QMBOX_MAX_INVERT);

	setWindowTitle(tr("Qt Math Box v1.2"));

	gridMain->setColumnStretch(2, 2);
	gridMain->setRowStretch(4, 2);

	clearInputList();
}

QMBoxMainWindow :: ~QMBoxMainWindow()
{
	clearInputList();
}

int64_t
QMBoxMainWindow :: getValue()
{
	long long value = 0;
	QString text;
	QChar *ch;
	char linebuffer[128];
	int offset;
	int c;
 
	text = ledValue->text();

	ch = text.data();
	
	offset = 0;
	
	while (ch->isNull() == 0) {

		c = ch->toLatin1();
		if (c == 0)
			c = ' ';

		if (offset < (int)(sizeof(linebuffer) - 1))
			linebuffer[offset++] = c;

		ch++;
	}

	linebuffer[offset] = 0;

	if (sscanf(linebuffer, "0x%llx", &value) != 1 &&
	    sscanf(linebuffer, "%lld", &value) != 1)
		return (0);

	return (value);
}

uint32_t
QMBoxMainWindow :: getExpr()
{
	QString text;
	QChar *ch;
	uint32_t expr;
	int c;
 
	expr = 0;
	text = ledExpression->text();

	ch = text.data();

	while (ch->isNull() == 0) {

	       c = ch->toLatin1();
	       if (c >= 'a' && c <= 'z')
		       expr |= 1 << (c - 'a');
	       if (c >= 'A' && c <= 'A' + (31 - ('z' - 'a')))
			expr |= 1 << (c - 'A' + ('z' - 'a'));

	       ch++;
	}
	return (expr);
}

void
QMBoxMainWindow :: clearInputList(void)
{
	uint32_t x;

	free(usedPtr);
	usedPtr = 0;
	free(dataPtr);
	dataPtr = 0;
	free(exprPtr);
	exprPtr = 0;
	dataLenRealLin = 0;
	dataLenDisplay = 0;
	dataLenRealLog = 0;
	invertMask = 0;
	addMask = 0;

	for (x = 0; x != QMBOX_MAX_INVERT; x++) {
		cbxInvert[x]->setChecked(0);
		cbxAdd[x]->setChecked(0);
	}
}

void
QMBoxMainWindow :: parseInputList(QString str)
{
	QChar *ch;
	char *pp;
	char linebuffer[128];
	uint32_t count;
	int offset;
	int c;
	int round;
	long long value;
	uint8_t used;

	clearInputList();

	str += "\n";

	for (round = 0; round != 2; round++) {

		ch = str.data();
		offset = 0;
		count = 0;

		while (ch->isNull() == 0) {

			c = ch->toLatin1();
			if (c == 0)
				c = ' ';

			if (c == '\n') {

				linebuffer[offset] = 0;

				if (linebuffer[0] >= '0' && linebuffer[0] <= '9') {
					pp = linebuffer;
					value = strtoll(pp, &pp, 0);

					while (pp[0] == ' ' || pp[0] == '\t')
					    pp++;

					if (pp[0] >= '0' && pp[0] <= '9')
						used = strtoll(pp, &pp, 0) ? 1 : 0;
					else
						used = 1;

					if (round == 1) {
						dataPtr[count] = value;
						usedPtr[count] = used;
					}
					count++;
				}

				offset = 0;
				ch++;
				continue;
			}

			if (offset < (int)(sizeof(linebuffer) - 1))
				linebuffer[offset++] = c;

			ch++;
		}

		if (round == 0 && count != 0) {
			dataLenDisplay = count;
			while (((-count) & count) != count)
				count += ~(count - 1) & count;
			dataLenRealLin = count;
			if (count == 0)
				break;
			dataLenRealLog = 0;
			while (count != (1U << dataLenRealLog))
				dataLenRealLog++;

			usedPtr = (uint8_t *)malloc(count * sizeof(uint8_t));
			if (usedPtr == 0)
				break;
			dataPtr = (int64_t *)malloc(count * sizeof(int64_t));
			if (dataPtr == 0)
				break;
			exprPtr = (int64_t *)malloc(count * sizeof(int64_t));
			if (exprPtr == 0)
				break;
		}
		if (round == 0 && count == 0) {
			break;
		}
		if (round == 1) {
			while (count < dataLenRealLin) {
				dataPtr[count] = 0;
				usedPtr[count] = 0;
				count++;
			}
		}
	}

	if (round != 2)
		clearInputList();
}

void
QMBoxMainWindow :: modifyInputList(uint32_t expr, int64_t value,
    uint8_t method, uint8_t is_neg)
{
	if (expr >= dataLenRealLin)
		return;

	if (is_neg) {
		switch (method) {
		case TYPE_CRC_AND:
		case TYPE_CRC_OR:
		case TYPE_CRC_SUMBITS:
			value = crcNeg(value);
			break;
		default:
			value = -value;
			break;
		}
	}
	switch (method) {
	case TYPE_NORMAL:
		dataPtr[expr] += value;
		break;
	case TYPE_XOR_OR:
		expandXorOr(dataPtr, expr, value,
			dataLenRealLog);
		break;
	case TYPE_XOR_AND:
		expandXorAnd(dataPtr, expr, value,
			dataLenRealLog);
		break;
	case TYPE_CRC_AND:
		expandCrcAnd(dataPtr, expr, value,
			dataLenRealLog);
		break;
	case TYPE_CRC_OR:
		expandRevCrcAnd(dataPtr, expr, value,
			dataLenRealLog);
		break;
	case TYPE_ADD_AND:
		expandAddAnd(dataPtr, expr, value,
			dataLenRealLog);
		break;
	case TYPE_ADD_OR:
		expandRevAddAnd(dataPtr, expr, value,
			dataLenRealLog);
		break;
	case TYPE_CRC_SUMBITS:
		expandCrcSumbits(dataPtr, expr, value,
			dataLenRealLog);
		break;
	case TYPE_AND_SUMBITS:
		expandAndSumbits(dataPtr, expr, value,
			dataLenRealLog);
		break;
	case TYPE_MOD_SUMBITS:
		/* TODO */
		break;
	}
}

int64_t
QMBoxMainWindow :: addNeg(int64_t x)
{
	int64_t r;
	int64_t k;
	int64_t kp;

	if (addCarry == 1) {
		if (addMod == 0)
			return (-x);
		else
			return ((addMod - (x % addMod)) % addMod);
	}

	r = 0;
	k = 1;

	while (1) {
		kp = k;
		k *= addBase;
		if (kp != (k / addBase))
			break;
	}

	kp--;

	while (kp != 0) {
		if (kp & 1)
			r = add(r, x);
		x = add(x, x);
		kp /= 2;
	}
	return (r);
}

int64_t
QMBoxMainWindow :: add(int64_t x, int64_t y)
{
	int64_t r;
	int64_t c;
	int64_t k;
	int64_t kn;

	if (addCarry == 1) {
		if (addMod == 0)
			return (x + y);
		else
			return ((x + y) % addMod);
	}

	c = r = 0;
	k = 1;

	while (1)
	{
		kn = k * addBase;
		if (k != (kn / addBase))
			break;

		c = c + (x % addBase) + (y % addBase);
		x /= addBase;
		y /= addBase;

		if (c < 0)
			r += ((addBase - (c % addBase)) % addBase) * k;
		else
			r += (c % addBase) * k;

		c -= (c % addBase);
		c = (c / addBase) * addCarry;
		k = kn;
	}
	return (r);
}

uint64_t
QMBoxMainWindow :: crcXor(uint64_t a, uint64_t b)
{
	uint64_t k = 1;
	uint64_t r = 0;

	if (crcBase == 2)
		return ((a ^ b) % crcLimit);

	while ((a || b) && k) {
		r += (((a % crcBase) + (b % crcBase)) % crcBase) * k;
		a /= crcBase;
		b /= crcBase;
		k *= crcBase;
		k %= crcLimit;
	}
	return (r);
}

uint64_t
QMBoxMainWindow :: crcNeg(uint64_t x)
{
	uint64_t r = 1;
	uint64_t y;

	if (crcPoly == 0)
		y = (crcLimit - 1ULL);
	else
		y = (crcModLen - 1ULL);

	while (y != 0 && x != 1) {
		if (y & 1ULL)
			r = crcAdd(r, x);
		x = crcAdd(x, x);
		y /= 2ULL;
	}
	if (r & 1)
		r = crcXor(r, crcOddxor);
	return (r);
}

uint8_t
QMBoxMainWindow :: crcNonZero(uint64_t x)
{
	if (crcPoly == 0) {
		return (x > 1);
	} else if ((crcPolyMsb + 1) == crcPoly && crcBase == 2) {
		x = sumbits64(x);
		return (x > 1 && x < sumbits64(crcPolyMsb - 2ULL));
	} else {
		return (crcNeg(x) != x);
	}
}

uint64_t
QMBoxMainWindow :: crcAdd(uint64_t x, uint64_t y)
{
	uint64_t r = 0;
	uint64_t to;

	while (y != 0) {
		while (y % crcBase) {
			r = crcXor(r, x);
			y--;
		}

		y /= crcBase;
		x *= crcBase;
		x %= crcLimit;

		if (crcPoly != 0) {

			to = crcBase;

			while (x >= crcPolyMsb) {
				if (!to--) {
					crcFailure++;
					break;
				}
				x = crcXor(x, crcPoly);
			}
		}
	}
	if (r == 0)
		crcFailure++;
	if (r & 1)
		r = crcXor(r, crcOddxor);
	return (r);
}

/*
 * Inverse CRC AND transform.
 *
 * f(x,y) = ((x & y) == y) ? 1 : 0;
 */
void
QMBoxMainWindow :: inverseCrcXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z + (x / 2)] = crcAdd(b, crcNeg(a));
			}
		}
	}
}

/*
 * Inverse reverse CRC OR transform.
 *
 * f(x,y) = ((x | y) == y) ? 1 : 0;
 */
void
QMBoxMainWindow :: inverseRevCrcXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z] = crcAdd(a, crcNeg(b));
			}
		}
	}
}

/*
 * Forward CRC AND transform.
 *
 * f(x,y) = ((x & y) == y) ? 1 : 0;
 */
void
QMBoxMainWindow :: forwardCrcXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z + (x / 2)] = crcAdd(a, b);
			}
		}
	}
}

/*
 * Forward reverse CRC OR transform.
 *
 * f(x,y) = ((x | y) == y) ? 1 : 0;
 */
void
QMBoxMainWindow :: forwardRevCrcXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z] = crcAdd(a, b);
			}
		}
	}

	/* reverse samples */
	for (x = 0; x != (max / 2); x++) {
		a = ptr[x];
		ptr[x] = ptr[max-1-x];
		ptr[max-1-x]= a;
	}
}

/*
 * Inverse additive transform.
 *
 * f(x,y) = ((x & y) == y) ? 1 : 0;
 */
void
QMBoxMainWindow :: inverseAddXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;
	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z + (x / 2)] = add(b, addNeg(a));
			}
		}
	}
}

/*
 * Inverse reverse additive transform.
 *
 * f(x,y) = ((x | y) == y) ? 1 : 0;
 */
void
QMBoxMainWindow :: inverseRevAddXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z] = add(a, addNeg(b));
			}
		}
	}
}

/*
 * Forward additive transform.
 *
 * f(x,y) = ((x & y) == y) ? 1 : 0;
 */
void
QMBoxMainWindow :: forwardAddXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z + (x / 2)] = add(a, b);
			}
		}
	}
}

/*
 * Forward reverse additive transform.
 *
 * f(x,y) = ((x | y) == y) ? 1 : 0;
 */
void
QMBoxMainWindow :: forwardRevAddXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z] = add(a, b);
			}
		}
	}

	/* reverse samples */
	for (x = 0; x != (max / 2); x++) {
		a = ptr[x];
		ptr[x] = ptr[max-1-x];
		ptr[max-1-x]= a;
	}
}

/*
 * Xor Or transform.
 *
 * f(x,y) = ((x | y) == y) ? 1 : 0;
 */
void
QMBoxMainWindow :: orXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z] = a ^ b;
			}
		}
	}
}

/*
 * Exclusive-or transform.
 *
 * f(x,y) = ((x & y) == y) ? 1 : 0;
 */
void
QMBoxMainWindow :: xorXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z + (x / 2)] = a ^ b;
			}
		}
	}
}

/*
 * Sumbits-CRC transform
 *
 * f(x,y) = (sumbits32(x & y) & 1) ? -1 : 1;
 */
void
QMBoxMainWindow :: sumbitsCrcXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z] = crcAdd(a, b);
				ptr[y + z + (x / 2)] =
				    crcAdd(a, crcNeg(b));
			}
		}
	}
}

/*
 * Sumbits-and transform
 *
 * f(x,y) = (sumbits32(x & y) & 1) ? -1 : 1;
 */
void
QMBoxMainWindow :: sumbitsAndXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	uint32_t y;
	uint32_t z;
	int64_t a;
	int64_t b;

	for (x = 2; x <= max; x *= 2) {
		for (y = 0; y != max; y += x) {
			for (z = 0; z != (x / 2); z++) {
				a = ptr[y + z];
				b = ptr[y + z + (x / 2)];
				ptr[y + z] = add(a, b);
				ptr[y + z + (x / 2)] = add(a, addNeg(b));
			}
		}
	}
}

/*
 * Sumbits-mod transform (XOR correlation)
 */
void
QMBoxMainWindow :: sumbitsModXform64(int64_t *ptr, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	int64_t temp[max];

	for (x = 0; x != max; x++)
		temp[x] = ptr[x];

	sumbitsAndXform64(ptr, log2_max);
	sumbitsAndXform64(temp, log2_max);

	if (addMod != 0) {
		for (x = 0; x != max; x++)
			ptr[x] = (ptr[x] * temp[x]) % addMod;
	} else {
		for (x = 0; x != max; x++)
			ptr[x] = ptr[x] * temp[x];
	}

	sumbitsAndXform64(ptr, log2_max);

	for (x = 0; x != max; x++)
		ptr[x] >>= log2_max;
}

uint32_t
QMBoxMainWindow :: xforward(uint32_t x)
{
	if (polMode & 1) {
		x = ((x ^ invertMask) - addMask) & (dataLenRealLin - 1);
	} else {
		x = ((x + addMask) ^ invertMask) & (dataLenRealLin - 1);
	}
	return (x);
}

uint32_t
QMBoxMainWindow :: xinverse(uint32_t x)
{
	if (polMode & 1) {
		x = ((x + addMask) ^ invertMask) & (dataLenRealLin - 1);
	} else {
		x = ((x ^ invertMask) - addMask) & (dataLenRealLin - 1);
	}
	return (x);
}

void
QMBoxMainWindow :: showInput(void)
{
	QString temp;
	uint32_t count = 0;
	uint32_t n;
	int64_t div;
	char linebuffer[128];
	uint32_t x;
	uint32_t y;
	uint8_t skip = 0;

	if (dataLenRealLin == 0)
		return;

	for (x = 0; x != dataLenRealLin; x++) {
		int64_t value;

		value = dataPtr[x];

		switch (viewMode) {
		case TYPE_CRC_AND:
		case TYPE_CRC_OR:
		case TYPE_CRC_SUMBITS:
			if (value == 0)
				value = 1;
			break;
		default:
			break;
		}
		exprPtr[xforward(x)] = value;
	}

	crcFailure = 0;

	switch (viewMode) {
	case TYPE_NORMAL:
		temp += "f(x) = f(x);\n\n";
		break;
	case TYPE_XOR_OR:
		temp += "f(x,y) = ((x | y) == y) ? 1 : 0;\n\n";
		break;
	case TYPE_XOR_AND:
		temp += "f(x,y) = ((x & y) == y) ? 1 : 0;\n\n";
		break;
	case TYPE_CRC_AND:
		temp += "f(x,y) = ((x & y) == y) ? 1 : 0;\n\n";
		skip = 1;
		break;
	case TYPE_CRC_OR:
		temp += "f(x,y) = ((x | y) == y) ? 1 : 0;\n\n";
		skip = 1;
		break;
	case TYPE_ADD_AND:
		temp += "f(x,y) = ((x & y) == y) ? 1 : 0;\n\n";
		break;
	case TYPE_ADD_OR:
		temp += "f(x,y) = ((x | y) == y) ? 1 : 0;\n\n";
		break;
	case TYPE_CRC_SUMBITS:
		temp += "f(x,y) = (sumbits(x & y) & 1) ? 1 : 0;\n\n";
		skip = 1;
		break;
	case TYPE_AND_SUMBITS:
		temp += "f(x,y) = (sumbits(x & y) & 1) ? 1 : 0;\n\n";
		break;
	case TYPE_MOD_SUMBITS:
		break;
	}

	for (y = 0; y != xformPwr; y++) {
		switch (viewMode) {
		case TYPE_XOR_OR:
			orXform64(exprPtr, dataLenRealLog);
			break;
		case TYPE_XOR_AND:
			xorXform64(exprPtr, dataLenRealLog);
			break;
		case TYPE_CRC_AND:
			inverseCrcXform64(exprPtr, dataLenRealLog);
			break;
		case TYPE_CRC_OR:
			inverseRevCrcXform64(exprPtr, dataLenRealLog);
			break;
		case TYPE_ADD_AND:
			inverseAddXform64(exprPtr, dataLenRealLog);
			break;
		case TYPE_ADD_OR:
			inverseRevAddXform64(exprPtr, dataLenRealLog);
			break;
		case TYPE_CRC_SUMBITS:
			sumbitsCrcXform64(exprPtr, dataLenRealLog);

			/* Optimise away +/- into + only */

			for (x = 1; x != dataLenRealLin; x++) {
				exprPtr[0] = crcAdd(exprPtr[0], exprPtr[x]);
				exprPtr[x] = crcNeg(exprPtr[x]);
				exprPtr[x] = crcAdd(exprPtr[x], exprPtr[x]);
			}
			break;
		case TYPE_AND_SUMBITS:
			sumbitsAndXform64(exprPtr, dataLenRealLog);

			/* Optimise away +/- into + only */

			for (x = 1; x != dataLenRealLin; x++) {
				exprPtr[0] = add(exprPtr[0], exprPtr[x]);
				exprPtr[x] = addNeg(add(exprPtr[x], exprPtr[x]));
			}
			break;
		case TYPE_MOD_SUMBITS:
			sumbitsModXform64(exprPtr, dataLenRealLog);
			break;
		}
	}

	for (x = ((sortMode == SORT_INC) ? 0 : dataLenRealLin - 1);
	     x != ((sortMode == SORT_INC) ? dataLenRealLin : (uint32_t)-1);
	     x = ((sortMode == SORT_INC) ? x + 1 : x - 1)) {

		if (exprPtr[x] == 0)
			continue;
		if (skip == 1) {
			/* check for zero */
			if (crcNonZero(exprPtr[x]) == 0)
				continue;
		}

		snprintf(linebuffer, sizeof(linebuffer), 
		    "0x%016llx ", (long long)exprPtr[x]);

		temp += linebuffer;

		for (n = 0; n != 64; n++) {
			if (exprPtr[x] & (1ULL << (63 - n)))
				temp += QChar('1');
			else
				temp += QChar('.');
		}

		temp += " * ";

		if (x == 0) {
			temp += QChar('1');
		} else {
			for (n = 0; n != ('z' - 'a'); n++) {
				if (x & (1 << n))
					temp += QChar('a' + n);
			}
			for (; n != 32; n++) {
				if (x & (1 << n))
					temp += QChar('A' - 'z' + 'a' + n);
			}
		}

		div = exprPtr[x];
		if (div && x) {
			div = divOdd64(div / (~(div - 1) & div),
			    x / (~(x - 1) & x));
		} else {
			div = 0;
		}

		snprintf(linebuffer, sizeof(linebuffer), " (0x%08x)", x);

		temp += linebuffer;

		switch (viewMode) {
		case TYPE_XOR_OR:
		case TYPE_XOR_AND:
		case TYPE_ADD_AND:
		case TYPE_ADD_OR:
			for (n = 0; n != ('z' - 'a'); n++) {
				if (x & (1 << n)) {
					snprintf(linebuffer, sizeof(linebuffer),
					    " %c=0x%08llx", n + 'a',
					    (long long)(div << n));
					temp += linebuffer;
				}
			}
			for ( ; n != 32; n++) {
				if (x & (1 << n)) {
					snprintf(linebuffer, sizeof(linebuffer),
					    " %c=0x%08llx", 'A' - 'z' + 'a' + n,
					    (long long)(div << n));
					temp += linebuffer;
				}
			}
			break;
		case TYPE_CRC_AND:
		case TYPE_CRC_OR:
		case TYPE_CRC_SUMBITS:
			snprintf(linebuffer, sizeof(linebuffer),
			    " neg=0x%016llx", (long long)crcNeg(exprPtr[x]));
			temp += linebuffer;
			break;
		default:
			break;
		}
		temp += "\n";

		count++;
	}

	snprintf(linebuffer, sizeof(linebuffer), "\nCount = %d / %d / %d\n",
	    count, dataLenDisplay, dataLenRealLin);

	temp += linebuffer;

	switch (viewMode) {
	case TYPE_CRC_AND:
	case TYPE_CRC_OR:
	case TYPE_CRC_SUMBITS:
		if (crcFailure != 0) {
			snprintf(linebuffer, sizeof(linebuffer),
			    "\nErrors = %lld\n", (long long)crcFailure);
			temp += linebuffer;
		}
		break;
	default:
		break;
	}

	edtOutput->setPlainText(temp);
}

void
QMBoxMainWindow :: expandXorOr(int64_t *ptr, uint32_t expr, int64_t value, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;

	for (x = 0; x != max; x++) {
		if ((x | expr) == expr)
			ptr[x] ^= value;
	}
}

void
QMBoxMainWindow :: expandXorAnd(int64_t *ptr, uint32_t expr, int64_t value, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;

	for (x = 0; x != max; x++) {
		if ((x & expr) == expr)
			ptr[x] ^= value;
	}
}

void
QMBoxMainWindow :: expandCrcAnd(int64_t *ptr, uint32_t expr, int64_t value,
    uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;

	for (x = 0; x != max; x++) {
		if ((x & expr) == expr)
			ptr[x] = crcAdd(ptr[x], value);
	}
}

void
QMBoxMainWindow :: expandRevCrcAnd(int64_t *ptr, uint32_t expr, int64_t value,
    uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;

	for (x = 0; x != max; x++) {
		if ((x | expr) == expr)
			ptr[x] = crcAdd(ptr[x], value);
	}
}

void
QMBoxMainWindow :: expandAddAnd(int64_t *ptr, uint32_t expr, int64_t value, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;

	for (x = 0; x != max; x++) {
		if ((x & expr) == expr)
			ptr[x] = add(ptr[x], value);
	}
}

void
QMBoxMainWindow :: expandRevAddAnd(int64_t *ptr, uint32_t expr, int64_t value, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;

	for (x = 0; x != max; x++) {
		if ((x | expr) == expr)
			ptr[x] = add(ptr[x], value);
	}
}

void
QMBoxMainWindow :: expandCrcSumbits(int64_t *ptr, uint32_t expr, int64_t value, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;

	for (x = 0; x != max; x++) {
		if (sumbits32(x & expr) & 1)
			ptr[x] = crcAdd(ptr[x], crcNeg(value));
		else
			ptr[x] = crcAdd(ptr[x], value);
	}
}

void
QMBoxMainWindow :: expandAndSumbits(int64_t *ptr, uint32_t expr, int64_t value, uint8_t log2_max)
{
	const uint32_t max = 1U << log2_max;
	uint32_t x;
	int64_t value_neg = addNeg(value);

	for (x = 0; x != max; x++) {
		if (sumbits32(x & expr) & 1)
			ptr[x] = add(ptr[x], value_neg);
		else
			ptr[x] = add(ptr[x], value);
	}
}

uint8_t
QMBoxMainWindow :: sumbits64(uint64_t x)
{
	return (sumbits32(x >> 32) + sumbits32(x));
}

uint8_t
QMBoxMainWindow :: sumbits32(uint32_t x)
{
	if (x == 0)
		return (0);
	x = ((x & (1U * 0xAAAAAAAAU)) / 2U) + (x & (1U * 0x55555555U));
	x = ((x & (3U * 0x44444444U)) / 4U) + (x & (3U * 0x11111111U));
	x = ((x & (15U * 0x10101010U)) / 16U) + (x & (15U * 0x01010101U));
	x = ((x & (255U * 0x01000100U)) / 256U) + (x & (255U * 0x00010001U));
	x = ((x & (65535U * 0x00010000U)) / 65536U) + (x & (65535U * 0x00000001U));
	return (x);
}

void
QMBoxMainWindow :: handleAddExpression(void)
{
	modifyInputList(getExpr(), getValue(), exprMode, 0);
	showInput();
}

void
QMBoxMainWindow :: handleSubExpression(void)
{
	modifyInputList(getExpr(), getValue(), exprMode, 1);
	showInput();
}

void
QMBoxMainWindow :: handleImport(void)
{
	parseInputList(edtInput->toPlainText());
	showInput();
}

void
QMBoxMainWindow :: handleExprMode(void)
{
	exprMode++;
	exprMode %= TYPE_MAX;
	switch (exprMode) {
	case TYPE_NORMAL:
		butExprMode->setText(tr("Expr Normal"));
		break;
	case TYPE_XOR_OR:
		butExprMode->setText(tr("Expr Xor Or"));
		break;
	case TYPE_XOR_AND:
		butExprMode->setText(tr("Expr Xor And"));
		break;
	case TYPE_CRC_AND:
		butExprMode->setText(tr("Expr CRC And"));
		break;
	case TYPE_CRC_OR:
		butExprMode->setText(tr("Expr CRC Or"));
		break;
	case TYPE_ADD_AND:
		butExprMode->setText(tr("Expr Add And"));
		break;
	case TYPE_ADD_OR:
		butExprMode->setText(tr("Expr Add Or"));
		break;
	case TYPE_CRC_SUMBITS:
		butExprMode->setText(tr("Expr Sum CRC"));
		break;
	case TYPE_AND_SUMBITS:
		butExprMode->setText(tr("Expr Sum And"));
		break;
	case TYPE_MOD_SUMBITS:
		butExprMode->setText(tr("Expr Sum Mod"));
		break;
	}
}

void
QMBoxMainWindow :: handlePolMode(void)
{
	polMode ^= 1;
	if (polMode & 1)
		butPolMode->setText(tr("Polarise"));
	else
		butPolMode->setText(tr("DePolarise"));
	showInput();
}

void
QMBoxMainWindow :: handleViewMode(void)
{
	viewMode++;
	viewMode %= TYPE_MAX;
	switch (viewMode) {
	case TYPE_NORMAL:
		butViewMode->setText(tr("View Normal"));
		break;
	case TYPE_XOR_OR:
		butViewMode->setText(tr("View Xor Or"));
		break;
	case TYPE_XOR_AND:
		butViewMode->setText(tr("View Xor And"));
		break;
	case TYPE_CRC_AND:
		butViewMode->setText(tr("View CRC And"));
		break;
	case TYPE_CRC_OR:
		butViewMode->setText(tr("View CRC Or"));
		break;
	case TYPE_ADD_AND:
		butViewMode->setText(tr("View Add And"));
		break;
	case TYPE_ADD_OR:
		butViewMode->setText(tr("View Add Or"));
		break;
	case TYPE_CRC_SUMBITS:
		butViewMode->setText(tr("View Sum CRC"));
		break;
	case TYPE_AND_SUMBITS:
		butViewMode->setText(tr("View Sum And"));
		break;
	case TYPE_MOD_SUMBITS:
		butViewMode->setText(tr("View Sum Mod"));
		break;
	}
	showInput();
}

void
QMBoxMainWindow :: handleClearInput(void)
{
	edtInput->setPlainText(QString());
}

void
QMBoxMainWindow :: handleSortDec(void)
{
	sortMode = SORT_DEC;
	showInput();
}

void
QMBoxMainWindow :: handleSortInc(void)
{
	sortMode = SORT_INC;
	showInput();
}

void
QMBoxMainWindow :: handleExprChanged(const QString &dummy)
{
	char linebuffer[128];
	uint32_t expr;

	expr = getExpr();

	if (expr < dataLenRealLin) {
		snprintf(linebuffer, sizeof(linebuffer), "Expression 0x%08x = 0x%08llx (%lld)",
		    expr, (long long)exprPtr[expr], (long long)exprPtr[expr]);
	} else {
		snprintf(linebuffer, sizeof(linebuffer), "Expression is out of range");
	}
		 

	lblStatus->setText(tr(linebuffer));
}

void
QMBoxMainWindow :: handleCbxChanged(int state)
{
	uint32_t x;
	uint32_t temp;

	for (temp = x = 0; x != QMBOX_MAX_INVERT; x++) {
		if (cbxInvert[x]->isChecked())
			temp |= (1U << x);
	}

	invertMask = temp;

	for (temp = x = 0; x != QMBOX_MAX_INVERT; x++) {
		if (cbxAdd[x]->isChecked())
			temp |= (1U << x);
	}
	addMask = temp;

	showInput();
}

void
QMBoxMainWindow :: handleCleanup(void)
{
	uint32_t x;
	uint32_t y;
	uint32_t z;
	uint32_t max = 1U << dataLenRealLog;

	if (dataLenRealLin == 0)
		return;

	switch (viewMode) {
	case TYPE_NORMAL:
		for (x = 0; x != max; x++) {
			if (usedPtr[x] == 0)
				dataPtr[x] = 0;
		}
		break;
	case TYPE_XOR_OR:
		for (x = max - 1; x != (uint32_t)-1U; x--) {
			z = xinverse(x);
			if (exprPtr[x] != 0 && usedPtr[z] == 0) {
				dataPtr[z] ^= exprPtr[x];
				for (y = 0; y != dataLenRealLin; y++)
					exprPtr[xforward(y)] = dataPtr[y];
				orXform64(exprPtr, dataLenRealLog);
			}
		}
		break;
	case TYPE_XOR_AND:
		for (x = 0; x != max; x++) {
			z = xinverse(x);
			if (exprPtr[x] != 0 && usedPtr[z] == 0) {
				dataPtr[z] ^= exprPtr[x];
				for (y = 0; y != dataLenRealLin; y++)
					exprPtr[xforward(y)] = dataPtr[y];

				xorXform64(exprPtr, dataLenRealLog);
			}
		}
		break;
	case TYPE_CRC_AND:
		for (x = 0; x != max; x++) {
			z = xinverse(x);
			if (crcNonZero(exprPtr[x]) && usedPtr[z] == 0) {
				dataPtr[z] = crcAdd(dataPtr[z],
				    crcNeg(exprPtr[x]));
				for (y = 0; y != dataLenRealLin; y++)
					exprPtr[xforward(y)] = dataPtr[y];

				inverseCrcXform64(exprPtr, dataLenRealLog);
			}
		}
		break;
	case TYPE_CRC_OR:
		for (x = max - 1; x != (uint32_t)-1U; x--) {
			z = xinverse(x);
			if (crcNonZero(exprPtr[x]) && usedPtr[z] == 0) {
				dataPtr[z] = crcAdd(dataPtr[z],
				    crcNeg(exprPtr[x]));
				for (y = 0; y != dataLenRealLin; y++)
					exprPtr[xforward(y)] = dataPtr[y];

				inverseRevCrcXform64(exprPtr, dataLenRealLog);
			}
		}
		break;
	case TYPE_ADD_AND:
		for (x = 0; x != max; x++) {
			z = xinverse(x);
			if (exprPtr[x] != 0 && usedPtr[z] == 0) {
				dataPtr[z] -= exprPtr[x];
				for (y = 0; y != dataLenRealLin; y++)
					exprPtr[xforward(y)] = dataPtr[y];

				inverseAddXform64(exprPtr, dataLenRealLog);
			}
		}
		break;
	case TYPE_ADD_OR:
		for (x = max - 1; x != (uint32_t)-1U; x--) {
			z = xinverse(x);
			if (exprPtr[x] != 0 && usedPtr[z] == 0) {
				dataPtr[z] -= exprPtr[x];
				for (y = 0; y != dataLenRealLin; y++)
					exprPtr[xforward(y)] = dataPtr[y];

				inverseRevAddXform64(exprPtr, dataLenRealLog);
			}
		}
		break;
	default:
		break;
	}
	showInput();
}

uint64_t
QMBoxMainWindow :: divOdd64(uint64_t r, uint64_t div)
{
	uint64_t m;

	div = -div + 1ULL;

	m = 1;
	while (m) {

		if (r & m) {
			r += div;
		}
		m *= 2ULL;
		div *= 2ULL;
	}
	return (r);
}

void
QMBoxMainWindow :: handleAddConfig(void)
{
	QMBoxAddDialog *dlg = new QMBoxAddDialog();

	dlg->ledCarry->setText(QString::number(addCarry,10));
	dlg->ledMod->setText(QString::number(addMod,10));
	dlg->ledBase->setText(QString::number(addBase,10));

	if (dlg->exec() == QDialog::Accepted) {
		addMod = dlg->ledMod->text().toLong();
		addCarry = dlg->ledCarry->text().toLong();
		addBase = dlg->ledBase->text().toLong();
		if (addCarry == 0)
			addCarry = 1;
		if (addBase < 1)
			addBase = 2;
		switch (viewMode) {
		case TYPE_ADD_AND:
		case TYPE_ADD_OR:
		case TYPE_AND_SUMBITS:
		case TYPE_MOD_SUMBITS:
			showInput();
			break;
		default:
			break;
		}
	}
	delete dlg;
}

void
QMBoxMainWindow :: handleCrcConfig(void)
{
	QMBoxCrcDialog *dlg = new QMBoxCrcDialog();
	uint64_t temp;
	uint64_t k = 1;

	dlg->ledCrcBase->setText(QString::number(crcBase,10));
	dlg->ledCrcPoly->setText(QString::number(crcPoly,10));
	dlg->ledCrcMod->setText(QString::number(crcModLen,10));
	dlg->ledCrcOddxor->setText(QString::number(crcOddxor,10));

	if (dlg->exec() == QDialog::Accepted) {
		crcBase = dlg->ledCrcBase->text().toLong();
		crcPoly = dlg->ledCrcPoly->text().toLong();
		crcModLen = dlg->ledCrcMod->text().toLong();
		crcOddxor = dlg->ledCrcOddxor->text().toLong();

		if (crcBase < 2)
			crcBase = 2;

		if (crcPoly <= crcBase) {
			crcPoly = 0;
			crcModLen = 0;
		}

		for (temp = crcPoly / crcBase; temp != 0; temp /= crcBase)
			k *= crcBase;

		crcPolyMsb = k;

		crcLimit = 1;
		while (crcLimit < ((1ULL << 63) / (crcBase * crcBase)))
			crcLimit *= crcBase;

		switch (viewMode) {
		case TYPE_CRC_AND:
		case TYPE_CRC_OR:
		case TYPE_CRC_SUMBITS:
			showInput();
			break;
		default:
			break;
		}
	}
	delete dlg;
}

void
QMBoxMainWindow :: handlePwrConfig(void)
{
	QMBoxPwrDialog *dlg = new QMBoxPwrDialog();

	dlg->ledPwr->setText(QString::number(xformPwr,10));

	if (dlg->exec() == QDialog::Accepted) {
		xformPwr = dlg->ledPwr->text().toLong();
		if (xformPwr <= 0)
			xformPwr = 1;
		else if (xformPwr > 32)
			xformPwr = 32;

		showInput();
	}
	delete dlg;
}

void
QMBoxMainWindow :: handleDiff(void)
{
	int64_t last;
	int64_t curr;
	uint32_t x;

	switch (viewMode) {
	case TYPE_XOR_AND:
	case TYPE_XOR_OR:
		last = 0;
		for (x = 0; x != dataLenRealLin; x++) {
			if (usedPtr[x] == 0)
				continue;
			curr = dataPtr[x];
			dataPtr[x] = crcXor(curr, last);
			last = curr;
		}
		break;
	case TYPE_CRC_AND:
	case TYPE_CRC_OR:
	case TYPE_CRC_SUMBITS:
		last = 1;
		for (x = 0; x != dataLenRealLin; x++) {
			if (usedPtr[x] == 0)
				continue;
			curr = dataPtr[x];
			dataPtr[x] = crcAdd(curr, crcNeg(last));
			last = curr;
		}
		break;
	case TYPE_NORMAL:
	case TYPE_ADD_AND:
	case TYPE_ADD_OR:
	case TYPE_AND_SUMBITS:
	case TYPE_MOD_SUMBITS:
		last = 0;
		for (x = 0; x != dataLenRealLin; x++) {
			if (usedPtr[x] == 0)
				continue;
			curr = dataPtr[x];
			dataPtr[x] = add(curr, addNeg(last));
			last = curr;
		}
		break;
	default:
		break;
	}
	showInput();
}

void
QMBoxMainWindow :: handleInteg(void)
{
	int64_t last;
	uint32_t x;

	switch (viewMode) {
	case TYPE_XOR_AND:
	case TYPE_XOR_OR:
		last = 0;
		for (x = 0; x != dataLenRealLin; x++) {
			if (usedPtr[x] == 0)
				continue;
			last = crcXor(dataPtr[x], last);
			dataPtr[x] = last;
		}
		break;
	case TYPE_CRC_AND:
	case TYPE_CRC_OR:
	case TYPE_CRC_SUMBITS:
		last = 1;
		for (x = 0; x != dataLenRealLin; x++) {
			if (usedPtr[x] == 0)
				continue;
			last = crcAdd(last, dataPtr[x]);
			dataPtr[x] = last;
		}
		break;
	case TYPE_NORMAL:
	case TYPE_ADD_AND:
	case TYPE_ADD_OR:
	case TYPE_AND_SUMBITS:
	case TYPE_MOD_SUMBITS:
		last = 0;
		for (x = 0; x != dataLenRealLin; x++) {
			if (usedPtr[x] == 0)
				continue;
			last = add(last, dataPtr[x]);
			dataPtr[x] = last;
		}
		break;
	default:
		break;
	}
	showInput();
}

int
main(int argc, char **argv)
{
	QApplication app(argc, argv);

	QMBoxMainWindow main;

	main.show();

	return (app.exec());
}
