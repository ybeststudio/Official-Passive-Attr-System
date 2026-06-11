// Add to includes:
#if defined(ENABLE_PASSIVE_ATTR)
#	include "PythonPassiveAttr.h"
#endif

// In `netSendAttr67ClosePacket(PyObject* poSelf, PyObject* poArgs)`, find this block:
{
	CPythonNetworkStream& rkNetStream = CPythonNetworkStream::Instance();
	rkNetStream.SendAttr67ClosePacket();
	return Py_BuildNone();
}

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
static bool netPassiveAttrGetMaterialPos(PyObject* poArgs, int iStartIndex, WORD awMaterialPos[4])
{
	for (int i = 0; i < 4; ++i)
	{
		int iPos = -1;
		if (!PyTuple_GetInteger(poArgs, iStartIndex + i, &iPos))
			return false;

		if (iPos < 0)
			return false;

		awMaterialPos[i] = static_cast<WORD>(iPos);
	}
	return true;
}

PyObject* netSendPassiveAttrOpen(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream::Instance().SendPassiveAttrPacket(
		CPythonPassiveAttr::SUBHEADER_CG_PASSIVE_ATTR_OPEN, 0, nullptr, 0xFFFF);
	return Py_BuildNone();
}

PyObject* netSendPassiveAttrClose(PyObject* poSelf, PyObject* poArgs)
{
	CPythonNetworkStream::Instance().SendPassiveAttrPacket(
		CPythonPassiveAttr::SUBHEADER_CG_PASSIVE_ATTR_CLOSE, 0, nullptr, 0xFFFF);
	return Py_BuildNone();
}

PyObject* netSendPassiveAttrChangePage(PyObject* poSelf, PyObject* poArgs)
{
	int iPage = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iPage))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendPassiveAttrPacket(
		CPythonPassiveAttr::SUBHEADER_CG_PASSIVE_ATTR_CHANGE_PAGE, static_cast<BYTE>(iPage), nullptr, 0xFFFF);
	return Py_BuildNone();
}

PyObject* netSendPassiveAttrAdd(PyObject* poSelf, PyObject* poArgs)
{
	int iPage = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iPage))
		return Py_BuildException();

	WORD awMaterialPos[4] = {};
	if (!netPassiveAttrGetMaterialPos(poArgs, 1, awMaterialPos))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendPassiveAttrPacket(
		CPythonPassiveAttr::SUBHEADER_CG_PASSIVE_ATTR_ADD, static_cast<BYTE>(iPage), awMaterialPos, 0xFFFF);
	return Py_BuildNone();
}

PyObject* netSendPassiveAttrCharge(PyObject* poSelf, PyObject* poArgs)
{
	int iPage = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iPage))
		return Py_BuildException();

	WORD awMaterialPos[4] = {};
	if (!netPassiveAttrGetMaterialPos(poArgs, 1, awMaterialPos))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendPassiveAttrPacket(
		CPythonPassiveAttr::SUBHEADER_CG_PASSIVE_ATTR_CHARGE, static_cast<BYTE>(iPage), awMaterialPos, 0xFFFF);
	return Py_BuildNone();
}

PyObject* netSendPassiveAttrActivateDeactivate(PyObject* poSelf, PyObject* poArgs)
{
	int iPage = 0;
	if (!PyTuple_GetInteger(poArgs, 0, &iPage))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendPassiveAttrPacket(
		CPythonPassiveAttr::SUBHEADER_CG_PASSIVE_ATTR_ACTIVATE_DEACTIVATE, static_cast<BYTE>(iPage), nullptr, 0xFFFF);
	return Py_BuildNone();
}

PyObject* netSendPassiveAttrUseItemJob(PyObject* poSelf, PyObject* poArgs)
{
	int iPage = 0;
	int iJobItemPos = 0xFFFF;
	if (!PyTuple_GetInteger(poArgs, 0, &iPage))
		return Py_BuildException();
	if (!PyTuple_GetInteger(poArgs, 1, &iJobItemPos))
		return Py_BuildException();

	CPythonNetworkStream::Instance().SendPassiveAttrPacket(
		CPythonPassiveAttr::SUBHEADER_CG_PASSIVE_ATTR_USE_ITEM_JOB, static_cast<BYTE>(iPage), nullptr, static_cast<WORD>(iJobItemPos));
	return Py_BuildNone();
}
#endif

// Find this line:
{ "SendAttr67ClosePacket", netSendAttr67ClosePacket, METH_VARARGS },

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
		{ "SendPassiveAttrOpen", netSendPassiveAttrOpen, METH_VARARGS },
		{ "SendPassiveAttrClose", netSendPassiveAttrClose, METH_VARARGS },
		{ "SendPassiveAttrChangePage", netSendPassiveAttrChangePage, METH_VARARGS },
		{ "SendPassiveAttrAdd", netSendPassiveAttrAdd, METH_VARARGS },
		{ "SendPassiveAttrCharge", netSendPassiveAttrCharge, METH_VARARGS },
		{ "SendPassiveAttrActivateDeactivate", netSendPassiveAttrActivateDeactivate, METH_VARARGS },
		{ "SendPassiveAttrUseItemJob", netSendPassiveAttrUseItemJob, METH_VARARGS },
#endif
