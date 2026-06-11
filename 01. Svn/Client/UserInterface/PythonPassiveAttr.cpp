#if defined(ENABLE_PASSIVE_ATTR)

#include "PythonPassiveAttr.h"

CPythonPassiveAttr::CPythonPassiveAttr()
	: m_poHandler(nullptr)
{
}

CPythonPassiveAttr::~CPythonPassiveAttr()
{
	ClearHandler();
}

void CPythonPassiveAttr::SetHandler(PyObject* poHandler)
{
	Py_XINCREF(poHandler);
	Py_CLEAR(m_poHandler);
	m_poHandler = poHandler;
}

void CPythonPassiveAttr::ClearHandler()
{
	if (!m_poHandler)
		return;

	if (Py_IsInitialized())
		Py_CLEAR(m_poHandler);
	else
		m_poHandler = nullptr;
}

PyObject* CPythonPassiveAttr::GetHandler() const
{
	return m_poHandler;
}

void CPythonPassiveAttr::Process(BYTE bSubHeader, BYTE bPage, BYTE bResult, BYTE bActive, DWORD dwRemainSec, BYTE bHasRelic)
{
	if (!m_poHandler || !Py_IsInitialized())
		return;

	PyCallClassMemberFunc(m_poHandler, "PassiveAttrProcess", Py_BuildValue("(iiiiii)", bSubHeader, bPage, bResult, bActive, dwRemainSec, bHasRelic));
}

PyObject* passiveAttrSetHandler(PyObject* poSelf, PyObject* poArgs)
{
	PyObject* poHandler = nullptr;
	if (!PyTuple_GetObject(poArgs, 0, &poHandler))
		return Py_BadArgument();

	CPythonPassiveAttr::Instance().SetHandler(poHandler);
	return Py_BuildNone();
}

void initpassiveAttr()
{
	static PyMethodDef s_methods[] =
	{
		{ "SetHandler", passiveAttrSetHandler, METH_VARARGS },
		{ nullptr, nullptr, 0 },
	};

	PyObject* poModule = Py_InitModule("passiveAttr", s_methods);

	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SUBHEADER_GC_OPEN", CPythonPassiveAttr::SUBHEADER_GC_PASSIVE_ATTR_OPEN);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SUBHEADER_GC_CHANGE_WINDOW", CPythonPassiveAttr::SUBHEADER_GC_PASSIVE_ATTR_CHANGE_WINDOW);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SUBHEADER_GC_CHARGE", CPythonPassiveAttr::SUBHEADER_GC_PASSIVE_ATTR_CHARGE);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SUBHEADER_GC_ADD", CPythonPassiveAttr::SUBHEADER_GC_PASSIVE_ATTR_ADD);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SUBHEADER_GC_ACTIVATE_DEACTIVATE", CPythonPassiveAttr::SUBHEADER_GC_PASSIVE_ATTR_ACTIVATE_DEACTIVATE);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SUBHEADER_GC_USE_ITEM_JOB", CPythonPassiveAttr::SUBHEADER_GC_PASSIVE_ATTR_USE_ITEM_JOB);

	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_GC_OK", CPythonPassiveAttr::PASSIVE_ATTR_GC_OK);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_GC_FAIL", CPythonPassiveAttr::PASSIVE_ATTR_GC_FAIL);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_GC_FAIL_MAX_ATTR", CPythonPassiveAttr::PASSIVE_ATTR_GC_FAIL_MAX_ATTR);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_JOB_ITEM", CPythonPassiveAttr::PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_JOB_ITEM);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_GC_FAIL_FULL_TIME", CPythonPassiveAttr::PASSIVE_ATTR_GC_FAIL_FULL_TIME);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_SUB_ITEM", CPythonPassiveAttr::PASSIVE_ATTR_GC_FAIL_NOT_ENOUGH_SUB_ITEM);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_GC_FAIL_COOLTIME", CPythonPassiveAttr::PASSIVE_ATTR_GC_FAIL_COOLTIME);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_GC_FAIL_CONTROL", CPythonPassiveAttr::PASSIVE_ATTR_GC_FAIL_CONTROL);

	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_PAGE_UP", CPythonPassiveAttr::PASSIVE_ATTR_PAGE_UP);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_PAGE_DOWN", CPythonPassiveAttr::PASSIVE_ATTR_PAGE_DOWN);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_OPEN_UP_SLOT", CPythonPassiveAttr::PASSIVE_ATTR_PAGE_UP);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_OPEN_DOWN_SLOT", CPythonPassiveAttr::PASSIVE_ATTR_PAGE_DOWN);

	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SLOT_INDEX_JOB", CPythonPassiveAttr::PASSIVE_ATTR_SLOT_INDEX_JOB);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SLOT_INDEX_WEAPON", CPythonPassiveAttr::PASSIVE_ATTR_SLOT_INDEX_WEAPON);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SLOT_INDEX_ELEMENT", CPythonPassiveAttr::PASSIVE_ATTR_SLOT_INDEX_ELEMENT);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SLOT_INDEX_ARMOR", CPythonPassiveAttr::PASSIVE_ATTR_SLOT_INDEX_ARMOR);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SLOT_INDEX_ACCE", CPythonPassiveAttr::PASSIVE_ATTR_SLOT_INDEX_ACCE);
	PyModule_AddIntConstant(poModule, "PASSIVE_ATTR_SLOT_INDEX_MAX", CPythonPassiveAttr::PASSIVE_ATTR_SLOT_INDEX_MAX);
}

#endif
