#pragma once
//// ������� ������������ ����
//#include "ess_stream.h"
//// ���� ��������� ��� ��������� XML
//#include "ess_xml.h"
//class C0
//{
//	short m_id;
//	// ������ ���������� �� C0
//	std::vector<C0*> m_children;
//	// ������� ������������ 
//	virtual void serialize(ess::archive_adapter& adapter)
//	{
//		ESS_STREAM(adapter, m_id);
//		ESS_STREAM(adapter, m_children);
//	}
//public:
//	// �������� ������ ������������
//	ESS_ROOT(C0)
//		// ��������������� RTTI
//		ESS_RTTI(C0, C0)
//};
//
//class C1 : public C0
//{
//	// ��� ������� � ��������� �����,
//	// ��������, ���� �� ������ ����
//	ESS_RTTI(C1, C0)
//};