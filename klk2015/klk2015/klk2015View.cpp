
// klk2015View.cpp : implementation of the Cklk2015View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "klk2015.h"
#endif

#include "klk2015Doc.h"
#include "klk2015View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592653589793238462643383279502884

// Cklk2015View

IMPLEMENT_DYNCREATE(Cklk2015View, CView)

BEGIN_MESSAGE_MAP(Cklk2015View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// Cklk2015View construction/destruction

Cklk2015View::Cklk2015View() noexcept
{
	// TODO: add construction code here

	this->background = new DImage();
	this->background->Load(CString("blue.png"));

}

Cklk2015View::~Cklk2015View()
{
	delete this->background;
}

BOOL Cklk2015View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cklk2015View drawing

void Cklk2015View::Translate(CDC* pDC, double dX, double dY, bool right)
{
	XFORM form = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&form, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);

}

void Cklk2015View::Rotate(CDC* pDC, double ugao, bool right)
{
	double angle = ugao * PI / 180;
	XFORM form;
	form.eM11 = cos(angle);
	form.eM12 = sin(angle);
	form.eM21 = -sin(angle);
	form.eM22 = -cos(angle);
	form.eDx = 0;
	form.eDy = 0;
	pDC->ModifyWorldTransform(&form, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Cklk2015View::Scale(CDC* pDC, double sX, double sY, bool right)
{
	XFORM form = { sX, 0, 0, sY, 0, 0 };
	pDC->ModifyWorldTransform(&form, right ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Cklk2015View::DrawPacman(CDC* pDC, CRect rect, float angle)
{
	double radius = rect.Width() / 2;
	double b = sin(angle) * radius;

	CBrush* brush = new CBrush(RGB(255, 255, 0));
	CBrush* oldBrush = pDC->SelectObject(brush);

	if (this->open) {
		int direction = pDC->GetArcDirection();
		pDC->SetArcDirection(AD_CLOCKWISE);
		pDC->Pie(-radius, -radius, radius, radius, rect.Width() / 2, -b, rect.Width() / 2, b);
		pDC->SetArcDirection(direction);
	}
	else {
		pDC->Pie(-radius, -radius, radius, radius, rect.Width() / 2, 0, rect.Width() / 2, 0);
	}

	Translate(pDC, -b / 3, 2 * b / 3, true);
	CBrush* eye = new CBrush(RGB(0, 0, 0));
	pDC->SelectObject(eye);
	pDC->Ellipse(-5, -5, 5, 5);

	pDC->SelectObject(oldBrush);
	brush->DeleteObject();
	delete brush;
	eye->DeleteObject();
	delete eye;
}

void Cklk2015View::DrawGhost(CDC* pDC, CRect rect)
{
	CPen* pen = new CPen(PS_COSMETIC, 1, RGB(255, 0, 0));
	CPen* oldPen = pDC->SelectObject(pen);
	CBrush* brush = new CBrush(RGB(255, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(brush);

	pDC->BeginPath();

	pDC->MoveTo(rect.right, 0);
	int odlDirection = pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(rect,  CPoint(rect.right, 0), CPoint(rect.left, 0));

	CRect* rect1 = new CRect(rect.left - rect.Width() / 12, rect.bottom, rect.left + rect.Width() / 12, rect.bottom + rect.Height() / 6);
	pDC->LineTo(rect.left, rect.bottom + rect1->Height());
	pDC->ArcTo(rect1, CPoint(rect1->left + rect.Width() / 12, rect1->bottom), CPoint(rect1->right, rect1->top + rect1->Height() / 2));

	rect1 = new CRect(rect.left + rect.Width() / 12, rect.bottom, rect.left + rect.Width() / 12 + rect.Width() / 6, rect.bottom + rect.Height() / 6);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(rect1, CPoint(rect1->left, rect1->bottom - rect1->Height() / 2), CPoint(rect1->right, rect1->bottom - rect1->Height() / 2));

	CRect* rect2 = new CRect(rect1->right, rect.bottom, rect1->right + rect.Width() / 6, rect.bottom + rect.Height() / 6);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(rect2, CPoint(rect2->left, rect2->bottom - rect2->Height() / 2), CPoint(rect2->right, rect2->bottom - rect2->Height() / 2));

	CRect* rect3 = new CRect(rect2->right, rect.bottom, rect2->right + rect.Width() / 6, rect.bottom + rect.Height() / 6);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(rect3, CPoint(rect3->left, rect3->bottom - rect3->Height() / 2), CPoint(rect3->right, rect3->bottom - rect3->Height() / 2));

	CRect* rect4 = new CRect(rect3->right, rect.bottom, rect3->right + rect.Width() / 6, rect.bottom + rect.Height() / 6);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(rect4, CPoint(rect4->left, rect4->bottom - rect4->Height() / 2), CPoint(rect4->right, rect4->bottom - rect4->Height() / 2));

	CRect* rect5 = new CRect(rect4->right, rect.bottom, rect4->right + rect.Width() / 6, rect.bottom + rect.Height() / 6);
	pDC->SetArcDirection(AD_CLOCKWISE);
	pDC->ArcTo(rect5, CPoint(rect5->left, rect5->bottom - rect5->Height() / 2), CPoint(rect5->right, rect5->bottom - rect5->Height() / 2));

	CRect* rect6 = new CRect(rect5->right, rect.bottom, rect5->right + rect.Width() / 6, rect.bottom + rect.Height() / 6);
	pDC->SetArcDirection(AD_COUNTERCLOCKWISE);
	pDC->ArcTo(rect6, CPoint(rect6->left, rect6->bottom - rect6->Height() / 2 ), CPoint(rect.right, rect6->bottom));
	pDC->LineTo(rect.right, 0);

	pDC->EndPath();
	pDC->FillPath();
	//pDC->StrokeAndFillPath();

	pDC->SelectObject(oldPen);
	XFORM old;
	pDC->GetWorldTransform(&old);

	CBrush* whiteEye = new CBrush(RGB(255, 255, 255));
	pDC->SelectObject(whiteEye);
	pDC->Ellipse(-12, -12, 12, 12);
	Translate(pDC, 3, 0);
	CBrush* blueEye = new CBrush(RGB(0, 0, 255));
	pDC->SelectObject(blueEye);
	pDC->Ellipse(-5, -5, 5, 5);

	Translate(pDC, 30, 0);

	pDC->SelectObject(whiteEye);
	pDC->Ellipse(-12, -12, 12, 12);
	Translate(pDC, 3, 0);
	pDC->SelectObject(blueEye);
	pDC->Ellipse(-5, -5, 5, 5);
	
	pDC->SetWorldTransform(&old);
	pDC->SelectObject(oldBrush);

	brush->DeleteObject();
	delete brush;
	pen->DeleteObject();
	delete pen;
}

void Cklk2015View::writeEnd(CDC* pDC)
{
	CFont myFont;
	myFont.CreateFontW(100, 0, 0, 0, 800, 0, 0, 0, 0, 0, 0, 0, 0, L"Arial");
	CFont* oldFont = pDC->SelectObject(&myFont);

	COLORREF oldTextColor = pDC->SetTextColor(RGB(255, 255, 255));
	int oldBkMode = pDC->SetBkMode(TRANSPARENT);

	pDC->TextOutW(500, 500, CString("THE END!"));

	pDC->SetTextColor(oldTextColor);
	pDC->SelectObject(oldFont);
	pDC->SetBkMode(oldBkMode);
	myFont.DeleteObject();
}

void Cklk2015View::checkEnd()
{
	if (moveY <= -325) {
		this->isEnd = false;
		return;
	}
	if (moveX <= -350) {
		this->isEnd = false;
		return;
	}
	if (moveX <= -155 && moveY <= -130) {
		this->isEnd = true;
		return;
	}
	
	this->isEnd = false;
}

void Cklk2015View::OnDraw(CDC* pDC)
{
	Cklk2015Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap* map = new CBitmap();
	map->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC->SelectObject(map);
	this->background->Draw(memDC, CRect(0, 0, background->Width(), background->Height()), rect);

	int prevMode = memDC->GetGraphicsMode();
	memDC->SetGraphicsMode(GM_ADVANCED);

	XFORM startForm;
	memDC->GetWorldTransform(&startForm);
	
	Translate(memDC, 400, 400);
	Translate(memDC, moveX, moveY);
	Rotate(memDC, rotatePackman);
	CRect packamanRect(-50, -50, 50, 50);
	DrawPacman(memDC, packamanRect, 10);

	memDC->SetWorldTransform(&startForm);
	Translate(memDC, 148, 168);
	//CRect(100, 120, 196, 216)
	CRect ghostRect(-48, -48, 48, 48);
	DrawGhost(memDC, ghostRect);

	memDC->SetWorldTransform(&startForm);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	if (this->isEnd) {
		writeEnd(pDC);
	}

	memDC->DeleteDC();
	delete memDC;
}


// Cklk2015View printing

BOOL Cklk2015View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cklk2015View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cklk2015View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cklk2015View diagnostics

#ifdef _DEBUG
void Cklk2015View::AssertValid() const
{
	CView::AssertValid();
}

void Cklk2015View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cklk2015Doc* Cklk2015View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cklk2015Doc)));
	return (Cklk2015Doc*)m_pDocument;
}
#endif //_DEBUG


// Cklk2015View message handlers


void Cklk2015View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_UP:
		moveY -= 5;
		this->open = !this->open;
		this->rotatePackman = -90;
		checkEnd();
		Invalidate();
		break;
	case VK_DOWN:
		moveY += 5;
		this->open = !this->open;
		this->rotatePackman = 90;
		checkEnd();
		Invalidate();
		break;
	case VK_LEFT:
		moveX -= 5;
		this->open = !this->open;
		this->rotatePackman = 180;
		checkEnd();
		Invalidate();
		break;
	case VK_RIGHT:
		moveX += 5;
		this->rotatePackman = 0;
		this->open = !this->open;
		checkEnd();
		Invalidate();
		break;
	default:
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL Cklk2015View::OnEraseBkgnd(CDC* pDC)
{
	return false;
}
