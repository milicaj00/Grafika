
// klk2016View.h : interface of the Cklk2016View class
//

#pragma once
#include "DImage.h"


class Cklk2016View : public CView
{
protected: // create from serialization only
	Cklk2016View() noexcept;
	DECLARE_DYNCREATE(Cklk2016View)

// Attributes
public:
	Cklk2016Doc* GetDocument() const;

private: 
	DImage* table;
	DImage* wood;

	int rotation = 0;
	int moveX = 0;
	bool hit = false;
	double pravac = 0;

// Operations
public:
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply);
	void Rotate(CDC* pDC, float angle, bool rightMultiply);
	void DrawStick(CDC* pDC, int w);
	void DrawBall(CDC* pDC, int w);
	void DrawTable(CDC* pDC, CRect rect);
	void DrawBorder(CDC* pDC, CRect rect, int w);
	void DrawHoles(CDC* pDC, CRect rect, int size);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~Cklk2016View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // debug version in klk2016View.cpp
inline Cklk2016Doc* Cklk2016View::GetDocument() const
   { return reinterpret_cast<Cklk2016Doc*>(m_pDocument); }
#endif

