/*
** FamiTracker - NES/Famicom sound tracker
** Copyright (C) 2005-2012  Jonathan Liss
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This program is distributed in the hope that it will be useful, 
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU 
** Library General Public License for more details.  To obtain a 
** copy of the GNU Library General Public License, write to the Free 
** Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
**
** Any permitted reproduction of these routines, in whole or in part,
** must bear this legend.
*/

#pragma once

#include "resource.h"

#include "SequenceEditor.h"
#include "InstrumentEditPanel.h"

// CInstrumentEditDlg dialog

class CInstrumentEditDlg : public CDialog
{
   Q_OBJECT
	DECLARE_DYNAMIC(CInstrumentEditDlg)

   // Qt stuff
protected:
   void paintEvent(QPaintEvent *);
   void mouseMoveEvent(QMouseEvent *event);
   void mousePressEvent(QMouseEvent *event);
   void mouseReleaseEvent(QMouseEvent *event);
   void mouseDoubleClickEvent(QMouseEvent *event);
   void closeEvent(QCloseEvent *);
   
public slots:
   void instTab_currentChanged(int arg1);
      
public:
	CInstrumentEditDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInstrumentEditDlg();

	void ChangeNoteState(int Note);
	void SetCurrentInstrument(int Index);

	bool IsOpened() const;

	void EndDialog(int nResult);

	CFamiTrackerDoc *GetDocument() const { return m_pDocument; };

// Dialog Data
	enum { IDD = IDD_INSTRUMENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void InsertPane(CInstrumentEditPanel *pPanel, bool Show);
	void ClearPanels();

	void SwitchOnNote(int x, int y);
	void SwitchOffNote(bool ForceHalt);

protected:
	// Constants
	static const int PANEL_COUNT = 2;

	static const int KEYBOARD_TOP;
	static const int KEYBOARD_LEFT;
	static const int KEYBOARD_WIDTH;
	static const int KEYBOARD_HEIGHT;

	static const TCHAR *CHIP_NAMES[];

protected:
	// Variables for keyboard
	int	m_iLastKey;
	int m_iActiveKey;

	// Variables for windows
	CInstrumentEditPanel *m_pPanels[PANEL_COUNT];
	CInstrumentEditPanel *m_pFocusPanel;
	bool m_bOpened;
	int m_iSelectedInstType;
	int m_iPanels;

	int	m_iInstrument;

	CFamiTrackerDoc *m_pDocument;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnTcnSelchangeInstTab(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
	void ChangeNoteOn(int Note, int Octave);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	virtual BOOL DestroyWindow();
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnNcLButtonUp(UINT nHitTest, CPoint point);
protected:
	virtual void PostNcDestroy();
};
