
// klk2016View.cpp : implementation of the Cklk2016View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "klk2016.h"
#endif

#include "klk2016Doc.h"
#include "klk2016View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592653589793238462643383279502884

// Cklk2016View

IMPLEMENT_DYNCREATE(Cklk2016View, CView)

BEGIN_MESSAGE_MAP(Cklk2016View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &Cklk2016View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// Cklk2016View construction/destruction

Cklk2016View::Cklk2016View() noexcept
{
	this->table = new DImage();
	this->table->Load(CString("felt2.jpg"));

	this->wood = new DImage();
	this->wood->Load(CString("wood.jpg"));
}

Cklk2016View::~Cklk2016View()
{
}

BOOL Cklk2016View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cklk2016View drawing

void Cklk2016View::OnDraw(CDC* pDC)
{
	Cklk2016Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap* btm = new CBitmap();
	btm->CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC->SelectObject(btm);

	memDC->SetGraphicsMode(GM_ADVANCED);

	DrawTable(memDC, rect);
	DrawBorder(memDC, rect, 40);
	DrawHoles(memDC, rect, 40);

	XFORM oldForm;
	memDC->GetWorldTransform(&oldForm);
	Translate(memDC, 600, 400, false);
	DrawBall(memDC, 30);

	Rotate(memDC, rotation, false);
	Translate(memDC, moveX, 0, false);
	Translate(memDC, -20, 0, false);
	Rotate(memDC, 180, false);

	DrawStick(memDC, 400);

	memDC->SetWorldTransform(&oldForm);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);

	btm->DeleteObject();
	delete btm;
	memDC->DeleteDC();
	delete memDC;
}


// Cklk2016View printing


void Cklk2016View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL Cklk2016View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cklk2016View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cklk2016View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void Cklk2016View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void Cklk2016View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// Cklk2016View diagnostics

#ifdef _DEBUG
void Cklk2016View::AssertValid() const
{
	CView::AssertValid();
}

void Cklk2016View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cklk2016Doc* Cklk2016View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cklk2016Doc)));
	return (Cklk2016Doc*)m_pDocument;
}
#endif //_DEBUG


// Cklk2016View message handlers

void Cklk2016View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Cklk2016View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	double ugao = angle * PI / 180;
	XFORM form;
	form.eDx = form.eDy = 0;
	form.eM11 = form.eM22 = cos(ugao);
	form.eM12 = sin(ugao);
	form.eM21 = -sin(ugao);
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Cklk2016View::DrawStick(CDC* pDC, int w)
{
	CPen* newPen = new CPen(PS_COSMETIC, 1, RGB(100, 50, 0));
	CPen* oldPen = pDC->SelectObject(newPen);
	CBrush* newBrush = new CBrush(RGB(205, 133, 63));
	CBrush* oldBrush = pDC->SelectObject(newBrush);

	CPoint* points = new CPoint[4];
	points[0] = CPoint(0, 0);
	points[1] = CPoint(0, w/100);
	points[2] = CPoint(2*w/3, w/65);
	points[3] = CPoint(2*w/3, 0);

	pDC->Polygon(points, 4);

	newBrush = new CBrush(RGB(100, 50, 0));
	pDC->SelectObject(newBrush);

	points = new CPoint[4];
	points[0] = CPoint(w, 0);
	points[1] = CPoint(w, w / 65);
	points[2] = CPoint(2 * w / 3, w / 65);
	points[3] = CPoint(2 * w / 3, 0);

	pDC->Polygon(points, 4);

	pDC->SelectObject(oldBrush);
	newBrush->DeleteObject();
	delete newBrush;
	pDC->SelectObject(oldPen);
	newPen->DeleteObject();
	delete newPen;
}

void Cklk2016View::DrawBall(CDC* pDC, int w)
{
	CPen* newPen = new CPen(PS_COSMETIC, 1, RGB(150, 0, 0));
	CPen* oldPen = pDC->SelectObject(newPen);
	CBrush* newBrush = new CBrush(RGB(200, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(newBrush);

	double r = w/2;
	pDC->Ellipse(-r, -r, r, r);

	pDC->SelectObject(oldBrush);
	newBrush->DeleteObject();
	delete newBrush;

	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 3, -3, true);
	double r1 = r / 3;
	pDC->Ellipse(-r1, -r1, r1, r1);

	pDC->SetWorldTransform(&oldForm);
	pDC->SelectObject(oldPen);
	newPen->DeleteObject();
	delete newPen;
}

void Cklk2016View::DrawTable(CDC* pDC, CRect rect)
{
	int sizeX = rect.Width() / table->Width() + 1;
	int sizeY = rect.Height() / table->Height() + 1;
	for (int i = 0; i < sizeX; i++) {
		for (int j = 0; j < sizeY; j++) {
			table->Draw(pDC,
				CRect(0, 0, table->Width(), table->Height()),
				CRect(i * table->Width(), j * table->Height(),
					(i + 1) * table->Width(), (j + 1) * table->Height()));
		}
	}
}

void Cklk2016View::DrawBorder(CDC* pDC, CRect rect, int w)
{
	CRgn r1, r2;
	r1.CreateRectRgn(rect.left + w, rect.top + w, rect.right - w, rect.bottom - w);
	r2.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
	r1.CombineRgn(&r1, &r2, RGN_XOR);
	pDC->SelectClipRgn(&r1);
	wood->Draw(pDC, CRect(0, 0, wood->Width(), wood->Height()), rect);
	pDC->SelectClipRgn(&r2);
}

void Cklk2016View::DrawHoles(CDC* pDC, CRect rect, int size)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	Translate(pDC, size, size, false);

	CBrush* newBrush = new CBrush(RGB(0, 0, 0));
	CBrush* oldBrush = pDC->SelectObject(newBrush);

	double r = size / 2;
	pDC->Ellipse(-r, -r, r, r);

	Translate(pDC, rect.Width() / 2 - size, 0, false);
	pDC->Ellipse(-r, -r, r, r);

	Translate(pDC, rect.Width() / 2 - size, 0, false);
	pDC->Ellipse(-r, -r, r, r);

	Translate(pDC, 0, rect.Height() - 2 * size, false);
	pDC->Ellipse(-r, -r, r, r);

	Translate(pDC, -rect.Width() / 2 + size, 0, false);
	pDC->Ellipse(-r, -r, r, r);

	Translate(pDC, -rect.Width() / 2 + size, 0, false);
	pDC->Ellipse(-r, -r, r, r);

	pDC->SetWorldTransform(&oldForm);

	pDC->SelectObject(oldBrush);
	newBrush->DeleteObject();
	delete newBrush;
}


BOOL Cklk2016View::OnEraseBkgnd(CDC* pDC)
{
	return false;
}


void Cklk2016View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case VK_LEFT:
		rotation += 10;
		Invalidate();
		break;
	case VK_RIGHT:
		rotation-= 10;
		Invalidate();
		break;
	case VK_UP:
		moveX += 10;
		Invalidate();
		break;
	case VK_DOWN:
		moveX -= 10;
		Invalidate();
		break;
	default:
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
