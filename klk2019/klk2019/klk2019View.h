
// klk2019View.h : interface of the Cklk2019View class
//

#pragma once
#include "DImage.h"


class Cklk2019View : public CView
{
protected: // create from serialization only
	Cklk2019View() noexcept;
	DECLARE_DYNCREATE(Cklk2019View)

// Attributes
public:
	Cklk2019Doc* GetDocument() const;

private:
	DImage* parts;
	double rotateLeg1 = 0;
	double rotateLeg2 = 0;
	double rotateArm1 = 0;
	double rotateArm2 = 180;
	double rotateBody = 0;
	int moveX = 0;
	int moveY = 0;

// Operations
public:
	void DrawBackground(CDC* pDC, CRect rc);
	void DrawImgTransparent(CDC* pDC, DImage* pImage);
	void Translate(CDC* pDC, float dX, float dY, bool rightMultiply = false);
	void Rotate(CDC* pDC, float angle, bool rightMultiply = false);
	void DrawArm1(CDC* pDC);
	void DrawArm2(CDC* pDC);
	void DrawLeg1(CDC* pDC);
	void DrawLeg2(CDC* pDC);
	void DrawBody1(CDC* pDC);
	void DrawTransformer(CDC* pDC);

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
	virtual ~Cklk2019View();
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

#ifndef _DEBUG  // debug version in klk2019View.cpp
inline Cklk2019Doc* Cklk2019View::GetDocument() const
   { return reinterpret_cast<Cklk2019Doc*>(m_pDocument); }
#endif

