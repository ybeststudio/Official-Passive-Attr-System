// Add to includes:
#if defined(ENABLE_PASSIVE_ATTR)
#	include "PythonPassiveAttr.h"
#endif

// Find this line:
initMount();

// Add after it:
#if defined(ENABLE_PASSIVE_ATTR)
	initpassiveAttr();
#endif

// Before
	if (LocaleService_LoadGlobal(hInstance))
		SetDefaultCodePage(LocaleService_GetCodePage());

	CPythonApplication* app = new CPythonApplication;
	app->Initialize(hInstance);

	bool ret = false;
	{
		CPythonLauncher pyLauncher;
		CPythonExceptionSender pyExceptionSender;
		SetExceptionSender(&pyExceptionSender);

		if (pyLauncher.Create())
		{
			ret = RunMainScript(pyLauncher, lpCmdLine); // ???? ??????? ????? ?????? ?????.
		}

		//ProcessScanner_ReleaseQuitEvent();

		// ???? ?????.
		app->Clear();

#if defined(ENABLE_MOUNT_UPGRADE_SYSTEM)
		CPythonMountUpGrade::Instance().ClearHandler();
#endif

		timeEndPeriod(1);
		pyLauncher.Clear();
	}

// After
	if (LocaleService_LoadGlobal(hInstance))
		SetDefaultCodePage(LocaleService_GetCodePage());

	CPythonApplication* app = new CPythonApplication;
	app->Initialize(hInstance);

	bool ret = false;
	{
		CPythonLauncher pyLauncher;
		CPythonExceptionSender pyExceptionSender;
		SetExceptionSender(&pyExceptionSender);

		if (pyLauncher.Create())
		{
			ret = RunMainScript(pyLauncher, lpCmdLine); // ???? ??????? ????? ?????? ?????.
		}

		//ProcessScanner_ReleaseQuitEvent();

		// ???? ?????.
		app->Clear();

#if defined(ENABLE_PASSIVE_ATTR)
		CPythonPassiveAttr::Instance().ClearHandler();
#endif
#if defined(ENABLE_MOUNT_UPGRADE_SYSTEM)
		CPythonMountUpGrade::Instance().ClearHandler();
#endif

		timeEndPeriod(1);
		pyLauncher.Clear();
	}
