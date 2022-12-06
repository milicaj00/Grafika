
// klk2019View.cpp : implementation of the Cklk2019View class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "klk2019.h"
#endif

#include "klk2019Doc.h"
#include "klk2019View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define PI 3.141592653589793238462643383279502884
// Cklk2019View

IMPLEMENT_DYNCREATE(Cklk2019View, CView)

BEGIN_MESSAGE_MAP(Cklk2019View, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_KEYDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// Cklk2019View construction/destruction

Cklk2019View::Cklk2019View() noexcept
{
	// TODO: add construction code here
	this->parts = new DImage[6];
	parts[0].Load(CString("background.jpg"));
	parts[1].Load(CString("body1.png"));
	parts[2].Load(CString("arm1.png"));
	parts[3].Load(CString("arm2.png"));
	parts[4].Load(CString("leg1.png"));
	parts[5].Load(CString("leg2.png"));
}

Cklk2019View::~Cklk2019View()
{
	delete[] parts;
}

BOOL Cklk2019View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// Cklk2019View drawing

void Cklk2019View::DrawBackground(CDC* pDC, CRect rc)
{
	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);
	int prevMode = memDC->GetGraphicsMode();
	memDC->SetGraphicsMode(GM_ADVANCED);
	CBitmap* bitmap = new CBitmap();
	bitmap->CreateCompatibleBitmap(pDC, rc.Width(), rc.Height());
	memDC->SelectObject(bitmap);
	this->parts[0].Draw(memDC, CRect(0, 0, parts[0].Width(), parts[0].Height()), rc);
	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
	delete memDC;
}

void Cklk2019View::DrawImgTransparent(CDC* pDC, DImage* pImage)
{
	CBitmap mask; //monohrom maska 1 b po px
	mask.CreateBitmap(pImage->Width(), pImage->Height(), 1, 1, NULL);

	CBitmap image; //copy image u bitmap
	image.CreateBitmap(pImage->Width(), pImage->Height(), 4, 8, NULL);
	image.SetBitmapBits(pImage->Width() * pImage->Height() * pImage->BPP(), pImage->GetDIBBits());

	CDC* srcDC = new CDC();
	CDC* dstDC = new CDC();
	srcDC->CreateCompatibleDC(pDC);
	dstDC->CreateCompatibleDC(pDC);

	CBitmap* srcOldBitmap = srcDC->SelectObject(&image);
	CBitmap* dstOldBitmap = dstDC->SelectObject(&mask);

	COLORREF removeColor = srcDC->GetPixel(0, 0);
	COLORREF srcOldBKColor = srcDC->SetBkColor(removeColor);
	dstDC->BitBlt(0, 0, pImage->Width(), pImage->Height(), srcDC, 0, 0, SRCCOPY);//u dst crno gde je slika -> mask

	COLORREF srcOldTextColor = srcDC->SetTextColor(RGB(255, 255, 255));
	srcDC->SetBkColor(RGB(0, 0, 0));
	srcDC->BitBlt(0, 0, pImage->Width(), pImage->Height(), dstDC, 0, 0, SRCAND); //u src pozadina crna slika u boji -> image

	dstDC->SelectObject(dstOldBitmap);
	delete dstDC;

	srcDC->SelectObject(&mask);
	pDC->BitBlt(0, 0, pImage->Width(), pImage->Height(), srcDC, 0, 0, SRCAND); //lepi crnu sliku na pozadinu

	srcDC->SelectObject(&image); //uzima sliku sa crnu pozadinu
	pDC->BitBlt(0, 0, pImage->Width(), pImage->Height(), srcDC, 0, 0, SRCPAINT);

	srcDC->SelectObject(srcOldBitmap);
	delete srcDC;
}
void Cklk2019View::Translate(CDC* pDC, float dX, float dY, bool rightMultiply)
{
	XFORM form = { 1,0,0,1,dX,dY };
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Cklk2019View::Rotate(CDC* pDC, float angle, bool rightMultiply)
{
	float ugao = angle * PI / 180;
	XFORM form;
	form.eM11 = form.eM22 = cos(ugao);
	form.eM12 = sin(ugao);
	form.eM21 = -sin(ugao);
	form.eDx = form.eDy = 0;
	pDC->ModifyWorldTransform(&form, rightMultiply ? MWT_RIGHTMULTIPLY : MWT_LEFTMULTIPLY);
}

void Cklk2019View::DrawArm1(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	Translate(pDC, 212, 85, false);
	Rotate(pDC, rotateArm1, false);
	Translate(pDC, -34, -31, false);
	DrawArm2(pDC);
	DrawImgTransparent(pDC, &this->parts[2]);
	pDC->SetWorldTransform(&oldForm);
}

void Cklk2019View::DrawArm2(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	Translate(pDC, 208, 102, false);
	Rotate(pDC, rotateArm2, false);
	Translate(pDC, -23, -61, false);
	DrawImgTransparent(pDC, &this->parts[3]);
	pDC->SetWorldTransform(&oldForm);
}

void Cklk2019View::DrawLeg1(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 122+30+200, 185+125+305, false);
	Rotate(pDC, rotateLeg1, false);
	Translate(pDC, -30, -125, false);
	DrawImgTransparent(pDC, &this->parts[4]);
	DrawBody1(pDC);
	pDC->SetWorldTransform(&oldForm);
}

