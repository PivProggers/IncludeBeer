#pragma once
//// главный заголовочный файл
//#include "ess_stream.h"
//// этот заголовок для хранилища XML
//#include "ess_xml.h"
//class C0
//{
//	short m_id;
//	// вектор указателей на C0
//	std::vector<C0*> m_children;
//	// функция сериализации 
//	virtual void serialize(ess::archive_adapter& adapter)
//	{
//		ESS_STREAM(adapter, m_id);
//		ESS_STREAM(adapter, m_children);
//	}
//public:
//	// задается корень наследования
//	ESS_ROOT(C0)
//		// устанавливается RTTI
//		ESS_RTTI(C0, C0)
//};
//
//class C1 : public C0
//{
//	// для примера – настоящий класс,
//	// вероятно, имел бы больше кода
//	ESS_RTTI(C1, C0)
//};