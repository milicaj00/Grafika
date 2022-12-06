
// klk2015View.h : interface of the Cklk2015View class
//

#pragma once
#include "DImage.h"

class Cklk2015View : public CView
{
protected: // create from serialization only
	Cklk2015View() noexcept;
	DECLARE_DYNCREATE(Cklk2015View)

// Attributes
public:
	Cklk2015Doc* GetDocument() const;

private:
	DImage* background;
	bool open = true;
	int moveX = 0;
	int moveY = 0;
	double rotatePackman = 0;
	bool isEnd = false;

// Operations
public:
	void Translate(CDC* pDC, double dX, double dY, bool right = false);
	void Rotate(CDC* pDC, double angle, bool right = false);
	void Scale(CDC* pDC, double sX, double sY, bool right = false);
	void DrawPacman(CDC* pDC, CRect rect, float angle);
	void DrawGhost(CDC* pDC, CRect rect);
	void writeEnd(CDC* pDC);
	void checkEnd();
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
	virtual ~Cklk2015View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // debug version in klk2015View.cpp
inline Cklk2015Doc* Cklk2015View::GetDocument() const
   { return reinterpret_cast<Cklk2015Doc*>(m_pDocument); }
#endif