void Cklk2019View::DrawLeg2(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);

	Translate(pDC, 155+200, 310+305, false);
	Rotate(pDC, rotateLeg2, false);
	Translate(pDC, -35, -60, false);
	DrawImgTransparent(pDC, &this->parts[5]);

	pDC->SetWorldTransform(&oldForm);
}

void Cklk2019View::DrawBody1(CDC* pDC)
{
	XFORM oldForm;
	pDC->GetWorldTransform(&oldForm);
	Translate(pDC, 25+215, 133-5, false);
	Rotate(pDC, rotateBody, false);
	Translate(pDC, -26, -133, false);
	DrawImgTransparent(pDC, &this->parts[1]);
	DrawArm1(pDC);
	pDC->SetWorldTransform(&oldForm);
}

void Cklk2019View::DrawTransformer(CDC* pDC)
{
	DrawLeg2(pDC);

	DrawLeg1(pDC);

}

void Cklk2019View::OnDraw(CDC* pDC)
{
	Cklk2019Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);

	CDC* memDC = new CDC();
	memDC->CreateCompatibleDC(pDC);

	CBitmap bmp;
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	memDC->SelectObject(bmp);

	memDC->SetGraphicsMode(GM_ADVANCED);

	DrawBackground(memDC, rect);
	DrawTransformer(memDC);

	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), memDC, 0, 0, SRCCOPY);
	memDC->DeleteDC();
	delete memDC;
}


// Cklk2019View printing

BOOL Cklk2019View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void Cklk2019View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void Cklk2019View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// Cklk2019View diagnostics

#ifdef _DEBUG
void Cklk2019View::AssertValid() const
{
	CView::AssertValid();
}

void Cklk2019View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Cklk2019Doc* Cklk2019View::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Cklk2019Doc)));
	return (Cklk2019Doc*)m_pDocument;
}
#endif //_DEBUG


// Cklk2019View message handlers




void Cklk2019View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'Q':
		rotateArm1 -= 10;
		if (rotateArm1 < -270) rotateArm1 = -270;
		Invalidate();
		break;
	case 'A':
		rotateArm1 += 10;
		if (rotateArm1 >= 0) rotateArm1 = 0;
		Invalidate();
		break;
	case 'E':
		rotateLeg1 += 10;
		if (rotateLeg1 > 0) rotateLeg1 = 0;
		Invalidate();
		break;
	case 'D':
		rotateLeg1 -= 10;
		if (rotateLeg1 < -100) rotateLeg1 = -100;
		Invalidate();
		break;
	case 'T':
		rotateArm2 += 10;
		if (rotateArm2 >= 180) rotateArm2 = 180;
		Invalidate();
		break;
	case 'G':
		rotateArm2 -= 10;
		if (rotateArm2 < -90) rotateArm2 = -90;
		Invalidate();
		break;
	case 'R':
		rotateLeg2 += 10;
		if (rotateLeg2 > 100) rotateLeg2 = 100;
		Invalidate();
		break;
	case 'F':
		rotateLeg2 -= 10;
		if (rotateLeg2 < 0) rotateLeg2 = 0;
		Invalidate();
		break;
	case 'W':
		rotateBody += 5;
		if (rotateBody > 0) rotateBody = 0;
		Invalidate();
		break;
	case 'S':
		rotateBody -= 5;
		if (rotateBody < -90) rotateBody = -90;
		Invalidate();
		break;

	case VK_RIGHT:
		rotateArm1 += 12;
		if (rotateArm1 >= 0) rotateArm1 = 0;
		rotateArm2 += 15;
		if (rotateArm2 >= 180) rotateArm2 = 180;
		rotateBody -= 1;
		if (rotateBody < 0) rotateBody = 0;
		rotateLeg1 += 5;
		if (rotateLeg1 > 0) rotateLeg1 = 0;
		rotateLeg2 -= 5;
		if (rotateLeg2 < 0) rotateLeg2 = 0;
		Invalidate();
		break;
	case VK_UP:
		moveY -= 5;
		Invalidate();
		break;
	case VK_DOWN:
		moveY += 5;
		Invalidate();
		break;
	case VK_LEFT:
		rotateArm1 -= 12;
		if (rotateArm1 < -270) rotateArm1 = -270;
		rotateArm2 -= 15;
		if (rotateArm2 < -90) rotateArm2 = -90;
		rotateBody += 1;
		if (rotateBody > 20) rotateBody = 20;
		rotateLeg1 -= 5;
		if (rotateLeg1 < -100) rotateLeg1 = -100;
		rotateLeg2 += 5;
		if (rotateLeg2 > 100) rotateLeg2 = 100;
		Invalidate();
		break;
	default:
		break;
	}
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL Cklk2019View::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

	return false;
}
