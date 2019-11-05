// Serialization.cpp : Определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include <string>
#include "serialization.h"
#include "classes.h"
//
//// главный заголовочный файл
//#include "ess_stream.h"
//// этот заголовок для хранилища XML
//#include "ess_xml.h"
//
//
//
//void Serialization(C0 c0, C1 c1) {
//	int version = 1;
//	std::string xml_root = "root";
//	std::string instance_name = "x";
//
//	try
//	{
//		// регистрируется класс
//		ess::Registry registry;
//		// разновидность с макросом для краткости
//		registry << ESS_REGISTER(C0, C0);
//
//		// где хранятся данные...
//		ess::xml_medium storage;
//		{
//			// экземпляр для сериализации
//			C0 c0;
//			C1 c1;
//
//			ess::xml_storing_adapter adapter(the_storage, xml_root, version);
//			// сохранить корневой C0
//			ess::stream(adapter, c0, "c0");
//			// сохранить производный C1
//			ess::stream(adapter, c1, "c1");
//		}
//	}
//}
//
//void Deserialization()
//{
//	C0* p0 = 0;
//	// восстановить из хранилища XML
//	Chordia::xml_source xmls(storage.c_str(), storage.size());
//	// и адаптер
//	ess::xml_loading_adapter adapter(xmls, xml_root, version);
//	// направить в указатель C0
//	ess::stream(adapter, p0, instance_name);
//	// теперь p0 готов к использованию
//	delete p0;
//}
//
