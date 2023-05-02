/*-
 * Copyright (c) 2011-2023 Hans Petter Selasky <hps@selasky.org>
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

#ifndef _QMBOX_H_
#define	_QMBOX_H_

#include <err.h>
#include <stdint.h>
#include <stdlib.h>

#include <QApplication>
#include <QPushButton>
#include <QGridLayout>
#include <QPlainTextEdit>
#include <QLabel>
#include <QLineEdit>
#include <QSpacerItem>
#include <QFont>
#include <QCheckBox>
#include <QSpinBox>
#include <QDialog>

#define	QMBOX_MAX_INVERT	20

class QMBoxAddDialog : public QDialog
{
	Q_OBJECT;
public:
	QMBoxAddDialog();
	~QMBoxAddDialog();

	QGridLayout *gridMain;

	QLabel *lblMod;
	QLabel *lblCarry;
	QLabel *lblBase;

	QLineEdit *ledMod;
	QLineEdit *ledCarry;
	QLineEdit *ledBase;

	QPushButton *btnOK;
	QPushButton *btnCancel;
};

class QMBoxCrcDialog : public QDialog
{
	Q_OBJECT;
public:
	QMBoxCrcDialog();
	~QMBoxCrcDialog();

	QGridLayout *gridMain;

	QLabel *lblCrcBase;
	QLabel *lblCrcPoly;
	QLabel *lblCrcMod;
	QLabel *lblCrcOddxor;

	QLineEdit *ledCrcBase;
	QLineEdit *ledCrcPoly;
	QLineEdit *ledCrcMod;
	QLineEdit *ledCrcOddxor;

	QPushButton *btnOK;
	QPushButton *btnCancel;
};

class QMBoxPwrDialog : public QDialog
{
	Q_OBJECT;
public:
	QMBoxPwrDialog();
	~QMBoxPwrDialog();

	QGridLayout *gridMain;

	QLabel *lblPwr;

	QLineEdit *ledPwr;

	QPushButton *btnOK;
	QPushButton *btnCancel;
};

class QMBoxMainWindow : public QWidget
{
	Q_OBJECT;
public:
	QMBoxMainWindow(QWidget *parent = 0);
	~QMBoxMainWindow();

	QGridLayout *gridMain;

	QLabel *lblValue;
	QLabel *lblExpression;
	QLabel *lblStatus;
	QLabel *lblXorBits;
	QLabel *lblAddBits;

	QLineEdit *ledValue;
	QLineEdit *ledExpression;

	QPushButton *butDiff;
	QPushButton *butInteg;

	QPushButton *butSortDec;
	QPushButton *butSortInc;

	QPushButton *butViewMode;
	QPushButton *butExprMode;
	QPushButton *butPolMode;

	QPushButton *butClearInput;
	QPushButton *butCleanup;

	QPushButton *butAddExpression;
	QPushButton *butSubExpression;
	QPushButton *butImport;

	QPushButton *butCrcConfig;
	QPushButton *butPwrConfig;
	QPushButton *butAddConfig;

	QPlainTextEdit *edtInput;
	QPlainTextEdit *edtOutput;

	QCheckBox *cbxInvert[QMBOX_MAX_INVERT];
	QLabel *lblInvert[QMBOX_MAX_INVERT];
	QCheckBox *cbxAdd[QMBOX_MAX_INVERT];

	enum {
		TYPE_NORMAL,
		TYPE_XOR_AND,
		TYPE_XOR_OR,
		TYPE_CRC_AND,
		TYPE_CRC_OR,
		TYPE_ADD_AND,
		TYPE_ADD_OR,
		TYPE_CRC_SUMBITS,
		TYPE_AND_SUMBITS,
		TYPE_MOD_SUMBITS,
		TYPE_MAX,
	};
	uint32_t viewMode;
	uint32_t exprMode;

	enum {
		SORT_DEC,
		SORT_INC,
		SORT_MAX,
	};
	uint32_t sortMode;
	uint32_t invertMask;
	uint32_t addMask;
	uint32_t polMode;

	uint8_t *usedPtr;
	int64_t	*dataPtr;
	int64_t	*exprPtr;

	uint32_t dataLenRealLin;
	uint32_t dataLenRealLog;
	uint32_t dataLenDisplay;

	uint64_t crcBase;
	uint64_t crcPolyMsb;
	uint64_t crcPoly;
	uint64_t crcModLen;
	uint64_t crcOddxor;
	uint64_t crcFailure;
	uint64_t crcLimit;
	uint64_t xformPwr;
	int64_t addMod;
	int64_t addCarry;
	int64_t addBase;

	int64_t getValue();
	uint32_t getExpr();

	int64_t addNeg(int64_t);
	int64_t add(int64_t, int64_t);
	uint64_t crcNeg(uint64_t);
	uint8_t crcNonZero(uint64_t);
	uint64_t crcAdd(uint64_t, uint64_t);
	uint64_t crcXor(uint64_t, uint64_t);

	void clearInputList(void);
	void parseInputList(QString);
	void modifyInputList(uint32_t expr, int64_t value, uint8_t method, uint8_t is_neg);
	void inverseCrcXform64(int64_t *, uint8_t);
	void inverseRevCrcXform64(int64_t *, uint8_t);
	void forwardCrcXform64(int64_t *, uint8_t);
	void forwardRevCrcXform64(int64_t *, uint8_t);
	void inverseAddXform64(int64_t *, uint8_t);
	void inverseRevAddXform64(int64_t *, uint8_t);
	void forwardAddXform64(int64_t *, uint8_t);
	void forwardRevAddXform64(int64_t *, uint8_t);
	void orXform64(int64_t *, uint8_t);
	void xorXform64(int64_t *, uint8_t);
	void sumbitsCrcXform64(int64_t *, uint8_t);
	void sumbitsAndXform64(int64_t *, uint8_t);
	void sumbitsModXform64(int64_t *, uint8_t);
	void showInput(void);
	void expandXorOr(int64_t *, uint32_t, int64_t, uint8_t);
	void expandXorAnd(int64_t *, uint32_t, int64_t, uint8_t);
	void expandAddAnd(int64_t *, uint32_t, int64_t, uint8_t);
	void expandRevAddAnd(int64_t *, uint32_t, int64_t, uint8_t);
	void expandCrcAnd(int64_t *, uint32_t, int64_t, uint8_t);
	void expandRevCrcAnd(int64_t *, uint32_t, int64_t, uint8_t);
	void expandCrcSumbits(int64_t *, uint32_t, int64_t, uint8_t);
	void expandAndSumbits(int64_t *, uint32_t, int64_t, uint8_t);
	uint32_t xforward(uint32_t x);
	uint32_t xinverse(uint32_t x);
	uint8_t sumbits64(uint64_t);
	uint8_t sumbits32(uint32_t);

	uint64_t divOdd64(uint64_t rem, uint64_t div);

public slots:
	void handleDiff(void);
	void handleInteg(void);
	void handleAddExpression(void);
	void handleSubExpression(void);
	void handleImport(void);
	void handleExprMode(void);
	void handleViewMode(void);
	void handleClearInput(void);
	void handleSortDec(void);
	void handleSortInc(void);
	void handleExprChanged(const QString &text);
	void handleCbxChanged(int state);
	void handlePolMode(void);
	void handleCleanup(void);
	void handleCrcConfig(void);
	void handleAddConfig(void);
	void handlePwrConfig(void);
};

#endif		/* _QMBOX_H_ */
