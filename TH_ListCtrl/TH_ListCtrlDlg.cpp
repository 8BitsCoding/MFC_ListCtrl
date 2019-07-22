
// TH_ListCtrlDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "TH_ListCtrl.h"
#include "TH_ListCtrlDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTHListCtrlDlg 대화 상자



CTHListCtrlDlg::CTHListCtrlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TH_LISTCTRL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTHListCtrlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA_LIST, m_data_list);
}

BEGIN_MESSAGE_MAP(CTHListCtrlDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_ADD_BTN, &CTHListCtrlDlg::OnBnClickedAddBtn)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_DATA_LIST, &CTHListCtrlDlg::OnLvnItemchangedDataList)
	ON_BN_CLICKED(IDC_MODIFY_BTN, &CTHListCtrlDlg::OnBnClickedModifyBtn)
END_MESSAGE_MAP()


// CTHListCtrlDlg 메시지 처리기

BOOL CTHListCtrlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	// 1. 리소스 뷰에서 다이얼로그를 모두 그린다.
	// 2. 리스트 컨트롤의 헤더 컨트롤을 추가
	LV_COLUMN add_column;

	add_column.mask = LVCF_TEXT | LVCF_WIDTH;		// LV_COLUMN 구조체에서 어떠한 정보만 쓰겠다고 명시
	
	add_column.pszText = L"이름";
	add_column.cx = 80;				// 폭
	m_data_list.InsertColumn(0, &add_column);

	add_column.pszText = L"전화번호";
	add_column.cx = 100;				// 폭
	m_data_list.InsertColumn(1, &add_column);

	add_column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_FMT;		// 정렬기능을 추가해보자.
	add_column.fmt = LVCFMT_CENTER;
	add_column.pszText = L"주소";
	add_column.cx = 200;				// 폭
	m_data_list.InsertColumn(2, &add_column);

	m_data_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);			// 전체 ROW를 선택할 수 있도록 옵션변경


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTHListCtrlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTHListCtrlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CTHListCtrlDlg::OnBnClickedAddBtn()
{
	// 3. 리스트 컨트롤에 아이템 추가
	CString str;
	GetDlgItemText(IDC_NAME_EDIT, str);

	int index = m_data_list.GetItemCount();
	LV_ITEM add_item;
	add_item.iItem = index;			// 0번째 아이템

	add_item.mask = LVIF_TEXT;		// Text 항목만 쓰겠다.
	add_item.pszText = (wchar_t *)(const wchar_t *)str;
	add_item.iSubItem = 0;		// 0번째 Column

	m_data_list.InsertItem(&add_item);


	GetDlgItemText(IDC_PHONE_EDIT, str);
	add_item.pszText = (wchar_t *)(const wchar_t *)str;
	add_item.iSubItem = 1;		// 1번째 Column

	m_data_list.SetItem(&add_item);

	GetDlgItemText(IDC_ADD_EDIT, str);
	//add_item.pszText = (wchar_t *)(const wchar_t *)str;
	//add_item.iSubItem = 2;		// 1번째 Column
	m_data_list.SetItemText(index, 2, str);		// 이런 방식도 가능
}


void CTHListCtrlDlg::OnLvnItemchangedDataList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	// 4. 리스트 컨트롤을 움직일때
	if (pNMLV->uNewState & LVIS_SELECTED)
	{
		// 새로운 항목을 선택
		SetDlgItemText(IDC_NAME_EDIT, m_data_list.GetItemText(pNMLV->iItem, 0));
		SetDlgItemText(IDC_PHONE_EDIT, m_data_list.GetItemText(pNMLV->iItem, 1));
		SetDlgItemText(IDC_ADD_EDIT, m_data_list.GetItemText(pNMLV->iItem, 2));
	}

	*pResult = 0;
}


void CTHListCtrlDlg::OnBnClickedModifyBtn()
{
	// 5. 선택된 리스트 컨트롤 데이터 변경
	POSITION pos = m_data_list.GetFirstSelectedItemPosition();
	if (pos == NULL)	// 어떠한 항목도 선택안함
	{

	}
	else {
		int index;
		while (pos != NULL) {
			index = m_data_list.GetNextSelectedItem(pos);
		}

		CString str;
		GetDlgItemText(IDC_NAME_EDIT, str);
		m_data_list.SetItemText(index, 0, str);

		GetDlgItemText(IDC_PHONE_EDIT, str);
		m_data_list.SetItemText(index, 1, str);

		GetDlgItemText(IDC_ADD_EDIT, str);
		m_data_list.SetItemText(index, 2, str);
	}
}
